/*****************************************************************************
*
* File			: NtlSoundMoodManager.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 
* Update		: 2009. 3. 11
* Abstract		: Simulation sound mood manager
*****************************************************************************
* Desc         : Manages situational presentation based on data rather than simple play of sound.
*****************************************************************************/

#ifndef __NTL_SOUND_MOOD_MANAGER_H__
#define __NTL_SOUND_MOOD_MANAGER_H__

// sound
#include "NtlSoundDefines.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSoundNode.h"

struct sWORLD_TBLDAT;
struct sGU_PLAY_BGM;
class CNtlSob;
class CNtlSoundNode;


class CNtlSoundMoodManager : public RWS::CEventHandler
{
public:
	struct sPLAY_BGM
	{
		SOUND_HANDLE		hHandle;
	};	

	typedef std::list<CNtlSoundNode*>			LIST_SOUND_NODE;

public:
	static CNtlSoundMoodManager*	GetInstance() { return m_pInstance; }
	static RwBool					CreateInstance();
	static VOID						DestroyInstance();

	CNtlSoundMoodManager();
	virtual ~CNtlSoundMoodManager();

	VOID					Update(RwReal fElapsed);

	VOID					Condition_EnterWorld(sWORLD_TBLDAT* pWorldTblData);
	VOID					Condition_AskPlay_from_Server(sGU_PLAY_BGM* pPlayBGM);

protected:
	VOID					HandleEvents(RWS::CMsg &pMsg);

protected:
	static CNtlSoundMoodManager* m_pInstance;

	sPLAY_BGM				m_tTMQ_BGM;	
	LIST_SOUND_NODE			m_listSoundNode;
};


static CNtlSoundMoodManager* GetNtlSoundMoodManager()
{
	return CNtlSoundMoodManager::GetInstance();
}

#endif