#include "precomp_dboclient.h"
#include "BroadCastGui.h"

// Core
#include "NtlDebug.h"

// gui library
#include "gui_renderer.h"
#include "GuiUtil.h"

// Table
#include "TableContainer.h"
#include "NpcTable.h"
#include "MobTable.h"

// simulation layer
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// Presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DialogPriority.h"


CBroadCastGui::CBroadCastGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_pPnlBack(NULL)
, m_pStbName(NULL)
, m_pHtmlText(NULL)
, m_pBroadCastConfig(NULL)
{
	for(RwInt32 i=0; i<dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		m_paFlaBalloon[i] = NULL;
	}

	m_byState = STATE_CLOSE;
	m_bCurUnit = FALSE;
	m_bPlay = FALSE;
	m_bSmallText = FALSE;

	m_fElapsed = 0.0f;
	m_fShakeTime = 0.0f;
}

CBroadCastGui::~CBroadCastGui()
{

}

RwBool CBroadCastGui::Create()
{
	NTL_FUNCTION( "CBroadCastGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BroadCastGui.srf", "gui\\BroadCastGui.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis			= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pPnlBack		= (gui::CPanel*)GetComponent( "backPanel" );
	m_pStbName		= (gui::CStaticBox*)GetComponent( "stbNameBox" );
	m_pHtmlText		= (gui::CHtmlBox*)GetComponent( "htmlText" );
	m_pHtmlText->SetLineSpace( 4 );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL] = (gui::CFlash*)GetComponent( "flaBalloonNone" );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR] = (gui::CFlash*)GetComponent( "flaBalloonStar" );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK] = (gui::CFlash*)GetComponent( "flaBalloonThink" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL] = (gui::CFlash*)GetComponent( "flaBalloonSmallNone" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR] = (gui::CFlash*)GetComponent( "flaBalloonSmallStar" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK] = (gui::CFlash*)GetComponent( "flaBalloonSmallThink" );
	m_pPnlIllust	= (gui::CPanel*)GetComponent( "pnlIllust" );

	m_surIllust.SetRect( m_pThis->GetScreenRect() );

	// Signal
	m_slotPaint	= m_pPnlIllust->SigPaint().Connect( this, &CBroadCastGui::OnPaint );
	m_slotMove	= m_pThis->SigMove().Connect( this, &CBroadCastGui::OnMove );

	LinkMsg( g_EventBroadMsgBegNfy, 0 );
	LinkMsg( g_EventBroadMsgEndNfy, 0 );
	LinkMsg( g_EventWorldChange, 0 );

	// Function callback
	m_aCallUpdateState[STATE_DIRECTION_IN_START]	= &CBroadCastGui::UpdateDirectionInStart;
	m_aCallUpdateState[STATE_DIRECTION_IN]			= &CBroadCastGui::UpdateDirectionIn;
	m_aCallUpdateState[STATE_DIRECTION_IN_END]		= &CBroadCastGui::UpdateDirectionInEnd;
	m_aCallUpdateState[STATE_OPEN]					= &CBroadCastGui::UpdateOpen;
	m_aCallUpdateState[STATE_DIRECTION_OUT_START]	= &CBroadCastGui::UpdateDirectionOutStart;
	m_aCallUpdateState[STATE_DIRECTION_OUT]			= &CBroadCastGui::UpdateDirectionOut;
	m_aCallUpdateState[STATE_DIRECTION_OUT_END]		= &CBroadCastGui::UpdateDirectionOutEnd;
	m_aCallUpdateState[STATE_CLOSE]					= &CBroadCastGui::UpdateClose;

	// Init (set to not show on screen)
	m_pThis->SetPosition( GetDboGlobal()->GetScreenWidth(), 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	m_pBroadCastConfig = GetDboGlobal()->GetDBOUIConfig()->GetBroadCastConfig();
	
	Show( false );

	NTL_RETURN(TRUE);
}

