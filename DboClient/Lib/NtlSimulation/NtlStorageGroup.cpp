// Precompile
#include "precomp_ntlsimulation.h"
#include "NtlStorageGroup.h"

// Storage
#include "NtlStorageMTContainer.h"
#include "NtlStorageTextSerializer.h"
#include "NtlStorageBinarySerializer.h"

//
#include "NtlTokenizer.h"

#include "NtlDebug.h"

CNtlStorageGroup::CNtlStorageGroup()
{

}

CNtlStorageGroup::~CNtlStorageGroup()
{

}

bool CNtlStorageGroup::Create()
{
	return true;
}

void CNtlStorageGroup::Destroy()
{
	for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
	{
		if( pair.second )
		{
			pair.second->Destroy();

			NTL_DELETE( pair.second );
		}
	}

	m_mapStorageUnit.clear();
}

/**
* \brief Conceptual Base load function of NtlStorageGroup
*If there is a Group that requires specific loading, inherit this Class
*Use by overriding in a child class.
* \param pS (CNtlStorageSerializer*) Serializer buffer to be used for Load.
* \return Success or not
*/
bool CNtlStorageGroup::Load(CNtlStorageSerializer* pS)
{
	if( eNTL_STORAGE_SERIALIZER_TEXT == pS->GetStorageSerializerType() )
	{
		CNtlTokenizer lexer( pS->GetData() );
		if( !lexer.IsSuccess() )
			return false;

		// Text Parser

		// [Category]
		// [KEY] = [VALUE];
		eNTL_STORAGE_TYPE eCurrentType = eNTL_STORAGE_INVALID;
		
		enum eTextState
		{
			eTEXT_KEY = 0,
			eTEXT_EQUAL,
			eTEXT_VALUE,
			eTEXT_SEMICOLON
		};

		eTextState eState = eTEXT_KEY;
		std::string strKey, strValue;
		for(;;)
		{
			std::string& token = lexer.PeekNextToken();

			// Check if the current token is a category and if it is a category then the next value is key -> = -> value -> ; Check the data with
			for( int i=0; i < eNTL_STORAGE_NUMS; ++i )
			{
				if( token.compare( g_acNtlStorageTypeString[i] ) == 0 )
				{
					eState = eTEXT_KEY;
					eCurrentType = (eNTL_STORAGE_TYPE)i;
					token = lexer.PeekNextToken();
					break;
				}
			}

			if( token.empty() )
				break;

			switch( eState )
			{
			case eTEXT_KEY:
				{
					if( token == "=" ||
						token == ";" )
					{
						return false;
					}
					else
					{
						strKey = token;
						eState = eTEXT_EQUAL;
					}
				}
				break;
			case eTEXT_EQUAL:
				{
					if( token != "=" )
					{
						return false;
					}
					else
					{
						eState = eTEXT_VALUE;
					}
				}
				break;
			case eTEXT_VALUE:
				{
					if( token == "=" ||
						token == ";" )
					{
						return false;
					}
					else
					{
						strValue = token;
						eState = eTEXT_SEMICOLON;
					}
				}
				break;
			case eTEXT_SEMICOLON:
				{
					if( token != ";" )
					{
						return false;
					}
					else
					{
						// If eTEXT_SEMICOLON is confirmed, change the stored Key and Value according to the current Current Type.
						// Put it in the unit. If the Unit is not included in the current Group of eCurrentType
						// An error occurs and return. In that case, except for the Key Value saved so far,
						// All items remain as default.
						UNITMAP::iterator it = m_mapStorageUnit.find( eCurrentType );
						if( it == m_mapStorageUnit.end() )
							return false;

						// If the currently entered KEY value does not exist in the MappingTable, the current line is discarded. That would be wrong
						// Only the KEY value items are not saved; the rest is processed normally.
						unsigned int uiKey = 0;
						if( !GetNtlStorageMTContainer()->GetKeyFromString( strKey, uiKey ) )
						{
							eState = eTEXT_KEY;
							continue;
						}
						else
						{
							(*it).second->SetData( uiKey, strValue );
						}

						eState = eTEXT_KEY;
					}
				}
				break;
			default:
				break;
			}
		}

		return true;
	}
	else if( eNTL_STORAGE_SERIALIZER_BINARY == pS->GetStorageSerializerType() )
	{
		// Binary must obtain the appropriate key for each.
		/*CNtlStorageBinarySerializer*pBianrySerializer = (CNtlStorageBinarySerializer*)pS;*/
		unsigned int uiKey;
		eNTL_STORAGE_TYPE eCurrentType = eNTL_STORAGE_INVALID;

		int nCount = 0;
		RwBool bLoaded = FALSE;
		while(!bLoaded)
		{
			// Infinite loop prevention
			if( ++nCount > 1000000 )
				break;

			if( pS->IsEmpty() )
				break;

			(*pS) >> uiKey;
			switch( uiKey )
			{
				// Case By Case
			case dSTORAGE_CATEGORY:
				{
					// Create a unit appropriate for the category.
					std::string strCategory;
					(*pS) >> strCategory;

					// Check what category it is
					eNTL_STORAGE_TYPE eType = eNTL_STORAGE_INVALID;;
					for( int i=0; i < (int)eNTL_STORAGE_NUMS; ++i )
					{
						if( strCategory.compare( g_acNtlStorageTypeString[i] ) == 0 )
						{
							eType = (eNTL_STORAGE_TYPE)i;

							break;
						}
					}

					if( eType == eNTL_STORAGE_INVALID )
					{
						// If you cannot find a defined category, the data is incorrect.
						return false;
					}

					// If it does not, overwrite it if you are making it.
					UNITMAP::iterator it = m_mapStorageUnit.find( eType );
					if( it == m_mapStorageUnit.end() )
					{
						CNtlStorageUnit* pUnit = NTL_NEW CNtlStorageUnit();
						if( !pUnit->Create( eType ) )
						{
							return false;
						}
					}

					eCurrentType = eType;					
				}
				break;
			case dSTORAGE_EOF:
				{
					// Loading completed normally
					bLoaded = TRUE;
				}
				break;
			default:
				{
					// Everything that is not a category is data based on uiKey.
					// Get the current current type
					UNITMAP::iterator it = m_mapStorageUnit.find( eCurrentType );
					if( it == m_mapStorageUnit.end() )
					{
						return false;
					}

					std::string strData;
					(*pS) >> strData;
					(*it).second->SetData( uiKey, strData );
				}
				break;

			}
		}

		// There is no need to input data again into ByNarySerializer.
		/*for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		{
			if( pair.second )
			{
				(*pBianrySerializer) << (unsigned int)dSTORAGE_CATEGORY;
				(*pBianrySerializer) << (std::string) g_acNtlStorageTypeString[pair.second->GetType()];

				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pBianrySerializer) << (std::string)GetNtlStorageMTContainer()->GetString( pairValue.first );
					(*pBianrySerializer) << (std::string)pairValue.second;
				}			
			}
		}
		*/

		return true;
	}

	return false;
}

