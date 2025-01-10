#ifndef __NTL_STORAGE_DEFINE_H__
#define __NTL_STORAGE_DEFINE_H__

// Flags to apply
enum eNTL_STORAGE_APPLY
{
	eNTL_STORAGE_APPLY_PRESENTATION				= 0x00000001,
	eNTL_STORAGE_APPLY_SIMULATION				= 0x00000010,
	eNTL_STORAGE_APPLY_CLIENT					= 0x00000100,
	eNTL_STORAGE_APPLY_PRESENTATION_INITONCE	= 0x00001000,
	eNTL_STORAGE_APPLY_ALL						= 0x11110111
};

/**
* \brief GROUP TYPE of NTL_STORAGE UNITs (matches 1:1 with one File.)
*/
enum eNTL_STORAGE_GROUP_TYPE
{
	eNTL_STORAGE_GROUP_ACCOUNT,		///< Account information
	eNTL_STORAGE_GROUP_SYSTEM,		///< system
	eNTL_STORAGE_GROUP_GAMEINFO,	///< Game information
	eNTL_STORAGE_GROUP_SCOUTER,		///< Character Scouter Information
	eNTL_STORAGE_GROUP_QUEST,		///< Quest indicator information
	eNTL_STORAGE_GROUP_CHARACTER,	///< Character game information

	eNTL_STORAGE_GROUP_NUMS,
	eNTL_STORAGE_GROUP_INVALID = 0xFF
};

/**
* \brief Type of NtlStorageUnit
*/
enum eNTL_STORAGE_TYPE
{
	eNTL_STORAGE_ACCOUNT,			///< account
	eNTL_STORAGE_GRAPHIC,			///< Graphics
	eNTL_STORAGE_SOUND,				///< sound
	eNTL_STORAGE_GAMEINFO,			///< Game information
	eNTL_STORAGE_ETC,				///< Other
	eNTL_STORAGE_CHAT,				///< Chat
	eNTL_STORAGE_SCOUTER,			///< Monster scouter and character scouter information (combat power measurement)
	eNTL_STORAGE_QUEST,				///< Quest
	eNTL_STORAGE_CHARACTER,			///< Settings for each character
	eNTL_STORAGE_CHARACTER_ETC,
	eNTL_STORAGE_CHARACTER_SCOUTER,
	eNTL_STORAGE_SYSTEM_ETC,

	eNTL_STORAGE_NUMS,
	eNTL_STORAGE_INVALID = 0xFF
};

// Must match eNTL_STORAGE_TYPE
extern const char* g_acNtlStorageTypeString[eNTL_STORAGE_NUMS];

/**
* \brief Data Types of NtlStorageValue
*/
enum eNTL_STORAGE_VARIABLE_TYPE
{
	eINT,			// INT type
	eFLOAT,			// FLOAT type
	eBOOL,			// bool type
	eSTRING,		// string type
	eUSER,			// custom type

	eNTL_STORAGE_VARIABLE_NUMS,
	eNTL_STORAGE_VARIABLE_INVALID = 0xFF
};

// Check function pointer (used to check the range of values ??associated with the key.)
typedef bool (*NtlStorageUnitCheckFunc)(unsigned int uiKey);

// Apply function pointer (connected to the unit and apply on a unit basis.)
class CNtlStorageUnit;
typedef bool (*NtlStorageUnitApplyFunc)(CNtlStorageUnit* pUnit, unsigned int uiFlags);

// Length of NTL_STORAGE key name
#define dNTL_STORAGE_MAX_KEY_NAME_LENGTH	(32+1)

// Length of NTL_STORAGE VALUE
#define dNTL_STORAGE_MAX_VALUE_LENGTH		(128+1)

// mapping table
struct SNtlStorageMappingTableData
{
	unsigned int		uiKey;
	unsigned char		byVarType;
	unsigned char		byCategoryType;
	char				acKeyName[dNTL_STORAGE_MAX_KEY_NAME_LENGTH];
	
	// Changes can only be made from the following
	NtlStorageUnitCheckFunc		ntlStorageFunc;		///< Check function pointer
	char						acDefaultValue[dNTL_STORAGE_MAX_VALUE_LENGTH];
};

