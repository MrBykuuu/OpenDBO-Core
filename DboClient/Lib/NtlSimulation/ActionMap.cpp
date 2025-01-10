#include "precomp_ntlsimulation.h"
#include "ActionMap.h"

// Framework
#include "NtlApplication.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CActionMap
//
//
/////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CActionMap::CActionMap( void )
{
	
}

/**
* \brief Copy Construction
*/
CActionMap::CActionMap( const CActionMap& rhs ) 
{
	this->m_mapAction = rhs.m_mapAction;
	this->m_mapKey = rhs.m_mapKey;
}

CActionMap::CActionMap( CActionMapManager* pManager ) 
: m_pManager(NULL)
{
	m_pManager = pManager;
}

/**
* \brief Destruction
*/
CActionMap::~CActionMap( void ) 
{
	Clear();
}

/**
*\breif Finds and returns Action by Key value.
* \param wKey (WORD) Created NTL Key
* \return ID of the ACTION found (ACTION_INVALID if there is no key)
*/
WORD CActionMap::FindAction( WORD wKey ) 
{
	ACTIONMAP::iterator it = m_mapAction.find( wKey );
	if( it == m_mapAction.end() )
		return ACTION_INVALID;

	return it->second;
}

/**
* \brief Finds and returns the Key as the Action value.
* \param wAction (WORD) ACTION ID
* \return (WORD) KEY DATA
*/
WORD CActionMap::FindKey( WORD wAction ) 
{
	ACTIONMAP::iterator it = m_mapKey.find( wAction );
	if( it == m_mapKey.end() )
		return dNTL_KEY_INVALID;

	return it->second;
}

/**
* \brief Sets the action to the data structure.
* \param wKey (WORD) Created NTL key
* \param wAction (WORD) ACTION ID
*/
BYTE CActionMap::SetAction( WORD wKey, WORD wAction ) 
{
	// Check whether the action is registered.
	uNtlKey uUseKey; 
	uUseKey.key = FindKey( wAction );

	// ADD: If it does not exist in the existing data structure
	if( uUseKey.key == dNTL_KEY_INVALID )
	{
		// Check whether the current key is being used.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );

		// If the current key is not used, add it.
		if( itActionMap == m_mapAction.end() )
		{
			m_mapKey[wAction] = wKey;
			m_mapAction[wKey] = wAction;

			return SET_RESULT_OK;
		}

		// If the current key is being used, the previously registered Action is saved and deleted.
		WORD wOldAction = (*itActionMap).second;
		m_mapAction.erase( itActionMap );

		// Set the Key of the corresponding action in KeyMap to None.
		ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
		(*itKeyMap).second = dNTL_KEY_EMPTY;

		// Store actions that have lost their keys in a vector.
		SetLastReleaseAction( wOldAction );

		// Register keys and actions.
		m_mapKey[wAction] = wKey;
		m_mapAction[wKey] = wAction;
		
		return SET_RESULT_ALREADY_KEY;
	}
	// UPDATE: If the action is registered but the key is missing
	else if( uUseKey.key == dNTL_KEY_EMPTY )
	{
		// Check whether the current key is being used.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );
		
		// If the current key is not used, it changes the current action immediately.
		if( itActionMap == m_mapAction.end() )
		{
			// register the key
			m_mapAction[wKey] = wAction;

			// Register an action
			ACTIONMAP::iterator itKeyMap = m_mapKey.find( wAction );
			if( itKeyMap != m_mapKey.end() )
				(*itKeyMap).second = wKey;

			return SET_RESULT_OK;
		}
		
		// If the current key is being used, the previously registered Action is saved and deleted.
		WORD wOldAction = (*itActionMap).second;
		m_mapAction.erase( itActionMap );

		// Set the Key of the corresponding action in KeyMap to None.
		ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
		(*itKeyMap).second = dNTL_KEY_EMPTY;

		// Store actions that have lost their keys in a vector.
		SetLastReleaseAction( wOldAction );

		// Changes the value in the action.
		itKeyMap = m_mapKey.find( wAction );
		if( itKeyMap != m_mapKey.end() )
			(*itKeyMap).second = wKey;

		// Register the key.
		m_mapAction[wKey] = wAction;

		return SET_RESULT_ALREADY_KEY;
	}
	// UPDATE: When an action is registered and a KEY also exists in the action
	// And if that KEY exists or does not exist in another action
	else
	{
		// Save and delete the action of the previously registered key.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );

		// If the key exists in another action
		if( itActionMap != m_mapAction.end() )
		{
			WORD wOldAction = (*itActionMap).second;

			// If the existing key and action are the same
			if( wOldAction == wAction && (*itActionMap).first == wKey)
				return SET_RESULT_SAME_KEY;

			m_mapAction.erase( itActionMap );

			// Set the Key of the corresponding action in KeyMap to None.
			ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
			(*itKeyMap).second = dNTL_KEY_EMPTY;

			// Store actions that have lost their keys in a vector.
			SetLastReleaseAction( wOldAction );

			// Save the old KEY in the ACTION to be newly registered and insert the changed key.
			ACTIONMAP::iterator itNewKey = m_mapKey.find( wAction );
			WORD wOldKey = (*itNewKey).second;

			(*itNewKey).second = wKey;

			// Deletes the existing ActionMap.
			itActionMap = m_mapAction.find( wOldKey );
			if( itActionMap != m_mapAction.end() )
				m_mapAction.erase( itActionMap );

			// Register again.
			m_mapAction[wKey] = wAction;

			return SET_RESULT_ALREADY_KEY;
		}

		// When the key you are currently trying to register does not exist in another action (normal processing)
		ACTIONMAP::iterator itNewKey = m_mapKey.find( wAction );
		WORD wOldKey = (*itNewKey).second;

		(*itNewKey).second = wKey;

		// Deletes the existing ActionMap.
		itActionMap = m_mapAction.find( wOldKey );
		if( itActionMap != m_mapAction.end() )
			m_mapAction.erase( itActionMap );

		// Register again.
		m_mapAction[wKey] = wAction;

		return SET_RESULT_OK;
	}
}

