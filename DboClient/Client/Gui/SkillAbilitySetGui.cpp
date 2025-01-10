#include "precomp_dboclient.h"
#include "SkillAbilitySetGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobSkillAttr.h"

// Dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "InfoWndManager.h"

// Client
#include "DialogPriority.h"

CSkillAbilitySetGui::CSkillAbilitySetGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_bySlotIdx(0xFF)
, m_bySelAbilityType(0xFF)
, m_bSelAutoMode(FALSE)
{
}

CSkillAbilitySetGui::~CSkillAbilitySetGui()
{

}

RwBool CSkillAbilitySetGui::Create()
{
	NTL_FUNCTION( "CSkillAbilitySetGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\SkillAbilitySet.srf", "gui\\SkillAbilitySet.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis		= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_SKILLABILITY_SET );

	m_pStbTitle	= (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pBtnClose	= (gui::CButton*)GetComponent( "btnClose" );

	// Sklill Ability Type field
	m_paBtn[DBO_RP_BONUS_TYPE_KNOCKDOWN]		= (gui::CButton*)GetComponent("btnKnockDown");
	m_paBtn[DBO_RP_BONUS_TYPE_RESULT_PLUS]		= (gui::CButton*)GetComponent("btnResult");
	m_paBtn[DBO_RP_BONUS_TYPE_EP_MINUS]			= (gui::CButton*)GetComponent("btnEp");
	m_paBtn[DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS]	= (gui::CButton*)GetComponent("btnKeepTime");
	m_paBtn[DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS] = (gui::CButton*)GetComponent("btnCastingTime");
	m_paBtn[DBO_RP_BONUS_TYPE_COOL_TIME_MINUS]	= (gui::CButton*)GetComponent("btnCoolTime");
	m_paBtn[DBO_RP_BONUS_TYPE_GUARD_CRASH]		= (gui::CButton*)GetComponent("btnGuardCrash");

	m_pStbManual = (gui::CStaticBox*)GetComponent("stbManual");
	m_pBtnManual = (gui::CButton*)GetComponent("btnManual");
	m_pStbDisable = (gui::CStaticBox*)GetComponent("stbDisable");
	m_pBtnDisable = (gui::CButton*)GetComponent("btnDisable" );

	m_pBtnAccept	= (gui::CButton*)GetComponent("btnAccept");
	m_pBtnCancel	= (gui::CButton*)GetComponent("btnCancel");

	// Locations of Icons
	m_aIconPosX[ICONPOS_FIRST_LEFT]		= 12;
	m_aIconPosX[ICONPOS_FIRST_CENTER]	= 66;
	m_aIconPosX[ICONPOS_FIRST_RIGHT]	= 120;
	m_aIconPosX[ICONPOS_SECOND_LEFT]	= 12;
	m_aIconPosX[ICONPOS_SECOND_CENTER]	= 66;
	m_aIconPosX[ICONPOS_SECOND_RIGHT]	= 120;
	m_aIconPosY[ICONPOS_FIRST_LEFT]		= 38;
	m_aIconPosY[ICONPOS_FIRST_CENTER]	= 38;
	m_aIconPosY[ICONPOS_FIRST_RIGHT]	= 38;
	m_aIconPosY[ICONPOS_SECOND_LEFT]	= 92;
	m_aIconPosY[ICONPOS_SECOND_CENTER]	= 92;
	m_aIconPosY[ICONPOS_SECOND_RIGHT]	= 92;

	

	m_pPanFocus = (gui::CPanel*)GetComponent("panFocus");

	// Signals
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnClose );
	for( int i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		m_aSlotClickedBtn[i] = m_paBtn[i]->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnSetting );
		m_aSlotMouseMove[i] = m_paBtn[i]->SigMouseMove().Connect( this, &CSkillAbilitySetGui::OnMouseMove );
	}
	m_slotClickedBtnManual = m_pBtnManual->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnSetting );
	m_slotClickedBtnDisable = m_pBtnDisable->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnSetting );
	/*m_slotMouseMoveBtnManual = m_pBtnManual->SigMouseMove().Connect( this, &CSkillAbilitySetGui::OnMouseMove );
	m_slotMouseMoveBtnDisable = m_pBtnDisable->SigMouseMove().Connect( this, &CSkillAbilitySetGui::OnMouseMove );*/
	m_slotClickedBtnAccept = m_pBtnAccept->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnAccept );
	m_slotClickedBtnCancel = m_pBtnCancel->SigClicked().Connect( this, &CSkillAbilitySetGui::OnClickedBtnClose );
	/*m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CSkillAbilitySetGui::OnMouseMove );*/
	m_slotMouseMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseMove().Connect( this, &CSkillAbilitySetGui::OnMouseMove );
	

	// Text
	m_pStbTitle->SetText( GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TEXT_TITLE" ) );
	m_pStbManual->SetText( GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TEXT_MANUAL" ) );
	m_pStbDisable->SetText( GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TEXT_DISABLE" ) );
	m_pBtnAccept->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_OK" ) );
	m_pBtnCancel->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_CANCEL" ) );

	Show ( false );

	// Links
	LinkMsg( g_EventRpBonusSetup );

	NTL_RETURN( TRUE );
}

VOID CSkillAbilitySetGui::Destroy()
{
	NTL_FUNCTION( "CSkillAbiltiySetGui::Destroy" );

	UnLinkMsg( g_EventRpBonusSetup );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CSkillAbilitySetGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRpBonusSetup )
	{
		SDboEventRpBonusSetup* pData = reinterpret_cast< SDboEventRpBonusSetup*> ( msg.pData );

		CNtlSobSkillIcon* pSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pData->pData );
		CNtlSobSkill* pSkill = (CNtlSobSkill*)pSkillIcon->GetSobObj();
		CNtlSobSkillAttr* pSkillAttr = (CNtlSobSkillAttr*)pSkill->GetSobAttr();
		sSKILL_TBLDAT* pSkillTbl = pSkillAttr->GetSkillTbl();

		SetPosPopup( pData->iXPos, pData->iYPos );

		m_bySlotIdx = pSkill->GetSlotIdx();
		

		// If it was open, close it and open it again. (To apply Show/Hide properly)
		if( GetDialogManager()->IsOpenDialog( DIALOG_SKILLABILITYSET ) )
			GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYSET );

		GetDialogManager()->OpenDialog( DIALOG_SKILLABILITYSET );

		// Type Setting
		ShowAllAbility( false );

		// Reset tooltip information
		SetMoveToolTipPos();
		memset( m_asTooltipType, 0xFF, sizeof( SAbilityInfo ) * ICONPOS_NUMS );

		for(int i=0; i < ICONPOS_NUMS; ++i )
			SetAbility( (eICONPOS)i, pSkillTbl->abyRpEffect[i], pSkillTbl->afRpEffectValue[i], pSkillTbl );

		// Focus settings
		m_pPanFocus->Show( false );
		if( !pSkillAttr->m_bRPBonusAutoMode )
		{
			// manual mode
			m_pBtnManual->ClickEnable( false );
		}
		else
		{
			if( pSkillAttr->m_byRPBonusType == DBO_RP_BONUS_TYPE_INVALID )
			{
				// Disabled
				m_pBtnDisable->ClickEnable( false );
			}
			else
			{
				if( pSkillAttr->m_byRPBonusType >= DBO_RP_BONUS_TYPE_COUNT )
				{
					DBO_FAIL( "CSkillAbilitySetGui::HandleEvents - invalid Ability Type!" );
					return;
				}

				if( m_abyAbilityIconPos[pSkillAttr->m_byRPBonusType] >= ICONPOS_NUMS )
				{
					DBO_FAIL( "CSkillAbilitySetGui::HandleEvents - Invalid Icon number!" );
					return;
				}
				
				m_pPanFocus->SetPosition( m_aIconPosX[ m_abyAbilityIconPos[pSkillAttr->m_byRPBonusType] ],
					m_aIconPosY[ m_abyAbilityIconPos[pSkillAttr->m_byRPBonusType] ] );
				m_pPanFocus->Show( true );
			}
		}

		RaiseTop();
	}
}

