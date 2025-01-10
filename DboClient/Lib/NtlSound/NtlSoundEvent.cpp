#include "precomp_ntlsound.h"
#include "NtlSoundEvent.h"


RWS::CEventId g_EventSoundReleaseSound;				// The sound of one channel is turned off

RWS::CEventId g_EventSoundFinishFade;				// Fade in/out is finished

RWS::CEventId g_EventSoundDSP;						// Sound effect related events

RWS::CEventId g_EventSoundEventMusic;				// channel events

RWS::CEventId g_EventSoundRestTime;					// A break until the next play of repetitive music

RWS::CEventId g_EventSoundKnockdown;				// When you get knocked down

RWS::CEventId g_EventSoundMinMaxRate;				// Change the overall Min and Max values ??of 3D sound.

RWS::CEventId g_EventSoundAddListRangeBGM;			// Add Range BGM to the list

RWS::CEventId g_EventSoundDelListRangeBGM;			// Delete Range BGM from the list

RWS::CEventId g_EventSoundChangeListRangeBGM;		// Change the value of Range BGM

RWS::CEventId g_EventSoundAddListShareBGM;			// Add Shard BGM to the list

RWS::CEventId g_EventSoundDeleteAllListShareBGM;	// Delete all Shard BGM from the list

RWS::CEventId g_EventSoundShareBGMPlayRate;			// Adjusting the probability of playing Shard BGM

RWS::CEventId g_EventSoundReleaseObjectGroup;		// Turn off all sounds in the Object Group