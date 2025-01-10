#include "precomp_dboclient.h"
#include "DiceGui.h"

// Core
#include "NtlDebug.h"
#include "NtlMath.h"

// Presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "InfoWndManager.h"
#include "DiceManager.h"
#include "DialogManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"


#define dDICE_FLASH_NAME		"Dice_play.swf"
#define dDICE_LIMIT_TIME		((RwReal)(DBO_MAX_TIME_PARTY_DICE_ITEM/1000))
#define dDICE_WAIT_FOR_DISAPPEAR	3.f		///< Time from when the dice number is displayed until it disappears


//////////////////////////////////////////////////////////////////////////
//	CActionDiceGui
//////////////////////////////////////////////////////////////////////////

CActionDiceGui::~CActionDiceGui()
{

}

RwBool CActionDiceGui::Create(RwUInt8 byFunctionFlag)
{
	NTL_FUNCTION( "CActionDiceGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Dice.srf", "gui\\ActionDice.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// Close window button
	m_pExitButton = (gui::CButton*)GetComponent( "btnExit" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CActionDiceGui::OnClicked_CloseButton);

	// dice flash
	m_pflashDice = (gui::CFlash*)GetComponent("flaDice");
	m_slotMovieEnd = m_pflashDice->SigMovieEnd().Connect( this, &CActionDiceGui::OnMovieEnd );
	m_slotFSCallback = m_pflashDice->SigFSCallBack().Connect( this, &CActionDiceGui::OnFSCallback );

	// Action Dice Description
	m_pActionDiceExplain = (gui::CStaticBox*)GetComponent("stbActionDiceExplain");	

	// Action Dice Description 2
	m_pActionDiceExplain2 = (gui::CStaticBox*)GetComponent("stbActionDiceExplain2");
	m_pActionDiceExplain2->SetTextColor( RGB(255, 192, 0) );

	// Number rolled on the dice
	m_pResultNum = (gui::CStaticBox*)GetComponent("stbResultNum");
	m_pResultNum->CreateFontStd(DEFAULT_FONT, 300, DEFAULT_FONT_ATTR);

	// default value
	m_byFunctionFlag = byFunctionFlag;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLOSE_BUTTON) )
		m_pExitButton->ClickEnable(false);

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
		m_pflashDice->Enable(false);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CActionDiceGui::Destroy()
{
	NTL_FUNCTION("CActionDiceGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	m_pflashDice->Unload();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CActionDiceGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pflashDice->Update(fElapsed);
}

RwBool CActionDiceGui::ReadyDice()
{
	if( m_bGenerateResultEvent )
		return FALSE;

	if( m_pflashDice->Load(dDICE_FLASH_NAME) == false )
	{
		DBO_FAIL("Not eixst dice flash file : " << dDICE_FLASH_NAME );		
		return FALSE;
	}

	CRectangle rtFlash = m_pflashDice->GetFrameResolution();

	m_pflashDice->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());

	return TRUE;
}

VOID CActionDiceGui::SetResultNum(RwUInt8 byNum)
{
	m_byResultNum = byNum;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "click");
	}
	else
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "play");
	}	
}

VOID CActionDiceGui::SetExplain(const WCHAR* pwcText)
{
	m_pActionDiceExplain->SetText(pwcText);
}

VOID CActionDiceGui::SetExplain2(const WCHAR* pwcText)
{
	m_pActionDiceExplain2->SetText(pwcText);
}

VOID CActionDiceGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CActionDiceGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	Switch(false);
	GetDiceManager()->LineUpDice();
}

VOID CActionDiceGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// When the dice stop rolling
	m_pResultNum->SetText(m_byResultNum);

	// Reset to roll the dice again
	ReadyDice();

	if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Enable(true);
		m_pflashDice->RestartMovie();
	}

	m_bGenerateResultEvent = FALSE;
	CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
}

VOID CActionDiceGui::OnFSCallback(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	// When you click on the dice
	CDboEventGenerator::DiceRequestResulttoServer(m_byWorkID);	

	m_pResultNum->Clear();
	m_pflashDice->Enable(false);
	m_bGenerateResultEvent = TRUE;
}

