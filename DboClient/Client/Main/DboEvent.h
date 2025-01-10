/*****************************************************************************
 *
 *File			: DboEvent.h
 *Author	    : HyungSuk, Jang
 *Copyright	    : NTL Co., Ltd.
 *Date			: 2005. 9. 28	
 *Abstract		: DBO standard event.
 *****************************************************************************
 *Desc          : 
 *
 *****************************************************************************/

#ifndef __DBO_EVENT_H___
#define __DBO_EVENT_H___

// Shared
#include "NtlSharedDef.h"

#include "ceventhandler.h"
#include "DboDef.h"
#include "NtlMail.h"
#include "ScriptLinkTable.h"
#include "NtlPacketGU.h"
#include "ItemTable.h"


//////////////////////////////////////////////////////////////////////////////////
/**
*Event related to network
*/

extern RWS::CEventId g_EventVirtualServerPacket;	// virutal server packet event

extern RWS::CEventId g_EventLogInServerConnect;		// Connected to lobby server.

extern RWS::CEventId g_EventLobbyServerConnect;		// Connected to lobby server.

extern RWS::CEventId g_EventGameServerConnect;		// Connected to the game server.	

//////////////////////////////////////////////////////////////////////////////////
// LogIn stage
extern RWS::CEventId g_EventLogInStageStateEnter;	// login stage state enter

extern RWS::CEventId g_EventLogInStageStateExit;	// login stage state exit

extern RWS::CEventId g_EventLogInStageTimeOut;			// login stage state exit


//////////////////////////////////////////////////////////////////////////////////
// Character selection & making

extern RWS::CEventId g_EventCharStageStateEnter;	// character stage state enter

extern RWS::CEventId g_EventCharStageStateExit;		// character stage state exit

extern RWS::CEventId g_EventCharMake;				// Character Making Related

extern RWS::CEventId g_EventLoginMessage;			// Login Message

extern RWS::CEventId g_EventLobbyMessage;			// Lobby Message

//////////////////////////////////////////////////////////////////////////////////
/**
*Events related to GUI
*/

extern RWS::CEventId g_EventDebug_for_Developer;	// Debugging mode events for developers

extern RWS::CEventId g_EventLobbyPacketHelper;		// login Packet helper

extern RWS::CEventId g_EventLoginGuiEnable;			// login gui enable

extern RWS::CEventId g_EventCharSelectGuiEnable;	// char select gui enable

extern RWS::CEventId g_EventMsgBoxShow;				// Message box show event

extern RWS::CEventId g_EventMsgBoxResult;			// Message box resount event

extern RWS::CEventId g_EventChatGuiMode;			// chatting gui mode

extern RWS::CEventId g_EventSayMessage;				// chatting message show

extern RWS::CEventId g_EventSetTextToChatInput;		// Set Text to Chat Input

extern RWS::CEventId g_EventTimeFixedNotify;		// Time Fixed Notify message

extern RWS::CEventId g_EventCautionSideNotify;		// Time Fixed Notify -> Caution SideIcon Notify

extern RWS::CEventId g_EventNotify;					// Notify

extern RWS::CEventId g_EventFlashNotify;			// Flash notify
extern RWS::CEventId g_EventFlashNotifyFinish;		// Flash notify finish

extern RWS::CEventId g_EventFlashNotifyString;		// Flash notify string

extern RWS::CEventId g_EventFlashFinishNotify;		// Flash finish notify

extern RWS::CEventId g_EventGroupNotify;			// Group Notify

extern RWS::CEventId g_EventNameNotify;				// Name Notify

extern RWS::CEventId g_EventIconMoveClick;			// Icon Move Click

extern RWS::CEventId g_EventIconPopupShow;			// PopupMenu show

extern RWS::CEventId g_EventIconPopupResult;		// PopupMenu Result

extern RWS::CEventId g_EventCalcPopupShow;			// CalcPopup SHow

extern RWS::CEventId g_EventCalcPopupResult;		// CalcPopup Result

extern RWS::CEventId g_EventPickedUpHide;			

extern RWS::CEventId g_EventShop;					// store events

extern RWS::CEventId g_EventDialog;					// dialog event

extern RWS::CEventId g_EventPostDialog;				// Dialog post event

extern RWS::CEventId g_EventUpdateRP;				// RP change event.
extern RWS::CEventId g_EventUpdateRpStock;          // RP Stock Event

extern RWS::CEventId g_EventUpdateExp;				// EXP change event.

extern RWS::CEventId g_EventCharObjDelete;			// Deleting character objects.

extern RWS::CEventId g_EventItemUpgradeResult;		// Item upgrade results

extern RWS::CEventId g_EventItemChangeOptionConfirmation;
extern RWS::CEventId g_EventItemChangeOptionResult;

extern RWS::CEventId g_EventItemChangeBattleAttributeResult;

extern RWS::CEventId g_EventRegisterItemUpgrade;	// Register in the item upgrade slot. (right mouse button)

extern RWS::CEventId g_EventRegisterItemChangeBattleAttribute;

extern RWS::CEventId g_EventCharTitleSelectRes;
extern RWS::CEventId g_EventCharTitleUpdate;

extern RWS::CEventId g_EventQuickSlotInfo;			// Output saved quick slot data.

extern RWS::CEventId g_EventQuickSlotDelNfy;		// Command to delete quick slot from server.

extern RWS::CEventId g_EventNPCDialogOpen;			// Open NPC-related dialog

extern RWS::CEventId g_EventEndterWorld;			// When entering the world or teleporting

extern RWS::CEventId g_EventMap;					// Minimap, zone, world map message

extern RWS::CEventId g_EventUserTrade;				// user trade

extern RWS::CEventId g_EventServerCharDialog;		// Creation of a dialog window by server command.

extern RWS::CEventId g_EventOpenHelpWindow;			// Condition Check is displayed as soon as the Help Wnd is opened.

