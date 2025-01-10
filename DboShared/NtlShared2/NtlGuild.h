//***********************************************************************************
//
//	File		:	NtlGuild.h
//
//	Begin		:	2007-03-19
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

const DWORD DBO_MAX_MEMBER_IN_GUILD = 190;
const DWORD DBO_MAX_SECOND_MASTER_IN_GUILD = 4;
const DWORD DBO_ZENNY_FOR_NEW_GUILD = 200000; //
const DWORD DBO_LEVEL_FOR_NEW_GUILD = 30;
const DWORD DBO_ZENNY_FOR_GUILD_POINT = 10000;
const DWORD DBO_POINT_FOR_GUILD_ZENNY = 5;
const DWORD DBO_ZENNY_FOR_GUILD_CHANGE_MARK = 10000000;

// WARNING:
// You can add a new guild function but MUST NOT REMOVE one. These index numbers are synchronized with DB.
// Guild features can be added, but not removed. Index numbers are synchronized with the DB.
// by YOSHIKI(2007-04-25)
// itzrnb -Since it is synchronized with the DB, it cannot be inserted at the beginning or in the middle. Add only

enum eDBO_GUILD_FUNCTION
{
	DBO_GUILD_FUNCTION_MAX_MEMBER_30 = 0,
	DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1,
	DBO_GUILD_FUNCTION_NOTICE_BOARD,
	DBO_GUILD_FUNCTION_POST,
	DBO_GUILD_FUNCTION_WAREHOUSE,

	DBO_GUILD_FUNCTION_MAX_MEMBER_50,
	DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2,

	DBO_GUILD_FUNCTION_MAX_MEMBER_70,

	DBO_GUILD_FUNCTION_MAX_MEMBER_90,
	DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3,
	DBO_GUILD_FUNCTION_CAN_SET_EMBLEM,

	DBO_GUILD_FUNCTION_MAX_MEMBER_110,

	DBO_GUILD_FUNCTION_MAX_MEMBER_130,
	DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_4,
	DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,
	DBO_GUILD_FUNCTION_CAN_USE_DOJO_TELEPORT,

	DBO_GUILD_FUNCTION_MAX_MEMBER_150,
//	Dbo guild function dojo upgrade 1,

	DBO_GUILD_FUNCTION_CAN_MAKE_DOGI,
	DBO_GUILD_FUNCTION_CAN_CHANGE_EMBLEM,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_2,
	DBO_GUILD_FUNCTION_TENKAICHI_SEED,

	DBO_GUILD_FUNCTION_MAX_MEMBER_170,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_3,
	DBO_GUILD_FUNCTION_DOJO_QUEST_1,
	DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_1,

	DBO_GUILD_FUNCTION_MAX_MEMBER_190,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_4,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_5,
	DBO_GUILD_FUNCTION_DOJO_QUEST_2,
	DBO_GUILD_FUNCTION_DOJO_VEHICLE,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_6,
	DBO_GUILD_FUNCTION_CAN_HAVE_BOOK,
	DBO_GUILD_FUNCTION_DOJO_UPGRADE_7,
	DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_2,

	DBO_GUILD_FUNCTION_COUNT,
	DBO_GUILD_FUNCTION_INVALID = 0xFF,
	DBO_GUILD_FUNCTION_FIRST = DBO_GUILD_FUNCTION_MAX_MEMBER_30,
	DBO_GUILD_FUNCTION_LAST = DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_2,
};

struct sDBO_GUILD_MARK
{
	
	bool IsIntialized()
	{
		if ( INVALID_BYTE == byMarkMain && INVALID_BYTE == byMarkMainColor && INVALID_BYTE == byMarkInLine &&
			INVALID_BYTE == byMarkOutLine && INVALID_BYTE == byMarkOutColor )
		{
			return false;
		}
		else
		{
			return true;
		}

	}
	
	BYTE					byMarkMain;
	BYTE					byMarkMainColor;
	BYTE					byMarkInLine;
	BYTE					byMarkInColor;
	BYTE					byMarkOutLine;
	BYTE					byMarkOutColor;
	
};

struct sDBO_DOGI_DATA
{
	bool IsIntialized(bool bIsDojo)
	{
		if ( bIsDojo )
		{
			if ( INVALID_BYTE == byDojoColor )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			if ( INVALID_BYTE == byGuildColor )
			{
				return false;
			}
			else
			{
				return true;
			}
		}

	}
	GUILDID         guildId;
	BYTE			byType;			// Lodging type

	BYTE		    byGuildColor;	// Guild Uniform Color

