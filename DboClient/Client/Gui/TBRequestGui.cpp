#include "precomp_dboclient.h"

// Core
#include "CEventHandler.h"

// Shared 
#include "NtlBudokai.h"

// Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "NtlPLEmblemMaker.h"

// Simulation
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobCharProxy.h"
#include "NtlSobCharProxyDecoration.h"

// Table
#include "BudokaiTable.h"
#include "TableContainer.h"

// Dbo
#include "DboDef.h"
#include "DboEvent.h"
#include "DialogDefine.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "AlarmManager.h"
#include "TBRequestGui.h"

/**
* \brief Construction
* \param pName	(const RwChar*) Name of the GUI
*/
CTBRequestGui::CTBRequestGui( const RwChar* pName )
: CNtlPLGui(pName)
{
	m_byGuiType			= TYPE_REQUEST;
	m_byRequestType		= REQUEST_INDIVIDUAL;
}

/**
* \brief Destruction
*/
CTBRequestGui::~CTBRequestGui()
{
	
}

/**
* \brief Create
*/
RwBool CTBRequestGui::Create()
{
	NTL_FUNCTION("CTBRequestGui::Create");

	if(!CNtlPLGui::Create("", dTBREQUEST_GUI_SURFACE_FILE, dTBREQUEST_GUI_FRAMEWORK_FILE))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis							= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_TB_REQUEST );
	
	// Common Gui
	m_pStbTitle		= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pBtnClose		= (gui::CButton*)GetComponent("btnClose");
	m_pBtnAccept	= (gui::CButton*)GetComponent("btnAccept");
	/*m_pBtnCancel	= (gui::CButton*)GetComponent("btnCancel");*/

	m_pBtnAccept->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQ_BUTTON_ACCEPT" ) );
	/*m_pBtnCancel->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQ_BUTTON_CANCEL" ) );*/

	m_slotClickedBtnClose	= m_pBtnClose->SigClicked().Connect( this, &CTBRequestGui::OnClickedBtnClose );
	m_slotClickedBtnAccept	= m_pBtnAccept->SigClicked().Connect( this, &CTBRequestGui::OnClickedBtnAccept );
	/*m_slotClickedBtnCancel	= m_pBtnCancel->SigClicked().Connect( this, &CTBRequestGui::OnClickedBtnCancel );*/

	// Request Individual
	m_pDlgIndividual		= (gui::CDialog*)GetComponent("dlgIndividual");
	m_pStbIndiName			= (gui::CStaticBox*)GetComponent("stbIndiName");
	m_pStbIndiNameDisplay	= (gui::CStaticBox*)GetComponent("stbIndiNameDisplay");
	m_pStbIndiLevel			= (gui::CStaticBox*)GetComponent("stbIndiLevel");
	m_pStbIndiLevelDisplay	= (gui::CStaticBox*)GetComponent("stbIndiLevelDisplay");
	m_pStbIndiClass			= (gui::CStaticBox*)GetComponent("stbIndiClass");
	m_pStbIndiClassDisplay	= (gui::CStaticBox*)GetComponent("stbIndiClassDisplay");
	m_pStbIndiRecord		= (gui::CStaticBox*)GetComponent("stbIndiRecord");
	m_pStbIndiRecordDisplay	= (gui::CStaticBox*)GetComponent("stbIndiRecordDisplay");
	m_pStbIndiRankingPoint	= (gui::CStaticBox*)GetComponent("stbIndiRankingPoint");
	m_pStbIndiRankingPointDisplay = (gui::CStaticBox*)GetComponent("stbIndiRankingPointDisplay");
	m_pStbIndiGuild			= (gui::CStaticBox*)GetComponent("stbIndiGuild");
	m_pStbIndiGuildDisplay	= (gui::CStaticBox*)GetComponent("stbIndiGuildDisplay");

	m_pDlgTeam				= (gui::CDialog*)GetComponent("dlgTeam");
	m_pStbTeamName			= (gui::CStaticBox*)GetComponent("stbTeamName");
	m_pIpbTeamName			= (gui::CInputBox*)GetComponent("ipbTeamName");
	m_pIpbTeamName->SetMaxLength(NTL_MAX_SIZE_PARTY_NAME);
	m_pIpbTeamName->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);

	m_pStbIndiName->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_NAME" ) );
	m_pStbIndiLevel->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_LEVEL" ) );
	m_pStbIndiClass->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_CLASS" ) );
	m_pStbIndiRecord->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_RECORD" ) );
	m_pStbIndiRankingPoint->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_RANKING" ) );
	m_pStbIndiGuild->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_GUILD" ) );

	m_pStbTeamName->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAM_REQ_TEAMNAME" ) );
	
	char acBuffer[512];
	for( int i=0; i < dTBREQUEST_TEAM_MEMBER; ++i )
	{
		sprintf_s( acBuffer, 512, "pnlMemberClass%d", i );
		m_paMemberClass[i]			= (gui::CPanel*)GetComponent( acBuffer );

		sprintf_s( acBuffer, 512, "stbMemberName%d", i );
		m_paMemberName[i]			= (gui::CStaticBox*)GetComponent( acBuffer );

		sprintf_s( acBuffer, 512, "stbMemberLevel%d", i );
		m_paMemberLevel[i]			= (gui::CStaticBox*)GetComponent( acBuffer );

		sprintf_s( acBuffer, 512, "stbMemberRankingPoint%d", i );
		m_paMemberRankingPoint[i]	= (gui::CStaticBox*)GetComponent( acBuffer );
	}

	// Individual exhibition receipt
	m_pDlgIndiTicket			= (gui::CDialog*)GetComponent("dlgIndiTicket");
	m_pStbTicketGuide		= (gui::CStaticBox*)GetComponent("stbTicketGuide");
	m_pStbTicketNumber		= (gui::CStaticBox*)GetComponent("stbTicketNumber");
	m_pStbDojoRecommned		= (gui::CStaticBox*)GetComponent("stbDojoRecommend");

	m_surEmblem.SetSize( dEMBLEM_TEXTURE_DEFAULT_SIZE, dEMBLEM_TEXTURE_DEFAULT_SIZE );
	m_surEmblem.SetPositionfromParent( 79, 208 );

	// Team match receipt
	m_pDlgTeamTicket		= (gui::CDialog*)GetComponent("dlgTeamTicket");
	m_pStbTicketTeamGuide	= (gui::CStaticBox*)GetComponent("stbTicketTeamGuide");
	m_pStbTicketTeamNumber	= (gui::CStaticBox*)GetComponent("stbTicketTeamNumber");
	m_pStbTicketTeamMember	= (gui::CStaticBox*)GetComponent("stbTicketTeamMember");

	m_pStbTicketTeamMember->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_SUCCESS_MEMBER" ) );


	for( RwInt32 i=0; i< dTBREQUEST_TEAM_MEMBER; ++i )
	{
		sprintf_s( acBuffer, 512, "stbTicketTeamMember%d", i );
		m_paTicketTeamMember[i] = (gui::CStaticBox*)GetComponent( acBuffer );
	}

	// Application number
	m_numTicket.Create( ND_CENTER, NVD_CENTER, 0 );
	m_numTicket.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_0" ) );
	m_numTicket.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_1" ) );
	m_numTicket.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_2" ) );
	m_numTicket.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_3" ) );
	m_numTicket.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_4" ) );
	m_numTicket.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_5" ) );
	m_numTicket.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_6" ) );
	m_numTicket.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_7" ) );
	m_numTicket.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_8" ) );
	m_numTicket.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_9" ) );	
	
	m_slotMove		= m_pThis->SigMove().Connect( this, &CTBRequestGui::OnMove );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CTBRequestGui::OnPaint );
	
	Show( false );

	LinkMsg( g_EventBudokaiUpdateStateNfy, 0 );
	LinkMsg( g_EventBudokaiUpdateMatchStateNfy, 0 );
	LinkMsg( g_EventBudokaiJoinIndividualRes, 0 );
	LinkMsg( g_EventBudokaiLeaveIndividualRes, 0 );
	LinkMsg( g_EventBudokaiJoinTeamInfoRes, 0 );
	LinkMsg( g_EventBudokaiJoinTeamRes, 0 );
	LinkMsg( g_EventBudokaiJoinTeamNfy, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamRes, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamNfy, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamMemberRes, 0 );
	LinkMsg( g_EventBudokaiLeaveTeamMemberNfy, 0 );
	LinkMsg( g_EventBudokaiJoinInfoRes, 0 );
	LinkMsg( g_EventBudokaiJoinStateNfy, 0 );

	LinkMsg( g_EventMsgBoxResult, 0 );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CTBRequestGui::Destroy()
{
	NTL_FUNCTION("CTBRequestGui::Destroy");

	if( m_surEmblem.GetTexture() )
	{
		if( !m_strEmblemTextureName.empty() )
		{
			m_surEmblem.UnsetTexture();
			GetEmblemMaker()->DestoyEmblem( m_strEmblemTextureName.c_str() );
			m_strEmblemTextureName.clear();
		}
	}

	UnLinkMsg( g_EventBudokaiUpdateStateNfy );
	UnLinkMsg( g_EventBudokaiUpdateMatchStateNfy );
	UnLinkMsg( g_EventBudokaiJoinIndividualRes );
	UnLinkMsg( g_EventBudokaiLeaveIndividualRes );
	UnLinkMsg( g_EventBudokaiJoinTeamInfoRes );
	UnLinkMsg( g_EventBudokaiJoinTeamRes );
	UnLinkMsg( g_EventBudokaiJoinTeamNfy );
	UnLinkMsg( g_EventBudokaiLeaveTeamRes );
	UnLinkMsg( g_EventBudokaiLeaveTeamNfy );
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberRes );
	UnLinkMsg( g_EventBudokaiLeaveTeamMemberNfy );
	UnLinkMsg( g_EventBudokaiJoinInfoRes );
	UnLinkMsg( g_EventBudokaiJoinStateNfy );

	UnLinkMsg( g_EventMsgBoxResult );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief SwitchDialog
