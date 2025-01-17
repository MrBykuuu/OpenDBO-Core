#include "precomp_dboclient.h"

// Core
#include "CEventHandler.h"

// Shared 
#include "NtlBudokai.h"

// Simulation
#include "NtlSLLogic.h"

// Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// Table
#include "BudokaiTable.h"
#include "TableContainer.h"

// Dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "AlarmManager.h"
#include "TBNewsGui.h"
#include "TBRequestGui.h"

// Gui
#include "GuiUtil.h"

/**
* \brief Construction
* \param pName	(const RwChar*) Name of the GUI
*/
CTBNewsGui::CTBNewsGui( const RwChar* pName )
: CNtlPLGui(pName)
{
	m_byCurFirstBox = 0;
	m_uiMudosaReaminTime = INVALID_DWORD;
	m_bMudosaUpdate = FALSE;
}

/**
* \brief Destruction
*/
CTBNewsGui::~CTBNewsGui()
{
	
}

/**
* \brief Create
*/
RwBool CTBNewsGui::Create()
{
	NTL_FUNCTION("CTBNewsGui::Create");

	if(!CNtlPLGui::Create("", dTBNEWS_GUI_SURFACE_FILE, dTBNEWS_GUI_FRAMEWORK_FILE))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis							= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_TB_NEWS );

	m_pStbTitle						= (gui::CStaticBox*)GetComponent("stbNewsTitle");
	m_pStbHeadLine					= (gui::CStaticBox*)GetComponent("stbNewsSubTitle");
	m_pStbNewsMark					= (gui::CStaticBox*)GetComponent("stbNewsMark");
	m_pPnlImage						= (gui::CPanel*)GetComponent("pnlNewsImage");
	m_pHtmlNews						= (gui::CHtmlBox*)GetComponent("htmlNews");
	
	m_pBtnClose						= (gui::CButton*)GetComponent("btnClose");
	m_pBtnSoloRequest				= (gui::CButton*)GetComponent("btnSoloRequest");
	m_pBtnSoloRequest->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_BUTTON_INDIREQ" ) );
	m_pBtnPartyRequest				= (gui::CButton*)GetComponent("btnPartyRequest");
	m_pBtnPartyRequest->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_BUTTON_TEAMREQ" ) );
	m_pBtnLicense					= (gui::CButton*)GetComponent("btnLicense");
	m_pBtnLicense->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_BUTTON_TICKET" ) );
	m_pBtnTournement				= (gui::CButton*)GetComponent("btnTournement");
	m_pBtnTournement->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_BUTTON_TOURNAMENT" ) );


	for( RwUInt8 i = TL_1; i < TL_NUMS; ++i )
	{
		// Input 
		char acBuffer[64];
		sprintf_s( acBuffer, 64, "stbNoticeText_%d", i+1 );
		m_pStbNoticeText[i] = (gui::CStaticBox*)GetComponent(acBuffer);
	}

	m_pBtnTournement->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_BUTTON_TOURNAMENT" ) );

	m_pDlgInfo						= (gui::CDialog*)GetComponent("dlgInfo");
	m_paStbInfoBox[0]				= (gui::CStaticBox*)GetComponent("stbInfo0");
	m_paStbInfoBox[1]				= (gui::CStaticBox*)GetComponent("stbInfo1");

	m_slotClickedBtnClose			= m_pBtnClose->SigClicked().Connect( this, &CTBNewsGui::OnClickedBtnClose );
	m_slotClickedBtnSoloRequest		= m_pBtnSoloRequest->SigClicked().Connect( this, &CTBNewsGui::OnClickedBtnSoloRequest );
	m_slotClickedBtnPartyRequest	= m_pBtnPartyRequest->SigClicked().Connect( this, &CTBNewsGui::OnClickedBtnPartyRequest );
	m_slotClickedBtnLicense			= m_pBtnLicense->SigClicked().Connect( this, &CTBNewsGui::OnClickedBtnLicense );
	m_slotClickedBtnTournement		= m_pBtnTournement->SigClicked().Connect( this, &CTBNewsGui::OnClickedBtnTournenment );

	GetNtlGuiManager()->AddUpdateFunc( this );

	Show( false );

	LinkMsg( g_EventBudokaiUpdateStateNfy, 0 );
	LinkMsg( g_EventBudokaiUpdateMatchStateNfy, 0 );
	LinkMsg( g_EventBudokaiJoinIndividualRes, 0 );
	LinkMsg( g_EventBudokaiLeaveIndividualRes, 0 );
	LinkMsg( g_EventBudokaiJoinTeamRes, 0 );
	LinkMsg( g_EventBudokaiJoinTeamNfy, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamRes, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamNfy, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamMemberRes, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamMemberNfy, 0 );
	LinkMsg( g_EventBudokaiJoinStateRes, 0 );
	LinkMsg( g_EventBudokaiJoinStateNfy, 0 );
	LinkMsg( g_EventBudokaiPrizeWinnerName, 0 );

	m_aBoxPresentInfo[0] = INFO_STATE;
	m_aBoxPresentInfo[1] = INFO_INDITIME;

	for( RwInt32 i = 0; i < dTBNEWS_INFO_BOX_NUMS; ++i )
	{
		SetInfoBox( m_aBoxPresentInfo[i], m_paStbInfoBox[i] );
	}

	InitInfoBox();

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CTBNewsGui::Destroy()
{
	NTL_FUNCTION("CTBNewsGui::Destroy");

	UnLinkMsg( g_EventBudokaiUpdateStateNfy );
	UnLinkMsg( g_EventBudokaiUpdateMatchStateNfy );
	UnLinkMsg( g_EventBudokaiJoinIndividualRes );
	UnLinkMsg( g_EventBudokaiLeaveIndividualRes );
	UnLinkMsg( g_EventBudokaiJoinTeamRes );
	UnLinkMsg( g_EventBudokaiJoinTeamNfy );
	UnLinkMsg( g_EventBudokaiLeaveTeamRes );
	UnLinkMsg( g_EventBudokaiLeaveTeamNfy );
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberRes );
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberNfy );
	UnLinkMsg( g_EventBudokaiJoinStateRes );
	UnLinkMsg( g_EventBudokaiJoinStateNfy );
	UnLinkMsg( g_EventBudokaiPrizeWinnerName );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CTBNewsGui::Update( RwReal fElapsed )
{
	if( IsShow() )
	{
		m_pHtmlNews->Update( fElapsed );
		
		CRectangle rtDialog = m_pDlgInfo->GetScreenRect();
		CRectangle rtInfoBox0 = m_paStbInfoBox[0]->GetScreenRect();
		CRectangle rtInfoBox1 = m_paStbInfoBox[1]->GetScreenRect();

		RwReal fMoveLefe = fElapsed * (RwReal)g_nTestVal;

		m_fBoxPresentPosX[0] -= (RwInt32)fMoveLefe;
		m_fBoxPresentPosX[1] -= (RwInt32)fMoveLefe;
		
		m_paStbInfoBox[0]->SetPosition( (RwInt32)m_fBoxPresentPosX[0], 0 );
		m_paStbInfoBox[1]->SetPosition( (RwInt32)m_fBoxPresentPosX[1], 0 );

		for( RwInt32 i=0; i< dTBNEWS_INFO_BOX_NUMS; ++i )
		{
			if( m_paStbInfoBox[i]->GetScreenRect().right < m_pDlgInfo->GetScreenRect().left )
			{
				RwInt32 nOtherBoxNum = (RwInt32)((i+1) % dTBNEWS_INFO_BOX_NUMS);
				RwInt32 nNewPosBackAnyBox = (m_paStbInfoBox[nOtherBoxNum]->GetScreenRect().left - m_pDlgInfo->GetScreenRect().left) + m_paStbInfoBox[nOtherBoxNum]->GetScreenRect().GetWidth();
				m_fBoxPresentPosX[i] = (RwReal) nNewPosBackAnyBox;
				m_paStbInfoBox[i]->SetPosition( nNewPosBackAnyBox, 0 );
				m_byCurFirstBox = (RwUInt8)((i+1) % dTBNEWS_INFO_BOX_NUMS);

				m_aBoxPresentInfo[i] = (m_aBoxPresentInfo[nOtherBoxNum]+1);
				m_aBoxPresentInfo[i] = (RwUInt8)(m_aBoxPresentInfo[i] % INFO_NUMS);

				SetInfoBox( m_aBoxPresentInfo[i], m_paStbInfoBox[i] );
			}
		}

		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		sBUDOKAI_JOIN_INFO* pJoinInfo;
		sBUDOKAI_UPDATE_STATE_INFO*	pMainStateInfo;
		sBUDOKAI_UPDATE_MATCH_STATE_INFO* pIndividualMatch;
		sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch;

		// If NULL, the information was received before avatar creation.
		// If it is not NULL, the data is set normally.
		if( pSobAvatar == NULL )
		{
			pMainStateInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo;
			pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
			pIndividualMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
			pTeamMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];
		}
		else
		{
			pMainStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
			pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
			pIndividualMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
			pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
		}

		if( pIndividualMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END &&
			pTeamMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END &&
			IsBudokaiOpen( pMainStateInfo->byState ) )
		{
			// Entry to Martial Artist Village ends: %s remain.
			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_MUDOSA_ENTER_LEFT_TIME" ), 
				pIndividualMatch->tmRemainTime > pTeamMatch->tmRemainTime ? Logic_GetTBRemainTimeString( pIndividualMatch->tmRemainTime ) : Logic_GetTBRemainTimeString( pTeamMatch->tmRemainTime ) );
			m_pStbNoticeText[TL_4]->SetText( awcBuffer );
		}
	}
}

