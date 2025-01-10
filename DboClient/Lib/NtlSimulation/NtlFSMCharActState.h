/*****************************************************************************
 *
 * File			: NtlFSMStateBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 13	
 * Abstract		: Finite state machine character action base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_CHARACTSTATE_H__
#define __NTL_FSM_CHARACTSTATE_H__

#include "NtlFSMCharActStateBase.h"
#include "NtlSoundDefines.h"
#include "NtlMath.h"
#include "NtlFrameSkipScheduler.h"

struct SHitStuff;

class CNtlSobElapsedController;
class CNtlSobProxySystemEffect;
class CNtlBehaviorCharIdlePattern;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMCharActCreateState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActCreateState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActCreateState)

private:

	RwUInt8 m_byReadyCnt;

public:

	CNtlFSMCharActCreateState();

	/**
	* State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);


	virtual RwBool IsActorActiveState(void);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMCharActSpawnState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSpawnState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSpawnState)

public:

	CNtlFSMCharActSpawnState();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);

	virtual RwBool IsActorActiveState(void);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActIdleState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActIdleState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActIdleState)

protected:

	enum 
	{
		STATE_IDLE_TOPDOWN,
		STATE_IDLE_SLIP,
		STATE_IDLE_LOOP
	};

	RwBool	m_bWaterIn;
	RwBool	m_bAdjustMoveCheck;
	RwUInt8 m_byIdleState;
	RwReal	m_fGravitySpeed;
	RwReal	m_fIdleCurrTime;
	RwReal	m_fIdleTime;
    RwReal  m_fCalcHeightTime;

	CNtlPLEntity *m_pPLSwimIdle;

	CNtlBehaviorCharIdlePattern *m_pIdlePattern;

protected:

	virtual void SetAnim(RwUInt8 byAnimOrder);
	RwBool CheckSwimming(void);

	void UpdateTopDown(RwReal fElapsed);
	void UpdateSlipping(RwReal fElapsed);
	void UpdateLoop(RwReal fElapsed);
    void UpdateCalcHeight(RwReal fElapsed);             ///< Recalculate the height.

    void UpdateTerror(RwReal fElapsed);                 ///< In case of fear, handle it.


	virtual void ChangeIdleState(RwUInt8 byState);

public:

	CNtlFSMCharActIdleState();
	~CNtlFSMCharActIdleState();

	/**
    *State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActMoveState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActMoveState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActMoveState)

protected:

	HFRAMESKIPID	m_hFrameSkipId;
	RwReal			m_fUpdateElapsed;

public:

	CNtlFSMCharActMoveState();
	~CNtlFSMCharActMoveState();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActMoveSwimming : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActMoveSwimming, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActMoveSwimming)

public:

	CNtlFSMCharActMoveSwimming();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActAdjustMoveState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActAdjustMoveState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActAdjustMoveState)

public:

	CNtlFSMCharActAdjustMoveState();

	/**
	/*  State update/event processing function.                                                                     
	*/
	virtual void Enter(void);
	virtual void Exit(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharGroundMove;

class CNtlFSMCharActDashState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActDashState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActDashState)

private:

	CNtlBehaviorCharGroundMove *m_pGroundMove;