extern RWS::CEventId g_EventOpenHelpContent;		// Open the htm file of the content at the same time as opening the Help Wnd.

extern RWS::CEventId g_EventEnableItemIcon;				// Whether or not ItemIcon is enabled.
extern RWS::CEventId g_EventCapsuleLockItem;			// Whether ItemIcon is Lock/Unlock
extern RWS::CEventId g_EventCapsuleLockItemWithoutBag;	// Whether ItemIcon is Lock/Unlock (for linked items in bag)

extern RWS::CEventId g_EventDirectMoveIcon;			// To move the icon directly.

extern RWS::CEventId g_EventPrivateShopSelectNfy;	// An event to let you know that you have selected a personal store item

extern RWS::CEventId g_EventRegPrivateShopCartItem;	// Register the item in your personal store cart.

extern RWS::CEventId g_EventRegPrivateShopItem;		// Register items for sale in your personal store.

extern RWS::CEventId g_EventPrivateShopBusinessStart;			// Start bargaining in private stores

extern RWS::CEventId g_EventPrivateShopBusinessFluctuations;	// Start of private store negotiation price adjustment

extern RWS::CEventId g_EventPrivateShopBusinessCloseWindow;

extern RWS::CEventId g_EventBackboard;				// Backboard Gui Events

extern RWS::CEventId g_EventSummonPet;				// Events for SummonPet UI

extern RWS::CEventId g_EventOpenSendMail;			// Open SendMail.

extern RWS::CEventId g_EventReadingMail;			// Reading ReadMail.

extern RWS::CEventId g_EventRegMailAttachItem;		// Register items in Mail

extern RWS::CEventId g_EventRegBindSealingAttachItem;

extern RWS::CEventId g_EventRegHoiPoiMixMaterialItem;

extern RWS::CEventId g_EventRegCCBDCouponItem;

extern RWS::CEventId g_EventRegOptionReconstructItem;

extern RWS::CEventId g_EventOpenQuickTeleport;
extern RWS::CEventId g_EventQuickTeleportLoad;
extern RWS::CEventId g_EventQuickTeleportUpdate;
extern RWS::CEventId g_EventQuickTeleportDelete;
extern RWS::CEventId g_EventQuickTeleportMove;

extern RWS::CEventId g_EventRegAuctionHouseItem;

extern RWS::CEventId g_EventMailSlotUpdate;			// Mail Slot has been updated

extern RWS::CEventId g_EventQuestMessage;			// Quest message window related events

extern RWS::CEventId g_EventQuestMessage2;			// Events related to Quest message2 window

extern RWS::CEventId g_EventRpBonusSetup;			// Open the Skill RpBonus Setup UI.

extern RWS::CEventId g_EventRpBonusSelect;			// You must select Skill RpBonus.

extern RWS::CEventId g_EventRpBonusAuto;			// Skill RpBonus was set to Auto.

extern RWS::CEventId g_EventTargetStatusRefresh;	// Refreshes the current target information window. Used to re-display information when using scouter.

extern RWS::CEventId g_EventRankBattleRankListRes;	// Receive a list of ranking boards.

extern RWS::CEventId g_EventRankBattleRankFindCharacterRes;	// Receive character search results from the ranking board.

extern RWS::CEventId g_EventRankBattleRankCompareDayRes;	// Receive comparison date request results from the ranking board.

extern RWS::CEventId g_EventRankBoardDisable;

extern RWS::CEventId g_EventZennyLootingEffect;		// Effect when picking up Zenny

extern RWS::CEventId g_EventItemCreatedEffect;		// Effect when an item is placed in the bag

extern RWS::CEventId g_EventSideDialog;				// Side dialog related

extern RWS::CEventId g_EventTMQRecordListRes;		// Receive a list of TMQ rankings.

extern RWS::CEventId g_EventTMQMemberListRes;		// Receive a list of ranked parties from TMQ rankings.

extern RWS::CEventId g_EventBudokaiNews;				// World¡¯s Best Martial Arts Newsletter
extern RWS::CEventId g_EventBudokaiPrizeWinnerName;		// World's Best Martial Arts Winner's Name

extern RWS::CEventId g_EventScriptLinkNotify;

extern RWS::CEventId g_EventBudokaiTournamentIndividualList;	// The World¡¯s Best Martial Arts Club Individual Tournament
extern RWS::CEventId g_EventBudokaiTournamentIndividualInfo;	
extern RWS::CEventId g_EventBudokaiTournamentTeamList;			// The World¡¯s Best Martial Arts Club Individual Tournament
extern RWS::CEventId g_EventBudokaiTournamentTeamInfo;	

extern RWS::CEventId g_EventDiceStart;				// open dice

extern RWS::CEventId g_EventDiceResult;				// Result of rolling the dice
extern RWS::CEventId g_EventDiceResultShow;			// Display dice results

extern RWS::CEventId g_EventDiceRollReqtoServer;	// Request the result of rolling the dice to the server

extern RWS::CEventId g_EventShopGambleBuyRes;			// Results of purchasing gambling items
extern RWS::CEventId g_EventGambleUseEffect;			// Directing the use of gambling items

extern RWS::CEventId g_EventPetition;				// Message related to user calming system

// game teleport proposal notify
extern RWS::CEventId g_EventTeleportProposalNfy;		///< Teleport proposal event
extern RWS::CEventId g_EventTeleportConfirmRes;			///< Teleport confirmed result

extern RWS::CEventId g_EventObserverMenu;					///< GM menu related

extern RWS::CEventId g_EventRPCharge;					// RP Charge;

extern RWS::CEventId g_EventBattleCombatMode;					//

extern RWS::CEventId g_EventItemIdentifyEffect;		///< Item emotional effect

// Hoipoi Mix
extern RWS::CEventId g_EventHoipoiMixItemMakeRes;		///< Creating items
extern RWS::CEventId g_EventHoipoiMixCreateStart;		///< Create items with recipes