/**
* \brief SwitchDialog
*/
RwInt32 CTBNewsGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		UpdateInterface();
		UpdateButtons();
		
		// Whether the information in the information box is updated or not
		UpdateInstanceInfo();
	}
	else
	{
		// When you close the newsletter, the application form also closes.
		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
			GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
	}

	return TRUE;
}

/**
* \brief HandleEvents
*/
void CTBNewsGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>(msg.pData);

		if( IsBudokaiOpen( pNotify->sStateInfo.byState ) )
			return;

		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
		{
			UpdateInterface();
			UpdateButtons();
		}
	}
	// When the state of the match changes, the news is set and updated.
	else if( msg.Id == g_EventBudokaiUpdateMatchStateNfy )
	{
	
		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
		{
			UpdateInterface();
			UpdateButtons();
		}
	}
	else if( msg.Id == g_EventBudokaiJoinIndividualRes ||
		msg.Id == g_EventBudokaiLeaveIndividualRes ||
		msg.Id ==  g_EventBudokaiJoinTeamRes || 
		msg.Id == g_EventBudokaiJoinTeamNfy ||
		msg.Id == g_EventBudokaiLeaveTeamRes ||
		msg.Id == g_EventBudokaiLeaveTeamMemberRes )		
	{
		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
		{
			UpdateInterface();
			UpdateButtons();
		}
	}
	else if( msg.Id == g_EventBudokaiLeaveTeamNfy )
	{
		// The team leader canceled the application for the World's Best Martial Arts Club.
		GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_TEAM_LEAVE_NOTIFY" );

		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
		{
			UpdateInterface();
			UpdateButtons();
		}
	}
	else if( msg.Id == g_EventBudokaiLeaveTeamMemberNfy )
	{
		SNtlEventBudokaiLeaveTeamMemberNfy* pNotify = reinterpret_cast<SNtlEventBudokaiLeaveTeamMemberNfy*>( msg.pData );

		WCHAR awcBuffer[256];
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAM_MEMBER_LEAVE_NOTIFY" ), pNotify->pwcMemberName );

		GetAlarmManager()->AlarmMessage( awcBuffer );
	}
	else if( msg.Id == g_EventBudokaiPrizeWinnerName )
	{
		SDboEventBudokaiPrizeWinnerName* pData = reinterpret_cast< SDboEventBudokaiPrizeWinnerName* >( msg.pData );

		if( !IsShow() )
			return;

		WCHAR awcWinnerName[NTL_MAX_SIZE_CHAR_NAME+1];
		WCHAR awcSecondWinnerName[NTL_MAX_SIZE_CHAR_NAME+1];

		::ZeroMemory( awcWinnerName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );
		::ZeroMemory( awcSecondWinnerName, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME+1 );

		// get sVARIABLE_DATA
		sVARIABLE_DATA* pVarData = reinterpret_cast<sVARIABLE_DATA*>(pData->pData);

		// winner
		int nWinnerSize = pVarData->GetSize( pData->wWinner );
		::CopyMemory( awcWinnerName, pVarData->Read( pData->wWinner ), nWinnerSize );

		// Runner-up Name
		int nSecondWinnerSize = pVarData->GetSize( pData->wSecondWinner );
		::CopyMemory( awcSecondWinnerName, pVarData->Read( pData->wSecondWinner ), nSecondWinnerSize );

		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		sBUDOKAI_JOIN_INFO* pJoinInfo;
		sBUDOKAI_UPDATE_STATE_INFO*	pMainStateInfo;
		sBUDOKAI_UPDATE_MATCH_STATE_INFO* pIndividualMatch;
		sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch;

		// If NULL, the information was received before avatar creation.
		// If it is not NULL, the data is set normally.
		if( pSobAvatar == NULL )
		{
			pMainStateInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo;
			pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
			pIndividualMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
			pTeamMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];
		}
		else
		{
			pMainStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
			pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
			pIndividualMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
			pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
		}


		if( pData->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
		{
			if( pIndividualMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END &&
				pTeamMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END )
			{
				SetPrizeText( NIT_STATE_END, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, m_pStbNoticeText[TL_1], awcWinnerName, awcSecondWinnerName );
			}
			else if( pIndividualMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END )
			{
				SetPrizeText( NIT_STATE_GUIDE, BUDOKAI_MATCH_TYPE_INDIVIDIAUL, m_pStbNoticeText[TL_2], awcWinnerName, awcSecondWinnerName );
			}
		}
		else
		{
			if( pIndividualMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END &&
				pTeamMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END )
			{
				SetPrizeText( NIT_STATE_END, BUDOKAI_MATCH_TYPE_TEAM, m_pStbNoticeText[TL_2], awcWinnerName, awcSecondWinnerName );
			}
			else if( pTeamMatch->byState == BUDOKAI_MATCHSTATE_MATCH_END )
			{
				SetPrizeText( NIT_STATE_GUIDE, BUDOKAI_MATCH_TYPE_TEAM, m_pStbNoticeText[TL_4], awcWinnerName, awcSecondWinnerName );
			}
		}
	}
}

