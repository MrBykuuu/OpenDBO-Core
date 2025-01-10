#include "precomp_dboclient.h"
#include "SkillHtbRpUse.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLPacketGenerator.h"
#include "InputActionMap.h"

// Presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "DialogManager.h"
#include "DboGlobal.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillHtbRpUseIcon
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSkillHtbRpUseIcon::CSkillHtbRpUseIcon()
: m_pFlaRpBall(NULL)
, m_pStbRpBall(NULL)
, m_byIndex(0xFF)
, m_bShow(FALSE)
, m_bEnable(FALSE)
{

}

CSkillHtbRpUseIcon::~CSkillHtbRpUseIcon()
{

}

VOID CSkillHtbRpUseIcon::Create( CSkillHtbRpUse* pGui, const RwChar* pFlashName, RwUInt8 byIndex )
{
	CRectangle rect;
	rect.SetRectWH( 0, 0, dSKILL_HTB_RP_ICON_WIDTH, dSKILL_HTB_RP_ICON_HEIGHT );

	m_pGui = pGui;

	m_pFlaRpBall = NTL_NEW gui::CFlash( rect, pGui->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(),
		pFlashName );

	rect.SetRectWH( 44, 44, 15, 20 );
	m_pStbRpBall = NTL_NEW gui::CStaticBox( rect, m_pFlaRpBall, GetNtlGuiManager()->GetSurfaceManager() );	
	m_pStbRpBall->CreateFontStd( DEFAULT_FONT, 120, DEFAULT_FONT_ATTR);
	m_pStbRpBall->SetTextStyle( COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbRpBall->SetEffectMode(TE_SHADOW);
	m_pStbRpBall->SetEffectColor(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pStbRpBall->Enable( false );

	m_byIndex = byIndex;

	m_slotFSCallback = m_pFlaRpBall->SigFSCallBack().Connect( this, &CSkillHtbRpUseIcon::OnFSCallBack );
	m_slotMovieEnd	= m_pFlaRpBall->SigMovieEnd().Connect( this, &CSkillHtbRpUseIcon::OnMovieEnd );

	m_pFlaRpBall->SetPosition( byIndex * dSKILL_HTB_RP_WIDTH_OFFSET, 0 );
}

VOID CSkillHtbRpUseIcon::Destroy( VOID )
{
	NTL_DELETE( m_pStbRpBall );
	NTL_DELETE( m_pFlaRpBall );
}

VOID CSkillHtbRpUseIcon::Update( RwReal fElapsed )
{
	m_pFlaRpBall->Update( fElapsed );
}

VOID CSkillHtbRpUseIcon::Show( RwBool bShow )
{
	if( bShow )
	{
		Select( FALSE );
	}
	else
	{
		m_pStbRpBall->Clear();
	}
	
	m_pFlaRpBall->Show( B2b(bShow) );
	m_pStbRpBall->Show( B2b(bShow) );
}

RwBool CSkillHtbRpUseIcon::IsShow()
{
	return m_pFlaRpBall->IsVisible();
}

VOID CSkillHtbRpUseIcon::SetPosition( RwInt32 nPosX, RwInt32 nPosY )
{
	m_pFlaRpBall->SetPosition( nPosX, nPosY );
}

VOID CSkillHtbRpUseIcon::GotoStartFrame()
{
	if( m_pFlaRpBall )
	{
		m_pFlaRpBall->Invoke( "gotoAndPlay", "%s", "start" );
		m_pFlaRpBall->RestartMovie();
	}
}

VOID CSkillHtbRpUseIcon::GotoEndFrame()
{
	if( m_pFlaRpBall )
	{
		m_pFlaRpBall->Invoke( "gotoAndPlay", "%s", "end" );
	}
}

VOID CSkillHtbRpUseIcon::GotoAutoFrame()
{
	if( m_pFlaRpBall )
	{
		m_pFlaRpBall->Invoke( "gotoAndPlay", "%s", "auto" );
	}
}

VOID CSkillHtbRpUseIcon::GotoDisableFrame()
{
	if( m_pFlaRpBall )
	{
		m_pFlaRpBall->Invoke( "gotoAndPlay", "%s", "disable" );
	}
}

VOID CSkillHtbRpUseIcon::OnFSCallBack( const char* pChar1, const char* pChar2 )
{
	if( !strcmp( pChar1, "push" ) )
	{
		if( m_pFlaRpBall )
		{
			Select( TRUE );
			m_pGui->SelectedHtpRpUse( m_byIndex );
		}
	}
	else if( !strcmp( pChar1, "TextOut" ) )
	{
		// Text out
		m_pStbRpBall->SetText( m_byIndex+1 );
	}
}

VOID CSkillHtbRpUseIcon::OnMovieEnd( gui::CComponent* pComponent )
{
	if( m_bSelected )	
		GetDialogManager()->CloseDialog( DIALOG_SKILL_HTB_RPUSE );
}

VOID CSkillHtbRpUseIcon::PlayMovie( RwBool bPlay )
{
	m_pFlaRpBall->PlayMovie( bPlay );
}

VOID CSkillHtbRpUseIcon::SetText( const char* pChar )
{
	m_pStbRpBall->AddText( pChar );
}

VOID CSkillHtbRpUseIcon::ClearText()
{
	m_pStbRpBall->Clear();
}


VOID CSkillHtbRpUseIcon::Select(RwBool bSelect)
{
	m_bSelected = bSelect;
}

VOID CSkillHtbRpUseIcon::Enable( RwBool bEnable )
{
	m_bEnable = bEnable;
}

RwBool CSkillHtbRpUseIcon::IsEnable()
{
	return m_bEnable;
}

RwUInt8 CSkillHtbRpUseIcon::GetIndex()
{
	return m_byIndex;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillHtbRpUse
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSkillHtbRpUse::CSkillHtbRpUse( const RwChar* pName )
: CNtlPLGui( pName )
, m_fPlayTime(0.0f)
, m_bActive(FALSE)
, m_bSelect(FALSE)
, m_bInput(FALSE)
{

}

CSkillHtbRpUse::~CSkillHtbRpUse()
{
	
}

RwBool CSkillHtbRpUse::Create( VOID )
{
	NTL_FUNCTION( "CSkillHtbRpUse::Create" );

	if( !CNtlPLGui::Create( "", "gui\\HTBRpUse.srf", "gui\\HTBRpUse.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis		= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );
	m_pPnlGaugeBack = (gui::CPanel*)GetComponent( "pnlTimeBack" );
	m_pPgbGaugeTime = (gui::CProgressBar*)GetComponent( "pgbLP" );

	// Created 7 Rp usage icons and 1 NONE icon
	for( RwUInt8 i=0; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		m_aRpuseIcon[i].Create( this, dSKILL_HTB_RP_FLASH_BALL, i );
	}
	m_RpUseIconNone.Create( this, dSKILL_HTB_RP_FLASH_NONE, dSKILL_HTB_RP_ICON_NUMS );

	m_slotKeyDown = m_pThis->GetGuiManager()->SigCaptureKeyDown().Connect( this, &CSkillHtbRpUse::OnKeyDown );

	Show( false );

	LinkMsg( g_EventHTBRPStock , 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

VOID CSkillHtbRpUse::Destroy()
{
	NTL_FUNCTION( "CSkillHtbRpUse::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventHTBRPStock );
	UnLinkMsg( g_EventSobInfoUpdate );

	for( RwInt32 i=0; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		m_aRpuseIcon[i].Destroy();
	}
	m_RpUseIconNone.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();	
}

VOID CSkillHtbRpUse::Update( RwReal fElapsed )
{
	if( !m_bActive || !IsShow() )
		return;

	m_fPlayTime += fElapsed;

	for( RwInt32 i=0; i<dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		m_aRpuseIcon[i].Update( fElapsed );
	}
	m_RpUseIconNone.Update( fElapsed );

	// Progress Bar Update
	RwReal fLeftTime = (dSKILL_HTB_RP_USE_TIME - m_fPlayTime);
	RwReal fLeftPercent = fLeftTime / dSKILL_HTB_RP_USE_TIME;
	
	m_pPgbGaugeTime->SetPos( (RwInt32)( fLeftPercent * 1000.0f ) );

	if( m_fPlayTime > dSKILL_HTB_RP_USE_TIME && m_bSelect == FALSE )
	{
		GetDialogManager()->CloseDialog( DIALOG_SKILL_HTB_RPUSE );
	}
}

RwInt32 CSkillHtbRpUse::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( !bOpen )
	{
		m_fPlayTime = 0.0f;
		m_bSelect = FALSE;
		m_bActive = FALSE;
	}

	return TRUE;
}

VOID CSkillHtbRpUse::HandleEvents( RWS::CMsg& msg )
{
	// Event notifying that HTB is being used
	if( msg.Id == g_EventHTBRPStock )
	{
		// Have Rp
		m_uiCurRpStock = Logic_GetRpStock(GetNtlSLGlobal()->GetSobAvatar());

		// Max RP
		RwUInt32 uiMaxRPStock = Logic_GetMaxRpStock(GetNtlSLGlobal()->GetSobAvatar());

		// Open the dialog and not show all components.
		if( GetDialogManager()->IsOpenDialog( DIALOG_SKILL_HTB_RPUSE ) )
			GetDialogManager()->CloseDialog( DIALOG_SKILL_HTB_RPUSE );

		if( GetDialogManager()->IsOpenDialog( DIALOG_SKILLABILITYUSE ) )
			GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYUSE );
		
		GetDialogManager()->OpenDialog( DIALOG_SKILL_HTB_RPUSE );
		InitAllIcon();
		ShowAllIcon( FALSE );

		// Set the size of the current dialog and set the coordinates
		m_pThis->SetSize( (uiMaxRPStock * dSKILL_HTB_RP_WIDTH_OFFSET) + dSKILL_HTB_RP_WIDTH_OFFSET, dSKILL_HTB_RP_HEIGHT );
		m_pThis->SetPosition( (GetDboGlobal()->GetScreenWidth()/2) - (m_pThis->GetScreenRect().GetWidth()/2),
			(RwInt32)((RwReal)GetDboGlobal()->GetScreenHeight() * 0.7f) );
		
		// Show as much as Max
		// Play as much as you have
		for( RwUInt32 i=0; i <m_uiCurRpStock; ++i )
		{
			m_aRpuseIcon[i].Show( true );
			m_aRpuseIcon[i].Enable( TRUE );
			m_aRpuseIcon[i].GotoStartFrame();
			m_aRpuseIcon[i].PlayMovie( true );
			m_aRpuseIcon[i].SetPosition( i * dSKILL_HTB_RP_WIDTH_OFFSET, 0 );
		}

		for( RwUInt32 i = m_uiCurRpStock; i < uiMaxRPStock; ++i )
		{
			m_aRpuseIcon[i].Show( true );
			m_aRpuseIcon[i].Enable( FALSE );
			m_aRpuseIcon[i].PlayMovie( true );
			m_aRpuseIcon[i].GotoDisableFrame();
			
			// Number
			char buf[4];
			sprintf_s( buf, 4, "%d", i+1 );
			m_aRpuseIcon[i].SetText( buf );
			m_aRpuseIcon[i].SetPosition( i * dSKILL_HTB_RP_WIDTH_OFFSET, 0 );
		}

		for( RwUInt32 i = uiMaxRPStock; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
		{
			m_aRpuseIcon[i].Show( false );
			m_aRpuseIcon[i].Enable( FALSE );
		}

		m_RpUseIconNone.SetPosition( uiMaxRPStock * dSKILL_HTB_RP_WIDTH_OFFSET, 0 );
		m_RpUseIconNone.Show( true );
		m_RpUseIconNone.Enable( TRUE );
		m_RpUseIconNone.PlayMovie( TRUE );
		m_RpUseIconNone.GotoStartFrame();

		// Progress Bar Settings
		// location
		RwInt32 nPgbX = (m_pThis->GetScreenRect().GetWidth()/2) - (m_pPnlGaugeBack->GetScreenRect().GetWidth()/2);
		RwInt32 nPgbY = dSKILL_HTP_RP_TIME_OFFSET_Y;

		m_pPnlGaugeBack->SetPosition( nPgbX, nPgbY );
		m_pPgbGaugeTime->SetPosition( nPgbX + dSKILL_HTB_RP_PGB_OFFSET_X, nPgbY + dSKILL_HTB_RP_PGB_OFFSET_Y );
		m_pPgbGaugeTime->SetRange( 0, 1000 );
		m_pPgbGaugeTime->SetPos( 1000 );

		// I'll take your input.
		m_bInput = TRUE;
		m_bActive = TRUE;
		m_fPlayTime = 0.0f;
		m_bSelect = FALSE;
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast< SNtlEventSobInfoUpdate* >( msg.pData );

		if( pUpdate->hSerialId != Logic_GetAvatarHandle() )
			return;

		if( !m_bActive )
			return;

		// Attribute related updates
		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

			// If the saved value is different from the current RP
			if( Logic_GetRpStock( pSobAvatar ) != m_uiCurRpStock && m_bSelect == FALSE )
			{
				m_uiCurRpStock = Logic_GetRpStock( pSobAvatar );

				for( RwUInt32 i = m_uiCurRpStock; i < Logic_GetMaxRpStock( pSobAvatar ); ++i )
				{
					m_aRpuseIcon[i].Enable( FALSE );
					m_aRpuseIcon[i].PlayMovie( true );
					m_aRpuseIcon[i].GotoDisableFrame();
				}

			}
		}
	}
}

VOID CSkillHtbRpUse::OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key )
{
	if( !m_bActive || !m_bInput || m_bSelect )
		return;

	if( key.m_nState == CKey::PRESSED &&
		key.m_nID & 0x40000000 )
		return;

	for( RwInt32 i=0; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		// Available RP
		if( m_aRpuseIcon[i].IsEnable() )
		{
			if( ( key.m_dwVKey - (m_aRpuseIcon[i].GetIndex()+49) ) == 0 )
			{
				GetInputActionMap()->RegisterIgnoreKey( (RwUInt8)key.m_dwVKey, 1 );

				m_aRpuseIcon[i].OnFSCallBack("push", "");
				m_bInput = FALSE;
				return;
			}
		}
		// Icon that is currently disabled and selects as much as the maximum value when pressed
		//else
		//{
		//	if( ( key.m_dwVKey - (m_aRpuseIcon[i].GetIndex()+49) ) == 0 )
		//	{
		//		GetInputActionMap()->RegisterIgnoreKey( (RwUInt8)key.m_dwVKey, 1 );

		//		// Have Rp
		//		m_uiCurRpStock = Logic_GetRpStock(GetNtlSLGlobal()->GetSobAvatar());

		//		// If you don't have any, choose nothing
		//		if( m_uiCurRpStock == 0 )
		//		{
		//			m_RpUseIconNone.OnFSCallBack( "push", "" );
		//			m_bInput = FALSE;
		//			return;
		//		}
		//		else
		//		{
		//			// If you have it, choose the maximum
		//			m_aRpuseIcon[ m_uiCurRpStock-1 ].OnFSCallBack( "push", "" );
		//			m_bInput = FALSE;
		//			return;
		//		}
		//		
		//	}
		//}
	}

	if( m_RpUseIconNone.IsEnable() )
	{
		if( ( key.m_dwVKey - ( m_RpUseIconNone.GetIndex()+49 ) ) == 0 )
		{
			GetInputActionMap()->RegisterIgnoreKey( (RwUInt8)key.m_dwVKey, 1 );

			m_RpUseIconNone.OnFSCallBack( "push", "" );
			m_bInput = FALSE;
			return;
		}
	}
}

VOID CSkillHtbRpUse::SelectedHtpRpUse( RwUInt8 byIndex )
{
	// I'll use 0!
	if( byIndex >= dSKILL_HTB_RP_ICON_NUMS )
	{	
		API_GetSLPacketGenerator()->SendHTBRPBallUseReq(0);

		for( RwInt32 i=0; i <dSKILL_HTB_RP_ICON_NUMS; ++i )
		{
			m_aRpuseIcon[i].Show( false );
			m_aRpuseIcon[i].ClearText();
		}

		m_RpUseIconNone.GotoEndFrame();
		m_RpUseIconNone.ClearText();
	}
	else
	{
		API_GetSLPacketGenerator()->SendHTBRPBallUseReq(byIndex+1);

		for( RwInt32 i=0; i <= byIndex; ++i )
		{
			m_aRpuseIcon[i].GotoEndFrame();
			m_aRpuseIcon[i].ClearText();
		}

		for( RwInt32 i=byIndex+1; i <dSKILL_HTB_RP_ICON_NUMS; ++i )
		{
			m_aRpuseIcon[i].Show( false );
		}
		m_RpUseIconNone.Show( false );
	}

	// When selected
	m_bSelect = TRUE;

	//// Clsoe Dialog
	//GetDialogManager()->CloseDialog( DIALOG_SKILL_HTB_RPUSE );

}

VOID CSkillHtbRpUse::ShowAllIcon( RwBool bShow )
{
	for( RwInt32 i=0; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		m_aRpuseIcon[i].Show( bShow );
	}
	m_RpUseIconNone.Show( bShow );
}

VOID CSkillHtbRpUse::InitAllIcon()
{
	for( RwInt32 i=0; i < dSKILL_HTB_RP_ICON_NUMS; ++i )
	{
		m_aRpuseIcon[i].Enable( FALSE );
		m_aRpuseIcon[i].Select( FALSE );

	}
	m_RpUseIconNone.Enable( FALSE );
	m_RpUseIconNone.Select( FALSE );
}
