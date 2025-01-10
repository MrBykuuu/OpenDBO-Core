/*****************************************************************************
*
*File			: DboLogic.h
*Author		    : HyungSuk, Jang
*Copyright	    : NTL Co., Ltd.
*Date			: 2005. 11. 08	
*Abstract		: DBO logic.h
*****************************************************************************
*Desc           : 
*
*****************************************************************************/

#ifndef __DBO_LOGIC_H__
#define __DBO_LOGIC_H__

//Resultcode
#include "ResultCodeString.h"

// Shared
#include "NtlItem.h"

// Gui
#include "gui_precomp.h"
#include "gui_surface.h"
#include "gui_texturemanager.h"

// Dbo
#include "DboDef.h"
#include "DialogDefine.h"


struct sSKILL_TBLDAT;
struct sHTB_SET_TBLDAT;
struct sITEM_TBLDAT;
struct sITEM_OPTION_TBLDAT;
struct sGuildWarehouseSlot;

class CNtlPLGui;


extern RwReal	g_fElapsedWeightValue;


BYTE Logic_GetCurrentChannel();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IconMoveManager related procedure logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RwBool Logic_ItemMoveProc(EDragDropType eSrcType, RwInt32 iSrcSlotIdx, RwUInt32 uiSrcSerialId, 
//						  EDragDropType eDestType, RwInt32 iDestSlotIdx, RwUInt32 uiDestSerialId, RwUInt32 uiDestParentSerialId);

RwBool Logic_ItemMoveProc( SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);

// peessi: Modify Logic_ItemMoveSubProc Return value
// desc: Changed to RwBool->RwUInt32(EDispStingType) to diversify monotonous message output. 
//			: Additionally, to unify the item inspection routine, all duplicate elements are removed and collected here.
//			: INVALID_DWORD (no message output, normal operation or PacketLock)
//			: Scouter slot-related subfunctions are deleted and applied as Equip's ScouterSlot.

std::string Logic_ItemMoveSubProcEquipToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcEquipToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcEquipToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcEquipToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx );
std::string Logic_ItemMoveSubProcEquipToGuildWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx );
//RwUInt32 Logic_ItemMoveSubProcBagSlotToEquip	( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcBagSlotToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcBagSlotToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcBagSlotToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx);
std::string Logic_ItemMoveSubProcBagSlotToGuildWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx);
std::string Logic_ItemMoveSubProcBagChildToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcBagChildToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcBagChildToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
//RwUInt32 Logic_ItemMoveSubProcBagChildToScouterSlot(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx);
std::string Logic_ItemMoveSubProcBagChildToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount);
std::string Logic_ItemMoveSubProcBagChildToGuildWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount);
//RwUInt32 Logic_ItemMoveSubProcScouterSlotToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx);
//RwUInt32 Logic_ItemMoveSubProcScouterSlotToWarehouse( SERIAL_HANDLE hSrcSerial, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx);
//RwUInt32 Logic_ItemMoveSubProcScouterSlotToGuildWarehouse( SERIAL_HANDLE hSrcSerial, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx);
std::string Logic_ItemMoveSubProcQuestToQuest( RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx );
std::string Logic_ItemMoveSubProcWarehouseToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx);
std::string Logic_ItemMoveSubProcWarehouseToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount );
std::string Logic_ItemMoveSubProcWarehouseToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount);
std::string Logic_ItemMoveSubProcWarehouseToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount);
//RwUInt32 Logic_ItemMoveSubProcWarehouseToScouterSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx);
std::string Logic_ItemMoveSubProcGuildWarehouseToEquip(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);
std::string Logic_ItemMoveSubProcGuildWarehouseToBagSlot(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);
std::string Logic_ItemMoveSubProcGuildWarehouseToBagChild(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);
//RwUInt32 Logic_ItemMoveSubProcGuildWarehouseToScouterSlot(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);
std::string Logic_ItemMoveSubProcGuildWarehouseToGuildWarehouse(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount = 1);

