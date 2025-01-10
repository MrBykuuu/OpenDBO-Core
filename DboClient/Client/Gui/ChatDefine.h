/******************************************************************************
* File			: ChatDefine.h
* Author		: Hong SungBock
* Copyright		: (?)NTL
* Date			: 2008. 7. 2
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once


#define CHATGUI_COLOR_GLOBAL				RGB(255,255,255)
#define CHATGUI_COLOR_WHISPER				RGB(107,246,169)
#define CHATGUI_COLOR_PARTY					RGB(99,227,255)
#define CHATGUI_COLOR_GUILD					RGB(255,156,90)
#define CHATGUI_COLOR_SHOUT					RGB(255,219,71)
#define CHATGUI_COLOR_SYSTEM_WARN			RGB(255,119,153)
#define CHATGUI_COLOR_SYSTEM_NOTICE			RGB(213,255,83)
#define CHATGUI_COLOR_TRADE					RGB(231,159,255)
#define CHATGUI_COLOR_FIND_PARTY			RGB(179, 102, 255)
#define CHATGUI_COLOR_RAID					RGB(255, 140, 102)

#define CHAT_DISP_MINIMUM_WIDTH				290
#define CHAT_DISP_MINIMUM_HEIGHT			95
#define CHAT_DISP_MAXIMUM_WIDTH				512		// Should not be larger than the width of outputDisplay of ChatDisplay.frm
#define CHAT_DISP_MAXIMUM_HEIGHT			512		// Must not be larger than the height of outputDisplay in ChatDisplay.frm

#define CHAT_DISP_LINE_GAP					2
#define CHAT_DISP_MAX_LINE					50

enum eChatType
{
	CHAT_TYPE_GENERAL,
	CHAT_TYPE_TRADE,
	CHAT_TYPE_FIND_PARTY,
	CHAT_TYPE_GUILD,
	CHAT_TYPE_PARTY,
	CHAT_TYPE_WHISPER,
	CHAT_TYPE_SHOUT,
	//	CHAT_TYPE_RAID,

	NUM_NET_CHAT_TYPE,

	// HelpText by user command. It has nothing to do with server messages.
	CHAT_TYPE_SYSTEM = NUM_NET_CHAT_TYPE,
	CHAT_TYPE_NOTIFY,

	CHAT_TYPE_COMBAT = 10,

	NUM_ALL_CHAT_TYPE,

	CHAT_SHORTCUT,							// Command

	INVALID_CHAT_TYPE
};


class CChatDisplayGui;

typedef std::list<CChatDisplayGui*>					LIST_NOTIFY;
typedef std::list<CChatDisplayGui*>::iterator		ITER_NOTIFY;