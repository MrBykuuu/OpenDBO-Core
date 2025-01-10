////////////////////////////////////////////////////////////////////////////////
// File: BudokaiTournamentGui.h
// Desc: World's First Martial Arts Competition Table
//
// 2008.06.04 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BUDOKAI_TOURNAMENT_GUI_H__
#define __BUDOKAI_TOURNAMENT_GUI_H__

#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "ceventhandler.h"
#include "NtlBudokai.h"
#include "NtlTimeQuest.h"

// participant 
class CBudokaiTournamentEntry
{
public:
	//! Constructor & Destructor
	CBudokaiTournamentEntry( gui::CComponent* pParent, RwUInt8 byPos,
							 RwInt32 nMark1Left, RwInt32 nMark1Top, RwInt32 nWidth, RwInt32 nHeight, RwInt32 nOffset );
	~CBudokaiTournamentEntry(VOID);

	//! Operation
	// 1. When you receive the participant packet, fill it out in full.
	VOID	SetEntry( const WCHAR* pName, RwUInt16 wJoinID );

private:
	//! Callback
	// 1. Launch InfoWnd when Mouse Enter&Leave
	gui::CSlot	m_slotMouseEnter;
	gui::CSlot	m_slotMouseLeave;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;

	VOID	OnMouseEnter( gui::CComponent* pComponent );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);

	//! Variable
	// 0. Finals participation number
	RwUInt8			m_byPos;
	RwUInt16		m_wJoinID;
	// 1. Participant¡¯s name.
	std::wstring	m_wstrName;
	// 2. Participation number CPanel.
	gui::CPanel*	m_ppnlMark;
	// 3. Focus Surface 
	CSurfaceGui		m_surFocus;	
};

// Daejeon Information
class CBudokaiTournamentFightInfo
{
public:
	//! Enumuration
	enum ANIMATIONSTATE { NONE, INIT_VERTICAL, HORIZONTAL, VERTICAL, END };
	enum CHILDSTATE { ENDOFCHILDANIMATION = 2 };

	struct stENTRYINFO
	{
		stENTRYINFO::stENTRYINFO(VOID)
		{
			bEntry = FALSE;
			wJoinID = INVALID_WORD;
			byScore = INVALID_BYTE;
		}

		RwBool		bEntry;
		RwUInt16	wJoinID;
		RwUInt8		byScore;
	};

	//! Constructor & Destructor
	CBudokaiTournamentFightInfo( CBudokaiTournamentFightInfo* pAboveNode, gui::CComponent* pParent, RwUInt8 byMatchNumber, RwUInt8 byMatchLevel, RwUInt8 byServerMatchNumber, RwUInt8 byServerMatchLevel, RwBool bParty,
								 RwInt32 nOriginLeft, RwInt32 nOriginTop , RwInt32 nWidth, RwInt32 nHeight, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nLineEndY );
	~CBudokaiTournamentFightInfo(VOID);

	//! Operation
	// 0. Update
	VOID	Update( RwReal fElapsedTime );		
	VOID	UpdateInitVertical( RwReal fElapsedTime );
	VOID	UpdateHorizontal( RwReal fElapsedTime );
	VOID	UpdateVertical( RwReal fElapsedTime );

	// 1. Fill in the line data when you receive the match results, and turn on the flag once the results are received.	The animation flag is also on~!!
	VOID	SetFightResult( RwUInt16 wWinnerJoinID, RwUInt16 wJoinID1, RwUInt16 wJoinID2, RwUInt8 byScore1, RwUInt8 byScore2 );

	// 2. At each advance, it is filled with data uploaded from below. 
	VOID	SetEntryData( RwUInt8 byIdx, RwUInt16 wJoinID );
		