// Zenny related.
std::string Logic_ItemMoveSubProcSubBagZennyToWarehouseZenny(SERIAL_HANDLE hDestParentSerial, RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcSubBagZennyToGuildWarehouseZenny(SERIAL_HANDLE hDestParentSerial, RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcSubBagZennyToTradeCartZenny(SERIAL_HANDLE hTargetHandle, RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcSubBagZennyToMailSystemZenny(RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcWareHouseZennyToSubBagZenny(RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcGuildWareHouseZennyToSubBagZenny(RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcTradeCartZennyToSubBagZenny(RwUInt32 uiZenny);
std::string Logic_ItemMoveSubProcMailSystemZennyToSubBagZenny(RwUInt32 uiZenny);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Other procedure logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_ItemRepairProc( RwUInt8 ucPlace, RwUInt8 ucSrcSlotIdx );
RwBool Logic_ItemIdentificationProc( RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx );
RwBool Logic_ItemIdentification_in_NPCShop_Proc( RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx );

RwBool Logic_ItemBeadProc(RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx);
RwBool Logic_ItemBeadDestroyProc(RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx);

RwBool Logic_ItemDeleteProc(sMsgDboItemInfo* pInfo, RwBool* pPacketLock );
RwBool Logic_ItemDirectEquipProc( SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx );
RwBool Logic_UseProc( SERIAL_HANDLE hSrcSerial, RwUInt32 param1 = 0xFF, RwUInt32 param2 = INVALID_SERIAL_ID, bool param3 = false);
RwBool Logic_UseProcSub(SERIAL_HANDLE hSrcSerial, RwUInt32 param1 = 0xFF, RwUInt32 param2 = INVALID_SERIAL_ID, bool param3 = false);
int Logic_UseProcSubForSpecialTypeItem(CNtlSob* pSob, RwUInt32 hTarget, CNtlSobIcon* pIcon);
RwBool Logic_UseProcRpBonusSkill( SERIAL_HANDLE hSrcSerial );
RwBool Logic_ItemBagClearProc( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx );

VOID   Logic_ItemDropConfirmProc();

RwInt32 Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( RwUInt8 byCommonPointType);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Event-related logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check whether the corresponding iType is updated
// iType: EEventAvatarInfoUpdateType type in NtlSLEvent.h file
// pData : pData of CMsg
RwBool Logic_IsUpdateType(RwInt32 iType, void* pData);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Texture Loading
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eTEXTURETYPE { TEXTYPE_NONE, TEXTYPE_ICON, TEXTYPE_ILLUST, TEXTYPE_MAP, TEXTYPE_LANDMARK, TEXTTYPE_WARFOG, TEXTTYPE_EMBLEM, TEXTTYPE_DWCCARD};

gui::CTexture* Logic_CreateTexture( RwChar* pFilename, eTEXTURETYPE eTexType = TEXTYPE_ICON, RwChar* pcPathName = NULL);
VOID		   Logic_DeleteTexture( gui::CTexture* pTexture );

std::string Logic_GetSmallIconName(const char* pcIconName);

gui::CSurface& Logic_GetPCClassIconSurface(RwUInt8 byClass, RwBool bOutLine = TRUE );
gui::CSurface& Logic_GetNPCClassIconSurface(RwUInt8 byClass, RwBool bOutLine = TRUE );
gui::CSurface& Logic_GetSmallRPTypeMark( RwInt32 eRpType );
gui::CSurface& Logic_GetBattleAttributeIconSurface(RwUInt8 byAttribute, RwBool bOutLine = TRUE );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check whether passive skill is available or not
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_IsPassiveDashUsePossible(void);

RwBool Logic_IsPassiveChargingUsePossible(void);

RwBool Logic_IsPassiveBlockingUsePossible(void);
RwBool Logic_IsPassiveBlockingUsePossibleCoolTime(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwBool	Logic_IsAirMode();
bool	Logic_IsAirPossible();
RwBool	Logic_IsAirDashPossible(void);
bool	Logic_IsPassiveAirPossible(); // check if we can start air-jump
bool	Logic_IsPassiveAirPossibleUi();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	guitar logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwInt32				Logic_GetPCClassFlagFromPCClass( RwUInt8 ucClass );					// Flag including 1st and 2nd positions
RwInt32				Logic_GetExplicitPCClassFlagFromPCClass( RwUInt8 ucClass );			// Flag that displays only the current class

eCONTAINER_TYPE		Logic_ConvertBagIdxToContainerType	( RwUInt8 ucBagIndex );
RwInt8				Logic_ConvertContainerTypeToBagIdx ( RwUInt8 ucContainerType );
eEQUIP_SLOT_TYPE	Logic_ConvertEquipSlotIdxToType	( RwUInt8 ucEquipSlotIdx );
RwUInt8				Logic_ConvertEquipTypeToSlotIdx	( eEQUIP_SLOT_TYPE eEquipType );
RwBool				Logic_EquipSlotTypeFlagCheck		(RwUInt32 usTypeFlag1, RwUInt32 usTypeFlag2 );
RwUInt32			Logic_ConvertEquipSlotIdxToFlag	( RwUInt8 ucEquipSlotIdx );

// Logic to find out the HANDLE using the server's container number and Pos
SERIAL_HANDLE		Logic_GetItemHandleFromContainerPos( RwUInt8 ucContainerType, RwUInt8 byPos );

bool				Logic_GetItemContainerPosFromHandle(SERIAL_HANDLE hHandle, RwUInt8& rbyPlace, RwUInt8& rbyPos);

RwUInt32			Logic_ConvertClassToClassFlag( RwUInt8 ucClass );

eCONTAINER_TYPE		Logic_ConverWarehouseIdxToContainderType(RwUInt8 byWarehouseIndex);
RwUInt8				Logic_ConverContainerTypeToWarehouseIdx	(eCONTAINER_TYPE eContainerType);

eCONTAINER_TYPE		Logic_ConverGuildWarehouseIdxToContainderType(RwUInt8 byGuildwarehouseIndex);
RwUInt8				Logic_ConverContainerTypeToGuildWarehouseIdx(eCONTAINER_TYPE eContainerType);

RwBool Logic_CanAcceptOthersAsk();

RwBool Logic_IsStackMovable( SERIAL_HANDLE hSrcSerial, SERIAL_HANDLE hDestSerial, RwUInt32 nSplitCount );
RwBool Logic_IsStackMovable_to_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseItem, SERIAL_HANDLE hSrcSerial, RwUInt32 nSplitCount );
RwBool Logic_IsStackMovable_from_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseItem, SERIAL_HANDLE hDestSerial, RwUInt32 nSplitCount );
RwBool Logic_IsStackMovable_from_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseSrcItem, sGuildWarehouseSlot* pGuildWarehouseDestItem, RwUInt32 nSplitCount );
RwBool Logic_IsBagClearable( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx );
RwBool Logic_FindEmptyItemSlot( RwInt32* pBagIndex, RwInt32* pSlotIndex );
RwBool Logic_FindEmptyItemSlot();

RwBool Logic_WorldItemLooting( CNtlSob* pWorldItem );

RwInt32 Logic_WhichBagHasItem( SERIAL_HANDLE hItemSerial );	// Returns which bag the item is in as a serial item. error = -1
CNtlSobItem* Logic_FindInventoryItemByTableId( RwUInt32 tblidx );
RwUInt32	 Logic_FindInventoryItemCountByTableId( RwUInt32 tblidx );
CNtlSobItem* Logic_FindEquipItemByTableId( RwUInt32 tblidx );
CNtlSobItem* Logic_FindInventoryItemMinDurByDurationGroup(RwUInt32 uiDurationGroup);
RwBool		Logic_FindInventoryItemByItemType(eITEM_TYPE eType, BYTE* byPlace, BYTE* byPos);

class CRetCaculDayHourMinSecond
{
public:
	RwUInt32 uiDay;		RwUInt32 uiHour;	RwUInt32 uiMin;		RwUInt32 uiSec;
};
VOID	Logic_CaculDayHourMinSecond( RwUInt32 uiInSecond, CRetCaculDayHourMinSecond* pOut);

VOID	Logic_EnableIcon( RwBool bEnable, RwUInt8 byPlace, RwUInt8 byPos ); // Icon lock mainly used in packet handler.

RwBool IsV2LessLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength);
RwBool IsV2LessorEqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength);
RwBool IsV2MoreLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength);
RwBool IsV2MoreorEqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength);
RwBool IsV2EqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength);