RwInt32 CSkillAbilitySetGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( !bOpen )
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILLABILITYSET )
		{
			GetInfoWndManager()->ShowInfoWindow(FALSE);
		}
	}

	return 1;
}

VOID CSkillAbilitySetGui::SetPosPopup( RwInt32 nX, RwInt32 nY )
{
	int nNewX, nNewY;
	if( nX > (RwInt32)(GetDboGlobal()->GetScreenWidth()/2) )
	{
		nNewX = nX - m_pThis->GetWidth() - 40;
	}
	else
	{
		nNewX = nX + 40;
	}

	if( nY > (RwInt32)(GetDboGlobal()->GetScreenHeight()/2) )
	{
		nNewY = nY - m_pThis->GetHeight() - 20;
	}
	else
	{
		nNewY = nY + 20;
	}
	
	m_pThis->SetPosition( nNewX , nNewY );
}

VOID CSkillAbilitySetGui::SetAbility( eICONPOS eIconPos, RwUInt8 byAbilityType, RwReal fValue, sSKILL_TBLDAT* pSkillTbl, RwBool bEnable /*= TRUE */ )
{
	if( byAbilityType == DBO_RP_BONUS_TYPE_INVALID )
	{
		if( pSkillTbl->dwNextSkillTblidx == INVALID_DWORD )
		{
			return;
		}

		// If RPBONUS TYPE is an INVALID value, refer to the index of the following table.
		sSKILL_TBLDAT* pNextSkillTbl = Logic_GetSkillDataFromTable( pSkillTbl->dwNextSkillTblidx );

		// If the value of the array index in the next skill table is also INVALID, the next value can be checked.
		// And from this point on, the skills referenced are not necessarily at the current level, so they are in a disabled state.
		// Display.
		SetAbility( eIconPos, pNextSkillTbl->abyRpEffect[ eIconPos ],
			pNextSkillTbl->afRpEffectValue[ eIconPos ], pNextSkillTbl, FALSE );
	}
	else
	{
		if( byAbilityType >= DBO_RP_BONUS_TYPE_INVALID )
		{
			DBO_FAIL( "CSkillAbilitySetGui::SetAbility - byAbilityType over from DBO_RP_BONUS_TYPE" );
			return;
		}

		if( eIconPos >= ICONPOS_NUMS )
		{
			DBO_FAIL( "CSkillAbilitySetGui::SetAbility - eIconPos over from ICONPOS" );
			return;
		}
		
		m_abyAbilityIconPos[ byAbilityType ] = (RwUInt8)eIconPos;

		// ButtonÀÇ Position
		m_paBtn[ byAbilityType ]->SetPosition( m_aIconPosX[ eIconPos ], m_aIconPosY[ eIconPos ] );
		CRectangle rect = m_paBtn[ byAbilityType ]->GetScreenRect();

		// Save ToolTip's Info
		m_asTooltipType[eIconPos].byAbilityType = byAbilityType;
		m_asTooltipType[eIconPos].fValue = fValue;
		m_asTooltipType[eIconPos].byEffectType = pSkillTbl->bySkill_Effect_Type[NTL_SYSTEM_EFFECT_1];
		m_asTooltipType[eIconPos].nX = rect.left;
		m_asTooltipType[eIconPos].nY = rect.top;

		if( !bEnable )
			m_paBtn[ byAbilityType ]->ClickEnable( false );

		m_paBtn[ byAbilityType ]->Show( true );
		// Add a ToolTip by referring to uiValue.
	}
}

