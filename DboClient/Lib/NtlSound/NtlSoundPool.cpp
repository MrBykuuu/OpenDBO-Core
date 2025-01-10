#include "precomp_ntlsound.h"
#include "NtlSoundEvent.h"


RWS::CEventId g_EventSoundReleaseSound;				// The sound of one channel is turned off

RWS::CEventId g_EventSoundFinishFade;				// Fade in/out is finished

RWS::CEventId g_EventSoundDSP;						// Sound effect related events

RWS::CEventId g_EventSoundEventMusic;				// EventMusic Channel Event

RWS::CEventId g_EventSoundRestTime;					// A break until the next play of repetitive music

RWS::CEventId g_EventSoundKnockdown;				// When you get knocked down

RWS::CEventId g_EventSoundMinMaxRate;				// Change the overall Min and Max values ??of 3D sound.