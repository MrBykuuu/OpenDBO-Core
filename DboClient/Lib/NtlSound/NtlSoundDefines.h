#pragma once

#include <string>

// Sound
#include "fmod/fmod.hpp"
#include "fmod/fmod_common.h"

//#define SOUND_DEBUG_LOG						///< Leave sound-related logs for debugging

class CNtlSoundDSP;


// distance //////////////////////////////////////////////////////////////////////////////
#define MIN_DISTANCE_BETWEEN_MINMAX		1.f	///< The basic value is that the maximum distance for sound performance must be greater than the minimum distance.


// hour //////////////////////////////////////////////////////////////////////////////
#define SOUND_FADEINOUT_INTERVAL_TIME	100		///< Time for volume change during Fade In/Out (unit: millisecond)
#define DELAY_EFFECT_SOUND_TIME			500		///< Play delay time between effect sounds of the same name

#define dBGM_FADE_OUT_TIME				2000	///< fade out time when the background sound disappears or is replaced


// Sound handle /////////////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_HANDLE				0xffffffff


// Number of channels ////////////////////////////////////////////////////////////////////////////////
#define MAX_FMOD_CHANNELS				4093	///< Maximum channel (hardware + software) supported by FMOD
#define MAX_DBO_CHANNELS				512		///< Number of channels used in Dbo
#define MAX_EFFECT_CHANNELS				40		///< The number of all channels of the effect that can be played at maximum


// Numerical range ///////////////////////////////////////////////////////////////////////////////
#define dNTLSOUND_VOLUME_DEFAULT				1.0f
#define dNTLSOUND_VOLUME_BACKGROUND_MAX			0.5f
#define dNTLSOUND_VOLUME_EVENT_MAX				0.7f

#define dNTLSOUND_PITCH_DEFAULT					1.0f
#define dNTLSOUND_PITCH_MINIMUM					0.0f
#define dNTLSOUND_PITCH_MAXIMUM					10.0f

// Etc
#define NTL_SOUND_MEMORY_POOL			10
#define MAX_SOUND_DUPLICATION			3		///< The maximum number of times a sound of the same name can be played simultaneously

#define d2D_SOUND_MODE		(FMOD_2D)
#define d3D_SOUND_MODE		(FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_3D_WORLDRELATIVE)



typedef unsigned int			SOUND_HANDLE;


enum eSoundResourceType
{
	SRT_CHANNEL,
	SRT_CHANNEL_GROUP,
};

enum eFMODPlayType
{
	FMODPLAYTYPE_3DMODE,						///< 3D default sound mode
	FMODPLAYTYPE_3D_TOOLMODE,					///< 3D Tool sound mode

	FMODPLAYTYPE_NUM
};

enum eSoundPlayState
{
	SPS_NONE,									///< The performance has ended or there is no sound information.
	SPS_PLAYING,								///< Playing
	SPS_PLAY_FADE_IN,							///< Fade in in progress
	SPS_PLAY_FADE_OUT,							///< Fade Out
	SPS_SLEEP,									///< Playing has stopped and is in standby mode.
};

enum eChannelGroupType
{
	CHANNEL_GROUP_FIRST = 0,

	CHANNEL_GROUP_UI_SOUND = CHANNEL_GROUP_FIRST,///< UI sound effects
	CHANNEL_GROUP_JINGLE_MUSIC,					///< (When an event occurs) Short music for quest acquisition, quest completion, etc.
	CHANNEL_GROUP_FLASH_MUSIC,					///< Flash background sound
	CHANNEL_GROUP_BGM,							///< DBO main background music
	CHANNEL_GROUP_AVATAR_VOICE_SOUND,			///< Avatar's voice
	CHANNEL_GROUP_AVATAR_EFFECT_SOUND,			///< Avatar effect sound
	CHANNEL_GROUP_VOICE_SOUND,					///< Actor's voice
	CHANNEL_GROUP_EFFECT_SOUND,					///< Effect sound
	CHANNEL_GROUP_OBJECT_MUSIC,					///< Object music, car sounds, etc. (Sounds only come from certain areas and the volume decreases the further away you go)
	CHANNEL_GROUP_AMBIENT_MUSIC,				///< Wide-area environmental noise (occurs at the same volume in a certain area)
	CHANNEL_GROUP_WEATHER_EFFECT_SOUND,			///< Weather sound effects (weather-related sounds are played briefly)
	CHANNEL_GROUP_WEATHER_MUSIC,				///< Weather music (repeat infinitely until stopped randomly)
	CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE,	///< Same as CHANNEL_GROUP_JINGLE_MUSIC but does not affect BGM

