/******************************************************************************
*File			: RaceExplainGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2006. 12. 8
*Abstract		: 
*****************************************************************************
*Desc			:Show descriptions of races in the lobby
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


class CRaceExplainGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CRaceExplainGui(const RwChar* pName);
	virtual ~CRaceExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetRace(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CRaceExplainGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< background

	CSurfaceGui			m_srtRaceNameBack;	///< Race name background

	gui::CStaticBox*	m_pTitleStatic;

	gui::CStaticBox*	m_pRaceName;		///< Race name
	gui::CHtmlBox*		m_pExplainHtml;		///< Race description
};