	//! Attribute
	RwUInt8		GetServerMatchNumber(VOID) { return m_byServerMatchNumber; }
	RwUInt8		GetServerMatchLevel(VOID) { return m_byServerMatchLevel; }
	RwUInt16	GetWinnerID(VOID) { return m_wWinnerTeamJoinID; }
	RwUInt16	GetLoserID(VOID) { return m_wLoserTeamJoinID; }
	RwUInt16	GetLeftEntryID(VOID) { return m_stEntry[0].wJoinID; }
	RwUInt16	GetRightEntryID(VOID) { return m_stEntry[1].wJoinID; }
	RwBool		IsPartyData(VOID) { return m_bParty; }
		
private:
	//! Implements
	VOID	SetAnimationState( ANIMATIONSTATE eAnimationState );

	RwInt32	GetHorizontalOrigin(VOID);
	RwInt32	GetHorizontalOffset(VOID);
	RwBool	IsLeftNode(VOID);
	
	//! Callback
	// 1. Send a match information request packet when you click the button. 	
	gui::CSlot	m_slotClickInfoBtn;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;

	VOID	OnClickInfoBtn( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);
	
	//! Variable
	// 1. CButton
	gui::CButton*	m_pbtnBattleInfo;
	// 2. 3 Line Surfaces, 5 if at the end, as much Rect information, if at the end
	CSurfaceGui		m_surLeftInitVerticalLine;
	CSurfaceGui		m_surRightInitVerticalLine;
	CSurfaceGui		m_surLeftHorizontalLine;
	CSurfaceGui		m_surRightHorizontalLine;
	CSurfaceGui		m_surCenterVerticalLine;

	CRectangle		m_rtLeftInitVerticalLine;
	CRectangle		m_rtRightInitVerticalLine;
	CRectangle		m_rtLeftHorizontalLine;
	CRectangle		m_rtRightHorizontalLine;
	CRectangle		m_rtCenterVerticalLine;

	RwInt32			m_nOriginLeft;
	RwInt32			m_nHorizontalOffset;
	
	// Movement time of each line (distance (pixel) /speed (pixel/time))
	RwReal			m_fMoveTimeInitVertical;
	RwReal			m_fMoveTimeHorizontal;
	RwReal			m_fMoveTimeVertical;

	// current time
	RwReal			m_fCurrentTime;

	// 3. Location information (several rounds and matches)
	RwUInt8			m_byMatchNumber;		// Number 0 from the left
	RwUInt8			m_byMatchLevel;			// Number 0 from the bottom
	RwUInt8			m_byServerMatchNumber;	// Number 0 from the left, server number
	RwUInt8			m_byServerMatchLevel;	// From the finals, number 0, server number
	// 4. Flag that must be animated (must receive termination signals from 2 child nodes. That is, it must be 2.
	RwInt32			m_nEndOfBelowAnimation;	
	// 5. Save time so that packets cannot be sent until a certain period of time has passed.
	RwUInt32		m_uiWaitTime;			
	// 6. Participation ID of each competitor
	stENTRYINFO		m_stEntry[2];			
	// 7. Match results
	RwUInt16		m_wWinnerTeamJoinID;
	RwUInt16		m_wLoserTeamJoinID;
	// 8. Animation state
	ANIMATIONSTATE	m_eAnimationState;		// When the result is received, a value other than 0 is received.
	// 9. Parent node 
	CBudokaiTournamentFightInfo* m_pAboveFightInfo;
	// 10. Parent component
	gui::CComponent*	m_pdlgParent;
	// 11. Is it party mode?
	RwBool			m_bParty;	
};

class CBudokaiTournamentEntryGroup
{
public:
	CBudokaiTournamentEntryGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nOffset, gui::CDialog* pDialog );
	~CBudokaiTournamentEntryGroup(VOID);

	VOID	Show( bool bShow );
	VOID	SetData( sVARIABLE_DATA* pData, WORD wEntryTeam );

private:
	//! Implementation
	
	//! Variables
	gui::CDialog* m_pDialog;
	CBudokaiTournamentEntry** m_pEntry;
	RwInt32	m_nEntryCount;
	RwInt32 m_nPosOffset;				// ID Offset
};

