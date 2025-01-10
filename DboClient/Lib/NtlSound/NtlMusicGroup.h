/*****************************************************************************
*
* File			: NtlMusicGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Background Music Channel Group
*****************************************************************************
* Desc			: When switching to another music, the previous music (PostMusic) is
*				  Fade Out and is located at the beginning of the map, and new music
*				  Located in the second part of the map
*
*				  Once PostMusic has started Fade Out, it must disappear.
*****************************************************************************/

#pragma once

#include <string>
#include "ceventhandler.h"
#include "NtlChannelGroup.h"


class CNtlMusicGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
public:
	CNtlMusicGroup(eChannelGroupType eGroup);
	virtual ~CNtlMusicGroup();

	virtual void			Update(float fElapsed);

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);

public:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
	virtual void	ReleaseFinishedSound(float fElapsed);

protected:
	float			m_fDelayReplayTime;						///< Rest time until next play of repeated music (unit: seconds)
	float			m_fReplayElapsed;						///< Time remaining until next playback of current music

	SOUND_HANDLE	m_hPostSound;							///< Sound handle to be deleted after fade out
};