#include "precomp_dboclient.h"
#include "TBMatchTeamObBlueGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlWorldConcept.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"

// Dbo
#include "DboGlobal.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMatchTeamBlueMemberGui
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CTBMatchTeamBlueMemberGui::CTBMatchTeamBlueMemberGui()
{
	m_bCreate = FALSE;
}

/**
* \brief Destruction
*/
CTBMatchTeamBlueMemberGui::~CTBMatchTeamBlueMemberGui()
{

}

/**
* \brief Create
*/
RwBool CTBMatchTeamBlueMemberGui::Create( const WCHAR* pCharName, SERIAL_HANDLE hMemberSerial )
{
	NTL_FUNCTION( "CTBMatchTeamBlueMemberGui::Create" );

	if( !CNtlPLGui::Create( "gui\\Observer.rsr", "gui\\TBMatchTeamObBlueGui.srf", "gui\\TBMatchTeamObBlueGui.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pPnlClass		= (gui::CPanel*)GetComponent("pnlClass");
	m_pPnlLeader	= (gui::CPanel*)GetComponent("pnlLeader");
	m_pStbLevel		= (gui::CStaticBox*)GetComponent("stbLevel");
	m_pStbName		= (gui::CStaticBox*)GetComponent("stbName");
	m_pPgbLp		= (gui::CProgressBar*)GetComponent("pgbLp");
	m_pPgbEp		= (gui::CProgressBar*)GetComponent("pgbEp");

	// Name
	m_pStbName->SetText( pCharName );

	// Member Data Update
	UpdateMemberData( hMemberSerial );

	// Name, level and various status settings
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hMemberSerial ));
	if( pPlayer == NULL )
	{
		DBO_FAIL( "CTBMatchIndiObBlueGui : must player handle is valid" << hMemberSerial );
		return FALSE;
	}

	RwUInt8 byClass = Logic_GetPlayerClass( pPlayer );
	m_pPnlClass->AddSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );

	// Link
	LinkMsg( g_EventSobInfoUpdate, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );
	m_bCreate = TRUE;

	Show( true );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchTeamBlueMemberGui::Destroy()
{
	NTL_FUNCTION( "CTBMatchTeamBlueMemberGui::Destroy" );

	if( m_bCreate )
		GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
*/
VOID CTBMatchTeamBlueMemberGui::Update( RwReal fElapsed )
{
	m_LpGauge.Update( fElapsed, m_pPgbLp );
	m_EpGauge.Update( fElapsed, m_pPgbEp );
}

/**
* \brief HandleEvents
*/
VOID CTBMatchTeamBlueMemberGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast< SNtlEventSobInfoUpdate* >( msg.pData );

		if( m_hMemberSerial != pUpdate->hSerialId )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateMemberData( pUpdate->hSerialId );
		}
	}
}

VOID CTBMatchTeamBlueMemberGui::UpdateMemberData( SERIAL_HANDLE hSerial )
{
	// Name, level and various status settings
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hSerial ));
	if( pPlayer == NULL )
	{
		DBO_FAIL( "CTBMatchIndiObBlueGui : must player handle is valid" << hSerial );
		return;
	}

	m_hMemberSerial	= hSerial;

	RwUInt32	uiLp		= Logic_GetLp( pPlayer );
	RwUInt32	uiMaxLp		= Logic_GetMaxLp( pPlayer );
	RwUInt32	uiEp		= Logic_GetEp( pPlayer );
	RwUInt32	uiMaxEp		= Logic_GetMaxEp( pPlayer );
	RwUInt8		byLevel		= Logic_GetLevel( pPlayer );

	m_pStbLevel->SetText( byLevel );
	m_pPgbLp->SetRange(0, uiMaxLp );
	m_pPgbEp->SetRange(0, uiMaxEp );

	// Gauge Animation
	m_LpGauge.SetValue( uiLp, uiMaxLp, m_pPgbLp );
	m_EpGauge.SetValue( uiEp, uiMaxEp, m_pPgbEp );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMatch
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CTBMatchTeamObBlueGui::CTBMatchTeamObBlueGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_pMemberGui(NULL)
, m_byMemberNums(0)
, m_bCreate(FALSE)
{

}

/**
* \brief Destruction
*/
CTBMatchTeamObBlueGui::~CTBMatchTeamObBlueGui()
{

}

