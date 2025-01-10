/*****************************************************************************
*
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Ntl Sound Manager by Singleton
*****************************************************************************
* Desc: Background music is usually played only one song and is converted to another background music
* The previous background music is Fade Out and the new background music is Fade In
*				do.
*
* FMOD uses the left hand coordinate system.
*
* In fmod, the volume is a float value between 0.0 and 1.0. Entered volume value
* fVolume <0.0 fVolume = 0.0
* fVolume> 1.0 fVolume = 1.0
*
* FMOD :: sound has information about the sound source.
* FMOD :: Channel has current play information of sound source.
*
* The isPlaying function is called to see if the sound has finished playing.
* In FMOD, compare the length of the entire sound with the offset of the sound currently playing.
*, The current offset of the finished sound is 0
I can not figure it out.
*
* Background music is read in stream format and background loading
* When background music is in Loop mode When you play another background music
* The Loop mode argument to the Play (..) function does not apply.
* If you have PostMusic when background music changes, you can change PostMusic status
* To apply to new background music SetBackgroundMusicLoop ()
* Should be called.
*
* SoundManager interface for sounding situations
Control directly from the Sound Layer via event handlers rather than stretching
*				lets do it
*****************************************************************************/

#pragma once

#include "NtlSoundMessage.h"
#include "NtlSoundDefines.h"

#pragma comment(lib, "fmod_vc.lib")


class CNtlChannelGroup;
class CNtlSoundDSP;
class CNtlSound;
class CNtlSoundSubSystem;

class CNtlSoundManager
{
protected:
	CNtlSoundManager();
	CNtlSoundManager(const CNtlSoundManager& soundManager);

public:
	virtual ~CNtlSoundManager();

	static CNtlSoundManager* GetInstance();

	RwBool		IsUsableSound();


	// System

	///< FMOD System initialization, Channel Group initialization
	///< fDopplerScale: Reverberation (change in sound wave according to the movement speed of the sound source and the movement speed of the subject hearing the sound)
	///< fDistanceFactor: The ratio of the length at which the sound can be heard (ex: 1 meter, 1 centimeter)
	///< fRollOffScale: Attenuation (reduction in volume size with distance)
	void		Init(const char* pcPath, float fMasterVolume = 1.0, float fDopplerScale = 1.0,
					 float fDistacneFactor = 1.0, float fRollOffScale = 1.0);				