/**
* \brief User interface settings
*/
void CTBNewsGui::UpdateInterface()
{
	m_pThis->SetPosition( ( GetDboGlobal()->GetScreenWidth()-m_pThis->GetWidth() )/2,
		( GetDboGlobal()->GetScreenHeight()-m_pThis->GetHeight() )/2 );

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sBUDOKAI_JOIN_INFO* pJoinInfo;
	sBUDOKAI_UPDATE_STATE_INFO*	pMainStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pIndividualMatch;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch;
	
	// If NULL, the information was received before avatar creation.
	// If it is not NULL, the data is set normally.
	if( pSobAvatar == NULL )
	{
		pMainStateInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo;
		pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
		pIndividualMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
		pTeamMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];
	}
	else
	{
		pMainStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
		pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
		pIndividualMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
		pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
	}

	// %d World's Best Ballroom
	WCHAR awcBuffer[128];
	swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_TITLE" ), pMainStateInfo->wSeasonCount );
	m_pStbTitle->SetText( awcBuffer );

	// The best ballroom table in the world
	CBudokaiTable* pBudokaiTable = API_GetTableContainer()->GetBudokaiTable();
	if( !pBudokaiTable )
	{
		DBO_FAIL("CTBNewsGui::SetUserInterface - Must valid budokai table");
		return;
	}
	
	// Budokai table information
	sBUDOKAI_TBLINFO* pBudokaiTblInfo = pBudokaiTable->GetBudokaiTblInfo();
	if( !pBudokaiTblInfo )
	{
		DBO_FAIL("CTBNewsGui::SetUserInterface - Must valid budokai table info");
		return;
	}

	// Update according to main state
	switch( pMainStateInfo->byState )
	{
		// Notice
	case BUDOKAI_STATE_JUNIOR_OPEN_NOTICE:
	case BUDOKAI_STATE_OPEN_NOTICE:
		{
			UpdateNoticeImage( NIT_STATE_GUIDE );

			// Find the preliminary application period
			tm tmMinorStart;
			tm tmMinorEnd;
			tm tmDojoStart;
			tm tmDojoEnd;

			m_pStbHeadLine->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_HEADLINE_OPENNOTICE" ) );
			m_pPnlImage->ClearSurface();
			m_pPnlImage->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsOpenNotice" ) );
			std::wstring wstrHtmlFile = dTBNEWS_SCRIPT_FILE_FOLDER;
			wstrHtmlFile += pBudokaiTblInfo->sNoticeFile.wszOpenNotice;
			m_pHtmlNews->SetHtmlFromFile( wstrHtmlFile.c_str() );

			errno_t errid = _localtime32_s( &tmDojoStart, (__time32_t*)&pMainStateInfo->tmNextStepTime );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - "<< pMainStateInfo->tmNextStepTime );
			}

			time_t timeMinorStart = pMainStateInfo->tmNextStepTime + pBudokaiTblInfo->dwDojoRecommendTime;
			errid = _localtime32_s( &tmMinorStart, (__time32_t*)&timeMinorStart );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - "<< timeMinorStart );
			}

			tmDojoEnd = tmMinorStart;

			time_t timeMinorEnd = pMainStateInfo->tmNextStepTime + pBudokaiTblInfo->dwDojoRecommendTime + pBudokaiTblInfo->dwRegisterTime;
			errid = _localtime32_s( &tmMinorEnd, (__time32_t*)&timeMinorEnd );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateuserInterface - " << timeMinorEnd );
			}

			// Dojo recommendation period
			m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_DOJO_TITLE" ) );
			m_pStbNoticeText[TL_2]->SetText( Logic_GetTBDateToDateString( tmDojoStart, tmDojoEnd ) );

			// Preliminary application period
			m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_MINOR_TITLE" ) );
			m_pStbNoticeText[TL_4]->SetText( Logic_GetTBDateToDateString( tmMinorStart, tmMinorEnd ) );
		}
		break;
	case BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND:
	case BUDOKAI_STATE_DOJO_RECOMMEND:
		{
			UpdateNoticeImage( NIT_STATE_GUIDE );

			m_pStbHeadLine->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_HEADLINE_DOJORECOMMEND" ) );
			m_pPnlImage->ClearSurface();
			m_pPnlImage->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsDojoRecommend" ) );
			std::wstring wstrHtmlFile = dTBNEWS_SCRIPT_FILE_FOLDER;
			wstrHtmlFile += pBudokaiTblInfo->sNoticeFile.wszDojoRecommend;
			m_pHtmlNews->SetHtmlFromFile( wstrHtmlFile.c_str() );

			tm tmMinorStart;
			tm tmMinorEnd;
			tm tmDojoStart;
			tm tmDojoEnd;

			time_t timeDojoStart = pMainStateInfo->tmNextStepTime - pBudokaiTblInfo->dwDojoRecommendTime;
			time_t timeDojoEnd = pMainStateInfo->tmNextStepTime;
			time_t timeMinorStart = timeDojoEnd;
			time_t timeMinorEnd = timeDojoStart + pBudokaiTblInfo->dwRegisterTime;

			errno_t errid = _localtime32_s( &tmDojoStart, (__time32_t*)&timeDojoStart );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - " << timeDojoStart );
			}

			errid = _localtime32_s( &tmDojoEnd, (__time32_t*)&timeDojoEnd );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - " << timeDojoEnd );
			}

			errid = _localtime32_s( &tmMinorStart, (__time32_t*)&timeMinorStart );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - "<< timeMinorStart );
			}

			errid = _localtime32_s( &tmMinorEnd, (__time32_t*)&timeMinorEnd );
			if( 0 != errid )
			{
				DBO_FAIL( "UpdateInterface - "<< timeMinorEnd );
			}

			// Dojo recommendation period
			m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_DOJO_TITLE" ) );
			m_pStbNoticeText[TL_2]->SetText( Logic_GetTBDateToDateString( tmDojoStart, tmDojoEnd ) );

			// Preliminary application period
			m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_MINOR_TITLE" ) );
			m_pStbNoticeText[TL_4]->SetText( Logic_GetTBDateToDateString( tmMinorStart, tmMinorEnd ) );
		}
		break;
	case BUDOKAI_STATE_JUNIOR_OPEN:
	case BUDOKAI_STATE_OPEN:
	case BUDOKAI_STATE_JUNIOR_CLEAR:
	case BUDOKAI_STATE_CLEAR:
		{
			// Set HeadLine
			// Set Html
			SetNews( pIndividualMatch->byState, pTeamMatch->byState );
			
			// Set Upper Notice ,Lower Notice
			SetMatchNotice( pIndividualMatch->byState, pTeamMatch->byState );
		}
		break;
		
	default:
		break;
	}

	m_pStbNewsMark->Clear();
	m_pStbNewsMark->GetSurface()->clear();

	if( GetBudokaiType(pMainStateInfo->byState) == BUDOKAI_TYPE_ADULT )
	{
		m_pStbNewsMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TBNewsGui.srf", "srfTBNewsAdultTab" ) );
		// After completing the resource work, update the color to match.
		/*m_pStbNewsMark->SetText( GetDisplayStringManager()->GetString( DST_BUDOKAI_TYPE_ADULT_TITLE ) ); */
	}
	else
	{
		m_pStbNewsMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TBNewsGui.srf", "srfTBNewsChildTab" ) );
		/*m_pStbNewsMark->SetText( GetDisplayStringManager()->GetString( DST_BUDOKAI_TYPE_CHILD_TITLE ) ); */
	}
}