VOID CBroadCastGui::Destroy()
{
	NTL_FUNCTION( "CBroadCastGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventBroadMsgBegNfy );
	UnLinkMsg( g_EventBroadMsgEndNfy );
	UnLinkMsg( g_EventWorldChange );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 
}

VOID CBroadCastGui::Update( RwReal fElapsed )
{
	if( !m_bPlay )
		return;

	// If Flash is playing, it will be updated.
	for( RwInt32 i=0; i<dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		if( m_paFlaBalloon[i]->IsPlayMovie() )
			m_paFlaBalloon[i]->Update( fElapsed );

		if( m_paFlaBalloonSmall[i]->IsPlayMovie() )
			m_paFlaBalloonSmall[i]->Update( fElapsed );
	}

	if( m_pHtmlText->IsVisible() )
		m_pHtmlText->Update( fElapsed );

	// Call the Update function for each state
	(this->*m_aCallUpdateState[m_byState])( fElapsed );
}

VOID CBroadCastGui::HandleEvents( RWS::CMsg &msg )
{
	// g_EventBroadMsgBegNfy: Event to start BroadCast Message
	if( msg.Id == g_EventBroadMsgBegNfy )
	{
		SNtlEventBroadMsgBegNfy* pNotify = reinterpret_cast< SNtlEventBroadMsgBegNfy* >( msg.pData );

		// If there is no unit currently being output, it starts immediately.
		// If there is a unit that is currently being output, set it as new data according to the type that is currently being output.
		if( !m_bCurUnit )
		{
			SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
			SetCurMsgData( m_CurUnit );

			Play( TRUE );

			SetState( STATE_DIRECTION_IN_START );

			return;
		}
		else
		{
			switch( m_CurUnit.m_byMsgType )
			{
			case eBROAD_MSG_TYPE_TS:
			case eBROAD_MSG_TYPE_EMERGENCY:
				{
					// It refers to the same type, completely terminates the current one, and sets it with new data.
					if( pNotify->byMsgType == eBROAD_MSG_TYPE_TS ||
						pNotify->byMsgType == eBROAD_MSG_TYPE_EMERGENCY )
					{
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );
						
						return;
					}
					else
					{
						DeleteDeqUnit( pNotify->byMsgType );

						// Save the current unit and set it as a new unit to play.
						SaveCurUnitData();
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
				}
				break;
			case eBROAD_MSG_TYPE_MINI_NARRATION:
				{
					if( pNotify->byMsgType != eBROAD_MSG_TYPE_MINI_NARRATION )
					{
						// If there are TS and Emergency, delete them.
						DeleteDeqUnit( eBROAD_MSG_TYPE_TS );
						DeleteDeqUnit( eBROAD_MSG_TYPE_EMERGENCY );
						
						// Save the current unit and set it as a new unit to play.
						SaveCurUnitData();

						// New unit settings
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
					else
					{
						// In the case of mini narration, ignore the current mini narration and set a new mini narration.
						// Print out.
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
				}
				break;
			}
		}
	}
	// g_EventBroadMsgEndNfy : Event to delete the currently output message
	else if( msg.Id == g_EventBroadMsgEndNfy )
	{
		SNtlEventBroadMsgEndNfy* pNotify = reinterpret_cast< SNtlEventBroadMsgEndNfy* >( msg.pData );

		// If the unit currently being output is a unit to be deleted, it goes to the terminated state. If the unit is reserved,
		// Delete all of the same types among reserved units.
		if( m_bCurUnit )
		{
			if( pNotify->byMsgType == m_CurUnit.m_byMsgType )
			{
				m_bCurUnit = FALSE;
				SetState( STATE_DIRECTION_OUT_START );
			}
			else
			{
				DeleteDeqUnit( pNotify->byMsgType );
			}
		}
	}
	// g_EventWorldChange: Event that the world has changed. Terminates the currently printing process and deletes all reserved units.
	if( msg.Id == g_EventWorldChange )
	{
		SNtlEventWorldChange* pWorldChange = reinterpret_cast<SNtlEventWorldChange*>(msg.pData);

		if(pWorldChange->bWorldChange)      
		{            
			if( m_bCurUnit )
			{
				m_bCurUnit = FALSE;
				SetState( STATE_DIRECTION_OUT_START );

				DeleteDeqUnitAll();
			}
		}
	}
}

/**
* \brief The production that appears begins
*/
VOID CBroadCastGui::UpdateDirectionInStart( RwReal fElapsed )
{
	// Set Illust.
	SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
	SetillustPos( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		
	// Open the dialog.
	GetDialogManager()->OpenDialog( DIALOG_BROADCAST );

	// Set the content in the HtmlBox
	SetHtmlString( m_sCurData.wstrSpeech.c_str(), m_sCurData.wstrSpeech.length() );
	m_pHtmlText->Show( false );
	
	// Hide all speech bubbles.
	HideAllBalloon();

	// BroadCast and calculates the X and Y positions on the screen. Read from (UIConfig.xml)
	RwInt32 nXPos = (RwInt32)(m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth());
	RwInt32 nYPos = (RwInt32)(m_pBroadCastConfig->fPosYRate * GetDboGlobal()->GetScreenHeight());

	// Initializes according to ShowHide Type.
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		{
			m_pThis->SetPosition( nXPos, nYPos );
			SetillustPos( nXPos, nYPos );
			
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );

			ShowBalloon( (RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			m_pThis->SetPosition( nXPos, nYPos );
			SetillustPos( nXPos, nYPos );
				
			m_pThis->SetAlpha( 0 );
			m_surIllust.SetAlpha( 0 );

			ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
		
			m_conAlpha.SetAccel(0.0f, 255.0f, m_pBroadCastConfig->fBlendStartVel,
				m_pBroadCastConfig->fBlendAccelVel );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			m_pThis->SetPosition( GetDboGlobal()->GetScreenWidth(), nYPos );
			SetillustPos( GetDboGlobal()->GetScreenWidth(), nYPos );

			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			
			m_conAccel.SetAccel( (RwReal)GetDboGlobal()->GetScreenWidth(), 
				(RwReal)m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth(), 
				(RwReal)m_pBroadCastConfig->fSlideStartVel, 
				(RwReal)m_pBroadCastConfig->fSlideAccelVel );

		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			m_pThis->SetPosition( (RwInt32)(nXPos)+(RwInt32)m_pBroadCastConfig->fFadeSlidePos, nYPos );
			SetillustPos( (RwInt32)(nXPos)+(RwInt32)m_pBroadCastConfig->fFadeSlidePos, nYPos );

			m_conAccel.SetAccel( (RwReal)(m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth())+m_pBroadCastConfig->fFadeSlidePos, 
				(RwReal)m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth(), 
				m_pBroadCastConfig->fFadeSlideStartVel, 
				m_pBroadCastConfig->fFadeSlideAccelVel );

			m_pThis->SetAlpha( 0 );
			m_surIllust.SetAlpha( 0 );
		}
		break;
	}

	SetState( STATE_DIRECTION_IN );
}

/**
* \brief Incoming production
*/
VOID CBroadCastGui::UpdateDirectionIn( RwReal fElapsed )
{
	// According to the Show Hide, the work that needs to be done during the incoming production is performed.
	RwInt32 nYPos = (RwInt32)(m_pBroadCastConfig->fPosYRate * GetDboGlobal()->GetScreenHeight());
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		SetState( STATE_DIRECTION_IN_END );
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			RwBool bUpdate = m_conAlpha.Update( fElapsed );
			m_pThis->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );
			m_surIllust.SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), nYPos);

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), nYPos );
			
			m_pThis->SetAlpha( (RwUInt8)(m_conAccel.GetCurrentProgress() * 255.f ));
			m_surIllust.SetAlpha( (RwUInt8)(m_conAccel.GetCurrentProgress() * 255.f) );

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	}	
}

