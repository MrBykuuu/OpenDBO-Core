#include "precomp_dboclient.h"
#include "InterfaceShakeGui.h"

// Core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// Gui
#include "gui_guimanager.h"
#include "mouse.h"

// Simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Dbo
#include "DialogManager.h"
#include "DboGlobal.h"
 
/**
* \brief constructor
*/
CInterfaceShakeGui::CInterfaceShakeGui( const RwChar* pName ) 
: CNtlPLGui(pName)
{
	m_nOldMouseX = 0;
	m_nMouseX = 0;
	m_nTrackValue = 0;
	m_nOffsetX = 0;
	m_nTrackMin = 0;
	m_nTrackMax = 0;
	m_dwLeftKey = 0;
	m_dwRightKey = 0;

	m_bMouseToggle = FALSE;
	m_bKeyToggle = FALSE;
	m_bComplete = FALSE;
	
	m_pPgbTrack = NULL;
	m_slotKeyDown = NULL;

	m_fElapsedTime = 0.0f;
	m_fShakeElapsedTime = 0.0f;
	m_fKeyDownElapsedTime = 0.0f;
}

/**
* \brief destructor
*/
CInterfaceShakeGui::~CInterfaceShakeGui() 
{

}

/**
* \brief Create
*/
RwBool CInterfaceShakeGui::Create() 
{
	NTL_FUNCTION( "CInterfaceShakeGui::Create" );

	if( !CNtlPLGui::Create("", dSHAKE_FILE_SURFACE , dSHAKE_FILE_FRAME ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pPgbTrack = (gui::CProgressBar*)GetComponent( "pgbTrack" );
	m_pPanTrack = (gui::CPanel*)GetComponent( "pnlTrackBack" );
	m_pPgbTrack->GetRange( m_nTrackMin, m_nTrackMax );

	// Priority
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );

	//m_pFlashLeftArrow = (gui::CFlash*)GetComponent( "flaLeftKey" );
	//m_pFlashRightArrow = (gui::CFlash*)GetComponent( "flaRightKey" );
	m_pFlashMouseBack = (gui::CFlash*)GetComponent( "flaMouseBack" );

	//m_pBtnLeftKey = (gui::CButton*)GetComponent( "btnLeftKey" );
	//m_pBtnRightKey = (gui::CButton*)GetComponent( "btnRightKey" );
	m_pStbMouse = (gui::CStaticBox*)GetComponent( "stbMouse" );

	Show( false );

	// Link
	LinkMsg( g_EventKnockDownNfy, 0 );
	LinkMsg( g_EventKnockDownWakeUpNfy, 0 );

	NTL_RETURN( TRUE );
}

/**
* \brief Destroy
*/
VOID CInterfaceShakeGui::Destroy() 
{
	NTL_FUNCTION( "CInterfaceShakeGui::Destroy" );

	// Unlink
	UnLinkMsg( g_EventKnockDownWakeUpNfy );
	UnLinkMsg( g_EventKnockDownNfy );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

/**
* \brief HandleEvents
*/
VOID CInterfaceShakeGui::HandleEvents( RWS::CMsg &msg ) 
{
	NTL_FUNCTION("CInterfaceShakeGui::HandleEvents");
	
	// Notify that there has been a knockdown
	if( msg.Id == g_EventKnockDownNfy )
	{
		// Launches the interface.
		GetDialogManager()->OpenDialog( DIALOG_INTERFACE_SHAKE );
	}
	// Notify that knockdown has been released (received and processed only when the interface UI is open)
	else if ( msg.Id == g_EventKnockDownWakeUpNfy 
		&& GetDialogManager()->IsOpenDialog( DIALOG_INTERFACE_SHAKE ) )
	{
		// Close the interface.
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}

	NTL_RETURNVOID();
}

/**
* \brief SwitchDialog
*Executed when OpenDialog or CloseDialog is performed in DialogManager.
* \param bOpen Open = TRUE /Close = FALSE
* \return Success or not
*/
RwInt32 CInterfaceShakeGui::SwitchDialog( bool bOpen ) 
{
	if( bOpen == TRUE )
		ShowInterface();
	else if( bOpen == FALSE )
		CloseInterface();	

	return 1;
}

/**
* \brief Update
* \param fElapsed 경과시간
*/
VOID CInterfaceShakeGui::Update( RwReal fElapsed ) 
{
	// time increase
	m_fElapsedTime += fElapsed;
	m_fShakeElapsedTime += fElapsed;
	m_fKeyDownElapsedTime += fElapsed;
	m_fElapsedShakeTime += fElapsed;

	// flash update
	m_pFlashMouseBack->Update( fElapsed );

	// Update mouse position
	m_nMouseX = CMouse::GetX();					// current mouse
	m_nOffsetX = (m_nMouseX - m_nOldMouseX);		// Get increment from previous mouse
	m_nOldMouseX = m_nMouseX;					// The previous mouse becomes the current mouse.

	// True right, False left
	if( m_bMouseToggle )
	{
		if( m_nOffsetX > dSHAKE_MOVE_DISTANCE )
		{
			ProgressUpdateFromMouse();
			m_bMouseToggle = FALSE;
			m_pStbMouse->SetPosition( dSHAKE_MOUSE_LEFT_X, dSHAKE_MOUSE_Y );
		}
	}
	else
	{
		if( m_nOffsetX < -dSHAKE_MOVE_DISTANCE )
		{
			ProgressUpdateFromMouse();
			m_bMouseToggle = TRUE;
			m_pStbMouse->SetPosition( dSHAKE_MOUSE_RIGHT_X, dSHAKE_MOUSE_Y );
		}
	}

	// ProgressBar Set
	m_pPgbTrack->SetPos( m_nTrackValue );

	// Todo : Time Over
	if( m_fElapsedTime > dSHAKE_MAX_WAKEUP )
	{
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}

	// Success when Max of ProgressBar is reached
	if( m_nTrackValue >= m_nTrackMax && m_bComplete == FALSE )
		m_bComplete = TRUE;

	// If successful, the packet should be sent and the dialog should be closed.
	if( m_bComplete )
	{
		// Even if you succeed, if the minimum time is not reached, wait.
		if( m_fElapsedTime < dSHAKE_MIN_WAKEUP )
			return;
		
		// KnockDown's Recovery Packet
		GetDboGlobal()->GetGamePacketGenerator()->SendCharKnockDownReleaseNfy();
		
		// Closes the dialog.
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}
}

/**
* \brief ProgressBar increases due to mouse
*/
VOID CInterfaceShakeGui::ProgressUpdateFromMouse() 
{
	// gain
	if( m_fShakeElapsedTime < dSHAKE_MOUSE_FIRSTTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_FIRST;
	}
	else if( m_fShakeElapsedTime < dSHAKE_MOUSE_SECONDTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_SECOND;
	}
	else
		m_nTrackValue += dSHAKE_ADVANTAGE_THIRD;

	// Mouse shaking elapsed time reset & mouse offset reset
	m_fShakeElapsedTime = 0.0f;
	m_nOffsetX = 0;

	// limits
	if( m_nTrackValue > m_nTrackMax )
		m_nTrackValue = m_nTrackMax;
}

/**
* \brief ProgressBar Update FromKey board
*/
VOID CInterfaceShakeGui::ProgressUpdateFromKeyboard() 
{
	// gain
	if( m_fKeyDownElapsedTime < dSHAKE_KEYBOARD_FIRSTTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_FIRST;
	}
	else if ( m_fKeyDownElapsedTime < dSHAKE_KEYBOARD_SECONDTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_SECOND;
	}
	else
		m_nTrackValue += dSHAKE_ADVANTAGE_THIRD;

	// KeyDown Reset elapsed time
	m_fKeyDownElapsedTime = 0.0f;

	// Track value limits
	if( m_nTrackValue > m_nTrackMax )
		m_nTrackValue = m_nTrackMax;
}

/**
* \brief Open the UI and initialize the necessary information.
*/
VOID CInterfaceShakeGui::ShowInterface( VOID ) 
{
	m_nMouseX = CMouse::GetX();
	m_nOldMouseX = m_nMouseX ;
	m_nTrackValue = 0;
	m_nOffsetX = 0;
	m_bMouseToggle = FALSE;
	m_bKeyToggle = FALSE;
	m_bComplete = FALSE;
	m_fElapsedTime = 0.0f;
	m_fShakeElapsedTime = 0.0f;
	m_fKeyDownElapsedTime = 0.0f;
	m_dwLeftKey = GetInputActionMap()->FindKey( ACTION_AVATAR_LEFTTURN ) & 0xFF;
	m_dwRightKey = GetInputActionMap()->FindKey( ACTION_AVATAR_RIGHTTURN ) & 0xFF;
	
	GetNtlGuiManager()->AddUpdateFunc( this );

	m_pFlashMouseBack->RestartMovie();

	Show( TRUE );
}

/**
* \brief Closes the UI and performs tasks that need to be released.
*/
VOID CInterfaceShakeGui::CloseInterface( VOID ) 
{
	m_slotKeyDown = 0;
	m_slotKeyUp = 0;
	
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	Show( FALSE );
}

/**
* \brief OnKeyDown
*Function connected to SigCaptureKeyDown of the dialog manager as a callback
*/
VOID CInterfaceShakeGui::OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key ) 
{
	if( key.m_dwVKey == m_dwLeftKey )
	{
		m_pBtnLeftKey->Enable( FALSE );
		if( m_bKeyToggle )
		{
			ProgressUpdateFromKeyboard();
			m_bKeyToggle = FALSE;
		}
	}
	else if( key.m_dwVKey == m_dwRightKey )
	{
		m_pBtnRightKey->Enable( FALSE );
		if( !m_bKeyToggle )
		{
			ProgressUpdateFromKeyboard();
			m_bKeyToggle = TRUE;
		}
	}
}

/**
* \brief OnKeyUp
*Function connected to SigCaptureKeyUp of the dialog manager as a callback
*/
VOID CInterfaceShakeGui::OnKeyUp( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key ) 
{
	if( key.m_dwVKey == m_dwLeftKey )
		m_pBtnLeftKey->Enable( TRUE );
	else if( key.m_dwVKey == m_dwRightKey )
		m_pBtnRightKey->Enable( TRUE );
}

