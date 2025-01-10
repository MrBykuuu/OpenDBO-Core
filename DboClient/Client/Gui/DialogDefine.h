/******************************************************************************
*File           : DialogDefine.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : August 11, 2006
*Abstract		:  
*****************************************************************************
*Desc           : Dialog related definitions
*****************************************************************************/

#pragma once

enum eDialogVisible
{
	DIALOGVISIBLE_FALSE,					///< Closes the dialog.
	DIALOGVISIBLE_TRUE						///< Opens the dialog.
};

// Be careful not to overlap the SideDilaog index
enum eDialogType
{
	DIALOG_FIRST,

	// GM Dialog (not affected by ESC key)

	DIALOG_GM_FIRST = DIALOG_FIRST,
	
	DIALOG_GM_QUICKSLOT = DIALOG_GM_FIRST,

	DIALOG_GM_LAST = DIALOG_GM_QUICKSLOT,


	// Default dialog (default dialog to be displayed during game, not affected by ESC key)

	DIALOG_DEFAULT_FIRST,

	DIALOG_HP = DIALOG_DEFAULT_FIRST,		///< HP Windows
	DIALOG_EXP,								///< EXP Window (Experience Points)
	DIALOG_CHAT,							///< Chat window
	DIALOG_QUICKSLOT,						///< Quick Slot Window	
	DIALOG_BAGSLOT,							///< Capsule slot	
	DIALOG_MINIMAP,							///< Minimap	
	DIALOG_MAINMENUBUTTON,					///< Main menu button
	DIALOG_SIDEICON,						///< side icon
	DIALOG_SIDEDIALOG_MANAGER,				///< Side dialog manager	
	DIALOG_SIDEDIALOG_CONTROLLER,			///< Side dialog controller
	DIALOG_PARTYMENU,						///< Party menu (has the party member window as a child)
	DIALOG_GM_CHATTING,						///< GM Chat
	DIALOG_GROUP_CHAT_MANAGER,				///< Group chat manager
	DIALOG_DICE_MANAGER,					///< Dice Manager
	DIALOG_VEHICLE,							///< Vehicle control panel

	DIALOG_DEFAULT_LAST = DIALOG_VEHICLE,
	

	// Event Dialog (Basically it is not affected by ESC key)

	DIALOG_EVENT_FIRST,

	DIALOG_TARGER_UI = DIALOG_EVENT_FIRST,	///< Target UI //19 in TW client
	DIALOG_SCOUTER_MEASURE,					///< Scouter combat power measurement
	DIALOG_SHOPING_CART,					///< cart in store
	DIALOG_TRADECART,						///< Trade Cart
	DIALOG_PRIVATESHOP_CART,				///< Personal store cart
	DIALOG_NETPYSHOP_TRADE,					///< NetPy Shop Trade Cart
	DIALOG_WAREHOUSE_1,						///< Warehouse 1
	DIALOG_WAREHOUSE_2,						///< Warehouse 2
	DIALOG_WAREHOUSE_3,						///< Warehouse 3
	DIALOG_WAREHOUSE_COMMON,				///< Shared warehouse
	DIALOG_DBC_REWARD,						///< Dragon Ball Reward GUI
	DIALOG_DBC_DRAGON_DIALOG,				///< Dragon God¡¯s conversation speech bubble GUI
	DIALOG_DROPITEM_INFO,					///< Drop item information window
	DIALOG_TMQ_REWARD,						///< Time Machine Quest Reward Window
	DIALOG_BACKBOARD,						///< Backboard window that covers the entire screen	
	DIALOG_FLASH_NOTIFY,					///< Notify notification window using Flash
	DIALOG_INTERFACE_SHAKE,					///< Window to speed up recovery by manipulating the interface at the time of knockdown
    DIALOG_NET_CONNECT_BOX,                 ///< Dialog box that displays the status when server connection fails
    DIALOG_TELECAST,                        ///< Broadcasting window //by daneos: is this required?? 
	DIALOG_TIME_NOTIFY,						///< Time notification display window
	DIALOG_GUILD_WAREHOUSE_1,				///<Guild No. 1 Warehouse
	DIALOG_GUILD_WAREHOUSE_2,				///< Guild Warehouse No. 2
	DIALOG_GUILD_WAREHOUSE_3,				///<Guild warehouse 3
	DIALOG_TUTORIAL_DIALOG_FOCUS,			///< Tutorial dialog focus window
	DIALOG_TUTORIAL_EXAMPLE,				///< Tutorial example window
	DIALOG_TUTORIAL_MESSAGE,				///< Tutorial message
	DIALOG_TUTORIAL_CONFIRM,				///< Tutorial confirmation message
	DIALOG_PETITION_SATISFACTION,			///< GM consultation satisfaction survey window
	DIALOG_BROADCAST,
	DIALOG_PET_STATUSBAR,					///< Pet status window    
	DIALOG_PET_SKILL_SLOT,                ///<Skill slot window that appears when summoning a pet

