/******************************************************************************
*File           : GMPlayerGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : November 17, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Player information seen by GM
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

class CPlayerInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPlayerInfoGui(const RwChar* pName);
	virtual ~CPlayerInfoGui();

	RwBool		Create();
	VOID		Destroy();

	RwBool		SetPlayerSerial(SERIAL_HANDLE hPlayer);

	// To avoid interfering with the DialogManager's Movable
	virtual void	SetMovable(RwBool bMovable) {}

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPlayerInfoGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);

protected:
	gui::CSlot					m_slotCloseButton;

	gui::CButton*				m_pExitButton;

	gui::CStaticBox*			m_pPlayerName;
	gui::CStaticBox*			m_pLevel;
	gui::CStaticBox*			m_pRace;
	gui::CStaticBox*			m_pClass;
	gui::CStaticBox*			m_pRecord;
	gui::CStaticBox*			m_pGuild;
	gui::CStaticBox*			m_pTeam;

	gui::CStaticBox*			m_pLevel_content;
	gui::CStaticBox*			m_pRace_content;
	gui::CStaticBox*			m_pClass_content;
	gui::CStaticBox*			m_pRecord_content;
	gui::CStaticBox*			m_pGuild_content;
	gui::CStaticBox*			m_pTeam_content;

	SERIAL_HANDLE				m_hPlayer;
};