/**
* \brief When the incoming production ends
*/
VOID CBroadCastGui::UpdateDirectionInEnd( RwReal fElapsed )
{
	// Interval state recovery
	m_pHtmlText->SetIntervalTextDisable();

	// Show/Hide Direction
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			ShowBalloon( (RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			
			ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	}

	// Text Direction
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
		{
			m_conAlpha.SetAccel( 0.f, 255.f, m_pBroadCastConfig->fBlendTextStartVel,
				m_pBroadCastConfig->fBlendTextAccelVel );
			m_pHtmlText->SetAlpha( 0 );
		}
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		{
			m_pHtmlText->SetIntervalTextEnable( m_pBroadCastConfig->fIntervalTextTime );
			m_pHtmlText->SetPresentNum( (RwInt32)m_pBroadCastConfig->fIntervalPresNum );
		}
		break;
	}

	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	}

	m_fShakeTime = 0.0f;
	m_fElapsed = 0.0f;

	SetState( STATE_OPEN );
}

/**
* \brief Direction during opening
*/
VOID CBroadCastGui::UpdateOpen( RwReal fElapsed )
{
	// Text Direction (only blending is produced separately)
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
		{
			RwBool bUpdate = m_conAlpha.Update( fElapsed );
			m_pHtmlText->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				m_pHtmlText->SetAlpha( 255 );
		}
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		break;
	}

	// Shake Direction (Shake is common)
	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_NORMAL] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_NORMAL];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_NORMAL] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_NORMAL];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY );
		}
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_WARNING] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_WARNING];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_WARNING] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_WARNING];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY );
		}
		break;
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_DANGER] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_DANGER];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_DANGER] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_DANGER];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY);
		}
		break;
	}

	m_fElapsed += fElapsed;
	
	// When the current unit's time expires, it moves on.
	if( m_sCurData.fDisplayTime < m_fElapsed )
	{
		// Special case: If HtmlData has the next page, it returns to the Open state again. (In contrast to Line)
		if( m_pHtmlText->SetNextPage() )
		{
			m_fElapsed = 0.0f;
			SetState( STATE_OPEN );
			return;
		}

		vecdef_BroadMsgDataList::iterator it = m_CurUnit.m_vecMsgDataList.begin();
		m_CurUnit.m_vecMsgDataList.erase( it );
		SetState( STATE_DIRECTION_OUT_START );
	}
}

