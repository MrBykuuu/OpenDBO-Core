#pragma once

#include "ceventhandler.h"
#include "NtlFileSerializer.h"
#include "NtlSlEvent.h"
#include "NtlDebug.h"

class CNtlFileSerializer;
class CNtlSLTBCrowdController;
class CNtlSobNpc;

#define dDIRECTION_FILE_EXT		"dtf"

#define dDIRECTION_FILE_BUFFER	1024*1024			// Set the default buffer to 1 MB.
#define dDIRECTION_FILE_GLOW	1024*1024

#define dDIRECTION_GAME_DATA_FILE		".\\property\\TenkaichiDirection.dtf"

/// state of the audience
enum ETBCrowdStepType
{
	E_STEP_BASIC,
	E_STEP_MATCH_READY,    
	E_STEP_STAGE_READY,
	E_STEP_STAGE_RUN,
	E_STEP_STAGE_FINISH,
	E_STEP_MATCH_FINISH,
	E_STEP_AWARD,
	E_STEP_BATTLE_END,
	E_STEP_HTB,
	E_STEP_KNOCKDOWN,
	E_STEP_KO,
    E_STEP_FINAL_DIRECTION,

	E_STEP_COUNT,
	E_STEP_NONE = 0xff,
};

/**
* \ingroup NtlSimulation
* \brief Manager class that manages audience presentation
*
* \date 2008-07-21
* \author agebreak
*/


class CNtlSLTBCrowdManager : public RWS::CEventHandler
{
public:
	typedef std::map<std::string, CNtlSLTBCrowdController*>				MAP_CONTROLLER;
	typedef std::map<std::string, CNtlSLTBCrowdController*>::iterator	ITER_MAP_CONTROLLER;

	typedef std::list<CNtlSLTBCrowdController*>							LIST_CONTROLLER;
	typedef std::list<CNtlSLTBCrowdController*>::iterator				ITER_LIST_CONTROLLER;

	static RwBool		ms_bDirectionToolMode;

	static CNtlSLTBCrowdManager* GetInstance();
	static void                  DeleteInstance();    

	void    Update(RwReal fElapsedTime);
	RwBool  Save(const RwChar* szFileName);
	RwBool  Load(const RwChar* szFileName);
	RwBool	Load(CNtlFileSerializer& rSerializer);
	virtual void HandleEvents(RWS::CMsg &pMsg);

	void	AddController(CNtlSLTBCrowdController* pController);
	void	RemoveController(const std::string& keyName);

	void	AddPlayController(CNtlSLTBCrowdController* pController);
	void	AddPlayController(const std::string& keyName);
	void	RemovePlayController(const std::string& keyName);

	void	AddStep(ETBCrowdStepType eStep);
	void	ChangeStep(ETBCrowdStepType eStep);                 ///< Change the step status.
	void	ReserveStep(ETBCrowdStepType eStep);				///< The next state is additionally reserved.    

    CNtlSobNpc* GetRefreeActor();                               ///< Returns a judgment object.    

protected:
	CNtlSLTBCrowdManager(void);
	~CNtlSLTBCrowdManager(void);

	void    Create();
	void    Destroy();
	ETBCrowdStepType    ConvertServerState(SNtlEventMinorMatchStateUpdate* pData);                ///< Change server state to client state.
	ETBCrowdStepType    ConvertServerState(SNtlEventMajorMatchStateUpdate* pData);                ///< Change server state to client state.	
    ETBCrowdStepType    ConvertServerState(SNtlEventFinalMatchStateUpdate* pData);                ///< Change server state to client state.
	void				GetLuaFuncName(ETBCrowdStepType eStep, char* pcLuaFuncName, RwInt32 iBufferSize);
    void                CreateSobRefree();

protected:
	static CNtlSLTBCrowdManager* m_pInstance;

	MAP_CONTROLLER							m_mapControllerContainer;

	LIST_CONTROLLER							m_listCurController;
	ETBCrowdStepType						m_eReservatedStep;

    CNtlSobNpc*                             m_pRefreeActor;     ///< Judgment object (created only in the manager and used publicly)

	RwBool									m_bToolMode;
};

static CNtlSLTBCrowdManager* GetNtlSLCrowdManager()
{
	return CNtlSLTBCrowdManager::GetInstance();
};

static void DeleteNtlSLCrowdManager()
{
	CNtlSLTBCrowdManager::DeleteInstance();
}