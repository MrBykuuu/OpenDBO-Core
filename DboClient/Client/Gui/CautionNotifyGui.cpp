#include "precomp_dboclient.h"
#include "CautionNotifyGui.h"

// Sound
#include "GUISoundDefine.h"

// Shared
#include "NtlBattle.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"

// Dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "DialogPriority.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNotifyTimer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CNotifyTimer::CNotifyTimer()
: m_pCallTimer(0)
, m_fCurrentTime(0.0f)
, m_fIntervalTime(dDEFAULT_NOTIFY_TIMER_INVERVAL_TIME)
{

}

/**
* \brief Constructions
*/
CNotifyTimer::CNotifyTimer(RwReal fIntervalTime) 
: m_pCallTimer(0)
, m_fCurrentTime(0.0f)
{
	m_fIntervalTime = fIntervalTime;
}

/**
* \brief Destruction
*/
CNotifyTimer::~CNotifyTimer() 
{
	UnLinkIntervalTimer();
}

/**
* \brief Sets the interval at which the timer will be triggered.
* \param fIntervalTIme (RwReal) Interval at which the timer will fire.
*/
void CNotifyTimer::SetIntervalTime( RwReal fIntervalTime ) 
{
	m_fIntervalTime = fIntervalTime;
}

/**
* \brief Update
* \param fElapsed (RwReal) Time elapsed from previous update.
*/
void CNotifyTimer::Update( RwReal fElapsed ) 
{
	m_fCurrentTime += fElapsed;

	if( m_fCurrentTime > m_fIntervalTime )
	{
		m_fCurrentTime = 0.0f;

		if( m_pCallTimer )
			m_pCallTimer->Call( m_fIntervalTime );
	}
}


