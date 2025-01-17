////////////////////////////////////////////////////////////////////////////////
// File: TBTournamentGui.h
// Desc: The World's First Martial Arts Competition Table
//
// 2008.09.25 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_TOURNAMENT_GUI_H__
#define __TB_TOURNAMENT_GUI_H__

#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "ceventhandler.h"
#include "NtlBudokai.h"
#include "NtlTimeQuest.h"

// Tournament Fight Information
class CTBTournamentFightInfo
{
public:
	//! Enumuration
	enum CHILDSTATE { LEFT_ANIMATION_END = 0x01, RIGHT_ANIMATION_END = 0x02, ENDOFCHILDANIMATION = 0x03 };

	struct stENTRYINFO
	{
		stENTRYINFO::stENTRYINFO(VOID)
		{
			bEntry = FALSE;
			wJoinID = INVALID_WORD;
			byScore = INVALID_BYTE;			
		}

		RwBool			bEntry;
		RwUInt16		wJoinID;
		RwUInt8			byScore;		
	};

	//! Constructor & Destructor
	CTBTournamentFightInfo( CTBTournamentFightInfo* pAboveNode, RwUInt8 byMatchNumber, RwUInt8 byMatchLevel, RwBool bParty );		
	~CTBTournamentFightInfo(VOID);

	//! Operation
	// 1. Fill in the line data when you receive the match results, and turn on the flag once the results are received.	The animation flag is also on~!!
	VOID	SetFightResult( RwUInt8 byMatchResult, RwUInt16 wWinnerJoinID, RwUInt16 wJoinID1, RwUInt16 wJoinID2, RwUInt8 byScore1, RwUInt8 byScore2 );

	// 2. At each advance, it is filled with data uploaded from below. 
	VOID	SetEntryData( RwUInt8 byIdx, RwUInt16 wJoinID );

	// 3. Actions when the view information button is pressed
	VOID	ClickFightInfo(VOID);

	// 4. Print name
	VOID	ShowNameToInfoWnd( RwUInt8 byTeamPos );
	
	// 4. When the animation ends.
	VOID	EndOfAnimation(VOID);

	//! Attribute
	RwUInt8		GetMatchNumber(VOID) { return m_byMatchNumber; }
	RwUInt8		GetMatchLevel(VOID) { return m_byMatchLevel; }
	RwUInt16	GetWinnerID(VOID) { return m_wWinnerTeamJoinID; }
	RwUInt16	GetLeftEntryID(VOID) { return m_stEntry[0].wJoinID; }
	RwUInt16	GetRightEntryID(VOID) { return m_stEntry[1].wJoinID; }
	RwUInt8		GetWinnerPosition(VOID);
	RwBool		IsPartyData(VOID) { return m_bParty; }
	RwBool		IsEnableToStartAnimation(VOID) { return m_stEntry[0].bEntry && m_stEntry[1].bEntry; }

	CTBTournamentFightInfo* GetAboveFightInfo(VOID) { return m_pAboveFightInfo; }

private:
	//! Variable
	// Location information (several rounds and matches)
	RwUInt8			m_byMatchNumber;	// Number 0 from the left, server number
	RwUInt8			m_byMatchLevel;		// From the finals, number 0, server number
	// Flag that must be animated (must receive termination signals from 2 child nodes. That is, it must be 2)
	RwInt32			m_nEndOfBelowAnimation;	
	// Participation ID of each competitor
	stENTRYINFO		m_stEntry[2];			
	// Match Results
	RwUInt8			m_byMatchResult;	// eMATCH_RESULT
	RwUInt16		m_wWinnerTeamJoinID;	
	// parent node 
	CTBTournamentFightInfo* m_pAboveFightInfo;
    // Are you in party mode
	RwBool			m_bParty;	
};

class CTBTournamentInfoGroup
{
public:
	CTBTournamentInfoGroup( RwUInt8 byPosOffset, RwInt32 nEntryCount, RwUInt8 byTopOfDepth, RwUInt8 byEndOfDepth, RwBool bParty, gui::CFlash* pFlash, gui::CStaticBox* pWinnerName, gui::CStaticBox* pWinnerEmblem );
	virtual ~CTBTournamentInfoGroup(VOID);

	VOID	Update( RwReal fElapsed );
	VOID	Show( bool bShow );
	
	virtual	VOID	CreateFightData( RwUInt8 byIndex, CTBTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber );	
	virtual VOID	SetEntryData( RwUInt8 byIndex, RwUInt16 wJoinID );
	virtual RwBool	SetFightResultData( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2, 
										RwUInt8 byMatchResult, RwUInt16 wWinnerTeam, RwUInt8 byScore1, RwUInt8 byScore2 );

	CTBTournamentFightInfo*	FindFightInfo( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber );	
	
