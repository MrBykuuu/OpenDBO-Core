/******************************************************************************
* File			: GUISoundDefine.h
* Author		: Hong SungBock
* Copyright		: (?)NTL
* Date			: 2007. 2. 21
* Abstract		: 
*****************************************************************************
* Desc			: Defines the name of the sound file used in the system.
*****************************************************************************/

#pragma once

#include "NtlSoundManager.h"


#define GSD_SYSTEM_ICON_SET					"System\\SYSTEM_ICON_SET.wav"
///< Skill Icon, etc. to put in the Quick Slot. "Patch"

#define GSD_SYSTEM_ICON_SELECT				"System\\SYSTEM_ICON_SELECT.wav"
///< The sound when I picked Icon.

#define GSD_SYSTEM_ICON_WASTE				"System\\SYSTEM_ICON_WASTE.wav"	
///< The sound made when an Icon is thrown away from the Quick Slot. ¡°Kasak¡±. (Image of paper being put into the trash can.)

#define GSD_SYSTEM_BUTTON_CLICK				"System\\SYSTEM_BUTTON_CLICK.wav"
///< Click sound when the Button is pressed.¡¸Kachi¡¹.

#define GSD_SYSTEM_BUTTON_CANCEL			"System\\SYSTEM_BUTTON_CANCEL.wav"
///<Sound when Cancel is pressed.¡¸Tut¡¹.

#define GSD_SYSTEM_BUTTON_FOCUS				"System\\SYSTEM_Button_Focus.wav"
///< Sound when the button is focused.

#define GSD_SYSTEM_BUTTON_DISABLE			"System\\SYSTEM_Button_Disable_Alarm.wav"
///< Sound when the Disable button is clicked.

#define GSD_SYSTEM_WINDOW_OPEN				"System\\SYSTEM_WINDOW_OPEN.wav"
///< The sound when a window opens. ¡°Sun¡±.

#define GSD_SYSTEM_WINDOW_CLOSE				"System\\SYSTEM_WINDOW_CLOSE.wav"
///< The sound when a window closes.¡¸Shock¡¹.

#define GSD_SYSTEM_ITEM_REPAIR				"System\\SYSTEM_ITEM_REPAIR.wav"
///< Sound when repairing an item. ¡°Can¡±. (With the image of a blacksmith shop.)

#define GSD_SYSTEM_WINDOW_ALERT				"System\\SYSTEM_WINDOW_ALART.wav"
///< The sound when the warning window appears.¡¸Gon¡¹.

#define GSD_SYSTEM_NOTICE					"System\\SYSTEM_NOTICE.wav"
///< The sound made when a message indicating a good thing appears on the screen.¡¸Pin!¡¹.

#define GSD_SYSTEM_ITEM_SPEND				"System\\SYSTEM_ITEM_SPEND.wav"
///< Sound when deciding to use a consumption item. ¡°For one.¡±

#define GSD_SYSTEM_ITEM_EQUIP				"System\\SYSTEM_ITEM_EQUIP.wav"
///< Sound when equipping an equipment item. ¡°Sasak¡±. (Image of sleeves going through clothes.)

#define GSD_SYSTEM_ITEM_TAKEOFF				"System\\SYSTEM_ITEM_TAKEOFF.wav"
///< Sound when releasing an equipment item. ¡°Sock¡±. (Image of sleeves going through clothes.)

#define GSD_SYSTEM_TAB_CLICK				"System\\SYSTEM_TAB_CLICK.wav"
///< Sound when selecting Tab. ¡°Sak¡±. (Image of paper being replaced.)

//******************************************Need to talk..... ?
#define GSD_SYSTEM_POPUPWINDOW_OPEN			"System\\SYSTEM_POPWIN_OPEN.wav"
///< Sound when PopupWindow opens

#define GSD_SYSTEM_POPUPWINDOW_CLOSE		"System\\SYSTEM_POPWIN_CLOSE.wav"
///< Sound when PopWindow is closed