/**
* \brief Conceptual Base save function of NtlStorageGroup
*If there is a Group that needs to be specially saved, inherit this Class.
*Use by overriding in a child class.
* \param pS (CNtlStorageSerializer*) Serializer buffer to be used for save.
* \return Success or not
*/
bool CNtlStorageGroup::Save(CNtlStorageSerializer* pS)
{
	// Storage
	if( eNTL_STORAGE_SERIALIZER_TEXT == pS->GetStorageSerializerType() )
	{
		//CNtlStorageTextSerializer pSerializer = (CNtlStorageTextSerializer)s;

		CNtlStorageTextSerializer* pTextSerializer = (CNtlStorageTextSerializer*)pS;

		for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		{
			(*pTextSerializer) << g_acNtlStorageTypeString[pair.second->GetType()];
			(*pTextSerializer) << "\n";

			if( pair.second )
			{
				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pTextSerializer) << (std::string)GetNtlStorageMTContainer()->GetString( pairValue.first );
					(*pTextSerializer) << " = ";
					(*pTextSerializer) << (std::string)pairValue.second;	// Data ( std::string )
					(*pTextSerializer) << ";\n";
				}			
			}

			(*pTextSerializer) << "\0";
		}

		return true;
	}
	else if( eNTL_STORAGE_SERIALIZER_BINARY == pS->GetStorageSerializerType() )
	{
		CNtlStorageBinarySerializer* pBianrySerializer = (CNtlStorageBinarySerializer*)pS;

		for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		{
			if( pair.second )
			{
				(*pBianrySerializer) << (unsigned int)dSTORAGE_CATEGORY;
				(*pBianrySerializer) << (std::string) g_acNtlStorageTypeString[pair.second->GetType()];

				for each( std::pair< unsigned int, std::string > pairValue in pair.second->m_mapStorageData )
				{
					(*pBianrySerializer) << (unsigned int)pairValue.first;
					(*pBianrySerializer) << (std::string)pairValue.second;
				}			
			}
		}

		(*pBianrySerializer) << (unsigned int)dSTORAGE_EOF;

		return true;
	}

	return false;
}

