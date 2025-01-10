/*****************************************************************************
 *
 * File			: NtlStageManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Stage Manager.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_STAGE_MANAGER__
#define __NTL_STAGE_MANAGER__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

class CNtlStage;

/**
 * \ingroup Client
 *This is a class that manages the active stage.
 *The stage manager class receives and processes events. 
 *Stage creation/deletion exists in event.
 *g_EventCreateStage and g_EventDeleteStage are used for event id.
 *Declared in NtlGlobalEvent.h.
  */

class CNtlStageManager : public RWS::CEventHandler
{
private:

	std::list<CNtlStage*> m_listStage;	 /**< active stage objectlist*/

protected:

	/**
	* 
    *  \return CNtlStageManager class�� ��ӹ޴� class���� �Լ��� ������ �Ͽ� stage name�� �ش��ϴ� stage�� �����Ͽ�
    *  �����Ѵ�.
    *  ������ stage�� active stage list�� ��ϵȴ�.
	*/
	virtual CNtlStage* CreateStage(const char *pStageName) = 0;

	/**
	* 
    *  stage�� �����ϰ�, active stage list���� �����Ѵ�.
	*/
	void DeleteStage(CNtlStage *pStage);

	/**
    *
    *  \return ���� active�� stage�߿��� stage name�� CNtlStage�� ã�Ƽ� �����Ѵ�.
    */
	CNtlStage* FindActiveStage(const char *pStageName);


public:

	CNtlStageManager();
	virtual ~CNtlStageManager();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void PostRender(void);

	RwBool ExistStage(const char *pStageName);

	CNtlStage* GetActiveStage(void);
	
	/**
    *
    *  event handler
	*/
	virtual void HandleEvents(RWS::CMsg &pMsg);
};

#endif