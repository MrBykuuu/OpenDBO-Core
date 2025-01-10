/******************************************************************************
*File           : DBCSideIcon.h
*Author         : agebreak
*Copyright      : NTL Co., Ltd.
*Date           : July 27, 2007
*Abstract		:
*****************************************************************************
*Desc           : Side icon related to dragon ball collection
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"
#include "NtlSLEvent.h"

// Share
#include "NtlTimeQuest.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "Windowby3.h"
#include "SideIconGui.h"

class CDBCSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CDBCSideIconGui(const RwChar* pName);
	virtual ~CDBCSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID				SetIconImageType(eDBCSideIconType eType);
	eDBCSideIconType	GetIconImageType();
	RwBool			IsCollectComplete();			///< Check whether all Dragon Balls have been collected.
	VOID			ResetSheduleInfo();				///< Returns the state to the state received from the server. (Call after using Dragon Ball)

protected:			
	gui::CSlot			m_slotNoSeasonButton;
	gui::CSlot			m_slotSeasonButton;
	gui::CSlot			m_slotCollectButton;

	gui::CButton*		m_pBtnNoSeason;			///< Display icon outside of Dragon Ball collection period
	gui::CButton*		m_pBtnSeason;			///< Display icon during Dragon Ball collection period
	gui::CButton*		m_pBtnCollect;			///< Icon for when all Dragon Balls are collected

	eDBCSideIconType	m_eIconImageType;		
	RwBool				m_bDBCComplete;			///< Have you collected all the dragon balls?
};


class CDBCSideViewGui : public CSideViewBase, RWS::CEventHandler
{
public:
	CDBCSideViewGui(const RwChar* pName);
	virtual ~CDBCSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetState(eDBCSideIconType eState);
	VOID			SetNumber(RwInt32 iNumber);
	VOID			LocateComponent();

	VOID			OnExitClicked(gui::CComponent* pComponent);	
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnResize( RwInt32 iOldW, RwInt32 iOldH );
	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotCloseButton;	
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;

	CWindowby3			m_BackPanel;		///< background

	gui::CStaticBox*	m_pNotify;			///< Notification message
	gui::CStaticBox*	m_pTitle;			///< Side View Title	
	gui::CButton*		m_pExitButton;		///< Close button	
};