#define GSD_SYSTEM_TAB_FOCUS				"System\\SYSTEM_TAB_FOCUS.wav"
//< Sound when the mouse moves up the tab

#define GSD_SYSTEM_TREE_OPEN				"System\\SYSTEM_TREE_OPEN.wav"
//< When the tree unfolds

#define GSD_SYSTEM_TREE_CLOSE				"System\\SYSTEM_TREE_CLOSE.wav"
//< When the tree is closed

#define GSD_SYSTEM_TUTORIAL_MSGBOX			"System\\TUTORIAL_MSGBOX.wav"
//< tutorial msgbox 

#define GSD_SYSTEM_TUTORIAL_OK				"System\\TUTORIAL_OK.wav"

#define GSD_SYSTEM_CHECKBOX					"System\\GENERAL_CHKBOX.wav"
//< When the checkbox is checked

//******************************************complete

#define GSD_SYSTEM_MESSAGEBOX				"System\\SYSTEM_MSGBOX.wav"
//#define GSD_SYSTEM_MESSAGEBOX ""
/////< The sound when MES Box comes out.
//
#define GSD_SYSTEM_ENEMY_SELECT				"System\\SYSTEM_ENEMY_FOCUS.wav"	
////<When choosing an enemy
//
#define GSD_SYSTEM_ENEMY_FOUSE				"System\\SYSTEM_ENEMY_SELECT.wav"	
////<When hovering the mouse over an enemy
//
#define GSD_SYSTEM_CHARACTER_ROTATE			"System\\SYSTEM_CHAR_ROTATE.wav"
/////< Rotate Character
//
#define GSD_SYSTEM_PARTS_EQUIP				"System\\SYSTEM_EQUIP.wav"	
////< When equipping scout parts
//
#define GSD_SYSTEM_ABILITY_UI_OPEN			"System\\SYSTEM_SKILL_OPEN.wav"
////< When the skill ability UI appears
//
#define GSD_SYSTEM_GUARD_GAGE				"System\\SYSTEM_GUARD_GAGEDOWN.wav"
////< When the guard gauge decreases
//
#define GSD_SYSTEM_GUARD_GAGE_END			"System\\SYSTEM_GUARD_GAGEDOWN_END.wav"
////< When the guard gauge decreases
//

#define GSD_SYSTEM_PUBLIC_NOTIFY			"System\\SYSTEM_NOTICE.wav"	
///< Alarm sound when a notice appears

#define GSD_SYSTEM_ACTION_NOTIFY			"System\\SYSTEM_NOTICE_ACTION.wav"
/////< Notification sound that appears depending on the result of an action
//
#define GSD_SYSTEM_WARNING_NOTIFY			"System\\SYSTEM_NOTICE_WARNING.wav"
/////< Notification sound when you do something you shouldn't do ps. Class is CCautionNotifyGui
//
#define GSD_SYSTEM_PARTY_NOTIFY				"System\\SYSTEM_NOTICE_PARTY.wav"
/////< Notification sound when calling a user from a party or guild
//
#define GSD_SYSTEM_TERRAIN_NOTIFY			"System\\SYSTEM_NOTICE_TERRAIN.wav"	

/////< When you go to a new terrain, a notification sound informs you of the terrain name.
//
#define GSD_SYSTEM_ITEM_LIGHT_ON			"System\\SYSTEM_ITEM_UPGRADE.wav"

////< When the color changes to suit the attribute (when moving to an upgradeable space)

#define GSD_SYSTEM_KEY_DOWN					"System\\GENERAL_TEXTINPUT.wav"
//< sound of typing

//
#define GSD_SYSTEM_DRAGOBALL_UI_OPEN		"System\\SYSTEM_DB_OPEN.wav"	

