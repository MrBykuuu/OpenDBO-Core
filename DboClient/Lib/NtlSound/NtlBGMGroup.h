/*****************************************************************************
*
* File			: NtlBGMGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 28
* Abstract		: Background Music Channel Group
*****************************************************************************
* Desc			: When switching to another background music, the previous background music (PostMusic) is
*				  Fade Out and disappear
*
*				  Background music has priority, so the priority background music is played.
*				  Only after the game is finished, the waiting background music will be played.
*****************************************************************************/

#pragma once

#include <string>
#include "ceventhandler.h"
#include "NtlChannelGroup.h"

class CNtlBGMPlayList;


class CNtlBGMGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
protected:
	struct sPLAY_BGM_INFO
	{
		SOUND_HANDLE	hHandle;
		eBGMType		eType;
	};

public:
	CNtlBGMGroup(eChannelGroupType eGroup);
	virtual ~CNtlBGMGroup();

	virtual bool			Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplicatoin = 0);
	virtual void			Update(float fElapsed);
	virtual void			Destory();

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	void					StopImmediately(SOUND_HANDLE hHandle);

public:
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

protected:
	void			Update_Replay(float fElapsed);
	void			ClearBGMList(SOUND_HANDLE hHandle);

	virtual void	ReleaseFinishedSound(float fElapsed);

protected:
	float			m_fReplayElapsed;						///< Time remaining until next playback of current music

	sPLAY_BGM_INFO	m_tPlaySound;							///< Sound currently playing
	sPLAY_BGM_INFO	m_tPostSound;							///< Sound to be deleted after fade out

	CNtlBGMPlayList*	m_pBGMPlayList;
};