extern RWS::CEventId g_EventHintViewClosed;				///< Delete the side view.

// Loading
extern RWS::CEventId g_EventLoading;

// NetPyShop
extern RWS::CEventId g_EventNetPyShopEvent;

// Commercial extend
extern RWS::CEventId g_EventCommercialExtendCommand;

//Hlshop
extern RWS::CEventId g_EventHLShopEvent;
extern RWS::CEventId g_EventHLShopEventBuy;
extern RWS::CEventId g_EventHLShopEventGift;
extern RWS::CEventId g_EventHLShopEventItemInfo;
extern RWS::CEventId g_EventHLShopEventItemInfoRes;
extern RWS::CEventId g_EventHLShopEventItemBuyRes;
extern RWS::CEventId g_EventHLShopEventItemMoveRes;
extern RWS::CEventId g_EventHLShopEventItemUseRes;
extern RWS::CEventId g_EventHLShopEventItemAddNfy;
extern RWS::CEventId g_EventHLShopEventItemDelNfy;
extern RWS::CEventId g_EventHLShopEventItemGiftRes;

// Open BagGui, ScouterBackgroundGui, ...
extern RWS::CEventId g_EventOpenBagGui; 
extern RWS::CEventId g_EventOpenScouterBackgroundGui;

// test code
extern RWS::CEventId g_EventSCS;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eServerConnectType
{
	SERVER_CONNECT_TYPE_CONNENCT_SUCCESS,
	SERVER_CONNECT_TYPE_CONNENCT_FAIL,
	SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT
};

struct SDboEventServerConnect
{
	RwUInt8		byConnectType;
};


//////////////////////////////////////////////////////////////////////////////////
/**
*Events, enums related to GUI
*/

enum eDEVELOPER_TYPE
{
	DEVELOPER_RESET_GUI,						///< Delete the entire GUI and reload it
	DEVELOPER_PREVIEW_OPEN,						///< Open Preview Gui for development
	DEVELOPER_PREVIEW_CLOSE,					///< Close the Preview Gui for development
};

struct sDboDeveloperData
{
	RwUInt32	uiType;
};

struct SDboEventLogInStageStateEnter
{
	RwUInt8 byState;
};

struct SDboEventLogInStageStateExit
{
	RwUInt8 byState;
};

struct SDboEventLogInStageTimeOut
{
	RwUInt8 byState;
};

//struct SDboEventConnectBoxShow
//{
//	//WCHAR		pString[MAX_MSG_BOX_STRING_LEN];
//	std::wstring	wstrString;
//	RwUInt8			byMsgBoxType;
//	RwUInt32		uiMsgProcessType;
//	RwReal			fRemainTime;	
//	sMsgBoxData*	pExData;
//};

enum eNetMsgBoxProcessType
{
	NPT_NONE,

	
};

struct SDboEventCharStageStateEnter
{
	RwUInt8 byState;
};

struct SDboEventCharStageStateExit
{
	RwUInt8 byState;
};

struct SNtlEventCharMake
{
	RwBool	bSetDefaultCamera;
	BYTE	byRace;
	BYTE	byClass;		
	BYTE	byHair;
	BYTE	byHairColor;	
	BYTE	byFace;
	BYTE	bySkinColor;
	BYTE	byGender;
};

enum eLoginEventType
{
	LOGIN_EVENT_SUCCESS_LOGIN,						// Login succeeded.
	LOGIN_EVENT_DISCONNECTED_AUTH_SERVER,			// Disconnected from authentication server
	LOGIN_EVENT_FINISH_LOGO_PLAY,					// Each company's logo has been displayed.
	LOGIN_EVENT_START_INTRO_MOVIE_PLAY,				// Show intro video
	LOGIN_EVENT_SHOW_CONTRACT_PAGE,					// Show terms and conditions page
	LOGIN_EVENT_FINISH_LOADING_AND_MOVIE,			// Loading and movie finished
	LOGIN_EVENT_RETRY_LOGIN_REQ,					// Login failed and try again
};

struct SDboEventLoginMessage
{
	RwUInt8		byMessage;							// message
	RwReal		fValue;								// value
	RwUInt32	uiExData;
};

enum eLobbyEventType
{	
	LMT_UPDATE_SERVER,									// server update	
	LMT_UPDATE_CHANNEL,									// channel updates
	LMT_REFRESH_SERVER,									// Server status update
	LMT_REFRESH_CHANEL,									// Channel status update
	LMT_UPDATE_CHARACTER_LIST,							// The character list has been updated
	LMT_RENAME_CHARACTER,								// Character name changed //this is 8 in tw
	LMT_SELECT_CHARACTER,								// Selected a character //this is 9 in tw
	LMT_DESTORY_LOBBY_AVATAR,							// Delete the avatar information in the lobby
	LMT_DESTORY_LOBBY_WORLD,							// Delete the world in the lobby

	LMT_SELECT_CHANNEL,									// Selected Channel
	LMT_CREATE_CHARACTER,								// Pressed the character creation button

	LMT_GAME_SERVER_WAIT_CHECK_REQ,						// Check if connection to Game Server is possible
	LMT_START_CONNECT_GAME_SERVER,						// Start connecting to the Game Server //19 in tw

	LMT_ROTATION_STOP,									// Stop Rotation					//20 in tw
	LMT_ROTATION_FOREWARD,								// The character stops looking straight ahead.
	LMT_ROTATION_LEFT,									// Left Rotation
	LMT_ROTATION_RIGHT,									// Right Rotation			//23 in tw
	LMT_ROTATION,										// Rotates by a given value
	LMT_ZOOM_IN,										// Zoom in on the character
	LMT_ZOOM_OUT,										// Zoom out the character
	LMT_ZOOM_FAR,										// furthest zoom level
	LMT_ZOOM_DEFUALT,									// Default zoom to character
	LMT_ZOOM_FROM_FAR_TO_DEFUALUT,						// Changes from the furthest zoom level to the default level
};

