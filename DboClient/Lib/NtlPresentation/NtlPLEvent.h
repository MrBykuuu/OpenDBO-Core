/*****************************************************************************
 *
 * File			: NtlPLEvent.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 9. 29	
 * Abstract		: presentation layer event type
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLEVENT_H__
#define __NTL_PLEVENT_H__

#include "ceventhandler.h"

//AnimationEvent related
extern RWS::CEventId g_EventAnimEnd;			 //When animation is finished
extern RWS::CEventId g_EventAnimHit;             //Animation Hit Event
extern RWS::CEventId g_EventAnimFootStep;		 //Animation When the soles of your feet touch the ground
extern RWS::CEventId g_EventWeightTime;			 //Animaiton Weight Event
extern RWS::CEventId g_EventSubWeaponActive;	 //Activate the sub weapon (e.g. pull out the sword worn on your back)
extern RWS::CEventId g_EventSubWeaponDeActive;	 //Deactivate the sub weapon (e.g. put the drawn sword on your back again)
extern RWS::CEventId g_EventSummon;				 //Make summmon active. (Timing event where a summon summons a summon.)
extern RWS::CEventId g_EventAlpha;               // Events for Alpha Fade
extern RWS::CEventId g_EventAnimTMQ;             // TMQ production event
extern RWS::CEventId g_EventExplosion;           // explosion event
extern RWS::CEventId g_EventAnimPostEffect;      // Post Effect Event
extern RWS::CEventId g_EventAnimDirect;          // Production event
extern RWS::CEventId g_EventAnimColorChange;     // Color change event
extern RWS::CEventId g_EventAnimStretch;         // An event that increases the number of examples
extern RWS::CEventId g_EventAnimTrigger;         // Trigger events used in a variety of ways
extern RWS::CEventId g_EventAnimSkillCancel;	 // A point at which animatin can be canceled during skill animation.

struct SNtlEventAnimEnd
{
	RwUInt32 uiSerialId;
	RwUInt32 uiBaseAnimKey;
	RwUInt32 uiUpperAnimKey;
};

struct SNtlEventAnimHit
{
	RwUInt32	uiSerialId;
	void		*pData;							//SEventAnimHit(Character\Base  NtlAnimEventData.h)
};


struct SNtlEventFootStep
{
	RwUInt32 uiSerialId;                        ///< Serial ID of character object
    void*    pData;                             // S event foot step

    //RwBool   bLeftFoot;                        ///< Foot type (True: left, False: right)
};

struct SNtlEventWeightTime						//SEventWeightTime(Character\Base NtlAnimEventData.h)
{
	RwUInt32	uiSerialId;
	void		*pData;
};

struct SNtlEventSubWeaponActive					//Activate the sub weapon (e.g. pull out the sword worn on your back)
{
	RwUInt32 uiSerialId;
};

struct SNtlEventSubWeaponDeActive				//Deactivate the sub weapon (e.g. put the drawn sword on your back again)
{
	RwUInt32 uiSerialId;
};

struct SNtlEventSummon							//Make summmon active. (Timing event where a summon summons a summon.)
{
	RwUInt32 uiSerialId;
};

struct SNtlEventAlpha                           /// Alpha Fade Event
{
    RwUInt32 uiSerialId;
    void*    pData;                             ///< Pointer to Alpha Fade Event object
};

struct SNtlPLEventTMQ
{
    RwUInt32    uiSeiralID;                         ///< Serial ID of the time machine object that sent the event
    RwBool      bIn;                                ///< Whether to enter TMQ (if True, enter /if False, exit)
};

struct SNtlPlEventExplosion
{
    RwUInt32    uiSerialID;                     ///< Serial ID of object
    void*       pData;                          ///< Pointer to Explosion Event structure
};

struct SNtlEventPostEffect
{
    RwUInt32    uiSerialID;                     ///< ID of the avatar that generates the event
    void*       pData;                          ///< Pointer to Post Effect Event structure
};

/// Production event
struct SNtlEventDirect
{
    RwUInt32    uiSerialID;                     ///< ID of the object that generates the event
    void*       pData;                          ///< Pointer to SEventDirect structure
};

/// color change event
struct SNtlEventColorChange
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< Pointer to SEventColorChange structure
};

/// Bonus event
struct SNtlEventStretch
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< Pointer to SEventStretch structure
};

struct SNtlEventTrigger
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< Pointer to SEventTrigger structure
};

struct SNtlEventAnimSkillCancel
{
	RwUInt32    uiSerialID;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world field switching effects
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY;
extern RWS::CEventId NPEI_IS_MAPNAME_CHANGE;

// struct sNTL_EVENT_FOG
// {
// 	void *pData;
// };
// 
// struct sNTL_EVENT_VARIATION
// {
// 	void *pData;
// };
// 
struct sNTL_EVENT_MAPNAME
{
 	void *pData;
};
// 
// struct sNTL_EVENT_SECTOR_CHANGED
// {
// 	void *pData;
// };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// trigger
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventTriDoodads;

struct sNTL_EVENT_TRI_DOODADS
{
	// 0 : Create, 1 : Delete
	RwBool				FlgTriDoodads;
	std::vector<void*>	vecDat;
};

extern RWS::CEventId g_EventCreateEventObject;
extern RWS::CEventId g_EventThreadLoadingComplete; // An event that notifies the end of thread loading of an entity.

/// EVENT Object
struct SNtlPLEventEventObject
{
    RwBool          bCreate;        // TRUE : Create, FALSE : Delete
    void*           pData;          // CNtlPlObject Entity
};

struct SNtlPlThreadLoadingComplete
{
    RwUInt32    uiSerialID;                     ///< Serial ID of the entity
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gui Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////
/**
* Resolution Resize
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventResize;						// Resize client window

struct SNtlPLEventResize
{
	RwInt32 iWidht;
	RwInt32 iHeight;
	RwBool bRestore_from_Minimize;
};

//////////////////////////////////////////////
/**
* Cinematic view show / hide
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicViewShow;
extern RWS::CEventId g_EventCinematicBalloon;
extern RWS::CEventId g_EventCinematicFlash;
extern RWS::CEventId g_EventCinematicEcho;
extern RWS::CEventId g_EventResetCinematic;				// Reset Cinematic Action. Called only from GUI


//////////////////////////////////////////////
/**
* Cinematic view show / hide
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicFadeOut;           // Cinematic directing begins
extern RWS::CEventId g_EventCinematicFadeIn;	        // End of cinematic production

//////////////////////////////////////////////
/**
* Cinematic Break
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicBreak;


//////////////////////////////////////////////
/**
* ProfilerGui show / hide
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowProfilerGui;

//////////////////////////////////////////////
/**
* Chat Balloon
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowSpectatorBalloon;		
extern RWS::CEventId g_EventHideAllSpectatorBalloon;	

//////////////////////////////////////////////
/**
* Player related
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowPlayerName;                     ///< Name Show/Hide

extern RWS::CEventId g_EventDirectPlayCancel;



struct SNtlEventCinematicViewShow
{
	RwBool			bShow;
};

struct SNtlEventCinematicBalloon
{
	enum CINEMATICBALLOONPLAY
	{
		PLAY_BEGIN,
		PLAY_PROGRESS
	};

	RwBool			bShow;
	RwUInt32		hSerialId;
	RwUInt32		uiQuestTextTblId;
	RwUInt8			byBalloonType;
	RwUInt8			byEmotionType;
	RwUInt8			byPlayType;
};

struct SNtlEventCinematicFlash
{
	// Flash 
	enum PLAYTIMETYPEFLAG 
	{
		FLAG_BEGIN	= 0x01,
		FLAG_PLAY	= 0x02,
		FLAG_END	= 0x04,

		BEGIN_PLAY_END	= FLAG_BEGIN | FLAG_PLAY | FLAG_END,	// begin + play + end time
		BEGIN_PLAY		= FLAG_BEGIN | FLAG_PLAY,				// begin + play time
		PLAY_END		= FLAG_PLAY | FLAG_END,					// play + end time
		PLAY			= FLAG_PLAY								// play time
	};

	RwBool			bShow;
	RwChar*			pFilename;
	RwBool			bUseTime;							// True: Part-time False: Directed once
	RwReal			fLifeTime;
	void*			pEchoData;
	RwReal			fBeginAlphaTime;
	RwReal			fEndAlphaTime;		
	RwUInt32		flagPlayTimeType;										
};

struct SNtlEventCinematicEcho
{
	void*			hCinematic;
};

struct SNtlEventResetCinematic
{
	enum TYPE { CINEMATIC, QUESTNARRATION, DBCNARRATION };

	RwUInt32	eDemandResetObject;
};

enum ECinematicFadeInOutType
{
	CINEMATIC_FADEINOUT_SCREEN_PANEL,
	CINEMATIC_FADEINOUT_SHUTDOWN_TV,

	CINEMATIC_INVALID
};

struct SNtlEventCinematicFadeInOut
{
	RwUInt8		byType;
	RwReal		fLifeTime;		// Interpolation time.
	RwUInt8		byRed;
	RwUInt8		byGreen;
	RwUInt8		byBlue;
	RwUInt8		byEndAlpha;
};

struct SNtlEventShowProfilerGui
{
	RwBool		bShow;
};

struct SNtlEventShowSpectatorBalloon
{
	RwV3d		vPos;
	WCHAR*		szMessage;
	RwReal		fLifeTime;
	RwReal		fFadeInTime;
	RwUInt8		byBalloonType;
};

struct SNtlEventShowPlayerName
{
	RwUInt32	hSerialId;
	RwBool		bShow;
};

struct SNtlEventCharCreateComplete
{
    RwUInt32    hSerialId;
};


#endif