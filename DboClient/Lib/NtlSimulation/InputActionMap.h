/*****************************************************************************
 *
 *File			: InputAction.h
 *Author		: HyungSuk, Jang
 *Copyright	: (аж)NTL
 *Date			: 2005. 8. 26	
 *Abstract		: Input action 
 *****************************************************************************
 *Desc         : 
 *
 *1st Refactoring(2008. 1. 15 ~ 1. 20 ) : 
 *Completely separate data structure and manager ( InputActionMap <-> ActionMap )
 *
 *****************************************************************************/

#ifndef __INPUT_ACTION_MAP_H__
#define __INPUT_ACTION_MAP_H__

#include "NtlCallbackWidget.h"
#include "InputHandler.h"
#include "ceventhandler.h"
#include "NtlSLDef.h"

// Action map

#include "ActionMap.h"

// Decameter Structure

typedef struct _SInputDashMap
{
	RwUInt8 byDown;
	RwReal fTime;
	RwUInt32 uiMoveFlags;
}SInputDashMap;

#define NTL_KEYBOARD_DBCLICK_TIME		0.15f

/**
* \ingroup client
*ActionMap Manager
*/
class CInputActionMap : public RWS::CEventHandler
{
public:
	CInputActionMap();
	~CInputActionMap();

	static CInputActionMap* GetInstance(void);

	RwBool Create(void); 
	void Destroy(void);
	void Update(RwReal fElapsed);
	void HandleEvents(RWS::CMsg &pMsg);
	void HandleEventActionmapLoadInfo(RWS::CMsg& msg);
	void HandleEventsActionMapUpdateRes(RWS::CMsg& msg);
	void Reset(void);
	void ResetMoveFlags(void);

	int KeyDownHandler(unsigned int pKeyData);
	int KeyUpHandler(unsigned int pKeyData);

	int MouseDownHandler(unsigned int pMouseData);
	int MouseUpHandler(unsigned int pMouseData);

	void SetActive(RwBool bActive);

public:
	// CActionMap interface
	void			InitInputMode();					///< Disable input mode
	RwUInt32		GetInputMode();						///<Which ACTION is INPUTMODE?
	void			SetInputMode( RwUInt32 nAction );	///< Specify input mode for nAction

	std::wstring	GetKeyName( RwUInt32 nAction );		///< Name of the key held by the action
	unsigned short	FindKey( RwUInt32 nAction );

	void			InitDefaultActionMap();				///< Set as default action map

	RwBool			ApplyActionMap();					///< Application of action map (packet transmission to server)
	void			CancleActionMap();					///< Cancel Apply

	// Ignore Key
	RwBool			RegisterIgnoreKey( RwUInt8 byKey, int nRefCount );
	RwBool			RemoveIgnoreKey( RwUInt8 byKey );
	RwBool			IsIgnoreKey( RwUInt8 byKey, RwBool bRef = FALSE );
	void			ClearIgnoreKey();

	// ActionMap Mode
	RwUInt8			GetActionMapMode();

protected:
	RwInt32 GetKeyDownCount(RwUInt32 uiFlags);
	RwUInt32 ConvertServerFlags(RwUInt32 uiFlags);

	void HitTestDownDbClickDashMap(SInputDashMap& sDashMap);
	void HitTestUpDbClickDashMap(SInputDashMap& sDashMap);
	void CallDashMove(RwUInt32 uiServerDashMoveFlags);

	// Functions changed to process by passing action ID
	void ActionDownMoveHandler(RwUInt32 uiAction);		///< Movement action handler by action
	void ActionDownDashMoveHandler(RwUInt32 uiAction);	///< Dash action handler by action
	void ActionDownBehaviorHandler(RwUInt32 uiAction);	///< Action by action Action handler
	void ActionDownGuiHandler(RwUInt32 uiAction);		///< GUI action handler by action

	void ActionUpMoveHandler(RwUInt32 uiAction);			
	void ActionUpDashMoveHandler(RwUInt32 uiAction);		
	void ActionUpBehaviorHandler(RwUInt32 uiAction);		
	void ActionUpGuiHandler(RwUInt32 uiAction);			

	void RegisterFlagMap();
	RwBool IsDownAction(RwUInt32 uiAction);
	RwBool SetFlagAction(RwUInt32 uiAction, RwBool bDown);

	RwBool KeyReference( RwUInt8 byChar, RwBool bDown = TRUE);

private:
	static CInputActionMap* m_pInstance;

	INPUT_HANDLE m_hKeyDown;
	INPUT_HANDLE m_hKeyUp;
	INPUT_HANDLE m_hMouseDown;
	INPUT_HANDLE m_hMouseUp;

