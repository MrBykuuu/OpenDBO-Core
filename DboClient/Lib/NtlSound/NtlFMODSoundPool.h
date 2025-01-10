/*****************************************************************************
* File			: NtlFMODSoundPool.h
* Author		: Hong SungBock
* Copyright		: (?)NTL
* Date			: 2009. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: FMOD::Sound 
*****************************************************************************/

#pragma once


class CNtlFMODSoundPool
{
public:
	struct sPOOL
	{
		FMOD::Sound*	pFMODSound;
		RwInt32			iUseCount;
	};

	typedef std::map<std::string, sPOOL*>			MAP_FMODSOUND;
	typedef std::map<std::string, FMOD::Sound*>		MAP_RESIDENT;

public:
	static RwBool	CreateInstance();
	static VOID		DestroyInstance();
	static CNtlFMODSoundPool* GetInstance() { return m_pInstance; }

	CNtlFMODSoundPool();
	virtual ~CNtlFMODSoundPool();

	FMOD::Sound*		GetFMODSound(const char* pcFileName);

protected:
	VOID				LoadResidentPool();

protected:
	static CNtlFMODSoundPool* m_pInstance;

	MAP_FMODSOUND		m_mapFMODSoundPool;
	MAP_RESIDENT		m_mapResidentPool;		///< Frequently used FMOD::Sound resides in memory

	RwUInt32			m_uiFileSize_FMODSound;
	RwUInt32			m_uiFileSize_ResidentPool;
};

static CNtlFMODSoundPool* GetSoundPool()
{
	return CNtlFMODSoundPool::GetInstance();
}