////< When the Dragon Ball altar opens
//
#define GSD_SYSTEM_DRAGOBALL_UI_CLOSE		"System\\SYSTEM_DB_CLOSE.wav"	
	
////< When the Dragon Ball altar closes
//
#define GSD_SYSTEM_POTAL_ICON_MOUSE_OVER	"System\\SYSTEM_PORTAL_MOUSE_OVER.wav"

#define GSD_SYSTEM_MOUSE_CLICK_HV			"System\\SYSTEM_MOUSE_CLICK_HV.wav"

////< When you hover the mouse over the icon where you can enter the portal
//

#define GSD_SYSTEM_DRAGOBALL_REGISTRATION	"System\\SYSTEM_DB_REG.wav"	
//< When the dragon ball gets stuck in the stone

#define GSD_SYSTEM_DRAGOBALL_UI_LIGHT		"System\\SYSTEM_DB_LIGHT.wav"	

////< When the lights turn on at the Dragon Ball altar
//
//
#define GSD_SYSTEM_NEW_MAIL_ALRAM			"System\\SYSTEM_NEW_MAIL.wav"	

////< When a new letter arrives
//
#define GSD_SYSTEM_MAIL_SEND				"System\\SYSTEM_SEND_MAIL.wav"	

////< When sending an email
//
#define GSD_SYSTEM_SCOUTER_LOOP				"System\\GENERAL_Scouter_Loop.wav"
//< Scout measurement certificate

#define GSD_SYSTEM_SCOUTER_SCAN				"System\\GENERAL_Scouter_Scan.wav"
//< When setting scout target
//******************************************complete

#define GSD_SYSTEM_USE_MONEY				"System\\SYSTEM_USE_MONEY.wav"
///< Sound when using Money. A pleasant sound, ¡°Chain¡±.

#define GSD_SYSTEM_DROP_NORMAL				"System\\SYSTEM_DROP_NORMAL.wav"
///< Item Drop -The sound of Mob disappearing and Capsule and Zenny falling. A sound that is not too harsh to the ear, ¡°Born¡±.

#define GSD_SYSTEM_DROP_LUCKY				"System\\SYSTEM_DROP_LUCKY.wav"
///< Item Drop -Lucky!! I was lucky enough to have a better Capsule than usual and a sound with more Zenny. A sound that didn't bother my ears at all, "Boyon".

#define GSD_SYSTEM_DROP_DBALL				"System\\SYSTEM_DROP_DBALL.wav"
///< Item Drop -Sound effect for dropping the dragon ball.

#define GSD_SYSTEM_DROP_POWER				"System\\SYSTEM_DROP_POWER.wav"
///< Item Drop -Sound effect for dropping the stone.

#define GSD_SYSTEM_ITEM_GET					"System\\SYSTEM_ITEM_GET.wav"
///< Item Drop -Sound when the dropped item is obtained. ¡°Sun¡±.

#define GSD_SYSTEM_SKILL_LEARN				"BGM\\JINGLE_SKILL_GET.ogg"
///< Sound when learning the skill. A sound like a church bell, ¡°gon¡±.

#define GSD_SYSTEM_SKILL_RESETONE			"System\\SYSTEM_CANCEL_HV.wav"

#define GSD_SYSTEM_SKILL_DISABLE			"System\\SYSTEM_SKILL_DISABLE.wav"
///< Sound when a skill or item cannot be used. A soft, low sound that does not bother the ears, ¡°gon¡±.

#define GSD_SYSTEM_QUEST_GIVE_UP			"System\\SYSTEM_QUEST_GIVE_UP.wav"
///< Sound when giving up a Quest. A regretful sound, ¡°Whoa...¡±.

#define GSD_SYSTEM_TRIGGER_OBJECT_CLICK		"System\\GENERAL_Click_01.wav"
///< Sound made when clicking on the trigger object

#define GSD_JINGLE_QUEST_START				"BGM\\JINGLE_QUEST_START.ogg"
///< A brave feeling that gives you a sense of determination when you start the Quest.