*/
RwInt32 CTBRequestGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
		UpdateInterface();

	return TRUE;
}

/**
* \brief HandleEvents
*/
void CTBRequestGui::HandleEvents( RWS::CMsg& msg )
{
	// When the main state of the world's best martial arts club is changed
	//// 1. If the main state is in CLOSE or CLEAR status and the application is floating, close it.
	if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>( msg.pData );

		if( pNotify->sStateInfo.byState == BUDOKAI_STATE_CLEAR ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_CLEAR ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_CLOSE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_CLOSE )
		{
			if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
				GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
		}
	}
	// When the match state of World's First Budokai is changed
	// 1. If the match state is not the Register period and the GUI type is Application, close it.
	else if( msg.Id == g_EventBudokaiUpdateMatchStateNfy )
	{
		sNtlEventBudokaiUpdateMatchStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateMatchStateNfy*>( msg.pData );

		if( m_byGuiType == TYPE_REQUEST )
		{
			if( m_byRequestType == REQUEST_INDIVIDUAL &&
				pNotify->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
			{
				if( pNotify->sStateInfo.byState != BUDOKAI_MATCHSTATE_REGISTER )
				{
					if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
						GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
				}
			}
			else if( m_byRequestType == REQUEST_TEAM &&
				pNotify->byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
			{
				if( pNotify->sStateInfo.byState != BUDOKAI_MATCHSTATE_REGISTER )
				{
					if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
						GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
				}
			}
		}
	}
	else if( msg.Id == g_EventBudokaiJoinIndividualRes ||
		msg.Id == g_EventBudokaiJoinTeamRes ||
		msg.Id == g_EventBudokaiJoinTeamNfy )
	{
		UpdateInterface();
	}
	else if( msg.Id == g_EventBudokaiJoinTeamInfoRes )
	{
		SNtlEventBudokaiJoinTeamInfoRes* pResult = reinterpret_cast<SNtlEventBudokaiJoinTeamInfoRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			for( int i=0; i < pResult->byMemberCount; ++i )
			{
				m_paMemberClass[i]->ClearSurface();
				m_paMemberClass[i]->AddSurface( Logic_GetPCClassIconSurface( pResult->asPointInfo[i].byClass, FALSE ) );
				m_paMemberName[i]->SetText( pResult->asPointInfo[i].wszName );
				
				WCHAR awcBuffer[32];
				swprintf_s( awcBuffer, 32, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAM_REQ_TEAMLEVEL" ), pResult->asPointInfo[i].byLevel );
				m_paMemberLevel[i]->SetText( awcBuffer );
				m_paMemberRankingPoint[i]->SetText( (RwInt32)pResult->asPointInfo[i].fPoint );
			}
		}
	}
	else if( msg.Id == g_EventBudokaiLeaveIndividualRes )
	{
		SNtlEventBudokaiLeaveIndividualRes* pResult = reinterpret_cast<SNtlEventBudokaiLeaveIndividualRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
				GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
		}
	}
	// Consequences of party battle application cancellation (team leader)
	// Success: Convert the receipt into an application and initialize the information. If there are no party members, switch to the application form for a solo exhibition.
	else if( msg.Id == g_EventBudokaiLeaveTeamRes )
	{
		SNtlEventBudokaiLeaveTeamRes* pResult = reinterpret_cast<SNtlEventBudokaiLeaveTeamRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
				GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
		}
	}
	// Notification that party application has been canceled (to all team members)
	// Convert the receipt into an application form and initialize the information. (If you are a team member, open the party application form. Otherwise, switch to the individual application form.)
	else if( msg.Id == g_EventBudokaiLeaveTeamNfy )
	{
		if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
			GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
	}
	// If a pre-party member cancels the pre-party application (member)
	else if( msg.Id == g_EventBudokaiLeaveTeamMemberRes )
	{
		SNtlEventBudokaiLeaveTeamMemberRes* pResult = reinterpret_cast<SNtlEventBudokaiLeaveTeamMemberRes*>( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
				GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
		}
	}
	// If a pre-party member cancels the pre-party application (to team members)
	else if( msg.Id == g_EventBudokaiLeaveTeamMemberNfy )
	{
		SNtlEventBudokaiLeaveTeamMemberNfy* pNotify = reinterpret_cast<SNtlEventBudokaiLeaveTeamMemberNfy*>( msg.pData );

		for( RwInt32 i = 0; i< dTBREQUEST_TEAM_MEMBER; ++i )
		{
			if( wcsstr( pNotify->pwcMemberName, m_paTicketTeamMember[i]->GetText().c_str() ) == 0 )
			{
				m_paTicketTeamMember[i]->Clear();
			}
		}
	}
	// If you are eliminated in the middle
	else if( msg.Id == g_EventBudokaiJoinStateNfy )
	{
		SNtlEventBudokaiJoinStateNfy* pNotify = reinterpret_cast<SNtlEventBudokaiJoinStateNfy*>( msg.pData );

		if( pNotify->byJoinState == BUDOKAI_JOIN_STATE_DROPOUT )
		{
			if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
				GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
		}
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		if (pEvent->eResult == MBR_OK)
		{
			// TO DO
			//switch (pEvent->uiWorkId)
			//{
			//case MBW_TB_REQUEST_INDI_CANCEL:
			//	GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiLeaveIndividualReq();
			//	break;
			//case MBW_TB_REQUEST_TEAM_CANCEL:
			//	{
			//		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			//		if( pSobAvatar == NULL )
			//			return;

			//		sBUDOKAI_JOIN_INFO* pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();
			//		if( pJoinInfo == NULL )
			//			return;

			//		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
			//		if( pAvatarInfo == NULL )
			//			return;

			//		for( RwInt32 i = 0; i< NTL_MAX_MEMBER_IN_PARTY; i++ )
			//		{
			//			if( wcscmp( pJoinInfo->sTeamInfo.aMemberInfo[i].wszName, pAvatarInfo->sCharPf.awchName ) == 0 )
			//			{
			//				// Only when i is 0, the person who applied is considered the party leader, and everyone else is considered a member.
			//				if( i == 0 )
			//					GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiLeaveTeamReq();
			//				else
			//					GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiLeaveTeamMemberReq();
			//				break;
			//			}	
			//		}
			//	}
			//	break;
			//default:
			//	break;
			//}	
		}
	}
}

