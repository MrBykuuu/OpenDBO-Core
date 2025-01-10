////////////////////////////////////////////////////////////////////////////////
// File: TBGui.h
// Desc: Tenkaichi Budokai Gui Group
//       Gui related to World's Best Martial Arts Club. They are created and deleted according to the season and match.
//
// 2007.08.23 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_GUI_H__
#define __TB_GUI_H__

#include "ceventhandler.h"

class CTBMinorMatchInfoGui;
class CTBMinorDirectionGui;
class CTBMajorMatchInfoGui;
class CTBMajorDirectionGui;
class CTBFinalMatchInfoGui;
class CTBFinalDirectionGui;
class CTBTournamentMatchInfoGui;
class CTBTournamentGui;
class CTBNewsGui;
class CTBRequestGui;
class CTBMinorMatchResultGui;
class CTBMatchResultGui;
class CTBAwardResultGui;
class CResultNarrationGui;
class CCounterGui;
class CTBMatchObserverVersusGui;
class CTBMatchIndiObRedGui;
class CTBMatchIndiObBlueGui;
class CTBMatchTeamObRedGui;
class CTBMatchTeamObBlueGui;

#define TB_MATCH_RESULT_REMAIN_TIME		10.99f

class CTBGui : public RWS::CEventHandler
{
public:
	//! Destructor
	~CTBGui(VOID) {}

	//! Operation
	static	VOID CreateInstance(VOID);
	static	VOID DeleteInstance(VOID);

	VOID	Create( RwUInt8 byMatchType );
	VOID	Destroy(VOID);

	// Create and delete newsletters
	VOID	CreateBudokaiNewsOnce();	///< Run only once after loading
	VOID	CreateBudokaiNews();
	VOID	DestroyBudokaiNews();
	
	//! Access
	static CTBGui*	GetInstance(VOID);

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	// When updating each Match State, participants and spectators are processed separately.
	VOID	HandleEventMinorMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventMinorMatchStateUpdateObserver( RWS::CMsg& msg );
	VOID	HandleEventMajorMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventMajorMatchStateUpdateObserver( RWS::CMsg& msg );
	VOID	HandleEventFinalMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventFinalMatchStateUpdateObserver( RWS::CMsg& msg );
	
	// To setGauge when an object is created in the middle
	VOID	HandleEventSobCreate( RWS::CMsg& msg );

	//! Implements
	VOID	SetPlayerState( RwUInt32 hSerial );
	VOID	SetBattleGauge(VOID);
	VOID	UnsetBattleGauge(VOID);
	VOID	ProgressMessageNotify( RwUInt8 byMsgId );
	VOID	DirectionMode( RwBool bDirection );
	VOID	SetCounterPosition( RwInt32 nScreenHeight );
	VOID	SetCounterTime( RwReal fTime );
	VOID	HideCounterTime(VOID);

	//! Variable
	CTBMinorMatchInfoGui*		m_pMinorMatchInfoGui;
	CTBMinorDirectionGui*		m_pMinorDirectionGui;
	CTBMajorMatchInfoGui*		m_pMajorMatchInfoGui;
	CTBMajorDirectionGui*		m_pMajorDirectionGui;
	CTBFinalMatchInfoGui*		m_pFinalMatchInfoGui;
	CTBFinalDirectionGui*		m_pFinalDirectionGui;	
	CTBTournamentGui*			m_pTournamentGui;
	CTBTournamentMatchInfoGui*	m_pTournamentMatchInfoGui;
	CTBMatchResultGui*			m_pMatchResultGui;
	CTBAwardResultGui*			m_pAwardResultGui;
	CTBMinorMatchResultGui*		m_pMinorMatchResultGui;
	CResultNarrationGui*		m_pResultNarration;
	CCounterGui*				m_pCounter;

	// Spectator GUI
	CTBMatchObserverVersusGui*	m_pObserverVersusGui;
	CTBMatchIndiObRedGui*		m_pMatchIndiObRedGui;
	CTBMatchIndiObBlueGui*		m_pMatchIndiObBlueGui;
	CTBMatchTeamObRedGui*		m_pMatchTeamObRedGui;
	CTBMatchTeamObBlueGui*		m_pMatchTeamObBlueGui;

	// World¡¯s Best Martial Arts Newsletter
	CTBNewsGui*					m_pNewsGui;
	CTBRequestGui*				m_pRequestGui;

	RwBool						m_bCreate;
	
	static CTBGui*				m_pInstance;

	//! Prevented default constructor
	CTBGui(VOID);
};

static CTBGui* GetTBGuiInstance(VOID)
{
	return CTBGui::GetInstance();
}

#endif//__TB_GUI_H__