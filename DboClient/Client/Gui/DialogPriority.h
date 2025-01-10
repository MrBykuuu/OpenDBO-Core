/******************************************************************************
*File           : DialogPriority.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : February 12, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Defines the rendering priority of a dialog
*                 Larger permutations come first
*                 Range: 0 ~ 65534
*
*Leave the priorities wide apart so that additional priorities can be added in the future.
*Make it easy
*****************************************************************************/

#pragma once


#define dDIALOGPRIORITY_FLASH_PROLOG									58000
#define dDIALOGPRIORITY_POPUP_MSGBOX									56000
#define dDIALOGPRIORITY_MSGBOX											55000
#define dDIALOGPRIORITY_CHANNEL_CHANGE									54000
#define dDIALOGPRIORITY_ESCMENU											53000
#define dDIALOGPRIORITY_NOTIFY											50000

#define dDIALOGPRIORITY_BROADCAST_EMERGENCY								49500

#define dDIALOGPRIORITY_HLSHOPBUY										49101
#define dDIALOGPRIORITY_HLSHOP											49100

#define dDIALOGPRIORITY_CINEMATIC										49000

#define dDIALOGPRIORITY_BROADCAST_TS									48500

#define dDIALOGPRIORITY_RANKBOARD										48000
#define dDIALOGPRIORITY_WORLDMAP_QUEST_SEARCH							47001
#define dDIALOGPRIORITY_WORLDMAP										47000

#define dDIALOGPRIORITY_TB_TOURNEMENT									46600
#define dDIALOGPRIORITY_TB_REQUEST										46500
#define dDIALOGPRIORITY_TB_NEWS											46400

#define dDIALOGPRIORITY_RESULT_NARRATION								46300
#define dDIALOGPRIORITY_RESULT											46200

#define dDIALOGPRIORITY_CALC_POPUP										46000	// calculating machine

#define dDIALOGPRIORITY_PRIVATE_BUSINESS_PRICE							45100	// Negotiated price adjustment window

#define dDIALOGPRIORITY_EFDIALOG										45000	// Entire Focusing Dialog
#define dDIALOGPRIORITY_EFDIALOG_BACKGROUND								44999	// Entire Focusing Dialog backgound

#define dDIALOGPRIORITY_TBGAMBLERESULT									40000	// World¡¯s Best Martial Arts Gambling Result Window

#define dDIALOGPRIORITY_PRIVATE_BUSINESS								35000	// bargaining window

#define dDIALOGPRIORITY_RPBONUS											34000
#define dDIALOGPRIORITY_SKILLABILITY_SET								33500	///<Skill ability designation window

#define dDIALOGPRIORITY_MANIA_TIME_SET									33250	///< Game Mania Time Reset Window

#define dDIALOGPRIORITY_CHATTING_OPTION									33155

#define dDIALOGPRIORITY_TUTORIAL_FOCUS									33100

#define dDIALOGPRIORITY_MAINMENU										33000

#define dDIALOGPRIORITY_INVENTORY										32995

#define dDIALOGPRIORITY_PORTAL											32990

#define dDIALOGPRIORITY_DEFAULT											32767

#define dDIALOGPRIORITY_QUICKSLOT										32567

#define dDIALOGPRIORITY_PARTY_MENU										32400

#define dDIALOGPRIORITY_SUBGAUGE										32100
#define dDIALOGPRIORITY_CHATTING										32000

#define dDIALOGPRIORITY_SIDEICON										31000

#define dDIALOGPRIORITY_SIDE_DIALOG										400

#define dDIALOGPRIORITY_BROADCAST_MININARRATION							350

#define dDIALOGPRIORITY_CHAT_BALLOON									300

#define dDIALOGPRIORITY_DROP_ITEM_INFO									250