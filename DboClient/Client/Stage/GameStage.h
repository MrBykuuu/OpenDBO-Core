/*****************************************************************************
 *
 *File			: GameStage.h
 *Author	    : HyungSuk, Jang
*Copyright	    : NTL Co., Ltd.
 *Date			: 2005. 8. 17	
 *Abstract		: Game stage class
 *****************************************************************************
 *Desc         : 
 *
 *****************************************************************************/


#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#include "NtlStage.h"
#include "NtlSobAvatar.h"
#include "InputHandler.h"
#include "eventtimer.h"
#include "ceventhandler.h"
#include "NtlCharsetPrint.h"

#include "AvatarController.h"

class CNtlPLWorldEntity;
class CGameGuiGroup;
class CNtlGameCameraManager;
class CDboTSCMain;
class CGameStageState;
class CGameLoadingThread;
class CGameLoadingScene;
class CGameTeleportScene;

class CGameStage : public CNtlStage, public RWS::CEventHandler
{
public:

	enum EGameUpdateType
	{
		EGUT_GAME_ENTER_LOADING,
		EGUT_GAME_TELEPORT_LOADING,
		EGUT_GAME_IDLE
	};

private:

	CDboTSCMain			*m_pTSCMain;
	CNtlSobAvatar		*m_pAvatar;	
	CAvatarController	m_AvatarController;
	CGameGuiGroup		*m_pGuiGroup;
	CGameStageState		*m_pStageState;

	CGameLoadingThread	*m_pLoadingThread;	// thread loading 
	CGameLoadingScene	*m_pLoadingScene;
	CGameTeleportScene	*m_pTeleportScene;

	INPUT_HANDLE m_hKeyboardDown;
			
	// charset print
	CHARSET_HANDLE m_hCharsetAvatarPos;
	CHARSET_HANDLE m_hCharsetAvatarDir;
	CHARSET_HANDLE m_hCharsetAvatarFightingMode;
	CHARSET_HANDLE m_hCharsetAvatarAirMode;
	CHARSET_HANDLE m_hCharsetAvatarMoveSpeed;
	CHARSET_HANDLE m_hCharsetAvatarAttackSpeed;
    CHARSET_HANDLE m_hCharsetAvatarState;

	CHARSET_HANDLE m_hCharsetFieldIdx;
	CHARSET_HANDLE m_hCharsetSectorIdx;
	CHARSET_HANDLE m_hCharsetAvatarAngle;
	CHARSET_HANDLE m_hCharsetPlayerCount;
	CHARSET_HANDLE m_hCharsetMonsterCount;

	CHARSET_HANDLE m_hCharsetTargetLoc;
	CHARSET_HANDLE m_hCharsetTargetDir;
	CHARSET_HANDLE m_hCharsetTargetMoveSpeed;
	CHARSET_HANDLE m_hCharsetTargetAttackSpeed;
	CHARSET_HANDLE m_hCharsetTargetState;
	CHARSET_HANDLE m_hCharsetFightingMode;
	CHARSET_HANDLE m_hCharsetAirMode;

	// Woody1019
	CNtlPLWorldEntity* m_pWorldEntity;

	EGameUpdateType		m_eUpdageType;
	RwBool				m_bCheckResourceScheduling;
	RwBool				m_bWorldReady;
	RwBool				m_bAvatarReady;
	RwBool				m_bCharReadyCommunityServer;
	RwBool				m_bWorldChange;

public:
	// Multi thread loading
		// Function called by main thread
		// Purpose: Performs preliminary work before starting thread data loading.
	void PreMutiThreadLoading( void );
		// Function called by the data loading thread
		// Purpose: Perform data loading using threads.
	void MutiThreadLoading( void );
		// Function called by main thread
		// Purpose: Perform post-processing after loading thread data.
	void PostMutiThreadLoading( void );

	virtual void PostRender(void);

protected:

	// Woody1019
	void	CreateWorld(RwV3d& vAvatarPos);
	void	UpdateWorld(RwV3d& vAvatarPos);
	void	DeleteWorld();

	void	AvatarCreate(void);
	void	AvatarReCreate(void);
	RwBool	IsAvatarCreate(void);
	RwBool	IsAvatarReCreate(void);
	RwV3d	GetUpdateWorldPosition(void);
	void	LoadingEndProc(void);

	//----------------------------
	// update

	void	UpdateGameEnterLoading(RwReal fElapsed);
	void	UpdateGameIdle(RwReal fElapsed);
	void	UpdateGameTeleportLoading(RwReal fElapsed);

	void	UpdateLoadingScene(RwReal fElapsed);
	RwBool	UpdateLoadingThread(RwReal fElapsed);
	void	UpdateGameEnterLoadingSchedulingWorld(RwReal fElapsed);

	void	UpdateTeleportLoadingNoneScene(RwReal fElapsed);
	void	UpdateTeleportLoadingReadyScene(RwReal fElapsed);
	void	UpdateTeleportLoadingTeleportScene(RwReal fElapsed);
	void	UpdateTeleportLoadingLoadEndScene(RwReal fElapsed);
	void	UpdateTeleportLoadingSpawnReadyScene(RwReal fElapsed);
	void	UpdateTeleportLoadingLoadFinishScene(RwReal fElapsed);

    void    UpdateSoundListener();                              ///< Set the sound listener according to the situation.
	
	void	UpdateDebugDisplay(RwReal fElapsed);

	// event handler
	void	EventProcUpdateTick(RwReal fElapsed);
	void	EventProcWorldChange(RwBool bWorldChange);

	void	ResetGui();

public:

	// avooo
	RwBool	IsActiveLoadingScene();
	
public:

	CGameStage(const char *pStageName);
	~CGameStage();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

	// Input
	int KeyboardDownHandler(unsigned int uiKeyData);
};

#endif