	CNtlCallbackParam1 *m_pCallKeyboardMove;		///< Keyboard movement
	CNtlCallbackParam1 *m_pCallKeyboardDashMove;	///< Keyboard dash movement
	CNtlCallbackParam0 *m_pCallAvatarSelect;		///< Select avatar
	CNtlCallbackParam1 *m_pCallJump;				///< jump
	CNtlCallbackParam1 *m_pCallCharging;			///< Collect energy
	CNtlCallbackParam1 *m_pCallBlocking;			///< block mode
	CNtlCallbackParam1 *m_pCallSitAndStand;			///< sit down and stand up
	CNtlCallbackParam0 *m_pCallAutoRun;				///< Auto run
	CNtlCallbackParam1 *m_pCallSkillQuickSlotDown;	///< Quick Slot Download
	CNtlCallbackParam1 *m_pCallSkillQuickSlotUp;	///< Quick slot up
	CNtlCallbackParam1 *m_pCallSkillQuickSlotExDown;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotExUp;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotEx2Down;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotEx2Up;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotChange;///< Quick Slot PREV, NEXT
	CNtlCallbackParam0 *m_pCallExit;				///< Exit
	CNtlCallbackParam0 *m_pCallLooting;				///< Pick up items
	CNtlCallbackParam1 *m_pCallDialogAction;		///< Action to send to Dialog
	CNtlCallbackParam0 *m_pCallTabKey;				///<Tab actions
	CNtlCallbackParam0 *m_pCallAutoTarget;			///< Select a nearby enemy
	CNtlCallbackParam0 *m_pCallAutoAttack;			///< Auto attack
	CNtlCallbackParam0 *m_pCallAutoFollow;			///< Auto follow
	CNtlCallbackParam1 *m_pCallPartySelect;			///< Select party member
	CNtlCallbackParam0 *m_pCallPartyAutoSelect;		///< Automatic selection of party members
    CNtlCallbackParam0 *m_pCallScouterUse;          ///< Using Scouter (by agebreak)
	CNtlCallbackParam1 *m_pCallTargetMarking;		///< Target marking
	CNtlCallbackParam1 *m_pCallTargetSelecting;		///< Target selection
	CNtlCallbackParam1 *m_pCallChatPageChange;			///< Chat page (0 UP 1 DOWN)
	

	RwUInt32	m_uiKey1MoveValidFlags;
	RwUInt32	m_uiKey2MoveValidFlags;
	RwUInt32	m_uiMoveFlags;						///< Move Flags used in the client
	RwBool		m_bRBtnDown;
	RwUInt32	m_uiServerMoveFlags;				///< Move Flags sent to the server
	RwBool		m_bActive;							///< Whether InputActionMap is working
	RwBool		m_bAcceptServer;					///< Status applicable to server

	typedef std::map<RwUInt32, RwBool> FLAGMAP;		///< Actions that need to be taken to restore the state
	FLAGMAP m_mapFlag;

	typedef std::map<RwUInt8, RwUInt8>	IGNOREMAP;	///< Map of ignored keys
	IGNOREMAP m_mapIgnore;

	typedef std::map<RwUInt16, RwUInt8> SPECMAP;	///< Action that requires a specific action
	SPECMAP	m_mapSpecialAction;

	typedef std::map<RwUInt8, RwUInt8> UPDOWNREF;	///< Check the number of times the key goes up/down.
	UPDOWNREF m_mapUpdownRef;

	SInputDashMap m_sFrontDashMap;
	SInputDashMap m_sLeftDashMap;
	SInputDashMap m_sRightDashMap;
	SInputDashMap m_sBackDashMap;

	// Action Map class (ActionMap.h) by Kell
	CActionMapManager m_ActionMapManager;

public:

	template <class Callbackclass>	
	void LinkKeyboardMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkKeyboardMove(void);

	template <class Callbackclass>
	void LinkKeyboardDashMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkKeyboardDashMove(void);

	template <class Callbackclass>
	void LinkAvatarSelect(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAvatarSelect(void);

	template <class Callbackclass>
	void LinkJump(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkJump(void);

	template <class Callbackclass>
	void LinkCharging(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiCharging));
	void UnLinkCharging(void);

	template <class Callbackclass>
	void LinkBlocking(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiBlocking));
	void UnLinkBlocking(void);