	DIALOG_ITEM_EXCHANGE_CART,

	DIALOG_SCRAMBLE_NOTIFY,					///< Dojo competition progress notification window
	DIALOG_CCBD_NOTIFY,
	DIALOG_CCBD_REWARD,

	DIALOG_EVENT_LAST = DIALOG_CCBD_REWARD,

	
	// A dialog drawn on the screen by the user's operation

	DIALOG_NORMAL_FIRST, 

	DIALOG_MAINMENU = DIALOG_NORMAL_FIRST,	///< Main menu	
	DIALOG_ESCMENU,							///< ESC Menu			//83 IN TW
	DIALOG_STATUS,							///< Status window			
	DIALOG_SKILL,							///< Skill window
	DIALOG_CHAT_LOG,						///< Chat log window
    DIALOG_COMMUNITY,						///< Community dialog
    DIALOG_FRIEND_LIST,                     ///< Friend list/black list
	DIALOG_WORLDMAP,						///< World Map
	DIALOG_OPTIONWND,						///< Options window
	DIALOG_HELPWND,							///< Help window	
	DIALOG_PRIVATESHOP,						///< Private store
	DIALOG_MAILSYSTEM,						///< Mail system
	DIALOG_MAILSYSTEM_READ,					///< Mail system: Reading mail
	DIALOG_DBC_ALTAR,						///< Dragon Ball Collection Altar Window
    DIALOG_COMMU_TARGET,                    ///< Community target window

	DIALOG_HOIPOIMIX_RECIPE,
	DIALOG_HOIPOIMIX_CRAFT,

	DIALOG_CAPSULE_1,						///< bag
	DIALOG_CAPSULE_2,
	DIALOG_CAPSULE_3,
	DIALOG_CAPSULE_4,
	DIALOG_CAPSULE_5,
	DIALOG_CAPSULE_NETPY,					///< 

	//DIALOG_MSGBOX,							///< Message box (center alignment)
	
	DIALOG_QUESTLIST,						///< Quest list 
	DIALOG_QUESTPROGRESS,					///< Quest progress	
	
	DIALOG_SKILL_RPBONUS,					///< RP BONUS skill use GUI
	DIALOG_SKILL_RPBONUS_AUTO,

	DIALOG_RANKBOARD,						///< Ranking Board
	DIALOG_MASCOT,
	DIALOG_BUDOKAI_NEWS,					///< World's Best Martial Arts Newsletter
	DIALOG_BUDOKAI_REQUEST,					///< World¡¯s Best Martial Arts Application Form
	DIALOG_BUDOKAI_TOURNAMENT,
	DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO,	///< Tournament MatchInfo

	DIALOG_TBGAMBLERESULT,					///< Gamble capsule confirmation window
	DIALOG_CHANNGEL_CHANGE,					///< Channel change window during game //137 in tw
	DIALOG_PETITION,						///< GM consultation request window

	DIALOG_SKILLABILITYSET,					///< Skill Ability Settings GUI
	DIALOG_SKILLABILITYUSE,

	DIALOG_SKILL_HTB_RPUSE,					///< HTB Skill Rp Use Gui

	DIALOG_ITEM_OPTIONRESTRUCTURE,
	DIALOG_QUICK_TELEPORT,
	DIALOG_PLAYER_TITLE,

	DIALOG_NORMAL_LAST = DIALOG_PLAYER_TITLE,


	//NPC-related dialog (Basically, it is not affected by ESC key)

	DIALOG_NPCDIALOG_FIRST,

	DIALOG_MULTIDIALOG = DIALOG_NPCDIALOG_FIRST,///< NPC multi-dialog
	
	DIALOG_QUESTMESSAGE,					///< Quest message	
	DIALOG_QUESTPROPOSAL,					///< Quest suggestion	
	DIALOG_QUESTREWARD,						///< Quest Reward
	DIALOG_ITEMUPGRADE,						///< Item upgrade	
	DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE,
	DIALOG_RBBOARD,							///< Rank Battle Bulletin Board
	DIALOG_RBCHALLENGE,						///< Ranked Battle Challenge Information
	DIALOG_NPCSHOP,							///< NPC Shop	
	DIALOG_TMQBOARD,						///< Time Machine Quest Bulletin Board	
	DIALOG_ULTIMATEDUNGEON,
	DIALOG_WAREHOUSEBAR,					///< Warehouse control bar	
    DIALOG_PORTAL,                          ///< Portal NPC 
	DIALOG_OBJECT_CLICK,					///< Click on object
	DIALOG_GUILD_WAREHOUSEBAR,				///< Guild Warehouse Control Bar
	DIALOG_DOJO_INFO,						///< Stamp information window
	DIALOG_DOJO_ACCOUNT,					///< Paint warehouse usage history
	DIALOG_DOGI,							///< Uniform dialog
	DIALOG_DOJO_UPGRADE,					///< Painting Upgrade Dialog

