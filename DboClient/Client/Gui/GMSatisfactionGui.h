/******************************************************************************
*File           : GMChatGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 21, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Chat dialog with GM
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

class CGMChatGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGMChatGui(const RwChar* pName);
	virtual ~CGMChatGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	// avooo's comment: GM chat is not what the game is about and there are already other dialogs.
	//					 In a crowded situation, it is impossible to make a permanent spot for GM chat.
	//					 GM chat allows users to change their location at any time.
	virtual void	SetMovable(RwBool bMovable) {}

	VOID		SetGMText(const WCHAR* pwcText);

protected:
	CGMChatGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnGotFocus();
	VOID			OnLostFocus();
	VOID			OnInputReturn();
	VOID			OnIMEChange( RwInt32 uiIMEState );
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);

protected:
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
	gui::CSlot			m_slotReturn;
	gui::CSlot			m_slotIMEChange;
	gui::CSlot			m_slotCaptureWheelMove;

	gui::COutputBox*	m_pOutDisplay;
	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pIME;
	gui::CInputBox*		m_pInput;
};