	BYTE			byDojoColor;	// Uniform coating color


};

struct sDBO_GUILD_DATA
{
	GUILDID			guildId;
	WCHAR			wszName[NTL_MAX_SIZE_GUILD_NAME + 1]; // Guild Name


	CHARACTERID		guildMaster;
	CHARACTERID		guildSecondMaster[DBO_MAX_SECOND_MASTER_IN_GUILD];

	DWORD			dwGuildReputation;
	DWORD			dwMaxGuildPointEver;
	QWORD			qwGuildFunctionFlag;

	DBOTIME			timeToDisband;

	sDBO_GUILD_MARK	sMark;
	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME + 1]; //Player Name who wrote announcement

	WCHAR			awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];	
	sDBO_DOGI_DATA  sDogi; 
};

typedef sDBO_GUILD_DATA sDBO_GUILD_INFO;


// Data for GMTool [4/22/2008 SGpro]

struct sDBO_GMTOOL_GUILD_DATA
	: public sDBO_GUILD_DATA
{
	SERVERFARMID	serverfarmID;
	WCHAR			wszMasterName[NTL_MAX_SIZE_CHAR_NAME + 1];
	DWORD			dwZenny;	
};

// Search data for GMTool [7/1/2008 SGpro]

struct sDBO_GMTOOL_GUILD_SEARCH_DATA
{
	GUILDID			guildId;
	SERVERFARMID	serverfarmID;
	WCHAR			wszName[NTL_MAX_SIZE_GUILD_NAME + 1];
	DWORD			dwGuildReputation;
	DWORD			dwMaxGuildPointEver;
	WCHAR			wszMasterName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID		guildMaster;	
};

struct sDBO_GUILD_MEMBER_DATA
{
	ACCOUNTID		accountId;
	CHARACTERID		charId;
	WCHAR			wszMemberName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byLevel;
	DWORD			dwReputation;
};

struct sDBO_GUILD_MEMBER_INFO
{
	sDBO_GUILD_MEMBER_INFO() :
			charId(INVALID_CHARACTERID),
			byRace(INVALID_BYTE),
			byClass(INVALID_BYTE),
			byLevel(INVALID_BYTE),
			dwReputation(INVALID_DWORD),
			bIsOnline(false),
			mapNameTblidx(INVALID_TBLIDX)
	{
		::ZeroMemory(wszMemberName, sizeof(wszMemberName));
	}

	sDBO_GUILD_MEMBER_INFO(sDBO_GUILD_MEMBER_INFO* pGuildMemberInfo) :
		charId(pGuildMemberInfo->charId),
		byRace(pGuildMemberInfo->byRace),
		byClass(pGuildMemberInfo->byClass),
		byLevel(pGuildMemberInfo->byLevel),
		dwReputation(pGuildMemberInfo->dwReputation),
		bIsOnline(pGuildMemberInfo->bIsOnline),
		mapNameTblidx(pGuildMemberInfo->mapNameTblidx)
	{
		::CopyMemory(wszMemberName, pGuildMemberInfo->wszMemberName, NTL_MAX_SIZE_CHAR_NAME * sizeof(WCHAR));
		wszMemberName[NTL_MAX_SIZE_CHAR_NAME] = L'\0';
	};


	CHARACTERID		charId;
	WCHAR			wszMemberName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byLevel;
	DWORD			dwReputation;
	bool			bIsOnline;
	TBLIDX			mapNameTblidx; //New

};

// Data for GMTool [4/22/2008 SGpro]

struct sDBO_GMTOOL_GUILD_MEMBER_DATA
	: public sDBO_GUILD_MEMBER_DATA
{
	SERVERFARMID serverfarmID;
	GUILDID guildID;
	WCHAR wszAccountName[NTL_MAX_SIZE_USERID + 1];
};

struct sDBO_GUILD_FUNCTION_INFO
{
	eDBO_GUILD_FUNCTION		function;			
	eDBO_GUILD_FUNCTION		prerequisite;
	BYTE					byPassive;
	DWORD					dwRequiredPoint;
	DWORD					dwRequiredZenny;
	BYTE					byRequiredRock;
};

enum eDBO_GUILD_SEARCH_TYPE
{	
	DBO_GUILD_SEARCH_TYPE_GUILDNAME = 0,
	DBO_GUILD_SEARCH_TYPE_LEADERNAME,
	DBO_GUILD_SEARCH_TYPE_ALL,
};

