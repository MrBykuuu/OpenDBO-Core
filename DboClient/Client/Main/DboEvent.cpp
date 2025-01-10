#include "precomp_dboclient.h"
#include "DboEvent.h"

#include "ceventhandler.h"


RWS::CEventId g_EventVirtualServerPacket;	// virutal server packet event

RWS::CEventId g_EventLogInServerConnect;	// Connected to the LogIn server.

RWS::CEventId g_EventLobbyServerConnect;	// Connected to the lobby server.

RWS::CEventId g_EventGameServerConnect;		// Connected to the game server.	


RWS::CEventId g_EventLogInStageStateEnter;	// login stage state enter

RWS::CEventId g_EventLogInStageStateExit;	// login stage state exit

RWS::CEventId g_EventLogInStageTimeOut;			// login stage time out


RWS::CEventId g_EventCharStageStateEnter;	// character stage state enter

RWS::CEventId g_EventCharStageStateExit;	// character stage state exit

RWS::CEventId g_EventCharMake;				// Character Making Related

RWS::CEventId g_EventLoginMessage;			// Login Message

RWS::CEventId g_EventLobbyMessage;			// Lobby Message


RWS::CEventId g_EventDebug_for_Developer;	// Debugging mode events for developers

RWS::CEventId g_EventLobbyPacketHelper;		// login Packet helper

RWS::CEventId g_EventLoginGuiEnable;		// login gui enable

RWS::CEventId g_EventCharSelectGuiEnable;	// char select gui enable

RWS::CEventId g_EventMsgBoxShow;			// MsgBox show event

RWS::CEventId g_EventMsgBoxResult;			// Message box resount event

RWS::CEventId g_EventChatGuiMode;			// chatting gui mode

RWS::CEventId g_EventSayMessage;			// chatting message show

RWS::CEventId g_EventSetTextToChatInput;	// Set Text to Chat Input

RWS::CEventId g_EventTimeFixedNotify;		// Time Fixed Notify message

RWS::CEventId g_EventCautionSideNotify;		// Time Fixed Notify -> Caution SideIcon Notify

RWS::CEventId g_EventNotify;				// Notify

RWS::CEventId g_EventFlashNotify;			// Flash notify
RWS::CEventId g_EventFlashNotifyFinish;		// Flash notify finish

RWS::CEventId g_EventFlashNotifyString;		// Flash notify string

RWS::CEventId g_EventFlashFinishNotify;		// Flash finish notify

RWS::CEventId g_EventGroupNotify;			// Group Notify

RWS::CEventId g_EventNameNotify;			// Name Notify

RWS::CEventId g_EventIconMoveClick;			// Icon Move Click

RWS::CEventId g_EventIconPopupShow;			// PopupMenu show

RWS::CEventId g_EventIconPopupResult;		// PopupMenu Result

RWS::CEventId g_EventCalcPopupShow;			// Calc popup

RWS::CEventId g_EventCalcPopupResult;		// CalcPopup Result

RWS::CEventId g_EventPickedUpHide;			

RWS::CEventId g_EventShop;					// store events

RWS::CEventId g_EventDialog;				// dialog event

RWS::CEventId g_EventPostDialog;			// Dialog post event

RWS::CEventId g_EventUpdateRP;				// Rp change event

RWS::CEventId g_EventUpdateRpStock;         // RP Stock Change Event

RWS::CEventId g_EventUpdateExp;				// Exp increase event.

RWS::CEventId g_EventCharObjDelete;			// Character extinction event.

RWS::CEventId g_EventSobCreateDropItemInfo;

RWS::CEventId g_EventItemUpgradeResult;		// Item upgrade results

RWS::CEventId g_EventItemChangeOptionConfirmation;
RWS::CEventId g_EventItemChangeOptionResult;

RWS::CEventId g_EventItemChangeBattleAttributeResult;		// Item upgrade results

RWS::CEventId g_EventRegisterItemUpgrade;	// Register in item upgrade slot (right mouse button)

