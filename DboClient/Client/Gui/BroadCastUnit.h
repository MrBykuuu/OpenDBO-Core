/******************************************************************************
* File			: BroadCastUnit.h
* Author		: Haesung Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 27
* Abstract		: 
*****************************************************************************
*Desc           : GUI unit responsible for game TS message, server emergency, and mini narration
*****************************************************************************/

#ifndef __BROAD_CAST_UNIT_H__
#define __BROAD_CAST_UNIT_H__

#include "NtlSLEvent.h"
#include "DboTSCoreDefine.h"

class CBroadCastUnit
{
public:
	CBroadCastUnit();
	CBroadCastUnit( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData );
	CBroadCastUnit( const CBroadCastUnit& unit );
	virtual ~CBroadCastUnit();

	VOID					SetData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData );

	RwUInt8					m_byMsgType;
	vecdef_BroadMsgDataList	m_vecMsgDataList;
};

#endif//__BROAD_CAST_UNIT_H__