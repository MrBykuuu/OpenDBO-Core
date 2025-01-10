/*****************************************************************************
 *
 * File			: Global.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 9. 14	
 * Abstract		: DBO global.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_GLOBAL_H__
#define __DBO_GLOBAL_H__

#include "set"
#include "NtlClientNet.h"
#include "NtlItem.h"
#include "NtlService.h"
#include "NtlPacketCU.h"
#include "NtlPacketGU.h"
#include "DboPacketHandler.h"
#include "DboPacketGenerator.h"
#include "NtlSLDef.h"
#include "NtlSlEvent.h"
#include "DBOUIConfig.h"

// Chatting filter
#include "SayFilter.h"

static DWORD sm_cameraMoveCount = 0;

typedef void (*CallbackLoadContract)(const RwChar* pFileName, void** pData, int* iSize);
extern CallbackLoadContract g_fnCallback_LoadContract;

void LinkLoadContract(CallbackLoadContract fn);
void UnLinkLoadContract();


class CNtlClientNet;
class CNtlSobAvatar;

#define dMAX_SIZE_DEV_FUNC_ARRAY							(DBO_ALLOWED_FUNC_FOR_DEV_COUNT/8 + 1)

// config data
struct SConfigData
{
	std::string strAddr;		// account server ip address (considering hard coding due to hacking problem?)
	DWORD dwPort;				// account server ip port (considering hard coding due to hacking problem?)
    std::string strBugTrapServerIP;         
    DWORD       dwBugTrapServerPort;
    std::string strBugTrapFolder;
	std::string strLocalize;

    SConfigData()
    {
        strBugTrapFolder = "DBOG Client";
    }
};

typedef struct _SConnectAtomic
{
	enum 
	{
		SOCKET_STATE_DISCONNECT,
		SOCKET_STATE_CONNECT,
		SOCKET_STATE_DISCONNECT_REQ
	};

	HSERVER		hSocket;
	RwUInt8		byConnState;
	RwChar		chServerIP[NTL_MAX_LENGTH_OF_IP+1];
	RwUInt16	wServerPort;
	RwChar		chAuthKey[NTL_MAX_SIZE_AUTH_KEY+1];
	RwBool		bBanishment;	///< Whether you have been subject to operational sanctions

	void Init(void)
	{
		hSocket			= INVALID_HSERVER;
		byConnState		= SOCKET_STATE_DISCONNECT;
		memset(chServerIP, 0, sizeof(RwChar)*(NTL_MAX_LENGTH_OF_IP+1));
		wServerPort		= 0;
		memset(chAuthKey, 0, sizeof(RwChar)*(NTL_MAX_SIZE_AUTH_KEY+1));
		bBanishment		= false;
	}

}SConnectAtomic;


typedef struct _SCharServerInfo
{
	RwChar		chServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	RwUInt16	wServerPort;
	RwUInt32	uiLoad;
}SCharServerInfo;

// network connect info
typedef struct _SConnectData
{
	SConnectAtomic sAuthCon;
	SConnectAtomic sCharCon;
	SConnectAtomic sGameCon;
	SConnectAtomic sChatCon;

	// character server type.
	typedef std::list<SCharServerInfo*> ListCharServer;
	ListCharServer listCharServer;

	//Uuid
	RwUInt32	uiAccountId;
	RwUInt8		byLastServerFarmID;

	RwBool		bChangingGameServer;

	// Dev func information
	RwChar			m_acAllowFuncForDev[dMAX_SIZE_DEV_FUNC_ARRAY];
	
	void			Init(void);
	void			Destroy(void);

	void			ResetCharacterServer(void);
	void			AddCharacterServer(const RwChar *pServerIP, RwUInt16 wServerPort, RwUInt32 uiLoad);
	RwBool			PopCharacterServer(SCharServerInfo *pInfo);

}SConnectData;


// user info
typedef struct _SUserData
{
	WCHAR wchUserID[NTL_MAX_SIZE_USERID+1];			// user id
	WCHAR wchPassword[NTL_MAX_SIZE_USERPW+1];		// user password

	void Init(void)
	{
		Clear();
	}

	void Clear(void)
	{
		memset(wchUserID, 0, sizeof(WCHAR)*(NTL_MAX_SIZE_USERID+1));
		memset(wchPassword, 0, sizeof(WCHAR)*(NTL_MAX_SIZE_USERPW+1));
	}

}SUserData;

// game data
typedef struct _SGameData
{
	RwBool			bChatMode;
	RwBool			bFirstWorldEnter;			///< Are you entering the world for the first time?
	RwBool			bInputActive;		
	RwBool			bDebugDisplay;
	SERIAL_HANDLE	hAskedPCSerial;				///< The handle of the PC that asked something
    SERIAL_HANDLE   hCommunityTargetSerial;     ///< Handle of the PC selected (targeted) at the community level
	RwBool			bAvatarReCreate;			///< When entering another physical game server, delete the avatar and create it again.

	void Init(void)
	{
		bChatMode			= FALSE;
		bFirstWorldEnter	= TRUE;
		bInputActive		= TRUE;
		bDebugDisplay = TRUE; // FALSE;
		hAskedPCSerial		= INVALID_SERIAL_ID;
        hCommunityTargetSerial = INVALID_SERIAL_ID;
		bAvatarReCreate		= FALSE;
	}

}SGameData;

struct sContractInfo
{
	char*			pcContract;
	WCHAR*			pwcContract;
	WCHAR*			pwcRealContract;
	RwInt32			uiContractVersion;
	RwBool			bAcceptGameContract;			///< Whether to accept the terms or not
};


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

class CDboGlobal
{
private:

	static CDboGlobal *m_pInstance;

private:

	/**
	*Screen size variable.
	*/
	RwUInt32		m_uiScreenWidth;				/** screen width */
	RwUInt32		m_uiScreenHeight;				/** screen height */

	/**
	*Game lobby brief information
	*/
	RwUInt8			m_byPostSelectedServerHandle;	/** Handle of the previously selected server */
	RwUInt8			m_bySelectedServerHandle;		/** Handle of currently selected server */
	RwUInt8			m_bySelectedCharIndex;			/** selected character index */

	/**
	*Game data processing variables.
	*/
	SUserData			m_sUser;					/** dbo user data */
	SConfigData			m_sConfig;					/** dbo config data */	
	SConnectData		m_sConnect;					/** dbo network connect data */		
	SGameData			m_sGame;					/** dbo game data */

	// Dragon Ball Collection Period Information Structure
	SNtlEventDBCSchedule_Info m_sDBCSheduleInfo;
	    
	/**
	*network lib variable.
	*/
	CNtlClientNet *m_pNetwork;						/** network lib */

	/**
	*Packet processing variables.
	*/

	CNtlNetSender *m_pLoginNetSender;					/** login network sender */
	CNtlNetSender *m_pLobbyNetSender;					/** lobby network sender */
	CNtlNetSender *m_pGameNetSender;					/** game network sender */
	CNtlNetSender *m_pChatNetSender;					/** chatting network sender */	

	CLoginPacketGenerator *m_pLoginPacketGenerator;		/** login packet generator */
	CLobbyPacketGenerator *m_pLobbyPacketGenerator;		/** lobby packet generator */
	CGamePacketGenerator *m_pGamePacketGenerator;		/** game packet generator */
	CChatPacketGenerator *m_pChatPacketGenerator;		/** chatting packet generator */

    CDBOUIConfig*   m_pDBOUIConfig;                     ///< DBO UI Config information object (2007.11.15 by agebreak)

	sContractInfo*	m_pContractInfo;
	RwBool			m_bEnterTutorial;					/** When entering the game by pressing the Cancel button on the tutorial entry interface, */
	RwBool			m_bDirectionFirstConnect;			/** Whether you want to show the production when you log in for the first time */