/**
* \brief The beginning of the ending and the production
*/
VOID CBroadCastGui::UpdateDirectionOutStart( RwReal fElapsed )
{
	// Text Direction Things that need to be restored
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		{
			m_pHtmlText->SetIntervalTextDisable();
		}
		break;
	}

	// Shake Direction (Shake is common)
	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		{
			SetillustPos( m_rectBackIllust.left, m_rectBackIllust.top );
		}
		break;
	}

	// If there are any remaining messages in the current unit's message list, set the illustration, text, balloon, etc.
	// It goes back to STATE_OPEN state. (Only if data is valid)
	if( !m_CurUnit.m_vecMsgDataList.empty() && m_bCurUnit )
	{
		RwBool bChangeIllust = FALSE;
		sBROAD_MSG_DATA* pOldData = (sBROAD_MSG_DATA*)&(*m_CurUnit.m_vecMsgDataList.begin());
		if( m_sCurData.uiOwnerTblIdx != pOldData->uiOwnerTblIdx )
			bChangeIllust = TRUE;

		SetCurMsgData( m_CurUnit );

		if( bChangeIllust )
		{
			switch( m_CurUnit.m_byMsgType )
			{
			case eBROAD_MSG_TYPE_TS:
			case eBROAD_MSG_TYPE_MINI_NARRATION:
				{
					SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
				}
				break;
			case eBROAD_MSG_TYPE_EMERGENCY:
				{
					SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
				}
				break;
			}
		}

		// Text settings
		SetHtmlString( m_sCurData.wstrSpeech.c_str(), m_sCurData.wstrSpeech.length() );
		m_pHtmlText->Show( true );

		// Balloon setting
		HideAllBalloon();
		ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );

		// Text Direction
		switch( m_sCurData.eUISpeechDirType )
		{
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
			{
			}
			break;
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
			{
				m_conAlpha.SetAccel( 0.f, 255.f, m_pBroadCastConfig->fBlendTextStartVel,
					m_pBroadCastConfig->fBlendTextAccelVel );
				m_pHtmlText->SetAlpha( 0 );
			}
			break;
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
			{
				m_pHtmlText->SetIntervalTextEnable( m_pBroadCastConfig->fIntervalTextTime );
				m_pHtmlText->SetPresentNum( (RwInt32)m_pBroadCastConfig->fIntervalPresNum );
			}
			break;
		}

		m_fElapsed = 0.0f;

		SetState( STATE_OPEN );
		
		return;
	}
	
	// Show / Hide Direction
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			// It must be added or subtracted from the current state.
			m_conAlpha.SetAccel( (RwReal)m_surIllust.GetAlpha(), (RwReal)0, m_pBroadCastConfig->fBlendStartVel,
				m_pBroadCastConfig->fBlendAccelVel );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			// Close speech bubble
			HideAllBalloon();
			m_pHtmlText->Show( false );
			// Apply a sliding factor using an acceleration controller
			m_conAccel.SetAccel( (RwReal)m_pThis->GetScreenRect().left, 
				(RwReal)GetDboGlobal()->GetScreenWidth(), 
				m_pBroadCastConfig->fSlideStartVel, 
				m_pBroadCastConfig->fSlideAccelVel);
			SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			// Close speech bubble
			HideAllBalloon();
			m_pHtmlText->Show( false );
			// Apply a sliding factor using an acceleration controller
			m_conAccel.SetAccel( (RwReal)m_pThis->GetScreenRect().left, 
				(RwReal)(m_pBroadCastConfig->fPosXRate*GetDboGlobal()->GetScreenWidth())+m_pBroadCastConfig->fFadeSlidePos, 
				m_pBroadCastConfig->fFadeSlideStartVel, 
				m_pBroadCastConfig->fFadeSlideAccelVel);

			SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		}
		break;
	}

	SetState( STATE_DIRECTION_OUT );
}

