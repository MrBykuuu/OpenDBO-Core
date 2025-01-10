/*****************************************************************************
 *
 *File			: NtlStageManager.h
 *Author	    : HyungSuk, Jang
 *Copyright	    : NTL Co., Ltd.
 *Date			: 2005. 7. 15	
 *Abstract		: DBO Stage Manager.
 *****************************************************************************
 *Desc          : 
 *
 *****************************************************************************/

#ifndef __STAGE_MANAGER__
#define __STAGE_MANAGER__

#include "NtlStageManager.h"


/**
 * \ingroup Client
 *This is a Stage Manager object used in DBO, and inherits the CNtlStageManager class.
 *Here, there is a part that creates the stage that is actually used.
 * 
 */
class CStageManager : public CNtlStageManager
{
private:

	static CStageManager *m_pInstance;

protected:

	/**
	* 
    *  \return Creates and returns the stage corresponding to the stage name. 
	*/
	virtual CNtlStage* CreateStage(const char *pStageName);
	
public:

	CStageManager();
	virtual ~CStageManager();

	static CStageManager* GetInstance(void);

	virtual bool Create(void);
	virtual void Destroy(void);
};

#endif

