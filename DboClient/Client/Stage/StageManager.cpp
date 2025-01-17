#include "precomp_dboclient.h"
#include "StageManager.h"
#include "NtlDebug.h"
#include "LogInStage.h"
#include "LobbyStage.h"
#include "GameStage.h"
#include "DboDef.h"

CStageManager* CStageManager::m_pInstance = 0;

/**
*
* Default constructor for CStageManager
*
*/
CStageManager::CStageManager()
{
	NTL_PRE(m_pInstance == 0);
	m_pInstance = this;
}

/**
*
* Destructor for CStageManager
*
*/
CStageManager::~CStageManager()
{
}

CStageManager* CStageManager::GetInstance(void)
{
	return m_pInstance;
}

/**
* 
*  \return Creates and returns the stage corresponding to the stage name. 
*  \param pSatgeName is the stage name.
*/
CNtlStage* CStageManager::CreateStage(const char *pStageName)
{
	NTL_FUNCTION("CStageManager::CreateStage" );

	std::string strName = pStageName;

	CNtlStage *pStage = NULL;

	if(strName == LOGIN_STAGE_NAME)
	{
		pStage = NTL_NEW CLogInStage(pStageName);
	}
	else if(strName == CHARACTER_STAGE_NAME)
	{
		pStage = NTL_NEW CLobbyStage(pStageName);
	}
	else if(strName == GAME_STAGE_NAME)
	{
		pStage = NTL_NEW CGameStage(pStageName);
	}
	
	NTL_RETURN(pStage );
}

bool CStageManager::Create(void)
{
	if(!CNtlStageManager::Create())
		return false;

	return true;
}

void CStageManager::Destroy(void)
{
	CNtlStageManager::Destroy();
}


