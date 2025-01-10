/*****************************************************************************
* File			: TBMatchIndiObRedGui.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2008. 11. 24
* Abstract		: World's Best Martial Arts Spectator Individual Exhibition Red Team GUI
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TB_MATCH_INDI_OB_RED_GUI_H__
#define __TB_MATCH_INDI_OB_RED_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GaugeAnimation.h"
#include "NtlSlDef.h"
#include "NtlWorldConceptTB.h"

class CTBMatchIndiObRedGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchIndiObRedGui( const RwChar* pName );
	virtual ~CTBMatchIndiObRedGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	VOID		Update( RwReal fElapsed );

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg& msg);

	// Implements
	VOID		SetUserData();
	VOID		UpdateUserData(SERIAL_HANDLE hSerial);

protected:
	stTBudokaiMember*	GetRedTeamUser();

protected:
	gui::CStaticBox*	m_pStbName;
	gui::CProgressBar*	m_pPgbLp;
	gui::CProgressBar*	m_pPgbEp;
	gui::CStaticBox*	m_pStbLevel;
	gui::CStaticBox*	m_pStbLp;
	gui::CStaticBox*	m_pStbEp;

	CGaugeAnimation		m_LpGauge;
	CGaugeAnimation		m_EpGauge;

	RwBool				m_bSetData;
};

#endif//__TB_MATCH_INDI_OB_RED_GUI_H__