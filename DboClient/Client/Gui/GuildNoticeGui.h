/*****************************************************************************
*File           : GuildNoticeGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : March 23, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Guild notice writing window
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "Windowby3.h"

class CGuildNoticeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eNOTICE_TYPE
	{
		NOTICE_TYPE_GUILD,
		NOTICE_TYPE_DOJO,
	};

public:
	CGuildNoticeGui(const RwChar* pName);
	virtual ~CGuildNoticeGui();

	virtual RwBool		Create(eNOTICE_TYPE eNoticeType);
	virtual VOID		Destroy();

	VOID				Active(bool bOpen);
	VOID				SetInputFocus();

protected:
	CGuildNoticeGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnReturn_NoticeInput();

	VOID			OnClicked_OKButton(gui::CComponent* pComponent);
	VOID			OnClicked_CancelButton(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotReturnInput;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;


	gui::CStaticBox*	m_pTitle;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pOKButton;
	gui::CButton*		m_pCancelButton;

	gui::CInputBox*		m_pNoticeInput;						///< Notice field

	CWindowby3			m_srfBackboard;						///< Background

	eNOTICE_TYPE		m_eNoticeType;
};