/*****************************************************************************
 *
 * File			: NtlBehaviorChar.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: character behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_CHAR_H__
#define __NTL_BEHAVIOR_CHAR_H__

#include "NtlBehaviorBase.h"
#include "NtlBehaviorData.h"
#include "NtlAnimEventData.h"

class CNtlSobProxySystemEffect;
class CNtlPLCharacter;
class CNtlSobVehicle;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharSpawn : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharSpawn, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSpawn)

private:

	enum ESpawnType
	{
		SPAWN_NORMAL,
		SPAWN_SPECIAL,
		SPAWN_TMQ_IN
	};

	RwUInt8	m_byType;
	RwBool	m_bEnterShow;    
	RwBool	m_bSpawnAnimExist;
	RwReal	m_fTime;    
    RwReal	m_fSpawnDelayTime;

	CNtlSobProxySystemEffect	*m_pVSEAlpha;

	void	SetSpawnType(void);
	
	void	SetNormalSpawnAnim(RwReal fSpawnTime);
	void	SetNormalSpawnVisualEffect(void);

	void	SetSpecialSpawnVisualEffect(void);

	void	EnterNormalSpawn(void);
	void	EnterSpecialSpawn(void);
	void	EnterTMQInSpawn(void);

	void	ExitNormalSpawn(void);
	void	ExitTMQInSpawn(void);

	void	UpdateNormalSpawn(RwReal fElapsed);
	void	UpdateTMQSpawn(RwReal fElapsed);
	
public:

	CNtlBehaviorCharSpawn(); 
	~CNtlBehaviorCharSpawn();

	virtual void		Enter(void);
	virtual void		Exit(void);
	virtual void		Update(RwReal fElapsed);
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorSmoothHeading : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorSmoothHeading, 1)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorSmoothHeading)

private:

	RwReal m_fDeltaAngleY;
	RwReal m_fUpdateAngleY;

	bool	m_bSetAngleX;
	RwReal m_fDeltaAngleX;
	RwReal m_fUpdateAngleX;

	void SetAnim(RwReal fDeltaAngleY, RwReal fDeltaAngleX);

public:

	CNtlBehaviorSmoothHeading();
	~CNtlBehaviorSmoothHeading();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Normal attack.

class CNtlBehaviorCharPushFollow;

class CNtlBehaviorCharNormalAttack : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharNormalAttack, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharNormalAttack)

protected:

#define NTL_MAX_HITGROUP	NTL_MAX_NUMBER_OF_SKILL_TARGET
#define NTL_MAX_HITNUM		16
	
	RwV3d			m_vPushDir;
	RwInt32			m_iHitGroupNum;
	RwInt32			m_iHitNum;
	RwUInt8			m_iHitIdx;
	SHitStuff		*m_ppHit[NTL_MAX_HITGROUP][NTL_MAX_HITNUM];
	RwReal			m_fHitTime[NTL_MAX_HITNUM];
	RwReal			m_fAnimTime;					/** attack animation full time */
	RwReal			m_fAnimSpeed;					/** attack animation speed */

	CNtlBehaviorCharPushFollow *m_pPushFollow;


protected:

	void				Init(void);
	void				Delete(void);
		
	void				SetTransform(const SAttackStuff *pAttackStuff);
	void				SetAnim(const SAttackStuff *pAttackStuff);
	void				SetAnimNoAttackStuff(void);
	void				CalcHitStuff(const SAttackStuff *pAttackStuff, RwInt8 byHitGroupIdx);
	RwUInt8				CalcPowerHitNum(void);
	SAttackDamageStuff	CalcDamage(SAttackDamageStuff sTotDamageStuff, RwUInt8 byTotHitNum, RwUInt8 byPowerHitNum, RwBool bPowerHit);

	void				CreatePushFollow(RwBool bOneStepFollow);
	void				DeletePushFollow(void);

	void				EventProcPhysicHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx);
	void				EventProcProjectileHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx);
	void				EventProcHit(RwBool bOneStepFollow);
	void				EventProcSkillAffectingCancel(void);