public:

	CNtlFSMCharActDashState();

	/**
	/*  State update/event processing function.                                                                     
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSitDownState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSitDownState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSitDownState)

public:

	CNtlFSMCharActSitDownState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSitState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSitState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSitState)

public:

	CNtlFSMCharActSitState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActStandUpState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActStandUpState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActStandUpState)

public:

	CNtlFSMCharActStandUpState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSmoothHeadingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSmoothHeadingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSmoothHeadingState)

public:

	CNtlFSMCharActSmoothHeadingState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActTriggerOperateState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActTriggerOperateState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActTriggerOperateState)

private:

	SERIAL_HANDLE	m_hTargetSerialId;

	void SetAnim(void);

public:

	CNtlFSMCharActTriggerOperateState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActJumpState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActJumpState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActJumpState)

private:

	RwBool m_bMoveQueueing;

	HFRAMESKIPID	m_hFrameSkipId;
	RwReal			m_fUpdateElapsed;

public:

	CNtlFSMCharActJumpState();
	~CNtlFSMCharActJumpState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActAirJumpState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActAirJumpState, NTL_DEFAULT_MEMORY_POOL)
		DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActAirJumpState)

private:

	HFRAMESKIPID	m_hFrameSkipId;
	RwReal			m_fUpdateElapsed;

public:

	CNtlFSMCharActAirJumpState();
	~CNtlFSMCharActAirJumpState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFallingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFallingState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFallingState)

public:

	CNtlFSMCharActFallingState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActChargingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActChargingState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActChargingState)

private:

	RwBool m_bCharging;
	RwReal m_fTime;

public:

	CNtlFSMCharActChargingState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActBlockingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActBlockingState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActBlockingState)

private:

	RwBool	m_bEquipedStuff;
	RwBool	m_bGuardCrush;
	RwReal	m_fCrushTime;

public:

	CNtlFSMCharActBlockingState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFollowState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFollowState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFollowState)

public:

	CNtlFSMCharActFollowState();

	/**
	  *State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:
    RwUInt8 m_byMoveForm;                       ///< Current moving object type (ground/swimming)
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFightingFollowState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFightingFollowState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFightingFollowState)

public:

	CNtlFSMCharActFightingFollowState();

	/**
    *State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:
    RwUInt8 m_byMoveForm;                       ///< Current moving object type (ground/swimming)
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFightingPoseState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFightingPoseState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFightingPoseState)

private:

	RwReal m_fTime;
    RwBool m_bIsPlayInterAni;                   ///< Flag to determine whether interpolated animation is playing
	RwBool m_bAdjustMoveCheck;

public:

	CNtlFSMCharActFightingPoseState();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

    void    SetPose2IdleAnim();                 ///< Play the interpolated animation to Idle.
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobAttackSyncStatus;

enum EAnimHitDelayState
{
	EAHD_NONE,
	EAHD_PAUSE,
	EAHD_FAST,
	EAHD_FINISH
};

class CAnimHitDelay
{
	DECLEAR_MEMORY_POOL(CAnimHitDelay, NTL_DEFAULT_MEMORY_POOL)

private:

	
	EAnimHitDelayState	m_eState;
	CNtlSobActor		*m_pActor;
	RwReal				m_fCurrTime;
	RwReal				m_fHitDelay;

private:

	void	EnterState(EAnimHitDelayState eState);
	void	ExitState(EAnimHitDelayState eState);

	void	ChangeState(EAnimHitDelayState eState);

public:

	CAnimHitDelay();
	~CAnimHitDelay();

	void	Update(RwReal fElapsed);
	RwBool	IsFinish(void);

	void	SetAnimHitDelay(CNtlSobActor *pActor, RwReal fDelayTime);
};

class CNtlFSMCharActFightingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFightingState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFightingState)

public:

	CNtlFSMCharActFightingState();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFightingShrinkState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFightingShrinkState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFightingShrinkState)

private:
	RwReal m_fTime;

public:

	CNtlFSMCharActFightingShrinkState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActHurtState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActHurtState, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActHurtState)

public:

	CNtlFSMCharActHurtState();

	/**
    *  State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActKnockDownState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActKnockDownState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActKnockDownState)

private:

	RwBool m_bDirectEnd;   /** When creating a character on the server, the status is knockdown. */

public:

	CNtlFSMCharActKnockDownState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSlidingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSlidingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSlidingState)

public:

	CNtlFSMCharActSlidingState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActStunState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActStunState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActStunState)

