/*****************************************************************************
 *
 *File			: AvatarController.h
 *Author	    : HongHoDong
*Copyright	    : NTL Co., Ltd.
 *Date			: 2005. 11. 13	
 *Abstract		: NTl CAvatarController
 *****************************************************************************
 *Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_AVATAR_CONTROLLER__
#define __NTL_AVATAR_CONTROLLER__

#include "NtlCamera.h"
#include "NtlGameCamera.h"
#include "InputHandler.h"
#include "ceventhandler.h"

#include "NtlSLEvent.h"
#include "NtlSobActor.h"
#include "NtlSLEventHandler.h"
#include "NtlDebug.h"
#include "NtlDeviceRestoreHandler.h"
#include "NtlPLPicking.h"
#include "DboDef.h"


enum ECameraRotateFlag
{
	CAMERA_ROTATE_FLAG_NONE,	
	CAMERA_ROTATE_FLAG_ADJUST,	
	CAMERA_ROTATE_FLAG_ACCORD,	
};

/**
* \ingroup Client
*Updates the camera with input from the keyboard and mouse, and controls the movement and rotation of the camera.
*It has the function of sending an event to the Avatar. And the Keyboard and Mouse
*At the same time, it also handles when pressed and events for ActionMap are also sent here.
* 
*/
class CAvatarController : public RWS::CEventHandler
{
public:

	CAvatarController();
	~CAvatarController();

	RwBool	Create();
	void	Destroy();

	void Update(RwReal fElapsed);
	void HandleEvents(RWS::CMsg &pMsg);
	
	/**
	* mouse and keyboard hander Function.	
	*/

	int	MouseWheelHandler(unsigned int uiMouseData);

	int	MouseDownHandler(unsigned int uiMouseData);
	int	MouseUpHandler(unsigned int uiMouseData);

	int	KeyboardDownHandler(unsigned int uiKeyData);
	int KeyboardUpHandler(unsigned int uiKeyData);
	
	int RestoreHandler(void);		///< Device Restore processing function
		
	/**
	*Input action map processing function.
	*/ 
	int ActionMapKeyboardMove(unsigned int uiMoveFlags);
	int ActionMapKeyboardDashMove(unsigned int uiMoveFlags);
	int ActionAvatarSelect(void);
	int ActionJump(unsigned int uiMoveFlags);
	int ActionCharging(unsigned int uiCharging);
	int	ActionBlocking(unsigned int uiBlocking);
	int ActionSitAndStand(unsigned int bKeyDown);
	int ActionLooting(void);
	int ActionAutoTarget(void);
	int ActionAutoAttack(void);
	int ActionAutoFollow(void);
	int ActionPartySelect(unsigned int uiIdx);
    int ActionAutoRun();
    int ActionScouterUse(void);
    int ActionPartyShareTargetMarking(unsigned int uiSlot);
    int ActionPartyShareTargetSelect(unsigned int uiSlot);
    int ActionPartyAutoSelect();

private:

	void TeleportReset(void);

	/**
	*picking pl world entity
	*/
	void PickPLWorld(SWorldPickInfo& sPickInfo);

	/**
	*picking pl character entity
	*/
	void PickPLObject(SWorldPickInfo& sPickInfo, int someEnum, RwInt32 nXPos, RwInt32 nYPos);
	void PickPLObject(SWorldPickInfo& sPickInfo);

	/**
	*It is necessary to immediately change the direction of the avatar by referring to the camera direction, by changing the default move flag.
	*/
	RwUInt8 ConvertStraightMoveFlags(RwUInt8 uiMoveFlags);

	/**
	*Rotate the avatar directly with reference to the camera direction (in combination with the current move flag).
	*/
	RwReal CalcAvatarStraightRotate(RwUInt32 uiMoveFlags);

