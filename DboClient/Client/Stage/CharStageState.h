
/*****************************************************************************
 *
 *File			: CharStageState.h
 *Author	    : HyungSuk, Jang
 *Copyright	    : NTL Co., Ltd.
 *Date			: 2006. 12. 11	
 *Abstract		: Character stage state
 *****************************************************************************
 *Desc          : 
 *
 *****************************************************************************/

#ifndef __CHARACTER_STAGE_STATE_H__
#define __CHARACTER_STAGE_STATE_H__

// Dbo
#include "DboDef.h"

class CBackboardGui;
class CCharCreateGuiGui;
class CCharSelectGui;
class CServerSelectGui;
class CCharGuiGroup;

class CCharStageState
{
protected:
	enum eChangeDelayTime
	{		
		CDT_WAITFOR_SERVER_REPLY			= 30, // Time to wait to receive a packet from the server (unit: seconds)

		CDT_IMMEDIATLY_RETURN_LOGIN_MESSAGE	= 3, // Time to display a message indicating that you will return to logging in immediately
	};

	static CCharStageState* m_pInstance;

	ECharacterStageState	m_eState;			///< Current status
	ECharacterStageState	m_eReservateState;	///< Reserved status

	CBackboardGui*			m_pTurnOverBackboard;
	CCharCreateGuiGui*		m_pCharCreateGui;
	CCharSelectGui*			m_pCharSelectGui;
	CServerSelectGui*		m_pServerSelectGui;

	RwReal					m_fElapedTime;
	
	RwBool					m_bReadyModel;		///< Model loading is complete
	RwBool					m_bReadyWorld;		///< Background loading is complete
	RwBool					m_bShowMessageBox;
	RwBool					m_bSendGameEnterPacket;

	typedef void (CCharStageState::*FuncUpdate)(RwReal fElapsed);
	FuncUpdate m_fnUpdate[CHAR_MAX_STATE];

private:
	
	void FuncUpdateInformation(RwReal fElapsed);
	void FuncUpdateServerEnter(RwReal fElapsed);
	void FuncUpdateServerIdle(RwReal fElapsed);
	void FuncUpdateServerChange(RwReal fElapsed);
	void FuncUpdateServerExit(RwReal fElapsed);
	
	void FuncUpdateMakeEnter(RwReal fElapsed);
	void FuncUpdateMakeIdle(RwReal fElapsed);
	void FuncUpdateMakeExit(RwReal fElapsed);
	void FuncUpdateMakeRequest(RwReal fElapsed);

	void FuncUpdateSelectInfomation(RwReal fElapsed);
	void FuncUpdateSelectInfomationFromServerList(RwReal fElapsed);
	void FuncUpdateSelectEnter(RwReal fElapsed);
	void FuncUpdateSelectIdle(RwReal fElapsed);
	void FuncUpdateSelectDelChar(RwReal fElapsed);
	void FuncUpdateSelectRecoveryChar(RwReal fElapsed);
	void FuncUpdateSelectRenameReq(RwReal fElapsed);
	void FuncUpdateSelectExit(RwReal fElapsed);

	void FuncUpdateReturnLogin(RwReal fElapsed);

	void FuncUpdateGameServerWaitCheckReq(RwReal fElapsed);
	void FuncUpdateCharacterSelectReq(RwReal fElapsed);
	void FuncUpdateSuccessEnterGame(RwReal fElapsed);
	void FuncUpdateWaitingGameServerResponse(RwReal fElapsed);
	void FuncUpdateFailEnter(RwReal fElapsed);

	void FuncUpdateImmediatelyReturnLoginEnter(RwReal fElapsed);
	void FuncUpdateImmediatelyReturnLoginIdle(RwReal fElapsed);
	void FuncUpdateImmediatelyReturnLoginExit(RwReal fElapsed);

	void FuncUpdateWaitforTutorial(RwReal fElapsed);
	void FuncUpdateReadyEnterTutorial(RwReal fElapsed);
	void FuncUpdateWaitCancel(RwReal fElapsed);

	void FuncUpdateWaitGameServerConnect(RwReal fElapsed);
	void FuncUpdateCancelWaitGameServerConnect(RwReal fElapsed);

	void FuncUpdateStateEnd(RwReal fElapsed);
	
	void EnterState(ECharacterStageState eState);
	void ExitState(ECharacterStageState eState);

	RwUInt8 UpdateAlpha(RwReal fElaped, RwBool bPlusAlpha);	

public:

	CCharStageState();
	~CCharStageState();

	static CCharStageState* GetInstance(void);


	static void CreateInstance();
	static void DestroyInstance();


	void RegisterGUI(CCharGuiGroup* pGuiGroup);

	void Update(RwReal fElapsed);

	void ReservateState(ECharacterStageState eState);
	void ChangeState(ECharacterStageState eState);

	ECharacterStageState GetCurrentState();	
	ECharacterStageState GetReservateState();	

	RwBool IsState(ECharacterStageState eState);

	void SetReadyModel(RwBool bReady) { m_bReadyModel = bReady; }
	void SetReadyWorld(RwBool bReady) { m_bReadyWorld = bReady; }

	RwUInt8 GetSelectedChannelIndex();
};

static CCharStageState* GetCharStageState(void)
{
	return CCharStageState::GetInstance();
}

#endif