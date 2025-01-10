//***********************************************************************************
//
//	File		:	NtlMail.h
//
//	Begin		:	2007-08-6
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	john@ntl-inc.com
//
//	Desc		:	
//
//***********************************************************************************


#pragma once
#include "NtlItem.h"
#include "NtlSharedType.h"

const BYTE		MAIL_SEND_REQUIRED_LEVEL = 29;		// Level required to send mail

const DWORD		NTL_MAX_BASIC_MAIL_SEND_ZENNY	= 10;		// Commission

const DWORD		NTL_MAX_ATTACH_MAIL_SEND_ZENNY	= 20;		// Commission

const DWORD		NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS	= 100000000;	// Maximum possible attachment amount

const DWORD		NTL_MAX_MAIL_SLOT_COUNT	= 30;				// Maximum number of mail slots

const DWORD		NTL_MAIL_RELOAD_TIME    = 1 * 60 * 1000;    // Need to adjust DB contents and synchronization time

const DWORD		NTL_MAIL_DISTANCE_CHECK_TIME = 3 * 1000;	// Distance check time to mailbox 

const BYTE		NTL_DEFAULT_MAIL_KEEPING_PERIOD = 10;		// Default mail retention period. Day unit

const DWORD		NTL_MAX_COUNT_OF_MULTI_DEL		= 6;		// Maximum number of multidels possible

const BYTE		NTL_MAX_COUNT_MULTI_MAIL		= 50;		// GM-TOOL Multimail Maximum

const BYTE		NTL_MAX_LOAD_MAIL_PACKET		= 8;
//-----------------------------------------------------------------------------------
// Mail type 
//-----------------------------------------------------------------------------------

enum eMAIL_TYPE
{
	eMAIL_TYPE_NONE,			// When using error

	eMAIL_TYPE_BASIC,			// TEXT ONLY

	eMAIL_TYPE_ITEM,			// Item

	eMAIL_TYPE_ZENNY,			// Zenny

	eMAIL_TYPE_ITEM_ZENNY,		// ITEM + ZENNY

	eMAIL_TYPE_ITEM_ZENNY_REQ,  // ITEM + ZENNY REQUEST

	eMAIL_TYPE_ZENNY_REQ,		// ZENNY REQUEST

	eMAIL_COUNT,				// Use the string for eMAIL_TYPE in GMTool. So we need Count[10/8/2007 SGpro]

};

//-----------------------------------------------------------------------------------
// sender type 
//-----------------------------------------------------------------------------------

enum eMAIL_SENDER_TYPE
{
	eMAIL_SENDER_TYPE_BASIC,		// General user

	eMAIL_SENDER_TYPE_REPLY,		// Reply email

	eMAIL_SENDER_TYPE_RETURN,		// return mail

	eMAIL_SENDER_TYPE_SYSTEM,		// System

	eMAIL_SENDER_TYPE_GM,			// Gm

	eMAIL_SENDER_TYPE_QUEST,

};

struct sMAIL_SLOT
{
	MAILID aMailID[NTL_MAX_MAIL_SLOT_COUNT];
};

struct sMAIL_BRIEF
{
	BYTE	byMailType;
	DWORD	dwZenny;
	bool	bIsRead;
	bool	bIsAccept;
	bool	bIsLock;
	BYTE	bySenderType;
	DBOTIME	endTime;
	DWORD	dwHandle;
};

struct sMAIL_DATA
{
	MAILID				mailID;			// mail id

	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// E mail type

	BYTE 				bySenderType;	// E mail sender type

	DWORD				dwZenny;		// Req or Give Zenny

	BYTE				byExpired;		// Expiration date up to 10 days

	sITEM_DATA			sItemData;		// Item contents	

	sDBO_TIME			tCreateTime;	// Sent date

	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// sent character name

	BYTE				byTextSize;
	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// Email contents

	DBOTIME				endTime;
	DWORD				dwHandle;
};

struct sMAIL_GM_DATA
{
	MAILID				mailID;			// mail id

	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// E mail type

	BYTE 				bySenderType;	// E mail sender type

	DWORD				dwZenny;		// Req or Give Zenny

	BYTE				byExpired;		// Expiration date up to 10 days

	sITEM_DATA			sItemData;		// Item contents	

	sDBO_TIME			tCreateTime;	// Sent date

	CHARACTERID			FromCharID;
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// sent character name

	BYTE				byTextSize;
	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// Email contents

	bool				bIsTemp;	// tempMail ? True

};

struct sMAIL_PROFILE
{
	MAILID				mailID;			// mail id

	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// E mail type

	BYTE 				bySenderType;	// E mail sender type

	DWORD				dwZenny;		// Req or Give Zenny

	BYTE				byExpired;		// Expiration date up to 10 days

	sITEM_PROFILE		sItemProfile;	// Item contents	

	sDBO_TIME			tCreateTime;	// Sent date

	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// sent character name

	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// Email contents

	DBOTIME				endTime;
};
struct sMAIL_NEW_PROFILE
{
	MAILID				mailID;			// mail id

	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// E mail type

	BYTE 				bySenderType;	// E mail sender type

	DWORD				dwZenny;		// Req or Give Zenny

	BYTE				byExpired;		// Expiration date up to 10 days

	sITEM_PROFILE		sItemProfile;	// Item contents	

	sDBO_TIME			tCreateTime;	// Sent date

	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// sent character name

	DBOTIME				endTime;
};

struct sMAIL_NEW_GM_PROFILE
{
	MAILID				mailID;			// mail id

	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// E mail type

	BYTE 				bySenderType;	// E mail sender type

	DWORD				dwZenny;		// Req or Give Zenny

	BYTE				byExpired;		// Expiration date up to 10 days

	sITEM_PROFILE		sItemProfile;	// Item contents	

	sDBO_TIME			tCreateTime;	// Sent date

	CHARACTERID			FromCharID;
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// sent character name

	bool				bIsTemp;	// tempMail ? True

};

struct sMAIL_NEW_BREIF
{
	BYTE				byMailCount; // Total

	BYTE				byUnReadManager;
	BYTE				byUnReadNormal;
};