	DIALOG_BIND_SEALING,
	DIALOG_ITEM_EXCHANGE_SHOP,				//buy items with items

	DIALOG_NETPYSHOP,
	DIALOG_DURATION_EXTEND_CONFIRM,			///< Extension Dialog

	DIALOG_HLSHOP,
	DIALOG_HLSHOP_WAREHOUSE,
	DIALOG_HLSHOP_BUY,
	DIALOG_HLSHOP_BUY_CONFIRM,
	DIALOG_HLSHOP_GIFT,
	DIALOG_HLSHOP_GIFT_CONFIRM,
	DIALOG_AUCTION_HOUSE,

	DIALOG_CCBD_BOARD,

	DIALOG_NPCDIALOG_LAST = DIALOG_CCBD_BOARD,

	DIALOG_NUM,

	DIALOG_LAST = DIALOG_NPCDIALOG_LAST,

	DIALOG_UNKNOWN = 0xFF,					///< Invalid dialog index
};

enum eDialogMode
{	
	DIALOGMODE_ITEM_REPAIR,					///< Item repair mode
	DIALOGMODE_ITEM_IDENTIFICATION,			///< Unidentified item appraisal
	DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION,	///< Evaluate unidentified items in the store
	DIALOGMODE_NARRATION,					///< Narration mode

	DIALOGMODE_CHATTING_RESIZE_HORI,		///< Vertical resize of chat display window
	DIALOGMODE_CHATTING_RESIZE_RIGHTUP,		///< Resize the upper right corner of the chat display window
	DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN,	///< Resize the lower right corner of the chat display window
	DIALOGMODE_CHATTING_RESIZE_VERT,		///< Resize the chat display window horizontally

	DIALOGMODE_ITEM_DISASSEMBLE,
	DIALOGMODE_ITEM_BEAD,
	DIALOGMODE_SKILL_SELECT,

	DIALOGMODE_UNKNOWN,

	DIALOGMODE_FIRST = DIALOGMODE_ITEM_REPAIR,
	DIALOGMODE_LAST  = DIALOGMODE_NARRATION
};


// Regular dialog attribute
#define dRDA_NORMAL							0x00000000
#define dRDA_EXCLUSIVE						0x00000001	///< Do not yield when another regular dialog opens
#define dRDA_HALFSIZE						0x00000002	///< It is aligned at the bottom at about half the size of other regular dialogs.
#define dRDA_NOT_ESC						0x00000004	///< It is not affected by the ESC key

// Entire focusing dialog attribute
#define dEFDA_NORMAL						0x00000000
#define dEFDA_BACKBOARD						0x00000001


// GUI Common Define
#define dDIALOG_CLEINT_EDGE_GAP				0

#define DBOGUI_DIALOG_TITLE_X				30
#define DBOGUI_DIALOG_TITLE_Y				2

#define DBOGUI_STACKNUM_WIDTH				32
#define DBOGUI_STACKNUM_HEIGHT				32
#define DBOGUI_STACKNUM_FONT				"detail"
#define DBOGUI_STACKNUM_FONTHEIGHT			75
#define DBOGUI_STACKNUM_FONTEFFECTMODE		TE_OUTLINE
#define DBOGUI_STACKNUM_FONTATTR			0
#define DBOGUI_STACKNUM_ALIGN				COMP_TEXT_LEFT | COMP_TEXT_DOWN

#define DBOGUI_SLOT_KEYNAME_FONT			"detail"
#define DBOGUI_SLOT_KEYNAME_FONTHEIGHT		75
#define DBOGUI_SLOT_KEYNAME_FONTEFFECTMODE	TE_OUTLINE
#define DBOGUI_SLOT_KEYNAME_FONTATTR		0
#define DBOGUI_SLOT_KEYNAME_ALIGN			COMP_TEXT_LEFT | COMP_TEXT_UP

#define DBOGUI_QUESTBALLOON_FONTHEIGHT		105

#define DBOGUI_ICON_SIZE					32
#define DBOGUI_ICON_SIZEDIFF				2

#define NTL_LINKED_DIALOG_GAP				5

#define dINPUTBOX_CARET_WIDTH				2
#define dINPUTBOX_CARET_HEIGHT				12