VOID CActionDiceGui::Switch(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		if( m_bGenerateResultEvent )
		{
			// The dice flash animation ended and the dice GUI was closed before showing the results.
			// In addition to the dice GUI, events are sent to places that display dice results (chat, speech bubbles, etc.)
			CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
		}

		CheckInfoWindow();

		Show(false);
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

		if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
			m_pflashDice->Enable(true);

		m_pResultNum->Clear();
		m_pflashDice->Unload();
		m_byResultNum			= 0;
		m_bGenerateResultEvent	= FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
//	CItemDiceGui
//////////////////////////////////////////////////////////////////////////

CItemDiceGui::CItemDiceGui(const RwChar* pName, RwUInt8 byWorkID)
:CDiceGui(pName, byWorkID)
,m_fElapsed(0.f)
,m_pItemProfile(NULL)
{}

CItemDiceGui::~CItemDiceGui()
{

}

RwBool CItemDiceGui::Create(RwUInt8 byFunctionFlag)
{
	NTL_FUNCTION( "CItemDiceGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Dice.srf", "gui\\ItemDice.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// Close window button
	m_pExitButton = (gui::CButton*)GetComponent( "btnExit" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CItemDiceGui::OnClicked_CloseButton);

	// dice flash
	m_pflashDice = (gui::CFlash*)GetComponent("flaDice");
	m_slotMovieEnd = m_pflashDice->SigMovieEnd().Connect( this, &CItemDiceGui::OnMovieEnd );
	m_slotFSCallback = m_pflashDice->SigFSCallBack().Connect( this, &CItemDiceGui::OnFSCallback );

	// Auto Dice Description
	m_pAutoDiceExplain = (gui::CStaticBox*)GetComponent("stbAutoDiceExplain");
	
	// Number rolled on the dice
	m_pResultNum = (gui::CStaticBox*)GetComponent("stbResultNum");
	m_pResultNum->CreateFontStd(DEFAULT_FONT, 300, DEFAULT_FONT_ATTR);

	// Item Slot Background
	m_srfItemSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dice.srf", "srfItemSlot" ) );
	m_srfItemSlot.SetPositionfromParent(228, 18);

	// Dice rolling time limit background
	m_srfTimeGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dice.srf", "srfTimeGaugeBack" ) );
	m_srfTimeGaugeBack.SetPositionfromParent(21, 73);

	m_slot.Create(m_pThis, DIALOG_DICE_MANAGER, REGULAR_SLOT_ITEM_TABLE);
	m_slot.SetPosition_fromParent(230, 34);

	// Dice roll time limit
	m_pTimeGauge = (gui::CProgressBar*)GetComponent("TimeGauge");	

	// default value
	m_byFunctionFlag = byFunctionFlag;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLOSE_BUTTON) )
		m_pExitButton->ClickEnable(false);

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
		m_pflashDice->Enable(false);

	// Signals
	m_slotMove			= m_pThis->SigMove().Connect( this, &CItemDiceGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CItemDiceGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CItemDiceGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CItemDiceGui::OnPaint );

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CItemDiceGui::Destroy()
{
	NTL_FUNCTION("CItemDiceGui::Destroy");

	CheckInfoWindow();

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	m_pflashDice->Unload();

	m_slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CItemDiceGui::Update(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	m_pflashDice->Update(fElapsed);

	if( m_WaitDiceDisappear.bWaitDisappear )
	{
		if( m_WaitDiceDisappear.fElapsedDisappear != 0.f )
		{
			m_WaitDiceDisappear.fElapsedDisappear -= fElapsed;

			if( m_WaitDiceDisappear.fElapsedDisappear < 0.f )
			{
				m_WaitDiceDisappear.fElapsedDisappear = 0.f;
				GetDiceManager()->Cancel(m_byWorkID);
			}
		}
	}


	if( m_fElapsed == dDICE_LIMIT_TIME )
		return;

	m_fElapsed += fElapsed;
	if( m_fElapsed > dDICE_LIMIT_TIME )
	{
		m_fElapsed = dDICE_LIMIT_TIME;
		m_WaitDiceDisappear.bWaitDisappear = TRUE;
	}


	RwInt32 iMaxRange, iPos;

	m_pTimeGauge->GetRange(iPos, iMaxRange); // iPos is used as a dummy here

	iPos = (RwInt32)CNtlMath::Interpolation((RwReal)iMaxRange, 0.f, m_fElapsed/dDICE_LIMIT_TIME);
	m_pTimeGauge->SetPos(iPos);
}

RwBool CItemDiceGui::ReadyDice()
{
	m_fElapsed		= 0.f;

	m_WaitDiceDisappear.fElapsedDisappear	= dDICE_WAIT_FOR_DISAPPEAR;
	m_WaitDiceDisappear.bWaitDisappear		= FALSE;

	m_pflashDice->Enable(true);

	if( m_pflashDice->Load(dDICE_FLASH_NAME) == false )
	{
		DBO_FAIL("Not eixst dice flash file : " << dDICE_FLASH_NAME );		
		return FALSE;
	}

	CRectangle rtFlash = m_pflashDice->GetFrameResolution();

	m_pflashDice->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());
	
	return TRUE;
}

VOID CItemDiceGui::SetItemData(void* pItemData)
{
	if( !pItemData )
	{
		DBO_FAIL("Invalid item data pointer");
		return;
	}

	m_slot.Clear();

	sITEM_PROFILE* pItemProfile = (sITEM_PROFILE*)pItemData;

	if( pItemProfile->bNeedToIdentify )
	{
		m_slot.SetSerialType(REGULAR_SLOT_ITEM_NOT_IDENTIFICATION);
	}
	else
	{
		m_slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
	}

	m_slot.SetIcon(pItemProfile->tblidx);
	m_pItemProfile = pItemProfile;
}

VOID CItemDiceGui::SetExplain(const WCHAR* pwcText)
{
	m_pAutoDiceExplain->SetText(pwcText);
}

VOID CItemDiceGui::SetResultNum(RwUInt8 byNum)
{
	m_byResultNum = byNum;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "click");
	}
	else
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "play");
	}
}

