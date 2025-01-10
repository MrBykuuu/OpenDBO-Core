 /*****************************************************************************
 *
 * File			: NtlActionMapDef.h
 * Author		: Haesung, Cho
 * Copyright	: (аж)NTL
 * Date			: 2008. 1. 15	
 * Abstract		: Simulation ActionMap definition
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_ACTIONMAP_DEF_H__
#define __NTL_ACTIONMAP_DEF_H__

///////////////////////////////////////////////////////////////////////////////
// NTL Key Definition
///////////////////////////////////////////////////////////////////////////////
#define NTL_KEY_BACKSPACE	   0x08
#define NTL_KEY_TAB			   0x09
#define NTL_KEY_RETURN		   0x0D
#define NTL_KEY_NUMPAD0        0x60
#define NTL_KEY_NUMPAD1        0x61
#define NTL_KEY_NUMPAD2        0x62
#define NTL_KEY_NUMPAD3        0x63
#define NTL_KEY_NUMPAD4        0x64
#define NTL_KEY_NUMPAD5        0x65
#define NTL_KEY_NUMPAD6        0x66
#define NTL_KEY_NUMPAD7        0x67
#define NTL_KEY_NUMPAD8        0x68
#define NTL_KEY_NUMPAD9        0x69
#define NTL_KEY_MULTIPLY       0x6A
#define NTL_KEY_ADD            0x6B
#define NTL_KEY_SEPARATOR      0x6C
#define NTL_KEY_SUBTRACT       0x6D
#define NTL_KEY_DECIMAL        0x6E
#define NTL_KEY_DIVIDE         0x6F
#define NTL_KEY_F1             0x70
#define NTL_KEY_F2             0x71
#define NTL_KEY_F3             0x72
#define NTL_KEY_F4             0x73
#define NTL_KEY_F5             0x74
#define NTL_KEY_F6             0x75
#define NTL_KEY_F7             0x76
#define NTL_KEY_F8             0x77
#define NTL_KEY_F9             0x78
#define NTL_KEY_F10            0x79
#define NTL_KEY_F11            0x7A
#define NTL_KEY_F12            0x7B

#define NTL_KEY_SHIFT          0x10
#define NTL_KEY_CONTROL        0x11
#define NTL_KEY_MENU           0x12
#define NTL_KEY_PAUSE          0x13
#define NTL_KEY_CAPITAL        0x14
#define NTL_KEY_TOCHINESE	   0x19
#define NTL_KEY_ESCAPE         0x1B
#define NTL_KEY_CONVERT        0x1C
#define NTL_KEY_NONCONVERT     0x1D
#define NTL_KEY_ACCEPT         0x1E
#define NTL_KEY_MODECHANGE     0x1F
#define NTL_KEY_SPACE          0x20
#define NTL_KEY_PRIOR          0x21
#define NTL_KEY_NEXT           0x22
#define NTL_KEY_END            0x23
#define NTL_KEY_HOME           0x24
#define NTL_KEY_LEFT           0x25
#define NTL_KEY_UP             0x26
#define NTL_KEY_RIGHT          0x27
#define NTL_KEY_DOWN           0x28
#define NTL_KEY_SELECT         0x29
#define NTL_KEY_PRINT          0x2A
#define NTL_KEY_EXECUTE        0x2B
#define NTL_KEY_SNAPSHOT       0x2C
#define NTL_KEY_INSERT         0x2D
#define NTL_KEY_DELETE         0x2E
#define NTL_KEY_HELP           0x2F
#define NTL_KEY_LEFTWINDOW	   0x5b
#define NTL_KEY_RIGHTWINDOW    0x5c
#define NTL_KEY_NUMLOCK        0x90
#define NTL_KEY_SCROLL         0x91
#define NTL_KEY_COMMA		   0xBC		// ','
#define NTL_KEY_DASH		   0xBD
#define NTL_KEY_PERIOD		   0xBE		// '.'
#define NTL_KEY_OEM_2		   0xBF		// '/'
#define NTL_KEY_EQUAL		   0xBB
#define NTL_KEY_WAVE		   0xC0		// '~'
#define NTL_KEY_WON			   0xDC		// '\'
#define NTL_KEY_LBRACKET		0xDB	// '['
#define NTL_KEY_RBRACKET		0xDD	// ']'

///////////////////////////////////////////////////////////////////////////////
// DragonBall Online Action
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// ActionMap Key Status
///////////////////////////////////////////////////////////////////////////////

#define NTL_ALT_BIT   0x0100
#define NTL_CTRL_BIT  0x0200			// combination key
#define NTL_SHIFT_BIT 0x0400

#define NTL_KEY_UPDATE 0x4000
#define NTL_KEY_EMPTY 0x8000

///////////////////////////////////////////////////////////////////////////////
// ETC
///////////////////////////////////////////////////////////////////////////////

enum eACTION
{
	// Avatar behavior from 0 to 19
	ACTION_AVATAR_FORWARD = 0,		///< forward
	ACTION_AVATAR_BACKWARD,			///< Reverse
	ACTION_AVATAR_LEFTTURN,			///< Turn left
	ACTION_AVATAR_RIGHTTURN,		///< Turn right
	ACTION_AVATAR_SITDOWN,			///< sit
	ACTION_AVATAR_JUMP,				///< jump
	ACTION_AVATAR_AUTORUN,			///< Auto run
	ACTION_AVATAR_SCOUTER,			///< Using Scouter
	ACTION_AVATAR_CHARGE,			///< Collect energy
	ACTION_AVATAR_LOOTING,			///< Pick up items
	ACTION_AVATAR_BLOCKING,			///< Avatar blocking
	ACTION_AVATAR_LEFTSIDE,
	ACTION_AVATAR_RIGHTSIDE,
	ACTION_AVATAR_FREEDOWN,

	// Target actions from 20 to 39
	ACTION_TARGET_SELF = 20,		///< Choose yourself
	ACTION_TARGET_1STPARTY,			///< Select party member 1
	ACTION_TARGET_2NDPARTY,			///< Select party member 2
	ACTION_TARGET_3RDPARTY,			///< Select party member 3
	ACTION_TARGET_4THPARTY,			///< Select party member 4
	ACTION_TARGET_MEMBER,
	ACTION_TARGET_AUTO,				///< Select a nearby enemy
	ACTION_TARGET_AUTOATK,			///<Auto attack
	ACTION_TARGET_AUTOFOLLOW,		///< Auto follow
	ACTION_TARGET_MARKING_1,		///< Specify shared target 1
	ACTION_TARGET_MARKING_2,		///< Specify shared target 2
	ACTION_TARGET_MARKING_3,		///< Specify sharing target 3
	ACTION_TARGET_MARKING_4,		///< Specify shared target 4
	ACTION_TARGET_MARKING_5,		///< Specify share target 5
	ACTION_TARGET_SELECTING_1,		///< Select sharing target 1
	ACTION_TARGET_SELECTING_2,		///< Select sharing target 2
	ACTION_TARGET_SELECTING_3,		///< Select sharing target 3
	ACTION_TARGET_SELECTING_4,		///< Select sharing target 4
	ACTION_TARGET_SELECTING_5,		///< Select sharing target 5
	ACTION_TARGET_AUTOTARGET_PARTY,	///< Automatic selection of party members

	// Quick actions from 40 to 59
	ACTION_QUICK_1 = 40,			///< Quick slot no. 1
	ACTION_QUICK_2,					///< Quick Slot No. 2
	ACTION_QUICK_3,					///< Quick slot 3
	ACTION_QUICK_4,					///< Quick slot 4
	ACTION_QUICK_5,					///< Quick slot 5
	ACTION_QUICK_6,					///< Quick slot 6
	ACTION_QUICK_7,					///< Quick slot 7
	ACTION_QUICK_8,					///< Quick slot 8
	ACTION_QUICK_9,					///< Quick slot 9
	ACTION_QUICK_0,					///< Quick slot 0
	ACTION_QUICK_MIN,				///< Quick Slot-Burn
	ACTION_QUICK_PLUS,				///< Quick Slot + Burn
	ACTION_QUICK_PREV,				///< Previous Quick Slot
	ACTION_QUICK_NEXT,				///< Next quick slot
	ACTION_QUICK_2NDCAP,			///< Capsule Kit No. 2
	ACTION_QUICK_3RDCAP,			///< Capsule Kit No. 3
	ACTION_QUICK_4THCAP,			///< Capsule Kit No. 4
	ACTION_QUICK_5THCAP,			///< No. 5 Capsule Kit

	// Windows behavior from 60 to 79
	ACTION_WINDOW_PROFILE = 60,			///< Detailed Information Window
	ACTION_WINDOW_SKILL,			///< Skill information window
	ACTION_WINDOW_QUEST,			///< Quest Window
	ACTION_WINDOW_MINIMAP,			///< Minimap
	ACTION_WINDOW_WORLDMAP,			///< World Map
	ACTION_WINDOW_MAINCAP,			///< Main Capsule Kit
	ACTION_WINDOW_PARTY,			///< Party information ///< Disappeared during planning stage
	ACTION_WINDOW_GUILD,			///< Guild information
	ACTION_WINDOW_FRIEND,			///< Friend information
	ACTION_WINDOW_OPTION,			///< Options
	ACTION_WINDOW_HELP,				///< Help
	ACTION_WINDOW_MAIN,				///< Main menu
	ACTION_WINDOW_RANKBOARD,		///< Rank Board

	ACTION_WINDOW_HOIPOIMIX,
	ACTION_WINDOW_MASCOT,
	ACTION_WINDOW_DWC,

	// Reserve empty seats from 80 to 159
	ACTION_QUICK_1_EX = 80,				///< Quick slot first expansion number 1
	ACTION_QUICK_2_EX,					///< Quick Slot No. 2
	ACTION_QUICK_3_EX,					///< Quick slot 3
	ACTION_QUICK_4_EX,					///< Quick slot 4
	ACTION_QUICK_5_EX,					///< Quick slot 5
	ACTION_QUICK_6_EX,					///< Quick slot 6
	ACTION_QUICK_7_EX,					///< Quick slot 7
	ACTION_QUICK_8_EX,					///< Quick slot 8
	ACTION_QUICK_9_EX,					///< Quick slot 9
	ACTION_QUICK_0_EX,					///< Quick slot 0
	ACTION_QUICK_MIN_EX,				///< Quick Slot-Burn
	ACTION_QUICK_PLUS_EX,				///< Quick Slot + Burn
	ACTION_QUICK_1_EX2,					///< Quick slot first expansion number 1
	ACTION_QUICK_2_EX2,					///< Quick Slot No. 2
	ACTION_QUICK_3_EX2,					///< Quick slot 3
	ACTION_QUICK_4_EX2,					///< Quick slot 4
	ACTION_QUICK_5_EX2,					///< Quick slot 5
	ACTION_QUICK_6_EX2,					///< Quick slot 6
	ACTION_QUICK_7_EX2,					///< Quick slot 7
	ACTION_QUICK_8_EX2,					///< Quick slot 8
	ACTION_QUICK_9_EX2,					///< Quick slot 9
	ACTION_QUICK_0_EX2,					///< Quick slot 0
	ACTION_QUICK_MIN_EX2,				///< Quick Slot-Burn
	ACTION_QUICK_PLUS_EX2,				///< Quick Slot + Burn
	ACTION_QUICK_SUMMON,
	ACTION_QUICK_SUMMON2,

	// Minimap related from 160 to 169
	ACTION_MINIMAP_ZOOMOUT = 160,			///< Zoom out minimap
	ACTION_MINIMAP_ZOOMIN,					///< Enlarge minimap

	// 170 to 179 related to chat window
	ACTION_CHAT_PGUP = 170,				///< Previous page of chat window
	ACTION_CHAT_PGDN,					///<Chat window next page

	// Related to all commands from 180 to 199
	ACTION_GLOBAL_CANCLE = 180,			///< Cancel (Targeting, Windows, Main Menu)
	ACTION_GLOBAL_SNAPSHOT,				///< Screenshot
	ACTION_GLOBAL_CHAT,					///< Chat input
	ACTION_GLOBAL_RENDERGUI,

	ACTION_NUMS = 255,
	ACTION_INVALID = ACTION_NUMS, 				///< Last number of action ID
};

#define dNTL_KEY_EMPTY		0x8000		// There is no key defined.
#define dNTL_KEY_INVALID	0xffff		// The key value is INVALID.

/**
*\breif NTL key type
* NTL Define:
* 0 0 0 0 0 0 0 0    00000000
* E           C A    Key Data
*Upper 8 bits: C = CTRL, A = ALT
*Lower 8 bits: Key Data
*/
typedef union _uNtlKey
{
	unsigned short key;			// 16Bit

	/**
	*A common structure that divides the key into high-order bits and low-order bits.
	*/
	struct _BIT
	{
		unsigned short ori:8;	// Used for original key
		unsigned short bit8:1;	// Bit flags used as combination keys
		unsigned short bit7:1;
		unsigned short bit6:1;
		unsigned short bit5:1;
		unsigned short bit4:1;
		unsigned short bit3:1;
		unsigned short bit2:1;
		unsigned short bit1:1;
	} BIT;
} uNtlKey;

