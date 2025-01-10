/*****************************************************************************
 *
 * File			: NtlLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Main Logic.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_LOGIC__
#define __NTL_LOGIC__

#include "ceventhandler.h"
#include "NtlSysEvent.h"


//RWS_DEFINE_EVENT(iMsgUpdateTick, 0, "Sent just before each MsgUpateTick.");

/**
 * \ingroup Client
 *This is an object that sends system events to all simulation objects on the client.
 *Here, there are free tick, 10 frame sync tick, 20 frame sync tick, and 10 frame sync tick.
 *
 */

class CNtlLogic
{
private:
    
	bool m_bEnabled;			/**< tick의 enable 및 disable flag*/

	float m_fFreeElapsed;		/**< free sync tick*/
	float m_fSync30Elapsed;		/**< 30 delta time variable used for frame sync tick*/
	float m_fSync20Elapsed;		/**< 20 delta time variable used for frame sync tick*/
	float m_fSync10Elapsed;		/**< delta time variable used for 10 frame sync ticks*/
	
public:

	CNtlLogic();

	bool Create(void);
	void Destroy(void);

	/**
	* \ingroup Client
	*Send various sync tick events to the client's rendering object.
	*All simulation objects receive tick events and update.
	*
	*/
	void Update(float fElapsed);

	// simulation object tick의 enable 및 disable
	void SetEnabled(bool bEnabled);
};

inline void CNtlLogic::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}

#endif
