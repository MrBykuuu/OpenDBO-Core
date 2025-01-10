#pragma once

class CScheduler;
class CScheduleElement;

enum ESCHEDULE_STEP
{
	ESCHEDULE_STEP_NONE,
	ESCHEDULE_STEP_BEGIN,
	ESCHEDULE_STEP_SCHEDULING,
	ESCHEDULE_STEP_END,
};

class CScheduler
{
public:
	typedef std::list<CScheduleElement*>	LIST_ELE;
	typedef LIST_ELE::iterator				LIST_ELE_IT;

protected:
	LIST_ELE			m_listScheduleElement;
	RwReal				m_fElapsedTime;

public:
	CScheduler();
	virtual ~CScheduler();

	virtual RwBool				Scheduling(RwReal fElapsedTime);
	virtual RwBool				SchedulingProcess();
	virtual void				RemovePresentSchedule();

	virtual void				AddSchedule(CScheduleElement* pNewScheduleElement);
	virtual void				RemoveSchedule(RwInt32 iPrimaryKey);
	virtual void				ClearSchedule();

	virtual CScheduleElement*	GetFirstScedule();
};

class CScheduleElement
{
protected:
	RwInt32			m_iPrimaryKey;
	RwReal			m_fUsedTime;
	ESCHEDULE_STEP	m_eScheduleStep;


public:
	CScheduleElement(RwInt32 iPrimaryKey, RwReal fUsedTime);
	virtual ~CScheduleElement();

	virtual RwInt32			GetPrimaryKey()		{ return m_iPrimaryKey; }
	virtual RwReal			GetUsedTime()		{ return m_fUsedTime; }
	virtual ESCHEDULE_STEP	GetScheduleStep()	{ return m_eScheduleStep; }

	// Scheduling preparation.
	// RETURN : TRUE (preparation Success)
	//			FALSE (Preparation Failed) -Removed from the Scheduler List.
	virtual RwBool	Begin() = 0;

	// Execute scheduling.
	// RETURN : TRUE (when requesting a call again next time)
	//          FALSE (if there is nothing to schedule or scheduling is finished)
	virtual RwBool	Scheduling() = 0;

	// End of scheduling.
	// RETURN : Called after scheduling is completed. RETURN has no meaning.
	//          After being called, it is removed from the Scheduler List.  
	virtual RwBool	End() = 0;

	friend class CScheduler;
};