VOID CSkillAbilitySetGui::ShowAllAbility( bool bShow )
{
	for( int i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		m_paBtn[i]->Show( bShow );
		m_paBtn[i]->ClickEnable( true );
	}

	m_pBtnManual->ClickEnable( true );
	m_pBtnDisable->ClickEnable( true );
}

VOID CSkillAbilitySetGui::SelectedAbility( RwUInt8 byAbilityType, RwBool bAutoMode )
{
	m_bySelAbilityType = byAbilityType;
	m_bSelAutoMode = bAutoMode;

	m_pBtnManual->ClickEnable( true );
	m_pBtnDisable->ClickEnable( true );
	m_pPanFocus->Show( false );

	if( !bAutoMode )
	{
		m_pBtnManual->ClickEnable( FALSE );
	}
	else
	{
		if( byAbilityType == DBO_RP_BONUS_TYPE_INVALID )
		{
			m_pBtnDisable->ClickEnable( false );
		}
		else
		{
			CRectangle rect = m_paBtn[byAbilityType]->GetScreenRect();
			CRectangle rectParent = m_pThis->GetScreenRect();
			CRectangle rectOffset = rect - rectParent ;
			m_pPanFocus->SetPosition( rectOffset.left, rectOffset.top );
			m_pPanFocus->Show( true );
		}
	}
}

