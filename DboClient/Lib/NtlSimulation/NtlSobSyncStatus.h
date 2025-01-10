/*****************************************************************************
 *
 * File			: NtlSobSyncStatus.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 16	
 * Abstract		: Simulation Status object
 *****************************************************************************
 * Desc         : It is used to synchronize damage and hp/mp/cp fill packets sent from the server.
 *                Even if the server deals damage first, it can be processed later due to attack animation synchronization.
                  In this case, if you apply the current hp to the actor as is, the avatar gui hp bar will be jagged and strange.
				  It seems. In this case, it was created to handle animation synchronization damage and hp/mp/cp.
 *****************************************************************************/

#ifndef __NTL_SOB_SYNC_STATUS_H__
#define __NTL_SOB_SYNC_STATUS_H__

#include "NtlSob.h"

class CNtlSobActor;

class CNtlSobSyncStatus : public CNtlSob
{
protected:

	RwBool m_bActive;
	RwBool m_bFinish;
	
	/**
	*constructor
	*/
	CNtlSobSyncStatus();
	
public:

	/**
	*destructor
	*/
	virtual ~CNtlSobSyncStatus();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual RwBool IsFinish(void);

	virtual void Update(RwReal fElapsed);

	
public:

	void Active(void);

	virtual void EventGenerator(void) {}
};

inline void CNtlSobSyncStatus::Active(void)
{
	m_bActive = TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobAttackSyncStatus : public CNtlSobSyncStatus
{
	DECLEAR_MEMORY_POOL(CNtlSobAttackSyncStatus, NTL_DEFAULT_MEMORY_POOL)

private:

	RwReal m_fActiveTime;
	RwReal m_fCurrTime;

public:

	CNtlSobAttackSyncStatus();
	virtual ~CNtlSobAttackSyncStatus();

	virtual void Destroy(void);

	virtual RwBool IsFinish(void);

	virtual void Update(RwReal fElapsed);

	void ActiveTime(RwReal fTime);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlSobDieSyncStatus : public CNtlSobSyncStatus
{
	DECLEAR_MEMORY_POOL(CNtlSobDieSyncStatus, NTL_DEFAULT_MEMORY_POOL)

protected:

	RwReal m_fTime;

public:

	CNtlSobDieSyncStatus();
	virtual ~CNtlSobDieSyncStatus();

	virtual void Destroy(void);
	virtual void EventGenerator(void);
    virtual void Update(RwReal fElapsed);
};


#endif
