/*****************************************************************************
 *
 * File			: NtlRender.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Main Render.
 *****************************************************************************
 * º≥∏Ì : 
 *
 *****************************************************************************/

#ifndef __NTL_RENDER__
#define __NTL_RENDER__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

/**
 * \ingroup Client
 *This is an object that sends a system event to the client's rendering object.
 *Update all rendering objects.
 *
 */

class CNtlRender
{
private:
    
	bool m_bEnabled;

	float m_fFreeElapsed;		/**< free sync tick*/

public:

	CNtlRender();

	// It is called immediately after the CNtlRender object is created.
    bool Create(void);

	// Called immediately before the CNtlRender object is released.
	void Destroy(void);

	/**
	* \ingroup Client
	*Send a tick event to the client's rendering object.
	*
	*/
	void Update(float fElapsed);

	// rendering enable and disable
	void SetEnabled(bool bEnabled); 
};


inline void CNtlRender::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}


#endif
