/*****************************************************************************
 *
 * File			: NtlSLLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2005. 11. 10	
 * Abstract		: simulation logic.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLLOGIC_H__
#define __NTL_SLLOGIC_H__

#include "NtlSharedType.h"
#include "NtlSLLogicDef.h"
#include "NtlSLEvent.h"
#include "NtlSLServerDeclear.h"
#include "NtlAnimEventData.h"

struct sWORLD_INFO;
struct sDOJO_INFO;
struct sEmblemFactor;

class CNtlSob;
class CNtlSobActor;
class CNtlSobPlayer;
class CNtlSobNpc;
class CNtlSobElapsedController;
class CNtlSobItem;
class CNtlSobItemAttr;
class CNtlSobVehicle;

class CNtlStorageUnit;


extern RwReal	g_fCharColliAngle;
extern RwReal	g_fCharSlippingAngle;
extern RwUInt32	g_uiFlashBalloonWidth;
extern RwUInt32 g_uiFlashBalloonHeight;
extern RwReal	g_fMatrixTime;
extern RwReal	g_fMatrixValue;
extern RwReal	g_fMatrixDelayTime;
extern RwReal	g_fMatrixCamera;
extern RwBool	g_bDispPacketLockMsg;
extern RwBool	g_bDispServerMsg;

extern RwUInt8	g_byShadowRed;
extern RwUInt8	g_byShadowGreen;
extern RwUInt8	g_byShadowBlue;

extern RwReal   g_fTestVal;             /// Global variables used in various tests
extern RwReal   g_fTestVal2;
extern RwReal   g_fTestVal3;

extern RwInt32	g_nTestVal;

extern RwInt32	g_nBroadCastGuiPosX;

extern RwUInt32 g_uiTargetMarkingId;    ///< ID of current target marketing object

extern RwBool	g_bActiveAnimSync;

extern RwUInt32	g_uiLimitMemory;

extern RwUInt32	g_uiCreateHitEffectRate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register function pointer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef const WCHAR* (*Callback_GetNPCJobName)(RwUInt8 byJob);
extern Callback_GetNPCJobName g_fnGetNPCJobName;

void LinkCallback_GetNPCJobName(Callback_GetNPCJobName fn);
const WCHAR* Logic_GetSLNPCJobName(RwUInt8 byJob);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_GetCurMemory(void);

void   Logic_CalcLimitMemory(SIZE_T TotalPhysMemory);

RwBool Logic_IsCreateHitEffect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// game master key definition
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_IsDevUser(void);

RwBool Logic_IsUIDevInfoVisible(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// monster/player logic
// Note: Speed ??will be based on table data, status, and buffs.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor attribute logic

// Name
const WCHAR* Logic_GetName(CNtlSob *pSobObj);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LP
void		Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp);

void		Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp, RwUInt32 uiMaxLp);

void		Logic_SetLpPlusMinus(CNtlSob *pSobObj, RwInt32 iDeltaLp);

RwUInt32	Logic_GetLp(CNtlSob *pSobObj);

RwUInt32	Logic_GetMaxLp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EP
void		Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp);

void		Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp, RwUInt32 uiMaxEp);

RwUInt32	Logic_GetEp(CNtlSob *pSobObj);

RwUInt32	Logic_GetMaxEp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RP
void		Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp);
void		Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp, RwUInt32 uiMaxRp);
void        Logic_SetRpStock(CNtlSob* pSobObj, RwUInt32 uiRpStock);                 
void        Logic_SetRpStockMax(CNtlSob* pSobObj, RwUInt32 uiRpStockMax);
RwUInt32	Logic_GetRp(CNtlSob *pSobObj);
RwUInt32	Logic_GetMaxRp(CNtlSob *pSobObj);
RwUInt32    Logic_GetRpStock(CNtlSob* pSobObj);
RwUInt32    Logic_GetMaxRpStock(CNtlSob* pSobObj);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AP

void		Logic_SetAp(CNtlSob *pSobObj, int nAp, int nMaxAp);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Level
void		Logic_SetLevel(CNtlSob *pSobObj, RwUInt8 byLevel);

RwUInt8		Logic_GetLevel(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exp
void		Logic_SetExp(CNtlSob *pSobObj, RwUInt32 uiExp);

RwUInt32	Logic_GetExp(CNtlSob *pSobObj);

void		Logic_SetMaxExp(CNtlSob *pSobObj, RwUInt32 uiMaxExp);

RwUInt32	Logic_GetMaxExp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Zenny
void		Logic_SetZenny(RwUInt32 uiZenny);
RwUInt32	Logic_GetZenny(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HLS Cash
void		Logic_SetHlsCash(DWORD dwCash);
DWORD		Logic_GetHlsCash(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sp
void		Logic_SetSp( RwUInt32 uiSp );

RwUInt32	Logic_GetSp(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Honor Point
void		Logic_SetHonorPoint( RwUInt32 uiHonorPoint );
RwUInt32	Logic_GetHonorPoint(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mudosa Point
void		Logic_SetMudosaPoint( RwUInt32 uiMudosaPoint );
RwUInt32	Logic_GetMudosaPoint(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// avatar status value
void		Logic_SetStatus(CNtlSob *pSobObj, RwUInt8 byAttributeTotalCount, RwUInt8 *pData);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Speed
void		Logic_SetSpeed(CNtlSobActor *pActor, RwReal fWalkSpeed, RwReal fRunSpeed);
RwReal		Logic_GetDefaultFrontWalkSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFrontWalkSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDefaultFrontRunSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFrontRunSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDefaultBackSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetBackSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetTurningSpeed(CNtlSobActor *pActor);
RwBool		Logic_IsActorWalkMove(CNtlSobActor *pActor, RwUInt8 byMoveForm);
void		Logic_SetAttackAnimSpeed(CNtlSobActor *pActor, RwReal fAnimSpeed);
RwReal		Logic_GetAttackAnimSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetSkillAnimSpeed(CNtlSobActor *pActor, sSKILL_TBLDAT *pSkilTblData);
RwReal      Logic_GetRunAnimSpeed(CNtlSobActor* pActor);
RwReal		Logic_GetFrontFlySpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFlyDashSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFlyAccelSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDashSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDashAnimSpeed(CNtlSobActor* pActor);

float		Logic_GetMaxDashDistanceForward(CNtlSobActor *pActor);
float		Logic_GetMaxDashDistanceBackLeftRight(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// player job change 
RwUInt8		Logic_GetPlayerRace(CNtlSobActor *pActor);

RwUInt8		Logic_GetPlayerClass(CNtlSobActor *pActor);

RwUInt8     Logic_GetPlayerGender(CNtlSobActor *pActor);

void		Logic_SetReputation(CNtlSobActor *pActor, RwUInt32 uiReputation);

RwUInt32	Logic_GetReputation(CNtlSobActor *pActor);

RwUInt8		Logic_ConverClasstoRace(RwUInt8 byClass);

RwBool		Logic_IsChild(CNtlSobActor *pActor);

RwBool		Logic_IsFirstClass(CNtlSobPlayer* pPlayer);	// Is it a primary position?

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attack , npc community , skill use range

RwReal		Logic_GetRadius(CNtlSobActor *pActor);
RwReal		Logic_GetAttackRange(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);
RwReal		Logic_GetCommunityRange(CNtlSobActor *pActor);
RwReal		Logic_GetCommunityOutRange(CNtlSobActor *pActor);
void		Logic_BeginActorCommunity(CNtlSobActor *pActor, CNtlSobActor *pTarActor); 
RwReal		Logic_GetSkillUseRange(sSKILL_TBLDAT *pSkillTblData);
RwReal      Logic_GetSkillApplyRange(CNtlSobActor* pActor, RwUInt32 uiSkillTblidx);         /// Range Returns the range of the skill.
RwBool		Logic_IsMasterSkill(SERIAL_HANDLE hSkillSerial);
RwReal		Logic_GetCurrentCastingTime(CNtlSobActor *pActor);
RwReal		Logic_GetMaxCastingTime(CNtlSobActor *pActor);
void		Logic_SetSkillReqFlag(RwBool bFlag);
void		Logic_UpdateSkillResTimeOut(RwReal fElapsed);

float		Logic_GetRange(RwV3d& vPos, RwV3d& vTargetPos);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Active, HTB Skill Time Notifier

RwReal		Logic_GetCastingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetCoolingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetKeepingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetDOTValueFactor(CNtlSobActor* pActor);
RwReal		Logic_GetDOTTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetRequiredEPFactor(CNtlSobActor* pActor);
//RwReal		Logic_GetCastingTimeModifier(CNtlSobActor* pActor);
//RwReal		Logic_GetCoolingTimeModifier(CNtlSobActor* pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State and mode of actor

RwBool		Logic_IsCommunityActor(CNtlSobActor *pActor);           // Is the actor a community actor? (npc, trigger object, world item)
RwBool		Logic_IsGambleActor(CNtlSobActor *pActor);
RwBool		Logic_IsOperationTriggerActor(CNtlSobActor *pActor);    // Is it an operation trigger actor?
RwUInt32	Logic_GetDirectIdTriggerActor(CNtlSobActor *pActor);
RwUInt32	Logic_GetActorStateId(CNtlSobActor *pActor);            // Pass the actor's state id.
RwUInt32    Logic_GetActorStateFlags(CNtlSobActor* pActor);          // Returns the Actor's current State Flag.
SERIAL_HANDLE Logic_GetActorTargetSerialId(CNtlSobActor *pActor);   // The serial id that the actor is targeting
RwBool		Logic_IsActorBattleMode(CNtlSobActor *pActor);          // actor Are you currently in combat?
RwBool		Logic_CanSkillUse(CNtlSobActor *pActor);                // Is the actor able to use skills?
RwBool		Logic_IsNotFollowMoveSkill(CNtlSobActor *pActor, RwUInt8 bySlotIdx);
void		Logic_GetAvatarSkillApplyArea(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, std::list<SERIAL_HANDLE>& listApplyActor);      // area skill
void		Logic_GetAvatarSkillApplyArea(SERIAL_HANDLE hTargetSerial, RwUInt32 uiSkillId, std::list<SERIAL_HANDLE>& listApplyActor);       // area skill 
RwBool		Logic_IsEnemyTargetFromActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);        // Is the target an enemy to the actor?
RwBool		Logic_IsEnemyTargetFromAvatarActor(SERIAL_HANDLE hTargetSerial);        // Is the target hostile to the avatar?
RwBool		Logic_IsEnemyTagetFromPetActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);      // Is the target an enemy to the pet actor?
RwBool      Logic_isEnemyTargetOcclusion(SERIAL_HANDLE hTargetSerial);  ///< Is the target covered by a shield?
CNtlSobActor* Logic_FindNearEnemyFromAvatarActor(void);             // Finds the nearest enemy for an avatar.
CNtlSobActor* Logic_GetAutoTarget(CNtlSobActor* pActor);            ///< Find automatic targeting partner.
RwBool      Logic_IsBlackList(RwUInt32 uiSerialID);                 // Check if the other character is on your blocked list.
RwBool      Logic_IsDirectMode(CNtlSobActor* pActor);               ///< Check if it is in Direct Mode (out of control).
RwBool      Logic_CanTranslateIdleState(CNtlSobActor* pActor);      ///< Is it possible to transition to the Idle state? 
RwBool      Logic_CanTargetted(CNtlSobActor* pActor);               ///< Is targeting possible?
RwBool      Logic_CanAttacked(CNtlSobActor* pActor);		//< Check if target can be attacked
void        Logic_SetTargetMarkingID(RwUInt32 uiSerialId);          ///< Check target marked ID
RwUInt32    Logic_GetTargetMarkingID();
void		Logic_GetSkillFinalLocation(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, RwV3d* vFinalLoc, RwV3d* vFinalSubjectLoc);
void		Logic_GetSkillFinalLocation2(SERIAL_HANDLE hTargetSerial, sSKILL_TBLDAT *pSkillTblData, RwV3d * vFinalLoc, RwV3d * vFinalSubjectLoc);

void        Logic_SetActorAnimation(RwUInt32 uiSerialID, RwUInt32 uiAnimID, RwBool bLoop);             ///< Set the animation.

RwBool		Logic_IsMovePatternApply(CNtlSobActor* pActor);
void		Logic_GetMovePatternSync(RwUInt32 uiSerialID, RwUInt8 byPatternId);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ground fly height
RwBool		Logic_IsGroundFlyActor(CNtlSobActor *pActor);

RwReal		Logic_GetGroundFlyHeight(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hit Mark

const RwChar* Logic_GetVisualHitMark(CNtlSobActor *pActor, EAttackType eAttackType, RwBool bCritical, RwBool bPowerEffect);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NPC

void		Logic_MakeMobBrief(sMOB_BRIEF *pBrief, TBLIDX tblidx);

void		Logic_MakeNPCBrief(sNPC_BRIEF *pBrief, TBLIDX tblidx);

void		Logic_MakeMobSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir);

void		Logic_MakeNPCSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir);

///< Returns table data with NPC Serial number.
sNPC_TBLDAT* Logic_GetNPCDataFromSob(SERIAL_HANDLE hSerial);

TBLIDX		Logic_GetNPCTriggerId(CNtlSob *pSobObj);

// Current quest NPC is gone 
RwReal		Logic_GetQuestNpcDistanceFromAvatar(CNtlSob *pSobNpc);

RwBool		Logic_IsDragonBallNPC(TBLIDX tblidx);

RwBool		Logic_IsDragonBallNPCActor(SERIAL_HANDLE hSerialId);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trigger object

// quest and trigger agency

RwBool		Logic_IsTriggerActive(CNtlSob *pSobObj);

TBLIDX		Logic_GetTriggerObjectId(CNtlSob *pSobObj);

RwBool		Logic_IsNormalQuest( RwUInt16 tID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Object

TBLIDX		Logic_GetDynamicObjTriggerId(CNtlSob* pSobObj);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item

void			Logic_SetItemStack(CNtlSobItem* pSobItem, RwUInt8 ucStackNum);

// Obtain the item table mounted on the weapon equip slot.
sITEM_TBLDAT*	Logic_GetEquipedWeaponItemTableData(CNtlSob *pSobObj);

// Obtain the item serial mounted on the corresponding equip slot.
SERIAL_HANDLE	Logic_GetEquipedItemSerial(CNtlSob *pSobObj, RwUInt8 bySlotIdx);

// Is the item table a staff weapon?
RwBool			Logic_IsEquipedStaffWeapon(sITEM_TBLDAT *pItemTblData);

RwUInt32        Logic_GetIdleAnimationID(CNtlSob* pSobObj);     ///< Returns an IDLE animation appropriate for the weapon equipped state.

// Obtain the item serial mounted on the sub weapon equip slot.
SERIAL_HANDLE	Logic_GetEquipedSubWeaponItemSerial(CNtlSob *pSobObj);

// Is it a movable sub weapon?
RwBool			Logic_IsMoveableSubWeapon(sITEM_TBLDAT *pItemTblData); 

// Is it a movable sub weapon?
RwBool			Logic_IsMoveableSubWeapon(TBLIDX tblidx);

// Shouldn't the sub weapon be attached to the character in the active state?
RwBool			Logic_IsActiveNotAttachSubWeapon(TBLIDX tblidx);

// Is it a sub weapon that needs to be changed from the main weapon?
RwBool			Logic_IsWeaponChangeSubWeapon(TBLIDX tblidx);

//Getting item grade -Makoto
RwUInt8         Logic_GetBootsGrade(CNtlSob* pSobObj);

// Is it a costume item?
RwBool			Logic_IsCostumeItem(sITEM_TBLDAT *pItemTblData);

// Should the current item be hidden?
RwBool			Logic_IsHideCostumeItem(RwUInt16 wHideBigFlags, void *pData);

// With items that can be sold
RwBool			Logic_IsCanSellItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSellItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSellItem( sITEM_PROFILE* pItemProfile );

// Check if item can be discarded
RwBool			Logic_IsCanDiscardItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanDiscardItem(CNtlSobItemAttr* pSobItemAttr);
RwBool			Logic_IsCanDiscardItem(sITEM_PROFILE* pItemProfile);

// Is this an item that can be traded between users?
RwBool			Logic_IsCanUserTradeItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanUserTradeItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanUserTradeItem( sITEM_PROFILE* pItemProfile );

RwBool			Logic_IsCanUserAuctionItem(CNtlSobItemAttr* pSobItemAttr);

// Is this an item that can be stored in the warehouse?
RwBool			Logic_IsCanSaveWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveWarehouse( sITEM_PROFILE* pItemProfile );

// Is this an item that can be stored in a shared warehouse?
RwBool			Logic_IsCanSaveCommonWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveCommonWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveCommonWarehouse( sITEM_PROFILE* pItemProfile );

// Is it an item with an expiration date?
RwBool			Logic_IsTimeLimitItem( CNtlSobItemAttr* pItemAttr );
RwBool			Logic_IsTimeLimitItem( sITEM_PROFILE* pItemProfile );
RwBool			Logic_IsTimeLimitItem( sITEM_TBLDAT* pITEM_TBLDAT );



// Returns the number of empty slots in all bags
RwInt32			Logic_GetCountEmptySlotofBag();

// Starting from the first bag, find the first empty slot and return it.
void			Logic_GetFirstEmptySlot_of_Bag(RwUInt32& uiBagIndex, SERIAL_HANDLE& hBagHandle, RwUInt32& uiSlot_of_bag);

// Finds and returns the first empty slot starting from the first warehouse.
void			Logic_GetFirstEmptySlot_of_Warehouse(RwUInt8& byWarehouseIndex, SERIAL_HANDLE& hWarehouseHandle,
													 RwUInt8& bySlot_of_warehouse);

// If the item with the handle received as an argument is in the warehouse, the index and slot number are returned.
RwBool			Logic_FindWarehouseItem(SERIAL_HANDLE hItem, RwUInt8& byWarehouseIndex, RwUInt8& bySlot_of_warehouse);

// Finds and returns the first empty slot starting from the first guild warehouse.
void			Logic_GetFirstEmptySlot_of_GuildWarehouse(RwUInt8& byGuildWarehouseIndex, RwUInt8& bySlot_of_GuildWarehouse);

// First slot of empty scouter
RwUInt8			Logic_GetFirstEmptyScouterSlot();

// Returns the handle of the first item in the bag equal to the item index received as an argument.
SERIAL_HANDLE	Logic_FirstItem_in_Bag(TBLIDX index);


///< Returns table data with Charm Serial number.
sCHARM_TBLDAT*	Logic_GetCharmDataFromTable(TBLIDX index);

///< Returns table data by MERCHANT Serial number.
sMERCHANT_TBLDAT* Logic_GetMerchantDataFromTable(TBLIDX index);

///< Returns table data by Item Serial or Sob number.
sITEM_TBLDAT*	Logic_GetItemDataFromTable(TBLIDX index);
sITEM_TBLDAT*	Logic_GetItemDataFromSob(SERIAL_HANDLE hSerial);

///< Returns SobItemAttr as Item Serial.
CNtlSobItemAttr* Logic_GetItemSobAttrFromSobSerial( SERIAL_HANDLE hSerial );

// Check whether the corresponding scouter part type is installed.
bool			Logic_ExistScouterEffect(int nEffectCode);

bool			Logic_IsValidScouterEquipped(bool bShowByDefault);


RwReal			Logic_GetItemSystemEffectValue(TBLIDX idxItem, RwUInt32 uiSystemEffect);
RwReal			Logic_GetItemSystemEffectValue(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiSystemEffect);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Skill

sSKILL_TBLDAT*	Logic_GetSkillDataFromTable(TBLIDX index);
RwBool          Logic_IsSkillSystemEffectCode(TBLIDX index, RwUInt32 uiSystemEffect);               ///< Check if there is a system effect code for the skill.
RwReal          Logic_GetSkillSystemEffectValue(TBLIDX index, RwUInt32 uiSystemEffect);             ///< Returns the skill°Øs system effect value.
void			Logic_SetSkillExp(RwUInt32 uiSkillSlotIdx, RwUInt16 wExp);
void			Logic_SetActiveRpSkillBonus(RwUInt8 byRpBonusType);
RwUInt8			Logic_GetActiveRpSkillBonus(void);
RwBool          Logic_IsCastingTargetSkill(TBLIDX tblidx);                      ///< Is this a skill that displays the casting target? (Mega Dodongpa)
RwBool			Logic_IsStrongCastSkill(TBLIDX tblidx);							///< Is this a strong casting skill?
RwBool			Logic_IsSkillUseAfterAttack(SERIAL_HANDLE hSkillSerial);

void			Logic_SkillInfoText(WCHAR* wchMsg, unsigned int uiColor, BYTE byMsgType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Target

// serial avatar

CNtlSobActor*	Logic_GetAvatarActor(void);

SERIAL_HANDLE	Logic_GetAvatarHandle(void);

SERIAL_HANDLE	Logic_GetAvatarTargetHandle(void);

bool			Logic_SobTarget(RwUInt32 hTargetSerial, BYTE byFollow);

const WCHAR*	Logic_GetAvatarName(void);

// A function to find the heading with the target actor.
void			Logic_CharHeading(CNtlSobActor *pActor, SERIAL_HANDLE uiTargetSeriaIId);

// Is the target currently dead?
RwBool			Logic_CharIsDie(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarTargetStateChange(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarStateChange(void);

RwBool          Logic_IsBehind(CNtlSob* pSob, RwV3d& vTargetPos);   /// Determine if you are behind.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor math logic

RwBool			Logic_InFollowRange(CNtlSobActor *pActor, CNtlSobActor *pTarActor, RwReal fFollowRange);

RwBool			Logic_InFollowRange(CNtlSobActor *pActor, const RwV3d& v3TargetPos, RwReal fFollowRange);

RwBool			Logic_CanActorCommunity(CNtlSobActor* pActor, CNtlSobActor* pTarActor, RwReal fFollowRange);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor world presenation coordinate

RwBool			Logic_GetWorldHeight(const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal = NULL);

RwBool			Logic_GetWorldHeight(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal = NULL);

void			Logic_GetWorldSlop(CNtlSobActor *pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold = 30.f, RwReal fDownThreshold = 30.f);

void			Logic_GetWorldSlopForVehicle(CNtlSobVehicle* pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold = 30.f, RwReal fDownThreshold = 30.f);

RwReal			Logic_GetSwimmingOffset(CNtlSobActor *pActor);

RwReal			Logic_GetSwimmingHeight(CNtlSobActor *pActor, const RwV3d *pPos);

RwBool			Logic_IsSwimmingActor(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff);

RwBool			Logic_IsSwimming(CNtlSobActor *pActor, const RwV3d *pPos, RwReal fWorldHeight, RwReal fWaterHeight);

const RwV3d*	Logic_GetCameraPos(const RwCamera *pCamera);

const RwV3d*	Logic_GetCameraAt(const RwCamera *pCamera);

const RwV3d*	Logic_GetCameraUp(const RwCamera *pCamera);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// character collision 

void			Logic_EnableCharacterCollisionSlant(RwBool bSlantCheck);
void			Logic_EnableCharacterSlipping(RwBool bSlippingCheck);
RwUInt8			Logic_CharacterCollisionEx(CNtlSobActor *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump = FALSE);
RwBool			Logic_CharacterSlipping(CNtlSobActor *pActor, RwV3d& vNewPos, RwReal fElapsed);
RwBool			Logic_IsMoveWorldArea(CNtlSobActor *pActor, RwV3d vCurrPos, RwV3d vDestPos);                    ///< Examine terrain properties to determine whether the area is moveable.
RwBool          Logic_IsCollisionLine(RwV3d& vStartPos, RwV3d& vEndPos, OUT CNtlPLEntity* pColEntity = NULL);                                        ///< Checks whether there is an object collision between two points.

RwUInt8			Logic_VehicleCollisionEx(CNtlSobVehicle *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump = FALSE);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor

CNtlSobPlayer*	Logic_GetPlayer_from_CharID(CHARACTERID charID);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commuity

// Whether you are the leader of the party
RwBool			Logic_I_am_PartyLeader();

// Whether you are a member of the same party as yourself
RwBool			Logic_IsMyPartyMember(SERIAL_HANDLE hSerial);

RwBool			Logic_CanPartyLeave();

RwBool			Logic_CanPartyInvite();

// Whether you are a guild leader
RwBool			Logic_I_am_GuildLeader();

// Whether you are a deputy guild leader
RwBool			Logic_I_am_SecondGuildLeader();

// Check if it is included in the party sharing target. If included, the slot number is returned; if not, -1 is returned.
RwInt32         Logic_isPartyShareTarget(SERIAL_HANDLE hSerial);
RwUInt32        Logic_GetShareTargetSerial(RwInt32 nSlot);
RwInt32         Logic_GetShareTargetBlankSlot();

sDOJO_INFO*		Logic_GetDojoInfo(SERIAL_HANDLE hNPC);

eDBO_TEAM		Logic_GetScrambleTeam(eTOBJECT_STATE_TYPE_C eTObjectState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world information

RwUInt8			Logic_GetActiveWorldRuleType(void);

bool			Logic_GetActiveWorldAllowFlight();

sWORLD_INFO*	Logic_GetActiveWorldInfo(void);

HOBJECT			Logic_GetCurrentWorldTriggerOffsetHandle(void);

RwUInt32		Logic_GetActiveWorldId(void);

RwUInt32		Logic_GetActiveWorldTableId(void);

const RwChar*	Logic_GetActiveWorldName(void);

// Color the name above an entity in the world based on the entity's state.
void			Logic_SetHeadNameColor(CNtlSob* pSob);

void			Logic_SetNickNameColor(CNtlSob* pSob);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// privateshop
// Can I open a store?
RwBool			Logic_IsCanCreatePrivateShop();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TMQ
RwBool			Logic_IsTMQBoarding(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
RwBool			Logic_IsCoolTimeUpdate(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direction processing function.

RwBool			Logic_ProcKnockdownMatrixDirection(CNtlSob *pSobObj, RwUInt32 uiMatrixSerialId, const SHitStuff *pHitStuff);
RwBool          Logic_IsCinematic();            ///< Returns whether a cinematic is currently being produced.


// Setting whether clickable or not
void            Logic_SetClickDisable(CNtlSobActor* pActor, RwBool bDisable);
RwBool          Logic_IsClickDisable();

// Auto Run
void			Logic_SetAutoRun(RwBool bAutoRun);

//////////////////////////////////////////////////////////////////////////
// Logic related to buses and vehicles
RwBool          Logic_IsBus(CNtlSobActor* pSobObj);             /// Check whether the object is a bus.
RwBool			Logic_IsBus(SERIAL_HANDLE hHandle);

RwBool          Logic_IsVehicleDriver(CNtlSobActor* pSobObj);   /// Check whether the actor in question is the driver of the vehicle.
RwBool			Logic_IsVehicleDriver(SERIAL_HANDLE hHandle);

// TRUE: Rideable or the item is not a Vehicle item
// FALSE: Boarding is not possible
RwBool			Logic_CanRideOnVehicle(SERIAL_HANDLE hHandle, CNtlSobItem* pSobItem);

TBLIDX			Logic_GetRegisterFuelItemTableIndex();

bool			Logic_IsAirMode(CNtlSobActor* pSobObj); //is flying

//////////////////////////////////////////////////////////////////////////
// flag
void			Logic_SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
void			Logic_UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool			Logic_IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

//////////////////////////////////////////////////////////////////////////
// Transformation-related logic
RwBool          Logic_IsTransform(CNtlSobActor* pSobActor);         ///< Check whether the transformation has occurred.
RwUInt32        Logic_IsTransformSkill(RwUInt32 uiSkillTblId);      ///< Check whether the skill is a transformation skill.
RwBool          Logic_IsTransformGreatNamek(CNtlSobActor* pSobActor);    ///< Check if it is a giant Namek.         (Made separately at the request of the team leader by agebreak)
RwBool          Logic_IsTransformSpinAttack(CNtlSobActor* pSobActor);   ///< Check to see if a rotation attack is in progress.      (Rotation attack is transformed on the server)
RwBool          Logic_IsTransformRollingAttack(CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// tutorial
RwBool			Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE byType);
RwBool			Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE byType);
RwBool			Logic_CanAttack_in_Tutorial(ETL_ATTACK_TYPE byType);

void			Logic_SendTutorialCondition(ETL_CONDITION_TYPE byType, RwUInt32 uiParam1 = 0xffffffff );

//
BYTE			 Logic_GetServerState(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// Condition processing related
RwBool      Logic_IsCondition(CNtlSobActor* pActor, RwUInt64 uiConditon);         ///< Check if the relevant condition exists.
RwBool      Logic_RandomMove(CNtlSobActor* pActor, RwReal fDistance);             ///< Moves the actor randomly.


void		Logic_ShowHTBRPSelectGui(CNtlSobActor* pActor, SERIAL_HANDLE hTargetSerial);
void		Logic_ShowSandBagRPSelectGUI(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// Apply function
bool		Logic_ApplyNtlStorageUnit( CNtlStorageUnit* pUnit, unsigned int uiFlags );


//////////////////////////////////////////////////////////////////////////
// PC room related functions
void        Logic_SetNetPy(DWORD dwNetPy);
DWORD       Logic_GetNetPy();

//////////////////////////////////////////////////////////////////////////
// Sob object visual related

// Change the color of the character's actual uniform or obtain the color.
// The uniform color of the SobPlayer object must be changed directly using the SobPlayer function.
RwUInt8		Logic_GetSobPlayerDogiColor(RwUInt32 hSobPlayerHandle);
RwUInt8		Logic_GetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwBool bUICharacter = FALSE);
RwBool		Logic_SetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwUInt8 byColorIndex, RwBool bUICharacter = FALSE, RwBool bForce = FALSE);

RwBool		Logic_GetEmblemFactor(sEmblemFactor* pOutEmblemFactor, CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// Quantity/index related

// Check whether the index can be saved
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

// Check whether the index can be stored including invalid values.
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

// Check whether the index can actually be used for reference.
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

//////////////////////////////////////////////////////////////////////////
// Camera controller
RwBool Logic_IsNPCCameraMode(void);

void	Logic_CancelNpcFacing();


//////////////////////////////////////////////////////////////////////////
// sound related

SOUND_HANDLE	Logic_PlayGUISound(char* pcFileName);

// Makes a sound when picking up or placing an item in a normal slot.
void			Logic_PlayItemPickUpSound(sITEM_TBLDAT* pITEM_TBLDAT);
void			Logic_PlayItemPutDownSound(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt8 byDestPlace = 0);

//
bool			Logic_CanRequestTrade();
bool			Logic_CanRequestFreeBattle();


#endif