VOID CActionMap::SetLastReleaseAction( WORD wOldAction ) 
{
	if( (int)m_vecReleaseAction.size() > 10 )
		m_vecReleaseAction.clear();

	m_vecReleaseAction.push_back( wOldAction );
}

/**
* \brief Finally, returns the Action whose key has been released.
*/
WORD CActionMap::GetLastReleaseAction() 
{
	if( m_vecReleaseAction.empty() )
		return ACTION_INVALID;

	WORD usReleaseAction = m_vecReleaseAction.back();
	m_vecReleaseAction.pop_back();

	return usReleaseAction;
}

/**
* \brief Initializes the data structure.
*/
VOID CActionMap::Clear() 
{
	m_mapAction.clear();
	m_mapKey.clear();
	ClearReleaseAction();
}

/**
* \brief Initializes the data structure that stores released actions.
*/
VOID CActionMap::ClearReleaseAction() 
{
	m_vecReleaseAction.clear();
}

/**
* \brief Saves the current data to the incoming data.
*/
VOID CActionMap::CaptureMap( CActionMap* pCaptureMap ) 
{
	pCaptureMap->GetKeyMap() = m_mapKey;
	pCaptureMap->GetActionMap() = m_mapAction;
}

/**
* \brief Constructs the current data with the incoming data.
*/
VOID CActionMap::RestoreMap( CActionMap* pCaptureMap ) 
{
	m_mapKey = pCaptureMap->GetKeyMap();
	m_mapAction = pCaptureMap->GetActionMap();
}

/**
* \brief = Operator overloading: Copy the data in the rhs term to the this term.
*/
CActionMap& CActionMap::operator=(const CActionMap& rhs)
{
	this->m_mapAction = rhs.m_mapAction;
	this->m_mapKey = rhs.m_mapKey;

	return (*this);
}