struct sDBO_GUILD_CAHNGE_INFO
{
	WCHAR			wszName[NTL_MAX_SIZE_GUILD_NAME + 1];	// ''


	DWORD			dwGuildReputation;		// Guild reputation

	DWORD			dwMaxGuildPointEver;	// Load The highest dwGuildReputation value so far


	DWORD64			qwGuildFunctionFlag;	// Replace


	sDBO_GUILD_MARK	sMark;					// initial 0xff


	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME + 1];	// ''

	WCHAR			awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];	//''	

	sDBO_DOGI_DATA  sDogi; 
};

enum eDBO_GUILD_UPDATE_TYPE
{
	DBO_GUILD_UPDATE_TYPE_CANCEL_DEL = 0,
	DBO_GUILD_UPDATE_TYPE_NOTICE,
	DBO_GUILD_UPDATE_TYPE_NAME,
	DBO_GUILD_UPDATE_TYPE_MARK,
	DBO_GUILD_UPDATE_TYPE_REPUTATION,
	DBO_GUILD_UPDATE_TYPE_FUNCTION,
	DBO_GUILD_UPDATE_TYPE_ALL,
};

enum eDBO_GUILD_ZENNY_UPDATE_TYPE
{
	DBO_GUILD_ZENNY_UPDATE_TYPE_LEADER_INCOME = 0,	// Deposit by party leader -> Currently, this is the only information available.

	DBO_GUILD_ZENNY_UPDATE_TYPE_LEADER_OUTCOME,	// Withdrawal by party leader -> Currently, this is the only information available.

	DBO_GUILD_ZENNY_UPDATE_TYPE_VILLAGE_INCOME,		// Village quests, etc.

	DBO_GUILD_ZENNY_UPDATE_TYPE_BATTLE_OUTCOME,		// Compensation amount for dojo contest/non-radio refusal 

	DBO_GUILD_ZENNY_UPDATE_TYPE_BATTLE_INCOME,		// Refund of application fee for dojo battle/non-military battle

};


extern const sDBO_GUILD_FUNCTION_INFO g_aGuildFunctionInfo[DBO_GUILD_FUNCTION_COUNT];

// Checks if a guild has a specified guild function.
// Determine whether it has a guild function.
// by YOSHIKI(2007-04-25)

bool Dbo_HasGuildFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction);

// Retrieves information of a specified guild function.
// Obtain information about the specified guild function.
// by YOSHIKI(2007-04-25)

const sDBO_GUILD_FUNCTION_INFO* Dbo_GetGuildFunctionInfo(eDBO_GUILD_FUNCTION eGuildFunction);

// Checks if a guild can obtain guild function.
// Determine whether the guild function can be acquired.
// by YOSHIKI(2007-04-25)

bool Dbo_CanGetGuildFunction(DWORD64 qwGuildFunctionFlag, DWORD dwPoint, DWORD dwZenny, eDBO_GUILD_FUNCTION eGuildFunction, WORD* pwResultCode);

bool Dbo_CanGetDojoFunction(DWORD64 qwGuildFunctionFlag, DWORD dwPoint, DWORD dwZenny, eDBO_GUILD_FUNCTION eGuildFunction, WORD* pwResultCode);


bool Dbo_GetUIGuildFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction);

bool Dbo_GetUIDojoFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction);

// Returns how many members a guild can have at most.
// It tells you the maximum number of guild members a guild can have.
// by YOSHIKI(2007-04-26)

DWORD Dbo_GetMaxGuildMemberCount(DWORD64 qwGuildFunctionFlag);

// Returns how many second masters a guild can have at most.
// This tells you the maximum number of Second Masters a guild can have.
// by YOSHIKI(2007-04-26)

DWORD Dbo_GetMaxGuildSecondMasterCount(DWORD64 qwGuildFunctionFlag);

DWORD Dbo_GetGuildPoint(DWORD dwZenny);

BYTE GetDojoPeaceType( eDBO_GUILD_FUNCTION eGuildFunction );

bool IsGuildFunction( eDBO_GUILD_FUNCTION eGuildFunction );

bool IsDojoFunction( eDBO_GUILD_FUNCTION eGuildFunction );

DWORD Dbo_GetGuildDojoLevel(DWORD64 qwGuildFunctionFlag);

BYTE Dbo_GetDojoLevel( BYTE eGuildFunction );

DWORD64 AddGuildFunction( DWORD64 qwGuildFunctionFlag, BYTE byFunction );

DWORD64 RemoveGuildFunction( DWORD64 qwGuildFunctionFlag, BYTE byFunction );