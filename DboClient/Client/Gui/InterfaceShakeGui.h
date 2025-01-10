/*****************************************************************************
 *
 *File			: InterfaceShakeGui.h
 *Author	    : Haesung, Cho
 *Copyright	    : NTL Co., Ltd.
 *Date			: December 10, 2007	
 *Abstract		: DBO KnockDown Recovery UI (WAKE UP GUI).
 *****************************************************************************
 *Desc: 
 *1. When knocked down, the UI must appear.
 *2. A ProgressBar appears and when it is full, the knockdown is recovered. (at least 4 to 8 seconds)
 *3. When you shake the mouse left or right, the gauge must be full.
 *4. You must receive input from the left and right arrow keys on the keyboard. (InputActionMap)
 *5. It must be appropriate for the shaking speed of the mouse and the input speed of the keyboard.
 *
 *Todo: Currently the keyboard is excluded. If you say the keyboard will be included later, just delete all comments.
 *(including script)
 *****************************************************************************/

#ifndef __INTERFACE_SHAKE_H__
#define __INTERFACE_SHAKE_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
 
// Define
#define dSHAKE_MAX_WAKEUP 8.0f		// Maximum time for WakeUp UI
#define dSHAKE_MIN_WAKEUP 4.0f		// Minimum time for WakeUp UI

#define dSHAKE_MOVE_DISTANCE 0		// Minimum distance at which the mouse will respond

#define dSHAKE_MOUSE_LEFT_X		55		// Position of the mouse image to be moved when the mouse moves to the left
#define dSHAKE_MOUSE_RIGHT_X	70		// Position of the mouse image to be moved when the mouse moves to the right

#define dSHAKE_MOUSE_Y			48		// height of mouse

// File Locate
#define dSHAKE_FILE_SURFACE "gui\\InterfaceShakeGui.srf"
#define dSHAKE_FILE_FRAME "gui\\InterfaceShakeGui.frm"

/**
*Define the benefit time zone (the first hour must be less than the second hour)
*First time: +3
*Second time: +2
*Others: +1
*/
#define dSHAKE_MOUSE_FIRSTTIME 0.3f		// Mouse: The first time to benefit
#define dSHAKE_MOUSE_SECONDTIME 0.5f	// Mouse: The Second Time to Benefit
#define dSHAKE_KEYBOARD_FIRSTTIME 0.1f	// keyboard
#define dSHAKE_KEYBOARD_SECONDTIME 0.3f	// keyboard

/**
*Number of benefits
*/
#define dSHAKE_ADVANTAGE_FIRST	3		// within the first hour
#define dSHAKE_ADVANTAGE_SECOND	2		// within the second time
#define dSHAKE_ADVANTAGE_THIRD	1		// Other than that

/**
* \ingroup client
* \brief WakeUp GUI
*When a character in the game is knocked down, the user can move the mouse more quickly
*Allows knockdown status to be recovered.
*/
class CInterfaceShakeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CInterfaceShakeGui( const RwChar* pName );
	virtual ~CInterfaceShakeGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg &msg);
	RwInt32		SwitchDialog(bool bOpen);		///< Open/Close in DialogManager

	// Update
	VOID		Update( RwReal fElapsed );
	VOID		ProgressUpdateFromMouse();
	VOID		ProgressUpdateFromKeyboard();

	// Interface
	VOID		ShowInterface( VOID );			///< Run WakeUp Gui.
	VOID		CloseInterface( VOID );			///< Close.

	// Call back
	VOID		OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key);
	VOID		OnKeyUp( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key);
	
protected:
	gui::CProgressBar*	m_pPgbTrack;		///< Track
	gui::CPanel*		m_pPanTrack;		///< Track Panel
	gui::CFlash*		m_pFlashLeftArrow;	///< LeftArrow Flash
	gui::CFlash*		m_pFlashRightArrow;	///< RightArrow Flash
	gui::CFlash*		m_pFlashMouseBack;	///< MouseBack Flash
	gui::CButton*		m_pBtnLeftKey;		///< Left Key
	gui::CButton*		m_pBtnRightKey;		///< Right Key
	gui::CStaticBox*	m_pStbMouse;		///< Mouse Static Box
		
	gui::CSlot	m_slotKeyDown;				///< KeyDown Signal
	gui::CSlot	m_slotKeyUp;				///< KeyUp Signal

	RwInt32		m_nOldMouseX;				///< Old Mouse X coordinate
	RwInt32		m_nMouseX;					///< New Mouse X coordinates
	RwInt32		m_nOffsetX;					///< Distance moved by mouse X coordinate (-, +)
	RwInt32		m_nTrackValue;				///< Number of ProgressBar
	RwInt32		m_nTrackMax;				///< Maximum length of track
	RwInt32		m_nTrackMin;				///< Minimum length of track

	RwBool		m_bMouseToggle;				///< Left and right sides of mouse (TRUE, FALSE)
	RwBool		m_bKeyToggle;				///< Left, right of key (TRUE, FALSE)
	RwBool		m_bComplete;				///< Successful recovery

	RwReal		m_fElapsedTime;				///< Time taken by UI
	RwReal		m_fShakeElapsedTime;		///< Time taken to increase once with the mouse
	RwReal		m_fKeyDownElapsedTime;		///< Time taken to increase once with the keyboard
	RwReal		m_fElapsedShakeTime;		///< Shaking Time

	RwUInt32	m_dwLeftKey;				///< ActionMap left and right arrow keys
	RwUInt32	m_dwRightKey;
};
 
#endif