	void		Reset();
	void		Release();					///< FMOD system Shut down

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos);	///< Lister 3D position settings

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos, 
									float fXFoward, float fYFoward, float fZFoward, 
									float fXUp, float fYUp, float fZUp); 

	int			Play(sNtlSoundPlayParameta* pParameta);

	int			ReplayEnvironmentSound(CNtlSound* pSound);///< Play object music again. (Handle does not change)
	int			Replay(CNtlSound* pSound);		///< Replays the sound that was already being played.

	void		Update(float fElapsed);			///< Each ChannelGroup Update and FMOD::System->update()

	typedef void (F_CALL* FMOD_DEBUG_CALLBACK)(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* function, const char* message);


	//////////////////////////////////////////////////////////////////////////
	//				 Master Group
	//////////////////////////////////////////////////////////////////////////

	unsigned int	GetPlayingChannels();			///< Number of sound channels currently playing

	///< volume
	void		SetMasterVolume(float fVolume);
	float		GetMasterVolume();

	// Apply Effect
	void		SetMasterEffect(FMOD_DSP_TYPE eType);
	void		ReleaseMasterEffect(FMOD_DSP_TYPE eType);

	void		SetMute(bool bMute);					///< Mute settings for the entire sound


	//////////////////////////////////////////////////////////////////////////
	//				 Channel Group
	//////////////////////////////////////////////////////////////////////////

	void		StopGroup(int iChannelGroup);	///< Immediately releases the entire channel group.

	CNtlChannelGroup*	GetChannelGroup(int iChannelGroup);
	char*		GetGroupName(int iChannelGroup);

	void		SetGroupVolume(int iChannelGroup, float fVolume);///< Set the volume of ChannelGroup
	float		GetGroupVolume(int iChannelGroup);				///< Returns the volume of ChannelGroup

	// Apply Effect
	void		SetGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);
	void		ReleaseGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);

	void		SetMute(int iChannelGroup, bool bMute);	///< Set activation of ChannelGroup t: off, f: on

	void		SetProhibition(int iChannelGroup, RwBool bProhibition);
	RwBool		IsProhibition(int iChannelGroup);

	void		SetValidGroupRange(int iChannelGroup, float fRange);	///< Set the playable distance for each channel group.
	float		GetValidGroupRange(int iChannelGroup);				///< Find out the playable distance for each channel group.

	void		FadeIn(int iGroup, float fDestVolume, unsigned int ulTime);
	///< Does not execute if fVolume is greater than the current volume or ulTime is less than 0

	void		FadeOut(int iGroup, float fDestVolume, unsigned int ulTime);
	///< Does not execute if fVolume is smaller than the current volume or ulTime is less than 0

	bool		IsMute(int iChannelGroup);


	//////////////////////////////////////////////////////////////////////////
	//				 Channel
	//////////////////////////////////////////////////////////////////////////

	void		Stop(SOUND_HANDLE& rHandle);	///< Releases the sound.
	int			SetSoundPosition(SOUND_HANDLE hHandle, float fPosX, float fPosY, float fPosZ);	///< Moves the position of the channel in 3D space.

	const char*	GetSoundName(SOUND_HANDLE hHandle);	///< returns the name of the sound

	void		SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);	///< Set the volume of the channel
	float		GetChannelVolume(SOUND_HANDLE hHandle);					///< Returns the volume of Channel

	// Apply Effect
	void		SetChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);
	void		ReleaseChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);

	void		SetMinMaxDistance(SOUND_HANDLE hHandle, float fMinDistance, float fMaxDistance);
	void		GetMinMaxDistance(SOUND_HANDLE hHandle, float &fMinDistance, float &fMaxDistance);

	void		FadeIn(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< Does not execute if fVolume is greater than the current volume or ulTime is less than 0

	void		FadeOut(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< Does not execute if fVolume is smaller than the current volume or ulTime is less than 0

	CNtlSound*	GetSound(SOUND_HANDLE hHandle);			///< Returns sound information.

protected:
	void		CreateChannelGroups();		///< Initialize Channel Group	

	int			CanPlay(int iChannelGroups, const char* pcName,
						float fPosX, float fPosY, float fPosZ);	///< Whether new sounds can be played

	int			CanPlay(sNtlSoundPlayParameta* pParameta);

	bool		IsValidGroupRange(int iChannelGroup, float fPosX, float fPosY, float fPosZ);
	///< Check whether the sound is within the playable range for each channel group

	float		LengthFromListenerToSound(float fPosX, float fPosY, float fPosZ);
	bool		IsExistGroup(int iGroup);	///< true: appropriate group index, false: non-existent group index


protected:
	FMOD::ChannelGroup*		m_pMasterChannelGroup;		///< Top Channel Group of Ntl Sound
	CNtlChannelGroup*		m_apChannelGroup[NUM_CHANNEL_GROUP];	///< Ntl Channel Group		
	CNtlSoundDSP*			m_pMasterDSP;
	
	CNtlSoundSubSystem*		m_pSubSystem;

private: 
	std::map<FMOD_DSP_TYPE, FMOD::DSP*> m_mapMasterDSP;
	std::map<int, std::map<FMOD_DSP_TYPE, FMOD::DSP*>> m_mapGroupDSP;

#ifdef _DEBUG
	int						m_iDebugFlag;				///< for crtdbg
#endif
};

static CNtlSoundManager* GetSoundManager()
{
	return CNtlSoundManager::GetInstance();
}