// NtlStorage Bool Define
#define dSTORAGE_TRUE		"true"
#define dSTORAGE_FALSE		"false"

// Scouter Measurement Information
typedef struct _SMemoryScouterData
{
	_SMemoryScouterData::_SMemoryScouterData(VOID)
	{
		uiPower = 0;
	}

	unsigned int uiPower;				// combat power

}SMemoryScouterData;

// Quest Information
typedef struct _SMemoryQuestData
{
	_SMemoryQuestData::_SMemoryQuestData(VOID)
	{
		bIsNew = FALSE;
		bIsIndicated = FALSE;
	}

	RwBool	bIsNew;
	RwBool	bIsIndicated;
}SMemoryQuestData;

// Key ID Defines
// Rule: The KEY ID is incremented in sequence or greater than the last Defined ID.
// Do not add inserts in the middle of an absolute deletion. DEFINE, which is not used, is just a comment.
//----------------------------------------------------------------------------------------

#define dSTORAGE_CATEGORY						0

// Account
#define dSTORAGE_ACCOUNT_LASTCHAR				1000		// Last played character name

// Graphic
#define dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH		2000		// RESOLUTION WIDTH
#define dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT		2001
#define dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH		2002		// color
#define dSTORAGE_GRAPHIC_GAMMA					2003		// luminosity
#define dSTORAGE_GRAPHIC_WINDOW_MODE			2004		// windowed mode
#define dSTORAGE_GRAPHIC_PRESET					2005		// Preset levels 0~3, 4 = user setting
#define dSTORAGE_GRAPHIC_TERRAIN_RANGE			2006		// terrain view distance
#define dSTORAGE_GRAPHIC_OBJECT_RANGE			2007		// object viewing distance
#define dSTORAGE_GRAPHIC_TERRAIN_SHADOW			2010		// terrain shadow
#define dSTORAGE_GRAPHIC_WATER_EFFECT			2011		// water effect
#define dSTORAGE_GRAPHIC_CHARACTER_RANGE		2012		// Character viewing distance
#define dSTORAGE_GRAPHIC_CHARACTER_EFFECT		2013		// Character upgrade effect 
#define dSTORAGE_GRAPHIC_CHARACTER_SHADOW		2014		// character shadow
#define dSTORAGE_GRAPHIC_MAGIC_EFFECT			2015		// Magic detailed effects
#define dSTORAGE_GRAPHIC_WEATHER_EFFECT			2016		// weather effects
#define dSTORAGE_GRAPHIC_TEXTURE_LEVEL			2017		// texture resolution
#define dSTORAGE_GRAPHIC_SHADER_HDR				2018		// HDR effect
#define dSTORAGE_GRAPHIC_CHARACTER_EDGE			2019		// cartoon outline
#define dSTORAGE_GRAPHIC_SHOW_FPS				2020
#define dSTORAGE_GRAPHIC_FPS					2021

// Sound
#define dSTORAGE_SOUND_MAIN_VOLUME				4000	// main volume
#define dSTORAGE_SOUND_BACK_VOLUME				4001	// background sound volume
#define dSTORAGE_SOUND_EFFECT_VOLUME			4002	// sound effect volume
#define dSTORAGE_SOUND_ENV_VOLUME				4003	// environmental sound volume

// Game info
#define dSTORAGE_GAMEINFO_GUILD_EMBLEM			5000	// school emblem
#define dSTORAGE_GAMEINFO_DOJO_MARK				5001	// seal mark
#define dSTORAGE_GAMEINFO_SCOUTER_VISIBLE		5002	// scout sign
#define dSTORAGE_GAMEINFO_ITEM_DROP				5003	// show item drop
// Etc
#define dSTORAGE_ETC_FRIEND_NOTIFY				6000	// Friend connection notification
#define dSTORAGE_ETC_GUILD_MEMBER_NOTIFY		6001	// Student access notification
#define dSTORAGE_ETC_QUESTTEXT_EFFECT			6002	// quest text effect
#define dSTORAGE_ETC_USER_TRADE					6003	// transaction
#define dSTORAGE_ETC_MOVEABLEWINDOW				6004	// Whether the window can be moved
#define dSTORAGE_ETC_MOUSE_DASH					6005	// mouse dash interval
#define dSTORAGE_ETC_MOUSE_FOV					6006
#define dSTORAGE_ETC_MOUSE_VIEW_POINT			6007

