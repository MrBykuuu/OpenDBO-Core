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
#include "NtlSoundDefines.h"

extern RWS::CEventId g_EventSoundReleaseSound;				// The sound of one channel is turned off

extern RWS::CEventId g_EventSoundFinishFade;				// Fade in/out is finished

extern RWS::CEventId g_EventSoundDSP;						// Sound effect related events

extern RWS::CEventId g_EventSoundEventMusic;				// channel events

extern RWS::CEventId g_EventSoundRestTime;					// A break until the next play of repetitive music

extern RWS::CEventId g_EventSoundKnockdown;					// When you get knocked down

extern RWS::CEventId g_EventSoundMinMaxRate;				// Change the overall Min and Max values ??of 3D sound.

extern RWS::CEventId g_EventSoundAddListRangeBGM;			// Add Range BGM to the list

extern RWS::CEventId g_EventSoundDelListRangeBGM;			// Delete Range BGM from the list

extern RWS::CEventId g_EventSoundChangeListRangeBGM;		// Change the value of Range BGM

extern RWS::CEventId g_EventSoundAddListShareBGM;			// Add Shard BGM to the list

extern RWS::CEventId g_EventSoundDeleteAllListShareBGM;		// Delete all Shard BGM from the list

extern RWS::CEventId g_EventSoundShareBGMPlayRate;			// Adjusting the probability of playing Shard BGM

extern RWS::CEventId g_EventSoundReleaseObjectGroup;		// Disable all sounds in the Object Group

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
	EVENT_MUSIC_EVENT_MUSIC_HAD_PLAY,			///< Event music started playing
	EVENT_MUSIC_ALL_EVENT_MUSIC_FINISH,			///< All Event music has stopped playing

	EVENT_MUSIC_LEAVE_MAIN_WORLD_CONCEPT,		///< The main world concept has been changed to another concept.

	EVENT_MUSIC_START_PVP_BGM,					///< PVP BGM starts
	EVENT_MUSIC_END_PVP_BGM,					///< PVP BGM ends

	EVENT_MUSIC_WAIT_PVP_BGM,
	EVENT_MUSIC_END_WAIT,

	EVENT_MUSIC_START_PRIVATE_RANKBATTLE_BGM,	///< Rank Battle solo BGM begins
	EVENT_MUSIC_END_PRIVATE_RANKBATTLE_BGM,		///< End of individual ranked battle

	EVENT_MUSIC_START_PARTY_RANKBATTLE_BGM,		///< Rank Battle Party BGM begins
	EVENT_MUSIC_END_PARTY_RANKBATTLE_BGM,		///< Rank Battle Party Battle End

	EVENT_MUSIC_END_CINEMATIC_BGM,				///< End of cinematic BGM

	EVENT_MUSIC_START_MAIN_THEME,				///< Main theme BGM starts
	EVENT_MUSIC_PROLOG_THMEM_BGM_PLAY,			///< Prolog BGM Play
	EVENT_MUSIC_END_MAIN_THEME,					///< End of main theme
	EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME,		///< Immediately exits without main theme FadeOut	

	EVENT_MUSIC_VOLUME_DOWN_BECAUSE_MOVIE_PLAY,	///< Reduce the Master volume to play the movie
	EVENT_MUSIC_VOLUME_UP_BECAUSE_MOVIE_PLAY,	///< The movie ends and the Master volume is turned up.

	EVENT_MUSIC_FINISH_THEME_BGM,				///< If THEME BGM is playing, quit	

	EVENT_MUSIC_LOOP_THEME_BGM,					///< Loop THEME BGM
	EVENT_MUSIC_NOT_LOOP_THEME_BGM,				///< Play THEME BGM only once

	EVENT_MUSIC_TEMP_FINISH_TMQ,				///< Temporary TMQ ends

	EVENT_MUSIC_TB_DIRECTION_BGM_PLAY,			///< The World's Best Ballroom Production BGM Play
	EVENT_MUSIC_TB_DIRECTION_BGM_STOP,			///< World's Best Ballroom Production BGM Stop

	EVENT_MUSIC_TB_PARTY_BGM_PLAY,				///< World¡¯s Best Ballroom Party Exhibition BGM Play
	EVENT_MUSIC_TB_PARTY_BGM_STOP,				///< World¡¯s Best Ballroom Party Pre-BGM Stop

	EVENT_MUSIC_TB_PRIVATE_BGM_PLAY,			///< World¡¯s Best Martial Arts Solo Exhibition BGM Play
	EVENT_MUSIC_TB_PRIVATE_BGM_STOP,			///< World¡¯s Best Martial Arts Solo Exhibition BGM Stop

	EVENT_MUSIC_RAGNE_STOP,						///< Range BGM Stop
	EVENT_MUSIC_REMOVE_RAGNE_BGM,				///< The Range BGM list was deleted from the BGM group.

	EVENT_MUSIC_START_SCRAMBLE_READY_BGM,		///< Dojo competition
	EVENT_MUSIC_STOP_SCRAMBLE_READY_BGM,
	EVENT_MUSIC_START_SCRAMBLE_START_BGM,
	EVENT_MUSIC_END_SCRAMBLE_START_BGM,
};

struct SNtlEeventMusic
{
	RwUInt8			byType;
};

struct SNtlEventMusicRestTime
{
	RwUInt8			byChannelGroup;
	RwReal			fRestTime;				///< Unit: seconds
	RwUInt8			byBGMType;				///< Valid only for BGM channel groups
};

struct SNtlEventKnockdown
{
	RwBool			bKnockdown;
};

struct SNtlEventMinMaxRate
{
	RwReal			fMinMaxRate;
};

struct SNtlEventAddListRangeBGM
{
	RwUInt32*		phResultHandle;
	const RwChar*	pcFileName;
	RwReal			fRadius;
	RwV3d*			pPosition;
};

struct SNtlEventDelListRangeBGM
{
	RwUInt32		hSound;
};

struct SNtlEventRangeBGMChangeParam
{
	RwUInt32		hSound;
	const RwChar*	pcFileName;
	RwReal			fRadius;
	RwV3d*			pPosition;
};

struct SNtlEventAddListShareBGM
{
	const RwChar*	pcFileName;
};

struct SNtlEventShareBGMPlayRate
{
	RwReal			fPlayRate;
};