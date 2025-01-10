/*****************************************************************************
*
*File			: NtlSLTMQ.h
*Author		: Hong SungBock
*Copyright		: (аж)NTL
*Date			: 
*Update		: 10. 29
*Abstract		: Simulation time machine class
*****************************************************************************
*Desc         : 
*
*****************************************************************************/

#ifndef __NTL_MACHINE_H__
#define __NTL_MACHINE_H__

// Core

#include "NtlDebug.h"
#include "ceventhandler.h"

// Share
#include "NtlTimeQuest.h"

// Simulation
#include "NtlSLCommunityAgent.h"

struct sTIMEQUEST_TBLDAT;

#define dTMQTYPE_NONE		MAX_TIMEQUEST_TYPE

#define dINVALID_STAGE		-1

class CNtlTMQ : public NtlSLCommunityAgent, public RWS::CEventHandler
{
public:
	CNtlTMQ();
	virtual ~CNtlTMQ();

	struct sWaitInfo
	{
		sTIMEQUEST_TBLDAT*			pTMQTable;	///< TMQ table		
		RwReal						fRemainTime;///< Time remaining until TMQ draw/entry
		RwBool						bTeleport;	///< You can teleport once through the TMQ lottery.
		SERIAL_HANDLE				hNPCHandle;	///< Handle of NPC that applied for TMQ
	};

	struct sTMQRunning
	{
		sTIMEQUEST_TBLDAT*		pTMQTable;			///< TMQ table
		eTIMEQUEST_DIFFICULTY	byDifficultyFlag;	///< Difficulty flag (eTIMEQUEST_DIFFICULTY_FLAG)
		eTIMEQUEST_MODE			eTimeQuestMode;		///< eTIMEQUEST_MODE

		RwUInt8				byStage;
		RwReal				fLimitedTime;		///< Time remaining
		RwBool				bUpdateTime;		///< Whether to update remaining time

		RwReal				fLastDayRecord;		///< Shortest TMQ record so far
		RwUInt32			uiClearTime;
		RwUInt32			uiBonusTime;
		RwBool				bDayRecord;
		RwBool				bBestRecord;

		// avooo's comment: For more detailed information about the progress, go to sWORLD_INFO in SAvatarInfo.
	};

	virtual	RwBool			Create(void);
	VOID					Destroy(void);
	VOID					Update(RwReal fElapsed);

	VOID					ClearBoardInfo();
	VOID					ClearRunningInfo();

	VOID					SetNPCHandle(SERIAL_HANDLE hNPCHandle);

	// TMQ Reservation Information
	sTIMEQUEST_TBLDAT*		GetTMQTable_for_RegRoom();
	RwReal					GetRemainTime_in_Room();
	RwBool					CanTMQTeleport();
	SERIAL_HANDLE			GetNPCHandle();

	// TMQ progress information
	VOID					SetStage(RwUInt8 byStage) { m_RunningInfo.byStage = byStage; }

	sTIMEQUEST_TBLDAT*		GetTMQTable();
	eTIMEQUEST_MODE			GetTMQMode();
	eTIMEQUEST_DIFFICULTY	GetDifficult();
	RwReal					GetRemainTime_in_TMQ();
	RwUInt8					GetStageNum();
	RwReal					GetLastDayRecord();
	RwUInt32				GetClearTime();
	RwUInt32				GetBonusTime();
	RwBool					IsDayRecord();
	RwBool					IsBestRecord();

	VOID					HandleEvents(RWS::CMsg &pMsg);

protected:
	RwUInt8					GetNearDifficult(TBLIDX tmqTblidx);

protected:
	sWaitInfo		m_WaitInfo;			///< Waiting information after registration
	sTMQRunning		m_RunningInfo;		///< TMQ progress information
};

inline VOID CNtlTMQ::SetNPCHandle(SERIAL_HANDLE hNPCHandle)
{
	m_WaitInfo.hNPCHandle = hNPCHandle;
}

inline sTIMEQUEST_TBLDAT* CNtlTMQ::GetTMQTable_for_RegRoom()
{
	return m_WaitInfo.pTMQTable;
}

inline RwReal CNtlTMQ::GetRemainTime_in_Room()
{
	return m_WaitInfo.fRemainTime;
}

inline RwBool CNtlTMQ::CanTMQTeleport()
{
	return m_WaitInfo.bTeleport;
}

inline SERIAL_HANDLE CNtlTMQ::GetNPCHandle()
{
	return m_WaitInfo.hNPCHandle;
}

inline sTIMEQUEST_TBLDAT* CNtlTMQ::GetTMQTable()
{
	return m_RunningInfo.pTMQTable;
}

inline eTIMEQUEST_MODE CNtlTMQ::GetTMQMode()
{
	return m_RunningInfo.eTimeQuestMode;
}

inline eTIMEQUEST_DIFFICULTY CNtlTMQ::GetDifficult()
{
	return m_RunningInfo.byDifficultyFlag;
}

inline RwReal CNtlTMQ::GetRemainTime_in_TMQ()
{
	return m_RunningInfo.fLimitedTime;
}

inline RwUInt8 CNtlTMQ::GetStageNum()
{
	return m_RunningInfo.byStage;
}

inline RwReal CNtlTMQ::GetLastDayRecord()
{
	return m_RunningInfo.fLastDayRecord;
}

inline RwUInt32 CNtlTMQ::GetClearTime()
{
	return m_RunningInfo.uiClearTime;
}

inline RwUInt32 CNtlTMQ::GetBonusTime()
{
	return m_RunningInfo.uiBonusTime;
}

inline RwBool CNtlTMQ::IsDayRecord()
{
	return m_RunningInfo.bDayRecord;
}

inline RwBool CNtlTMQ::IsBestRecord()
{
	return m_RunningInfo.bBestRecord;
}

#endif