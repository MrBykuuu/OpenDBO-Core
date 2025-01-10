#include "precomp_ntlframework.h"
#include "NtlStageManager.h"
#include "NtlStage.h"
#include "NtlDebug.h"
#include "NtlProfiler.h"

/**
*
* Default constructor for CNtlStageManager
*
*/
CNtlStageManager::CNtlStageManager()
{
}

/**
*
*  Destructor for CNtlStageManager
*
*/
CNtlStageManager::~CNtlStageManager()
{
}


/**
*
*  \param pStageName is the stage name.
*
*  \return Returns a pointer to the stage corresponding to the stage name. If not, NULL.
*/
CNtlStage* CNtlStageManager::FindActiveStage(const char *pStageName)
{
	NTL_FUNCTION("CNtlStageManager::FindActiveStage" );

	CNtlStage *pStage = NULL;
	std::list<CNtlStage*>::iterator it;

	for(it = m_listStage.begin(); it != m_listStage.end(); it++)
	{
		const char *pName = (*it)->GetName();
		if(pName == NULL || strcmp(pName, pStageName) != 0)
			continue;

		pStage = (*it);
		m_listStage.erase(it);

		break;
	}

	NTL_RETURN(pStage);
}

RwBool CNtlStageManager::ExistStage(const char *pStageName)
{
	NTL_FUNCTION("CNtlStageManager::ExistStage" );

	CNtlStage *pStage = NULL;
	std::list<CNtlStage*>::iterator it;

	for(it = m_listStage.begin(); it != m_listStage.end(); it++)
	{
		const char *pName = (*it)->GetName();
		if(pName == NULL || strcmp(pName, pStageName) != 0)
			continue;

		NTL_RETURN(TRUE);
	}

	NTL_RETURN(FALSE);
}

CNtlStage* CNtlStageManager::GetActiveStage(void)
{
	std::list<CNtlStage*>::iterator it = m_listStage.begin();
	if(it == m_listStage.end())
		return NULL;

	return (*it);
}


/**
*Destroys the stage.
*  \param pStage is the stage pointer to be destroyed.
*/
void CNtlStageManager::DeleteStage(CNtlStage *pStage)
{
	NTL_FUNCTION("CNtlStageManager::DeleteStage" );

	if(pStage)
	{
		pStage->Destroy();
		NTL_DELETE(pStage);
	}

	NTL_RETURNVOID();
}

/**
*  This function creates an instance of the stage manager and then calls it immediately.
*  The function of this function registers the event processed by the stage manager.
*  Events include stage create and stage delete events.
*/

bool CNtlStageManager::Create(void)
{
	NTL_FUNCTION("CNtlStageManager::Create" );

	RegisterMsg (g_EventCreateStage, "g_EventCreateStageStr", "SNtlEventStageData");
	RegisterMsg (g_EventDeleteStage, "g_EventDeleteStageStr", "SNtlEventStageData");

	LinkMsg(g_EventCreateStage, 0);
	LinkMsg(g_EventDeleteStage, 0);

	NTL_RETURN(true);
}

/**
*  This function is called before deleting the stage manager object.
*  All registered events are released here.
*  Delete all managed active stages.
*/

void CNtlStageManager::Destroy(void)
{
	NTL_FUNCTION("CNtlStageManager::Destroy" );

	UnLinkMsg(g_EventCreateStage);
	UnLinkMsg(g_EventDeleteStage);

	UnRegisterMsg ( g_EventCreateStage );
	UnRegisterMsg ( g_EventDeleteStage );

	std::list<CNtlStage*>::reverse_iterator it;

	for(it = m_listStage.rbegin(); it != m_listStage.rend(); it++)
	{
		DeleteStage(*it);
	}

	m_listStage.clear();

	NTL_RETURNVOID();
}


void CNtlStageManager::PostRender(void)
{
	NTL_SPROFILE("CNtlStageManager::PostRender")

	std::list<CNtlStage*>::reverse_iterator it;

	for(it = m_listStage.rbegin(); it != m_listStage.rend(); it++)
	{
		(*it)->PostRender();
	}

	NTL_EPROFILE()
}


/**
*  Event Handler function.
*  \param pMsg is an event message, and the message contains event id and data information.
*/

void CNtlStageManager::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlStageManager::HandleEvents");

	if(pMsg.Id == g_EventCreateStage)
	{
		SNtlEventStageData *pEventData = static_cast<SNtlEventStageData*>(pMsg.pData);
		CNtlStage *pNewStage = CreateStage(pEventData->chStageName); 
		if(pNewStage->Create()) 
			m_listStage.push_back(pNewStage); 
	}

	else if(pMsg.Id == g_EventDeleteStage)
	{
		SNtlEventStageData *pEventData = static_cast<SNtlEventStageData*>(pMsg.pData);
		DeleteStage(FindActiveStage(pEventData->chStageName)); 
	}

	NTL_RETURNVOID();
}