/**
* \brief Check the current map and any changes.
*/
BOOL CActionMap::CheckMap( ACTIONMAP& mapCapture ) 
{
	// If m_mapAction and m_mapCapture are the same, nothing has changed.
	if( m_mapKey == mapCapture )
		return FALSE;

	ACTIONMAP::iterator itOriginal;
	ACTIONMAP::iterator itCapture;

	// 1. Inspection using the action map as the original
	for(itOriginal = m_mapKey.begin(); itOriginal != m_mapKey.end(); ++itOriginal)
	{
		itCapture = mapCapture.find( itOriginal->first );

		// If it is present in the original but not in Capture
		if( itCapture == mapCapture.end() )
		{
			// registration
			m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_ADD, itOriginal->first, itOriginal->second );
		}
		// If both exist
		else
		{
			// If the height is different
			if( itOriginal->second != itCapture->second )
			{
				// change
				m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_UPDATE, itOriginal->first, itOriginal->second );
			}
		}
	}

	// 2. Inspection using the capture map as the original
	for( itCapture = mapCapture.begin(); itCapture != mapCapture.end(); ++itCapture )
	{
		itOriginal = m_mapKey.find( itCapture->first );

		// If it is in the capture but not in the original
		if( itOriginal == m_mapKey.end() )
		{
			// Delete
			m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_DEL, itCapture->first, itCapture->second );
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CActionMapManager
//
//
/////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Contruction
*/
CActionMapManager::CActionMapManager(void)
: m_wInputAction(ACTION_INVALID)
, m_pActionMap(NULL)
, m_pCaptureMap(NULL)
, m_pDefaultMap(NULL)
{
	Create();
}

/**
* \brief Destruction
*/
CActionMapManager::~CActionMapManager(void)
{
	Destroy();
}

/**
* \brief Create
*/
VOID CActionMapManager::Create( VOID ) 
{
	m_pActionMap = NTL_NEW CActionMap( this );
	m_pCaptureMap = NTL_NEW CActionMap( this );
	m_pDefaultMap = NTL_NEW CActionMap( this );

	// Init Variable data
	memset( m_asUpdateData, 0, sizeof( sSHORTCUT_UPDATE_DATA ) * NTL_SHORTCUT_MAX_COUNT );
	m_byUpdateCount = 0;

	m_byActionMapMode = ACTIONMAP_MODE_USERSETTING;

	RegisterKeyName();
	RegisterActionType();
	RegisterKeyType();
}

/**
* \brief Destroy
*/
VOID CActionMapManager::Destroy( VOID ) 
{
	NTL_DELETE( m_pDefaultMap );
	NTL_DELETE( m_pCaptureMap );
	NTL_DELETE( m_pActionMap );
}

/**
* \brief Defines the action map used by default.
*/
VOID CActionMapManager::InitDefaultActionMap() 
{
	m_pActionMap->Clear();

	// Avatar action
	SetSingleKey( 'W', ACTION_AVATAR_FORWARD );
	SetSingleKey( 'S', ACTION_AVATAR_BACKWARD );
	SetSingleKey( 'A', ACTION_AVATAR_LEFTTURN );
	SetSingleKey( 'D', ACTION_AVATAR_RIGHTTURN );
	SetSingleKey( 'Q', ACTION_AVATAR_LEFTSIDE);
	SetSingleKey( 'E', ACTION_AVATAR_RIGHTSIDE);
	SetSingleKey( 'X', ACTION_AVATAR_SITDOWN );
	SetSingleKey( ' ', ACTION_AVATAR_JUMP );
	SetSingleKey( '*', ACTION_AVATAR_AUTORUN );
	SetSingleKey( 'T', ACTION_AVATAR_SCOUTER );
	SetSingleKey( 'Z', ACTION_AVATAR_CHARGE );
	SetSingleKey( 'V', ACTION_AVATAR_LOOTING );
	SetSingleKey( 'R', ACTION_AVATAR_BLOCKING );

	// Target selection and shortcut key settings
	SetSingleKey(NTL_KEY_WAVE, ACTION_TARGET_SELF);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD1, ACTION_TARGET_1STPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD2, ACTION_TARGET_2NDPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD3, ACTION_TARGET_3RDPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD4, ACTION_TARGET_4THPARTY);

	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_TAB, ACTION_TARGET_MEMBER);

	SetSingleKey(NTL_KEY_TAB, ACTION_TARGET_AUTO);
	SetSingleKey('F', ACTION_TARGET_AUTOATK);
	SetSingleKey('/', ACTION_TARGET_AUTOFOLLOW);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F1, ACTION_TARGET_MARKING_1 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F2, ACTION_TARGET_MARKING_2 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F3, ACTION_TARGET_MARKING_3 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F4, ACTION_TARGET_MARKING_4 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F5, ACTION_TARGET_MARKING_5 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F1, ACTION_TARGET_SELECTING_1 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F2, ACTION_TARGET_SELECTING_2 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F3, ACTION_TARGET_SELECTING_3 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F4, ACTION_TARGET_SELECTING_4 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F5, ACTION_TARGET_SELECTING_5 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_TAB, ACTION_TARGET_AUTOTARGET_PARTY );

	// Quick Slot shortcut key settings
	SetSingleKey('1', ACTION_QUICK_1);
	SetSingleKey('2', ACTION_QUICK_2);
	SetSingleKey('3', ACTION_QUICK_3);
	SetSingleKey('4', ACTION_QUICK_4);
	SetSingleKey('5', ACTION_QUICK_5);
	SetSingleKey('6', ACTION_QUICK_6);
	SetSingleKey('7', ACTION_QUICK_7);
	SetSingleKey('8', ACTION_QUICK_8);
	SetSingleKey('9', ACTION_QUICK_9);
	SetSingleKey('0', ACTION_QUICK_0);
	SetSingleKey(NTL_KEY_DASH, ACTION_QUICK_MIN);
	SetSingleKey(NTL_KEY_EQUAL, ACTION_QUICK_PLUS);
	SetCombineKey(NTL_KEY_F1, ACTION_QUICK_1_EX);
	SetCombineKey(NTL_KEY_F2, ACTION_QUICK_2_EX);
	SetCombineKey(NTL_KEY_F3, ACTION_QUICK_3_EX);
	SetCombineKey(NTL_KEY_F4, ACTION_QUICK_4_EX);
	SetCombineKey(NTL_KEY_F5, ACTION_QUICK_5_EX);
	SetCombineKey(NTL_KEY_F6, ACTION_QUICK_6_EX);
	SetCombineKey(NTL_KEY_F7, ACTION_QUICK_7_EX);
	SetCombineKey(NTL_KEY_F8, ACTION_QUICK_8_EX);
	SetCombineKey(NTL_KEY_F9, ACTION_QUICK_9_EX);
	SetCombineKey(NTL_KEY_F10, ACTION_QUICK_0_EX);
	SetCombineKey(NTL_KEY_F11, ACTION_QUICK_MIN_EX);
	SetCombineKey(NTL_KEY_F12, ACTION_QUICK_PLUS_EX);
	SetCombineKey(NTL_ALT_BIT | '1', ACTION_QUICK_1_EX2);
	SetCombineKey(NTL_ALT_BIT | '2', ACTION_QUICK_2_EX2);
	SetCombineKey(NTL_ALT_BIT | '3', ACTION_QUICK_3_EX2);
	SetCombineKey(NTL_ALT_BIT | '4', ACTION_QUICK_4_EX2);
	SetCombineKey(NTL_ALT_BIT | '5', ACTION_QUICK_5_EX2);
	SetCombineKey(NTL_ALT_BIT | '6', ACTION_QUICK_6_EX2);
	SetCombineKey(NTL_ALT_BIT | '7', ACTION_QUICK_7_EX2);
	SetCombineKey(NTL_ALT_BIT | '8', ACTION_QUICK_8_EX2);
	SetCombineKey(NTL_ALT_BIT | '9', ACTION_QUICK_9_EX2);
	SetCombineKey(NTL_ALT_BIT | '0', ACTION_QUICK_0_EX2);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_DASH, ACTION_QUICK_MIN_EX2);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_EQUAL, ACTION_QUICK_PLUS_EX2);
	SetSingleKey(0xDB, ACTION_QUICK_PREV); // '['
	SetSingleKey(0xDD, ACTION_QUICK_NEXT); // ']'