/**
* \brief Create
*/
RwBool CTBMatchTeamObBlueGui::Create()
{
	NTL_FUNCTION( "CTBMatchTeamObBlueGui::Create" );

	CRectangle rect;
	rect.SetRectWH( 0, 0, 1, 1 );
	m_pThis = NTL_NEW gui::CDialog( &rect, GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );

	LinkMsg( g_EventMajorMatchStateUpdate, 0 );
	LinkMsg( g_EventFinalMatchStateUpdate, 0 );

	Show( false );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchTeamObBlueGui::Destroy()
{
	DestroyMemberGui();

	NTL_DELETE( m_pThis );

	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
}

VOID CTBMatchTeamObBlueGui::CreateMemberGui()
{
	// Reads the number of blue team members.
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObBlueGui : must World concept is valid" );		

	stTBudokaiTeam* pBlueTeam = pTBWorldConcept->GetBlueTeam();
	DBO_ASSERT( pBlueTeam, "CTBmatchIndiObBlueGui : must Budokai red team is valid" );

	// Size
	m_byMemberNums = (RwUInt8)pBlueTeam->vecpMember.size();
	
	// Returns if member is 0
	if( m_byMemberNums == 0 )
		return;

	// Created according to size
	m_pMemberGui = NTL_NEW CTBMatchTeamBlueMemberGui[m_byMemberNums];

	for(RwInt32 i=0; i < m_byMemberNums; ++i )
	{
		m_pMemberGui[i].Create( pBlueTeam->vecpMember[i]->wstrName.c_str(), pBlueTeam->vecpMember[i]->hSerial );
	}

	// To fit in the middle in full screen
	int nHeight = (m_byMemberNums * dTEAMMEMBER_GUI_OFFSET_Y) - (dTEAMMEMBER_GUI_OFFSET_Y - dTEAMMEMBER_GUI_HEIGHT);
	int nStartPosY = (GetDboGlobal()->GetScreenHeight()/2) - (nHeight/2);
	int nStartPosX = (GetDboGlobal()->GetScreenWidth()-dTEAMMEMBER_GUI_WIDTH);
	for(RwInt32 i=0; i < m_byMemberNums; ++i )
	{
		m_pMemberGui[i].SetPosition( nStartPosX, nStartPosY + ( i * dTEAMMEMBER_GUI_OFFSET_Y ) );
	}

	m_bCreate = TRUE;
}

VOID CTBMatchTeamObBlueGui::DestroyMemberGui()
{
	for(RwInt32 i=0; i < m_byMemberNums; ++i )
	{
		m_pMemberGui[i].Destroy();
	}
	NTL_ARRAY_DELETE( m_pMemberGui );

	m_byMemberNums = 0;

	m_bCreate = FALSE;
}

/**
* \brief Update
* \param fElapsed (RwReal) Time elapsed from previous update.
*/
VOID CTBMatchTeamObBlueGui::Update( RwReal fElapsed )
{
	if( m_bCreate )
	{
		for(RwInt32 i=0; i< m_byMemberNums; ++i )
		{
			m_pMemberGui[i].Update( fElapsed );
		}
	}
}

/**
* \brief HandleEvents
*/
VOID CTBMatchTeamObBlueGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CTBMatchTeamObBlueGui::HandleEvents : must World concept is valid" );

		if( pData->bEnterState )
		{
			// Release it if it is not in READY, RUN, or FINISH status.
			if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
			{
				if( !m_bCreate )
				{
					// Return if you are a participant				
					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// Return if playing single
					if( !pTBWorldConcept->IsPartyBattle() )
						return;

					CreateMemberGui();
				}
			}
			else
			{
				if( m_bCreate )
					DestroyMemberGui();
			}
		}
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CTBMatchTeamObBlueGui::HandleEvents : must World concept is valid" );

		if( pData->bEnterState )
		{	
			// Release it if it is not in READY, RUN, or FINISH status.
			if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
			{
				if( !m_bCreate )
				{
					// Return if you are a participant				
					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// Return if playing single
					if( !pTBWorldConcept->IsPartyBattle() )
						return;

					// Created when Team Info comes in.
					CreateMemberGui();
				}
			}
			else
			{
				if( m_bCreate )
					DestroyMemberGui();
			}
		}		
	}
}