/**
* \brief Sets the type of application.
*/
void CTBRequestGui::SetRequestType( RwUInt8 byRequestType )
{
	m_byRequestType = byRequestType;
}

/**
* \brief Specifies the application type according to the status of participation in the World's Best Martial Arts Club.
*/
void CTBRequestGui::UpdateJoinInfo()
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sBUDOKAI_JOIN_INFO* pJoinInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pSoloMatch;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch;

	// If NULL, the information was received before avatar creation.
	// If it is not NULL, the data is set normally.
	if( pSobAvatar == NULL )
	{
		pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
		pSoloMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
		pTeamMatch = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM];
	}
	else
	{
		pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
		pSoloMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();
		pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();
	}

	if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_DROPOUT )
	{
		// Exception (If you are rejected, neither the receipt nor the application form should appear.)
	}
	else if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_PLAY )
	{
		// Receipt must be visible
		m_byGuiType = TYPE_TICKET;
		
		if( pJoinInfo->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, L"%s %s",
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" ),
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_TICKET_TITLE" ) );
			m_pStbTitle->SetText( awcBuffer );

			m_numTicket.SetNumber( (RwInt64)pJoinInfo->sIndividualInfo.wJoinId );
		}
		else if( pJoinInfo->byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, L"%s %s",
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" ),
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_TICKET_TITLE" ) );
			m_pStbTitle->SetText( awcBuffer );

			m_numTicket.SetNumber( (RwInt64)pJoinInfo->sTeamInfo.wJoinId );
		}
	}
	else if( pJoinInfo->byJoinState == INVALID_BUDOKAI_JOIN_STATE )
	{
		m_byGuiType = TYPE_REQUEST;

		if( m_byRequestType == REQUEST_INDIVIDUAL )
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, L"%s %s",
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" ),
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQUEST_TITLE" ) );
			m_pStbTitle->SetText( awcBuffer );
		}
		else if( m_byRequestType == REQUEST_TEAM )
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, L"%s %s",
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" ),
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQUEST_TITLE" ) );
			m_pStbTitle->SetText( awcBuffer );
		}
	}
}

