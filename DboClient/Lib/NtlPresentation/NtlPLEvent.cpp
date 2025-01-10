#include "precomp_ntlpresentation.h"
#include "NtlPLEvent.h"


//Animation Event
RWS::CEventId g_EventAnimEnd;			//Animation is finished
RWS::CEventId g_EventAnimHit;           //Animation Attack Event
RWS::CEventId g_EventAnimFootStep;      //When the sole of the character's foot touches the ground while running
RWS::CEventId g_EventWeightTime;		//Animation WeightTimeEvent
RWS::CEventId g_EventSubWeaponActive;	 //Activate the sub weapon (e.g. pull out the sword worn on your back)
RWS::CEventId g_EventSubWeaponDeActive;	 //Deactivate the sub weapon (e.g. put the drawn sword on your back again)
RWS::CEventId g_EventSummon;			 //Make summmon active. (Timing event where a summon summons a summon.)
RWS::CEventId g_EventAlpha;               // Events for Alpha Fade
RWS::CEventId g_EventAnimTMQ;            // TMQ production event
RWS::CEventId g_EventExplosion;           // explosion event
RWS::CEventId g_EventAnimPostEffect;      // Post Effect Event
RWS::CEventId g_EventAnimDirect;          // Directed event
RWS::CEventId g_EventAnimColorChange;     // Color change event
RWS::CEventId g_EventAnimStretch;         // Arm stretching event
RWS::CEventId g_EventAnimTrigger;         // trigger event
RWS::CEventId g_EventAnimSkillCancel;	  // anim cancel point	


// world field switching effects
RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY;
RWS::CEventId NPEI_IS_MAPNAME_CHANGE;


// Trigger
RWS::CEventId g_EventTriDoodads;	// Trigger when map objects are loaded or deleted


// Event Object
RWS::CEventId g_EventCreateEventObject; ///< When an event object is loaded or deleted
RWS::CEventId g_EventThreadLoadingComplete; // An event that notifies the end of thread loading of an entity.


// gui event
RWS::CEventId g_EventResize;					// Resize client window

RWS::CEventId g_EventCinematicViewShow;	
RWS::CEventId g_EventCinematicBalloon;
RWS::CEventId g_EventCinematicFlash;
RWS::CEventId g_EventCinematicEcho;
RWS::CEventId g_EventResetCinematic;			// Reset Cinematic Action. Called only from GUI


RWS::CEventId g_EventCinematicFadeOut;         // Cinematic directing begins
RWS::CEventId g_EventCinematicFadeIn;          // End of cinematic production

RWS::CEventId g_EventCinematicBreak;			// Cinematic directing break

RWS::CEventId g_EventShowProfilerGui;			// ProfilerGUI Open/Close

RWS::CEventId g_EventShowSpectatorBalloon;
RWS::CEventId g_EventHideAllSpectatorBalloon;

RWS::CEventId g_EventShowPlayerName;			// PlayerName Entity°¡ Show/Hide

RWS::CEventId g_EventDirectPlayCancel;
