//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CNtlStorageUnit.cpp
//  @ Date : 2009-01-22
//  @ Author : 
//
//

#include "precomp_ntlsimulation.h"
#include "NtlStorageUnit.h"
#include "NtlSerializer.h"
#include "NtlStorageMTContainer.h"

// MappingTable default value getter
#define INSERT_DEFAULT_VALUE( mapname, key ) \
	mapname[key] = GetNtlStorageMTContainer()->GetDefaultString( key );

#define CHANGE_RESET_VALUE( key ) \
	SetData( key, GetNtlStorageMTContainer()->GetDefaultString( key ) );

CNtlStorageUnit::CNtlStorageUnit()
: m_unitApplyFunc(NULL)
{

}

CNtlStorageUnit::~CNtlStorageUnit()
{

}

/**
* \brief Create a Storage Unit. Receive the Type and Serializer and create them appropriately.
*/
bool CNtlStorageUnit::Create( eNTL_STORAGE_TYPE eType )
{
	m_eStorageType = eType;

	switch( eType )
	{
	case eNTL_STORAGE_ACCOUNT:
		
		break;
	case eNTL_STORAGE_GRAPHIC:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_GAMMA);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_WINDOW_MODE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_PRESET);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_TERRAIN_RANGE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_OBJECT_RANGE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_TERRAIN_SHADOW);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_WATER_EFFECT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_CHARACTER_RANGE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_CHARACTER_EFFECT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_CHARACTER_SHADOW);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_MAGIC_EFFECT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_WEATHER_EFFECT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_TEXTURE_LEVEL);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_SHADER_HDR);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GRAPHIC_CHARACTER_EDGE);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_GRAPHIC_SHOW_FPS);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_GRAPHIC_FPS);
		break;
	case eNTL_STORAGE_SOUND:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_SOUND_MAIN_VOLUME);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_SOUND_BACK_VOLUME);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_SOUND_EFFECT_VOLUME);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_SOUND_ENV_VOLUME);
		break;
	case eNTL_STORAGE_GAMEINFO:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GAMEINFO_GUILD_EMBLEM);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GAMEINFO_DOJO_MARK);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GAMEINFO_SCOUTER_VISIBLE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_GAMEINFO_ITEM_DROP);
		break;
	case eNTL_STORAGE_ETC:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_FRIEND_NOTIFY);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_GUILD_MEMBER_NOTIFY);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_QUESTTEXT_EFFECT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_USER_TRADE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_MOVEABLEWINDOW);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_MOUSE_DASH);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_ETC_MOUSE_FOV);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_ETC_MOUSE_VIEW_POINT);
		break;
	case eNTL_STORAGE_CHAT:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_INPUTMODE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_WINDOW_LOCK);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_MOUSE_SCROLL);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_WINDOW);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_WINDOW);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_WINDOW_OPACITY);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_NORMAL);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_WISPHER);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_BASIC_FIND_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_GUILD);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_TRADE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_SHOUT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_BASIC_SYSTEM);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_NORMAL);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_WISPHER);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND_FIND_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_GUILD);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_TRADE);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_SHOUT);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_CHAT_EXTEND_SYSTEM);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_NORMAL);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_WISPHER);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_FIND_PARTY);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_GUILD);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_TRADE);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_SHOUT);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHAT_EXTEND2_SYSTEM);
		break;
	case eNTL_STORAGE_SCOUTER:

		break;
	case eNTL_STORAGE_QUEST:

		break;
	case eNTL_STORAGE_CHARACTER:
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_EX_QUICKSLOT1);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_EX_QUICKSLOT2);
		INSERT_DEFAULT_VALUE(m_mapStorageData,dSTORAGE_ETC_QUICKSLOT_LOCK);
		break;
	case eNTL_STORAGE_CHARACTER_ETC:
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME);
		break;
	case eNTL_STORAGE_CHARACTER_SCOUTER:
	{
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_CHARACTER_SCOUTER_ON);
	}
	break;
	case eNTL_STORAGE_SYSTEM_ETC:
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_SYSTEM_ETC_DISCORD);
		INSERT_DEFAULT_VALUE(m_mapStorageData, dSTORAGE_SYSTEM_ETC_CONTRACT);
		break;

	default:
		break;
	}

	return true;
}

void CNtlStorageUnit::Destroy()
{
	m_mapStorageData.clear();
}

bool CNtlStorageUnit::GetData( unsigned int uiKey, std::string& strOut )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// I couldn't find the key.
		return false;
	}

	strOut = (*it).second;
	return true;
}

bool CNtlStorageUnit::SetData( unsigned int uiKey, std::string strData )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// If the key is not found, check whether the key exists in the mapping table container.
		eNTL_STORAGE_TYPE eType = GetNtlStorageMTContainer()->GetStorageType( uiKey );
		if( eType == eNTL_STORAGE_INVALID )
		{
			return false;
		}

		// Check whether the type of the current unit matches the type held by the key.
		if( eType != m_eStorageType )
		{
			return false;
		}

		// If they match, data appropriate to the Key value is created and entered.
		m_mapStorageData[uiKey] = strData;
		return true;
	}

	(*it).second = strData;
	return true;
}

eNTL_STORAGE_TYPE CNtlStorageUnit::GetType()
{
	return m_eStorageType;
}

void CNtlStorageUnit::UnitCheck()
{
	NtlStorageUnitCheckFunc func;
	for each( std::pair< unsigned int, std::string > pair in m_mapStorageData )
	{
		func = GetNtlStorageMTContainer()->GetUnitCheckFunc( pair.first );

		if( func )
			(*func)( pair.first );
	}
}