//	SetSingleKey('', ACTION_QUICK_2NDCAP);
//	SetSingleKey('', ACTION_QUICK_3RDCAP);
//	SetSingleKey('', ACTION_QUICK_4THCAP);
//	SetSingleKey('', ACTION_QUICK_5THCAP);
//	SetSingleKey('', ACTION_QUICK_SUMMON1);
//	SetSingleKey('', ACTION_QUICK_SUMMON2);


	// Window Shortcut key settings
	SetSingleKey('C', ACTION_WINDOW_PROFILE);
	SetSingleKey('K', ACTION_WINDOW_SKILL);
	SetSingleKey('J', ACTION_WINDOW_QUEST);
	SetSingleKey('N', ACTION_WINDOW_MINIMAP);
	SetSingleKey('M', ACTION_WINDOW_WORLDMAP);
	SetSingleKey('I', ACTION_WINDOW_MAINCAP);
	SetSingleKey('P', ACTION_WINDOW_PARTY);
	SetSingleKey('G', ACTION_WINDOW_GUILD);
	SetSingleKey('L', ACTION_WINDOW_FRIEND);
	SetSingleKey('O', ACTION_WINDOW_OPTION);
	SetSingleKey('H', ACTION_WINDOW_HELP);
	SetCombineKey(NTL_CTRL_BIT | 'M', ACTION_WINDOW_MAIN);
	SetSingleKey('B', ACTION_WINDOW_RANKBOARD);
	//SetSingleKey('U', ACTION_WINDOW_SCOUTERUI); //added scouter shortcut
	SetSingleKey('.', ACTION_WINDOW_HOIPOIMIX);
	SetSingleKey(',', ACTION_WINDOW_MASCOT);

	// Fixed (unchangeable)
	SetSingleKey(NTL_KEY_SUBTRACT, ACTION_MINIMAP_ZOOMOUT);
	SetSingleKey(NTL_KEY_ADD, ACTION_MINIMAP_ZOOMIN);

	SetSingleKey(NTL_KEY_PRIOR, ACTION_CHAT_PGUP);
	SetSingleKey(NTL_KEY_NEXT, ACTION_CHAT_PGDN);

	SetSingleKey(NTL_KEY_ESCAPE, ACTION_GLOBAL_CANCLE);
	SetSingleKey(NTL_KEY_SNAPSHOT, ACTION_GLOBAL_SNAPSHOT);
	SetSingleKey(NTL_KEY_RETURN, ACTION_GLOBAL_CHAT);

	// Create a default map.
	m_pActionMap->CaptureMap( m_pDefaultMap );
}

/**
* \brief Set the key to be used as a single, not a combination.
* \param byKey (BYTE) KEY DATA
* \param wAction (WORD) ACTION ID
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::SetSingleKey(BYTE byKey, WORD wAction )
{
	return m_pActionMap->SetAction( (WORD)byKey, wAction );
}

/**
* \brief Set the key used as the combination key.
* \param wKey (BYTE) KEY DATA
* \param wAction (WORD) ACTION ID
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::SetCombineKey( WORD wKey, WORD wAction ) 
{
	return m_pActionMap->SetAction( wKey, wAction );
}

/**
* \brief Find the Action with the created key.
* \param wKey (WORD) KEY DATA
* \return ACTION ID
*/
WORD CActionMapManager::FindAction( WORD wKey ) 
{
	switch( m_byActionMapMode )
	{
	case ACTIONMAP_MODE_USERSETTING:
		return m_pActionMap->FindAction( wKey );
	case ACTIONMAP_MODE_DEFAULT:
		return m_pDefaultMap->FindAction( wKey );
	default:
		return m_pActionMap->FindAction( wKey );
	}
}

/**
* Find the key with the \brief action.
* \param wAction (WORD) ACTION ID
* \return KEY DATA
*/
WORD CActionMapManager::FindKey( WORD wAction ) 
{
	return m_pActionMap->FindKey( wAction );
}

/**
* \brief Specifies the input mode for the action map.
* \param wAction (WORD) ACTION ID to be the input mode
* \return Success
*/
BOOL CActionMapManager::SetInputAction( WORD wAction ) 
{
	// If the action is a fixed action
	if( IsFixedAction( wAction ) )
	{
		SetLastResult( SET_RESULT_FIXED_ACTION );
		m_wInputAction = ACTION_INVALID;
		return FALSE;
	}

	m_wInputAction = wAction;

	return TRUE;
}