void Logic_NameNotify(const WCHAR* pwcText);

bool Logic_IsUsableCharacterName(const WCHAR* pwcText);
RwBool Logic_IsUsableCharacterName_with_WarningMessage(const WCHAR* pwcText);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	user option load/save
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const WCHAR* Logic_GetActiveServerName(void);

RwBool Logic_MakeLoadGameOptFileName(std::string& strFileName, const WCHAR *pCharName);
RwBool Logic_MakeSaveGameOptFileName(std::string& strFileName, const WCHAR *pCharName);

void Logic_LoadAccountOption(void);
void Logic_SaveAccountOption(void);

void Logic_LoadSystemOption(void);
void Logic_SaveSystemOption(void);

void Logic_LoadGameOption(void);
void Logic_SaveGameOption(void);

void Logic_LoadCharacterOption(void);
void Logic_SaveCharacterOption(void);

void Logic_LoadScouterOption(void);
void Logic_SaveScouterOption(void);

void Logic_LoadQuestOption(void);
void Logic_SaveQuestOption(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Various callback functions of the game.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_CallbackHtmlUserTag(const WCHAR *pTag, RwUInt32 uiId, std::wstring& strOut);
void Logic_CallbackHtmlLinkTag(gui::CComponent* pComponent, RwUInt8 byType, RwUInt32 uiIndex, std::wstring& strFilename);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	string related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const WCHAR* Logic_GetClassName(RwUInt8 byClass);	// return class name
const WCHAR* Logic_GetNPCJobName(RwUInt8 byJob);	// Return NPC class name
const WCHAR* Logic_GetPlayerRaceName(RwUInt8 byRace);	// Returns Player race name
const WCHAR* Logic_GetNPCRaceName(RwUInt8 byRace);	// Return NPC race name
const WCHAR* Logic_GetMobRaceName(RwUInt8 byRace);	// Returns mob race name
const WCHAR* Logic_GetBattleAttributeName(RwUInt8 byAttr);
const WCHAR* Logic_GetSkillName(RwUInt32 tblidx);
const WCHAR* Logic_GetHTBName(RwUInt32 tblidx);
const WCHAR* Logic_GetItemName(RwUInt32 tblidx);
const WCHAR* Logic_GetHoipoiMixSkillName(RwUInt8 byType);	// Return hoipomix skill name
const WCHAR* Logic_GetHoipoiMixRecipeName(RwUInt32 tblIdx);
const WCHAR* Logic_GetTeleportTypeName(RwUInt32 uiTeleportType);
const WCHAR* Logic_GetPartyItemLootingMethod(BYTE byLootingMethod);
const WCHAR* Logic_GetPartyZeniLootingMethod(BYTE byLootingMethod);
const WCHAR* Logic_GetGuildFunctionString(BYTE byFunction);
const WCHAR* GetActionDisplayStringNum(RwUInt32 actionId);

const WCHAR* Logic_GetItemRestrictionString(BYTE byRestrictState);
const WCHAR* Logic_GetItemRestrictionTypeString(BYTE byRestrictType);
const char* Logic_GetItemTypeName(BYTE byItemType);

const WCHAR* Logic_GetItemWorldRuleRestrictionString(BYTE byWorldRule);

RwBool	Logic_GetSystemEffectText( RwUInt32 uiSystemEffectIdx, RwReal fValue, std::wstring& wstrText, RwUInt8 byPercentType, sSKILL_TBLDAT* pSkillData = NULL, RwUInt32* pColor = NULL, RwBool* pBuffShow = NULL );

void Logic_DialogModeMessage(eDialogMode eMode);
std::string Logic_GetWarnningWorldConceptActionMessageIndex(RwUInt32 uiGrade);
void Logic_WarnningWorldConceptActionMessage(RwUInt32 uiGrade);

void Logic_timeToDetailTime(time_t tTime, sDetailTime& sTime);

///< Show the total amount of time
WCHAR* Logic_GetTimeString(RwReal fSecond);
WCHAR* Logic_GetTimeStringWithMillisecond(RwReal fSecond);

// get time days and hours
WCHAR* Logic_GetTimeRemainString(DBOTIME timeRemain);
WCHAR* Logic_GetTimeTotalString(DWORD timeSeconds);

// The display focuses on the perceived time rather than the total amount of time.
WCHAR* Logic_GetCoolTimeString(RwReal fSecond);

///< Get the name of the region using the table index.
RwBool Logic_GetAreaNameFromTblIDX(TBLIDX tblIdx, std::wstring* pstrString);

///< The returned string must be stored in another memory space.
RwChar* Logic_GetFormatString(const RwChar* pcFormat, ...);
WCHAR* Logic_GetFormatWideString(const WCHAR* pwcFormat, ...);

WCHAR* Logic_NumToWchar(int nNumber);
WCHAR* Logic_NumToWchar(float fNumber);
WCHAR* Logic_NumToWchar(DWORD dwNumber);
WCHAR* Logic_NumToWchar(RwUInt32 dwNumber);

//turn number to money ex 1000 -> 1,000
WCHAR*	Logic_FormatZeni(DWORD dwZeni);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Zenny related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwUInt32 Logic_AllRepairCost();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	World Item related.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_WorldItemPick(SERIAL_HANDLE hPickSerial);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Target
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Logic_SobTarget(RwUInt32 hTargetSerial, BYTE byFollow);
//void Logic_SobTarget( RwUInt32 hTargetSerial );
//void Logic_SobTargetRelease(void);
bool Logic_AvatarTarget(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Skill related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwReal Logic_GetHTBTotalPower( sHTB_SET_TBLDAT* pHTBData );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Battle attribute icon and tooltip settings
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_SetBattleAttributeMark( gui::CPanel* pComponent, RwUInt8 byBattleAttribute, RwBool bOutLine = TRUE );
RwUInt32 Logic_GetBattleAttributeColor( RwUInt8 byBattleAttribute );

// Difference in grade between two entities
enum eCounterpartGradeType { CGT_STRONG_1, CGT_SIMILAR, CGT_WEAK_1 };
RwUInt8 Logic_GetCounterpartGradeType(CNtlSobAttr* pOriginalSobAttr, CNtlSobAttr* pCounterpartSobAttr);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Dialog related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// When the size of the client decreases, it leaves the client window.
// Dialog Manager, Side Dialog Manager, Side Icon Manager
// Readjust the positions of unmanaged dialogs.
void Logic_LocateDialog_in_CleintRect(CNtlPLGui* pGui, RwBool bForce = FALSE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Related to the world's best ballroom
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const WCHAR* Logic_GetTBTypeString( RwUInt8 byMainState );
const WCHAR* Logic_GetStringTBMatchType( RwUInt8 byTBMatchType );
const WCHAR* Logic_GetStringTBMatchState( RwUInt8 byTBMatchState );
const WCHAR* Logic_GetAvatarTBJoinState();								// Current avatar participation status
const WCHAR* Logic_GetTBRemainTimeString( BUDOKAITIME tmTime );
const WCHAR* Logic_GetTBDateToDateString( tm tmAtTime, tm tmToTime );
const WCHAR* Logic_GetTBDateString( tm tmTime );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DBOTIME
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const WCHAR* Logic_GetMailRemainTimeString( DBOTIME tmTime );
RwInt32		 Logic_IsMailRemainTimeOverDay( DBOTIME tmTime );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resurrection message box 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Logic_ShowRegenBox(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Video related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Logic_SetOptionMoviePlay();
void Logic_SetOptionMovieStop();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// color related
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwUInt32 Logic_GetItemRankColor( RwUInt8 byRank );

const WCHAR* Logic_GetItemRankName(RwUInt8 byRank);



#endif