public:

	CNtlBehaviorCharNormalAttack();
	~CNtlBehaviorCharNormalAttack();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};





///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Normal attack.

class CNtlBehaviorCharPushFollow : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharPushFollow, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharPushFollow)

protected:

	RwReal m_fPushSpeed;
	RwReal m_fCurrDist;
	RwV3d m_vPushDir;

public:

	CNtlBehaviorCharPushFollow();
	~CNtlBehaviorCharPushFollow();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	
public:

	void SetPushSpeed(RwReal fPushSpeed);
	void SetPushDir(const RwV3d *pPushDir);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharPush : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharPush, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharPush)

public:
    CNtlBehaviorCharPush();
    ~CNtlBehaviorCharPush();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual void Exit(void);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

private:
    void CreateTimeSequence(void);          ///< push sequence node creation function.
    void DeleteTimeSequence(void);          ///< push sequence node deletion function. 
    RwReal GetMaxTime(void) const;          ///< Life time of the object.
    RwReal GetMaxDist(void) const;          ///< Maximum distance of movement.
    void   UpdatePosition(RwReal fElapsed);                ///< Update the location.

private:
	RwV3d m_vPushDir;		/** push direction */
	RwReal m_fPrevDist;     /** Since distance based on time is used, the previous dist is stored. */
	RwReal m_fTime;			/** push cumulative time */
	RwReal m_fDistScale;    /** In the time sequence, the time and distance are fixed, but the push distance is not fixed. */
	RwV3d  m_vDestPos;		/** Final position to move due to push */

	CNtlDistTimeSequence *m_pSeq;	/** push sequence node */
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharSliding : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharSliding, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSliding)

private:

	enum 
	{
		SS_SLIDING_LADING,
		SS_SLIDING_STOP
	};
	
	RwUInt32 m_uiSlidingState;	/** sliding state */
	RwV3d m_vPushDir;			/** sliding direction */
	RwReal m_fPrevDist;		    /** Since distance based on time is used, the previous dist is stored. */
	RwReal m_fTime;				/** sliding cumulative time */
	RwReal m_fDistScale;		/** In a time sequence, the time and distance are fixed, but the sliding distance is not fixed. */
	RwV3d  m_vDestPos;			/** Final position to move to due to sliding */

	CNtlDistTimeSequence *m_pSeq;	/** sliding sequence node */

private:

	/**
	*Sliding sequence node creation function.
	*/
	void CreateTimeSequence(void);

	/**
	*Sliding sequence node deletion function. 
	*/
	void DeleteTimeSequence(void);

	/**
	*Life time of the object.
	*/
	RwReal GetMaxTime(void) const;

	/**
	*Maximum distance of movement.
	*/
	RwReal GetMaxDist(void) const;

	/**
	*update sliding landing.
	*/
	void UpdateSlidingLanding(RwReal fElapsed);

	/**
	*update sliding stop.
	*/
	void UpdateSlidingStop(RwReal fElapsed);

	/**
	*Calculating sliding coordinates.
	*/
	void UpdatePosition(RwReal fTime);

	/**
	*Function that changes the sliding state.
	*/
	void ChangeSlidingState(RwUInt32 uiState);

public:

	/**
	*constructor
	*/
	CNtlBehaviorCharSliding();

	/**
	*destructor
	*/
	~CNtlBehaviorCharSliding();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	/**
	*Because the object is used by referring to CNtlBeCharData object information, it is used when updating a sliding object.
	*When another sliding command is received while the current sliding behavior is in operation. Information update.
	*/
	virtual void UpdateData(void);

public:

	void SetSlidingDir(const RwV3d *pSlidingDir);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharShock : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharShock, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharShock)

