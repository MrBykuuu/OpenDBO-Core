/*****************************************************************************
 *
 *File			: CharacterStage.h
 *Author	    : HyungSuk, Jang
 *Copyright	    : NTL Co., Ltd.
 *Date			: 2006. 11. 28	
 *Abstract		: Character stage class
 *****************************************************************************
 *Desc          : 
 *
 *****************************************************************************/

#ifndef __CHARACTER_STAGE_H__
#define __CHARACTER_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLSky.h"
#include "NtlPLCharacter.h"
#include "CharGuiGroup.h"
#include "NtlSobAvatar.h"

#include "DboEvent.h"
#include "InputHandler.h"


class CLobbyStage : public CNtlStage, public RWS::CEventHandler
{
#define dCAMERA_STEP_DISTANCE	0.1f
#define dCAMERA_MOVE_TIME		0.1f

	struct sCameraData
	{
		RwV3d		v3Pos;
	};

	enum eCharHeightType
	{
		CHT_CHILD,
		CHT_ADULT,

		NUM_CHT
	};

	enum eZoomLevel
	{
		ZL_ZOOM_OUT,
		ZL_ZOOM_IN,

		NUM_ZL
	};

	struct sCameraZoom
	{
		RwReal				fElapsed;
		RwV3d				v3CameraSrcPos;
		RwV3d				v3CameraDestPos;
		RwV3d				v3CameraSrcDir;
		RwV3d				v3CameraDestDir;

		sCameraZoom()
		:fElapsed(0.f)
		{}
	};

private:
	CNtlPLWorldEntity		*m_pWorldEntity;						// Main World
	
	RwV3d					m_vCameraPos;							// Camera Position
	RwV3d					m_vCameraPosAt;							// Where the camera sees							
	RwV3d					m_vAvatarPos;							// Location of Avatar
	RwV3d					m_vAvatarUpdatePos;

	RwReal					m_fAvatarAngleY;						// Avatar's Y axis rotation value

	CNtlSobAvatar			*m_pAvatar;								// Avatar
	RwInt32					m_iAvatarRotState;						// Avatar Rotation State
	RwReal					m_fRotSpeedPerSec;						// Avatar Rotation Speed

	CCharGuiGroup			*m_pGuiGroup;							// Gui Group
	RwBool					m_bWorldAddCamera;						// World Add/Remove
		
	RwInt8					m_byRace;
	RwInt8					m_byCharHeightType;

	// Saves all wheel messages for natural response to mouse wheel scrolling
	std::list<sCameraZoom>	m_listZoomMode;
	RwReal					m_fReservatedZoomStep;
	RwReal					m_fTotalZoomStep;
	RwReal					m_fTotalDistance;
	RwReal					m_fTotalDirDistance;

	sCameraData				m_CameraData[RACE_COUNT + 1][NUM_CHT][NUM_ZL];
	RwV3d					m_v3Dir[RACE_COUNT + 1][NUM_CHT][NUM_ZL];

	RwBool					m_bOptionApply;							// When creating a CLobbayStage object, it has the value FALSE only once.

private:
	INPUT_HANDLE m_hKeyDown;

private:
	void CheckDeletedCharacter();

	int	KeyboardDownHandler(unsigned int uiKeyData);

	// Process network
	void RequestLobbyInfo();

	// event handler
	void EventProcUpdateTick(RwReal fElapsed);
	
	// Change Create
	void CreateDummyAvatar( BYTE byRace, 
		                    BYTE byClass, 
							BYTE byGender, 
							BYTE byFace, 
							BYTE byHair, 
							BYTE bySkinColor, 
							BYTE byHairColor);

	void CreateDownLoadAvatar( sPC_SUMMARY *pCharInfo);

	void SetupDefaultCamerabyRace(RwUInt8 byRace);							//Initialize Avatar location and Camera location
	void CreateBackGroundStage( RwUInt32 uiRace, RwBool bForce = false);	//Create a World (created only once)
	void TeleportStage();													//Teleport to the location set by SetUpDefaultCamerabyRace
																			//Loading occurs when teleporting.

	void DestroyBackGroundStage();
	void DestroyAvatar();

public:
	CLobbyStage(const char *pStageName);
	~CLobbyStage();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:
	void OnTimer(void);
}; 

#endif