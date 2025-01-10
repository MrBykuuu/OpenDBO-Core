/******************************************************************************
*File           : ClassExplainGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 17, 2007
*Abstract		: 
*****************************************************************************
*Desc           : Shows a description of the class in the lobby
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


class CClassExplainGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CClassExplainGui(const RwChar* pName);
	virtual ~CClassExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetClass(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CClassExplainGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< background

	CSurfaceGui			m_srtClassNameBack;	///< class name background

	gui::CStaticBox*	m_pTitleStatic;
	gui::CStaticBox*	m_pClassName;		///< class name
	gui::CHtmlBox*		m_pExplainHtml;		///< Class description
};