private:

	RwV3d m_vPushDir;		/** shock direction */
	RwV3d m_vDestPos;		/** Final position to move due to shock */
	RwReal m_fTime;			/** shock cumulative time */
	RwReal m_fDistScale;    /** In the time sequence, the time and distance are fixed, but the shock distance is not fixed. */
		
	CNtlDistTimeSequence *m_pSeq;

private:

	/**
	*hit shock sequence node creation function.
	*/
	void CreateTimeSequence(void);

	/**
	*hit shock sequence node deletion function. 
	*/
	void DeleteTimeSequence(void);

	/**
	*hit shock object data reset function.
	*/
	void ResetTimeSequence(void);

	/**
	*Time sequence node update function.
	*param1: Immediately previous cumulative time.
	*param2: Current accumulated time.
	*/
	void UpdateTimeSequence(RwReal fOldTime, RwReal fCurrTime);

	/**
	*Life time of the object.
	*/
	RwReal GetMaxTime(void) const;

	/**
	*Maximum distance of movement.
	*/
	RwReal GetMaxDist(void) const;

public:

	/**
	*Constructor.
	*/
	CNtlBehaviorCharShock();

	/**
	*Destructor.
	*/
	~CNtlBehaviorCharShock();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	/**
	*Because the object is used by referring to CNtlBeCharData object information, it is used when updating the shock object.
	*When another shock command is received while the current shock behavior is in operation. Information update.
	*/
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//
class CNtlBehaviorCharKnockDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharKnockDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharKnockDown)

public:
	enum 
	{
		KD_FALLUP,
		KD_LANDING,
		KD_FALLDOWN,
		KD_KNOCK,
		KD_KNOCK_EXIT
	};

public:
    CNtlBehaviorCharKnockDown();
    ~CNtlBehaviorCharKnockDown();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual void Exit(void);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

    /**
    *Since the object is used by referring to the CNtlBeCharData object information, it is used when updating the knock down object.
    *When another knock down command is received while the current knock down behavior is in effect. Information update.
    */
    virtual void UpdateData(void);
    void ChangeKnockDownState(RwUInt32 uiState);    ///< Function that transitions the state of knockdown bahavior.
    void SetKnockDownDir(const RwV3d *pKnockDownDir);   ///< knock down direction.

private:
	void CreateFallupTimeSequence(void);    ///< knockdown fall up sequence node creation function.
	void CreateLandingTimeSequence(void);   ///< knockdown landing sequence node creation function.
	void DeleteTimeSequence(void);          ///< knockdown sequence node deletion function. 
	void UpdateFallUp(RwReal fElapsed);     ///< knockdown fall up update 함수.
	void UpdateLanding(RwReal fElapsed);    ///< knockdown landing update function.
	void UpdateFallDown(RwReal fElapsed);   ///< knockdown fall down update 함수.
	void UpdateKnock(RwReal fElapsed);      ///< update function when knockdown is down.
	void CalcLanding(RwReal fElapsed);      ///< Calculate landing state.
    RwV3d UpdatePosition(RwReal fElapsed, RwV3d& vPos, RwReal fSpeed);   ///< Calculate position excluding y value (collision handling)

protected:
    RwUInt32 m_uiKnockDownState;		/** knock down state */
    RwBool m_bStanding;					/** knock down standing */
    RwBool m_bFainting;					/** knock down fainting */

    RwReal m_fTime;						/** known down cumulative time */
    RwV3d m_vKnockDownDir;				/** knock down direction */
    RwV2d m_vPrevDist;					/** Since distance based on time is used, the previous dist is stored. */
    RwV2d m_vFallDelta;					/** falling delta dist */
    RwReal m_fDistScale;				/** In the time sequence, the time and distance are fixed, but the push distance is not fixed. */

    CNtl2DTimeSequence *m_pSeq;				/** knock down fall up sequence node */
    CNtlDistTimeSequence *m_pLandingSeq;	/** knock donw landing sequence node */
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurt : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurt, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurt)

