#include "precomp_ntlframework.h"
#include "NtlLogic.h"
#include "NtlDebug.h"

/**
*
*  constructor for CNtlLogic
*
*/
CNtlLogic::CNtlLogic() : m_bEnabled(false) ,
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
bool CNtlLogic::Create(void)
{
	NTL_FUNCTION("CNtlLogic::Create");

	RWS::CEventHandler::RegisterMsg (g_EventUpdateTick, "g_EventUpdateTickStr", "float");
		
	NTL_RETURN(true);
	
}

/**
*
*  Releases registered events for various ticks.
*
*/
void CNtlLogic::Destroy(void)
{
	NTL_FUNCTION("CNtlLogic::Destroy");

    RWS::CEventHandler::UnRegisterMsg ( g_EventUpdateTick );
	
	NTL_RETURNVOID();
}

/**
*
*  Send update tick evnet to simulation object.
*  Also, synchronized ticks at 30, 20, and 10 frames are calculated and sent.
*
*/
void CNtlLogic::Update(float fElapsed)
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