	template <class Callbackclass>
	void LinkSitAndStand(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int bKeyDown));
	void UnLinkSitAndStand(void);

	template <class Callbackclass>
	void LinkAutoRun(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoRun(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotDown(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotUp(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotExDown(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotExDown(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotExUp(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotExUp(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotEx2Down(Callbackclass* cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotEx2Down(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotEx2Up(Callbackclass* cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotEx2Up(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotChange));
	void UnLinkSkillQuickSlotChange(void);

	template <class Callbackclass>
	void LinkGameExit(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkGameExit(void);

	template <class Callbackclass>
	void LinkLooting(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkLooting(void);

	template <class Callbackclass>
	void LinkDialogHotKey(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKey));
	void UnLinkDialogHotKey(void);

	template <class Callbackclass>
	void LinkTabKey(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkTabKey(void);

	template <class Callbackclass>
	void LinkAutoTarget(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoTarget(void);

	template <class Callbackclass>
	void LinkAutoAttack(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoAttack(void);

	template <class Callbackclass>
	void LinkAutoFollow(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoFollow(void);

	template <class Callbackclass>
	void LinkPartySelect(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkPartySelect(void);

	template <class Callbackclass>
	void LinkPartyAutoSelect(Callbackclass *cbclass, int (Callbackclass::*callback)(void));
	void UnLinkPartyAutoSelect(void);

    template <class Callbackclass>
    void LinkScouterUse(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
    void UnLinkScouterUse(void);

	template <class Callbackclass>
	void LinkActionMapUpdate(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiAction, unsigned int iChange));
	void UnLinkActionMapUpdate(void);

	template <class Callbackclass>
	void LinkTargetMarking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkTargetMarking(void);

	template <class Callbackclass>
	void LinkTargetSelecting(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkTargetSelecting(void);

	template <class Callbackclass>
	void LinkChatPageChange(Callbackclass *cbclass,int ( Callbackclass::*callback)(unsigned int uiOption));
	void UnLinkChatPageChane(void);
};

static CInputActionMap* GetInputActionMap(void)
{
	return CInputActionMap::GetInstance(); 
}


template <class Callbackclass>	
void CInputActionMap::LinkKeyboardMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int)) //<-uiMoveFalgs??? 
{
	NTL_PRE(m_pCallKeyboardMove == 0);
	m_pCallKeyboardMove = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkKeyboardDashMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs)) //?
{
	NTL_PRE(m_pCallKeyboardDashMove == 0);
	m_pCallKeyboardDashMove = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAvatarSelect(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallAvatarSelect == 0);
	m_pCallAvatarSelect = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}


template <class Callbackclass>
void CInputActionMap::LinkJump(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs))
{
	NTL_PRE(m_pCallJump == 0);
	m_pCallJump = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkCharging(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiCharging))
{
	NTL_PRE(m_pCallCharging == 0);
	m_pCallCharging = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkBlocking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiBlocking))
{
	NTL_PRE(m_pCallBlocking == 0);
	m_pCallBlocking = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSitAndStand(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int bKeyDown))
{
	NTL_PRE(m_pCallSitAndStand == 0);
	m_pCallSitAndStand = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoRun(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallAutoRun == 0);
	m_pCallAutoRun = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotDown == 0);
	m_pCallSkillQuickSlotDown = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotUp == 0);
	m_pCallSkillQuickSlotUp = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotExDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotExDown == 0);
	m_pCallSkillQuickSlotExDown = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotExUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotExUp == 0);
	m_pCallSkillQuickSlotExUp = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotEx2Down(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotEx2Down == 0);
	m_pCallSkillQuickSlotEx2Down = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotEx2Up(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotEx2Up == 0);
	m_pCallSkillQuickSlotEx2Up = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotChange))
{
	NTL_PRE(m_pCallSkillQuickSlotChange == 0 );
	m_pCallSkillQuickSlotChange = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass, callback);
}

template <class Callbackclass>
void CInputActionMap::LinkGameExit(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallExit == 0);
	m_pCallExit = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkLooting(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallLooting == 0 );
	m_pCallLooting = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkDialogHotKey(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKey))
{
	NTL_PRE( m_pCallDialogAction == 0 );
	m_pCallDialogAction = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTabKey(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallTabKey == 0 );
	m_pCallTabKey = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoTarget(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoTarget == 0 );
	m_pCallAutoTarget = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoAttack(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoAttack == 0 );
	m_pCallAutoAttack = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoFollow(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoFollow == 0 );
	m_pCallAutoFollow = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkPartySelect(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallPartySelect == 0 );
	m_pCallPartySelect = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkPartyAutoSelect(Callbackclass *cbclass, int (Callbackclass::*callback)(void))
{
	NTL_PRE( 0 == m_pCallPartyAutoSelect );
	m_pCallPartyAutoSelect = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkScouterUse(Callbackclass *cbclass, int (Callbackclass::*callback)(void))
{
    NTL_PRE( 0 == m_pCallScouterUse );
    m_pCallScouterUse = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTargetMarking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallTargetMarking == 0 );
	m_pCallTargetMarking = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTargetSelecting(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallTargetSelecting == 0 );
	m_pCallTargetSelecting = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkChatPageChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiOption))
{
	NTL_PRE( m_pCallChatPageChange == 0 );
	m_pCallChatPageChange = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

#endif