struct SDboEventLobbyMessage
{
	RwUInt8		byMessage;							// message
	RwReal		fValue;								// value
	RwUInt32	uiExData;
};

struct SDboEventVirtualServerPacket
{
	RwInt32 iDataLen;
	RwChar chPacket[MAX_VIRTUAL_SERVER_PACKET_SIZE];
};

struct SDboEventLoadingEnable
{
	RwBool bEnable;		// TRUE or FALSE
};

struct SDboEventMsgBoxShow	// Stage transition event data structer.
{
	std::wstring	wstrString;
	std::string		strKey;
	RwBool			bPopup;
	RwReal			fShowTime;
	RwBool			bAcceptDuplicate;
	RwBool			bHasInput;
	sMsgBoxData*	pData;
	RwBool			bUpdate;
	std::list<sMsgBoxCustomBtn>* plistCustomBtn;
};

struct SDboEventMsgBoxResult
{
	RwInt32			eResult;
	std::string		strID;
	sMsgBoxData*	pData;
};

struct SDboEventChatGuiMode
{
	RwBool			bMode;
};

struct SDboEventSayMessage
{
	RwUInt8 byMsgType;
	RwUInt32 uiSerial;
	WCHAR wchUserName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR wchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
	BYTE	byChannel;
};

struct SDboEventTimeFixedNotify
{
	RwBool			bActive;
	std::string		strStringID;
	const WCHAR*	pString;
};

/**
* \brief Events sent from CautionNotify to Caution SideIcon
*/
struct SDboEventCautionSideNotify
{
	RwBool			bActive;
	std::string		strStringID;	///< String index
	const WCHAR*	pString;		///< String
	RwReal			fLifeTime;		///< duration
};

struct SDboEventNotify
{
	enum TYPE { ACTION, PUBLIC, NPC, RESULTCODE, TERRITORY };
	
	RwUInt32	eType;
	WCHAR		wchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
};

struct SDboEventFlashNotify
{
	RwUInt8		byProgressType;	
	RwUInt8		byMessageValue;
	RwUInt8		byMessageType;
};

struct SDboEventFlashNotifyString
{
	RwChar*		pcFlashFile;
	RwChar*		pcSecondFlashFile;
	RwBool		bFinishNotify;
};

struct SDboEventFlashFinishNotify
{
	RwUInt8		byProgressType;	
	RwUInt8		byMessageValue;
	RwUInt8		byMessageType;
	RwChar*		pcFlashFile;
	RwChar*		pcSecondFlashFile;
};

enum eGroupNotifyType
{
	GROUP_NOTIFY_PARTY,
	GROUP_NOTIFY_GUILD,
};

struct SDboEventGroupNotify
{
	RwUInt8		byType;
	WCHAR*		pwcText;
};

struct SDboEventNameNotify
{
	WCHAR*		pwcText;
};

struct SDboEventIconPopupShow
{
	RwBool bShow;
	RwUInt32 uiSerial;	
	RwInt32	nSrcPlace;
	RwInt32 nSrcSlotIdx;
	RwInt32 nXPos;
	RwInt32 nYPos;
};

struct SDboEventIconPopupResult
{
	RwUInt32 uiSerial;
	RwInt32 nSrcPlace;
	RwInt32 nSrcSlotIdx;
	RwInt32	nWorkId;
};

struct SDboEventCalcPopupShow
{
	RwBool bShow;
	RwUInt32 uiSerial;
	RwInt32	nSrcPlace;	
	RwInt32 nXPos;
	RwInt32 nYPos;
	RwInt32 uiMaxValue;
	bool	bAllowNull;
};

struct SDboEventCalcPopupResult
{
	RwUInt32	uiSerial;
	RwInt32		nSrcPlace;
	RwUInt32	uiValue;
};

struct SDboEventShop
{
	RwInt32			iType;

	RwUInt32		uiSerial;		///< NPC or Item
	unsigned long	ulPrice;		///< Item Price
	wchar_t*		pcItemName;		///< Item name
	RwUInt32		needItemCount;

	// for Server
	RwInt32			iPlace;			///< NPC Stores tab, or bag number
	RwInt32			iPosition;		///< The position of the item registered in the tab, the slot number in the bag
	RwInt32			iOverlapCount;	///< amount
};

enum eShopEventType
{
	TRM_NPCSHOP_OPEN,				///< NPC store opened
	TRM_ITEM_EXCHANGE_SHOP_OPEN,

	TRM_REG_ITEM_BY_NPCSHOP,		///< Registered to purchase items at the NPC store.
	TRM_REG_ITEM_BY_NPCSHOP_LARGE_BUY,///< Registered in bulk to purchase items at NPC stores.
	TRM_REG_ITEM_BY_BAG,			///< Registered to sell items from bag.
	
	TRM_BUY_SUCCESS,				///< Purchase successful
	TRM_SELL_SUCCESS,				///< Sales success	

	TRM_CLOSE,						///< Close the cart.

	TRM_MESSAGETYPE_NUM
};

struct SDboEventDialog
{
	RwInt32			iType;			///< NPC event type

	RwInt32			iSrcDialog;		///< Dialog that generates events
	RwInt32			iDestDialog;	///< Dialog that receives events

	RwInt32			iUserData;
	RwInt32			iUserData2;
	RwUInt32		uiUserData3;
	RwUInt32		uiUserData4;
};

enum eDialogEventType
{	
	DIALOGEVENT_NPC_BYEBYE,						///< Stop talking to NPCs, trading, etc. and leave.
	DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG,		///< Start trading the items in the bag.
												///< UserData: bag slot index, UserData2: slot number
	DIALOGEVENT_END_TRADING_ITEM_IN_BAG,		///< The transaction of the items in the bag has ended.
												///< UserData: bag slot index, UserData2: slot number
	DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG,		///< Upgrade the items in the bag.
												///< UserData: bag slot index, UserData2: slot number
	DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG,		///< Finish upgrading the items in the bag.
												///< UserData: bag slot index, UserData2: slot number