/**
* \brief Prints the appropriate button in the current state.
*/
void CTBNewsGui::UpdateButtons()
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	sBUDOKAI_JOIN_INFO* pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();
	sBUDOKAI_UPDATE_STATE_INFO* pMainStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pIndividualMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
	if( pJoinInfo == NULL )
		return;
	
	m_pBtnSoloRequest->Show( false );
	m_pBtnPartyRequest->Show( false );
	m_pBtnLicense->Show( false );
	m_pBtnTournement->Show( false );

	if( pMainStateInfo->byState != BUDOKAI_STATE_OPEN &&
		pMainStateInfo->byState != BUDOKAI_STATE_JUNIOR_OPEN )
		return;
	
	// Application button and receipt button
	if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_PLAY )
	{
		if( pIndividualMatch->byState != BUDOKAI_MATCHSTATE_MATCH_END &&
			pTeamMatch->byState != BUDOKAI_MATCHSTATE_MATCH_END )
		{
			m_pBtnLicense->Show( true );
		}
	}
	else if( pJoinInfo->byJoinState == INVALID_BUDOKAI_JOIN_STATE )
	{
		if( pIndividualMatch->byState == BUDOKAI_MATCHSTATE_REGISTER )
			m_pBtnSoloRequest->Show( true );		

		if( pTeamMatch->byState == BUDOKAI_MATCHSTATE_REGISTER )
			m_pBtnPartyRequest->Show( true );
	}

	// bracket button
	if( pIndividualMatch->byState >= BUDOKAI_MATCHSTATE_MAJORMATCH_32 &&
		pIndividualMatch->byState <= BUDOKAI_MATCHSTATE_MATCH_END)
	{
		m_pBtnTournement->Show( true );
	}
	else if( pTeamMatch->byState >= BUDOKAI_MATCHSTATE_MAJORMATCH_32 &&
		pTeamMatch->byState <= BUDOKAI_MATCHSTATE_MATCH_END )
	{
		m_pBtnTournement->Show( true );
	}
}

/**
* \brief Close
*/
void CTBNewsGui::OnClickedBtnClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_NEWS );
}

/**
* \brief Open application GUI
*/
void CTBNewsGui::OnClickedBtnSoloRequest( gui::CComponent* pComponent )
{
	CTBRequestGui* pRequestGui = (CTBRequestGui*)GetDialogManager()->GetDialog( DIALOG_BUDOKAI_REQUEST );
	if( !pRequestGui )
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	pRequestGui->SetRequestType( BUDOKAI_MATCH_TYPE_INDIVIDIAUL );
	GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_REQUEST );
}

void CTBNewsGui::OnClickedBtnPartyRequest( gui::CComponent* pComponent )
{
	if( !Logic_I_am_PartyLeader() )
	{
		// Only party leaders can apply.
		GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_NEWS_CAN_ONLY_LEADER" );
		return;
	}

	CTBRequestGui* pRequestGui = (CTBRequestGui*)GetDialogManager()->GetDialog( DIALOG_BUDOKAI_REQUEST );
	if( !pRequestGui )
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	pRequestGui->SetRequestType( BUDOKAI_MATCH_TYPE_TEAM );
	GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_REQUEST );
}

void CTBNewsGui::OnClickedBtnLicense( gui::CComponent* pComponent )
{
	CTBRequestGui* pRequestGui = (CTBRequestGui*)GetDialogManager()->GetDialog( DIALOG_BUDOKAI_REQUEST );
	if( !pRequestGui )
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	sBUDOKAI_JOIN_INFO* pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();
	if( pJoinInfo == NULL )
		return;

	if( pJoinInfo->byMatchType == INVALID_BUDOKAI_MATCH_TYPE )
		return;

	pRequestGui->SetRequestType( pJoinInfo->byMatchType );
	GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_REQUEST );
}