public:

	CNtlFSMCharActStunState();

	/**
	  *State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:    
    void    OnStone(RwBool bEffect);                      ///< Processing of petrified state
    void    OnCandy(RwBool bEffect);                      ///< Candy state handling

protected:
    BYTE              m_byStunType;                               ///< Stun type eDBO_STUN_TYPE
    RwReal            m_fOrgAnimSpeed;                            ///< Original animation speed
    RwBool            m_bApplyStun;
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSleepState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSleepState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSleepState)

public:

	CNtlFSMCharActSleepState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActParalyzeState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActParalyzeState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActParalyzeState)

public:

	CNtlFSMCharActParalyzeState();

	/**
	*  State update/event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

private:

	float	m_fOrigAnimSpeed;
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSkillFollowState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSkillFollowState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSkillFollowState)

public:

	CNtlFSMCharActSkillFollowState();

	virtual void Enter(void);
	virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:
    RwBool  CheckInFollowRange();               ///< Check whether you are within the skill usage range.

protected:
    RwUInt8 m_byMoveForm;                       ///< Current moving object type (ground/swimming)

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSkillFocusState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSkillFocusState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSkillFocusState)

private:

public:

	CNtlFSMCharActSkillFocusState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSkillReadyState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSkillReadyState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSkillReadyState)

private:

	RwBool m_bSkillReq;
	RwReal m_fTime;

public:

	CNtlFSMCharActSkillReadyState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSkillCastingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSkillCastingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSkillCastingState)

private:

	enum 
	{
		CASTING_STATE_START,
		CASTING_STATE_LOOP
	};

	RwUInt8 m_byCastingState;
	RwBool	m_bSkillSuccess;
    RwReal  m_fOcclusionCheckTime;                      ///< Time to check object occlusion during casting
	
private:

	void SetTrans(void);
	void SetAnim(void);    
	
public:

	CNtlFSMCharActSkillCastingState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMCharActSkillActionState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSkillActionState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSkillActionState)

private:	

	SERIAL_HANDLE	m_hPetSerialId;

    // Skill cancel related
	RwBool			m_bAnimCancelPoint;
    SERIAL_HANDLE   m_uiSkillTblId;               ///< Table ID of skill
    RwBool          m_bRightFinish;               ///< Whether it has been canceled normally without being canceled

public:

	CNtlFSMCharActSkillActionState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:    
};

/**
* \ingroup NtlSimulation
* \brief Skill Action Loop status
*
* \date 2008-11-20
* \author agebreak
*/
class CNtlFSMCharKeepingEffect : public CNtlFSMCharActStateBase
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharKeepingEffect, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharKeepingEffect)

public:
    CNtlFSMCharKeepingEffect();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:

	UINT32    m_uiAnimLoopId;
	UINT32    m_uiAnimEndId;
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActSocialActionState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActSocialActionState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActSocialActionState)

public:

	CNtlFSMCharActSocialActionState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActHTBFollowState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActHTBFollowState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActHTBFollowState)

public:

	CNtlFSMCharActHTBFollowState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActHTBReadyState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActHTBReadyState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActHTBReadyState)

public:

	CNtlFSMCharActHTBReadyState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActHTBState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActHTBState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActHTBState)

private:

	enum EHtbState
	{
		EHTB_PROGRESS,
		EHTB_FINISH,
		EHTB_CANCEL
	};

	RwBool		m_bSimulationMode;
	RwBool		m_byHTPStepState;
	RwUInt8		m_byHTBNextStep;
	RwUInt8		m_byFirstActiveStep;	// The step where skill is produced for the first time
	RwBool		m_bHTBSuccess;
	RwUInt32	m_uiSkillTblId;		// Skill ID of the htb set currently in use
	RwUInt8		m_byHTBSetType;
	CNtlSobElapsedController *m_pHTBElapController;
	CNtlSobElapsedController *m_pSandbagElapController;

private:

	void EnableSimulation(RwBool bEnable);
	
	RwReal	GetFirstSkillRange(void);
	void	NextStepProc(void);
	RwBool	StepCancelProc(void);
	RwUInt8 FindFirstActiveStep(void);
	RwBool	IsFinishHTBStep(void);
	
public:

	CNtlFSMCharActHTBState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActHTBSandbagState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActHTBSandbagState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActHTBSandbagState)

private:

	RwBool	m_bFinishSandbag;
	RwUInt8	m_bySetType;
    RwBool  m_bSobFaint;                        ///< Did he die while being hit?    

private:

	void DetermineBehavior(RWS::CMsg &pMsg);

	void BehaviorKnockDownProc(void);
	void BehaviorTossUp(void);
	void BehaviorTossDown(void);
	void BehaviorHomingDown(void);

public:

	CNtlFSMCharActHTBSandbagState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlFSMCharActItemReadyState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActItemReadyState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActItemReadyState)

public:

	CNtlFSMCharActItemReadyState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActItemCastingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActItemCastingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActItemCastingState)

private:

	CNtlPLEntity *m_pPLEntity;
    RwUInt32      m_tblUseItemTableIndex;

