#pragma once

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlWorldConceptDefine.h"
/**
 * \ingroup NtlSimulation
 * \brief Super class of each World Concept
 * \date 2007-06-14
 * \author agebreak
 */

class CNtlSobActor;

class CNtlWorldConceptController
{
public:

	CNtlWorldConceptController(void);
	virtual ~CNtlWorldConceptController(void);

	virtual void		Update(RwReal fElapsedTime);				///< Update processing function for contents suitable for each concept
	virtual RwBool		IsEnableAction(EAvatarAction eAction);		///< Determine whether the Avatar's Action can currently be processed according to each concept.
	virtual void		ChangeState(RwInt32 iState);				///< State Changes the state.	
	virtual RwInt32		GetState() {return m_iWorldState;}			///< Returns the current State.
	
	virtual void		BeforeLoadingDirect(void)	{}
	virtual void		AfterLoadingDirect(void)	{}

	virtual RwBool		IsBeforeLoadingDirect(void)	{ return TRUE; }
	virtual RwBool		IsAfterLoadingDirect(void)	{ return TRUE; }

	virtual RwBool		IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos)	{ return TRUE; }
	virtual RwBool		IsVisiblePCInCinematic(VOID) { return FALSE; }

	virtual RwBool		CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode) { return TRUE; }			

	void				SetConceptType(EWorldPlayConcept eType);
	EWorldPlayConcept	GetConceptType(void);

	void				Pause(RwBool bPause);
	RwBool				IsPause(void)				{ return m_bPause; }

	virtual void		HandleEvents(RWS::CMsg &pMsg) { return; }


protected:

	RwReal				m_fStateTime;									///< Elapsed time in each state
	RwInt32				m_iWorldState;									///< Current state		
	EWorldPlayConcept	m_eType;
	RwBool				m_bPause;
};