/**
* \brief Open Bracket GUI
*/
void CTBNewsGui::OnClickedBtnTournenment( gui::CComponent* pComponent )
{
	GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_TOURNAMENT );
}

/**
* \brief Initializes the location of the Info Box.
*/
void CTBNewsGui::InitInfoBox()
{
	CRectangle rtScreen = m_pDlgInfo->GetScreenRect();

	// Initialization of StaticBox position
	m_fBoxPresentPosX[0] = (RwReal)rtScreen.GetWidth();
	m_paStbInfoBox[0]->SetPosition( (RwInt32)m_fBoxPresentPosX[0], 0 );
	m_paStbInfoBox[0]->GetFitWidthToString();
	m_fBoxPresentPosX[1] = (RwReal)rtScreen.GetWidth()+(RwReal)m_paStbInfoBox[0]->GetScreenRect().GetWidth();
	m_paStbInfoBox[1]->SetPosition( (RwInt32)m_fBoxPresentPosX[1], 0 );
	m_paStbInfoBox[1]->GetFitWidthToString();
}

/**
* \brief Gets messages about the following status.
*/
WCHAR* CTBNewsGui::GetNextStepString( RwUInt8 byMatchType )
{
	static WCHAR awcBuffer[256] = L"";

	sBUDOKAI_UPDATE_STATE_INFO* pStateInfo = NULL;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pInfo = NULL;
	
	if( byMatchType == INVALID_BUDOKAI_MATCH_TYPE )
	{
		swprintf_s( awcBuffer, 256, L"%s", L"" );
		return awcBuffer;
	}

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
	{
		if( byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
			pInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
		else
			pInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];

		pStateInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo;
	}
	else
	{
		if( byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
			pInfo = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
		else
			pInfo = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();

		pStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
	}

	if( !IsBudokaiOpen( pStateInfo->byState ) )
	{
		swprintf_s( awcBuffer, 256, L"%s", L"" );
		return awcBuffer;
	}
	
	switch( pInfo->byState )
	{
	case BUDOKAI_MATCHSTATE_REGISTER:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_REGISTER" ),
			Logic_GetStringTBMatchType( byMatchType ), Logic_GetTBRemainTimeString( pInfo->tmRemainTime ));
		break;
	case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MINOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ), Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_MINOR_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_MINOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ));
		break;
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ),
			Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
		break;
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ),
			Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
		break;
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ),
			Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_MAJOR_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
		break;
	case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_SEMIFINAL_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ), Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_SEMIFINAL_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ) );
		break;
	case BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_WAIT_FINAL_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ), Logic_GetTBRemainTimeString( pInfo->tmRemainTime ) );
		break;
	case BUDOKAI_MATCHSTATE_FINAL_MATCH:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_FINAL_MATCH" ),
			Logic_GetStringTBMatchType( byMatchType ) );
		break;
	case BUDOKAI_MATCHSTATE_MATCH_END:
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_STATE_MATCH_END" ),
			Logic_GetStringTBMatchType( byMatchType ) );
		break;
	}

	return awcBuffer;
}

