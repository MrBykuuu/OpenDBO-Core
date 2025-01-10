#include "precomp_dboclient.h"
#include "TBMatchIndiObRedGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulaion
#include "NtlWorldConcept.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"

/**
* \brief Construction
* \param pName	(const RwChar*) GUI name
*/
CTBMatchIndiObRedGui::CTBMatchIndiObRedGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_bSetData( FALSE )
{

}

/**
* \brief Destruction
*/
CTBMatchIndiObRedGui::~CTBMatchIndiObRedGui()
{

}


/**
* \brief Create
*Create a GUI related to the StatusBar of the RED team, the spectator of the World's Best Martial Arts individual exhibition.
*/
RwBool CTBMatchIndiObRedGui::Create()
{
	NTL_FUNCTION("CTBMatchIndiObRedGui::Create");

	if( !CNtlPLGui::Create("gui\\Observer.rsr", "gui\\TBMatchIndiObRedGui.srf", "gui\\TBMatchIndiObRedGui.frm") )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis		= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbName	= (gui::CStaticBox*)GetComponent("stbName");
	m_pPgbLp	= (gui::CProgressBar*)GetComponent("pgbLP");
	m_pPgbEp	= (gui::CProgressBar*)GetComponent("pgbEP");
	m_pStbLp	= (gui::CStaticBox*)GetComponent("stbLP");
	m_pStbEp	= (gui::CStaticBox*)GetComponent("stbEP");
	m_pStbLevel	= (gui::CStaticBox*)GetComponent("stbLevel");

	// Link
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventMajorMatchStateUpdate, 0 );
	LinkMsg( g_EventFinalMatchStateUpdate, 0 );

	// Add Update
	GetNtlGuiManager()->AddUpdateFunc( this );

	Show( false );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchIndiObRedGui::Destroy()
{
	NTL_FUNCTION("CTBMatchIndiObRedGui::Destroy");

	// Remove Update
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchStateUpdate );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed (RwReal) Time elapsed from previous frame
*/
VOID CTBMatchIndiObRedGui::Update( RwReal fElapsed )
{
	m_LpGauge.Update( fElapsed, m_pPgbLp );
	m_EpGauge.Update( fElapsed, m_pPgbEp );
}

/**
* \brief HandleEvents
* \param msg		(RWS::CMsg&) Event message structure
*/
VOID CTBMatchIndiObRedGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		// If data is not specified or the GUI is not currently displayed, there is no need to update.
		if( !m_bSetData || !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast< SNtlEventSobInfoUpdate* >( msg.pData );
	
		stTBudokaiMember* pMember = GetRedTeamUser();

		// Only the current red team players are updated.
		if( pMember->hSerial != pUpdate->hSerialId )
			return;
		
		// Attribute related updates
		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateUserData( pUpdate->hSerialId );
		}
	}
	else if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

		if( pData->bEnterState )
		{
			if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
			{
				if( !IsShow() )
				{
					// Return if you are a participant
					CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
					DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObRedGui::HandleEvents : must World concept is valid" );

					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// If it's a party play, return
					if( pTBWorldConcept->IsPartyBattle() )
						return;

					pMember = GetRedTeamUser();

					// When team information comes, enter the user's data.
					SetUserData();

					SetPosition( 0, 0 );
					Show( true );
				}
			}
			else
			{
				if( IsShow() )
					Show( false );

			}
		}		
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

		if( pData->bEnterState )
		{
			if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
			{
				if( !IsShow() )
				{
					// Return if you are a participant
					CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
					DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObRedGui::HandleEvents : must World concept is valid" );

					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// If it's a party play, return
					if( pTBWorldConcept->IsPartyBattle() )
						return;

					pMember = GetRedTeamUser();

					if( !pMember )
						return;

					SetPosition( 0, 0 );

					// When team information comes, enter the user's data.
					SetUserData();

					Show( true );
				}
			}
			else
			{
				if( IsShow() )
					Show( false );
			}
		}        
	}
}

/**
* \brief Red Team's Plyaer Data Update
*/
VOID CTBMatchIndiObRedGui::SetUserData()
{
	stTBudokaiMember* pMember = GetRedTeamUser();
	DBO_ASSERT( pMember, "CTBMatchIndiObRedGui : must Budokai member is valid" );

	m_bSetData = TRUE;
	
	// Check the user and do not update if the current user is NOAPPLY or GIVEUP.
	if( pMember->byState == MATCH_MEMBER_STATE_NOAPPLY ||
		pMember->byState == MATCH_MEMBER_STATE_GIVEUP )
	{
		return;
	}

	// name
	m_pStbName->SetText( pMember->wstrName.c_str() );

	UpdateUserData( pMember->hSerial );
}

VOID CTBMatchIndiObRedGui::UpdateUserData( SERIAL_HANDLE hSerial )
{
	// Name, level and various status settings
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hSerial ));
	DBO_ASSERT( pPlayer, "CTBMatchIndiObRedGui : must player handle is valid" );

	RwUInt32	uiLp		= Logic_GetLp( pPlayer );
	RwUInt32	uiMaxLp		= Logic_GetMaxLp( pPlayer );
	RwUInt32	uiEp		= Logic_GetEp( pPlayer );
	RwUInt32	uiMaxEp		= Logic_GetMaxEp( pPlayer );
	RwUInt8		byLevel		= Logic_GetLevel( pPlayer );

	m_pStbLevel->SetText( byLevel );
	m_pPgbLp->SetRange(0, uiMaxLp );
	m_pPgbEp->SetRange(0, uiMaxEp );
	m_pStbLp->Format( "%u / %u", uiLp, uiMaxLp );
	m_pStbEp->Format( "%u / %u", uiEp, uiMaxEp );
	
	// Gauge Animation
	m_LpGauge.SetValue( uiLp, uiMaxLp, m_pPgbLp );
	m_EpGauge.SetValue( uiEp, uiMaxEp, m_pPgbEp );
}

/**
* \brief Returns user data of the Red team
*/
stTBudokaiMember* CTBMatchIndiObRedGui::GetRedTeamUser()
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObRedGui : must World concept is valid" );		

	stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetRedTeam();
	if( !pRedTeam )
	{
		DBO_FAIL( "CTBmatchIndiObRedGui : must Budokai red team is valid" );
		return NULL;
	}


	if( pRedTeam->vecpMember.size() <= 0 )
		return NULL;

	VEC_TBTEAMMEMBER* pMembers = &pRedTeam->vecpMember;
	stTBudokaiMember* pMember = reinterpret_cast<stTBudokaiMember*>(pMembers->front());

	return pMember;
}