public:

	CNtlFSMCharActItemCastingState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActFaintingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActFaintingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActFaintingState)

public:

	CNtlFSMCharActFaintingState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActDieState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActDieState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActDieState)

private:

	enum EDieState
	{
		DIE_FADEOUT,
		DIE_END
	};

	RwBool m_bDirectDie;		/**  This is used when the character needs to go into a die state as soon as it is first created. */
	RwUInt8 m_byDieState;		/**  die state */
	
	void UpdateFadeOut(RwReal fElapsed);

public:

	CNtlFSMCharActDieState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActTeleportState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActTeleportState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActTeleportState)

private:
	CNtlSobProxySystemEffect* m_pVisualSystemEffect;

public:

	CNtlFSMCharActTeleportState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool IsActorActiveState(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActPrivateShopState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActPrivateShopState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActPrivateShopState)

private:

	RwBool	m_bDirectShop;
    CNtlSobProxySystemEffect* pSystemEffect;


public:

	CNtlFSMCharActPrivateShopState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActDirectionState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActDirectionState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActDirectionState)

private:

	enum 
	{
		FSMDS_NORMAL_UPDATE,
		FSMDS_NORMAL_TMQENTER_FAIL
	};

	RwBool						m_bTimeLoop;
	RwReal						m_fLoopTime;
	RwReal						m_fCurrTime;
	RwUInt8						m_byState;
	RwUInt8						m_byFuncFlag;

	CNtlSobProxySystemEffect	*m_pSpawnAlpha;

private:

	void				NormalUpdate(RwReal fElapsed);
	void				TMQEnterFailUpdate(RwReal fElapsed);

public:

	CNtlFSMCharActDirectionState();

	virtual void		Enter(void);
	virtual void		Exit(void);
	virtual void		Update(RwReal fElapsed);
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActDespawnState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActDespawnState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActDespawnState)

private:

	enum EDespawnType
	{
		EDESPAWN_NONE,
		EDESPAWN_DESPAWN1,
		EDESPAWN_DRAGONBALL_NPC,
		EDESPAWN_NPC_PORTAL
	};

	enum EDespawnNoneState
	{
		DESPAWN_NONE_START,
		DESPAWN_NONE_END
	};

	enum EDespawn1State
	{
		DESPAWN1_START,
		DESPAWN1_END
	};

	enum EDespawnDragonBallNpcState
	{
		DESPAWN_DRAGONBALL_FADEOUT,
		DESPAWN_DRAGONBALL_END
	};

	enum EDespawnNpcPortal
	{
		DESPAWN_NPC_PORTAL_CAMERA_RESET,
		DESPAWN_NPC_PORTAL_START,
		DESPAWN_NPC_PORTAL_END
	};
	
	RwUInt8						m_byType;
	RwUInt8						m_byState;		/**  die state */
	
	CNtlSobProxySystemEffect	*m_pDespawn1Alpha;

private:
	
	void						UpdateDespawn1(RwReal fElapsed);
	void						UpdateDragonBallNpc(RwReal fElapsed);
	void						UpdateDragonBallNpcFadeOut(RwReal fElapsed);
	void						UpdateNpcPortal(RwReal fElapsed);

	void						ExitDragonBallNpc(void);
    void						OnEffectDragonDespawn(void);                ///< Processing of extinction effect used in case of dragon god
    void                        CreateDespawnEffect(CNtlSobActor* pActor);  ///< Creates a despawn effect.

public:

	CNtlFSMCharActDespawnState();

	virtual void				Enter(void);
	virtual void				Exit(void);
	virtual void				Update(RwReal fElapsed);
	virtual RwUInt32			HandleEvents(RWS::CMsg &pMsg);

};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActLeavingState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActLeavingState, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActLeavingState)

private:


public:

	CNtlFSMCharActLeavingState();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

// on the bus
// 2008.04.07 by agebreak
class CNtlFSMCharActOnBus : public CNtlFSMCharActStateBase
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharActOnBus, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActOnBus)

public:
    CNtlFSMCharActOnBus();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);    
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

    void    SetRideOn(RwBool bRideOn, RwBool bCreateEffect = TRUE);

