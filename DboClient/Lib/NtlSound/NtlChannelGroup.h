/*****************************************************************************
*
* File			: NtlChannelGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Sound channel group
*****************************************************************************
* Desc			: Channel grouped according to the nature of the sound channel in the game
*                 can be managed in batches.
*****************************************************************************/

#pragma once

#include <map>
#include <string>

// Core
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"

// Sound
#include "NtlSoundDefines.h"

class CNtlSound;
class CNtlSoundDSP;

class CNtlChannelGroup
{
public:
	CNtlChannelGroup(eChannelGroupType eGroup);
	virtual ~CNtlChannelGroup();

	typedef std::map<SOUND_HANDLE, CNtlSound*>				SOUND_MAP;
	typedef std::map<SOUND_HANDLE, CNtlSound*>::value_type	SOUND_VALUE;
	typedef std::map<SOUND_HANDLE, CNtlSound*>::iterator	SOUND_ITER;

	typedef std::map<FMOD_DSP_TYPE, FMOD::DSP*>				MAP_DSP;
	typedef std::map<FMOD_DSP_TYPE, FMOD::DSP*>::iterator	MAP_DSP_ITER;


	virtual bool			Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplicatoin = 0);
	///< Registers the channel group created by FMOD::System
	///< uiSoundDuplicatoin: Number of times the same sound can be played repeatedly at the same time

	virtual void			Reset();
	virtual void			Update(float fElapsed);
	virtual void			Destory();	

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual void			StopGroup();

	virtual CNtlSound*		GetSound(SOUND_HANDLE hHandle);

	virtual void			AddEffect(FMOD_DSP_TYPE eType);
	virtual void			RemoveEffect(FMOD_DSP_TYPE eType);
	virtual void			RemoveAllEffect();

	virtual bool			ReleaseLowRankChannel();
	virtual int				CanPlay(const char* pcName);

	virtual void			SetMute(bool bMute);

	virtual void			SetProhibition(RwBool bProhibition);
	virtual RwBool			IsProhibition();

	virtual void			SetValidGroupRange(float fRange);
	virtual float			GetValidGroupRange();

	virtual void			SetGroupVolume(float fVolume);
	virtual bool			SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);
	virtual void			SetVolume_FadeInit();
	virtual void			SetFadeVolume(float volume);
	virtual float			GetGroupVolume();
	virtual sNtlVolume*		GetNtlVolume();

	virtual void			SetGroupPitch(float fPitch);
	virtual void			SetPitch_SlowMotionRate(float fPitchRate);
	virtual void			SetPitch_InitSlowMotionRate();
	virtual float			GetGroupPitch();

	CNtlSoundDSP*			GetDSP();

	virtual eChannelGroupType GetGroupType();
	virtual unsigned int	GetPlayingChannels();
	virtual unsigned int	GetSoundDuplication();
	virtual FMOD::ChannelGroup*	GetFMODChannelGroup();

	virtual bool			IsExistSound(SOUND_HANDLE hHandle);	
	virtual bool			IsMute();

protected:	
	virtual void			ReleaseFinishedSound(float fElapsed);

public:	
	eChannelGroupType			m_eGroup;					///< Channel group
	unsigned int				m_uiSoundDuplication;		///< Number of times the same sound can be played repeatedly at the same time

	sNtlVolume					m_tVolume;
	sNtlPitch					m_tPitch;

	float						m_fValidRange;				///< Distance from listener that can play
	RwBool						m_bProhibition;				///< Not playing at all, not using the mute function	

	CNtlSoundDSP*				m_pDSP;
	FMOD::ChannelGroup*			m_pMasterLayer;				///< FMOD channel group master layer
	SOUND_MAP					m_mapGroup;
	MAP_DSP						m_mapDSP;
};