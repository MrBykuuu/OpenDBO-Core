#pragma once

#include <set>


typedef std::set<std::string>					SET_ERRORMSG;
typedef std::set<SOUND_HANDLE>					SET_SOUNDHANDLE;


class CNtlSoundGlobal
{
public:
	static CNtlSoundGlobal*			m_pInstance;

	static FMOD::System*			m_pFMODSystem;				///< FMOD::System instance
		
	static std::string				m_strFilePath;
	static float					m_fMinMaxRate;
	static sNtlVolume				m_tMasterVolume;

	static SET_ERRORMSG				m_setMsgError;				//Global variable to filter out the same error message
	static SET_SOUNDHANDLE			m_setPlayingSoundHandle;
};