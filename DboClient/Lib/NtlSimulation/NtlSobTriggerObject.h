/*****************************************************************************
 *
 * File			: NtlSobTriggerObject.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 16	
 * Abstract		: Simulation trigger object
 *****************************************************************************
 * Desc         : Provides functions for objects for which triggers are set. 
 *                Synchronize with the server.
 *                Handle is handled by the server like a general simulation object method. 
 *                Not an allocation method, but the same algorithm between the server and client.
 *                is used to allocate a handle on the client itself.
 *****************************************************************************/

#ifndef __NTL_SOB_TRIGGEROBJECT_H__
#define __NTL_SOB_TRIGGEROBJECT_H__

#include "NtlSobActor.h"

struct SNtlEventTObjectUpdateState;

struct SNtlSobTriggerObjectState
{
	RwUInt8			byMainState;
	RwUInt8			bySubState;
};

class CNtlSobTriggerObject : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobTriggerObject, NTL_DEFAULT_MEMORY_POOL )

protected:

	RwReal			m_fTime;
	RwUInt8			m_byQuestMark;
	RwBool			m_bActiveQuestMark;
	RwUInt16		m_tID;

	//State of trigger object
	RwUInt8						m_byStateType;
	SNtlSobTriggerObjectState	m_sPreState;
	SNtlSobTriggerObjectState	m_sCurState;

protected:

	void	QeustMarkCheck(RwReal fElapsed);

public:

	CNtlSobTriggerObject(VOID);
	virtual ~CNtlSobTriggerObject(VOID);

	virtual RwBool Create(VOID);
	virtual void Destroy(VOID);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual CNtlSobInfluence* CreateSobInfluence(void);

	virtual void Update( RwReal fElapsed );

	virtual void HandleEvents( RWS::CMsg& pMsg );

	virtual void SetDirection(const RwV3d *pDir);

public:

	RwBool CanClicked(void);

	RwBool IsScouterObject( void );

	RwBool IsTLQObject( void );

	RwBool IsPIDObject( void );

	RwBool IsDojoSealObject( void );

	void ActiveQuestMark(RwBool bQuestMark);

	RwBool ShowTOForSCT( RwBool bShow );

	SNtlSobTriggerObjectState GetCurrentTriggerObject(void) { return m_sCurState; }

protected:

	void InitState( void );
	void UpdateState( SNtlEventTObjectUpdateState *pSobTObjUpdateState );

	void PlayClickSound( void );
};

#endif