// Chatting
#define dSTORAGE_CHAT_INPUTMODE					7000	// Chat window input mode
#define dSTORAGE_CHAT_WINDOW_LOCK				7001	// Lock chat window
#define dSTORAGE_CHAT_MOUSE_SCROLL				7002	// mouse scroll
#define dSTORAGE_CHAT_EXTEND_WINDOW				7003	// Extended chat windo
#define dSTORAGE_CHAT_WINDOW_OPACITY			7004	// window transparency
#define dSTORAGE_CHAT_BASIC_NORMAL				7005	// Default window: General conversation
#define dSTORAGE_CHAT_BASIC_WISPHER				7006	// Default window: Whisper
#define dSTORAGE_CHAT_BASIC_PARTY				7007	// Default Window: Party
#define dSTORAGE_CHAT_BASIC_GUILD				7008	// Default window: Yupa
#define dSTORAGE_CHAT_BASIC_TRADE				7009	// Primary Window: Transactions
#define dSTORAGE_CHAT_BASIC_SHOUT				7010	// Default Window: Shout
#define dSTORAGE_CHAT_BASIC_SYSTEM				7011	// Default Windows: System
#define dSTORAGE_CHAT_EXTEND_NORMAL				7012	// Extension window: General conversation
#define dSTORAGE_CHAT_EXTEND_WISPHER			7013	// Extended Window: Whisper
#define dSTORAGE_CHAT_EXTEND_PARTY				7014	// Extended Windows: Party
#define dSTORAGE_CHAT_EXTEND_GUILD				7015	// Extension Window: Yupa
#define dSTORAGE_CHAT_EXTEND_TRADE				7016	// Extended Window: Transactions
#define dSTORAGE_CHAT_EXTEND_SHOUT				7017	// Extended Window: Shout
#define dSTORAGE_CHAT_EXTEND_SYSTEM				7018	// Extended Windows: System
#define dSTORAGE_CHAT_BASIC_FIND_PARTY			7019
#define dSTORAGE_CHAT_EXTEND_FIND_PARTY			7020
#define dSTORAGE_CHAT_EXTEND2_NORMAL			7021
#define dSTORAGE_CHAT_EXTEND2_WISPHER			7022
#define dSTORAGE_CHAT_EXTEND2_PARTY				7023
#define dSTORAGE_CHAT_EXTEND2_GUILD				7024
#define dSTORAGE_CHAT_EXTEND2_TRADE				7025
#define dSTORAGE_CHAT_EXTEND2_SHOUT				7026
#define dSTORAGE_CHAT_EXTEND2_SYSTEM			7027
#define dSTORAGE_CHAT_EXTEND2_FIND_PARTY		7028

#define dSTORAGE_CHAT_EXTEND2_WINDOW			7029

// Scouter
#define dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO	6000
#define dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO		6001
#define dSTORAGE_SCOUTER_INFO_TBLID						6002
#define dSTORAGE_SCOUTER_INFO_POWER						6003

// Quest Indicator
#define	dSTORAGE_QUEST_MEMORY_INFO						7000
#define dSTORAGE_QUEST_INFO_ID							7001
#define dSTORAGE_QUEST_INFO_NEW							7002
#define dSTORAGE_QUEST_INFO_INDICATE					7003

// Character Info
#define dSTORAGE_ETC_EX_QUICKSLOT1						8000	// Additional quick slot 1
#define dSTORAGE_ETC_EX_QUICKSLOT2						8001	// Additional quick slot 2
#define dSTORAGE_ETC_QUICKSLOT_LOCK						8002	// Lock quick slot

#define dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME		9000	// show character name in discord 

#define dSTORAGE_CHARACTER_SCOUTER_ON					9010	// scout status

#define dSTORAGE_SYSTEM_ETC_DISCORD						10000	// Enable/disable discorrd
#define dSTORAGE_SYSTEM_ETC_CONTRACT					10001	// contract/game policy

#define dSTORAGE_EOF									0xFFFFFFFF

#endif//__NTL_STORAGE_DEFINE_H__