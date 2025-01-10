#include "precomp_ntlsimulation.h"
#include "NtlSobEventObject.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSobEventObjectProxy.h"

DEFINITION_MEMORY_POOL(CNtlSobEventObject)

CNtlSobEventObject::CNtlSobEventObject(VOID)
{
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_NOT_TARGETED);
}

CNtlSobEventObject::~CNtlSobEventObject(VOID)
{
}

RwBool CNtlSobEventObject::Create(VOID)
{
	NTL_FUNCTION(__FUNCTION__);

	// proxy settings
	m_pSobProxy = NTL_NEW CNtlSobEventObjectProxy();
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if(!CNtlSobActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// Set class name.
	SetClassName(SLCLASS_NAME_EVENT_OBJECT);

	NTL_RETURN(TRUE);
}

void CNtlSobEventObject::Destroy(VOID)
{
	NTL_FUNCTION(__FUNCTION__);

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

void CNtlSobEventObject::Update( RwReal fElapsed )
{
	CNtlSobActor::Update(fElapsed);
}

void CNtlSobEventObject::HandleEvents( RWS::CMsg& pMsg )
{
	NTL_FUNCTION(__FUNCTION__);

	if(pMsg.Id == g_EventSobCreate)
	{
		// Event object attribute events.
		//GetSobAttr()->HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		// Coordinate and direction settings.
		//RwV3dAssignMacro(&m_vPos, &pSobCreate->vLoc);		
	}

	else if(pMsg.Id == g_EventSobDelete)
	{
		
	}

	CNtlSobActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}
