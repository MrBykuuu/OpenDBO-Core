/*****************************************************************************
 *
 * File			: NtlFSMAgent.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 2. 9	
 * Abstract		: FSM agent base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_AGENT_H__
#define __NTL_FSM_AGENT_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlDumpTarget.h"
#include "NtlFSMDef.h"

class CNtlFSMController;
class CNtlSobActor;
class CNtlFSMEventQueue;

class CNtlFSMAgent
{
protected:

	CNtlSobActor			*m_pActor;
	CNtlFSMController		*m_pController;
	CNtlFSMEventQueue		*m_pEventQueued;
	RwUInt32				m_uiCurrStateId;
	RwUInt32				m_uiNextStateId;
	
public:
	CNtlFSMAgent();
	virtual ~CNtlFSMAgent();

	virtual RwBool		Create(void) { return TRUE; }
	virtual void		Destroy(void);
    virtual void		Update(RwReal fElapsed);	
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
	virtual void		Dump(CNtlDumpTarget& dump);        ///< Data Dump

	virtual void		NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);        ///< update notify function(state change).
	virtual void		NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2); ///< event notify function.

	virtual RwUInt32	AcquireNextState(RwUInt32 uiCurrStateId) = 0;      ///< Decide on Next State.	
    void				SetController(const CNtlFSMController *pController);           ///< Add Controller to Agent.

    virtual void		AddEvent(RWS::CMsg &pMsg);                             ///< Queuing the event to the agent.
	virtual void		RemoveAllEvents(void);                                 ///< Delete all events queued in the Agent.
	virtual void		EventProc(void);                                       ///< Processes events queued to the Agent.
	RwBool				IsQueuingEventExist(void);

    //--Functions that obtain game property values
    virtual RwBool		IsFightingMode(void);
	virtual RwBool		IsAirMode(void);
	virtual RwBool		IsClickDisable(void);
	virtual RwBool		IsClientUIDisable(void);
    virtual RwBool		IsAttackDisallow(void);
    virtual RwBool		IsDie(void);

    //--- Property -----	
	void				SetActor(const CNtlSobActor *pActor);
	CNtlSobActor*		GetActor(void) const;
	
	void				SetCurrStateId(RwUInt32 uiCurrStateId);
	RwUInt32			GetCurrStateId(void) const;

	void				SetNextStateId(RwUInt32 uiNextStateId);
	RwUInt32			GetNextStateId(void) const;
	void				SetNextStateClear(void);
	
};


inline void CNtlFSMAgent::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlFSMAgent::GetActor(void) const
{
	return m_pActor;
}

inline void CNtlFSMAgent::SetCurrStateId(RwUInt32 uiCurrStateId)
{
	m_uiCurrStateId = uiCurrStateId;
}

inline RwUInt32 CNtlFSMAgent::GetCurrStateId(void) const
{
	return m_uiCurrStateId;
}

inline void CNtlFSMAgent::SetNextStateId(RwUInt32 uiNextStateId)
{
	m_uiNextStateId = uiNextStateId;
}

inline RwUInt32 CNtlFSMAgent::GetNextStateId(void) const
{
	return m_uiNextStateId;
}

inline void CNtlFSMAgent::SetNextStateClear(void)
{
	m_uiNextStateId = NTL_FSMSID_INVALID;
}

inline RwBool CNtlFSMAgent::IsFightingMode(void)
{
	return FALSE;
}

inline RwBool CNtlFSMAgent::IsAirMode(void)
{
	return FALSE;
}

inline RwBool CNtlFSMAgent::IsClickDisable(void)
{
	return FALSE;
}

inline RwBool CNtlFSMAgent::IsClientUIDisable(void)
{
	return FALSE;
}

inline RwBool CNtlFSMAgent::IsAttackDisallow(void)
{
	return FALSE;
}

inline RwBool CNtlFSMAgent::IsDie(void)
{
	return FALSE;
}

#endif