/**
* \brief Outputs text that matches the current state of the infobox using the number and component of the infobox.
* \param byInfoBoxNum (RwUInt8) Number of information box
* \param pStbInfoBox (gui::CStaticBox*) Pointer to the component.
*/
void CTBNewsGui::SetInfoBox( RwUInt8 byInfoBoxNum, gui::CStaticBox* pStbInfoBox )
{
	switch( byInfoBoxNum )
	{
	case INFO_STATE:
		{
			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			
			// If NULL, the information was received before avatar creation.
			// If it is not NULL, the data is set normally.
			sBUDOKAI_JOIN_INFO* pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();
			if( pSobAvatar == NULL )
				pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
			else
				pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	

			// apply button
			if( pJoinInfo->byJoinState == INVALID_BUDOKAI_JOIN_STATE )
			{
				pStbInfoBox->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_YOU_ARE_NOT_ENTRANT" ) );
			}
			else
			{
				WCHAR awcFinalResBuffer[256];
				if( pJoinInfo->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
					swprintf_s( awcFinalResBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_INDI_STATE" ), Logic_GetAvatarTBJoinState() );
				else
					swprintf_s( awcFinalResBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_INFO_TEAM_STATE" ), Logic_GetAvatarTBJoinState() );

				pStbInfoBox->SetText( awcFinalResBuffer );
			}

			RwInt32 nWidth = pStbInfoBox->GetFitWidthToString() + ( dTBNEWS_INFO_BOX_MARGIN*2 );
			if( nWidth < dTBNEWS_INFO_BOX_MIN_WIDTH )
				nWidth = dTBNEWS_INFO_BOX_MIN_WIDTH;
			pStbInfoBox->SetSize( nWidth, pStbInfoBox->GetScreenRect().GetHeight() );
		}
		break;
	case INFO_INDITIME:
		{
			pStbInfoBox->SetText( GetNextStepString( BUDOKAI_MATCH_TYPE_INDIVIDIAUL ) );	

			RwInt32 nWidth = pStbInfoBox->GetFitWidthToString() + ( dTBNEWS_INFO_BOX_MARGIN*2 );
			if( nWidth < dTBNEWS_INFO_BOX_MIN_WIDTH )
				nWidth = dTBNEWS_INFO_BOX_MIN_WIDTH;
			pStbInfoBox->SetSize( nWidth, pStbInfoBox->GetScreenRect().GetHeight() );
		}
		break;
	case INFO_TEAMTIME:
		{
			pStbInfoBox->SetText( GetNextStepString( BUDOKAI_MATCH_TYPE_TEAM ) );

			RwInt32 nWidth = pStbInfoBox->GetFitWidthToString() + ( dTBNEWS_INFO_BOX_MARGIN*2 );
			if( nWidth < dTBNEWS_INFO_BOX_MIN_WIDTH )
				nWidth = dTBNEWS_INFO_BOX_MIN_WIDTH;
			pStbInfoBox->SetSize( nWidth, pStbInfoBox->GetScreenRect().GetHeight() );
		}
		break;
	default:
		break;
	}
}

/**
* \brief If the text of the information currently displayed needs to be changed, change it and adjust the position.
*/
void CTBNewsGui::UpdateInstanceInfo()
{
	for( RwInt32 i = 0; i < dTBNEWS_INFO_BOX_NUMS; ++i )
	{
		RwInt32 nOldWidth = m_paStbInfoBox[i]->GetScreenRect().GetWidth();
		SetInfoBox( m_aBoxPresentInfo[i], m_paStbInfoBox[i] );

		// If the size has changed and the current box is the first box, the position of the box behind it is adjusted.
		if( nOldWidth != m_paStbInfoBox[i]->GetScreenRect().GetWidth() )
		{
			RwInt32 nOtherBox = (i+1) % dTBNEWS_INFO_BOX_NUMS;
			if( m_paStbInfoBox[i]->GetScreenRect().left < m_paStbInfoBox[nOtherBox]->GetScreenRect().left )
			{
				RwInt32 nParentLeft = m_pDlgInfo->GetScreenRect().left;
				RwInt32 nCurrentBoxLeft = m_paStbInfoBox[i]->GetScreenRect().left;
				
				// Position by parent ( current box )
				nCurrentBoxLeft -= nParentLeft;
		
				m_fBoxPresentPosX[nOtherBox] = (RwReal)(nCurrentBoxLeft + m_paStbInfoBox[i]->GetScreenRect().GetWidth());
				m_paStbInfoBox[nOtherBox]->SetPosition( (RwInt32)m_fBoxPresentPosX[nOtherBox], 0 );				
			}
		}
	}
}

void CTBNewsGui::SetNews( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState )
{
	// The best ballroom table in the world
	CBudokaiTable* pBudokaiTable = API_GetTableContainer()->GetBudokaiTable();
	if( !pBudokaiTable )
	{
		DBO_FAIL("CTBNewsGui::SetNews - Must valid budokai table");
		return;
	}

	// Budokai table information
	sBUDOKAI_TBLINFO* pBudokaiTblInfo = pBudokaiTable->GetBudokaiTblInfo();
	if( !pBudokaiTblInfo )
	{
		DBO_FAIL("CTBNewsGui::SetNews - Must valid budokai table info");
		return;
	}
	
	if( byIndiMatchState == BUDOKAI_MATCHSTATE_MATCH_END &&
		byTeamMatchState == BUDOKAI_MATCHSTATE_MATCH_END )
	{
		m_pStbHeadLine->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_HEADLINE_AWARD" ) );
		m_pPnlImage->ClearSurface();
		m_pPnlImage->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsAward" ) );

		std::wstring wstrHtmlFile = dTBNEWS_SCRIPT_FILE_FOLDER;
		wstrHtmlFile += pBudokaiTblInfo->sNoticeFile.wszAward;
		m_pHtmlNews->SetHtmlFromFile( wstrHtmlFile.c_str() );
	}
	else
	{
		if( byIndiMatchState == BUDOKAI_MATCHSTATE_REGISTER )
		{
			m_pStbHeadLine->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_HEADLINE_REGISTER" ) );
			m_pPnlImage->ClearSurface();
			m_pPnlImage->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsRequest" ) );
			std::wstring wstrHtmlFile = dTBNEWS_SCRIPT_FILE_FOLDER;
			wstrHtmlFile += pBudokaiTblInfo->sNoticeFile.wszRequest;
			m_pHtmlNews->SetHtmlFromFile( wstrHtmlFile.c_str() );
		}
		else
		{
			// Set headlines
			m_pStbHeadLine->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_HEADLINE_MAJOR" ) );

			// Random Html
			RwUInt8 byRandomNews = (RwUInt8)gui::RandomNumber( 0, BUDOKAI_NEW_FILE_COUNT-1 );

			CHAR acBuffer[256];
			sprintf_s( acBuffer, 256, "srfTBNewsRandom_%d", byRandomNews+1 );
			m_pPnlImage->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", acBuffer ) );

			std::wstring wstrHtmlFile = dTBNEWS_SCRIPT_FILE_FOLDER;
			wstrHtmlFile += pBudokaiTblInfo->sNoticeFile.awszNews[byRandomNews];
			m_pHtmlNews->SetHtmlFromFile( wstrHtmlFile.c_str() );
		}
	}
}

void CTBNewsGui::SetMatchNotice( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState )
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sBUDOKAI_JOIN_INFO* pJoinInfo;
	sBUDOKAI_UPDATE_STATE_INFO*	pMainStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pIndividualMatch;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch;

	// If NULL, the information was received before avatar creation.
	// If it is not NULL, the data is set normally.
	if( pSobAvatar == NULL )
	{
		pMainStateInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo;
		pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
		pIndividualMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
		pTeamMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];
	}
	else
	{
		pMainStateInfo = pSobAvatar->GetTenkaichiBudokai()->GetMainStateInfo();
		pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
		pIndividualMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
		pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
	}

	// The best ballroom table in the world
	CBudokaiTable* pBudokaiTable = API_GetTableContainer()->GetBudokaiTable();
	if( !pBudokaiTable )
	{
		DBO_FAIL("CTBNewsGui::SetNews - Must valid budokai table");
		return;
	}

	// Budokai table information
	sBUDOKAI_TBLINFO* pBudokaiTblInfo = pBudokaiTable->GetBudokaiTblInfo();
	if( !pBudokaiTblInfo )
	{
		DBO_FAIL("CTBNewsGui::SetNews - Must valid budokai table info");
		return;
	}

	if( byIndiMatchState == BUDOKAI_MATCHSTATE_MATCH_END &&
		byTeamMatchState == BUDOKAI_MATCHSTATE_MATCH_END )
	{
		UpdateNoticeImage( NIT_STATE_END );

		// Individual winner application
		GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiPrizeWinnerNameReq();

		// Application for team winner
		GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiPrizeTeamWinnerNameReq();

		BUDOKAITIME tmNextOpen = INVALID_DWORD;
		switch ( GetBudokaiType( pMainStateInfo->byState ) )
		{
		case BUDOKAI_TYPE_JUNIOR:
			tmNextOpen = CalcBudokaiNextStartTime( pIndividualMatch->tmNextStepTime, pBudokaiTblInfo->byOpenTerm, pBudokaiTblInfo->byOpenDayOfWeek, pBudokaiTblInfo->byOpenHour, pBudokaiTblInfo->byOpenMinute );
			break;
		case BUDOKAI_TYPE_ADULT:
			tmNextOpen = CalcBudokaiNextStartTime( pIndividualMatch->tmNextStepTime, pBudokaiTblInfo->byJiniorOpenTerm, pBudokaiTblInfo->byJiniorOpenDayOfWeek, pBudokaiTblInfo->byJiniorOpenHour, pBudokaiTblInfo->byJiniorOpenMinute );
			break;
		}
		
		if( tmNextOpen == INVALID_DWORD )
		{
			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NEXT_MUDOSA_OPEN" ), GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOPLAN" ) );
			m_pStbNoticeText[TL_3]->SetText( awcBuffer );
			return;
		}
		
		tm tmNextOpenNotice;
		errno_t errid = _localtime32_s( &tmNextOpenNotice, (__time32_t*)&tmNextOpen );
		if( 0 != errid )
		{
			DBO_FAIL( "SetMatchNotice - "<< tmNextOpen );
		}
		
		WCHAR awcBuffer[128];
		swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NEXT_MUDOSA_OPEN" ), Logic_GetTBDateString( tmNextOpenNotice ) );
		m_pStbNoticeText[TL_3]->SetText( awcBuffer );
						
		return;
	}

	// Upper notice announces the schedule depending on the status of the individual exhibition.
	if( byIndiMatchState == BUDOKAI_MATCHSTATE_REGISTER )
	{
		UpdateNoticeImage( NIT_STATE_GUIDE );

		m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_INDI_REGISTER" ) );
		
		if( pIndividualMatch->tmNextStepTime == 0 )
		{
			m_pStbNoticeText[TL_2]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
		}
		else
		{
			// Information on application for individual competition preliminary round
			tm tmWaitStart;
			tm tmWaitEnd;

			errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pIndividualMatch->tmNextStepTime );
			if( 0 != errid )
			{
				DBO_FAIL( "SetMatchNotice - "<< pIndividualMatch->tmNextStepTime );
			}

			time_t timeWaitStart = pIndividualMatch->tmNextStepTime - pBudokaiTblInfo->dwRegisterTime;
			errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
			if( 0 != errid )
			{
				DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
			}
			
			m_pStbNoticeText[TL_2]->SetText( Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
		}
	}
	else
	{
		switch( byIndiMatchState )
		{
		case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:		// Waiting for preliminaries			
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_INDI_STATE") );

				if( pIndividualMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_2]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{
					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pIndividualMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pIndividualMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pIndividualMatch->tmNextStepTime - pBudokaiTblInfo->dwMinorMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ), 
						Logic_GetStringTBMatchState( byIndiMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_2]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_INDI_STATE") );

				if( pIndividualMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_2]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{

					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pIndividualMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pIndividualMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pIndividualMatch->tmNextStepTime - pBudokaiTblInfo->dwMajorMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ), 
						Logic_GetStringTBMatchState( byIndiMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_2]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:	// waiting for semifinals
		case BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH:		// Wait for final: Not used
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_INDI_STATE") );
				
				if( pIndividualMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_2]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{
					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pIndividualMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pIndividualMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pIndividualMatch->tmNextStepTime - pBudokaiTblInfo->dwFinalMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ), 
						Logic_GetStringTBMatchState( byIndiMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_2]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
		case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:		// semifinal
		case BUDOKAI_MATCHSTATE_MINOR_MATCH:			// preliminaries 
		case BUDOKAI_MATCHSTATE_FINAL_MATCH:			// finals
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_INDI_STATE") );
				m_pStbNoticeText[TL_2]->SetText( Logic_GetStringTBMatchState( byIndiMatchState ) );
			}
			break;
		case BUDOKAI_MATCHSTATE_MATCH_END:				// end. grace time
			m_pStbNoticeText[TL_1]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_INDI_WINNER") );
			m_pStbNoticeText[TL_2]->Clear();
			//  packet request
			GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiPrizeWinnerNameReq();
			break;
		}
	}

	if( byTeamMatchState == BUDOKAI_MATCHSTATE_REGISTER )
	{
		UpdateNoticeImage( NIT_STATE_GUIDE );

		m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_TEAM_REGISTER" ) );

		if( pTeamMatch->tmNextStepTime == 0 )
		{
			m_pStbNoticeText[TL_4]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
		}
		else
		{
			// Information on application for individual competition preliminary round
			tm tmWaitStart;
			tm tmWaitEnd;

			errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pTeamMatch->tmNextStepTime );
			if( 0 != errid )
			{
				DBO_FAIL( "SetMatchNotice - "<< pTeamMatch->tmNextStepTime );
			}

			time_t timeWaitStart = pTeamMatch->tmNextStepTime - pBudokaiTblInfo->dwRegisterTime;
			errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
			if( 0 != errid )
			{
				DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
			}

			m_pStbNoticeText[TL_4]->SetText( Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
		}
	}
	else
	{
		switch( byTeamMatchState )
		{
		case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:		// Waiting for preliminaries
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_TEAM_STATE") );
				if( pTeamMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_4]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{

					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pTeamMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pTeamMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pTeamMatch->tmNextStepTime - pBudokaiTblInfo->dwMinorMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ),
						Logic_GetStringTBMatchState( byTeamMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_4]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_TEAM_STATE") );

				if( pTeamMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_4]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{

					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pTeamMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pTeamMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pTeamMatch->tmNextStepTime - pBudokaiTblInfo->dwMajorMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ),
						Logic_GetStringTBMatchState( byTeamMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_4]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:	// waiting for semifinals
		case BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH:		// Wait for final: Not used
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_TEAM_STATE") );

				if( pTeamMatch->tmNextStepTime == 0 )
				{
					m_pStbNoticeText[TL_4]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DATA_LOADING" ) );
				}
				else
				{
					tm tmWaitStart;
					tm tmWaitEnd;

					errno_t errid = _localtime32_s( &tmWaitEnd, (__time32_t*)&pTeamMatch->tmNextStepTime );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< pTeamMatch->tmNextStepTime );
					}

					time_t timeWaitStart = pTeamMatch->tmNextStepTime - pBudokaiTblInfo->dwFinalMatch_WaitTime;
					errid = _localtime32_s( &tmWaitStart, (__time32_t*)&timeWaitStart );
					if( 0 != errid )
					{
						DBO_FAIL( "SetMatchNotice - "<< timeWaitStart );
					}

					WCHAR awcBuffer[512];
					swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE" ),
						Logic_GetStringTBMatchState( byTeamMatchState ), Logic_GetTBDateToDateString( tmWaitStart, tmWaitEnd ) );
					m_pStbNoticeText[TL_4]->SetText( awcBuffer );
				}
			}
			break;
		case BUDOKAI_MATCHSTATE_MINOR_MATCH:				// preliminaries 
		case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:			// semifinal
		case BUDOKAI_MATCHSTATE_FINAL_MATCH:				// finals
			{
				UpdateNoticeImage( NIT_STATE_GUIDE );

				m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_TEAM_STATE") );
				m_pStbNoticeText[TL_4]->SetText( Logic_GetStringTBMatchState( byTeamMatchState ) );
			}
			break;
		case BUDOKAI_MATCHSTATE_MATCH_END:				// end. grace time
			UpdateNoticeImage( NIT_STATE_GUIDE );

			m_pStbNoticeText[TL_3]->SetText( GetDisplayStringManager()->GetString("DST_BUDOKAI_NEWS_NOTICE_TEAM_WINNER") );
			m_pStbNoticeText[TL_4]->Clear();
			GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiPrizeTeamWinnerNameReq();
			break;
		}
	}
}

