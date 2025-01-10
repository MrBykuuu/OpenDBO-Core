#include "precomp_ntlsimulation.h"
#include "NtlSLCommunityGroup.h"

// Core
#include "NtlDebug.h"


VOID CNtlSLCommunityGroup::ClearMember()
{
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		sCommunityMember* pMember = *it_list;
		NTL_DELETE(pMember);
	}

	m_listMember.clear();
	m_mapMember.clear();
}

sCommunityMember* CNtlSLCommunityGroup::GetMemberbyIndex(RwInt32 iIndex)
{
	RwInt32 iCount = 0;
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list, ++iCount )
	{
		if( iCount == iIndex )
			return *it_list;
	}

	return NULL;
}

sCommunityMember* CNtlSLCommunityGroup::GetMemberbyKey(RwUInt32 uiKey)
{
	COMMUNITY_MAP_ITER it_map = m_mapMember.find(uiKey);
	if( it_map != m_mapMember.end() )
		return it_map->second;

	return NULL;
}

sCommunityMember* CNtlSLCommunityGroup::GetMemberbyName(WCHAR* pwcName)
{
	if( !pwcName )
		return NULL;

    // NOTE: The name sent from the server is not guaranteed to be NULL. So you have to put it in a new way.
    std::wstring strCharName = pwcName;

	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		if( wcscmp((*it_list)->wszMemberName, strCharName.c_str()) == 0 )
			return *it_list;
	}

	return NULL;
}

RwInt32 CNtlSLCommunityGroup::GetMemberCount()
{
	return m_listMember.size();
}

COMMUNITY_ITER CNtlSLCommunityGroup::Begin()
{
	return m_listMember.begin();
}

COMMUNITY_ITER CNtlSLCommunityGroup::End()
{
	return m_listMember.end();
}

VOID CNtlSLCommunityGroup::Sort(RwUInt8 byDataType, RwBool bAscent)
{
	switch(byDataType)
	{
	case COMMUNITY_DATA_NAME:	dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentName, CompareDecentName);
	}
}

RwBool CNtlSLCommunityGroup::IsMember(RwUInt32 uiKey)
{
	COMMUNITY_MAP_ITER it_map = m_mapMember.find(uiKey);
	if( it_map != m_mapMember.end() )
		return TRUE;

	return FALSE;
}

RwBool CNtlSLCommunityGroup::AddMember(RwUInt32 uiKey, sCommunityMember* pNewMember)
{
	if( IsMember(uiKey) )
	{
		// avooo's temp
		//DBO_FAIL("Already exist member of key : " << uiKey);
		return FALSE;
	}

	pNewMember->uiKey = uiKey;

	m_listMember.push_back(pNewMember);
	m_mapMember[uiKey] = pNewMember;

	return TRUE;
}

VOID CNtlSLCommunityGroup::DelMember(RwUInt32 uiKey)
{
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		sCommunityMember* pMember = *it_list;
		if( pMember->uiKey == uiKey )
		{
			NTL_DELETE(pMember);
			m_listMember.erase(it_list);
			break;
		}
	}

	m_mapMember.erase(uiKey);
}