	DIALOGEVENT_BEGIN_ITEM_IN_OPTIONRECONSTRUCTION,
	DIALOGEVENT_END_ITEM_IN_OPTIONRECONSTRUCTION,

	DIALOGEVENT_OPEN_FAIL_NPC_DIALOG,			///< Failed to open NPC dialog
	DIALOGEVENT_INPUTEDITBOX_LOST_FOCUS_WITH_ESC_KEY,	///< InputEditbBox lost focus due to ESC key

	DIALOGEVENT_SKIP_MOUSE_WHEEL,				///< MouseWheel operates in the GUI Chatting Display window and ignores the MouseWheel of 3D logic.

	DIALOGEVENT_CLOSE_EMBLEM_MAKER,				///< Close the Emblem Maker gui in the Guild Dialog
	DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER,		///< Failed to close Emblem Maker gui in Guild Dialog

	DIALOGEVENT_CLOSE_GUILD_NOTICE,				///< Close the Notice gui in the Guild Dialog
	DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE,		///< Failed to close Guild Dialog's Notice gui

	DIALOGEVENT_REPAIR,							///< Fixed one item with low durability.
	DIALOGEVENT_REPAIR_ALL,						///< All items with low durability have been corrected.

	DIALOGEVENT_CREATE_DOJO_GUI,				/// Create a stamp GUI
};

struct SDboEventUpdateRP
{
	RwUInt32	hSerial;
	RwInt32		nRP;
	RwInt32		nMaxRP;
};

struct SDboEventUpdateRpStock
{
    RwUInt32    hSerial;
    RwInt32     nRpStock;
    RwInt32     nMaxRpStock;
	bool		bDropByTime;
};

struct SDboEventUpdateExp
{
	RwUInt32	uiIncreasedExp;         ///< Total increase
    RwUInt32    uiAcquisitionExp;       ///< Original increase amount, used for PC room bonus
    RwUInt32    uiBonusExp;             ///< Bonus increase amount, used for PC room bonuses.
};

struct SDboEventPartyContribution
{
	RwUInt32		iType;				///< Event type
	RwUInt8			byPos;				///< Slot location
	RwUInt32		uiValue;
};

struct SDboItemUpgradeResult
{
	WORD	wResultcode;
	BYTE	byGrade;
	WORD	wResultMessageCode;
	BYTE	byStoneCount;
	BYTE	byGuardStoneCount;
};

struct SDboRegisterItemUpgrade
{
	RwUInt32		hSrcSerial;
	RwUInt32		eSrcPlace;
	RwUInt32		uiSrcSlotIdx;
};

struct SDboItemChangeOptionResult
{
	void*	pItemOptionSet;
	BYTE	byBoxCount;
};

struct SDboItemChangeBattleAttributeResult
{
	WORD		wResultcode;
	RwUInt8		byBattleAttribute;
};

struct SDboNpcDialogOpen
{
	RwUInt32		hSerialId;
	RwInt32			eDialog;
};

enum eMapMessageType
{
	MMT_MINIMAP_ZOON_IN,				///< Enlarge minimap
	MMT_MINIMAP_ZOON_OUT,				///< Zoom out minimap

	MMT_APPEAR_HIDDEN_OBJECT,			///< Hidden object appears on screen
	MMT_DISAPPEAR_HIDDEN_OBJECT,		///< Hidden object disappears from the screen

	MMT_WARFOG_UPDATE,					///< Update the warfog

	MMT_MINIMAP_SHOW_OUR_TEAM,			///< Display our team on the minimap
	MMT_MINIMAP_HIDE_OUR_TEAM,			///< My team is not displayed on the minimap.

	MMT_MINIMAP_SHOW_OTHER_TEAM,		///< Displays the opposing team on the minimap
	MMT_MINIMAP_HIDE_OTHER_TEAM,		///< Does not display the opposing team on the minimap
};

struct SDboEventMap
{
	RwInt32			iMessage;
	RwUInt32		uiValue;
};

enum eUserTradeEventType
{
	USERTRADE_START,					///< start
	USERTRADE_ADD_ITEM,					///< Your item has been registered	
	USERTRADE_DEL_ITEM,					///< Your item has been unlocked	
	USERTRADE_UPDATE_ITEM,				///< The number of items has been changed
	USERTRADE_UPDATE_ZENNY,				///< Zenny has changed
	USERTRADE_REDAY,					///< Ready to trade

	USERTRADE_ADD_ITEM_NOTIFY,			///< Someone else's item has been registered
	USERTRADE_DEL_ITEM_NOTIFY,			///< Someone else's item has been unlocked
	USERTRADE_UPDATE_ITEM_NOTIFY,		///< Someone else changed the number of items
	USERTRADE_UPDATE_ZENNY_NOTIFY,		///< Someone else changed Zenny
	USERTRADE_REDAY_NOTIFY,				///< Someone else is ready to trade

	USERTRADE_END,						///< End
	USERTRADE_DECLINE,					///< Reject transaction application
	USERTRADE_ACCEPT,					///< Allow transaction application
};

struct SDboEventUserTrade
{
	RwUInt8			byEventType;
	RwUInt32		uiTarget;
	RwUInt32		uiHandle;
	RwUInt8			byCount;
	void*			pData;
};

struct SDboEventServerCharDialog
{
	RwUInt32		uiSerial;
	RwUInt8			byDialogType;
	RwUInt32		uiTextTblIdx;
	RwBool			bIsRefSpeechTable;
	RwUInt16		wTextLen;
	WCHAR*			pTextData;
};

struct SDboEventEnableItemIcon
{
	RwBool			bEnable;
	RwInt32			ePlace;
	RwInt32			nPlaceIdx;
	RwUInt32		uiSlotIdx;	
};