/**
* \brief Data type that stores Action and Key of ACTIONMAP
* KEY		: unsigned short
* DATA		: unsigned short 
*/
typedef std::map< unsigned short, unsigned short > ACTIONMAP;

/**
* \brief Vector for storing actions
*/
typedef std::vector< unsigned short > ACTIONVEC;

/**
* \brief Result of setting action map
*/
enum eACTIONMAP_SET_RESULT
{
	SET_RESULT_FAILED = 0,					// unknown error
	SET_RESULT_FIXED_KEY,					// The key is fixed and cannot be changed.
	SET_RESULT_FIXED_ACTION,				// An action is an action that cannot be changed.
	SET_RESULT_NOT_COMBINE_ACTION,			// This is an action that cannot use combination keys.
	SET_RESULT_ALREADY_KEY,					// Key already exists (existing key will be lost)
	SET_RESULT_ALREADY_ACTION,				// Action already exists (existing Action will be lost)
	SET_RESULT_SAME_KEY,					// The same key was set
	SET_RESULT_NEGATIVE_MODE,				// impossible mode
	SET_RESULT_OK,							// Succeeded normally
	SET_RESULT_NUMS,						// RESULT Quantity
	SET_RESULT_INVALID = SET_RESULT_NUMS
};

/**
* \brief Vector containing error code of action map
*/
typedef std::vector< BYTE > RESULTVEC;

/**
* \brief Action map properties (both KEY and ACTION possible)
* May be added.
*/
// #define TYPE_NONE 0x00000000 //No properties.
#define TYPE_FIXED			0x00000001		// Fixed type, cannot be changed.
#define TYPE_COMBINE		0x00000002		// Whether or not it is used as a combination key
#define TYPE_EXCLUSIVE		0x00000004		// Actions that cannot be used with combination keys

/**
* \brief Map storing key or action properties
*/
typedef std::map< unsigned short, int >		TYPEMAP;

/**
* \brief Map containing the names of keys
*/
typedef std::map<unsigned short, std::wstring> KEYNAMEMAP;

/**
* \brief Lists the modes that use ACTIONMAP.
*/
enum eACTIONMAP_MODE
{
	ACTIONMAP_MODE_USERSETTING = 0,
	ACTIONMAP_MODE_DEFAULT,

	ACTIONMAP_MODE_NUMS,
	ACTIONMAP_MODE_INVALID = ACTIONMAP_MODE_NUMS
};

// Count Define used when keys should be ignored indefinitely
#define ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE	0xFF

#endif//__NTL_ACTIONMAP_DEF_H__