class CBudokaiTournamentFightInfoGroup
{
public:
	CBudokaiTournamentFightInfoGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nEndLineY, gui::CDialog* pDialog );
	~CBudokaiTournamentFightInfoGroup(VOID);

	VOID	CreateData( RwUInt8 byIndex, CBudokaiTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwBool bParty );	// peessitemp : Annoying -_-
	//RwBool	SetEntryData( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2 );
	//RwBool	SetResultData( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwUInt16 wWinnerJoinID );

	CBudokaiTournamentFightInfo*	FindFightInfo( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber );
	
	VOID	Update( RwReal fElapsed );
	VOID	Show( bool bShow );	
	
private:
	//! Variables
	gui::CDialog* m_pDialog;
	CBudokaiTournamentFightInfo** m_ppFightInfo;
	RwInt32 m_nFightInfoCount;
	RwInt32 m_nPosOffset;
	
	CPos	m_posOrigin;
	RwInt32	m_nVerticalOffset;
	RwInt32	m_nHorizontalOffset;
	RwInt32 m_nEndLineY;
};

class CBudokaiTournamentGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum ePage { TOURNAMENT_INDIVIDUAL_A, TOURNAMENT_INDIVIDUAL_B, TOURNAMENT_PARTY, NUM_PAGE, INVALID_PAGE = 0xFFFFFFFF };

	//! Constructor & Destructor
	CBudokaiTournamentGui( const RwChar* pName );
	~CBudokaiTournamentGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32	SwitchDialog( bool bOpen );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );	
private:	
	//! Implements
	VOID	CreateDataGroup(VOID);
	VOID	SetPage( ePage ePageType );

	//! Callback
	gui::CSlot		m_slotClickClose;
	gui::CSlot		m_slotClickRightArrow;
	gui::CSlot		m_slotClickLeftArrow;
	gui::CSlot		m_slotClickPartyBtn;
	gui::CSlot		m_slotClickIndividualBtn;

	VOID	OnClickClose( gui::CComponent* pComponent );
	VOID	OnClickRightArrow( gui::CComponent* pComponent );
	VOID	OnClickLeftArrow( gui::CComponent* pComponent );
	VOID	OnToggledPartyBtn( gui::CComponent* pComponent, bool bToggleDown );
	VOID	OnToggledIndividualBtn( gui::CComponent* pComponent, bool bToggleDown );

	//! Variable
	gui::CDialog*						m_pPageDialog[NUM_PAGE];
	
	CBudokaiTournamentEntryGroup*		m_pEntryGroup[NUM_PAGE];
	CBudokaiTournamentFightInfoGroup*	m_pFightInfoGroup[NUM_PAGE];
	
	RwUInt8							m_byMaxEntry;			// How many classes at most. 
	ePage							m_eCurrenctPage;		// Currently visible page.
	RwUInt8							m_byIndividualFightInfoInputDoneCount;	// Number of currently entered FightInfo. (for server request) Individual match
	RwUInt8							m_byTeamFightInfoInputDoneCount;		// Number of currently entered FightInfo. (For server request) Party battle		

	gui::CButton*					m_pbtnRightArrow;
	gui::CButton*					m_pbtnLeftArrow;
	gui::CStaticBox*				m_pstbIndividualGroupType;

	gui::CButton*					m_pbtnParty;			// ToggleFormat button. When partying, the above three components disappear or are disabled.
	gui::CButton*					m_pbtnIndividual;
	
	gui::CPanel*					m_ppnlLineBack;			// line.
	gui::CPanel*					m_ppnlQuestionMark;		// -_-;;
	
	gui::CStaticBox*				m_pstbTitle;			
	gui::CStaticBox*				m_pstbInfomation;		// explanation.	

	gui::CButton*					m_pbtnClose;

public:
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA> m_mapIndividualEntryData;
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA> m_mapTeamEntryData;

	static sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* FindIndividualEntryData( RwUInt16 wJoinID );
	static sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* FindTeamEntryData( RwUInt16 wJoinID );
};


#endif//__BUDOKAI_TOURNAMENT_GUI_H__