NtlStorageUnitApplyFunc CNtlStorageUnit::GetApplyFunc()
{
	return m_unitApplyFunc;
}

bool CNtlStorageUnit::SetApplyFunc( NtlStorageUnitApplyFunc func )
{
	m_unitApplyFunc = func;

	return true;
}

bool CNtlStorageUnit::Apply( unsigned int uiFlags )
{
	if( m_unitApplyFunc )
	{
		if( !(*m_unitApplyFunc)( this, uiFlags ) )
			return false;
	}

	return true;
}

bool CNtlStorageUnit::GetBoolData( unsigned int uiKey )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// I couldn't find the key.
		return false;
	}

	if( (*it).second.compare( dSTORAGE_TRUE ) == 0 )
		return true;
	else
		return false;
}

int CNtlStorageUnit::GetIntData( unsigned int uiKey )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// I couldn't find the key.
		return 0;
	}

	return atoi( (*it).second.c_str() );
}

float CNtlStorageUnit::GetFloatData( unsigned int uiKey )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// I couldn't find the key.
		return 0.0f;
	}

	return (float)atof( (*it).second.c_str() );
}

std::string CNtlStorageUnit::GetStringData( unsigned int uiKey )
{
	DATAMAP::iterator it = m_mapStorageData.find( uiKey );
	if( it == m_mapStorageData.end() )
	{
		// I couldn't find the key.
		return std::string("");
	}

	return (*it).second;
}

bool CNtlStorageUnit::Reset()
{
	switch( m_eStorageType )
	{
	case eNTL_STORAGE_ACCOUNT:
		return true;
	case eNTL_STORAGE_GRAPHIC:
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_GAMMA);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_WINDOW_MODE);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_PRESET);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_TERRAIN_RANGE);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_OBJECT_RANGE);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_TERRAIN_SHADOW);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_WATER_EFFECT);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_CHARACTER_RANGE);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_CHARACTER_EFFECT);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_CHARACTER_SHADOW);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_MAGIC_EFFECT);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_WEATHER_EFFECT);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_TEXTURE_LEVEL);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_SHADER_HDR);
		CHANGE_RESET_VALUE(dSTORAGE_GRAPHIC_CHARACTER_EDGE);
		return true;
	case eNTL_STORAGE_SOUND:
		CHANGE_RESET_VALUE(dSTORAGE_SOUND_MAIN_VOLUME);
		CHANGE_RESET_VALUE(dSTORAGE_SOUND_BACK_VOLUME);
		CHANGE_RESET_VALUE(dSTORAGE_SOUND_EFFECT_VOLUME);
		CHANGE_RESET_VALUE(dSTORAGE_SOUND_ENV_VOLUME);
		return true;
	case eNTL_STORAGE_GAMEINFO:
		CHANGE_RESET_VALUE(dSTORAGE_GAMEINFO_GUILD_EMBLEM);
		CHANGE_RESET_VALUE(dSTORAGE_GAMEINFO_DOJO_MARK);
		CHANGE_RESET_VALUE(dSTORAGE_GAMEINFO_SCOUTER_VISIBLE);
		CHANGE_RESET_VALUE(dSTORAGE_GAMEINFO_ITEM_DROP);
		return true;
	case eNTL_STORAGE_ETC:
		CHANGE_RESET_VALUE(dSTORAGE_ETC_FRIEND_NOTIFY);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_GUILD_MEMBER_NOTIFY);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_QUESTTEXT_EFFECT);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_USER_TRADE);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_MOVEABLEWINDOW);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_MOUSE_DASH);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_MOUSE_FOV);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_MOUSE_VIEW_POINT);
		return true;
	case eNTL_STORAGE_CHAT:
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_INPUTMODE);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_WINDOW_LOCK);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_MOUSE_SCROLL);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_WINDOW);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_WINDOW);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_WINDOW_OPACITY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_NORMAL);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_WISPHER);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_FIND_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_GUILD);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_TRADE);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_SHOUT);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_BASIC_SYSTEM);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_NORMAL);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_WISPHER);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_FIND_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_GUILD);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_TRADE);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_SHOUT);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND_SYSTEM);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_NORMAL);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_WISPHER);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_FIND_PARTY);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_GUILD);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_TRADE);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_SHOUT);
		CHANGE_RESET_VALUE(dSTORAGE_CHAT_EXTEND2_SYSTEM);
		return true;
	case eNTL_STORAGE_SCOUTER:

		break;
	case eNTL_STORAGE_QUEST:

		break;
	case eNTL_STORAGE_CHARACTER:
		CHANGE_RESET_VALUE(dSTORAGE_ETC_EX_QUICKSLOT1);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_EX_QUICKSLOT2);
		CHANGE_RESET_VALUE(dSTORAGE_ETC_QUICKSLOT_LOCK);
		return true;

	case eNTL_STORAGE_CHARACTER_ETC:
		CHANGE_RESET_VALUE(dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME);
		return true;

	case eNTL_STORAGE_CHARACTER_SCOUTER:
		CHANGE_RESET_VALUE(dSTORAGE_CHARACTER_SCOUTER_ON);
		return true;

	case eNTL_STORAGE_SYSTEM_ETC:
		CHANGE_RESET_VALUE(dSTORAGE_SYSTEM_ETC_DISCORD);
		CHANGE_RESET_VALUE(dSTORAGE_SYSTEM_ETC_CONTRACT);
		return true;

	default:
		break;
	}

	return false;
}