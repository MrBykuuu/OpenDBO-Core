//***********************************************************************************
//
//	File		:	NtlMatch.h
//
//	Begin		:	2008-05-06
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************


#pragma once

#include "NtlSharedDef.h"
#include "NtlSharedType.h"
#include "NtlParty.h"


//---------------------------------------------------------------------------------------

#define	NTL_MAX_MINORMATCH_TEAM_COUNT	32


// Maximum extended stage count

#define NTL_MAX_MATCH_EXTENDED_STAGE_COUNT		3

// Maximum stage count

#define NTL_MAX_MATCH_STAGE_COUNT				(5 + NTL_MAX_MATCH_EXTENDED_STAGE_COUNT)

// The maximum number of Grand Final of the Grand Final will be counted. Note: Numbers greater than NTL_MAX_MATCH_EXTENDED_STAGE_COUNT can not be reached.

#define DBO_MAX_MAJORMATCH_EXTENDED_STAGE_COUNT	1


// The maximum number of items of the competition compensation at the first ball

#define	MAX_BUDOKAI_MATCH_REWARD_ITEM_COUNT		3

//---------------------------------------------------------------------------------------

// Match Type

enum eMATCHWORLD_TYPE
{
	MATCHWORLD_TYPE_MINOR_MATCH,			// Preliminary: eBUDOKAI_MINORMATCH_STATE

	MATCHWORLD_TYPE_MAJOR_MATCH,			// Mains: eBUDOKAI_MAJORMATCH_STATE

	MATCHWORLD_TYPE_FINAL_MATCH,			// Finals: eBUDOKAI_FINALMATCH_STATE


	MAX_MATCHWORLD_TYPE,
	INVALID_MATCHWORLD_TYPE = 0xff
};



// Member status

enum eMATCH_MEMBER_STATE
{
	MATCH_MEMBER_STATE_NONE,			// Initial state

	MATCH_MEMBER_STATE_NORMAL,			// Normal state

	MATCH_MEMBER_STATE_FAINT,			// Dead

	MATCH_MEMBER_STATE_OUTOFAREA,		// Outside

	MATCH_MEMBER_STATE_GIVEUP,			// Abandon status (connection termination, etc.)

	MATCH_MEMBER_STATE_NOAPPLY,			// There is PC information but it does not apply to game setting and judgment.

	MATCH_MEMBER_STATE_TELEPORTING,		// It is teleporting. Teleporting from inside the stadium to the inside

	MATCH_MEMBER_STATE_BE_LATE,			// Person entering the game, perception, applied from next game

	MATCH_MEMBER_STATE_RESCUE,			// Resurrection (teleport)

};


enum eMATCH_TEAM_TYPE
{
	MATCH_TEAM_TYPE_TEAM1,
	MATCH_TEAM_TYPE_TEAM2,

	MAX_MATCH_TEAM_TYPE_COUNT,
	INVALID_MATCH_TEAM_TYPE = 0xffff
};


//
enum eMATCH_RESULT
{
	MATCH_RESULT_SUCCESS,

	//	MATCH_RESULT_TYPE_ALL_LEAVED, //no one
	//	MATCH_RESULT_TYPE_STAY_ALONE, //left alone


	MATCH_RESULT_DRAW,						// draw

	MATCH_RESULT_WIN,						// A winner emerges


	MATCH_RESULT_TIMEOVER,					// time is running out


	MATCH_RESULT_DOJO_RECOMMENDER,			// dojo recommender

	MATCH_RESULT_UNEARNED_WIN,				// unearned win


	INVALID_MATCH_RESULT = 0xff,
};


// Preliminaries: Battle Royale

enum eBUDOKAI_MINORMATCH_STATE
{
	BUDOKAI_MINORMATCH_STATE_NONE,			// Initial state

	BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH,	// Preliminary waiting time

	BUDOKAI_MINORMATCH_STATE_WAIT,			// Waiting for the players

	BUDOKAI_MINORMATCH_STATE_DIRECTION,		// Direction

	BUDOKAI_MINORMATCH_STATE_MATCH_READY,	// Preparation for the game

	BUDOKAI_MINORMATCH_STATE_STAGE_READY,	// Stage preparation

	BUDOKAI_MINORMATCH_STATE_STAGE_RUN,		// Stage progress

	BUDOKAI_MINORMATCH_STATE_STAGE_SELECTION,	// A tied winner lottery

	BUDOKAI_MINORMATCH_STATE_STAGE_FINISH,	// End stage

	BUDOKAI_MINORMATCH_STATE_MATCH_FINISH,	// Game end

	BUDOKAI_MINORMATCH_STATE_END,			// End


	MAX_BUDOKAI_MINORMATCH_STATE,
	INVALID_BUDOKAI_MINORMATCH_STATE = INVALID_BYTE
};



// finals

enum eBUDOKAI_MAJORMATCH_STATE
{
	BUDOKAI_MAJORMATCH_STATE_NONE,			// initial state

	BUDOKAI_MAJORMATCH_STATE_WAIT,			// waiting for the competitors

