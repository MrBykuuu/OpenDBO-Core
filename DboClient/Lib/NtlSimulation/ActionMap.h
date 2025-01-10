/*****************************************************************************
* File			: ActionMap.h
* Author		: Haesung, Cho
* Copyright		: NTL Co., Ltd.
* Date			: 2007. 9. 20
* Abstract		: Action Map
*****************************************************************************
* Desc			: Define the data structure of the action map and create the manager of the action map.
* History  -----
* 2008.  2. 22	: Refactoring, separation of data structure and manager from existing action map method (addition of properties)
*****************************************************************************/

#ifndef __ACTION_MAP_H__
#define __ACTION_MAP_H__

// Simulation
#include "NtlActionMapDef.h"
#include "InputHandler.h"

// Shared
#include "NtlCharacter.h"

#include <map>
#include <vector>
#include <set>

class CActionMapManager;

/**
* \ingroup simulation
* \brief Data structure of NTL ActionMap
*/
class CActionMap
{
public:
	// Constructor & Destructor
	CActionMap(void);
	CActionMap( CActionMapManager* pManager );
	CActionMap(const CActionMap& rhs);
	~CActionMap(void);

	WORD	FindAction( WORD wKey );			// Find Action by Key value.
	WORD	FindKey( WORD wAction );			// Find the Key with Action.

	BYTE			SetAction( WORD wKey, WORD wAction );	// specify the key
	VOID			SetLastReleaseAction( WORD wOldAction );
	WORD			GetLastReleaseAction();

	VOID			Clear();									// Initialize ActionMap.
	VOID			ClearReleaseAction();

	VOID			CaptureMap( CActionMap* pCaptureMap );
	VOID			RestoreMap( CActionMap* pCaptureMap );
	BOOL			CheckMap( ACTIONMAP& mapCapture );

	ACTIONMAP&		GetActionMap()							{	return m_mapAction;	}
	ACTIONMAP&		GetKeyMap()							{	return m_mapKey;	}

	CActionMap& operator=(const CActionMap& rhs);				// Operator overloading

protected:
	ACTIONMAP		m_mapKey;				// Data structure < ACTION , KEY >
	ACTIONMAP		m_mapAction;			// Data structure < KEY, ACTION >
	ACTIONVEC		m_vecReleaseAction;		// Data structure for remembering released actions
	CActionMapManager*	m_pManager;	
};

/**
* \ingroup simulation
* \brief NTL ActionMap의 Manager
*/
class CActionMapManager
{
public:
	///< Construction & Destruction
	CActionMapManager(void);
	~CActionMapManager(void);

	///< Create & Destroy
	VOID	Create(VOID);
	VOID	Destroy(VOID);

	///< Basic action map settings (hard-coding)
	VOID	InitDefaultActionMap();

	BYTE	SetSingleKey( BYTE byKey, WORD wAction );		///< Single key setting
	BYTE	SetCombineKey( WORD wKey, WORD wAction );		///< Combination key setting

	WORD	FindAction( WORD wKey );
	WORD	FindKey( WORD wAction );

	WORD	GetInputAction()						{ return m_wInputAction; }
	BOOL	SetInputAction( WORD wAction );					///< Specify input mode
	VOID	ClearInputAction()						{ m_wInputAction = ACTION_INVALID; }

	BYTE	GetLastResult();
	VOID	SetLastResult( BYTE byResult );

	VOID	ClearResult();

	WORD	GetLastReleaseAction()					{ return m_pActionMap->GetLastReleaseAction(); }
	VOID			ClearReleaseAction()			{ m_pActionMap->ClearReleaseAction(); }

	std::wstring	GetKeyName( WORD wAction );

	WORD	InputHandlerDown( BYTE byKey );
	WORD	InputHandlerUp( BYTE byKey, std::list< WORD >& listUpAction );
	void	InputActionDown( WORD wAction );
	void	InputActionUp( WORD wAction );

	void	CheckDownActionWithCombineKey( BYTE byKey, std::list< WORD >& listUpAction );

	void	InputClear();

	VOID	CaptureActionMap();
	VOID	RestoreActionMap();

	void	AddUpdateData(BYTE byType, WORD wActionID, WORD wKey);
	RwBool	GetUpdateData(sSHORTCUT_UPDATE_DATA* pData , BYTE& byCount);

	VOID	ClearActionMap();

	VOID	SetActionMapMode( BYTE byActionMapMode );
	BYTE	GetActionMapMode()					{ return m_byActionMapMode; }
protected:
	WORD	MakeKey( BYTE byKey );
	virtual void	MakeComposition( uNtlKey& uMakeKey );

	VOID	RegisterKeyName(VOID);
	
	VOID	RegisterKeyType(VOID);
	VOID	RegisterActionType(VOID);

public:
	BOOL	IsFixedKey( WORD wKey )				{ return IsMapType( TYPE_FIXED, wKey, m_mapKeyType ); }
	BOOL	IsFixedAction( WORD wAction )		{ return IsMapType( TYPE_FIXED, wAction, m_mapActionType ); }
	BOOL	IsCombineKey( WORD wKey )			{ return IsMapType( TYPE_COMBINE, wKey, m_mapKeyType ); }
	BOOL	IsExclusiveAction( WORD wAction )	{ return IsMapType( TYPE_EXCLUSIVE, wAction, m_mapActionType ); }

	BOOL	IsMapType( int nType, WORD wKeyType, TYPEMAP& mapType );

protected:
	KEYNAMEMAP	m_mapKeyName;
	TYPEMAP		m_mapKeyType;
	TYPEMAP		m_mapActionType;

	typedef std::map< unsigned short, unsigned int > mapdef_InputAction;
	mapdef_InputAction	m_mapInputAction;							///< Reference list of Actions currently detected with InputHandlerDown

	RESULTVEC	m_vecResult;

	WORD		m_wInputAction;										///< When receiving key input, it has a value other than ACTION_INVALID.
	
	CActionMap*		m_pActionMap;									///< ActionMap used in the client
	CActionMap*		m_pCaptureMap;									///< It has something saved on the server.
	CActionMap*		m_pDefaultMap;									///< The most default ActionMap

	sSHORTCUT_UPDATE_DATA	m_asUpdateData[NTL_SHORTCUT_MAX_COUNT];	///< Array to store changes
	BYTE					m_byUpdateCount;						///< When there is a change in the key	

	BYTE					m_byActionMapMode;						///< ACTIONMAP usage mode
	
};

#endif // __ACTION_MAP_H__