/**
* \brief Sets the interface according to the current state. (Read the World's Best Martial Arts information and set it up.)
*/
void CTBRequestGui::UpdateInterface()
{
	m_pThis->SetPosition( ( GetDboGlobal()->GetScreenWidth()-m_pThis->GetWidth() )/2,
		( GetDboGlobal()->GetScreenHeight()-m_pThis->GetHeight() )/2 );

	// Dialog
	m_pDlgIndividual->Show( false );
	m_pDlgTeam->Show( false );
	m_pDlgIndiTicket->Show( false );
	m_pDlgTeamTicket->Show( false );

	// Check your current participation status.
	UpdateJoinInfo();

	if( m_byGuiType == TYPE_REQUEST )
	{
		// Dialogs
		if( m_byRequestType == REQUEST_INDIVIDUAL )
		{
			m_pDlgIndividual->Show( true );		

			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pSobAvatar == NULL )
				return;

			// name, level
			m_pStbIndiNameDisplay->SetText( Logic_GetAvatarName() );
			m_pStbIndiLevelDisplay->SetText( Logic_GetLevel( pSobAvatar ) );

			// class
			RwUInt8 byClass = Logic_GetPlayerClass(pSobAvatar);	// class
			m_pStbIndiClassDisplay->SetText( Logic_GetClassName( byClass ) );

			sRANKBATTLE_SCORE_INFO* pData = pSobAvatar->GetRankBattle()->GetRankBattleScore();
			
			// Records (how many wins and how many losses)
			WCHAR awcBuffer[256];
			swprintf( awcBuffer, 256, 
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_REQ_RECORD_DATA" ),
				pData->dwWin+pData->dwDraw+pData->dwLose,
				pData->dwWin,
				pData->dwLose );
			m_pStbIndiRecordDisplay->SetText( awcBuffer );

			// ranking points
			m_pStbIndiRankingPointDisplay->SetText( (RwInt32)pData->fPoint );

			// guild
			if( pSobAvatar->GetGuild()->IsHaveGroup() )
			{
				m_pStbIndiGuildDisplay->SetText( pSobAvatar->GetGuild()->GetGuildName() );
			}
			else
			{
				m_pStbIndiGuildDisplay->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_NOT_IN_GUILD" ) );
			}
		}
		else if( m_byRequestType == REQUEST_TEAM )
		{
			m_pDlgTeam->Show( true );

			
			// Set the party name as default in the input box.
			m_pIpbTeamName->Clear();
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pAvatar )
			{
				CNtlParty* pParty = pAvatar->GetParty();
				if( pParty )
				{
					m_pIpbTeamName->SetText( pParty->GetPartyName() );
				}
			}

			// Clear the list of applied members and request team information from the server.
			ClearRequestMemberInfo();
			GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiJoinTeamInfoReq();		
		}

		// Button
		m_pBtnAccept->Show( true );
		/*m_pBtnCancel->Show( false );*/

		m_pBtnAccept->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQ_BUTTON_ACCEPT" ) );
	}
	else if( m_byGuiType == TYPE_TICKET )
	{
		if( m_byRequestType == REQUEST_INDIVIDUAL )
		{
			m_numTicket.SetPosition( 215, 192 ); // Individual exhibition application location
			m_pDlgIndiTicket->Show( true );

			
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_SUCCESS_APPLICATION" ),
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" ) );
			m_pStbTicketGuide->SetText( awcBuffer );
			
			
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_SUCCESS_GUIDE" ), Logic_GetAvatarName() );
			m_pStbTicketNumber->SetText( awcBuffer );

			// Buttons
			m_pBtnAccept->Show( false );

			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

			sBUDOKAI_JOIN_INFO* pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();
			sBUDOKAI_UPDATE_MATCH_STATE_INFO* pSoloMatch = pSobAvatar->GetTenkaichiBudokai()->GetIndividualMatchStateInfo();

			if( pJoinInfo->sIndividualInfo.bDojoRecommender )
			{
				CNtlSobCharProxy* pSobCharProxy = (CNtlSobCharProxy*)pSobAvatar->GetSobProxy();
				const sEmblemFactor* pEmblemFactor = pSobCharProxy->GetDecorationProxy()->GetEmblemFactor();
			
				RwTexture* pTexture = GetEmblemMaker()->CreateEmblem( pEmblemFactor );
				if( pTexture )
				{
					m_strEmblemTextureName = RwTextureGetNameMacro( pTexture );
					m_texEmblem.Load( pTexture );
					m_surEmblem.SetTexture( &m_texEmblem );
				}
				else
				{
					if( m_surEmblem.GetTexture() )
					{
						m_surEmblem.UnsetTexture();
						GetEmblemMaker()->DestoyEmblem( m_strEmblemTextureName.c_str() );
						m_strEmblemTextureName.clear();
					}
				}
								
				m_pStbDojoRecommned->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_INDI_DOJO_RECOMMNED" ) );
			}
			else
			{
				if( m_surEmblem.GetTexture() )
				{
					m_surEmblem.UnsetTexture();
					GetEmblemMaker()->DestoyEmblem( m_strEmblemTextureName.c_str() );
					m_strEmblemTextureName.clear();
				}

				m_pStbDojoRecommned->Clear();
			}

			// Todo
			if( pSoloMatch->byState == BUDOKAI_MATCHSTATE_REGISTER )
				m_pBtnAccept->Show( true );
			else
				m_pBtnAccept->Show( false );
		}
		else if( m_byRequestType == REQUEST_TEAM )
		{
			m_numTicket.SetPosition( 215, 140 ); // Team match application location
			m_pDlgTeamTicket->Show( true );

			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pSobAvatar == NULL )
				return;

			sBUDOKAI_JOIN_INFO* pInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();

			// Bad data if not team data
			if( pInfo->byMatchType != BUDOKAI_MATCH_TYPE_TEAM )
				return;

			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_SUCCESS_APPLICATION" ),
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" ) );
			m_pStbTicketTeamGuide->SetText( awcBuffer );
			
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_BUDOKAI_SUCCESS_GUIDE" ), pInfo->sTeamInfo.wszTeamName );
			m_pStbTicketTeamNumber->SetText( awcBuffer );

			// Initialize member information and add new information.
			ClearTicketMemberInfo();
			for( RwInt32 i = 0; i< dTBREQUEST_TEAM_MEMBER; ++i )
			{
				// If you have a Class and a name of aMemberInfo
				if( ( pInfo->sTeamInfo.aMemberInfo[i].byClass != INVALID_BYTE )
					&& ( wcslen( pInfo->sTeamInfo.aMemberInfo[i].wszName ) > 0 ) )
				{
					m_paTicketTeamMember[i]->SetText( pInfo->sTeamInfo.aMemberInfo[i].wszName );
				}
			}

			// Buttons
			/*m_pBtnAccept->Show( false );*/

			sBUDOKAI_UPDATE_MATCH_STATE_INFO* pTeamMatch = pSobAvatar->GetTenkaichiBudokai()->GetTeamMatchStateInfo();

			if( pTeamMatch->byState == BUDOKAI_MATCHSTATE_REGISTER )
				m_pBtnAccept->Show( true );
			else
				m_pBtnAccept->Show( false );
		}

		m_pBtnAccept->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQ_BUTTON_CANCEL" ) );
	}
}