protected:    

	enum { BUS_RIDE_ON, BUS_RIDE_OFF_FALLING, BUS_DIDE_OFF_LANDING };

    CNtlSobActor*   m_pTargetActor;
	RwUInt8			m_byRideState;
	RwV3d			m_vRideOffPos;
	RwReal			m_fFallingSpeed;
    CNtlSobProxySystemEffect* m_pSystemEffect;
};

// on a vehicle
// 2008.11.08 by lleo52
class CNtlFSMCharActOnVehicle : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActOnVehicle, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActOnVehicle)

public:
	CNtlFSMCharActOnVehicle(); 

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	void    SetRideOn(RwBool bRideOn, RwV3d vPos = ZeroAxis);

protected:    

	enum { VEHICLE_RIDE_ON, VEHICLE_RIDE_OFF_FALLING, VEHICLE_DIDE_OFF_LANDING };

	CNtlSobActor*   m_pTargetActor;
	RwUInt8			m_byRideState;
	RwV3d			m_vRideOffPos;
	RwReal			m_fFallingSpeed;
};

// State where the bus (NPC) is rotating
// 2008.04.14 by agebreak
class CNtlFSMCharActTurning : public CNtlFSMCharActStateBase
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharActTurning, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActTurning)

public:
    CNtlFSMCharActTurning();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:        
    void    SetNextDirection(RwV3d& vDir);
};

// Transformation aftereffects after Super Saiyan transformation
// 2008.04.17 by agebreak
class CNtlFSMCharTransformSequela : public CNtlFSMCharActIdleState
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharTransformSequela, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharTransformSequela)

public:
    CNtlFSMCharTransformSequela();

    virtual void Enter();
    virtual void Exit();
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:
    virtual void SetAnim(RwUInt8 byAnimOrder);
    virtual void ChangeIdleState(RwUInt8 byState);

protected:
};

/**
 * \ingroup NtlSimulation
 * \brief Spin Attack Status
 *
 * \date 2008-10-21
 * \author agebreak
 */
class CNtlFSMCharSpinAttack : public CNtlFSMCharActStateBase
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharSpinAttack, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharSpinAttack)

public:
    CNtlFSMCharSpinAttack();
    ~CNtlFSMCharSpinAttack();

    virtual void Enter();
    virtual void Exit();
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:

protected:
    CNtlBehaviorCharGroundMove *m_pMoveBehavior;
};


class CNtlFSMCharRollingAttack : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharRollingAttack, NTL_SMALL_MEMORY_POOL)
		DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharRollingAttack)

public:
	CNtlFSMCharRollingAttack();
	~CNtlFSMCharRollingAttack();

	virtual void Enter();
	virtual void Exit();
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:

protected:
	CNtlBehaviorCharGroundMove *m_pMoveBehavior;
};



/**
 * \ingroup NtlSimulation
 * \brief State of being pushed by a spinning attack
 *
 * \date 2008-10-28
 * \author agebreak
 */
class CNtlFSMCharPushing : public CNtlFSMCharActStateBase
{
    DECLEAR_MEMORY_POOL(CNtlFSMCharPushing, NTL_SMALL_MEMORY_POOL)
    DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharPushing)

public:
    CNtlFSMCharPushing();
    ~CNtlFSMCharPushing();

    virtual void Enter();
    virtual void Exit();
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

protected:
protected:
};

/**
* \ingroup NtlSimulation
* \brief The state of getting on or off a vehicle
*
* \date 2008-11-06
* \author lleo52
*/
class CNtlFSMCharRideOnOff : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharRideOnOff, NTL_SMALL_MEMORY_POOL)
	DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharRideOnOff)

protected:

public:
	CNtlFSMCharRideOnOff();
	~CNtlFSMCharRideOnOff();

	virtual void Enter();
	virtual void Exit();
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMCharActAirAccelState : public CNtlFSMCharActStateBase
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActAirAccelState, NTL_DEFAULT_MEMORY_POOL)
		DECLEAR_FSMSTATE_FACTROY(CNtlFSMCharActAirAccelState)

protected:

	HFRAMESKIPID	m_hFrameSkipId;
	RwReal			m_fUpdateElapsed;

public:

	CNtlFSMCharActAirAccelState();
	~CNtlFSMCharActAirAccelState();

	/**
	*  state update / event processing function.
	*/
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};



#endif
