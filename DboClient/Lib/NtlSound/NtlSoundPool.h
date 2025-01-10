/*****************************************************************************
 *
 * File			: DboEvent.h
 * Author		: Hong Sungbock
 * Copyright	: (?)NTL
 * Date			: 2007. 5. 29
 * Abstract		: Ntl sound event.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "ceventhandler.h"

extern RWS::CEventId g_EventSoundReleaseSound;				// The sound of one channel is turned off

extern RWS::CEventId g_EventSoundFinishFade;				// Fade in/out is finished

extern RWS::CEventId g_EventSoundDSP;						// Sound effect related events

extern RWS::CEventId g_EventSoundEventMusic;				// EventMusic Channel Event

extern RWS::CEventId g_EventSoundRestTime;					// A break until the next play of repetitive music

extern RWS::CEventId g_EventSoundKnockdown;					// When you get knocked down

extern RWS::CEventId g_EventSoundMinMaxRate;				// Change the overall Min and Max values ??of 3D sound.

//////////////////////////////////////////////////////////////////////////
//	Structure
//////////////////////////////////////////////////////////////////////////

enum eDSPEvent
{
	DSP_ADD_REVERV,							///< Apply reverv effect to specific channel group
	DSP_REMOVE_REVERV,						///< Disables the reverv effect for a specific channel group
	DSP_REMOVE_ALL,							///< Disables all effects for a specific channel group
};

struct SNtlEventDSP
{
	RwUInt8			byType;
};

enum eEventMusic
{
	EVENT_MUSIC_EVENT_MUSIC_HAD_PLAY,		///< Event music started playing
	EVENT_MUSIC_ALL_EVENT_MUSIC_FINISH,		///< All Event music has stopped playing
};

struct SNtlEeventMusic
{
	RwUInt8			byType;
};

struct SNtlEventMusicRestTime
{
	RwUInt8			byChannelGroup;
	RwReal			fRestTime;				///< Unit: seconds
};

struct SNtlEventKnockdown
{
	RwBool			bKnockdown;
};

struct SNtlEventMinMaxRate
{
	RwReal			fMinMaxRate;
};