void CTBNewsGui::SetPrizeText( eNoticeType eNitType, RwUInt8 byMatchType, gui::CStaticBox* pComponent, const WCHAR* awcWinner, const WCHAR* awcSecondWinner )
{
	RwUInt32 uiWinnerLength = (RwUInt32)wcslen( awcWinner );
	RwUInt32 uiSecondWinnerLength = (RwUInt32)wcslen( awcSecondWinner );

	if( eNitType == NIT_STATE_GUIDE )
	{
		std::string uiStrPrizeWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_WINNER";
		std::string uiStrPrizeSecondWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_SECOND_WINNER";
		std::string uiStrNotPrize = "DST_BUDOKAI_NEWS_NOTICE_NOT_PRIZE";

		if( byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
		{
			uiStrPrizeWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_WINNER";
			uiStrPrizeSecondWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_SECOND_WINNER";
			uiStrNotPrize = "DST_BUDOKAI_NEWS_NTOCIE_NOT_PRIZE_TEAM";
		}

		if( uiWinnerLength > 0 && uiSecondWinnerLength > 0 )
		{
			// output all
			WCHAR awcPrizeWinner[128];
			swprintf_s( awcPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeWinner ), awcWinner );

			WCHAR awcSecondPrizeWinner[128];
			swprintf_s( awcSecondPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeSecondWinner ), awcSecondWinner );

			WCHAR awcText[256];
			swprintf_s( awcText, 256, L"%s, %s", awcPrizeWinner, awcSecondPrizeWinner );
			pComponent->SetText( awcText );

		}
		else if( uiWinnerLength > 0 )
		{
			// Print only winners
			// output all
			WCHAR awcPrizeWinner[128];
			swprintf_s( awcPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeWinner ), awcWinner );
			pComponent->SetText( awcPrizeWinner );
		}
		else
		{
			// There are no winners.
			pComponent->SetText( GetDisplayStringManager()->GetString( uiStrNotPrize ) );
		}
	}
	else if( eNitType == NIT_STATE_END )
	{
		std::string uiStrPrizeWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_WINNER";
		std::string uiStrPrizeSecondWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_SECOND_WINNER";
		std::string uiStrNotPrize = "DST_BUDOKAI_NEWS_NOTICE_NOT_PRIZE";
		std::string uiMatchType = "DST_BUDOKAI_NOTICE_SOLO";

		if( byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
		{
			uiStrPrizeWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_WINNER";
			uiStrPrizeSecondWinner = "DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_SECOND_WINNER";
			uiStrNotPrize = "DST_BUDOKAI_NEWS_NTOCIE_NOT_PRIZE_TEAM";
			uiMatchType = "DST_BUDOKAI_NOTICE_PARTY";
		}

		// Individual Exhibition: Winner/name, Runner-up/name
		// %s: %s / %s, %s / %s
		if( uiWinnerLength > 0 && uiSecondWinnerLength > 0 )
		{
			WCHAR awcPrizeWinner[128];
			swprintf_s( awcPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeWinner ), awcWinner );

			WCHAR awcSecondPrizeWinner[128];
			swprintf_s( awcSecondPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeSecondWinner ), awcSecondWinner );

			WCHAR awcNotice[256];
			swprintf_s( awcNotice, L"%s : %s, %s", GetDisplayStringManager()->GetString( uiMatchType ), awcPrizeWinner, awcSecondPrizeWinner );
			pComponent->SetText( awcNotice );

		}
		else if( uiWinnerLength > 0 )
		{
			WCHAR awcPrizeWinner[128];
			swprintf_s( awcPrizeWinner, 128, GetDisplayStringManager()->GetString( uiStrPrizeWinner ), awcWinner );

			WCHAR awcNotice[256];
			swprintf_s( awcNotice, L"%s : %s", GetDisplayStringManager()->GetString( uiMatchType ), awcPrizeWinner );
			pComponent->SetText( awcNotice );
		}
		else
		{
			WCHAR awcNotice[256];
			swprintf_s( awcNotice, L"%s : %s", GetDisplayStringManager()->GetString( uiMatchType ), GetDisplayStringManager()->GetString( uiStrNotPrize ) );
			pComponent->SetText( awcNotice );
		}
	}
}

