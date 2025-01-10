#pragma once

#include "NtlBehaviorCharMove.h"

/**
 * \ingroup NtlSimulation
 * \brief Character's behavior class while jumping
 *
 * \date 2009-04-17
 * \author agebreak
 */
class CNtlBehaviorCharJump : public CNtlBehaviorCharMove
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharJump, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharJump)

public:

    CNtlBehaviorCharJump();
    ~CNtlBehaviorCharJump();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);    
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

private:
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */

    RwReal              m_fSyncSendTime;
    RwUInt8				m_byJumpState;	        /** jump state */
    RwReal				m_fJumpTime;		    /** jump time */
    RwReal				m_fJumpUpSpeed;	        /** The speed of jumping upward */
    RwUInt8				m_byCollMoveImpossCnt;  /** collision move impossible count */
    RwBool				m_bRandingVisualEffect;	/** Did you express the landing visual effect? */
    RwBool				m_bStandRandingEnd;
    RwBool              m_bCreateBubbleEffect;  /** Did you express the bubble effect? */
    RwBool              m_bNoneDirJump;         ///< Whether to jump in place
    CNtlInstanceEffect* m_pWaterBubbleEffect;   /** Bubble effect when falling into water */

    void UpdatePositionMove(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, OUT RwV3d& vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeedScale = 1.0f);

    void UpdateJumpStart(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpLoop(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpStandLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpMoveLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpWaterLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateSendSyncCheck(RwReal fElapsedTime);
    RwBool UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime);       ///< Udpate function to calculate the current MoveSync value

    void ChangeJumpState(RwUInt8 byJumpState);
};