/**
* \brief Ending production
*/
VOID CBroadCastGui::UpdateDirectionOut( RwReal fElapsed )
{
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		{
			SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			RwBool bUpdate = m_conAlpha.Update(fElapsed);
			m_pThis->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );
			m_surIllust.SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), (RwInt32)(GetDboGlobal()->GetScreenHeight() * m_pBroadCastConfig->fPosYRate));

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), (RwInt32)(GetDboGlobal()->GetScreenHeight() * m_pBroadCastConfig->fPosYRate));
			m_pThis->SetAlpha( (RwUInt8)((1.0f-m_conAccel.GetCurrentProgress()) * 255.f ) );
			m_surIllust.SetAlpha( (RwUInt8)((1.0f-m_conAccel.GetCurrentProgress()) * 255.f ) );

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	}	
}

/**
* \brief End of directing
*/
VOID CBroadCastGui::UpdateDirectionOutEnd( RwReal fElapsed )
{
	// If there are any remaining units among the current units, a new unit is taken out.
	if( !m_deqUnit.empty() )
	{
		deqBroadUnit::iterator it = m_deqUnit.begin();
		SetCurUnitData( (*it).m_byMsgType, (*it).m_vecMsgDataList );
		SetCurMsgData( (*it) );
		m_deqUnit.pop_front();

		GetDialogManager()->CloseDialog( DIALOG_BROADCAST );
		SetState( STATE_DIRECTION_IN_START );
	}
	else
	{
		GetDialogManager()->CloseDialog( DIALOG_BROADCAST );
		SetState( STATE_CLOSE );
	}
}

/**
* \brief Ending
*/
VOID CBroadCastGui::UpdateClose( RwReal fElapsed )
{
	// Current data is invalid & play stops
	m_bCurUnit = FALSE;
	m_pStbName->Clear();
	
	Play( FALSE );
}

RwInt32 CBroadCastGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	return TRUE;
}

VOID CBroadCastGui::SetState( RwUInt8 byState )
{
	// Debug
	/*WCHAR* szBroadCast[] = { L"STATE_DIRECTION_IN_START", L"STATE_DIRECTION_IN", L"STATE_DIRECTION_IN_END",
		L"STATE_OPEN", L"STATE_DIRECTION_OUT_START", L"STATE_DIRECTION_OUT", L"STATE_DIRECTION_OUT_END",
		L"STATE_CLOSE" };

	WCHAR awcBuffer[256];
	swprintf_s( awcBuffer, 256, L"BroadCast State Change : %s",  szBroadCast[byState] ); 
	CNtlSLEventGenerator::SysMsg( awcBuffer, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );*/

	m_byState = byState;
}

/**
* \brief Sets the currently output unit.
*/
VOID CBroadCastGui::SetCurUnitData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecList )
{
	m_CurUnit.SetData( byMsgType, vecList );
	m_bCurUnit = TRUE;

	// Apply dialog priority
	
	RwUInt16 wPriority = dDIALOGPRIORITY_BROADCAST_TS;
	switch( byMsgType )
	{
	case eBROAD_MSG_TYPE_TS:
		wPriority = dDIALOGPRIORITY_BROADCAST_TS;
		break;
	case eBROAD_MSG_TYPE_EMERGENCY:
		wPriority = dDIALOGPRIORITY_BROADCAST_EMERGENCY;
		break;
	case eBROAD_MSG_TYPE_MINI_NARRATION:
		wPriority = dDIALOGPRIORITY_BROADCAST_MININARRATION;
		break;
	}
	
	m_pThis->SetPriority( wPriority );
}


