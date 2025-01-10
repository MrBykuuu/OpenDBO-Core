/******************************************************************************
*File           : CharAccountGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 15, 2007
*Abstract		:
*****************************************************************************
*Desc           : Displays account information
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "Inputhandler.h"

// Dbo
#include "Windowby3.h"

class CAccountGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CAccountGui(const RwChar* pName);
	virtual ~CAccountGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CAccountGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			SetServerInfo();
	VOID			SetServerState(RwUInt8 byState, RwUInt8 byRate);

	VOID			EnableButtons(bool bEnable);

	VOID			OnClicked_ServerSelectButton(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotSelectServer;
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< background

	CSurfaceGui			m_srtAccountBack;	///< Account information background

	gui::CStaticBox*	m_pTitleStatic;

	gui::CStaticBox*	m_pServerNameStatic;///< 'server name'
	gui::CStaticBox*	m_pServerStateStatic;///< 'Server status'	
	gui::CStaticBox*	m_pServerName;		///< server name
	gui::CStaticBox*	m_pServerState;		///< Server status

	gui::CButton*		m_pServerSelectButton;///< Server selection button
};