void CTBRequestGui::ClearRequestMemberInfo()
{
	for( RwInt32 i = 0; i < dTBREQUEST_TEAM_MEMBER; ++i )
	{
		m_paMemberClass[i]->ClearSurface();
		m_paMemberName[i]->Clear();
		m_paMemberLevel[i]->Clear();
		m_paMemberRankingPoint[i]->Clear();
	}
}

void CTBRequestGui::ClearTicketMemberInfo()
{
	for( RwInt32 i = 0; i < dTBREQUEST_TEAM_MEMBER; ++i )
	{
		m_paTicketTeamMember[i]->Clear();
	}
}

void CTBRequestGui::OnClickedBtnClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );
}

void CTBRequestGui::OnClickedBtnAccept( gui::CComponent* pComponent )
{
	if( m_byGuiType == TYPE_REQUEST )
	{
		if( m_byRequestType == REQUEST_INDIVIDUAL )
			GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiJoinIndividualReq();
		else if( m_byRequestType == REQUEST_TEAM )
		{
			// You cannot apply if you are not the party leader.
			if( !Logic_I_am_PartyLeader() )
			{
				GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_NEWS_CAN_ONLY_LEADER" );
				return;
			}

			if( wcslen( m_pIpbTeamName->GetText() ) < 4 )
			{
				// It must be at least 4 characters.
			}
			else
			{
				WCHAR awcBuffer[256];
				swprintf_s( awcBuffer, 256, L"%s", m_pIpbTeamName->GetText() );
				GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiJoinTeamReq( awcBuffer );
			}
		}	
	}
	else if( m_byGuiType == TYPE_TICKET )
	{		
		if( m_byRequestType == REQUEST_INDIVIDUAL )
		{
			GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_INDI_REQ_CANCEL" );
		}
		else if( m_byRequestType == REQUEST_TEAM )
		{
			GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_TEAM_REQ_CANCEL" );
		}
	}
}

//void CTBRequestGui::OnClickedBtnCancel( gui::CComponent* pComponent )
//{
//	if( GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_REQUEST ) )
//		GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_REQUEST );	
//}

void CTBRequestGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_numTicket.SetParentPos( rtScreen.left, rtScreen.top );
	m_surEmblem.SetPositionbyParent( rtScreen.left, rtScreen.top ); 
}

void CTBRequestGui::OnPaint()
{	
	if( m_byGuiType == TYPE_TICKET )
	{
		m_numTicket.Render();

		if( m_byRequestType == REQUEST_INDIVIDUAL )
		{
			if( m_surEmblem.GetTexture() )
				m_surEmblem.Render();
		}
	}
}