/**
* \brief Records the last setting result.
* \param byResult (BYTE) eACTIONMAP_SET_RESULT
*/
VOID CActionMapManager::SetLastResult( BYTE byResult ) 
{
	// A defense code you might not know about.
	if( (int)m_vecResult.size() > 10 )
		m_vecResult.clear();

	m_vecResult.push_back( byResult );
}

/**
* \brief Reads the last setting result.
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::GetLastResult() 
{
	if( m_vecResult.empty() )
		return SET_RESULT_INVALID;

	BYTE byLastResult = m_vecResult.back();
	m_vecResult.pop_back();

	return byLastResult;
}

/**
* \brief Clears the error code.
*/
VOID CActionMapManager::ClearResult() 
{
	m_vecResult.clear();
}

/**
* \brief Retrieves the name of the key using the action ID.
* \param usAction
* \return Name of key (Unicode)
*/
std::wstring CActionMapManager::GetKeyName( WORD wAction ) 
{
	uNtlKey uKey;
	memset( &uKey, 0, sizeof(uNtlKey) );

	// In case of default
	if( m_byActionMapMode == ACTIONMAP_MODE_USERSETTING )
	{
		uKey.key = m_pActionMap->FindKey( wAction );
	}
	else if( m_byActionMapMode == ACTIONMAP_MODE_DEFAULT )
	{
		uKey.key = m_pDefaultMap->FindKey( wAction );
	}

	std::wstring wstrKeyName;

	// If the key is not defined, returns blank.
	if( uKey.key == dNTL_KEY_EMPTY )
		return wstrKeyName;

	// If a combination key is included, enter the name of the combination key.
	if( uKey.BIT.bit8 == 0x1 )	// ALT
	{
		wstrKeyName += L"@";
	}
	if(	uKey.BIT.bit7 == 0x1 )	// CTRL
	{
		wstrKeyName += L"^";
	}

	// If a key exists, add the name of the key. If not, it is an unregistered key name.
	if( m_mapKeyName.end() != m_mapKeyName.find( uKey.BIT.ori ) )
	{
		wstrKeyName += m_mapKeyName[uKey.BIT.ori];
	}
	else
	{
		wstrKeyName += L"UNKNOWN";
	}

	return wstrKeyName.c_str();
	
}

/**
* \brief Input Handler
* \param byKey (BYTE) Key Data
* \return ACTION ID
*/
WORD CActionMapManager::InputHandlerDown( BYTE byKey ) 
{
	// If the action map manager is Input Mode, assign the key to the action.
	if( m_wInputAction != ACTION_INVALID )
	{
		//// The action map manager assigns the key only when bInput is TRUE.
		//if( bInput )
		//{
		//	// If it is a combination key, just return
		//	if( IsCombineKey( byKey ) )
		//		return ACTION_INVALID;

		//	// Create WORD-type NTL Key Data with the given key.
		//	WORD wMakeKey = MakeKey(byKey);

		//	// If the key is fixed, record an error code and return.
		//	if( IsFixedKey( wMakeKey ) )
		//	{
		//		SetLastResult( SET_RESULT_FIXED_KEY );
		//		m_wInputAction = ACTION_INVALID;
		//		return ACTION_INVALID;
		//	}

		//	// This is an action that cannot use a combination key, but if you try to set it with a combination key,
		//	if( IsExclusiveAction( m_wInputAction ) && wMakeKey > 0x00FF )
		//	{
		//		SetLastResult( SET_RESULT_NOT_COMBINE_ACTION );
		//		m_wInputAction = ACTION_INVALID;
		//		return ACTION_INVALID;
		//	}

		//	// SetAction
		//	SetLastResult( m_pActionMap->SetAction( wMakeKey, m_wInputAction ) );

		//	m_wInputAction = ACTION_INVALID;
		//}
		
		return ACTION_INVALID;
	}

	// Create an NTL key.
	WORD wMakeKey = MakeKey( byKey );

	// Extract the original from the current key
	WORD wReturnAction = ACTION_INVALID;
	WORD wFindOriginalKeyAction = FindAction( wMakeKey & 0x00FF );	
	if( IsExclusiveAction( wFindOriginalKeyAction ) )
	{
		wReturnAction = wFindOriginalKeyAction;
	}
	else
	{
		wReturnAction = FindAction( wMakeKey );
	}

	InputActionDown( wReturnAction );

	// If not, the action for the combined key is found and returned.
	return wReturnAction;
}

