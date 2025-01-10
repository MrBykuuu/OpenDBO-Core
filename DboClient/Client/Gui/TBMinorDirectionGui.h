////////////////////////////////////////////////////////////////////////////////
// File: TBMinorDirectionGui.h
// Desc: World's Best Martial Arts Preliminaries Directed by Gui
//
// 2008.09.18 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MINOR_DIRECTION_GUI_H__
#define __TB_MINOR_DIRECTION_GUI_H__

#include "NtlBudokai.h"
#include "TBCommonDirectionGui.h"

struct stENDINFO : public stCOMMONINFO
{
	stENDINFO(VOID) : pflaEnd( NULL ) {}

	gui::CFlash*	pflaEnd;
	CRectangle		rtOriginalEnd;

	VOID Create( gui::CFlash* pEnd );
	VOID Destroy(VOID);
	VOID Update( RwReal fElapsedTime );
	VOID StartMovie(VOID);
	VOID EndMovie(VOID);
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	gui::CSlot		slotMovieEnd;	

	VOID OnEndMovieEnd( gui::CComponent* pComponent );
};

struct stWINNERINFO : public stCOMMONINFO
{
	stWINNERINFO(VOID) : pflaWinner( NULL ), pstbWinnerName( NULL ) {}

	gui::CFlash*		pflaWinner;
	gui::CStaticBox*	pstbWinnerName;
	CRectangle			rtOriginalWinner;

	VOID Create( gui::CFlash* pWinner, gui::CStaticBox* pWinnerName );
	VOID Destroy(VOID);
	VOID Update( RwReal fElapsedTime );
	VOID SetWinnerName( const WCHAR* wszWinner );
	VOID StartMovie(VOID);
	VOID EndMovie(VOID);
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	gui::CSlot		slotWinnerMovieEnd;
	gui::CSlot		slotWinnerFSCallBack;

	VOID OnWinnerMovieEnd( gui::CComponent* pComponent );
	VOID OnWinnerFSCallBack( const RwChar* szCommand, const RwChar* szArgs );
};

class CTBMinorDirectionGui : public CTBCommonDirectionGui
{
public:
	//! Enum
	enum FLASHID { WAIT, TITLE, READYSTART };

	//! Constructor and Destructor
	CTBMinorDirectionGui( const RwChar* pName );
	~CTBMinorDirectionGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	//! Implementation
	VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! CallBack	
	VOID	OnWinnerMovieEnd( gui::CComponent* pComponent );
	//VOID	OnTitleMovieEnd( gui::CComponent* pComponent );

	//! Timer CallBack
	VOID	OnStartDice(VOID);
	VOID	OnShowDiceResult(VOID);

	gui::CSlot			m_slotWinnerMovieEnd;
	//gui::CSlot			m_slotTitleMovieEnd;
	
	//! Variables
	stENDINFO			m_stEnd;
	stWINNERINFO		m_stWinner;	

	//! Timer
	CTimerHandle		m_hDiceWinnerSelectTimer;		// The time from the drawing state until the dice are thrown.	
	CTimerHandle		m_hDiceWinnerShowTimer;			// The time at which the results of the dice are shown in a lottery situation.

	//! SelectionData
	TEAMTYPE					m_WinnerTeam;
	RwUInt8						m_bySelectionCount;
	sMINORMATCH_SELECTION_DATA	m_aSelectionData[BUDOKAI_MAX_MINORMATCH_TEAM_COUNT];
};

#endif //__TB_MAJOR_DIRECTION_GUI_H__