/**
* \brief Take out the data of the first vector of the unit and save it in the current data.
*/
VOID CBroadCastGui::SetCurMsgData( CBroadCastUnit& unit )
{
	vecdef_BroadMsgDataList::iterator it = unit.m_vecMsgDataList.begin();

	sBROAD_MSG_DATA* pData = (sBROAD_MSG_DATA*)&(*it);

	m_sCurData.eUIDirType				= pData->eUIDirType;
	m_sCurData.eUIShowHideType			= pData->eUIShowHideType;
	m_sCurData.eUIBalloonShapeType		= pData->eUIBalloonShapeType;
	m_sCurData.eUISpeechDirType			= pData->eUISpeechDirType;
	m_sCurData.eOwnerCondition			= pData->eOwnerCondition;
	m_sCurData.eOwnerType				= pData->eOwnerType;
	m_sCurData.uiOwnerTblIdx			= pData->uiOwnerTblIdx;
	m_sCurData.wstrSpeech				= pData->wstrSpeech;
	m_sCurData.fDisplayTime				= pData->fDisplayTime;

	m_pStbName->SetText( L"?" );

	// Naming
	if( m_sCurData.eOwnerType == eBROAD_MSG_OWNER_TYPE_NPC )
	{
		sNPC_TBLDAT* pNPCData = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData( m_sCurData.uiOwnerTblIdx ) );	
		if( pNPCData )
		{
			CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
			if( pNPCText )
				m_pStbName->SetText( pNPCText->GetText( pNPCData->Name ).c_str() );
		}
	}
	else if( m_sCurData.eOwnerType == eBROAD_MSG_OWNER_TYPE_MOB )
	{
		sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( API_GetTableContainer()->GetMobTable()->FindData( m_sCurData.uiOwnerTblIdx ) );
		if( pMobData )
		{
			CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
			if( pMobText )
				m_pStbName->SetText( pMobText->GetText( pMobData->Name ).c_str() );
		}
	}
	else
	{
		DBO_FAIL( "CBroadCastGui::SetCurMsgData - Invalid Owner type" );
		return;
	}
}

/**
* \brief Html String Enter . (size calculation)
*/
VOID CBroadCastGui::SetHtmlString( const WCHAR* pString, RwInt32 nSize )
{
	std::wstring wstrOriginText = gui::GetHtmlFromMemoryString( pString, nSize );

	// Large
	if( wstrOriginText.length() > dBROAD_HTML_TEXT_STANDARD )
	{
		m_pHtmlText->SetPosition( dBROAD_HTML_TEXT_LARGE_POSITION_X, dBROAD_HTML_TEXT_POSITION_Y );
		m_pHtmlText->SetSize( dBROAD_HTML_TEXT_LARGE_WIDTH, dBROAD_HTML_TEXT_HEIGHT );
		m_bSmallText = FALSE;
	}
	// Small
	else
	{
		m_pHtmlText->SetPosition( dBROAD_HTML_TEXT_SMALL_POSITION_X, dBROAD_HTML_TEXT_POSITION_Y );
		m_pHtmlText->SetSize( dBROAD_HTML_TEXT_SMALL_WIDTH, dBROAD_HTML_TEXT_HEIGHT );
		m_bSmallText = TRUE;
	}

	m_pHtmlText->SetHtmlFromMemory( pString , nSize);
}

/**
* \brief When storing the current unit, the updated time must be saved.
*/
VOID CBroadCastGui::SaveCurUnitData()
{
	//// debugging
	//CNtlSLEventGenerator::SysMsg( L"SaveCurUnitData", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );

	m_deqUnit.push_front( m_CurUnit );
}

/**
* \brief Deletes the message stored in Deq.
*/
VOID CBroadCastGui::DeleteDeqUnit( RwInt8 byMsgType )
{
	// If they are different, look for the same type among the stored units and delete them.
	deqBroadUnit::iterator it = m_deqUnit.begin();
	while( it != m_deqUnit.end() )
	{
		if( (*it).m_byMsgType == byMsgType )
		{
			it = m_deqUnit.erase( it );
			continue;
		}

		it++;
	}
}

/**
* \brief Deletes all saved Units.
*/
VOID CBroadCastGui::DeleteDeqUnitAll()
{
	// 
	deqBroadUnit::iterator it = m_deqUnit.begin();
	while( it != m_deqUnit.end() )
	{
		it = m_deqUnit.erase( it );
	}
}