WORD CActionMapManager::InputHandlerUp( BYTE byKey, std::list< WORD >& listUpAction )
{
	// If the action map manager is Input Mode, assign the key to the action.
	if( m_wInputAction != ACTION_INVALID )
	{
		// If it is a combination key, just return
		if( IsCombineKey( byKey ) )
			return ACTION_INVALID;

		// Create WORD-type NTL Key Data with the given key.
		WORD wMakeKey = MakeKey( byKey );

		// If the key is fixed, record an error code and return.
		if( IsFixedKey( wMakeKey ) )
		{
			SetLastResult( SET_RESULT_FIXED_KEY );
			m_wInputAction = ACTION_INVALID;
			return ACTION_INVALID;
		}

		// This is an action that cannot use a combination key, but if you try to set it with a combination key,
		if( IsExclusiveAction( m_wInputAction ) && wMakeKey > 0x00FF )
		{
			SetLastResult( SET_RESULT_NOT_COMBINE_ACTION );
			m_wInputAction = ACTION_INVALID;
			return ACTION_INVALID;
		}

		// Set Action
		SetLastResult( m_pActionMap->SetAction( wMakeKey, m_wInputAction ) );

		m_wInputAction = ACTION_INVALID;

		return ACTION_INVALID;
	}

	// If the currently pressed key is a combination key
	if( IsCombineKey( byKey ) )
	{
		// You must raise all the pushed actions that are triggered by the current combination key.
		CheckDownActionWithCombineKey( byKey, listUpAction );

		return ACTION_INVALID;
	}
	else
	{
		// Create an NTL key.
		WORD wMakeKey = MakeKey( byKey );

		// Extract the original from the current key
		WORD wReturnAction = ACTION_INVALID;
		WORD wFindOriginalKeyAction = FindAction( wMakeKey & 0x00FF );	
		if( IsExclusiveAction( wFindOriginalKeyAction ) )
		{
			wReturnAction = wFindOriginalKeyAction;
		}
		else
		{
			wReturnAction = FindAction( wMakeKey );
		}

		InputActionUp( wReturnAction );

		// If not, it finds and returns the action of the combined key.
		return wReturnAction;
	}
}

void CActionMapManager::InputActionDown( WORD wAction )
{
	if( wAction == ACTION_INVALID )
		return;

	mapdef_InputAction::iterator it = m_mapInputAction.find( wAction );

	// Can't find it
	if( it == m_mapInputAction.end() )
	{
		m_mapInputAction[wAction] = 1;
	}
	else
	{
		(*it).second++;
	}
}

void CActionMapManager::InputActionUp( WORD wAction )
{
	if( wAction == ACTION_INVALID )
		return;

	mapdef_InputAction::iterator it = m_mapInputAction.find( wAction );

	// Can't find it
	if( it != m_mapInputAction.end() )
	{
		(*it).second--;
		if( (*it).second <= 0 )
		{
			m_mapInputAction.erase( it );
		}
	}
}