RWS::CEventId g_EventRegisterItemChangeBattleAttribute;

RWS::CEventId g_EventCharTitleSelectRes;
RWS::CEventId g_EventCharTitleUpdate;

RWS::CEventId g_EventQuickSlotInfo;			// Output saved quick slot data.

RWS::CEventId g_EventQuickSlotDelNfy;		// Command to delete quick slot from server.

RWS::CEventId g_EventNPCDialogOpen;			// NPC-related dialog opens.

RWS::CEventId g_EventEndterWorld;			// When entering the world or teleporting

RWS::CEventId g_EventMap;					// Minimap, zone, world map message

RWS::CEventId g_EventUserTrade;				// user trade

RWS::CEventId g_EventServerCharDialog;		// Creation of a dialog window by server command.

RWS::CEventId g_EventOpenHelpWindow;		// Condition Check is displayed as soon as the Help Wnd is opened.

RWS::CEventId g_EventOpenHelpContent;

RWS::CEventId g_EventEnableItemIcon;			// Whether or not ItemIcon is enabled.
RWS::CEventId g_EventCapsuleLockItem;			// Whether ItemIcon is Lock/Unlock
RWS::CEventId g_EventCapsuleLockItemWithoutBag; // Whether ItemIcon is Lock/Unlock (for linked items in bag)

RWS::CEventId g_EventDirectMoveIcon;		// To move the icon directly.

RWS::CEventId g_EventPrivateShopSelectNfy;	// Notify to register an item

RWS::CEventId g_EventRegPrivateShopCartItem;	// Register the item in your personal store cart.

RWS::CEventId g_EventRegPrivateShopItem;	// Register items for sale in your personal store.

RWS::CEventId g_EventPrivateShopBusinessStart;			// Start bargaining in private stores

RWS::CEventId g_EventPrivateShopBusinessFluctuations;	// Start of private store negotiation price adjustment

RWS::CEventId g_EventPrivateShopBusinessCloseWindow;	// Private store Heungjeongchang Close

RWS::CEventId g_EventBackboard;				// Backboard Gui Events

RWS::CEventId g_EventSummonPet;		// Events for SummonPet UI

RWS::CEventId g_EventOpenSendMail;			// Open SendMail.

RWS::CEventId g_EventReadingMail;			// Reading ReadMail.

RWS::CEventId g_EventRegMailAttachItem;		// Register an item in the mail Attach window.

RWS::CEventId g_EventRegBindSealingAttachItem;		// Attach bind sealing item (equipment or seal coin)

RWS::CEventId g_EventRegHoiPoiMixMaterialItem;

RWS::CEventId g_EventRegCCBDCouponItem;		// Attach CCBD coupon

RWS::CEventId g_EventRegOptionReconstructItem;

RWS::CEventId g_EventOpenQuickTeleport;
RWS::CEventId g_EventQuickTeleportLoad;
RWS::CEventId g_EventQuickTeleportUpdate;
RWS::CEventId g_EventQuickTeleportDelete;
RWS::CEventId g_EventQuickTeleportMove;

RWS::CEventId g_EventRegAuctionHouseItem;

RWS::CEventId g_EventMailSlotUpdate;		// Mail Slot Updates

RWS::CEventId g_EventQuestMessage;			// Quest message window related events

RWS::CEventId g_EventQuestMessage2;			// Events related to Quest message2 window

RWS::CEventId g_EventRpBonusSetup;			// Open the Skill RpBonus Setup UI.

RWS::CEventId g_EventRpBonusSelect;			// You must select Skill RpBonus.

RWS::CEventId g_EventRpBonusAuto;		// Skill RpBonus was used as Auto.

RWS::CEventId g_EventTargetStatusRefresh;	// Refreshes the current target information window. Used to re-display information when using scouter.

RWS::CEventId g_EventRankBattleRankListRes;	// Receive a list of ranking boards.

RWS::CEventId g_EventRankBattleRankFindCharacterRes;	// Receive character search results from the ranking board.

