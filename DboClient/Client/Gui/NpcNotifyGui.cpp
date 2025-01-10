#include "precomp_dboclient.h"
#include "NpcNotifyGui.h"

//// Core
//#include "NtlDebug.h"
//
//// Presentation
//#include "NtlPLGuiManager.h"
//#include "NtlPLEvent.h"
//
//// Dbo
//#include "DboGlobal.h"
//#include "DboEvent.h"
//#include "GuiFunctor.h"
//
//// Test & Develop
//#include "DumpCommand.h"
//#include "AlarmManager.h"
//
///**
//*\brief constructor
//*/
//CNpcNotifyGui::CNpcNotifyGui( VOID ) 
//{
//	Init();
//}
//
///**
//*\brief constructor
//*/
//CNpcNotifyGui::CNpcNotifyGui( const RwChar* pName ) 
//: CNtlPLGui( pName )
//{
//	Init();
//}
//
///**
//*\brief destructor
//*/
//CNpcNotifyGui::~CNpcNotifyGui( VOID ) 
//{
//
//}
//
///**
//* \brief Create
//*/
//RwBool CNpcNotifyGui::Create( VOID ) 
//{
//	NTL_FUNCTION( "CNpcNotifyGui::Create" );
//
//	if( !CNtlPLGui::Create( "", "gui\\NpcNotify.srf", "gui\\NpcNotify.frm" ) )
//		NTL_RETURN( FALSE );
//
//	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );
//
//	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
//	m_pPanNpc = (gui::CPanel*)GetComponent( "panNpc" );
//	m_pStbText = (gui::CStaticBox*)GetComponent( "stbNotify" );
//	m_pFlaBalloon = (gui::CFlash*)GetComponent( "flaBalloon" );
//
//	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);
//	
//	Show(false);
//
//	LinkMsg( g_EventNotify, 0 );
//
//	NTL_RETURN( TRUE );
//}
//
///**
//* \brief Destroy
//*/
//VOID CNpcNotifyGui::Destroy( VOID ) 
//{
//	NTL_FUNCTION( "CNpcNotifyGui::Destroy" );
//	
//	UnLinkMsg( g_EventNotify );
//
//	CNtlPLGui::DestroyComponents();
//	CNtlPLGui::Destroy();
//
//	NTL_RETURNVOID();
//}
//
///**
//*\brief initialization
//*Returns Npc Notify to its initial state.
//*/
//VOID CNpcNotifyGui::Init( VOID ) 
//{
//	m_fCurrentTime = 0.0f;
//	m_pFlaBalloon = NULL;
//	m_pStbText = NULL;
//	m_pPanNpc = NULL;
//	SetState( NPC_NONE );
//}
//
///**
//*\brief Set various values ??appropriate for the Type.
//*\param eType (eTYPE) NPC_LEFT, NPC_CENTER, NPC_RIGHT
//*\return Success or not
//*/
//RwBool CNpcNotifyGui::SetNpcNotifyReady( eTYPE eType , eSTATE eState ) 
//{
//	//Size of current dialog
//	RwInt32 nThisW = m_pThis->GetWidth();
//	RwInt32 nThisH = m_pThis->GetHeight();
//
//	//size of screen
//	RwInt32 nScreenW = (RwInt32)GetDboGlobal()->GetScreenWidth();
//	RwInt32 nScreenH = (RwInt32)GetDboGlobal()->GetScreenHeight();
//
//	CPos posStart;
//	CPos posEnd;
//
//	//Calculate the coordinates to be slid according to the type
//	switch( eType )
//	{
//	case NPC_LEFT:
//		{
//			if( eState == NPC_SLIDING_IN )
//			{
//				posStart.x = 0 - nThisW;
//				posStart.y = nScreenH - nThisH;
//				posEnd.x = 0;
//				posEnd.y = posStart.y;
//			}
//			else
//			{
//				posStart.x = 0;
//				posStart.y = nScreenH - nThisH;
//				posEnd.x = 0 - nThisW;
//				posEnd.y = posStart.y;
//			}
//		}
//		break;
//	case NPC_CENTER:
//		{
//			if( eState == NPC_SLIDING_IN )
//			{
//				posStart.x = (nScreenW/2) - nThisW;
//				posStart.y = nScreenH;
//				posEnd.x = posStart.x;
//				posEnd.y = nScreenH - nThisH;
//			}
//			else
//			{
//				posStart.x = (nScreenW/2) - nThisW;
//				posStart.y = nScreenH - nThisH;
//				posEnd.x = posStart.x;
//				posEnd.y = nScreenH;
//			}
//		}
//		break;
//	case NPC_RIGHT:
//		{
//			if( eState == NPC_SLIDING_IN )
//			{
//				posStart.x = nScreenW;
//				posStart.y = nScreenH - nThisH;
//				posEnd.x = nScreenW - nThisW;
//				posEnd.y = posStart.y;
//			}
//			else
//			{
//				posStart.x = nScreenW - nThisW;
//				posStart.y = nScreenH - nThisH;
//				posEnd.x = nScreenW;
//				posEnd.y = posStart.y;
//			}
//		}
//		break;
//	}
//
//	SetSlidingPosition( posStart.x, posStart.y, posEnd.x, posEnd.y );
//	SetCurrentPosition( posStart.x, posStart.y );
//
//	return TRUE;
//}
//
///**
//*\brief Enter the coordinates for sliding.
//*\param nStartX Start
//*\param nStartY Start Y
//*\param nEndX X to be slid
//*\param nEndY Y to be slid
//*/
//VOID CNpcNotifyGui::SetSlidingPosition( RwInt32 nStartX, RwInt32 nStartY, RwInt32 nEndX, RwInt32 nEndY ) 
//{
//	m_posStart.x = nStartX;
//	m_posStart.y = nStartY;
//	m_posEnd.x = nEndX;
//	m_posEnd.y = nEndY;
//
//	DUMP_DEBUG_MSG( MakeStr( "SetSliding : StartX[%d], StartY[%d], EndX[%d], EndY[%d]", m_posStart.x, m_posStart.y, m_posEnd.x, m_posEnd.y ) );
//}
//
///**
//*\brief Sets the current position.
//*\param nCurrentX Current
//*\param nCurrentY Current Y
//*/
//VOID CNpcNotifyGui::SetCurrentPosition( RwInt32 nCurrentX, RwInt32 nCurrentY ) 
//{
//	m_posCurrent.x = nCurrentX;
//	m_posCurrent.y = nCurrentY;
//
//	DUMP_DEBUG_MSG( MakeStr( "SetCurrent : CurrentX[%d], CurrentY[%d]", m_posCurrent.x, m_posCurrent.y ) );
//}
//
///**
//* \brief HandleEvents
//*/
//VOID CNpcNotifyGui::HandleEvents( RWS::CMsg& msg ) 
//{
//	if( msg.Id == g_EventNotify )
//	{
//		SDboEventNotify* pNotify = reinterpret_cast<SDboEventNotify*>( msg.pData );
//
//		if( pNotify->eType == SDboEventNotify::NPC )
//		{
//			if( m_eState == NPC_SHOW )
//			{
//				m_pStbText->SetText( pNotify->wchMessage );
//				SetState( NPC_SHOW );
//			}
//			else
//			{
//				m_pStbText->SetText( pNotify->wchMessage );
//				SetState( NPC_SLIDING_IN );
//			}
//		}
//	}
//}
//
///**
//*\brief Update
//*\param fElapsed (RwReal) Time elapsed from previous update.
//*/
//VOID CNpcNotifyGui::Update( RwReal fElapsed ) 
//{
//	NpcUpdateProc( fElapsed );
//}
//
///**
//*\brief Specifies the state and inserts an appropriate function pointer.
//*\param eState (ENUM of the state)
//*/
//VOID CNpcNotifyGui::SetState( eSTATE eState ) 
//{
//	if( m_eState == NPC_SLIDING_IN )
//	{
//		//m_fCurrentTime = 0.0f;
//	}
//	else if( m_eState == NPC_SLIDING_OUT )
//	{
//		m_fCurrentTime = dNPCNOTIFY_SLIDING_IN_TIME - m_fCurrentTime;
//	}
//	else
//		m_fCurrentTime = 0.0f;
//
//	m_eState = eState;
//
//	// Test
//	WCHAR awcBuffer[256];
//
//	switch( eState )
//	{
//	case NPC_NONE:
//		StateUpdate = &CNpcNotifyGui::NpcUpdate_None;
//		Show(false);
//		
//		GetNtlGuiManager()->RemoveUpdateFunc( this );
//
//		swprintf_s( awcBuffer, 256, L"SetState : %s", L"NONE" );
//		break;
//	case NPC_SLIDING_IN:
//		StateUpdate = &CNpcNotifyGui::NpcUpdate_SlidingIn;
//		SetNpcNotifyReady( NPC_LEFT, NPC_SLIDING_IN );
//		
//		Show(true);
//		m_pStbText->Show(false);
//		m_pFlaBalloon->Show(false);
//		m_pPanNpc->Show(true);
//		
//		m_pFlaBalloon->PlayMovie( TRUE );
//		m_pFlaBalloon->AutoRewind( TRUE );
//
//		GetNtlGuiManager()->AddUpdateFunc( this );
//
//		swprintf_s( awcBuffer, 256, L"SetState : %s", L"SLIDING_IN" );
//		break;
//	case NPC_SHOW:
//		StateUpdate = &CNpcNotifyGui::NpcUpdate_Show;
//		m_pFlaBalloon->Show(true);
//		m_pStbText->Show(true);
//
//		swprintf_s( awcBuffer, 256, L"SetState : %s", L"SHOW" );
//		break;
//	case NPC_SLIDING_OUT:
//		StateUpdate = &CNpcNotifyGui::NpcUpdate_SlidingOut;
//		SetNpcNotifyReady( NPC_LEFT, NPC_SLIDING_OUT );
//
//		m_pFlaBalloon->Show(false);
//		m_pStbText->Show(false);
//
//		swprintf_s( awcBuffer, 256, L"SetState : %s", L"SLIDING_OUT" );
//		break;
//	}
//
//	GetAlarmManager()->AlarmMessage( awcBuffer );
//}
//
///**
//*\brief OnMove
//*\param nOldX (RwInt32) X Position of previous position
//*\param nOldY (RwInt32) Y Position of the previous position
//*/
//VOID CNpcNotifyGui::OnMove( RwInt32 nOldX, RwInt32 nOldY ) 
//{
//
//}
//
///**
//*\brief Npc Notify Update
//*In Npc Notify, call the function pointer that matches the state and type.
//*\param fElapsed (RwReal)Time elapsed from previous frame
//*/
//VOID CNpcNotifyGui::NpcUpdateProc( RwReal fElapsed ) 
//{
//	m_fCurrentTime += fElapsed;
//
//	(this->*StateUpdate)( fElapsed );
//}
//
//VOID CNpcNotifyGui::NpcUpdate_None(RwReal fElapsed) 
//{
//	
//}
//
//VOID CNpcNotifyGui::NpcUpdate_SlidingIn(RwReal fElapsed) 
//{
//	if( m_fCurrentTime > dNPCNOTIFY_SLIDING_IN_TIME )
//	{
//		SetState( NPC_SHOW );
//	}
//	else
//	{
//		ApplyCurrentPosition( dNPCNOTIFY_SLIDING_IN_TIME );
//	}
//}
//
//VOID CNpcNotifyGui::NpcUpdate_Show(RwReal fElapsed) 
//{
//	if( m_fCurrentTime > dNPCNOTIFY_SHOW_TIME )
//	{
//		SetState( NPC_SLIDING_OUT );
//	}
//	else
//	{
//		m_pFlaBalloon->Update( fElapsed );
//	}
//}
//
//VOID CNpcNotifyGui::NpcUpdate_SlidingOut(RwReal fElapsed) 
//{
//	if( m_fCurrentTime > dNPCNOTIFY_SLIDING_OUT_TIME )
//	{
//		SetState( NPC_NONE );
//	}
//	else
//	{
//		ApplyCurrentPosition( dNPCNOTIFY_SLIDING_OUT_TIME );
//	}
//}
//
//VOID CNpcNotifyGui::ApplyCurrentPosition(RwReal fTotalTime) 
//{
//	SetCurrentPosition( (RwInt32)GetGuiFuntor()->GetResultLineInterpolation( m_fCurrentTime, fTotalTime, (RwReal)m_posStart.x, (RwReal)m_posEnd.x ),
//						(RwInt32)GetGuiFuntor()->GetResultLineInterpolation( m_fCurrentTime, fTotalTime, (RwReal)m_posStart.y, (RwReal)m_posEnd.y ) );
//
//	m_pThis->SetPosition( m_posCurrent.x, m_posCurrent.y );
//}