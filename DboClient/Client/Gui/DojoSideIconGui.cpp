#include "precomp_dboclient.h"
#include "DojoSideIconGui.h"

// Shared
#include "NtlDojo.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// Client
#include "AlarmManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"


#define dSTRING_HEIGHT_GAP			(10)
#define dVIEWBOTTOM_GAP				(20)

#define dSHOW_TIME_LEFT_0			(20.f)
#define dSHOW_TIME_LEFT_1			(15.f)
#define dSHOW_TIME_LEFT_2			(13.f)
#define dSHOW_TIME_LEFT_3			(11.f)
#define dSHOW_TIME_LEFT_4			(10.f)
#define dSHOW_TIME_LEFT_5			(3.f)


//////////////////////////////////////////////////////////////////////////
// CDojoSideIconGui
//////////////////////////////////////////////////////////////////////////

CDojoSideIconGui::CDojoSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{
}

CDojoSideIconGui::~CDojoSideIconGui()
{

}

RwBool CDojoSideIconGui::Create()
{
	NTL_FUNCTION( "CDojoSideIconGui::Create" );

	if(!CNtlPLGui::Create("gui\\Dogi.rsr", "gui\\DojoSideIcon.srf", "gui\\DojoSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CDojoSideIconGui::OnIconButtonClicked);

	Show(false);	

	NTL_RETURN(TRUE);
}

VOID CDojoSideIconGui::Destroy()
{
	NTL_FUNCTION("CDojoSideIconGui::Destroy");	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CDojoSideIconGui::OnSideViewClosed()
{
}

void CDojoSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_DOJO, NULL);
}

//////////////////////////////////////////////////////////////////////////
// CDojoSideViewGui
//////////////////////////////////////////////////////////////////////////

CDojoSideViewGui::CDojoSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
,m_iViewHeight(0)
,m_fElapsed(0)
{
	for( RwUInt8 i = 0 ; i < dNUM_TIMER_MESSAGE ; ++i )
		m_bShowTimerMessage[i] = FALSE;
}

CDojoSideViewGui::~CDojoSideViewGui()
{

}

RwBool CDojoSideViewGui::Create()
{
	NTL_FUNCTION( "CDojoSideViewGui::Create" );

	if(!CNtlPLGui::Create("gui\\Dogi.rsr", "gui\\DojoSideIcon.srf", "gui\\DojoSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CDojoSideViewGui::OnClicked_ExitButton);

	m_pMessage = (gui::CStaticBox*)GetComponent("stbMessage");

	// background
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundTop"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundBottom"));

	GetNtlGuiManager()->AddUpdateFunc( this );

	// Signals
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CDojoSideViewGui::OnPaint );

	LinkMsg(g_EventDojoStateNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDojoSideViewGui::Destroy()
{
	NTL_FUNCTION("CDojoSideViewGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	UnLinkMsg(g_EventDojoStateNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDojoSideViewGui::Update(RwReal fElapsed)
{
	return;
/*
	m_fElapsed += fElapsed;
	if( m_fElapsed < 1.f )
		return;

	m_fElapsed = 0.f;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();
	CNtlGuild::sDOJO* pGuildDojo = pGuild->GetDojoInfo();

	RwReal fMinute = (RwReal)pGuildDojo->ui64Scramble_utilStart / 60.f;
	
	if( m_bShowTimerMessage[0] && fMinute < dSHOW_TIME_LEFT_0 )
	{
		if( DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE == pGuildDojo->eDojoTeamState )
		{
			m_pMessage->SetText("The dojo battle will begin in a moment, so please prepare for battle");
		}
		else if( DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE == pGuildDojo->eDojoTeamState )
		{
			GetAlarmManager()->AlarmMessage(L"Due to preparations for the competition, members of the dojo will be forced to move to the dojo entrance after a while, so please prepare.");
			m_pMessage->SetText("Please prepare as you will be forcibly moved to the entrance of the dojo in a moment to prepare for the battle.");
		}

		m_bShowTimerMessage[0] = false;

		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}
	else if( m_bShowTimerMessage[1] && fMinute < dSHOW_TIME_LEFT_1 )
	{
		if( DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE == pGuildDojo->eDojoTeamState )
		{
			m_pMessage->SetText("The dojo battle will begin in a moment, so please prepare for battle");
		}
		else if( DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE == pGuildDojo->eDojoTeamState )
		{
			GetAlarmManager()->AlarmMessage(L"As a contest is imminent, all members in the dojo will be moved to the dojo entrance in 5 minutes");
			m_pMessage->SetText("Please prepare as you will be forcibly moved to the entrance of the dojo in a moment to prepare for the battle.");
		}

		m_bShowTimerMessage[1] = false;
		
		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}
	else if( m_bShowTimerMessage[2] && fMinute < dSHOW_TIME_LEFT_2 )
	{
		if( DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE == pGuildDojo->eDojoTeamState )
		{
			m_pMessage->SetText("The dojo battle will begin in a moment, so please prepare for battle");
		}
		else if( DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE == pGuildDojo->eDojoTeamState )
		{
			GetAlarmManager()->AlarmMessage(L"As a contest is imminent, all members in the dojo will be moved to the dojo entrance in 3 minutes");
			m_pMessage->SetText("Please prepare as you will be forcibly moved to the entrance of the dojo in a moment to prepare for the battle.");
		}

		m_bShowTimerMessage[2] = false;
		
		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}
	else if( m_bShowTimerMessage[3] && fMinute < dSHOW_TIME_LEFT_3 )
	{
		if( DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE == pGuildDojo->eDojoTeamState )
		{
			m_pMessage->SetText("The dojo battle will begin in a moment, so please prepare for battle");
		}
		else if( DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE == pGuildDojo->eDojoTeamState )
		{
			GetAlarmManager()->AlarmMessage(L"As a contest is imminent, all members in the dojo will be moved to the dojo entrance in 1 minute");
			m_pMessage->SetText("Please prepare as you will be forcibly moved to the entrance of the dojo in a moment to prepare for the battle.");
		}

		m_bShowTimerMessage[3] = false;
		
		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}
	else if( m_bShowTimerMessage[4] && fMinute < dSHOW_TIME_LEFT_4 )
	{
		if( DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE == pGuildDojo->eDojoTeamState )
		{
			m_pMessage->SetText("The dojo battle will begin in a moment, so please prepare for battle");
		}
		else if( DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE == pGuildDojo->eDojoTeamState )
		{
			GetAlarmManager()->AlarmMessage(L"Due to preparations for the contest, members of the dojo have been forced to move to the dojo entrance");
			m_pMessage->SetText("Please prepare as you will be forcibly moved to the entrance of the dojo in a moment to prepare for the battle.");
		}

		m_bShowTimerMessage[4] = false;
		
		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}
	else if( m_bShowTimerMessage[5] && fMinute < dSHOW_TIME_LEFT_5 )
	{
		GetAlarmManager()->AlarmMessage(L"Preparation for the dojo battle is complete and you can enter the dojo");
		m_pMessage->SetText("The competition for the dojo will begin now, so please enter the dojo.");

		m_bShowTimerMessage[5] = false;
		
		CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
		if( pSideIcon )
		{
			CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
		}
		else
		{
			DBO_FAIL("Not exist dojo side icon");
		}
	}*/
}

VOID CDojoSideViewGui::SetMessage(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64NextStepTime)
{
	CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
	if( !pSideIcon )
		return;


	switch(byDojoState)
	{
	case eDBO_DOJO_STATUS_NORMAL:
		{
			// Do not display before 12 hours
			RwReal	fLeftTime	= (RwReal)(ui64NextStepTime - GetNtlSLGlobal()->GetServerTime());
			RwReal	fLimitTime	= 60.f * 60.f * 12.f;

			if( fLeftTime > fLimitTime )
			{
				pSideIcon->Show(false);
				return;
			}


			// Only the day when there is a contest for the painting must be shown.
			sDetailTime detailTime;
			DBOTIME tDBOTIME = ui64NextStepTime;

			Logic_timeToDetailTime(tDBOTIME, detailTime);

			if( detailTime.byMinute > 0 )
			{
				const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_NORMAL");
				if( pwcText )
				{
					m_pMessage->Format(pwcText, detailTime.byHour, detailTime.byMinute);
					GetAlarmManager()->AlarmMessage((WCHAR*)m_pMessage->GetText().c_str());
				}
			}
			else
			{
				const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_NORMAL2");
				if( pwcText )
				{
					m_pMessage->Format(pwcText, detailTime.byHour);
					GetAlarmManager()->AlarmMessage( (WCHAR*)m_pMessage->GetText().c_str() );
				}
			}		

			break;
		}
	case eDBO_DOJO_STATUS_RECEIVE:
	case eDBO_DOJO_STATUS_REJECT_END:
	case eDBO_DOJO_STATUS_STANDBY:
	case eDBO_DOJO_STATUS_INITIAL:
	case eDBO_DOJO_STATUS_READY:
	case eDBO_DOJO_STATUS_START:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				pSideIcon->Show(false);
				return;
			}

			CNtlDojo*		pDojo			= pAvatar->GetDojo();
			sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
			if( !pSCRAMBLE_INFO )
			{
				pSideIcon->Show(false);
				return;
			}

			if( pSCRAMBLE_INFO->uiScrambleDojoTableIndex != uiDojoTableIndex )
			{
				pSideIcon->Show(false);
				return;
			}

			WCHAR*	pwcText = L"";

			switch (byDojoState)
			{
				case eDBO_DOJO_STATUS_RECEIVE: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_RECEIVE"); break;
				case eDBO_DOJO_STATUS_REJECT_END: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_REJECT_END"); break;
				case eDBO_DOJO_STATUS_STANDBY: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_STANDBY"); break;
				case eDBO_DOJO_STATUS_INITIAL: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_INITIAL"); break;
				case eDBO_DOJO_STATUS_READY: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_READY"); break;
				case eDBO_DOJO_STATUS_START: pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_START"); break;
			}

			m_pMessage->SetText(pwcText);
			GetAlarmManager()->AlarmMessage(pwcText);

			break;
		}
	case eDBO_DOJO_STATUS_END:
		{
			WCHAR*	pwcText			= pwcText = (WCHAR*)GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_END");

			m_pMessage->SetText(pwcText);
			GetAlarmManager()->AlarmMessage(pwcText);
			break;
		}
	default:
		{
			DBO_FAIL("Invalid dojo state : " << byDojoState);
			{
				pSideIcon->Show(false);
				return;
			}
		}
	}


	CRectangle rect = m_pMessage->GetPosition();
	m_iViewHeight = rect.bottom + dSTRING_HEIGHT_GAP + dVIEWBOTTOM_GAP;

	pSideIcon->Show(true);
	CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
}

VOID CDojoSideViewGui::OnPressESC()
{

}

void CDojoSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
}

VOID CDojoSideViewGui::OnSideViewClose()
{
	Show(false);
}

VOID CDojoSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	CRectangle rtScreen;

	rtScreen.SetRectWH(rectSideIcon.left - m_BackPanel.GetWidth() + rectSideIcon.GetWidth(),
					   rectSideIcon.top - m_iViewHeight,
					   m_BackPanel.GetWidth(),
					   m_iViewHeight);	

	m_pThis->SetPosition(rtScreen);
	m_BackPanel.SetRect(rtScreen);
}

VOID CDojoSideViewGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{
	Show(false);
}

VOID CDojoSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

VOID CDojoSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDojoSideViewGui::HandleEvents");

	if( msg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );
		SetMessage(pEvent->uiDojoTableIndex, pEvent->byDojoState, pEvent->ui64time);
	}

	NTL_RETURNVOID();
}