private:

	void SetAnim(SHitStuff *pHitStuff);
	void SetTrans(SHitStuff *pHitStuff);

public:

	CNtlBehaviorCharHurt();
	~CNtlBehaviorCharHurt();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurtTossUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurtTossUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurtTossUp)

protected:

	enum
	{
		TOSS_UP_UP,
		TOSS_UP_IDLE,
		TOSS_UP_DOWN,
	};

	RwUInt8 m_byTossUpState;
	CNtlDistTimeSequence *m_pSeq;		// time sequence
	RwReal m_fTime;
	RwReal m_fPrevDist;

private:

	/**
	*animation setting
	*/
	void SetAnim(void);

	/**
	*push sequence node creation function.
	*/
	void CreateTimeSequence(void);

	/**
	*push sequence node deletion function. 
	*/
	void DeleteTimeSequence(void);

	/**
	*toss update up
	*/
	void UpdateUp(RwReal fElapsed);

	/**
	*toss update idle
	*/
	void UpdateIdle(RwReal fElapsed);

	/**
	*toss update down
	*/
	void UpdateDown(RwReal fElapsed);

public:

	CNtlBehaviorCharHurtTossUp();
	~CNtlBehaviorCharHurtTossUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurtTossDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurtTossDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurtTossDown)

protected:

	enum
	{
		TOSS_DOWN_DOWN,
		TOSS_DOWN_IDLE,
	};

	RwUInt8 m_byTossDownState;
	
private:

	/**
	*animation setting
	*/
	void SetAnim(void);

	/**
	*toss update up
	*/
	void UpdateDown(RwReal fElapsed);

	/**
	*toss update idle
	*/
	void UpdateIdle(RwReal fElapsed);

public:

	CNtlBehaviorCharHurtTossDown();
	~CNtlBehaviorCharHurtTossDown();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Vertical rise (follows target)

class CNtlBehaviorCharHomingUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHomingUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHomingUp)

private:

	RwUInt8 m_bySpeedSearch;
	RwV3d m_vOldTargetPos;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);


public:

	CNtlBehaviorCharHomingUp();
	~CNtlBehaviorCharHomingUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Vertical descent.

class CNtlBehaviorCharHomingDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHomingDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHomingDown)

private:

	CNtlDistTimeSequence	*m_pSeq;		// time sequence
	RwReal					m_fTime;
	RwReal					m_fPrevDist;

	RwBool					m_bLandingEnd;
	RwBool					m_bLandingAnimPlay;

	RwBool					m_bHomingDown;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);


	/**
	* push sequence node creation function.
	*/
	void CreateTimeSequence(void);

	/**
	* push sequence node deletion function. 
	*/
	void DeleteTimeSequence(void);

	void UpdateHomingDown(RwReal fElapsed);

	void UpdateIdleDown(RwReal fElapsed);

public:

	CNtlBehaviorCharHomingDown();
	~CNtlBehaviorCharHomingDown();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Ascends vertically (without following the target) and jumps a certain height.

class CNtlBehaviorCharJumpingUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharJumpingUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharJumpingUp)

private:

	CNtlDistTimeSequence *m_pSeq;		// time sequence
	RwReal m_fTime;
	RwReal m_fPrevDist;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);

	/**
	* push sequence node creation function.
	*/
	void CreateTimeSequence(void);

	/**
	* push sequence node deletion function. 
	*/
	void DeleteTimeSequence(void);


public:

	CNtlBehaviorCharJumpingUp();
	~CNtlBehaviorCharJumpingUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};
//////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief Namek's Demon God skill. Attracts the target.
 *
 * \date 2008-10-17
 * \author agebreak
 */
class CNtlBehaviorCharStretchAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharStretchAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharStretchAttack)

public:
    CNtlBehaviorCharStretchAttack();
    ~CNtlBehaviorCharStretchAttack();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

