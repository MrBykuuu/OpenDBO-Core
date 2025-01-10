/*****************************************************************************
*
* File			: NtlFSMUtil.h
* Author		: HyungSuk, Jang
* Copyright	    : NTL Co., Ltd.
* Date			: 2006. 11. 28	
* Abstract		: fsm utility
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_FSM_UTIL_H__
#define __NTL_FSM_UTIL_H__

class CNtlFSMStateBase;
class CNtlSobActor;

//Is it possible to move?
RwBool FSMUtil_CanMoveState(CNtlFSMStateBase *pStateBase);

// Is an attack possible?
RwBool FSMUtil_CanAttackState(CNtlFSMStateBase *pStateBase);

// Are you ready to be attacked?
RwBool FSMUtil_CanHurtState(CNtlFSMStateBase *pStateBase);

// Is the skill available?
RwBool FSMUtil_CanSkillUse(CNtlFSMStateBase *pStateBase);

// Is it idle?
RwBool FSMUtil_IsIdle(CNtlFSMStateBase *pStateBase);	

// Is it on the move?
RwBool FSMUtil_IsMove(CNtlFSMStateBase *pStateBase);		

// Is it a private store?
RwBool FSMUtil_IsPrivateShopping(CNtlFSMStateBase *pStateBase);

// Is it charging?
RwBool FSMUtil_IsCharging(CNtlFSMStateBase *pStateBase);

// Are you in a teleporting state?
RwBool FSMUtil_IsTeleporting(CNtlFSMStateBase *pStateBase);

// Are you on the bus?
RwBool FSMUtil_IsOnBus(CNtlFSMStateBase* pStateBase);

// Are you on board the vehicle?
RwBool FSMUtil_IsOnVehicle(CNtlFSMStateBase* pStateBase);



// 
RwBool FSMUtil_IsStandingAdjustMoveClass(CNtlSobActor *pActor);

RwBool FSMUtil_StandingAdjustMove(CNtlFSMStateBase* pStateBase, CNtlSobActor *pActor);

#endif
