/******************************************************************************
*File			: CharCreateGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2006. 12. 8
*Update		    : 2007. 10. 11
*Abstract		:
*****************************************************************************
*Desc			: CCharSelectGui
*****************************************************************************/

#pragma once

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

class CCharSelecterGui;
class CChannelSelectGui;
class CCharacterInfoGui;
class CAccountGui;


class CCharSelectGui : public CNtlPLGui, public RWS::CEventHandler
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
	CCharSelectGui(const RwChar* pName);
	virtual ~CCharSelectGui();

	RwBool		Create();
	VOID		Destroy();

	RwUInt8		GetSelectedChannelIndex();

	// Functions for GUI control in the character stage (lobby)
	VOID		SwitchDialog(bool bShow);
	VOID		EnableButtons(bool bEnable);

protected:
	CCharSelectGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);

	VOID			OnPress_LeftTurnButton(gui::CComponent* pComponent);
	VOID			OnPress_RightTurnButton(gui::CComponent* pComponent);
	VOID			OnRelease_TurnButton(gui::CComponent* pComponent);
	VOID			OnClickedGameStartButton(gui::CComponent* pComponent);
	VOID			OnClickedExitButton(gui::CComponent* pComponent);
	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseUp(const CKey& key);

protected:
	gui::CSlot			m_slotPressLeftButton;
	gui::CSlot			m_slotReleaseLeftButton;
	gui::CSlot			m_slotPressRightButton;
	gui::CSlot			m_slotReleaseRightButton;
	gui::CSlot			m_slotGameStartButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseUp;

	gui::CButton*		m_pLeftRotButton;	///< Left rotation button
	gui::CButton*		m_pRightRotButton;	///< Rotate right button
	gui::CButton*		m_pGameStartButton;	///< Game start button
	gui::CButton*		m_pExitButton;		///< Exit button

	CAccountGui*		m_pAccountGui;		///< Account information GUI
	CCharacterInfoGui*	m_pCharacterInfoGui;///< Character information GUI
	CCharSelecterGui*	m_pCharSelecterGui;	///< Character Selector GUI
	CChannelSelectGui*	m_pChannelSelectGui;///< Channel Select GUI

	RwUInt8				m_byRotationState;	///< Character rotation status

	sMOUSE_ROL			m_tMouseRol;		///< Rotate character using mouse
};