#define GSD_JINGLE_QUEST_FINISH				"BGM\\JINGLE_QUEST_FINISH.ogg"
///< Blessing music that seems to praise you when you finish a quest

#define GSD_JINGLE_LEVEL_UP					"BGM\\JINGLE_LEVEL_UP.ogg"
///< LevelUp

#define GSD_JINGLE_PVP_START				"BGM\\JINGLE_PVP_START.ogg"
///< PvP Start

#define GSD_JINGLE_PVP_END					"BGM\\JINGLE_PVP_END.ogg"
///< PvP End

#define GSD_GET_EXP							"System\\SYSTEM_Exp_Get.wav"
///< Exp Get

#define GSD_QUEST_ITEM_GET					"System\\SYSTEM_Quest_item_Get.wav"
///< Quest Item Get

#define GSD_RP_FULL							"System\\GENERAL_RP_Full.wav"
///< RP Full

#define GSD_DB_SCRAMBLE_START				"System\\SYSTEM_RB_Fight.wav"

#define GSD_SCOUTER_OPEN					"System\\GENERAL_Scouter_On.wav"

#define GSD_SCOUTER_CLOSE					"System\\GENERAL_Scouter_Off_s.wav"

#define GSD_HEART_BEAT						"Syatem\\GENERAL_Radar.wav"	// temporary
///< Repeat when HeartBeat HP decreases

/// Background music when the Dragon God appears
#define GSD_JINGLE_DRAGON                   "BGM\\JINGLE_SHENRON.ogg"

///< Sound effect to alert the user that caution is required
#define GSD_SYSTEM_WINDOW_ALREAT			"System\\SYSTEM_WINDOW_ALART.wav"

///< click click click
#define GSD_GENERAL_CLOCK_SECOND			"System\\GENERAL_Clock_Second.wav"

///< Music when you catch a HERO BOSS in a dungeon
#define GSD_FINISH_HERO_MOB					"BGM\\JINGLE_BOSS_DOWN.ogg"

#define GSD_WAGU_DRAW						"System\\System_WaguCacpsule.wav"

#define GSD_WAGU_FIRST_PRIZE				"System\\General_Fanfare_3.wav"

//////////////////////////////////////////////////////////////////////////
//						Background Mugic								//
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_PVP							"BGM\\BGM_PVP.ogg"

#define GSD_BGM_PRIVATE_RANKBATTLE			"BGM\\BGM_RANK_BATTLE_2m.ogg"
#define GSD_BGM_PARTY_RANKBATTLE			"BGM\\BGM_RANK_BATTLE_3m.ogg"

#define GSD_BGM_MAIN_THEME					"BGM\\BGM_MAIN_THEME.ogg"
#define GSD_BGM_PROLOG						"BGM\\BGM_PROLOGE.ogg"


//////////////////////////////////////////////////////////////////////////
//						Tenkaichi Budokai //
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_TB_STANDBY					"BGM\\BGM_TB_STANDBY.ogg"
#define GSD_BGM_TB_PRIVATE_BATTLE			"BGM\\BGM_TB_MAIN_m3.ogg"
#define GSD_BGM_TB_PARTY_BATTLE				"BGM\\BGM_TB_MAIN_m4.ogg"

#define GSD_TB_TOURNAMENT_START				"BGM\\JINGLE_TB_START.ogg"
#define GSD_TB_ROUND_END					"BGM\\JINGLE_PVP_END.ogg"
#define GSD_TB_MATCH_RESULT					"BGM\\JINGLE_TB_RESULT.ogg"


//////////////////////////////////////////////////////////////////////////
//						Dojo Battle //
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_DOJO_SCRAMBLE_READY			"BGM\\BGM_PROLOGE_B.ogg"
#define GSD_BGM_DOJO_SCRAMBLE_START			"BGM\\BGM_DOJO_BATTLE.ogg"