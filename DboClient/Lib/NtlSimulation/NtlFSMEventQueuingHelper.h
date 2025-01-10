/*****************************************************************************
 *
 * File			: NtlFSMEventQueuingHelper.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 9. 26	
 * Abstract		: FSM event queuing helper
 *****************************************************************************
 * Desc         : I'm using it temporarily for a little while.
 *				  - Create a function to stack the state in the agent.
 *
 *****************************************************************************/


#ifndef __NTL_FSM_EVENTQUEUING_HELPER_H__
#define __NTL_FSM_EVENTQUEUING_HELPER_H__

class CNtlFSMAgent;

void AgentQueuingTerrainClick(CNtlFSMAgent *pAgent, RwV3d vPos);

void AgentQueuingKeyboardMove(CNtlFSMAgent *pAgent, RwUInt8 byMoveFlags);

#endif