struct SDboEventCapsuleLockItem
{
	RwBool			bEnable;
	RwInt32			ePlace;
	RwInt32			nPlaceIdx;
	RwUInt32		uiSlotidx;
};

struct SDboEventCapsuleLockItemWithoutBag
{
	RwBool			bEnable;
	SERIAL_HANDLE	hItem;
};

struct SDboEventDirectMoveIcon
{
	RwUInt32		uiSrcSerial;
	RwUInt32		eSrcPlace;
	RwUInt32		eDestPlace;
	RwUInt32		uiSrcSlotIdx;
	RwUInt32		uiSrcStackCount;
};

struct SDboEventOpenHelpWindow
{
	RwUInt32		uiTableIndex;
};

struct SDboEventOpenHelpContent
{
	RwUInt32		uiDialogType;
};

struct SDboEventTMQ
{
	RwUInt32		uiMessage;
	RwUInt32		uiValue;
	RwUInt32		uiValue2;
	VOID*			pData;
}; 

struct SDboEventPrivateShopSelectNfy
{
	RwUInt8			uiPrivateShopPos;
	RwUInt8			uiItemState;
};

struct SDboEventRegPrivateShopCartItem
{
	RwUInt32		uiSerial;
	RwUInt32		uiPlace;
	RwUInt32		uiPos;
	RwUInt32		uiStackCount;
	RwUInt32		uiPrice;
	void*			pData;
};

struct SDboEventRegPrivateShopItem
{
	RwUInt32		uiPlace;
	RwUInt32		uiIndex;
	RwUInt32		uiPos;
};

/**
* \brief Structure of private store bargain start event
*/
struct SDboEventPrivateShopBusinessStart
{
	RwUInt8					uiType;					///< Store owner, customer type
	RwUInt32				uiShopOwner;			///< Storekeeper's handle
	WCHAR					awcTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];	///< Opponent's character name
	sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData;	///< Personal store item data structure
	void*					pGui;					///< GUI class pointer for personal store
};

/**
* \brief Structure of private store negotiation price adjustment (immediate purchase)
*/
struct SDboEventPrivateShopBusinessFluctuations
{
	RwUInt8					uiType;					///< Store owner, customer type
	sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData;	///< Personal store item data structure
	RwUInt32				uiStartPrice;			///< Negotiation starting price
};

/**
* \brief Event to close the private store bargain window
*/
struct SDboEventPrivateShopBusinessCloseWindowNfy
{
	RwUInt32				uiShopOwner;
};

enum eBackboardEventType
{
	BET_COLOR,							///< Change color
	BET_ALPHA,							///< Change transparency
};

struct SDboEventBackboard
{
	RwUInt32		uiMessage;
	RwUInt8			byValue;
	RwUInt8			byValue2;
	RwUInt8			byValue3;
};

struct SDboEventSummonPet
{
	RwBool			bCreate;			// TRUE for creation, False for destruction.
	RwUInt32		uiSerialID;			// Serial ID of Summon Pet
};

struct SDboEventOpenSendMail
{
	WCHAR			awcTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventReadingMail
{
	RwUInt32		uiMailID;
	RwBool			bReading;
};

enum eQuestMessageType
{
	QUEST_MESSAGE_QUEST_TEXT_TABLE,
	QUEST_MESSAGE_TMQ_SCRIPT,
	QUEST_MESSAGE_VISIT_QUEST,
};

struct SDboEventQuestMessage
{
	RwUInt32		uiValue;
	RwUInt32		uiValue2;
};

struct SDboEventQuestMessage2
{
	sQUEST_INFO*	pQuestInfo;
	RwUInt32		uiEventGenType;
	RwUInt32		uiID;
};

struct SDboEventRpBonusSetup
{
	RwInt32 iXPos;
	RwInt32 iYPos;
	VOID*	pData;
};

struct SDboEventRpBonusAuto
{
	RwUInt8 byRpBonusType;
};

struct SDboEventRankBattleRankListRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	RwUInt8		byRankInfoCount;
	RwUInt16	wRankInfo;
	VOID*		sData;
};

struct SDboEventRankBattleRankFindCharacterRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	WCHAR*		pwsCharName;
	RwUInt8		byRankInfoCount;
	RwUInt16	wRankInfo;
	VOID*		sData;
};

struct SDboEventRankBattleRankCompareDayRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	RwBool		bIsFullList;
	RwUInt8		byInfoCount;
	union
	{
		RwUInt16	wRankInfo;
		RwUInt16	wCompareInfo;
	};

	VOID*		sData;
};

struct SDboEventZennyLootingEffect
{
	RwUInt32	uiZenny;
};

struct SDboEventItemCreatedEffect
{
	RwUInt32	hSerial;
	RwUInt8		ucBagIdx;
	RwUInt8		ucSlotIdx;
};

enum eSideDialogMessageType
{
	SDIALOG_MESSAGE_CONTROLLER_ADD_BUTTON,		///< Add a button to Side dialog controller
	SDIALOG_MESSAGE_CONTROLLER_DEL_BUTTON,		///< Delete the button from Side dialog controller
	SDIALOG_MESSAGE_MINIMIZE,					///< Side dialog minimization
	SDIALOG_MESSAGE_INACTIVE,					///< Side dialog Close Minimize window
};

struct SDBoEventSideDialog
{
	RwUInt8		byMessage;
	RwUInt32	uiSrcSDialog;
	RwUInt32	uiDestSDialog;
	RwUInt32	uiExData;
};

struct SDboEventTMQRecordListRes
{
	RwUInt16	wResultCode;
	RwUInt32	tmqTblidx;
	RwUInt8		byDifficult;
	RwUInt8		byTeamCount;
	VOID*		paTeam;
};

struct SDboEventTMQMemberListRes
{
	RwUInt16	wResultCode;
	RwUInt32	tmqTblidx;
	RwUInt8		byDifficult;
	RwUInt8		byRank;
	VOID*		pTeam;
};

