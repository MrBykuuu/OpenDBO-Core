#include "precomp_ntlcore.h"
#include "NtlCoreApi.h"

#include "NtlDebug.h"
#include "NtlCoreLogic.h"
#include "NtlEventQueue.h"
#include "NtlSysEvent.h"

CNtlCoreLogic		*g_pCoreLogic	= NULL;

RwBool API_CoreInit(void)
{
	NTL_FUNCTION("API_CoreInit");

	 // event queue create
	if(!CNtlEventQueue::GetInstance()->Create())
	{
		NTL_RETURN(FALSE);
	}

	g_pCoreLogic = NTL_NEW CNtlCoreLogic;
	if(!g_pCoreLogic->Create())
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

void API_CoreTerminate(void)
{
	if(g_pCoreLogic)
	{
		g_pCoreLogic->Destroy();
		NTL_DELETE(g_pCoreLogic);
	}

	// event queue destory
	CNtlEventQueue::GetInstance()->Destroy();
}


void API_CoreUpdate(RwReal fElapsed)
{
	// event queue process filter
	RWS::CMsg msg;
	msg.Id = g_EventQueueProcessFilter;
	msg.pData = NULL;
	SendMsg(msg);

	// queue process
	CNtlEventQueue::GetInstance()->Process(fElapsed); 
	g_pCoreLogic->Update(fElapsed);
}

/**
* \ingroup NtlCore
*An event is sent when the system's Window Mode is toggled to Window <-> FullScreen.
*When the screenmode changes, work on layers higher than the core that requires work is processed by receiving g_EventToggleFullScreen.
*/
void API_CoreToggleFullScreenNotify(bool bFullScreen)
{
	RWS::CMsg msg;

	msg.Id = g_EventToggleFullScreen;
	msg.pData = reinterpret_cast<void*>(&bFullScreen);
	SendMsg(msg);
}