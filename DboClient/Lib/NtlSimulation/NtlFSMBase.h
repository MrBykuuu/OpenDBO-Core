/*****************************************************************************
 *
 * File			: NtlFSMBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 2. 9	
 * Abstract		: Finite state machine class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSMBASE_H__
#define __NTL_FSMBASE_H__

#include "NtlSLDef.h"

class CNtlFSMStateBase;
class CNtlSobActor;

/**
 * \ingroup NtlSimulation
 *This is a class that runs FSM in DBO.
 *Performs the container function and transition function of the state.
 * 
 */
class CNtlFSMBase
{
protected:

    typedef std::map<RwUInt32, CNtlFSMStateBase*> MapState;     ///< Key : State ID, Val : State Pointer
	MapState m_mapState;									    ///< State Container

	CNtlFSMStateBase* Find(RwUInt32 uiStateId);
public:

	virtual ~CNtlFSMBase() {}	
	virtual RwBool Create(void);    ///< Function called immediately after this class is created	
	virtual void Destroy(void);     ///< Function called before this class is destroyed

	RwBool            Register(CNtlFSMStateBase *pState);                                                   ///< Add State to State Container.	
	CNtlFSMStateBase* CreateState(CNtlSobActor *pActor, RwUInt32 uiNewStateId);                             ///< Function that creates the first State 	
	virtual CNtlFSMStateBase* Transition(CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId); ///< Convert State
};

#endif