	/**
	*Find the camera rotation sensitivity.
	*/
	void CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY);

	/**
	*When no mouse button is pressed and the character moves front and back using the keyboard
	*Calculate the delta angle to rotate the camera behind the character.
	*/
	void CalcCameraRotateToCharBack(void);

	/**
	*When no mouse button is pressed and the character moves front and back using the keyboard
	*Rotate the camera behind the character.
	*/
	void UpdateCameraRotateToCharBack(RwReal fElapsed);

	/**
	*In case of auto run.
	*/
	void UpdateAutoRun(RwReal fElapsed);

	/**
	*move mark effect create function.
	*/
	void CreateMoveMark(const RwV3d *pPos);

	/**
	*move mark effect delete function.
	*/
	void DeleteMoveMark(void);

	/**
	*move mark effect update function.
	*/
	void UpdateMoveMark(RwReal fElapsed);

	/*
	*/
	void UpdateMouse(RwReal fElapsed);
	
	/**
	*move mark effect update function.
	*/
	void UpdateChangeHeading(RwReal fElapsed);

	/*
		*change x angle while flying
	*/
	void UpdateFlightAngle(float fElapsed);

	/**
	*picking focus update function.
	*/
	void UpdatePickFocus(RwReal fElapsed);

	/**
	*Processing function when mouse left button up.
	*/
	void MouseUpLBtnHandler(const SMouseData *pData);

	/**
	*Processing function when mouse right button up.
	*/
	void MouseUpRBtnHandler(const SMouseData *pData);

	/**
	*Processing function when mouse move left button down.
	*/
	void MouseMoveLBtnHandler(int iPosX, int iPosY);

	/**
	*Processing function when mouse move right button down.
	*/
	void MouseMoveRBtnHandler(int iPosX, int iPosY);


	/*
	/ put mouse cursor to the middle
	*/
	void			ResetMousePosition();

private:

	bool			m_bHideMouse;

	// Picking
	SWorldPickInfo	m_sPickInfo;
	sWorldPickInfo_for_Cursor	m_sPickInfo_for_Cursor;

	RwReal			m_fPickTime;

	// Effect
	RwReal			m_fMoveMarkTime;
	CNtlPLEntity	*m_pMoveMark;
	RwUInt8	    	m_uiCurMoveFlags;	 // not used when we use autorun
	RwBool			m_bAutoRun;
	RwBool			m_bHTBRPStock;

	// Looting
	RwReal			m_fKeyLootingTime;

	ECameraRotateFlag m_eRotFlag;

	bool			m_bMouseAutoRun;

	RwBool			m_bLButtonMoveFlag;
	RwInt32			m_iOldLButtonMouseX;
	RwInt32			m_iOldLButtonMouseY;

	bool			m_bLButtonDownFlag;
	bool			m_bRButtonDownFlag;

	RwInt32			m_iDeltaLButtonMouseX;
	RwInt32			m_iDeltaLButtonMouseY;
	RwInt32			m_iDeltaRButtonMouseX;
	RwInt32			m_iDeltaRButtonMouseY;

	RwBool			m_bRButtonMoveFlag;
	RwInt32			m_iOldRButtonMouseX;
	RwInt32			m_iOldRButtonMouseY;

	RwBool			m_bChangeHeadingFlag;
	RwBool			m_bAutoRunKeyboardChangeHeading;
	RwBool			m_bSkipMouseWheel;
	RwReal			m_fChangeHeadingUpdateTime;
	RwReal			m_fChangeHeadingDeltaTime;
	RwReal			m_fChangeHeadingDeltaAngleY;
	RwReal			m_fChangeHeadingDeltaAngleX;

	// changle angleX with keyboard
	bool			m_bChangeAngleX;
	bool			m_bChangeAngleXUp;

	SERIAL_HANDLE   m_hFocusSerialId;

	INPUT_HANDLE	m_handleMouseWheel;
	INPUT_HANDLE	m_handleMouseDown;
	INPUT_HANDLE	m_handleMouseUp;
	INPUT_HANDLE	m_handleKeyDown;
	INPUT_HANDLE	m_handleKeyUp;

	// device restore
	DEVICE_RESTORE_HANDLE m_handleRestoreHandler;

private:

	void SetAutoRun(RwBool bAutoRun);
};

#endif