	CTBTournamentFightInfo*	GetDepthNumberFromFlashArgs( const RwChar* szArgs, RwUInt8* pDepth, RwUInt8* pNumber );
	CTBTournamentFightInfo*	GetDepthNumberTeamFromFlashArgs( const RwChar* szArgs, RwUInt8* pDepth, RwUInt8* pNumber, RwUInt8* pTeam );
	RwBool	IsMyTeamInThisGroup( RwUInt16 wjoinID );
	VOID	DrawLine( RwUInt8 byDepth, RwUInt8 byNumber, RwUInt8 byPos );
	VOID	FocusMyEntry( RwUInt8 byDepth, RwUInt8 byNumber, RwUInt8 byPos );

protected:
	//! CallBack
	VOID	OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs );

	gui::CSlot				m_slotFSCallBack;

	//! Variable
	CTBTournamentFightInfo**	m_ppFightInfo;
	RwInt32						m_nFightInfoCount;	
	RwInt32						m_nEntryCount;

	RwUInt8						m_byPosOffset;			
	RwUInt8						m_byEndOfDepth;
	RwUInt8						m_byTopOfDepth;
	RwBool						m_bParty;

	gui::CFlash*				m_pFlash;
	gui::CStaticBox*			m_pWinnerName;
	gui::CStaticBox*			m_pWinnerEmblem;
};

class CTBTournamentFinalInfoGroup : public CTBTournamentInfoGroup
{
public:
	enum NAMEINDEX { SEMIFINAL1_LEFT, SEMIFINAL1_RIGHT, SEMIFINAL2_LEFT, SEMIFINAL2_RIGHT, FINAL_LEFT, FINAL_RIGHT, NUM_NAME };
	CTBTournamentFinalInfoGroup( RwUInt8 byPosOffset, RwInt32 nEntryCount, RwUInt8 byTopOfDepth, RwUInt8 byEndOfDepth, RwBool bParty, gui::CFlash* pFlash, gui::CStaticBox* pWinnerName, gui::CStaticBox* pWinnerEmblem );
	virtual ~CTBTournamentFinalInfoGroup(VOID);

	virtual VOID	CreateFightData( RwUInt8 byIndex, CTBTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber );
	virtual VOID	SetEntryData( RwUInt8 byIndex, RwUInt16 wJoinID ) { /* Do Nothing */ }
	virtual RwBool	SetFightResultData( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2, 
										RwUInt8 byMatchResult, RwUInt16 wWinnerTeam, RwUInt8 byScore1, RwUInt8 byScore2 );
	VOID	SetFinalEntryData( NAMEINDEX eNameIndex, const WCHAR* szName );

protected:
	//! CallBack
	VOID	OnFinalFSCallBack( const RwChar* szCommand, const RwChar* szArgs );

	gui::CSlot			m_slotFinalFSCallBack;

	//! Variable
	gui::CStaticBox*	m_pName[NUM_NAME];	
	gui::CStaticBox*	m_pEmblem[NUM_NAME];
};

class CTBTournamentGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum ePage { TOURNAMENT_INDIVIDUAL_A, TOURNAMENT_INDIVIDUAL_B, TOURNAMENT_INDIVIDUAL_FINAL, TOURNAMENT_PARTY, NUM_PAGE, INVALID_PAGE = 0xFFFFFFFF };

	//! Constructor & Destructor
	CTBTournamentGui( const RwChar* pName );
	~CTBTournamentGui(VOID);

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
	VOID	CreateFightDataGroup(VOID);
	VOID	DeleteFightDataGroup(VOID);
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
	VOID	OnClickPartyBtn( gui::CComponent* pComponent );
	VOID	OnClickIndividualBtn( gui::CComponent* pComponent );

	//! Variable
	CTBTournamentInfoGroup*			m_pInfoGroup[NUM_PAGE];

	RwUInt8							m_byMaxEntry;							// A few lessons at most. 
	ePage							m_eCurrentPage;							// Currently visible page.
	RwUInt8							m_byIndividualFightInfoInputDoneCount;	// Number of currently entered FightInfo. (for server request) Individual match
	RwUInt8							m_byTeamFightInfoInputDoneCount;		// Number of currently entered FightInfo. (For server request) Party battle		

	gui::CButton*					m_pbtnRightArrow;
	gui::CButton*					m_pbtnLeftArrow;
	gui::CStaticBox*				m_pstbIndividualGroupType;

	gui::CButton*					m_pbtnParty;			// ToggleFormat button. When partying, the above three components disappear or are disabled.
	gui::CButton*					m_pbtnIndividual;

	gui::CStaticBox*				m_pstbTitle;			
	gui::CStaticBox*				m_pstbInfomation;		// Description.	
	gui::CButton*					m_pbtnNextInfomation;	// Next Description button.

	gui::CButton*					m_pbtnClose;

public:
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO> m_mapIndividualEntryInfo;
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO> m_mapTeamEntryInfo;
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA> m_mapIndividualEntryData;
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA> m_mapTeamEntryData;

	static sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* FindIndividualEntryInfo( RwUInt16 wJoinID );
	static sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* FindTeamEntryInfo( RwUInt16 wJoinID );
	static sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* FindIndividualEntryData( RwUInt16 wJoinID );
	static sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* FindTeamEntryData( RwUInt16 wJoinID );
};


#endif//__BUDOKAI_TOURNAMENT_GUI_H__