protected:
    RwReal  GetTargetDistance(CNtlSobActor* pTargetActor);              ///< Calculate the distance to the pulling target (compare with Radius)

protected:
    CNtlPLCharacter*    m_pCharacter;           ///< User's entity object    
    RwUInt32            m_uiTargetId;           ///< Target serial ID
    RwV3d               m_vTargetPos;           ///< Target location when the event first occurs
    RwBool              m_bPulling;             ///< Flag whether it is in a pulling or stretching state (FALSE in a stretching state, TRUE in a pulling state)    
    SEventStretch*      m_pStretchEvent;         ///< event structure
    RwReal              m_fStretchPullingSpeed;  ///< Current pulling speed
    RwMatrix            m_matDir;                ///< Shoulder direction matrix

    RwBool              m_bOrgBoneScale;        ///< Original bone scale flag
    RwReal              m_fOrgBoneLength[3];    ///< Original bone length
    RwReal              m_fOrgBoneWidth[3];     ///< Original bone thickness
};
//////////////////////////////////////////////////////////////////////////
/**
 * \ingroup NtlSimulation
 * \brief A skill to attack by warping behind the opponent's back (flash slash)
 *
 * \date 2008-11-20
 * \author agebreak
 */
class CNtlBehaviorCharActiveWarpBackAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveWarpBackAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveWarpBackAttack)

public:
    CNtlBehaviorCharActiveWarpBackAttack();
    ~CNtlBehaviorCharActiveWarpBackAttack();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

    void    OnWarpBack();                       ///<Warp behind the opponent.

protected:

	RwV3d		m_vFinalLoc;
};

//////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief Mine's short-distance spatial leap skill (moves near target)
 *
 * \date 2008-11-21
 * \author agebreak
 */
class CNtlBehaviorCharActiveWarp : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveWarp, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveWarp)

public:
    CNtlBehaviorCharActiveWarp();
    ~CNtlBehaviorCharActiveWarp();
    
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

    void    OnWarpTarget();                 ///<Warp around the target.

protected:

	RwV3d		m_vFinalLoc;
};

//////////////////////////////////////////////////////////////////////////

#define SKILL_THROUGH_SPEED 15.0f;
#define SKILL_THROUGH_PAST_POS 10.0f;

/**
 * \ingroup NtlSimulation
 * \brief Through Attack (Slash Style) Skill
 *
 * \date 2008-11-21
 * \author agebreak
 */
class CNtlBehaviorCharActiveThroughAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveThroughAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveThroughAttack)

public:

    CNtlBehaviorCharActiveThroughAttack();
    ~CNtlBehaviorCharActiveThroughAttack();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

	void    OnThroughAttack();																		 ///< 
    void    CalcHitDamage();                                                                    ///< Show damage to the right target

protected:

	RwV3d		m_vFinalLoc;
};

class CNtlBehaviorCharActivePull : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharActivePull, NTL_DEFAULT_MEMORY_POOL)
		DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActivePull)

public:

	CNtlBehaviorCharActivePull();
	~CNtlBehaviorCharActivePull();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	void	SetFinalLoc(RwV3d& loc);

protected:

	void    OnPull();	///< 

protected:

	RwV3d		m_vFinalLoc;
};

/**
 * \ingroup NtlSimulation
 * \brief Class used when a mob uses transformation skills (mobs only)
 *
 * \date 2009-01-16
 * \author agebreak
 */
class CNtlBehaviorCharTransform : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharTransform, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharTransform)

public:
    CNtlBehaviorCharTransform();
    ~CNtlBehaviorCharTransform();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);   

    void    SetAnimation(RwUInt32 uiAction, RwUInt32 uiActionLoop);       ///< Set the animation.
protected:

protected:
    RwUInt32    m_uiAnimAction;
    RwUInt32    m_uiAnimLoop;
};


#endif