void CActionMapManager::CheckDownActionWithCombineKey( BYTE byKey, std::list< WORD >& listUpAction )
{
	mapdef_InputAction::iterator it = m_mapInputAction.begin();
	while( it != m_mapInputAction.end() )
	{
		WORD wKey = m_pActionMap->FindKey( (*it).first );
		if( wKey != dNTL_KEY_INVALID )
		{
			if( wKey & byKey )
			{
				listUpAction.push_back( (*it).first );
				it = m_mapInputAction.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
}

/**
* \brief Make a copy of the action map.
*/
VOID CActionMapManager::CaptureActionMap() 
{
	m_pActionMap->CaptureMap( m_pCaptureMap );
}

/**
* \brief Return to the copy you made.
*/
VOID CActionMapManager::RestoreActionMap() 
{
	m_pActionMap->RestoreMap( m_pCaptureMap );
}

/**
* Add \brief UpdateData.
* \param byType (BYTE) 0: ADD, 1: DEL, 2: UPDATE
* \param wActionID (WORD) ACTION ID
* \param wKey (WORD) KEY DATA
*/
void CActionMapManager::AddUpdateData( BYTE byType, WORD wActionID, WORD wKey ) 
{
	m_asUpdateData[m_byUpdateCount].byType = byType;
	m_asUpdateData[m_byUpdateCount].wActionID = wActionID;
	m_asUpdateData[m_byUpdateCount].wKey = wKey;

	++m_byUpdateCount;
}

/**
*\breif Gets UpdateData.
* \param pData (sSHORTCUT_UPDATE_DATA*)
* \param byCount(BYTE&)
* \return
*/
RwBool CActionMapManager::GetUpdateData( sSHORTCUT_UPDATE_DATA* pData , BYTE& byCount ) 
{
	if( m_pActionMap->CheckMap( m_pCaptureMap->GetKeyMap() ) )
	{
		byCount = m_byUpdateCount;
		memcpy( pData, m_asUpdateData, sizeof(sSHORTCUT_UPDATE_DATA) * m_byUpdateCount);
		
		// Clear Update Data
		m_byUpdateCount = 0;
		memset( m_asUpdateData, 0, sizeof(sSHORTCUT_UPDATE_DATA) * NTL_SHORTCUT_MAX_COUNT);
		return TRUE;
	}
	return FALSE;
}

/**
* Create the \brief key in the format used in NTL.
* \param byKey (BYTE) KEY DATA
* \return NTL KEY
*/
WORD CActionMapManager::MakeKey( BYTE byKey ) 
{
	uNtlKey uMakeKey;
	memset( &uMakeKey, 0, sizeof( uNtlKey ) );

	// Set the current combination key
	MakeComposition( uMakeKey );

	// setting the key
	uMakeKey.BIT.ori = byKey;

	return uMakeKey.key;
}

/**
* \brief Set the combination key.
* \param uMakeKey (uNtlKey&) Union declared in NtlActionMapDef.h
*/
void CActionMapManager::MakeComposition( uNtlKey& uMakeKey ) 
{
	// Check the combination key
	if( ( GetKeyState( VK_LMENU ) & 0x80 ) || ( GetKeyState( VK_RMENU ) & 0x80 ) )
		uMakeKey.BIT.bit8 = 0x1;

	if( ( GetKeyState( VK_LCONTROL ) & 0x80 ) || ( GetKeyState( VK_RCONTROL ) & 0x80 ) )
		uMakeKey.BIT.bit7 = 0x1;
}

/**
* \brief Hard-codes the names of various keys.
*/
VOID CActionMapManager::RegisterKeyName( VOID ) 
{
	m_mapKeyName[NTL_KEY_BACKSPACE] = L"BackSP";
	m_mapKeyName[NTL_KEY_TAB] = L"TAB";
	m_mapKeyName[NTL_KEY_RETURN] = L"ENTER";

	m_mapKeyName[NTL_KEY_NUMPAD0] = L"NUM 0";
	m_mapKeyName[NTL_KEY_NUMPAD1] = L"NUM 1";
	m_mapKeyName[NTL_KEY_NUMPAD2] = L"NUM 2";
	m_mapKeyName[NTL_KEY_NUMPAD3] = L"NUM 3";
	m_mapKeyName[NTL_KEY_NUMPAD4] = L"NUM 4";
	m_mapKeyName[NTL_KEY_NUMPAD5] = L"NUM 5";
	m_mapKeyName[NTL_KEY_NUMPAD6] = L"NUM 6";
	m_mapKeyName[NTL_KEY_NUMPAD7] = L"NUM 7";
	m_mapKeyName[NTL_KEY_NUMPAD8] = L"NUM 8";
	m_mapKeyName[NTL_KEY_NUMPAD9] = L"NUM 9";
	m_mapKeyName[NTL_KEY_MULTIPLY] = L"NUM *";
	m_mapKeyName[NTL_KEY_ADD] = L"NUM +";
	m_mapKeyName[NTL_KEY_SUBTRACT] = L"NUM -";
	m_mapKeyName[NTL_KEY_DECIMAL] = L"NUM .";
	m_mapKeyName[NTL_KEY_DIVIDE] = L"NUM /";

	m_mapKeyName[NTL_KEY_F1] = L"F1";
	m_mapKeyName[NTL_KEY_F2] = L"F2";
	m_mapKeyName[NTL_KEY_F3] = L"F3";
	m_mapKeyName[NTL_KEY_F4] = L"F4";
	m_mapKeyName[NTL_KEY_F5] = L"F5";
	m_mapKeyName[NTL_KEY_F6] = L"F6";
	m_mapKeyName[NTL_KEY_F7] = L"F7";
	m_mapKeyName[NTL_KEY_F8] = L"F8";
	m_mapKeyName[NTL_KEY_F9] = L"F9";
	m_mapKeyName[NTL_KEY_F10] = L"F10";
	m_mapKeyName[NTL_KEY_F11] = L"F11";
	m_mapKeyName[NTL_KEY_F12] = L"F12";

	m_mapKeyName[NTL_KEY_PAUSE] = L"PAUSE";
	m_mapKeyName[NTL_KEY_TOCHINESE] = L"CHINESE";
	m_mapKeyName[NTL_KEY_ESCAPE] = L"ESC";
	m_mapKeyName[NTL_KEY_SPACE] = L"SPACE";
	m_mapKeyName[NTL_KEY_SHIFT] = L"SHIFT";

	m_mapKeyName[NTL_KEY_PRIOR] = L"PGUP";
	m_mapKeyName[NTL_KEY_NEXT] = L"PGDN";
	m_mapKeyName[NTL_KEY_END] = L"END";
	m_mapKeyName[NTL_KEY_HOME] = L"HOME";
	m_mapKeyName[NTL_KEY_INSERT] = L"INS";
	m_mapKeyName[NTL_KEY_DELETE] = L"DEL";	

	m_mapKeyName[NTL_KEY_LEFT] = L"◀";
	m_mapKeyName[NTL_KEY_UP] = L"▲";
	m_mapKeyName[NTL_KEY_RIGHT] = L"▶";
	m_mapKeyName[NTL_KEY_DOWN] = L"▼";

	m_mapKeyName[NTL_KEY_SELECT] = L"SELECT";
	m_mapKeyName[NTL_KEY_PRINT] = L"PRINT";
	m_mapKeyName[NTL_KEY_EXECUTE] = L"EXECUTE";
	m_mapKeyName[NTL_KEY_SNAPSHOT] = L"PRC/SRQ";
	m_mapKeyName[NTL_KEY_HELP] = L"HELP";
	m_mapKeyName[NTL_KEY_LEFTWINDOW] = L"LEFT WIN";
	m_mapKeyName[NTL_KEY_RIGHTWINDOW] = L"RIGHT WIN";

	m_mapKeyName[NTL_KEY_CAPITAL] = L"CAPS LOCK";
	m_mapKeyName[NTL_KEY_NUMLOCK] = L"NUMS LOCK";
	m_mapKeyName[NTL_KEY_SCROLL] = L"SCR LOCK";

	m_mapKeyName[NTL_KEY_DASH] = L"-";
	m_mapKeyName[NTL_KEY_EQUAL] = L"=";
	m_mapKeyName[NTL_KEY_WAVE] = L"~";

	m_mapKeyName[NTL_KEY_LBRACKET] = L"[";
	m_mapKeyName[NTL_KEY_RBRACKET] = L"]";

	m_mapKeyName[NTL_KEY_PERIOD] = L".";
	m_mapKeyName[NTL_KEY_COMMA] = L",";
	m_mapKeyName[NTL_KEY_OEM_2] = L"/";

	// Normal keys A~Z , 0 ~ 9
	WCHAR awcBuffer[32] = {0,};
	for( unsigned short uiChar = 0x41 ; uiChar <= 0x5A; ++uiChar)
	{
		swprintf_s( awcBuffer, 32, L"%c", uiChar);
		m_mapKeyName[uiChar] = awcBuffer;
	}

	for( unsigned short uiChar = 0x30 ; uiChar <= 0x39; ++uiChar )
	{
		swprintf_s( awcBuffer, 32, L"%c", uiChar);
		m_mapKeyName[uiChar] = awcBuffer;
	}

	// Not specified
	m_mapKeyName[NTL_KEY_EMPTY] = L" ";	
	
}

/**
* \brief Defines key properties.
*/
VOID CActionMapManager::RegisterKeyType( VOID ) 
{
	m_mapKeyType[NTL_KEY_SUBTRACT]	= TYPE_FIXED;	// NUM -
	m_mapKeyType[NTL_KEY_ADD]		= TYPE_FIXED;	// NUM +
	m_mapKeyType[NTL_KEY_PRIOR]		= TYPE_FIXED;	// PG UP
	m_mapKeyType[NTL_KEY_NEXT]		= TYPE_FIXED;	// PG DN
	m_mapKeyType[NTL_KEY_ESCAPE]	= TYPE_FIXED;	// ESC
	m_mapKeyType[NTL_KEY_SNAPSHOT]	= TYPE_FIXED;	// PRC/SRQ
	m_mapKeyType[NTL_KEY_RETURN]	= TYPE_FIXED;	// ENTER

//	m_mapKeyType[NTL_KEY_CONTROL]	= TYPE_COMBINE;	// CTRL	// disabled by daneos. If this is enabled, player stops moving when release CTRL
//	m_mapKeyType[NTL_KEY_MENU]		= TYPE_COMBINE; // ALT	// disabled by daneos. If this is enabled, player stops moving when release ALT
}

/**
* \brief Defines the properties of the action.
*/
VOID CActionMapManager::RegisterActionType( VOID ) 
{
	m_mapActionType[ACTION_MINIMAP_ZOOMOUT]		= TYPE_FIXED;
	m_mapActionType[ACTION_MINIMAP_ZOOMIN]		= TYPE_FIXED;
	m_mapActionType[ACTION_CHAT_PGUP]			= TYPE_FIXED;
	m_mapActionType[ACTION_CHAT_PGDN]			= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_CANCLE]		= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_SNAPSHOT]		= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_CHAT]			= TYPE_FIXED;

	// Movement-related actions should be excluded from other actions 
	m_mapActionType[ACTION_AVATAR_FORWARD]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_BACKWARD]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_LEFTTURN]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_RIGHTTURN]	= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_LEFTSIDE]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_RIGHTSIDE]	= TYPE_EXCLUSIVE;

	// The energy gathering action is also excluded from other actions.
	m_mapActionType[ACTION_AVATAR_CHARGE]		= TYPE_EXCLUSIVE;
}

