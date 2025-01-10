/*****************************************************************************
 *
 * File			: NtlSLPacketGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: simulation layer simulation object packet generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLPACKET_GENERATOR_H__
#define __NTL_SLPACKET_GENERATOR_H__

#include "NtlPacketGenerator.h"
#include "NtlSLDef.h"
#include "NtlQuest.h"
#include "NtlMail.h"
#include "NtlCharacter.h"

class CNtlSobActor;

class CNtlSLPacketGenerator : public CNtlPacketGenerator
{
public:

	CNtlSLPacketGenerator(CNtlNetSender *pNetSender);

	bool SendLoadingCompleteNfy(void);

	bool SendCharSpawnReady(void);

	bool SendCharReady(RwUInt8 byAvatarType);

	bool SendPetAttackTargetNfy(RwUInt8 byAvatarType);

	bool SendPetDismissReq(RwUInt8 byAvatarType);

	//Movement
	bool SendMouseMove(RwV3d vCurr, RwV3d vDest, RwUInt8 byAvatarType);

	bool SendKeyboardMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag, RwUInt8 byAvatarType);
	bool SendKeyboardAirMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag);

	bool SendMouseDashMove(RwV3d vDest);

	bool SendKeyboardDashMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag);

	bool SendAirDashMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveDirection);
	bool SendAirAccelMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveDirection);

	bool SendChangePosition(RwV3d vCurr, RwV3d vDir);

	bool SendChangeHeading(RwV3d vDir);

	bool SendCharMoveCollision(CNtlSobActor* pActor);
	bool SendCharMoveCollisionEnd(CNtlSobActor* pActor);

	bool SendCharTargetSelect(SERIAL_HANDLE hSerialId);

	bool SendCharTargetFacing(SERIAL_HANDLE hSerialId, bool bStart);

	bool SendCharFollowMove(SERIAL_HANDLE hTargetSerialId, RwReal fFollowRange, RwUInt8 byMovementReason, RwUInt8 byAvatarType);

	// Jump
	bool SendJump(RwUInt8 byMoveDirection, RwV3d vLoc, RwV3d vDir);

	bool SendAirJump(RwUInt8 byMoveDirection, RwV3d vLoc, RwV3d vDir);

	bool SendChangeDirectionFloating(RwUInt8 byMoveDirection, RwV3d vDir);

	bool SendJumpEnd(void);

	// Charging
	bool SendCharging(void);

	bool SendChargingEnd(void);

	bool SendBlocking(void);

	bool SendBlockingEnd(void);

	// sit/stand
	bool SendSitAndStand(RwBool bSitDown);

	// Falling
	bool SendFalling(RwBool bFalling, RwV3d vCurrPos, RwV3d vCurrDir, RwUInt8 byMoveDirection);

	//Air
	bool SendAirFalling(RwV3d vCurrPos, RwV3d vCurrDir, RwUInt8 byMoveDirection);
	bool SendAirEnd(RwV3d vCurrPos, RwV3d vCurrDir);

	// attack
	bool SendCharToggleFighting(RwBool bFightingMode, RwUInt8 byAvatarType);
	bool SendCharAttackBegin(RwUInt8 byType, RwUInt8 byAvatarType);
	bool SendCharAttackEnd(RwUInt8 byType, RwUInt8 byAvatarType);
	bool SendCharSkillReq(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx, RwUInt8 byRpBonusType, RwUInt8 byAvatarType);
	bool SendCharSkillTargetList(RwUInt8 byAvatarType);
	bool SendCharSocialReq(RwUInt32 uiTblId);
	bool SendCharHTBReq(SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx);
	bool SendCharHTBForward(RwV3d vCurr, RwV3d vDir);
	bool SendCharItemUseReq(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerialId, RwUInt8 byPlace, RwUInt8 byPos, SERIAL_HANDLE hRefObject, RwUInt8 byKeyPlace, RwUInt8 byKeyPos);
	bool SendCharTeleportReq(void);
	bool SendCharDirectPlayCancel();
	bool SendCharDirectPlayAck(void);
    bool SendCharSkillCastingCanceledNfy(WORD uiResultCode);

    // Movement-related processing when taking damage (knockdown, sliding, push)
    bool SendCharLocAfterKnockdownNfy(const RwV3d& vLoc, const RwV3d& vDir);
    bool SendCharLocAfterSlidingNfy(const RwV3d& vLoc, const RwV3d& vDir);
    bool SendCharLocAfterPushNfy(const RwV3d& vLoc, const RwV3d& vDir);

	// bind
	bool SendCharBindReq( TBLIDX bindObjectTblidx );

	// quest
	bool SendTSConfirmStepReq( BYTE byTSType, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, RwUInt32* uiParam, RwUInt8 byEventType, RwUInt32 uiEventData );
	bool SendQuestGiveUpReq( NTL_TS_T_ID tId );
	bool SendTSUpdateState( NTL_TS_T_ID tId, RwUInt8 byTSType, RwUInt8 byType, RwUInt16 wTSState, RwUInt32 uiParam );
	bool SendTSExcuteTriggerObject( HOBJECT hSource, HOBJECT hTarget, RwUInt8 byEvtGenType, RwUInt32 uiParam );
	bool SendQuestShare( NTL_TS_T_ID tId );
	bool SendQuestObjectVisitReq( NTL_TS_T_ID tId, RwUInt32 uiWorldID, RwUInt8 byObjType, RwUInt32 uiObjectTblIdx );

	// Private shop
	bool SendPrivateShopCreateReq();

	// Action map
	bool SendCharKeyUpdateReq(sSHORTCUT_UPDATE_DATA* pUpdateData, BYTE byCount);
	
	// Mail system
	bool SendMailLoadReq(RwUInt32 hSerialID, RwUInt8 uiCount, MAILID aMailID[NTL_MAX_MAIL_SLOT_COUNT]);

	// share target
	bool SendCrossFireReq(RwUInt32 hSerialID);

    // buses and vehicles
    bool SendRideOnBus(RwUInt32 hTargetId);                     ///< Request to board the bus.    
    bool SendRideOffBus();                                      ///< Request to get off the bus.    

	// Vehicle
	bool SendVehicle_EngineFuelRemoveNfy();
	bool SendVehicle_EndReq();
	bool SendVehicle_FuelInsertReq(BYTE byPlace, BYTE byPos);

	bool SendVehicleDirectPlayCancelNfy();

	bool SendVehicleStuntNfy();

	// Related to the world's best ballroom
	bool SendBudokaiMudosaTeleport(RwUInt8 byMudosaIndex);
	bool SendBudokaiJoinStateReq();
	bool SendBudokaiSocialAction(RwUInt32 uiAnimId);

	// HTB Member
	bool SendHTBRPBallUseReq(RwUInt8 byBallCount);
};


#endif