/*****************************************************************************
*File			: PacketTracer.h
*Author		    : woosung
*Copyright	    : NTL Co., Ltd.
*Date			: 2009. 08. 04
*Abstract		: 
*****************************************************************************
*Desc			:Dump packet name and time for packet tracking
*****************************************************************************/

#pragma once

#include "NtlPacketCommon.h"
#include "ceventhandler.h"
#include <string>
#include <map>

using namespace	std;

#ifdef _DEBUG		/// safety device					
//#define _ENABLE_PACKET_TRACE_ ///Disable ///Protocol can be changed with Defalut. Automatic build may fail
#endif

class CPacketTracer : public RWS::CEventHandler
{
public:
	CPacketTracer();
	~CPacketTracer();

	enum TRACE_MODE
	{
		CAPTURE_ALL,
		CAPTURE_RECEIVE,
		CAPTURE_SEND,
	};

	void	Create();
	void	Destroy();
	void	SetRangeOpCode(int iMin, int iMax = -1);
	void	SetTraceMode(TRACE_MODE eTraceMode);
	const char*	GetPacketName(int iOpCode);

	void	Activate();
	void	Deactivate();
	void	Process(sNTLPACKETHEADER* pPacket, RwBool bReceive);
	//! Event
	virtual VOID HandleEvents( RWS::CMsg& msg );

	//! Create/remove singleton 
	static	void	CreateInstance();
	static	void	DeleteInstance();
	static CPacketTracer*	GetInstance();						///< Singleton instance object

private:
	RwBool m_bAvtivate;
	TRACE_MODE m_eTraceMode;

    typedef map< RwUInt32, string > PacketEnumNameMap;
	PacketEnumNameMap m_mapPacketName;

	int m_iMinOpCode;
	int m_iMaxOpCode;

	static CPacketTracer*	m_pInstance;

};