VOID CSkillAbilitySetGui::OnClickedBtnClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYSET );
}

VOID CSkillAbilitySetGui::OnClickedBtnSetting( gui::CComponent* pComponent )
{
	if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_KNOCKDOWN] )
		SelectedAbility( DBO_RP_BONUS_TYPE_KNOCKDOWN , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_RESULT_PLUS] )
		SelectedAbility( DBO_RP_BONUS_TYPE_RESULT_PLUS , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_EP_MINUS] )
		SelectedAbility( DBO_RP_BONUS_TYPE_EP_MINUS , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS] )
		SelectedAbility( DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS] )
		SelectedAbility( DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_COOL_TIME_MINUS] )
		SelectedAbility( DBO_RP_BONUS_TYPE_COOL_TIME_MINUS , true );
	else if( pComponent == m_paBtn[DBO_RP_BONUS_TYPE_GUARD_CRASH] )
		SelectedAbility( DBO_RP_BONUS_TYPE_GUARD_CRASH , true );
	else if( pComponent == m_pBtnManual )
		SelectedAbility( DBO_RP_BONUS_TYPE_INVALID, false );
	else if( pComponent == m_pBtnDisable )
		SelectedAbility( DBO_RP_BONUS_TYPE_INVALID , true );
}

VOID CSkillAbilitySetGui::OnClickedBtnAccept( gui::CComponent* pComponent )
{
	GetDboGlobal()->GetGamePacketGenerator()->SendSkillRpBonusSettingReq( m_bySlotIdx, m_bySelAbilityType, m_bSelAutoMode );
	GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYSET );
}

VOID CSkillAbilitySetGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	if( !IsShow() )
		return;

	nX = CMouse::GetX();
	nY = CMouse::GetY();

	for( int i=0; i < ICONPOS_NUMS; ++i )
	{
		// When the mouse moves to a designated location, an appropriate info window appears.
		if( m_rtIconPos[i].PtInRect( nX, nY ) && m_asTooltipType[i].byAbilityType != DBO_RP_BONUS_TYPE_INVALID )
		{
			SetToolTip( m_asTooltipType[i].nX,
			m_asTooltipType[i].nY, 
			m_asTooltipType[i].byAbilityType, 
			m_asTooltipType[i].fValue,
			m_asTooltipType[i].byEffectType );

			return;
		}
	}

	CRectangle rectManual = m_pBtnManual->GetScreenRect();
	if( rectManual.PtInRect( nX, nY ) )
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
			rectManual.left, rectManual.top, 
			(VOID*)GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_MANUAL" ),
			DIALOG_SKILLABILITYSET );

		return;
	}
	
	CRectangle rectDisable = m_pBtnDisable->GetScreenRect();
	if( rectDisable.PtInRect( nX, nY ) )
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
			rectDisable.left, rectDisable.top, 
			(VOID*)GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_INVALID" ),
			DIALOG_SKILLABILITYSET );
	
		return;
	}

	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILLABILITYSET )
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