/**
* \brief Checks whether the attribute is defined in the specified map.
* \param nType (int) Property defined in NtlActionMapDef.h
* \param wKeyType (WORD) Key Type defined in the map, whether it is an action or a key.
* \param mapType (TYPEMAP&) Returns the map.
* \return (BOOL) Whether it is defined or not
*/
BOOL CActionMapManager::IsMapType( int nType, WORD wKeyType, TYPEMAP& mapType ) 
{
	TYPEMAP::iterator it = mapType.find( wKeyType );
	if( mapType.end() == it )
		return FALSE;

	return (*it).second & nType;
}

VOID CActionMapManager::ClearActionMap() 
{
	m_pActionMap->Clear();
	m_pCaptureMap->Clear();
	ClearResult();
	ClearInputAction();
	ClearReleaseAction();
}

/**
* \brief Specifies the mode in which ActionMap is used.
*/
VOID CActionMapManager::SetActionMapMode( BYTE byActionMapMode )
{
	if( byActionMapMode >= ACTIONMAP_MODE_INVALID )
		assert( 0 && "Not exist actionmap mode of eACTIONMAP_MODE(enum)" );

	if( m_byActionMapMode != byActionMapMode )
	{
		m_byActionMapMode = byActionMapMode;
		CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_OK );
	}
}

void CActionMapManager::InputClear()
{
	m_mapInputAction.clear();
}

