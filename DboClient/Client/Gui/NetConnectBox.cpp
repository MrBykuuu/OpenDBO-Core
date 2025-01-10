#include "precomp_dboclient.h"
//#include "NetConnectBox.h"
//
//// core
//#include "NtlDebug.h"
//
//// sound
//#include "GUISoundDefine.h"
//
//// presentation
//#include "NtlPLGuiManager.h"
//#include "NtlPLEvent.h"
//
//// dbo
//#include "DboEvent.h"
//#include "DboEventGenerator.h"
//#include "DialogDefine.h"
//#include "DboGlobal.h"
//#include "CharStageState.h"
//#include "DisplayStringDef.h"
//#include "DisplayStringManager.h"
//#include "DialogPriority.h"
//
//namespace
//{
//	#define dMESSAGE_Y					35
//	#define dMESSAGE_MINIMUN_HEIGHT		46
//}
//
//
//CNetConnectBox::CNetConnectBox(const RwChar* pName)
//:CNtlPLGui(pName)
//,m_byMsgBoxType(0)
//,m_uiMsgProcessType(0)
//,m_fRemainTime(0.f)
//,m_bRemainTime(false)
//{
//
//}
//
//CNetConnectBox::~CNetConnectBox()
//{
//}
//
//RwBool CNetConnectBox::Create()
//{
//	NTL_FUNCTION( "CNetConnectBox::Create" );
//
//	if(!CNtlPLGui::Create("gui\\NetConnect.rsr", "gui\\NetConnect.srf", "gui\\NetConnect.frm"))
//		NTL_RETURN(FALSE);
//
//	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());
//
//	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
//	m_pThis->SetPriority(dDIALOGPRIORITY_MSGBOX);
//
//	CRectangle rect;
//
//	//OK button
//	m_pOkBtn = (gui::CButton*)GetComponent("btnOK");
//	m_pOkBtn->SetText(GetDisplayStringManager()->GetString(DST_MSG_BTN_OK));
//	m_slotOk = m_pOkBtn->SigClicked().Connect( this, &CNetConnectBox::OnClickedOkButton );
//
//	//Cancel button
//	m_pCancelBtn = (gui::CButton*)GetComponent("btnCancel");
//	m_pCancelBtn->SetText(GetDisplayStringManager()->GetString(DST_MSG_BTN_CANCEL));
//	m_slotCencle = m_pCancelBtn->SigClicked().Connect( this, &CNetConnectBox::OnClickedCancelButton );	
//
//	//background
//	m_Backboard.SetType(CWindowby3::WT_HORIZONTAL);
//	m_Backboard.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfBackgroundUp" ));
//	m_Backboard.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfBackgroundCenter" ));
//	m_Backboard.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfBackgroundBottom" ));
//
//	//text background frame
//	m_Panel.SetType(CWindowby3::WT_HORIZONTAL);
//	m_Panel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfPanelUp" ));
//	m_Panel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfPanelCenter" ));
//	m_Panel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetConnect.srf", "srfPanelBottom" ));
//
//	//message content
//	rect.SetRectWH(20, dMESSAGE_Y, 283, 200);
//	m_pMessage = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER |  COMP_TEXT_VERTICAL_CENTER, TRUE);
//	m_pMessage->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
//	m_pMessage->Clear();
//	m_pMessage->Enable(false);
//
//
//	GetNtlGuiManager()->AddUpdateFunc(this);
//
//	// sig
//	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNetConnectBox::OnPaint );
//
//	Show(false);
//	
//	LinkMsg(g_EventConnectBoxShow);
//	LinkMsg(g_EventResize);
//
//	NTL_RETURN(TRUE);
//}
//
//VOID CNetConnectBox::Destroy()
//{
//	NTL_FUNCTION("CNetConnectBox::Destroy");
//
//	GetNtlGuiManager()->RemoveUpdateFunc(this);
//
//	UnLinkMsg(g_EventConnectBoxShow);
//	UnLinkMsg(g_EventResize);
//	
//	CNtlPLGui::DestroyComponents();
//	CNtlPLGui::Destroy(); 
//
//	NTL_RETURNVOID();
//}
//
//VOID CNetConnectBox::Update(RwReal fElapsed)
//{
//	if( IsShow() == false )
//		return;
//
//	if( m_bRemainTime == false )
//		return;
//
//    RaiseTop();
//	m_fRemainTime -= fElapsed;
//
//	if( m_fRemainTime <= 0.f )
//		ProcessCancel();
//	else
//		SetText();
//}
//
//VOID CNetConnectBox::CalcBoxSize()
//{
//	CRectangle rect;
//	RwInt32 iMessageAreaHeight;
//	RwInt32 iMessageHeight = m_pMessage->GetHeight();	
//	RwInt32 iScreenWidth = GetDboGlobal()->GetScreenWidth();
//	RwInt32 iDialogWidth = m_Backboard.GetWidth();
//	RwInt32 iDialogHeight = 0;
//	
//	//Specify the minimum size of the message box
//	if( iMessageHeight < dMESSAGE_MINIMUN_HEIGHT )
//		iMessageAreaHeight = dMESSAGE_MINIMUN_HEIGHT;
//	else
//		iMessageAreaHeight = iMessageHeight;
//
//
//	//OK, Cancle button visibility and location
//	//switch(m_byMsgBoxType)
//	//{
//	//case MBT_OK:
//	//	{
//	//		iDialogHeight = iMessageAreaHeight + 86;
//	//		m_pThis->SetSize(iDialogWidth, iDialogHeight);
//
//	//		m_pOkBtn->SetPosition((iDialogWidth - m_pOkBtn->GetWidth())/2, iMessageAreaHeight + 51);
//
//	//		m_pOkBtn->Show(true);
//	//		m_pCancelBtn->Show(false);
//
//	//		break;
//	//	}
//	//case MBT_OKCANCEL:
//	//	{
//	//		iDialogHeight = iMessageAreaHeight + 86;
//	//		m_pThis->SetSize(iDialogWidth, iDialogHeight);
//
//	//		m_pOkBtn->SetPosition(iDialogWidth/2 - m_pOkBtn->GetWidth() - 10, iMessageAreaHeight + 51);
//	//		m_pCancelBtn->SetPosition(iDialogWidth/2 + 10, iMessageAreaHeight + 51);
//
//	//		m_pOkBtn->Show(true);			
//	//		m_pCancelBtn->Show(true);
//
//	//		break;
//	//	}
//	//case MBT_CANCEL:
//	//	{
//	//		iDialogHeight = iMessageAreaHeight + 86;
//	//		m_pThis->SetSize(iDialogWidth, iDialogHeight);
//
//	//		m_pCancelBtn->SetPosition((iDialogWidth - m_pCancelBtn->GetWidth())/2, iMessageAreaHeight + 51);
//
//	//		m_pOkBtn->Show(false);
//	//		m_pCancelBtn->Show(true);
//
//	//		break;
//	//	}
//	//}
//
//	//Located in the center of the message box
//	m_pThis->SetPosition((GetDboGlobal()->GetScreenWidth() - m_Backboard.GetWidth())/2,
//						 (GetDboGlobal()->GetScreenHeight() - iDialogHeight)/2);
//
//	if( iMessageHeight < dMESSAGE_MINIMUN_HEIGHT )
//	{
//		//Position correction according to minimum size designation of message box
//		RwInt32 iAdjust = (dMESSAGE_MINIMUN_HEIGHT - iMessageHeight) / 2;
//		m_pMessage->SetPosition((m_pThis->GetWidth() - m_pMessage->GetWidth())/2, dMESSAGE_Y + iAdjust);
//	}
//	else
//		m_pMessage->SetPosition((m_pThis->GetWidth() - m_pMessage->GetWidth())/2, dMESSAGE_Y);
//
//	rect = m_pThis->GetPosition();
//
//	m_Backboard.SetRect(rect);
//	m_Panel.SetRectWH(rect.left + 8, rect.top + 25, m_Panel.GetWidth(), iMessageAreaHeight + 20);
//}
//
//VOID CNetConnectBox::SetText()
//{
//	if( m_bRemainTime )
//	{
//		std::wstring wstr;
//		WCHAR awcBuffer[GUI_TEXT_BUFFER_SIZE] = L"";
//
//		wstr = m_wstrString;
//		wstr += L"\\n\\n";
//		swprintf_s(awcBuffer, GUI_TEXT_BUFFER_SIZE, L"Remain time : %d", (RwInt32)m_fRemainTime);
//		wstr += awcBuffer;
//
//		m_pMessage->SetText(wstr.c_str());
//	}	
//	else
//	{
//		m_pMessage->SetText( m_wstrString.c_str() );
//	}
//}
//
//VOID CNetConnectBox::OnClickedOkButton(gui::CComponent* pComponent)
//{
//	switch(m_uiMsgProcessType)
//	{
//	case MBW_ASK_TUTORIAL:
//		{
//			GetCharStageState()->ChangeState(CHAR_STATE_READY_ENTER_TUTORIAL);
//			break;
//		}
//	case MBW_DEL_CHARACTER:
//		{
//			//delete character
//			RwUInt8 byServer = GetDboGlobal()->GetSelectedServerHandle();
//			LobbyInfo* pLobbyInfo = GetDboGlobal()->GetLobbyInfo(byServer);
//			SERIAL_HANDLE hCharacter = pLobbyInfo->character[GetDboGlobal()->GetSelectedCharIndex()].Summary.charId;
//
//			GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelReq(hCharacter);
//
//			GetCharStageState()->ChangeState(CHAR_STATE_SELECT_DEL_CHAR);
//			break;
//		}
//	case MBW_RECOVERY_CHARACTER:
//		{
//			//character recovery
//			RwUInt8 byServer = GetDboGlobal()->GetSelectedServerHandle();
//			LobbyInfo* pLobbyInfo = GetDboGlobal()->GetLobbyInfo(byServer);
//			SERIAL_HANDLE hCharacter = pLobbyInfo->character[GetDboGlobal()->GetSelectedCharIndex()].Summary.charId;
//			SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
//
//			GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelCancelReq(pConnectData->uiAccountId, hCharacter, pConnectData->byLastServerFarmID);
//
//			GetCharStageState()->ChangeState(CHAR_STATE_SELECT_RECOVERY_CHAR);
//			break;
//		}
//	case MBW_NET_CONNECT_FAIL:
//	case MBW_LOGIN_CONNECT_FAIL:
//	case MBW_AUTHKEY_FAIL:
//	case MBW_CHARACTRE_CONNECT_FAIL:
//	case MBW_POST_QUIT_MESSAGE:
//	case MBW_NET_DISCONNECT:
//		{
//			PostQuitMessage(0);
//			break;
//		}
//	}
//
//	SwitchDialog(false);
//}
//
//VOID CNetConnectBox::OnClickedCancelButton(gui::CComponent* pComponent)
//{
//	ProcessCancel();
//}
//
//VOID CNetConnectBox::ProcessCancel()
//{
//	switch(m_uiMsgProcessType)
//	{
//	case MBW_ASK_TUTORIAL:	
//		{
//			//Enter the game immediately
//			GetCharStageState()->ReservateState(CHAR_STATE_SUCCESS_ENTER_GAME);
//			GetCharStageState()->ChangeState(CHAR_STATE_SELECT_EXIT);
//
//			GetDboGlobal()->SetTutorialCancelEnter( TRUE );
//
//			break;
//		}
//	case MBW_WAIT_FOR_TUTORIAL:
//		{
//			GetDboGlobal()->GetGamePacketGenerator()->SendTutorialWaitCancelReq();
//			GetCharStageState()->ChangeState(CHAR_STATE_WAIT_CANCEL);
//			break;
//		}
//
//	}
//
//	SwitchDialog(false);
//}
//
//VOID CNetConnectBox::SwitchDialog(bool bSwitch)
//{
//	m_pThis->Popup(bSwitch);
//	Show(bSwitch);
//
//	if( !bSwitch )
//		m_bRemainTime = false;
//}
//
//VOID CNetConnectBox::OnPaint()
//{
//	m_Backboard.Render();
//	m_Panel.Render();
//}
//
//VOID CNetConnectBox::HandleEvents( RWS::CMsg &msg )
//{
//	NTL_FUNCTION("CNetConnectBox::HandleEvents");
//
//	//if(msg.Id == g_EventConnectBoxShow)
//	//{
//	//	SDboEventConnectBoxShow* pEvent = reinterpret_cast<SDboEventConnectBoxShow*>(msg.pData);
//
//	//	if( m_byMsgBoxType != MBT_NONE )
//	//	{
//	//		SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
//	//		if( pConnectData->sGameCon.bBanishment )
//	//			NTL_RETURNVOID();
//	//	}
//
//	//	m_wstrString		= pEvent->wstrString;
//	//	m_byMsgBoxType		= pEvent->byMsgBoxType;
//	//	m_uiMsgProcessType	= pEvent->uiMsgProcessType;
//	//	m_fRemainTime		= pEvent->fRemainTime;
//	//	if( pEvent->pExData )
//	//		memcpy_s( &m_ExData, sizeof( sMsgBoxData ), pEvent->pExData, sizeof( sMsgBoxData ) );
//	//	
//	//	if( m_fRemainTime > 0.f )
//	//		m_bRemainTime = true;
//	//	else
//	//		m_bRemainTime = false;
//
//	//	if( m_byMsgBoxType == MBT_NONE )
//	//	{
//	//		SwitchDialog(false);
//	//	}
//	//	else
//	//	{
//	//		SetText();
//	//		CalcBoxSize();
//	//		SwitchDialog(true);
//	//	}
//	//}
//	//else if(msg.Id == g_EventResize)
//	//{
//	//	CalcBoxSize();
//	//}
//
//	NTL_RETURNVOID();
//}
