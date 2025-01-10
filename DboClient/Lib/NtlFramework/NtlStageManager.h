/*****************************************************************************
 *
 * File			: NtlStageManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
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
    *  \return CNtlStageManager class를 상속받는 class에서 함수를 재정의 하여 stage name에 해당하는 stage를 생성하여
    *  리턴한다.
    *  생성된 stage는 active stage list에 등록된다.
	*/
	virtual CNtlStage* CreateStage(const char *pStageName) = 0;

	/**
	* 
    *  stage를 삭제하고, active stage list에서 제거한다.
	*/
	void DeleteStage(CNtlStage *pStage);

	/**
    *
    *  \return 현재 active된 stage중에서 stage name과 CNtlStage를 찾아서 리턴한다.
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