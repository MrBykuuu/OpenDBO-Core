/******************************************************************************
* File			: ChatOptionGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 7. 1
* Abstract		: 
*****************************************************************************
* Desc			: GUI that provides chat-related options
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "ChatDefine.h"
#include "ChatGui.h"

class CChatOptionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CChatOptionGui(const RwChar* pName);
	virtual ~CChatOptionGui();

	RwBool		Create(CChatGui* pChatGui);
	VOID		Destroy();

	VOID		EnableCatting(eChatType eType, BOOL bEnable);

	VOID		SwitchDialog(bool bOpen);

protected:
	CChatOptionGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_ChatTypeButton(gui::CComponent* pComponent);
	VOID			OnClicked_ToggleQuickButton(gui::CComponent* pComponent, bool bToggled);
	VOID			OnClicked_CommandButton(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotChatTypeButton[NUM_NET_CHAT_TYPE];
	gui::CSlot			m_slotToggleQuickButton[NUM_NET_CHAT_TYPE];
	gui::CSlot			m_slotCommand;

	gui::CButton*		m_pCloseButton;
	gui::CButton*		m_pChatTypeButton[NUM_NET_CHAT_TYPE];		///< Chat type selection button
	gui::CButton*		m_pToggleQuickButton[NUM_NET_CHAT_TYPE];	///< Quick button registration button
	gui::CButton*		m_pCommandButton;							///< Command button

	gui::CStaticBox*	m_pTitle;									///< Dialog title
	gui::CStaticBox*	m_pShoutcut[NUM_NET_CHAT_TYPE];				///< Show shortcut keys
	gui::CStaticBox*	m_pShoutcutExplain[NUM_NET_CHAT_TYPE];		///< Shortcut key description
	gui::CStaticBox*	m_pstbCommandShortcutExplain;
	gui::CStaticBox*	m_pCommand;			///< Command description

	CChatGui*			m_pChatGui;
};