/*****************************************************************************
 *
 * File			: NtlStage.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Stage.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_STAGE__
#define __NTL_STAGE__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

/**
 * \ingroup Client
 *client is a base class that groups all objects by game state unit and contains information.
 *In MMORPG, there may be Login Stage, Lobby Stage, Character Create Stage, Character Select Stage, Game Stage, etc.
 */

class CNtlStage 
{
private:

	std::string m_strName;
	
public:

	CNtlStage(const char *pStageName);
	virtual ~CNtlStage();

	// It is called immediately after the stage object is created. This is inherited from the sub-object and processed appropriately.

	virtual bool Create(void);

	// Called before the stage object is deleted. This is inherited from the sub-object and processed appropriately.

	virtual void Destroy(void);

	virtual void PostRender(void);

	// Get the stage name.

	const char* GetName(void);
};


inline const char* CNtlStage::GetName(void)
{
	return m_strName.c_str();
}

#endif