/**
* \brief Event structure that controls the world's best martial arts club newsletter interface
*/
struct SDboEventBudokaiNews
{
	RwBool		bShow;			///< Whether to open or close
	RwUInt8		byType;			///< Type to open
};

/**
* \brief Event to refer to Script Link Table
*/
struct SDboEventScriptLinkNotify
{
	RwUInt8		byType;								///< Type of content to connect
	RwUInt8		byAction;							///< Actions to take depending on the type
	WCHAR		wszValue[DBO_MAX_LENGTH_FILE + 1];	///< Value
};

struct SDboEventBudokaiTournamentIndividualList
{
	RwUInt16	wEntryTeam_var;
	RwUInt8		byMatchDataCount;
	RwUInt16	wMatchData_var;
	sVARIABLE_DATA*	pData;
};

struct SDboEventBudokaiTournamentIndividualInfo
{
	RwUInt16		wTeamType[2];
	sVARIABLE_DATA* pData;
};

struct SDboEventBudokaiTournamentTeamList
{
	RwUInt16	wEntryTeam_var;
	RwUInt8		byMatchDataCount;
	RwUInt16	wMatchData_var;
	sVARIABLE_DATA*	pData;
};

struct SDboEventBudokaiTournamentTeamInfo
{
	RwUInt16		wTeamType[2];
	sVARIABLE_DATA* pData;	
};

struct SDboEventDiceResult
{
	SERIAL_HANDLE hHandle;
	RwUInt8		byWorkID;
	RwUInt8		byResultNum;
};

typedef SDboEventDiceResult SDboEventDiceResultShow;

struct SDboEventDiceStart
{
	RwUInt8		byWorkID;
	void*		pExData;
};

struct SDboEventDiceRequesttoServer
{
	RwUInt8		byWorkID;
};

struct SDboEventShopGambleBuyRes
{
	RwUInt16 wResultCode;
	RwUInt32 hHandle;
	RwUInt32 hItem;
};

struct SDboEventGambleUseEffect
{
	RwUInt8	ucBagIdx;			// To create the illusion that the item goes into the bag after the gambling scene is over.
	RwUInt8	ucSlotIdx;
	RwUInt32 hCreateSerial;		///< Items created through gambling
	RwChar  szEffectFile[ DBO_MAX_LENGTH_ITEM_ICON_NAME + 1 ];		// Directed file name
};

struct SDboEventRegMailAttachItem
{
	SERIAL_HANDLE	hHandle;
	RwUInt32		uiPlace;
	RwUInt32		uiIndex;
	RwUInt32		uiPos;
};

struct SDboEventQuickTeleportLoad
{
	BYTE					byCount;
	sQUICK_TELEPORT_INFO*	asData;
};

struct SDboEventQuickTeleportUpdate
{
	void*	asData;
};

struct SDboEventQuickTeleportDeleteAndMove
{
	BYTE	bySlot;
};

enum ePetitionEventType
{
	PETITON_OPEN_DILAOG,						///< Opens the consultation (complaint) request window
	PETITON_CLOSE_DILAOG,						///< Close the consultation (complaint) request window
	PETITON_REQUEST_OK,							///< Request for consultation (complaint) has been forwarded to GM
	PETITON_MODIFY_OK,							///< Request for modification of consultation (complaint) has been sent to GM
	PETITON_CANCEL,								///< Consultation (complaint) has been cancelled.
	PETITON_FINISH,								///< Counseling (complaint) has been processed.

	PETITON_ENABLE_PETITON_GUI,					///< Activate PetitionGui

	PETITON_CHATTING_START,						///< Start GM chat
	PETITON_CHATTING_END,						///< End GM chat
	PETITON_FINISH_RESEARCH_SATISFACTION,		///< End of satisfaction survey
};

struct SDboEventPetition
{
	RwUInt32		uiEventType;
};

struct SDboEventTeleportProposalNfy
{
	RwUInt8				byTeleportType;
	RwUInt8				byInfoIndex;

	RwUInt16			wWaitTime;
	TBLIDX				worldTblidx;

	union
	{
		RwUInt32		dwReserve;
		RwUInt8			byBudokaiMatchDepth;
	};

	WCHAR				wszSummonnerName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventTeleportConfirmRes
{
	RwUInt16			wResultCode;
	RwUInt8				byTeleportIndex;
	bool				bTeleport;
	bool				bClearInterface;
};

enum eObserverMenuEventType
{
	OBSERVER_START_OBSERVER,					///< Starts the observer menu
	OBSERVER_END_OBSERVER,						///< Exit the observer menu
	OBSERVER_SHOW_PLAYER_INFO,					///< Shows PlayerInfo Gui
	OBSERVER_REMOVE_PLAYERINFO_GUI,				///< Remove PlayerInfo Gui
	OBSERVER_SELECT_POPUPMENU,					///< A selection was made from the Popup menu
	OBSERVER_TENKAICHI_MATCH_DATA_REQ,			///< Requested information on the progress of the World's Best Ball.
	OBSERVER_TENKAICHI_MATCH_DATA_RES,			///< Received information on the progress of the World's Best Ball.
	OBSERVER_TENKAICHI_MATCH_DATA,				///< World's Best Martial Arts Progress Information Contents
};

struct SDboEventObserverMenu
{
	RwUInt8			byEventType;
	SERIAL_HANDLE	hHandle;
	RwUInt32		uiValue;
	void*			pExdata;
};

struct SDboEventRPCharge
{
	RwBool			bIncrease;					///< or Decrease
	RwBool			bStop;						///< or Start
};

struct SDboEventBattleCombatMode
{
	bool			bCombatMode;
};

struct SDboEventItemIdentifyEffect
{
	RwBool			bHandle;					///< Whether it is a Handle value or a Pos value

