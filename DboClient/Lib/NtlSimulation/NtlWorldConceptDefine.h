/*****************************************************************************
 *
 * File			: NtlWorldConceptDefine.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2007. 7. 13
 * Abstract		: Simulation world concept defines
 *****************************************************************************
 * Desc         : Defines matters related to the World Concept.
 *
 *****************************************************************************/


#ifndef __NTL_WORLDCONCEPT_DEFINE_H__
#define __NTL_WORLDCONCEPT_DEFINE_H__

#define WORLD_CONCEPT_FIRST_GRADE	0x00010000
#define WORLD_CONCEPT_SECOND_GRADE	0x00020000
#define WORLD_CONCEPT_THIRD_GRADE	0x00040000
#define WORLD_CONCEPT_FOURTH_GRADE	0x00080000

/// What kind of state is controlled by the World Concept class?
// avooo's comment: Please add an index to Logic_WorldConceptMessage
enum EWorldPlayConcept
{
	WORLD_PLAY_NONE					= 0,

	//--Stage 1 Concept--//
	WORLD_PLAY_TUTORIAL				= 0x00010001,		///< Tutorial
	WORLD_PLAY_TIME_MACHINE			= 0x00010002,		///< Time Machine Quest
	WORLD_PLAY_RANK_BATTLE			= 0x00010003,		///< Rank Battle
	WORLD_PLAY_T_BUDOKAI			= 0x00010004,		///< World's Best Martial Arts Club (Battle Status)
	WORLD_PLAY_PARTY_DUNGEON		= 0x00010005,		///< Party Dungeon
	WORLD_PLAY_TLQ					= 0x00010006,		///< Time Leap Quest (TLQ)
	WORLD_PLAY_DOJO_SCRAMBLE		= 0x00010007,		///< Dojo competition
	WORLD_PLAY_DOJO_DUEL			= 0x00010008,		///< Dojo non-radio
	WORLD_PLAY_CCBD					= 0x00010009,
	WORLD_PLAY_DWC					= 0x00010010,

	//--Stage 2 Concept --//
	WORLD_PLAY_FREEPVP				= 0x00020000,		///< PVP(Battle) state
	WORLD_PLAY_NPC_COMMU			= 0x00020001,		///< NPC conversation status
	WORLD_PLAY_TRADE				= 0x00020002,		///< User Trade Status
	WORLD_PLAY_DRAGONBALL_COLLECT	= 0x00020003,		///< Event state to collect dragon balls and summon the dragon god
	WORLD_PLAY_PRIVATESHOP			= 0x00020004,		///< Personal store status (seller)
	WORLD_PLAY_PRIVATESHOP_VISITOR	= 0x00020005,		///< Personal store status (buyer)
	WORLD_PLAY_REMOTE_BEHAVIOUR		= 0x00020006,
	WORLD_PLAY_MASCOTEX_SKILL		= 0x00020007,
	
	//--Stage 3 Concept --//
	WORLD_PLAY_DIRECT				= 0x00040006,		///< Direction status (cinematic, narraion)

	WORLD_PLAY_DRAGONBALL_SCRAMBLE	= 0x00080001,

	WORLD_PLAY_FREEPVP_ZONE			= 0x00100001,	// free pvp for all
};


/// Transition state flags in World Concept
enum EWorldState
{
	WORLD_STATE_NONE,
	WORLD_STATE_ENTER,
	WORLD_STATE_IDLE,
	WORLD_STATE_EXIT,

	// Status related to Dragon Ball Collection
	WORLD_DBC_ALTAR_UI,
	WORLD_DBC_NIGHT_ON,
	WORLD_DBC_ALTAR_EFFECT,
	WORLD_DBC_SPAWN_DRAGON,
	WORLD_DBC_NARRATION_START,
	WORLD_DBC_REWARD_UI,
	WORLD_DBC_NARRATION_END,
	WORLD_DBC_DESTROY_DRAGON,
	WORLD_DBC_DESTROY_DB,
	WORLD_DBC_NIGHT_OFF,
};

enum EWorldStateNPC
{
	WORLD_NPC_SHOP = WORLD_STATE_EXIT + 1,
	WORLD_NPC_TRAINER,
	WORLD_NPC_ITEM_UPGRADE,
	WORLD_NPC_WAREHOUSE,
	WORLD_NPC_GUILD_WAREHOUSE,
};

/// Flag list for determining whether avatar action is possible or not
enum EAvatarAction
{
	E_ACTION_MOVE,
	E_ACTION_PICK_WORLD,
	E_ACTION_PICK_OBJECT,
	E_ACTION_TARGETTING,
	E_ACTION_ATTACK,
	E_ACTION_CHANGE_HEADING,
	E_ACTION_CAMERA_CONTROL,
	E_ACTION_CHARGING,
	E_ACTION_LOOTING,
	E_ACTION_CAN_COMMUNICATION,
};

enum EWorldStateTMQ
{
	WORLD_STATE_TMQ_WAIT,					// Create TMQ and wait for player entry
	WORLD_STATE_TMQ_TERRAIN_READY,			// Terrain loading complete
	WORLD_STATE_TMQ_PREPARE,				// After all players enter, check their conditions and decide whether to start or not.
	WORLD_STATE_TMQ_ARRIVE,					// Presentation status (waiting for event from TS, waiting for event when user appears)
	WORLD_STATE_TMQ_BEGIN,					// Start Game (Script Control)
	WORLD_STATE_TMQ_STAGE_READY,			// Stage preparation (script control)
	WORLD_STATE_TMQ_STAGE_START,			// Start Stage (Script Control)
	WORLD_STATE_TMQ_STAGE_FINISH,			// End Stage (Script Control)
	WORLD_STATE_TMQ_END,					// game over
	WORLD_STATE_TMQ_LEAVE,					// exit game status
	WORLD_STATE_TMQ_FAIL,					// Game over (TMQ failed)
	WORLD_STATE_TMQ_CLOSE,					// TMQ ends
};

#endif