	NUM_CHANNEL_GROUP,

	INVALID_CHANNEL_GROUP
};

enum eBGMType
{
	BGM_TYPE_SHARE_THEME,						///< DBO Common Theme
	BGM_TYPE_LOCAL_THEME,						///< Local theme song
	BGM_TYPE_MAIN_THEME,						///< BGM played outside of the game
	BGM_TYPE_RANGE,								///< BGM played within a certain range
	BGM_TYPE_PVP,								///< Player 1:1 Battle Theme Song
	BGM_TYPE_RANKBATTLE_BATTLE,					///< Rank Battle
	BGM_TYPE_THEME_BGM,							///< THEME BGM
	BGM_TYPE_SERVER_PLAYED,						///< BGM played upon request from the server
	BGM_TYPE_CINEMATIC,							///< Cinematic BGM	
	BGM_TYPE_WAIT,

	NUM_BGM_TYPE,

	INVALID_BGM_TYPE
};

enum eFadeInOutType
{	
	FADE_IN,									///< Apply Fade In
	FADE_OUT									///< Apply Fade Out
};

enum eStoreResult
{
	STORE_READY_TO_PLAY,						///< Play right away
	STORE_LIST,									///< Manage separately only on the list
	STORE_FAIL,									///< Save failed
};

struct sNtlVolume
{
	///<Range: 0.0f to 1.0f
	float		fMainVolume;
	float		fFadeVolume;
	float		fWhenMoviePlayVolume;			///< Volume when video is played

	sNtlVolume()
	:fMainVolume(1.f)
	,fFadeVolume(1.f)
	,fWhenMoviePlayVolume(1.f) {}
};

struct sNtlPitch
{	
	float		fMainPitch;					///< Range: 0.0f ~ 10.f (default: 1.f)
	float		fPitchRate;					///< Scope: No limit
	float		fSlowMotionPitchRate;		///< Scope: No limit

	sNtlPitch() : fMainPitch(1.f), fPitchRate(1.f), fSlowMotionPitchRate(1.f) {}
};

struct sNtlMinMax
{
	float		fMin, fMax;

	sNtlMinMax() : fMin(1.f), fMax(1.f + MIN_DISTANCE_BETWEEN_MINMAX) {}
};

struct sNtlSoundPlayParameta
{
	// for 2D
	SOUND_HANDLE		hHandle;			///< Returns the handle of the created Channel.
	int					iChannelGroup;		
	char*				pcFileName;
	
	// extra parameter
	eBGMType			eBGM_Type;
	bool				bBGMReplay;
	bool				bLoop;
	float				fVolume;			///< 0.0 ~ 1.f
	float				fPitch;				///< 0.0 ~ 10.0f

	// for 3D
	float				fXPos;
	float				fYPos;
	float				fZPos;	
	float				fMinDistance;		///< Minimum distance at which volume is maximized
	float				fMaxDistance;		///< Maximum distance at which volume is minimum

	sNtlSoundPlayParameta()
	:hHandle(INVALID_SOUND_HANDLE)
	,iChannelGroup(INVALID_CHANNEL_GROUP)
	,pcFileName(NULL)
	,eBGM_Type(BGM_TYPE_LOCAL_THEME)
	,fXPos(0.f)
	,fYPos(0.f)
	,fZPos(0.f)
	,bBGMReplay(false)
	,bLoop(false)
	,fVolume(dNTLSOUND_VOLUME_DEFAULT)
	,fMinDistance(10.f)
	,fMaxDistance(50.f)
	,fPitch(dNTLSOUND_PITCH_DEFAULT)
	{ }
};

// Convert FMOD's left-hand coordinate system to the renderer's coordinate system
#define dCONVERT_COORDINATE_X(fX)		(-1.f * fX)