RWS::CEventId g_EventRankBattleRankCompareDayRes;	// Receive comparison date request results from the ranking board.

RWS::CEventId g_EventRankBoardDisable;				// Switch the rank board to Disable state.

RWS::CEventId g_EventZennyLootingEffect;	// Effect when picking up Zenny

RWS::CEventId g_EventItemCreatedEffect;		// Effect when an item is placed in the bag

RWS::CEventId g_EventSideDialog;				// Side dialog related

RWS::CEventId g_EventTMQRecordListRes;		// Receive a list of TMQ rankings.

RWS::CEventId g_EventTMQMemberListRes;		// Receive a list of ranked parties from TMQ rankings.

RWS::CEventId g_EventBudokaiNews;				// Open or close the World's Best Martial Arts Newsletter UI.
RWS::CEventId g_EventBudokaiPrizeWinnerName;	

RWS::CEventId g_EventScriptLinkNotify;		// Script Link Table Event

RWS::CEventId g_EventBudokaiTournamentIndividualList;	// The World¡¯s Best Martial Arts Club Individual Tournament
RWS::CEventId g_EventBudokaiTournamentIndividualInfo;	
RWS::CEventId g_EventBudokaiTournamentTeamList;			// The World¡¯s Best Martial Arts Club Individual Tournament
RWS::CEventId g_EventBudokaiTournamentTeamInfo;	

RWS::CEventId g_EventDiceStart;					// open dice

RWS::CEventId g_EventDiceResult;				// Result of rolling the dice
RWS::CEventId g_EventDiceResultShow;			// Display dice results

RWS::CEventId g_EventDiceRollReqtoServer;		// Request the result of rolling the dice to the server

RWS::CEventId g_EventShopGambleBuyRes;			// Results of purchasing gambling items
RWS::CEventId g_EventGambleUseEffect;			// Directing the use of gambling items

RWS::CEventId g_EventPetition;				// Message related to user calming system

RWS::CEventId g_EventTeleportProposalNfy;		///< Teleport proposal event
RWS::CEventId g_EventTeleportConfirmRes;		///< Teleport confirmed result

RWS::CEventId g_EventObserverMenu;					///< GM menu related

RWS::CEventId g_EventRPCharge;					// RP Charge;

RWS::CEventId g_EventBattleCombatMode;			//

RWS::CEventId g_EventItemIdentifyEffect;

RWS::CEventId g_EventHoipoiMixItemMakeRes;		///< Creating items
RWS::CEventId g_EventHoipoiMixCreateStart;		///< Create an item using a recipe.

RWS::CEventId g_EventHintViewClosed;			///< Delete a side view.

RWS::CEventId g_EventLoading;

RWS::CEventId g_EventNetPyShopEvent;			///< NetPyShop Event

RWS::CEventId g_EventCommercialExtendCommand;	///< CommercialExtend command Event

RWS::CEventId g_EventHLShopEvent;				///< HLShop Event
RWS::CEventId g_EventHLShopEventBuy;
RWS::CEventId g_EventHLShopEventGift;
RWS::CEventId g_EventHLShopEventItemInfo;
RWS::CEventId g_EventHLShopEventItemInfoRes;
RWS::CEventId g_EventHLShopEventItemBuyRes;
RWS::CEventId g_EventHLShopEventItemMoveRes;
RWS::CEventId g_EventHLShopEventItemUseRes;
RWS::CEventId g_EventHLShopEventItemAddNfy;
RWS::CEventId g_EventHLShopEventItemDelNfy;
RWS::CEventId g_EventHLShopEventItemGiftRes;

RWS::CEventId g_EventOpenBagGui;				///< Occurs when BagGui is opened
RWS::CEventId g_EventOpenScouterBackgroundGui;	///< Occurs when ScouterBackgroundGui is opened (use scouter)

//------------------------------------------------------
// PC room related events
RWS::CEventId g_EventUpdateNetPy;
RWS::CEventId g_EventNetMarbleMemberShipNfy;

//-------------------------------------------------------
// test code
RWS::CEventId g_EventSCS;
