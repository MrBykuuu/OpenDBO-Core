/*****************************************************************************
*
* File			: NtlObjectGroup.h
* Author		: Hong SungBock
* Copyright		: (ÁÖ)NTL
* Date			: 2006. 7. 13
* Abstract		: Object music channel group
*****************************************************************************
* Desc			: When an object channel group loses priority to another channel group
*				  Force quit one random object channel and save its name
*				  If an empty channel arises in the future, the sound will be played in the order in which it was forced to stop.
*
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Sound
#include "NtlSoundDefines.h"
#include "NtlChannelGroup.h"

class CNtlObjectGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
public:
	CNtlObjectGroup(eChannelGroupType eGroup);
	virtual ~CNtlObjectGroup();

	virtual void			Destory();

	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual CNtlSound*		GetSound(SOUND_HANDLE hHandle);

	virtual bool			ReleaseLowRankChannel();
	virtual bool			IsExistSound(SOUND_HANDLE handle);

public:
	virtual void			PostUpdate(float fXPos, float fYPos, float fZPos);

	virtual void			StoreReleasedSound(CNtlSound* pSound);
	virtual void			StoreSleepingSound(CNtlSound* pSound);

	virtual void			DelSleepingSound(SOUND_HANDLE hHandle);
	virtual void			DelReleasedSound(SOUND_HANDLE hHandle);	

	virtual CNtlSound*		GetReleasedSoundbyPriority();	///< Returns the name of the sound that was forced to stop the longest ago.
	virtual void			SuccessRelay();					///< This should be called when the sound that has been terminated is successfully played again.

protected:
	virtual VOID			HandleEvents( RWS::CMsg &msg );


public:
	SOUND_MAP			m_mapReleasedSound;			///< Name of the sound that was forcefully terminated by the parent group
	///< When other channels are released, they are played in turn.

	SOUND_MAP			m_mapSleepingSound;			///<Sounds outside a certain radius are not played and are managed in a list.
};