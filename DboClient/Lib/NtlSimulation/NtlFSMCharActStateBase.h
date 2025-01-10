/*****************************************************************************
 *
 * File			: NtlFSMStateBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 2. 13	
 * Abstract		: Finite state machine character action base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_CHARACTSTATE_BASE_H__
#define __NTL_FSM_CHARACTSTATE_BASE_H__

#include "NtlFSMStateBase.h"

class CNtlFSMCharActStateBase : public CNtlFSMStateBase
{
protected:

	typedef RwUInt32 (CNtlFSMCharActStateBase::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;

protected:

	virtual RwUInt32 EventProcActionSitAndStand(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapJump(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapAirJump(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapSkillUse(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapHTBUse(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapItemUse(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapSocialUse(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapPrivateShopOpen(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapCharging(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapBlocking(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcActionMapAutoFollow(RWS::CMsg &pMsg);

	virtual RwUInt32 EventProcInputTerrainClick(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputKeyboardMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputKeyboardMoveStop(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputKeyboardDashMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputMouseDashMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputAirDashMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcInputAirAccelMove(RWS::CMsg& pMsg);
	virtual RwUInt32 EventProcInputChangeHeading(RWS::CMsg &pMsg);

	virtual RwUInt32 EventProcSobTargetSelect(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobAttackSelect(RWS::CMsg &pMsg);

	virtual RwUInt32 EventProcSobMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSecondDestMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSplineMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobDash(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobFollowMove(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobMoveStop(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobAttack(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSpecialAttack(RWS::CMsg& pMsg);
	virtual RwUInt32 EventProcSobAttacked(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobHit(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSit(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobJump(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobAirJump(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobAirAccel(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobJumpDirection(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobJumpEnd(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobDirectPlay(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobAnimPlay(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobPrivateShop(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobPrivateShopState(RWS::CMsg &pMsg);
    virtual RwUInt32 EventProcSobPushingNfy(RWS::CMsg& pMsg);

	virtual RwUInt32 EventProcSobSkillCancel(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSkillCasting(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSkillAction(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSkillActioned(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobHTBSkillAction(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobSocialAction(RWS::CMsg &pMsg);

	virtual RwUInt32 EventProcSobItemCancel(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobItemCasting(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobFainting(RWS::CMsg &pMsg);
	virtual RwUInt32 EventProcSobWeightTime(RWS::CMsg &pMsg);

	//Condition processing related
    virtual RwUInt32 EventSobTransformSequela(RWS::CMsg& pMsg);             ///< Handling of aftereffects of transformation for Super Saiyan only
    virtual RwUInt32 EventProcAfterEffectRemainTime(RWS::CMsg& pMsg);       ///< Processing of transformation aftereffects event
    virtual RwUInt32 EventProcCondConfused(RWS::CMsg& pMsg);                ///< Handling confusion
    virtual RwUInt32 EventProcCondTerror(RWS::CMsg& pMsg);                  ///< Handle fear state


protected:

	CNtlFSMCharActStateBase();

public:

	virtual ~CNtlFSMCharActStateBase() {}

	virtual RwBool Create(void);
	virtual void Destroy(void);

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	/**
    *  state clone function.
    */
	virtual void Clone(CNtlFSMStateBase *pState);
};

#endif