private:

	RwBool LoadConfigOptions(const RwChar *pFileName);

public:

	CDboGlobal();
	~CDboGlobal();

	static CDboGlobal* GetInstance(void);

	RwBool	Create(HWND hWnd);
	void	Destroy(void);

	void	Reset(void);		

public:

	void			SetScreenSize(RwUInt32 uiWidth, RwUInt32 uiHeight) { m_uiScreenWidth = uiWidth, m_uiScreenHeight = uiHeight; } 
	RwUInt32		GetScreenWidth(void) { return m_uiScreenWidth; }
	RwUInt32		GetScreenHeight(void) { return m_uiScreenHeight; }
	
	void			LoadingContractInfo();
			
	/**
	*Game data processing function.
	*/
	SUserData*		GetUserData(void)		{ return &m_sUser; }
	SConfigData*	GetConfigData(void)		{ return &m_sConfig; }
	SConnectData*	GetConnectData(void)	{ return &m_sConnect; }	
	SGameData*		GetGameData(void)		{ return &m_sGame; }

	// Dragon Ball Collection Information Processing Function
	void			SetDBCScheduleInfo(SNtlEventDBCSchedule_Info sDBCScheduleInfo) {m_sDBCSheduleInfo = sDBCScheduleInfo;}
	SNtlEventDBCSchedule_Info* GetDBCScheduleInfo() {return &m_sDBCSheduleInfo;}
	
	/**
	*Packet processing function.
	*/
	CNtlClientNet*			GetNetwork(void)					{ return m_pNetwork; }
	CLoginPacketGenerator*	GetLoginPacketGenerator(void)		{ return m_pLoginPacketGenerator; }
	CLobbyPacketGenerator*	GetLobbyPacketGenerator(void)		{ return m_pLobbyPacketGenerator; }
	CGamePacketGenerator*	GetGamePacketGenerator(void)		{ return m_pGamePacketGenerator; }
	CChatPacketGenerator*	GetChatPacketGenerator(void)		{ return m_pChatPacketGenerator; }

	/**
	* 
	*/
	
	void            SetCommunityTarget(SERIAL_HANDLE hSerial)   { m_sGame.hCommunityTargetSerial = hSerial;}
	SERIAL_HANDLE   GetCommunityTarget()                        { return m_sGame.hCommunityTargetSerial;}

	void			SetAskedPCSerial( SERIAL_HANDLE hSerial )	{ m_sGame.hAskedPCSerial = hSerial; }
	SERIAL_HANDLE	GetAskedPCSerial(void)						{ return m_sGame.hAskedPCSerial; }
	
	/**
	* debug display
	*/
	void			ToggleDebugDisplay( void )					{ m_sGame.bDebugDisplay = !m_sGame.bDebugDisplay; }
	RwBool			IsDebugDisplay(void)						{ return m_sGame.bDebugDisplay; }

    CDBOUIConfig*   GetDBOUIConfig()                            {return m_pDBOUIConfig;}        ///< Returns a DBO UI Config object.
	
	void			SetEnterTutorial( RwBool bEnter )			{ m_bEnterTutorial = bEnter; }
	RwBool			IsEnterTutorial()							{ return m_bEnterTutorial; }

	void			SetDirectionFirstConnect(RwBool bDirection) { m_bDirectionFirstConnect = bDirection; }
	RwBool			IsDirectionFirstConnect()					{ return m_bDirectionFirstConnect; }

	void			SetGameContract(RwBool bAccept);
	RwBool			IsAcceptGameContract();
	RwUInt32		GetContractVersion();
	WCHAR*			GetContract();

	RwBool			HaveFunction(eDBO_ALLOWED_FUNC_FOR_DEV eAllowFunc);
};

static CDboGlobal* GetDboGlobal(void)
{
	return CDboGlobal::GetInstance(); 
}

#endif

