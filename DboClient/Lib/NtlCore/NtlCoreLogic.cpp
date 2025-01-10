#include "precomp_ntlcore.h"
#include "NtlCoreLogic.h"
#include "NtlDebug.h"

/**
*
* constructor for CNtlLogic
*
*/
CNtlCoreLogic::CNtlCoreLogic() : m_bEnabled(true) ,
							 m_fFreeElapsed(0.0f),	
							 m_fSync30Elapsed(0.0f),	
							 m_fSync20Elapsed(0.0f),	
							 m_fSync10Elapsed(0.0f)
{
}

/**
*
*Register events for various ticks.
*
*/
bool CNtlCoreLogic::Create(void)
{
	NTL_FUNCTION("CNtlCoreLogic::Create");

	RWS::CEventHandler::RegisterMsg (g_EventUpdateTick, "g_EventUpdateTickStr", "float");
	RWS::CEventHandler::RegisterMsg (g_EventQueueProcessFilter, "g_EventQueueProcessFilterStr", "EventQueueProcessFilterStr");
	RWS::CEventHandler::RegisterMsg (g_EventToggleFullScreen, "g_EventToggleFullScreen", "bool");
		
	NTL_RETURN(true);
	
}

/**
*
*Releases registered events for various ticks.
*
*/
void CNtlCoreLogic::Destroy(void)
{
	NTL_FUNCTION("CNtlCoreLogic::Destroy");

    RWS::CEventHandler::UnRegisterMsg ( g_EventUpdateTick );
	RWS::CEventHandler::UnRegisterMsg ( g_EventQueueProcessFilter );
	RWS::CEventHandler::UnRegisterMsg ( g_EventToggleFullScreen );
	
	NTL_RETURNVOID();
}

/**
*
*Send update tick evnet to simulation object.
*Also, synchronized ticks at 30, 20, and 10 frames are calculated and sent.
*
*/
void CNtlCoreLogic::Update(float fElapsed)
{
	if(!m_bEnabled)
		return;

	g_MsgTick.Id = g_EventUpdateTick;
	m_fFreeElapsed = fElapsed;

	g_MsgTick.pData = reinterpret_cast<void*>(&m_fFreeElapsed);
	SendMsg(g_MsgTick);

	// Get input.
	// Update the logic. (including rendering objects)
}