	SERIAL_HANDLE	hItemHandle;
	RwUInt8			byPlace;
	RwUInt8			byPos;
};

// Hoipoi

struct SDboEventHoipoiMixItemMakeRes
{
	RwUInt16		wResultCode;
	TBLIDX			itemTblidx;
	HOBJECT			hObject;
	TBLIDX			idxRecipeTbl;
	SERIAL_HANDLE	hItem;
};

struct SDboEventHoipoiMixCreateStart
{
	TBLIDX			idxRecipeTbl;
};

enum eMailSlotUpdateType
{
	eMAILSLOT_UPDATE_LOCK = 0,

	eMAILSLOT_UPDATE_NUMS,
	eMAILSLOT_UPDATE_INVALID = 0xFF
};

struct SDboEventMailSlotUpdate
{
	RwUInt8		byType;
	RwUInt32	uiParam1;
	RwUInt32	uiParam2;
};

struct SDboEventShopNetPyItemStartRes
{
	RwUInt8 byType;	// 0: Basic 1: Limited quantity sales event NPC
};

enum eNetPyShopEventType
{
	eNETPYSHOP_EVENT_START,
	eNETPYSHOP_EVENT_REG_ITEM,
	eNETPYSHOP_EVENT_REG_ITEM_MAX,
	eNETPYSHOP_EVENT_BUY_SUCCESS,
	eNETPYSHOP_EVENT_END,

	eNETPYSHOP_NUMS,
	eNETPYSHOP_INVALID = 0xFF
};

// NetPyShop Event
struct SDboEventNetPyShopEvent
{
	RwUInt8 byEventType;

	RwUInt32		uiSerial;		///< NPC or Item
	unsigned long	ulPrice;		///< Item price
	wchar_t*		pcItemName;		///< Item name

	RwInt32			nPlace;			///< Store tab or bag number
	RwInt32			nPosition;		///< Location of the registered item in the tab, slot number in the bag
	RwInt32			nOverlapCount;	///< Number
};

enum eHLShopEventType
{
	eHLSHOP_EVENT_START,
	eHLSHOP_EVENT_REFRESH,
	eHLSHOP_EVENT_END
};

struct SDboEventHLShopEvent
{
	BYTE		byEventType;
	DWORD		dwCash;
};

struct SDboEventHLShopEventBuy
{
	bool		bConfirm;
	TBLIDX		hlsItemIdx;
};

struct SDboEventHLShopEventGift
{
	bool		bConfirm;
	TBLIDX		hlsItemIdx;
	WCHAR		wchPlayer[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventHLShopEventItemInfo
{
	BYTE byCount;
	sCASHITEM_BRIEF* aInfo;
};

struct SDboEventHLShopEventItemBuyRes
{
	DWORD dwCash;
};

struct SDboEventHLShopEventItemGiftRes
{
	DWORD dwCash;
};

struct SDboEventHLShopEventItemMoveRes
{
	QWORD qwProductId;
};

struct SDboEventHLShopEventItemAddNfy
{
	sCASHITEM_BRIEF* aInfo;
};

struct SDboEventHLShopEventItemDelNfy
{
	QWORD qwProductId;
};

// Commercial extend
enum eCommercialExtendCommandType
{
	eSTART_PROCESS_BUY_NPC_DURATION_ITEM,	/// Duration item Start trading process
	eSTART_PROCESS_BUY_NETPY_DURATION_ITEM,	/// Duration item Start trading process
	eSTART_PROCESS_MOVE_DURATION_ITEM,	/// duration item Begin yard rat movement process
	eSET_DURATION_ITEM,					/// Duration item to trade tblidx setting
	eSET_DURATION_EXTEND_TARGET_ITEM,	/// Target NtlSobItem setting to extend period
	eCHANGE_COMMERCIALEXTEND_MODE,		/// Change GUI_EXTEND_MODE from COMMERCIALEXTEND_DIALOG
	eOPEN_COMMERCIALEXTEND_DIALOG,		/// data: RwInt32 iMode, RwUInt32 uiItemIdx
};

struct SDboEventCommercialExtendNPCData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiNPCSerial; 
	sSHOP_BUY_CART ShopBuyCart;
};

struct SDboEventCommercialExtendNetpyData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiMerchantTblidx;
	BYTE byMerchanSlotPos;
};

struct SDboEventCommercialExtendYardratData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiProductId;
};

struct SDboEventCommercialExtendOpenData
{
	int iMode;						/// GUI_EXTEND_MODE enum
	RwUInt32 uiItemIdx;
};

struct SDboEventCommercialExtendCommand
{
	eCommercialExtendCommandType eCommandType;
	void*				pData;
};

// Quickslot
struct SDboEventQuickSlotDelNfy
{
	RwUInt8	bySlotIdx;
	HOBJECT	hHandle;
	RwBool	bSendPacket;
};

struct SDboEventCharTitleSelectRes
{
	TBLIDX	uiTitle;
};

struct SDboEventCharTitleUpdate
{
	TBLIDX	uiTitle;
	bool	bDelete;
};

struct SDboEventLoading
{
	RwUInt8 byMessage;
};

enum eLOADING_EVENT_MESSAGE
{
	LOADING_EVENT_FIRST_MOVIE_END,
	LOADING_EVENT_SECOND_MOVIE_END,
	LOADING_EVENT_THIRD_MOVIE_END,
};

//////////////////////////////////////////////////////////////////////////
// Korean PC room related 
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventUpdateNetPy;                       ///< When NetPy changes
extern RWS::CEventId g_EventNetMarbleMemberShipNfy;            ///< When you are notified that you are a PC room user

struct SDboEventUpdateNetPy
{
    DWORD   netPy;                                              ///< Current Netpy
    DWORD   dwAccumlationNetPy;                                 ///< Accumulated NetPy after connection
    DWORD   timeNextGainTime;                                   ///< Next NetPy acquisition time (seconds)
};

struct SDboEventBudokaiPrizeWinnerName
{
	BYTE	byMatchType;

	WORD	wWinner;
	WORD	wSecondWinner;

	// sVARIABLE_DATA
	void*	pData;
};

#endif