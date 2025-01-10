/*****************************************************************************
*
* File			: NtlSLCommunityAgent.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 3. 24
* Abstract		: Simulation community agent class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_COMMUNITY_AGENT_H__
#define __NTL_COMMUNITY_AGENT_H__

// simulation
#include "NtlSLParty.h"
#include "NtlSLGuild.h"
#include "NtlSLBlackList.h"
#include "NtlSLFriendList.h"

enum eCommunityAgentType
{
	COMMUNITY_AGENT_PARTY,
	COMMUNITY_AGENT_GUILD,
	COMMUNITY_AGENT_BLACKLIST,
	COMMUNITY_AGENT_FRIEND,

	NUM_COMMUNITY_AGENT,

	NONE_COMMUNITY_AGENT
};

class NtlSLCommunityAgent
{
public:
	NtlSLCommunityAgent();
	virtual ~NtlSLCommunityAgent();

	RwBool			Create(eCommunityAgentType eAgentType);
	VOID			Destroy();
	VOID			Clear();

	VOID			SetAgentType(eCommunityAgentType eAgentType);
	eCommunityAgentType GetAgentType();

	sCommunityMember* GetMemberbyIndex(RwInt32 iIndex);
	sCommunityMember* GetMemberbyKey(RwUInt32 uikey);
	RwInt32			GetMemberCount();

	COMMUNITY_ITER	Begin();
	COMMUNITY_ITER	End();

	//After executing Sort, the member structure may be different from the member structure previously received with GetMemberbyIndex().
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	RwBool			IsHaveGroup();
	RwBool			IsMember(RwUInt32 uiKey);

protected:
	CNtlSLCommunityGroup*		m_pCommunityGroup[NUM_COMMUNITY_AGENT];
	eCommunityAgentType			m_eAgentType;
};


inline VOID NtlSLCommunityAgent::Clear()
{
	if( m_eAgentType == NONE_COMMUNITY_AGENT )
		return;

	m_eAgentType = NONE_COMMUNITY_AGENT;
}

inline sCommunityMember* NtlSLCommunityAgent::GetMemberbyIndex(RwInt32 iIndex)
{
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return NULL;

	return m_pCommunityGroup[m_eAgentType]->GetMemberbyIndex(iIndex);
}

inline sCommunityMember* NtlSLCommunityAgent::GetMemberbyKey(SERIAL_HANDLE hSerial)
{
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return NULL;

	return m_pCommunityGroup[m_eAgentType]->GetMemberbyKey(hSerial);
}

inline RwInt32 NtlSLCommunityAgent::GetMemberCount()
{
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return 0;

	return m_pCommunityGroup[m_eAgentType]->GetMemberCount();
}

inline COMMUNITY_ITER NtlSLCommunityAgent::Begin()
{
	// return default community list end
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return m_pCommunityGroup[COMMUNITY_AGENT_PARTY]->End();

	return m_pCommunityGroup[m_eAgentType]->Begin();
}

inline COMMUNITY_ITER NtlSLCommunityAgent::End()
{
	return m_pCommunityGroup[m_eAgentType]->End();
}

inline RwBool NtlSLCommunityAgent::IsHaveGroup()
{
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return FALSE;

	return m_pCommunityGroup[m_eAgentType]->IsHaveGroup();
}

inline RwBool NtlSLCommunityAgent::IsMember(SERIAL_HANDLE hSerial)
{
	if( m_eAgentType >= NONE_COMMUNITY_AGENT )
		return FALSE;

	return m_pCommunityGroup[m_eAgentType]->IsMember(hSerial);
}

#endif