VOID CBroadCastGui::ShowBalloon( RwUInt8 byBalloonShape, RwBool bSmall /* = FALSE */, RwBool bShow /* = TRUE */)
{
	if( bSmall )
	{
		m_paFlaBalloonSmall[byBalloonShape]->Show(B2b(bShow) );
		m_paFlaBalloonSmall[byBalloonShape]->PlayMovie( bShow );
	}
	else
	{
		m_paFlaBalloon[byBalloonShape]->Show( B2b(bShow) );
		m_paFlaBalloon[byBalloonShape]->PlayMovie( bShow );
	}
}

VOID CBroadCastGui::HideAllBalloon()
{
	for( RwInt32 i =0; i< dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		m_paFlaBalloon[i]->PlayMovie( FALSE );
		m_paFlaBalloon[i]->Show( false );

		m_paFlaBalloonSmall[i]->PlayMovie( FALSE );
		m_paFlaBalloonSmall[i]->Show( false );
	}
}

/**
* Set \brief Illust as the File name.
*/
VOID CBroadCastGui::SetIllust( const RwChar* pIllustName )
{
	std::string strFilename = "illust\\";
	strFilename += pIllustName;

	if( !m_surIllust.SetTexture( strFilename.c_str() ) )
	{
		char acBuf[256];
		sprintf_s( acBuf, 256, "CBroadCastGui::SetIllust - Invalid illust file name - %s", strFilename.c_str() );
		DBO_FAIL( acBuf );
		std::string strDef = "illust\\";
		strDef += dBROAD_BALLOON_BTS_DEFAULT_IMG;
		m_surIllust.SetTexture( strDef.c_str() );
	}

	gui::CTexture* pTex = m_surIllust.GetTexture();
	if( pTex )
		m_surIllust.SetSize( pTex->GetWidth(), pTex->GetHeight() );
}

/**
* \brief The illustration is composed of index and condition.
*/
VOID CBroadCastGui::SetIllust( RwUInt32 uiOwnerTblIdx, RwUInt32 uiOwnerCondition )
{
	// Illust Setting
	// File name Setting : (TBLIDX)_(OwnerCondition)_m.png
	char acBuffer[256];
	sprintf_s( acBuffer, "%u_%d_m.png", m_sCurData.uiOwnerTblIdx, (int)m_sCurData.eOwnerCondition );

	std::string strFilename = "illust\\";
	strFilename += acBuffer;

	if( !m_surIllust.SetTexture( strFilename.c_str() ) )
	{
		char acBuf[256];
		sprintf_s( acBuf, 256, "CBroadCastGui::SetIllust - Invalid illust file name - %s", strFilename.c_str() );
		DBO_FAIL( acBuf );
		std::string strDef = "illust\\";
		strDef += dBROAD_BALLOON_BTS_DEFAULT_IMG;
		m_surIllust.SetTexture( strDef.c_str() );
	}

	gui::CTexture* pTex = m_surIllust.GetTexture();
	if( pTex )
		m_surIllust.SetSize( pTex->GetWidth(), pTex->GetHeight() );
}

VOID CBroadCastGui::SetillustPos( RwInt32 nX, RwInt32 nY )
{
	// The standard for receiving coordinates is the left and top of m_pThis.
	CRectangle rtDialog = m_pThis->GetScreenRect();
	RwInt32 nDlgWidth = rtDialog.GetWidth();
	RwInt32 nDlgHeight = rtDialog.GetHeight();

	CRectangle rtIllust;
	m_surIllust.GetRect( rtIllust );
	RwInt32 nIllWidth = rtIllust.GetWidth();
	RwInt32 nIllHeight = rtIllust.GetHeight();
	
	// So that it can be positioned in the middle of the current component.
	RwInt32 nXOffset = (nDlgWidth - nIllWidth)/2;
	RwInt32 nYOffset = (nDlgHeight - nIllHeight);
	
	m_surIllust.SetPosition( nX + nXOffset, nY + nYOffset);
}


VOID CBroadCastGui::Play( RwBool bPlay )
{
	m_bPlay = bPlay;
}

RwBool CBroadCastGui::IsPlay()
{
	return m_bPlay;
}

VOID CBroadCastGui::OnPaint()
{
	m_surIllust.Render();
}

VOID CBroadCastGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	m_surIllust.SetRect( m_pThis->GetScreenRect() );
}


