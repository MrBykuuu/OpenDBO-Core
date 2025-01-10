#include "precomp_ntlcore.h"
#include "NtlSysEvent.h"


RWS::CMsg		g_MsgTick;

RWS::CEventId	g_EventUpdateTick;
RWS::CEventId	g_EventSync30UpdateTick;
RWS::CEventId	g_EventSync20UpdateTick;
RWS::CEventId	g_EventSync10UpdateTick;
RWS::CEventId	g_EventRenderTick;

RWS::CEventId	g_EventCreateStage;
RWS::CEventId	g_EventDeleteStage;


RWS::CEventId	g_EventQueueProcessFilter;

// System is in fullscreen mode <-> window mode
RWS::CEventId	g_EventToggleFullScreen;