bool CNtlStorageGroup::Reset()
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->Reset() )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, int nData)
{
	std::ostringstream outstream;
	outstream << nData;
	std::string strData = outstream.str();

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, float fData)
{
	std::ostringstream outstream;
	outstream << fData;
	std::string strData = outstream.str();

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, std::string strData)
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData(unsigned int uiKey, bool bData)
{
	std::string strData;
	if( bData )
		strData = dSTORAGE_TRUE;
	else
		strData = dSTORAGE_FALSE;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::SetData( unsigned int uiKey, const char* acData )
{
	std::string strData( acData );
	
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->SetData( uiKey, strData ) )
			return true;

	return false;
}

bool CNtlStorageGroup::GetIntData(unsigned int uiKey, int& nData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			nData = atoi(strOut.c_str());
			return true;
		}
	

	return false;
}

bool CNtlStorageGroup::GetStringData(unsigned int uiKey, std::string& strData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			strData = strOut.c_str();
			return true;
		}

	return false;
}


bool CNtlStorageGroup::GetBoolData(unsigned int uiKey, bool& bData)
{
	std::string strOut;

	for each(std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit)
	{
		if (pair.second->GetData(uiKey, strOut))
		{
			if (strOut.compare(dSTORAGE_TRUE) == 0)
				bData = true;
			else
				bData = false;

			return true;
		}
	}

	return false;
}

bool CNtlStorageGroup::GetFloatData(unsigned int uiKey, float& fData)
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			fData = (float)atof( strOut.c_str() );
			return true;
		}

	return false;
}


void CNtlStorageGroup::UnitCheck()
{
	for each( std::pair< eNTL_STORAGE_TYPE, CNtlStorageUnit* > pair in m_mapStorageUnit )
		pair.second->UnitCheck();
}

void CNtlStorageGroup::SetStorageGroupType( eNTL_STORAGE_GROUP_TYPE eType )
{
	m_eGroupType = eType;
}

eNTL_STORAGE_GROUP_TYPE CNtlStorageGroup::GetStorageGroupType()
{
	return m_eGroupType;
}

int CNtlStorageGroup::GetIntData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return atoi(strOut.c_str());
		}

	return 0;
}

bool CNtlStorageGroup::GetBoolData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			if( strOut.compare( dSTORAGE_TRUE ) == 0 )
				return true;
			else
				return false;
		}

	return false;
}

float CNtlStorageGroup::GetFloatData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return (float)atof( strOut.c_str() );
		}

	return 0.0f;
}

std::string CNtlStorageGroup::GetStringData( unsigned int uiKey )
{
	std::string strOut;

	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		if( pair.second->GetData( uiKey, strOut ) )
		{
			return strOut;
		}

	return strOut;
}

bool CNtlStorageGroup::IsHave( eNTL_STORAGE_TYPE eType )
{
	UNITMAP::iterator it = m_mapStorageUnit.find( eType );
	if( it == m_mapStorageUnit.end() )
	{
		return false;
	}

	return true;
}

CNtlStorageUnit* CNtlStorageGroup::GetStorageUnit( eNTL_STORAGE_TYPE eType )
{
	UNITMAP::iterator it = m_mapStorageUnit.find( eType );
	if( it == m_mapStorageUnit.end() )
	{
		return NULL;
	}

	return (*it).second;
}

bool CNtlStorageGroup::SetUnitApplyFunc( NtlStorageUnitApplyFunc func )
{
	for each( std::pair<eNTL_STORAGE_TYPE, CNtlStorageUnit*> pair in m_mapStorageUnit )
		pair.second->SetApplyFunc( func );

	return true;
}