/******************************************************************************
*File           : CharInfoGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 17, 2007
*Abstract		:
*****************************************************************************
*Desc: Shows information about the currently selected character in the lobby.
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

class CCharacterInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CCharacterInfoGui(const RwChar* pName);
	virtual ~CCharacterInfoGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetChar(RwUInt8 byIndex);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CCharacterInfoGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			SetReservatedDelInfo(RwUInt8 byCharIndex);

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< background

	CSurfaceGui			m_srtInfoBack;		///< Character information background

	gui::CStaticBox*	m_pTitleStatic;		///<
	gui::CStaticBox*	m_pNameStatic;		///< 'name'
	gui::CStaticBox*	m_pLevelStatic;		///< 'Level'
	gui::CStaticBox*	m_pClassStatic;		///< 'class'
	gui::CStaticBox*	m_pZennyStatic;		///< ¡®SoZennyenny¡¯
	gui::CStaticBox*	m_pLastLocationStatic;///< 'Final location'
	gui::CStaticBox*	m_pDeleteDayStatic;	///< 'Scheduled deletion date'
	gui::CStaticBox*	m_pName;			///< Character name
	gui::CStaticBox*	m_pLevel;			///< Character level
	gui::CStaticBox*	m_pClass;			///< Character class
	gui::CStaticBox*	m_pZenny;			///< Character Owner Zenny
	gui::CStaticBox*	m_pLastLocation;	///< Character final location

	gui::CStaticBox*	m_pDeleteDay;		///< Expected date of character deletion
	gui::CStaticBox*	m_pDeleteDayExplain;///< Detailed description of character deletion date
};