/******************************************************************************
*File			: CharCreateGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2006. 12. 7
*Update		    : 2007. 10. 11
*Abstract		: 
*****************************************************************************
*Desc			: CharCreateGui
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Gui
#include "gui_button.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "Inputhandler.h"

// Dbo
#include "Windowby3.h"
#include "SlotGui.h"

class CCharMakePartGui;
class CRaceExplainGui;
class CClassExplainGui;


class CCharCreateGuiGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eRotation
	{
		ER_STOP,
		ER_TURN_LEFT,
		ER_TURN_RIGHT
	};

	struct sMOUSE_ROL
	{
		RwBool		bPressLeft;
		RwInt32		iOldX;
	};

public:
	CCharCreateGuiGui(const RwChar* pName);
	virtual ~CCharCreateGuiGui();

	RwBool		Create();
	VOID		Destroy();

	const WCHAR*	GetInputText();

	// Functions for GUI control in the character stage (lobby)
	VOID		SwitchDialog(bool bShow);

protected:
	CCharCreateGuiGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			EnableButtons(bool bEnable);
	VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);

	VOID			OnPress_LeftTurnButton(gui::CComponent* pComponent);
	VOID			OnPress_RightTurnButton(gui::CComponent* pComponent);
	VOID			OnRelease_TurnButton(gui::CComponent* pComponent);

	VOID			OnClick_MakeButton(gui::CComponent* pComponent);
	VOID			OnClick_ReturnButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID			OnCaptureMouseUp(const CKey& key);

	VOID			OnPaint();

public:
	gui::CSlot			m_slotPressLeftButton;
	gui::CSlot			m_slotReleaseLeftButton;
	gui::CSlot			m_slotPressRightButton;	
	gui::CSlot			m_slotReleaseRightButton;
	gui::CSlot			m_slotMakeButton;
	gui::CSlot			m_slotReturnButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;	
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseUp;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pLeftRotButton;	///< Left rotation button
	gui::CButton*		m_pRightRotButton;	///< Rotate right button
	gui::CButton*		m_pReturnButton;	///< Return button
	gui::CButton*		m_pMakeButton;		///< Create button
	gui::CButton*		m_pSaveButton;

	CSurfaceGui			m_srfNameBack;		///< Name Back

	gui::CStaticBox*	m_pNameStatic;		///< 'name'

	gui::CInputBox*		m_pNameInput;		///< Name input field

	CRaceExplainGui*	m_pRaceExplainGui;	///< Race Description Gui
	CClassExplainGui*	m_pClassExplainGui;	///< Class Description Gui
	CCharMakePartGui*	m_pCharMakePartGui;	///< A class that selects each part of the character

	RwUInt8				m_byRotationState;	///< Character rotation status

	sMOUSE_ROL			m_tMouseRol;
};