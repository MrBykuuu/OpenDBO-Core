/******************************************************************************
*File           : CCommunityGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : September 13, 2006
*Abstract		: 
*****************************************************************************
*Desc           : Community-related dialog template
*                 The content of each page is set according to PageType
*                 Messages and events are also delivered to the activated Page.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Gui
#include "gui_staticbox.h"
#include "gui_inputbox.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SlotGui.h"
#include "Windowby3.h"

class CCommunityBase;

class CCommunityGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	enum eCommunityPageType
	{
		PAGETYPE_GUILD,
		//PAGETYPE_RAID,

		NUM_PAGE,

		INVALID_PAGE
	};
	
	CCommunityGui(const RwChar* pName);
	virtual ~CCommunityGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

	VOID		SetPageType(RwInt32 iPageType);			///< Activates the page.
	RwInt32		GetPageType();							///< Returns the active page index

	CHARACTERID	GetGuildSelectedID();

protected:
	CCommunityGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetupPage(RwInt32 iPage);

	VOID			CloseButtonClicked(gui::CComponent* pComponent);	///<Pressed the close button.
	VOID			OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);	///< Pressed the tab button.

	VOID			OnPaint();

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);
	VOID			OnClickedBtnHelp( gui::CComponent* pComponent );

protected:	
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;	
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCloseButton;

	gui::CStaticBox*	m_pDialogName;				///< Dialog name static
	gui::CButton*		m_pExitButton;				///< Close window button
	CWindowby3			m_BackLineSurface;			///< Backline

	gui::CPanel*		m_pDumyPanel;				///< dummy

	gui::CTabButton*	m_pTabButton;				///< Tab button

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	RwInt32				m_iPageType;				///< Current page type

	///< Pages for each tab
	CCommunityBase*		m_pTabPage[NUM_PAGE];		///< GUI of each tab
};