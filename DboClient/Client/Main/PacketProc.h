/*****************************************************************************
 *
 *File			: PacketProc.h
 *Author	    : HyungSuk, Jang
 *Copyright   	: NTL Co., Ltd.
 *Date			: 2005. 9. 14	
 *Abstract		: packet process 
 *****************************************************************************
 *Desc          : 
 *
 *****************************************************************************/

#ifndef __PACKET_PROC_H__
#define __PACKET_PROC_H__

#include "NtlClientNet.h"

void NetworkLogCallback(const char *pNetLog);
void NetworkEventHander(HSERVER hServer, int nEvent);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPacketProc;
class CClientRecvPacketProc : public CRecvPacketProcess
{
public:

	CClientRecvPacketProc( const HSERVER hServer );

public:

	virtual bool RecvPacketProcess( void* const pData );

	virtual bool IsActivePop( void );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//

class CPacketProc
{
private:

	typedef std::list<void*> ListVirtualPacket;

	RwBool m_bPop;
	ListVirtualPacket m_listPacket;    

private:

	void PopPacket(HSERVER hSocketId);
	void PopVirutalPacket(void);
	
public:

	CPacketProc();
	~CPacketProc();

	static CPacketProc* GetInstance(void);

	/**
	*packet data from network
	*/
	void RecvPacketProc(void);

	/**
	*Packet from virtual server
	*/
	void RecvVirualPackProc(void *pPacket);

	/**
	*Will the packet data be taken out?
	*/
	void ActivePop(RwBool bPop);

	/**
	*Will the packet data be taken out?
	*/
	RwBool IsActivePop(void) const;

};


inline RwBool CPacketProc::IsActivePop(void) const
{
	return m_bPop;
}


#endif