/**
* \brief If a callback is registered in CNtlCallbackParam1float, release it.
*/
void CNotifyTimer::UnLinkIntervalTimer( void ) 
{
	if( m_pCallTimer )
		NTL_DELETE( m_pCallTimer );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCautionNotifyContinuance
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CCautionNotifyContinuance::CCautionNotifyContinuance()
: m_pNotifyGui(NULL)
, m_strStringID("")
, m_fRemainTime(0.0f)
{

}

/**
* \brief Constructions
*/
CCautionNotifyContinuance::CCautionNotifyContinuance( CCautionNotifyGui* pNotify, std::string& uiStringID, RwReal fLimitTime, RwReal fIntervalTime ) 
{
	m_pNotifyGui = pNotify;
	m_strStringID = uiStringID;
	m_fRemainTime = fLimitTime;
	
	m_Timer.SetIntervalTime( fIntervalTime );
	m_Timer.LinkIntervalTimer( this, &CCautionNotifyContinuance::OnIntervalTimer );
}

/**
* \brief Destruction
*/
CCautionNotifyContinuance::~CCautionNotifyContinuance() 
{
	m_Timer.UnLinkIntervalTimer();
}

/**
* \brief Update
* \param fElapsed (RwReal) Time elapsed from previous update.
*/
void CCautionNotifyContinuance::Update( RwReal fElapsed ) 
{
	m_Timer.Update( fElapsed );
}

/**
* \brief Finds the remaining time.
* \return remaining time
*/
RwReal CCautionNotifyContinuance::IsRemain() 
{
	return m_fRemainTime;
}

/**
* \brief Callback registered in CNotifyTimer
* \param fElapsed	(RwReal) IntervalTime
*/
int CCautionNotifyContinuance::OnIntervalTimer( RwReal fIntervalTime ) 
{
	m_fRemainTime -= fIntervalTime;

	WCHAR awcBuffer[256];
	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( m_strStringID ), (RwInt32)m_fRemainTime );
	m_pNotifyGui->ShowCautionNotify( TRUE, m_strStringID, awcBuffer );

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCautionNotifyGui
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CCautionNotifyGui::CCautionNotifyGui() 
{
	m_pStbOut = NULL;

	m_bFinish = TRUE;
	m_fLifeTime = 0.0f;
	m_fCurrentTime = 0.0f;

	m_bFadeOut = FALSE;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";

}

/**
* \brief Constructions
*/
CCautionNotifyGui::CCautionNotifyGui( const RwChar* pName )
: CNtlPLGui(pName)
{
	m_pStbOut = NULL;

	m_bFinish = TRUE;
	m_fLifeTime = 0.0f;
	m_fCurrentTime = 0.0f;

	m_bFadeOut = FALSE;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";
}

/**
* \brief Destruction
*/
CCautionNotifyGui::~CCautionNotifyGui() 
{
	for( CONTINUANCELIST::iterator it = m_listContinuance.begin(); it != m_listContinuance.end(); ++it )
	{
		NTL_DELETE( (*it) );
	}
	m_listContinuance.clear();

}

/**
* \brief Create
* \return Success or not
*/
RwBool CCautionNotifyGui::Create( VOID ) 
{
	NTL_FUNCTION("CCautionNotifyGui::Create");

	// Create Components
	if(!CNtlPLGui::Create("", "", "gui\\CautionNotify.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_NOTIFY );

	m_pStbOut = (gui::CStaticBox*)GetComponent("sttOut");


	Enable(FALSE);

	// Add Update
	GetNtlGuiManager()->AddUpdateFunc(this);

	// Link Message
	LinkMsg( g_EventTimeFixedNotify, 0 );
	LinkMsg( g_EventResize, 0 );
	
	// Register an event.
	RegisterHandler();

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CCautionNotifyGui::Destroy( VOID ) 
{
	NTL_FUNCTION( "CCautionNotifyGui::Destroy");

	UnLinkMsg( g_EventTimeFixedNotify );
	UnLinkMsg( g_EventResize );

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed (RwReal) Time elapsed from previous update.
*/
VOID CCautionNotifyGui::Update( RwReal fElapsed ) 
{
	// If the Continuance list is not empty, it is updated.
	if( !m_listContinuance.empty() )
	{
		CONTINUANCELIST::iterator it = m_listContinuance.begin();

		while( it != m_listContinuance.end() )
		{
			// Update and delete if RemainTime is lower than 1.0 seconds. (Callback has already been called before that)
			(*it)->Update( fElapsed );
			if( (*it)->IsRemain() <= 1.0f )
			{
				// delete
				CCautionNotifyContinuance *pContinuance = (*it);
				it = m_listContinuance.erase(it);
				NTL_DELETE( pContinuance );

				continue;
			}

			it++;
		}	
	}

	if( m_bFinish )
		return;

	m_fCurrentTime += fElapsed;
	
	// If the current time has passed longer than the duration, it fades out.
	if( m_fCurrentTime > m_fLifeTime )
	{
		m_bFadeOut = TRUE;
	}

	// It is FadeOut, and is erased when FadeOut is completed.
	if(m_bFadeOut)
	{
		m_fFadeOutTime += fElapsed;
		if(m_fFadeOutTime >= CAUTION_NOTIFY_FADEOUT_TIME)
		{
			m_bFinish = TRUE;
			m_fCurrentTime = 0.0f;
			Show(false);

			return;
		}

		RwUInt8 byAlpha = (RwUInt8)((CAUTION_NOTIFY_FADEOUT_TIME - m_fFadeOutTime)*255.0f/CAUTION_NOTIFY_FADEOUT_TIME);
		m_pStbOut->SetAlpha(byAlpha);
	}
}

/**
* \brief HandleEvents
* \param msg	(RWS::CMsg*) Event message structure
*/
VOID CCautionNotifyGui::HandleEvents( RWS::CMsg& msg ) 
{
	if( msg.Id == g_EventTimeFixedNotify )
	{
		SDboEventTimeFixedNotify* pNotify = reinterpret_cast<SDboEventTimeFixedNotify*>( msg.pData );

		// If registered in the handler, call the related function and return.
		HANDLEMAP::iterator it = m_mapNotifyHandler.find( pNotify->strStringID );
		if( it != m_mapNotifyHandler.end() )
		{
			(this->*((*it).second))(pNotify->bActive, pNotify->strStringID);
			return;
		}

		ShowCautionNotify( pNotify->bActive, pNotify->strStringID, pNotify->pString );

		// Currently, when the same message is duplicated, it is output every time a message comes in. If duplicates are to be made, it will be processed within ShowCautionNotify.
		Logic_PlayGUISound(GSD_SYSTEM_WARNING_NOTIFY);		
	}
	// Event fired when resolution changes
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize *pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );

		// resize
		PositionAlign( pData->iWidht, pData->iHeight );
	}
}

/**
* \brief Aligns the position of the text to be output.
*/
VOID CCautionNotifyGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight ) 
{
	m_pThis->SetPosition( (RwInt32)( (nScreenWidth - m_pThis->GetPosition().GetWidth() ) /2 ),
		(RwInt32)( (nScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fCautionNotiry_YPosRate ) ) );
}

/**
* \brief Sets and initializes the text in the current Caution.
*/
VOID CCautionNotifyGui::SetCurrentCautionNotify(std::string& strStringID, const WCHAR* pwcString )
{
	m_fCurrentTime = 0.0f;
	m_fFadeOutTime = 0.0f;
	m_fLifeTime = CAUTION_NOTIFY_LIFE_TIME;
	
	m_strStringID = strStringID;
	m_pStbOut->SetText( pwcString );
	m_pStbOut->SetAlpha(CAUTION_NOTIFY_DEFAULT_ALPHA);
	
	PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	
	m_bFinish = FALSE;
	m_bFadeOut = FALSE;

	Show(true);
}

/**
* \brief Function that outputs a warning message (if it is already displayed, the existing one is sent to SideIcon.)
*/
VOID CCautionNotifyGui::ShowCautionNotify( RwBool bActive, std::string& strStringID, const WCHAR* pwcString )
{
	if( bActive )
	{
		// If there is no CautionNotify already being output, it is output immediately.
		if( m_bFinish )
			SetCurrentCautionNotify( strStringID, pwcString );
		else
		{
			// If there is a CautionNotify being output, send the currently output CautionNotify to the SideIcon.
			// Register a new CautionNotify.
			CDboEventGenerator::CautionSideNotify( TRUE, strStringID, m_pStbOut->GetText().c_str(), m_fLifeTime-m_fCurrentTime);

			SetCurrentCautionNotify( strStringID, pwcString );
		}
	}
	else
	{
		// If the ID currently being printed and the ID sent for deletion are the same, it will be deleted.
		if( m_bFinish )
		{
			if( m_strStringID == strStringID )
				ClearCurrentCautionNotify();
		}
	}
}

/**
* \brief Initializes the current CautionNotify.
*/
VOID CCautionNotifyGui::ClearCurrentCautionNotify() 
{
	m_fCurrentTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";

	m_pStbOut->SetAlpha(CAUTION_NOTIFY_DEFAULT_ALPHA);
	m_pStbOut->Clear();

	m_bFinish = TRUE;

	Show(false);
}

/**
* \brief Free PVP Over-the-counter processing out message handler
* \param bActive (RwBool) Whether to register or delete
* \param uiStringID (RwUInt32) String index
*/
VOID CCautionNotifyGui::FreePvpOutSideHandler( RwBool bActive, std::string strStringID)
{
	// If bActive is FALSE, it is removed.
	if( bActive )
	{
		if( !m_listContinuance.empty() )
			DeleteContinuance( strStringID );
	
		// registration
		AddContinuance( strStringID, dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME, dDEFAULT_NOTIFY_TIMER_INVERVAL_TIME );

		// Print it out once.
		WCHAR awcBuffer[256];
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "Dst freepvp outside stage" ), (RwInt32)dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME );
		ShowCautionNotify( TRUE, strStringID, awcBuffer );
		
	}
	else
	{
		// If there is nothing in the list, it just returns.
		if( !m_listContinuance.empty() )
			DeleteContinuance( strStringID );
	}
}