VOID CSkillAbilitySetGui::SetMoveToolTipPos()
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_rtIconPos[ICONPOS_FIRST_LEFT].SetRectWH( rect.left + 12, rect.top + 38, 46, 46 );
	m_rtIconPos[ICONPOS_FIRST_CENTER].SetRectWH( rect.left + 66, rect.top + 38, 46, 46 );
	m_rtIconPos[ICONPOS_FIRST_RIGHT].SetRectWH( rect.left + 120, rect.top + 38, 46, 46 );
	m_rtIconPos[ICONPOS_SECOND_LEFT].SetRectWH( rect.left + 12, rect.top + 92, 46, 46 );
	m_rtIconPos[ICONPOS_SECOND_CENTER].SetRectWH( rect.left + 66, rect.top + 92, 46, 46 );
	m_rtIconPos[ICONPOS_SECOND_RIGHT].SetRectWH( rect.left + 120, rect.top + 92, 46, 46 );
}

VOID CSkillAbilitySetGui::SetToolTip( RwInt32 nX, RwInt32 nY, RwUInt8 byAbilityType, RwReal fValue, RwUInt8 byEffectType )
{
	switch( byAbilityType )
	{
	case DBO_RP_BONUS_TYPE_KNOCKDOWN:
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
			nX, nY, 
			(VOID*)GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_KNOCKDOWN" ),
			DIALOG_SKILLABILITYSET );
		break;
	case DBO_RP_BONUS_TYPE_RESULT_PLUS:
		{
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_RESULT_ABSOLUTE" ), Logic_NumToWchar(fValue));
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				nX, nY, 
				(VOID*)awcBuffer ,
				DIALOG_SKILLABILITYSET );
		}
		break;
	case DBO_RP_BONUS_TYPE_EP_MINUS:
		{
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_EP_MINUS" ), Logic_NumToWchar(fValue));
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				nX, nY, 
				(VOID*)awcBuffer,
				DIALOG_SKILLABILITYSET );
		}
		break;
	case DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS:
		{
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_KEEPTIME_PLUS" ), Logic_NumToWchar(fValue));
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				nX, nY, 
				(VOID*)awcBuffer,
				DIALOG_SKILLABILITYSET );
		}
		break;
	case DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS:
		{
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_CASTING_TIME_MINUS" ), Logic_NumToWchar(fValue));
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				nX, nY, 
				(VOID*)awcBuffer,
				DIALOG_SKILLABILITYSET );
		}
		break;
	case DBO_RP_BONUS_TYPE_COOL_TIME_MINUS:
		{
			WCHAR awcBuffer[512];
			swprintf_s( awcBuffer, 512, GetDisplayStringManager()->GetString( "DST_SKILL_RPBONUS_TOOLTIP_COOLTIME_MINUS" ), Logic_NumToWchar(fValue));
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				nX, nY, 
				(VOID*)awcBuffer,
				DIALOG_SKILLABILITYSET );
		}
		break;
	case DBO_RP_BONUS_TYPE_GUARD_CRASH:
		WCHAR awcBuffer[512];
		swprintf_s(awcBuffer, 512, GetDisplayStringManager()->GetString("DST_SKILL_RPBONUS_TOOLTIP_GUARD_CRASH"), Logic_NumToWchar(fValue));

		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
			nX, nY, 
			(VOID*)awcBuffer,
			DIALOG_SKILLABILITYSET );
		break;
	default:
		break;
	}
}