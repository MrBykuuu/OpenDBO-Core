/*****************************************************************************
 *
 * File			: NtlFSMStateBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 2. 9	
 * Abstract		: Finite state machine base class
 *****************************************************************************
 *Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_STATEBASE_H__
#define __NTL_FSM_STATEBASE_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlFSMDef.h"
#include "NtlDumpTarget.h"
#include "NtlDebug.h"

class CNtlSobActor;
class CNtlFSMAgent;
class CNtlBehaviorBase;
class CNtlPLEntity;

// Please do not change new class_name and delete pState to NTL_NEW and NTL_DELETE below.
// by Jang HyungSuk.

#define DECLEAR_FSMSTATE_FACTROY(class_name) \
public: \
	CNtlFSMStateBase* CreateStateFactory(void) \
	{ \
		return new class_name; \
	} \
	void DestroyStateFactory(CNtlFSMStateBase *pState) \
	{ \
		delete pState; \
	}

///////////////////////////////////////////////
	
#define CREATE_FSMSTATE_FACTORY(state_inst) \
	state_inst->CreateStateFactory(); 
	
#define DELETE_FSMSTATE_FACTORY(state_inst) \
	state_inst->DestroyStateFactory(state_inst);

///////////////////////////////////////////////

#define MAX_FSMS_NAME	32

///< Parent abstract class of FSM State classes
class CNtlFSMStateBase
{
protected:
	static RwBool		m_bTerminate;

	RwChar				m_strName[MAX_FSMS_NAME];
	RwUInt32			m_uiStateId;
	RwUInt32			m_uiNextStateId;
	RwUInt32			m_uiFlags;
	RwBool				m_bFinish;
	std::list<CNtlBehaviorBase*> m_listBehavior;
	std::list<CNtlPLEntity*> m_listPLEntity;
	
	CNtlSobActor		*m_pActor;
	CNtlFSMStateBase	*m_pStacked;

	CNtlFSMStateBase(); 

protected:
	void RemoveAllPLEntity(void);
		
public:

	CNtlFSMStateBase& operator = (const CNtlFSMStateBase& State);
	virtual ~CNtlFSMStateBase();

	/**
    *Factory function for creation/destruction of state.
    */
	virtual CNtlFSMStateBase* CreateStateFactory(void) = 0;
	virtual	void DestroyStateFactory(CNtlFSMStateBase *pState) = 0;

	/**
    *Function called when creating/destructing state.
    */
	virtual RwBool Create(void) { return TRUE; }
	virtual void Destroy(void);

	/**
    *State update/event processing function.
    */
	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	
	virtual void Dump(CNtlDumpTarget& dump);        ///< Data Dump
	virtual void Clone(CNtlFSMStateBase *pState);   ///< State Clone

    //--- Property Methods-----------------------------//
	void            SetStateName(const RwChar *pName);         
	const RwChar*   GetStateName(void) const;         

	void            SetStateId(RwUInt32 uiStateId);
	RwUInt32        GetStateId(void) const;
	
	void            SetNextStateId(RwUInt32 uiNextStateId);
	RwUInt32        GetNextStateId(void) const;
	
	void            SetFlags(RwUInt32 uiFlags);
	RwUInt32        GetFlags(void) const;

	void            SetActor(const CNtlSobActor *pActor);
	CNtlSobActor*   GetActor(void) const;

	/**
    *A function used to return to the previous state after the current state ends. (move => When a jump is entered, the move state is queued.)
    */
	void                SetQueued(const CNtlFSMStateBase *pQueuedState);
	CNtlFSMStateBase*   GetQueued(void) const;
	void                ReleaseQueued(void);

    /**
    *A function that checks whether the current state has ended.
    */
    void    ResetFinish(void);
    void    Finish(void);
    RwBool  IsFinish(void) const;

	/**
    *A function that sets the avtive behavior in the current state.
    */
	void                AddBehavior(CNtlBehaviorBase *pBehavior);
	RwInt32             GetBehaviorCount(void) const;
	CNtlBehaviorBase*   GetBehavior(RwInt32 iIdx);
	void                RemoveAllBehavior(void);
	void                UpdateBehavior(void);

	void                AddPLEntity(CNtlPLEntity *pPLEntity);   ///< Add Visual.	
	CNtlFSMAgent*       GetFSMAgent(void);                      ///< Get the current Agent.
	virtual RwBool      IsActorActiveState(void);               ///< Is the object in an active state?

	static void Terminate(RwBool bTerminate);
};

inline void CNtlFSMStateBase::SetStateName(const RwChar *pName)
{
	strcpy_s(m_strName, pName);
}

inline const RwChar* CNtlFSMStateBase::GetStateName(void) const
{
	return m_strName;
}


inline void CNtlFSMStateBase::SetStateId(RwUInt32 uiStateId)
{
	m_uiStateId = uiStateId;
}

inline RwUInt32 CNtlFSMStateBase::GetStateId(void) const
{
	return m_uiStateId;
}

inline void CNtlFSMStateBase::SetNextStateId(RwUInt32 uiNextStateId)
{
	m_uiNextStateId = uiNextStateId;
}

inline RwUInt32 CNtlFSMStateBase::GetNextStateId(void) const
{
	return m_uiNextStateId;
}

inline void CNtlFSMStateBase::SetFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

inline RwUInt32 CNtlFSMStateBase::GetFlags(void) const 
{
	return m_uiFlags;
}

inline void CNtlFSMStateBase::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlFSMStateBase::GetActor(void) const
{
	return m_pActor;
}

inline void CNtlFSMStateBase::SetQueued(const CNtlFSMStateBase *pStackedState)
{
	m_pStacked = const_cast<CNtlFSMStateBase*>(pStackedState);
}

inline CNtlFSMStateBase* CNtlFSMStateBase::GetQueued(void) const
{
	return m_pStacked;
}

inline RwInt32 CNtlFSMStateBase::GetBehaviorCount(void) const
{
	return (RwInt32)m_listBehavior.size();
}

inline CNtlBehaviorBase* CNtlFSMStateBase::GetBehavior(RwInt32 iIdx)
{
	if(iIdx >= (RwInt32)m_listBehavior.size())
		return NULL;

	RwInt32 iNum = 0;
	std::list<CNtlBehaviorBase*>::iterator it;
	for(it = m_listBehavior.begin(); it != m_listBehavior.end(); ++it)
	{
		if(iIdx == iNum)
			return (*it);
		iNum++;
	}

	return NULL;
}

inline void CNtlFSMStateBase::ResetFinish(void)
{
	m_bFinish = FALSE;
}

inline void CNtlFSMStateBase::Finish(void)
{
	m_bFinish = TRUE;
}

inline RwBool CNtlFSMStateBase::IsFinish(void) const
{
	return m_bFinish;
}


#endif