/**
* \brief Registers things to be registered in the handler.
*/
VOID CCautionNotifyGui::RegisterHandler() 
{
	m_mapNotifyHandler["DST_FREEPVP_OUTSIDE_STAGE"] = &CCautionNotifyGui::FreePvpOutSideHandler;
}

/**
* \brief Register StringID as Continuance.
* \param uiStringID		(RwUInt32) String index
* \param fLimitTime		(RwReal) Timeout
* \param fIntervalTime	(RwReal) Interval
*/
VOID CCautionNotifyGui::AddContinuance(std::string& strStringID, RwReal fLimitTime, RwReal fIntervalTime )
{
	CCautionNotifyContinuance* pContinuance = NTL_NEW CCautionNotifyContinuance( this, strStringID, fLimitTime, fIntervalTime );
	m_listContinuance.push_back( pContinuance );
}

/**
* \brief Deletes continuations with the same StringID registered in the Continuance List.
* \param uiStringID (RwUInt32) String index
*/
VOID CCautionNotifyGui::DeleteContinuance(std::string& strStringID)
{	
	CONTINUANCELIST::iterator it = m_listContinuance.begin();
	while( it != m_listContinuance.end() )
	{
		if( (*it)->GetStringID() == strStringID )
		{
			// delete
			CCautionNotifyContinuance *pContinuance = (*it);
			it = m_listContinuance.erase(it);
			NTL_DELETE( pContinuance );

			continue;
		}

		it++;
	}	
}