// Text Color
#define INFOCOLOR_0		RGB( 254, 254, 254 ) // white #fefefe
#define INFOCOLOR_1		RGB( 148, 191, 234 ) // blue #94bfea
#define INFOCOLOR_2		RGB(  38, 158, 255 ) // blue #269eff
#define INFOCOLOR_3		RGB( 172, 250,  80 ) // yellow/green #acfa50
#define INFOCOLOR_4		RGB( 176, 254,  90 ) // yellow/green #acfa50
#define INFOCOLOR_5		RGB( 243, 106, 124 ) // pink/red #f3677c
#define INFOCOLOR_6		RGB( 255, 168,  68 ) // orange #ffa844
#define INFOCOLOR_7		RGB( 255, 218,  75 ) // yellow #ffda4b
#define INFOCOLOR_8		RGB( 231, 121, 210 ) // purple #e779d2
#define INFOCOLOR_9		RGB( 180, 180, 180 ) // Grey
#define INFOCOLOR_13	RGB( 167, 184, 193 ) // Grey
#define INFOCOLOR_14	RGB( 35, 255, 60 ) // green #23ff3c

#define INFOCOLOR_LOBBY_FOC		RGB( 255, 255, 255 )
#define INFOCOLOR_LOBBY_DOWN	RGB( 193, 193, 193 )

#define RANKBATTLE_COLOR_YELLOW	RGB( 255, 254, 0 )
#define RANKBATTLE_COLOR_RED	RGB( 255, 0, 0 )
#define RANKBATTLE_COLOR_WHITE	RGB( 254, 254, 254 )
#define RANKBATTLE_COLOR_GRAY	RGB( 127, 127, 127 )
#define RANKBATTLE_COLOR_GREEN	RGB( 131, 255, 143 )

// button defalut color
#define NTL_BUTTON_UP_COLOR				RGB(255, 255, 255)
#define NTL_BUTTON_DOWN_COLOR			RGB(255, 210, 29)
#define NTL_BUTTON_FOCUS_COLOR			RGB(255, 255, 255)
#define NTL_BUTTON_DISABLE_COLOR		RGB(180, 180, 180)

// Party button color
#define NTL_PARTY_BUTTON_UP_COLOR			RGB(255, 210, 29)
#define NTL_PARTY_BUTTON_DOWN_COLOR			RGB(255, 210, 29)
#define NTL_PARTY_BUTTON_FOCUS_COLOR		RGB(255, 255, 255)
#define NTL_PARTY_BUTTON_DISABLE_COLOR		RGB(180, 180, 180)

// Team Color
#define NTL_RED_TEAM_COLOR		RGB( 255, 0, 0 )
#define NTL_BLUE_TEAM_COLOR		RGB( 0, 135, 255 )

#define NTL_BUTTON_CLICK_DIFFX			2 
#define NTL_BUTTON_CLICK_DIFFY			3 

// Rp gauge 
#define RPGAUGE_TIME_SERVER		1.0f
#define RPGAUGE_TIME_EPSILON	0.0f

// LP Warning ramp rate
#define dLPWARNING_RATE			0.3f

// RP Bonus Apply KeyTime
#define SKILL_RPBONUS_KEYTIME	0.5f

// Item Duration warning
#define ITEM_DURATION_WARNING	0.3f

// When you click on a dialog, a macro is used to draw the linked dialog and yourself at the top.
#define CAPTURE_MOUSEDOWN_RAISE(eDialog, PosX, PosY)	\
	if( m_pThis->GetParent()->GetChildComponentReverseAt((RwInt32)PosX, (RwInt32)PosY) != m_pThis ) \
	return; \
	\
	CRectangle rect = m_pThis->GetScreenRect(); \
	\
	if( rect.PtInRect((RwInt32)PosX, (RwInt32)PosY)) \
	{ \
		RaiseLinked(); \
		GetDialogManager()->RaiseDialogbyUser(eDialog); \
	}


// When you click on a dialog, the dialog linking that dialog and the dialogs linked to that dialog are displayed.
// Macro to draw at the top
#define CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(TopLinkedDialog, PosX, PosY)	\
	if( m_pThis->GetParent()->GetChildComponentReverseAt((RwInt32)PosX, (RwInt32)PosY) != m_pThis ) \
		return; \
	\
	CRectangle rect = m_pThis->GetScreenRect(); \
	\
	if( rect.PtInRect((RwInt32)PosX, (RwInt32)PosY)) \
		GetDialogManager()->GetDialog(TopLinkedDialog)->RaiseLinked();


// MouseWheelMove Macro
#define CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( bShow, pDialog, pComponent, pScrollBar, sDelta, pos )\
	if( !bShow ) return;\
	if( pDialog->GetParent()->GetChildComponentReverseAt( pos.x - pDialog->GetParent()->GetScreenRect().left, pos.y - pDialog->GetParent()->GetScreenRect().top ) != pDialog ) return;\
	if( pComponent->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT ) return;\
	\
	RwInt32 nValue = pScrollBar->GetValue();\
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();\
	RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;\
	\
	if( nDelta < 0 ) nDelta = 0;\
	else if( nDelta > nMaxValue ) nDelta = nMaxValue;\
	pScrollBar->SetValue( nDelta );