VOID CItemDiceGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	Switch(false);
	GetDiceManager()->LineUpDice();
}

VOID CItemDiceGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// When the dice stop rolling
	m_pResultNum->SetText(m_byResultNum);

	if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Enable(true);
	}

	m_bGenerateResultEvent = FALSE;
	CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
}

VOID CItemDiceGui::OnFSCallback(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	CDboEventGenerator::DiceRequestResulttoServer(m_byWorkID);	

	m_pResultNum->Clear();
	m_pflashDice->Enable(false);
	m_bGenerateResultEvent = TRUE;
}

VOID CItemDiceGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfItemSlot.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTimeGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_slot.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CItemDiceGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
	{
		if( m_slot.PtInRect(nX, nY) )
		{
			if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
				return;

			CRectangle rtScreen = m_pThis->GetScreenRect();

			if( m_slot.GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION )
			{
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													NULL, DIALOG_DICE_MANAGER );
			}
			else
			{
				if( m_pItemProfile )
				{
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													m_pItemProfile, DIALOG_DICE_MANAGER );
				}
				else
				{
					DBO_FAIL("Invalid item profile");
				}
			}

			return;
		}
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnPaint()
{
	m_srfItemSlot.Render();
	m_srfTimeGaugeBack.Render();

	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
		m_slot.Paint();
}

VOID CItemDiceGui::Switch(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		GetNtlGuiManager()->AddUpdateFunc( this );		
	}
	else
	{
		if( m_bGenerateResultEvent )
		{
			// The dice flash animation ended and the dice GUI was closed before showing the results.
			// In addition to the dice GUI, events are sent to places that display dice results (chat, speech bubbles, etc.)
			CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
		}

		CheckInfoWindow();

		Show(false);
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

		if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
			m_pflashDice->Enable(true);

		m_slot.Clear();
		m_pResultNum->Clear();
		m_pflashDice->Unload();
		m_byResultNum			= 0;
		m_bGenerateResultEvent	= FALSE;
		m_pItemProfile			= NULL;
	}
}