void CTBNewsGui::UpdateNoticeImage( eNoticeType eNitType )
{
	for(int i=TL_1; i < TL_NUMS; ++i )
		m_pStbNoticeText[i]->GetSurface()->clear();
	
	if( eNitType == NIT_STATE_GUIDE )
	{
		m_pStbNoticeText[TL_1]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsNoticeCategoryBack" ) );
		m_pStbNoticeText[TL_1]->SetTextColor( RGB( 255, 0, 0 ) );
		m_pStbNoticeText[TL_1]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_2]->SetTextColor( RGB( 0, 0, 0 ) );
		m_pStbNoticeText[TL_2]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_3]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsNoticeCategoryBack" ) );
		m_pStbNoticeText[TL_3]->SetTextColor( RGB( 255, 0, 0 ) );
		m_pStbNoticeText[TL_3]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_4]->SetTextColor( RGB( 0, 0, 0 ) );
		m_pStbNoticeText[TL_4]->SetTextStyle( COMP_TEXT_CENTER );
	}
	else if( eNitType == NIT_STATE_END )
	{
		m_pStbNoticeText[TL_1]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsNoticeCategoryBack" ) );
		m_pStbNoticeText[TL_1]->SetTextColor( RGB( 255, 255, 255 ) );
		m_pStbNoticeText[TL_1]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_2]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TBNewsGui.srf", "srfTBNewsNoticeCategoryBack" ) );
		m_pStbNoticeText[TL_2]->SetTextColor( RGB( 255, 255, 255 ) );
		m_pStbNoticeText[TL_2]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_3]->SetTextColor( RGB( 0, 0, 0 ) );
		m_pStbNoticeText[TL_3]->SetTextStyle( COMP_TEXT_CENTER );
		m_pStbNoticeText[TL_4]->SetTextColor( RGB( 0, 0, 0 ) );
		m_pStbNoticeText[TL_4]->SetTextStyle( COMP_TEXT_CENTER );
	}
}