/*****************************************************************************
 *
 *File			: DumpCommand.h
 *Author	    : HyungSuk, Jang
*Copyright	    : NTL Co., Ltd.
 *Date			: 2005. 12. 26
 *Update		: 2006. 9. 29
 *Abstract		: DBO dump command manager
 *****************************************************************************
 *Desc         : Modified to allow debugging messages to be entered from the client.
 *               Operates only in DEBUG mode
 *				  
 *Usage example
 *1) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, "Coordinates are incorrect");
 *2) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, MakeStr("Coordinate values ??(x: %d, y: %d) are incorrect", 30, 40));
 *3) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, MakeStr("%s does not exist", "monster"));
 * 
 *****************************************************************************/

#ifndef __DUMPCOMMAND_MANAGER_H__
#define __DUMPCOMMAND_MANAGER_H__

#include "NtlStringUtil.h"
#include "NtlDumpTarget.h"
#include "NtlSLDef.h"
#include "JudgeDump.h"

class CDBOCheatCommand;


#ifndef NDEBUG
	#define DUMP_DEBUG_MSG(eDumpType, msg) GetDumpCmdManager()->OutPutClientMessage(eDumpType, msg)
#else
	#define DUMP_DEBUG_MSG(eDumpType, msg)
#endif

class CDumpCmdManager
{
    friend class CDBOCheatCommand;

	typedef std::map<int, CNtlDumpTarget*>		MAP_DUMPTARGET;
	typedef std::map<int, CJudgeDump*>			MAP_JUDGEDUMP;

public:
	enum EDumpTarget
	{
		DUMP_TARGET_CONSOLE,
		DUMP_TARGET_GUI,
		DUMP_TARGET_FILE,

		NUM_DUMP_TARGET
	};

	static CDumpCmdManager *m_pInstance;

private:
	CNtlDumpTarget* m_apDumpTarget[NUM_DUMP_TARGET];
	MAP_DUMPTARGET m_mapActiveTarget;
	MAP_JUDGEDUMP m_mapActivejudgeDump;

	SERIAL_HANDLE m_hTargetSerialId;
	SERIAL_HANDLE m_hRegisterSerialId;
	RwBool m_bSendOut;
	RwBool m_bRecvOut;

    CDBOCheatCommand* m_pDBOCheatCommand;

private:

	CNtlDumpTarget* FindCmdTarget(RwInt32 iCmdId);
	

public:

	CDumpCmdManager();
	~CDumpCmdManager();

	static CDumpCmdManager* GetInstance(void);

	void SetTargetSerial(SERIAL_HANDLE hSerialId);
    void SetRegisterSerial(SERIAL_HANDLE hSerialId) {m_hRegisterSerialId = hSerialId;}
	void SendPacekt(void *pPacket);
	void RecvPacket(void *pPacket);
	void RecvNet(const char *pFormat);
    
	RwBool Command(const RwChar *pCmd);
	RwBool Command(const WCHAR *pCmd);

	// This is a message received from the client¡¯s console.
	void CleintConsoleCommand(const RwChar *pCmd);
	
	// A message is displayed on the client.
	void OutPutClientMessage(eDUMP_TYPE eDumpType, const RwChar* pMessage);
	void OutPutClientMessage(eDUMP_TYPE eDumpType, const WCHAR* pMessage);

    void ActiveDumpTarget(EDumpTarget eDumpTarget, RwBool bActive);
	void ActiveDumpType(eDUMP_TYPE eDumpType, RwBool bActive);

    void OutputSobObj(RwUInt32 uiSerialId);
    void OutputSobObj(CNtlDumpTarget &dump, RwUInt32 uiSeiralId);
	/// woosungs_test 20090723
	void OutputDump(const RwChar *pMsg, RwUInt32 iR = 255, RwUInt32 iG = 255, RwUInt32 iB = 255);

	RwBool IsOutputRecvPacketTarget(SERIAL_HANDLE hSerial);
};

static CDumpCmdManager* GetDumpCmdManager(void)
{
	return CDumpCmdManager::GetInstance();
}

static void CallbackNetSendPacket(void *pData)
{
	if(GetDumpCmdManager())
		GetDumpCmdManager()->SendPacekt(pData);
}

#endif