	BUDOKAI_MAJORMATCH_STATE_DIRECTION,		// production

	BUDOKAI_MAJORMATCH_STATE_MATCH_READY,	// game preparation

	BUDOKAI_MAJORMATCH_STATE_STAGE_READY,	// stage preparation

	BUDOKAI_MAJORMATCH_STATE_STAGE_RUN,		// Stage progress

	BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH,	// Stage End

	BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH,	// game over

	BUDOKAI_MAJORMATCH_STATE_END,			// end


	MAX_BUDOKAI_MAJORMATCH_STATE,
	INVALID_BUDOKAI_MAJORMATCH_STATE = INVALID_BYTE
};



// final

enum eBUDOKAI_FINALMATCH_STATE
{
	BUDOKAI_FINALMATCH_STATE_NONE,			// Initial state

	BUDOKAI_FINALMATCH_STATE_WAIT,			// Waiting for the electrons

	BUDOKAI_FINALMATCH_STATE_DIRECTION,		// Direction

	BUDOKAI_FINALMATCH_STATE_MATCH_READY,	// New game preparation

	BUDOKAI_FINALMATCH_STATE_STAGE_READY,	// Stage preparation

	BUDOKAI_FINALMATCH_STATE_STAGE_RUN,		// Stage progress

	BUDOKAI_FINALMATCH_STATE_STAGE_FINISH,	// End stage

	BUDOKAI_FINALMATCH_STATE_MATCH_FINISH,	// Game end

	BUDOKAI_FINALMATCH_STATE_FINALDIRECTION,// Final Directing

	BUDOKAI_FINALMATCH_STATE_AWARDING,		// Awarding

	BUDOKAI_FINALMATCH_STATE_END,			// End


	MAX_BUDOKAI_FINALMATCH_STATE,
	INVALID_BUDOKAI_FINALMATCH_STATE = INVALID_BYTE
};



//---------------------------------------------------------------------------------------
// Final Match


enum eFINALMATCH_TYPE
{
	FINALMATCH_TYPE_SEMIFINAL_1 = 0,
	FINALMATCH_TYPE_SEMIFINAL_2,
	FINALMATCH_TYPE_FINAL,

	FINALMATCH_TYPE_COUNT,
	INVALID_FINALMATCH_TYPE = 0xFF,
};

enum eFINALMATCH_RESULT
{
	FINALMATCH_RESULT_WINNER,			// win

	FINALMATCH_RESULT_SEMIWINNER,		// Runner-up

	FINALMATCH_RESULT_MATCH1_LOSER,		// Lost in semifinal game 1

	FINALMATCH_RESULT_MATCH2_LOSER,		// Lost 2nd semifinal game


	MAX_FINALMATCH_RESULT,
};


//---------------------------------------------------------------------------------------
#pragma pack(1)


struct sMATCH_SCORE
{
	BYTE	byTeam1;
	BYTE	byTeam2;
};


//
struct sMATCH_MEMBER_STATE_INFO
{
	HOBJECT		handle;				// pc handle

	BYTE		byState;			// E match member state

	BYTE		byLevel;
	BYTE		byClass;			// E pc class

	WORD		wGuildName;			// WCHAR		wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];

	WORD		wWinCount;			// RankBattle Record Win

	WORD		wDrawCount;			// RankBattle Record Draw

	WORD		wLoseCount;			// RankBattle Record Lose

	WORD		wCharName;			// WCHAR		wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

};


// MinorMatch TeamInfo

struct sMATCH_MINORMATCH_TEAM_INFO
{
	TEAMTYPE					wTeamType;
	WCHAR						wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	bool						bDojoRecommender;
	BYTE						byMemberCount;
	sMATCH_MEMBER_STATE_INFO	aMembers[NTL_MAX_MEMBER_IN_PARTY];
};

// MinorMatch TeamInfo Variable Data

struct sMATCH_MINORMATCH_TEAM_INFO_VAR
{
	TEAMTYPE	wTeamType;
	WORD		wTeamName_var;		//	WCHAR [NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	bool		bDojoRecommender;	// Dojo recommendation

	BYTE		byMemberCount;
	WORD		wMembers_var;		// sMATCH_MEMBER_STATE_INFO [NTL_MAX_MEMBER_IN_PARTY];

};

typedef	sMATCH_MINORMATCH_TEAM_INFO			sMATCH_MAJORMATCH_TEAM_INFO;
typedef sMATCH_MINORMATCH_TEAM_INFO_VAR		sMATCH_MAJORMATCH_TEAM_INFO_VAR;

typedef	sMATCH_MINORMATCH_TEAM_INFO			sMATCH_FINALMATCH_TEAM_INFO;
typedef sMATCH_MINORMATCH_TEAM_INFO_VAR		sMATCH_FINALMATCH_TEAM_INFO_VAR;


// MinorMatch Team score

struct sMATCH_MINORMATCH_TEAM_SCORE
{
	TEAMTYPE	teamType;
	BYTE		byScore;
};

#pragma pack()
