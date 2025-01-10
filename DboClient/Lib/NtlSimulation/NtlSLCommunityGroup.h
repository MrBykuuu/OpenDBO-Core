/*****************************************************************************
*
* File			: NtlSLCommunityGroup.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : Primitive class for managing data of related groups during the game
*
*Members are managed by overlapping maps and lists, which is difficult for large groups.
*For small groups where members join and withdraw frequently,
*This is because it is judged that the load of joining and withdrawing members is not large.
*****************************************************************************/

#ifndef __NTL_COMMUNITY_GROUP_H__
#define __NTL_COMMUNITY_GROUP_H__

#include <algorithm>

// Shared
#include "NtlSharedDef.h"


struct sCommunityMember
{
	RwUInt32		uiKey;
	WCHAR			wszMemberName[NTL_MAX_SIZE_CHAR_NAME + 1];	///< member name
};

typedef std::map<RwUInt32, sCommunityMember*>			COMMUNITY_MAP;
typedef std::map<RwUInt32, sCommunityMember*>::iterator	COMMUNITY_MAP_ITER;

typedef std::list<sCommunityMember*>					COMMUNITY_LIST;
typedef std::list<sCommunityMember*>::iterator			COMMUNITY_ITER;



// Macro for Sort function
#define dCOMMUNITY_GROUP_SORT(bAscentValue, function_Ascent, function_Decent) \
{	\
	if( bAscentValue )	\
	std::stable_sort(m_listMember.begin(), m_listMember.end(), function_Ascent);	\
	else	\
	std::stable_sort(m_listMember.begin(), m_listMember.end(), function_Decent);	\
	break;	\
}

// The data index to be sorted is defined by eCommunityDataType.
// Since the Sort for each community does not affect each other, the index is defined below.
// It is okay to overlap.
enum eCommunityDataType
{
	COMMUNITY_DATA_NAME,

	COMMUNITY_DATA_NEXT_INDEX,

    // Used when sorting the friend list
    COMMUNITY_DATA_LEVEL,
    COMMUNITY_DATA_LOC,
    COMMUNITY_DATA_CHANNEL,
};



class CNtlSLCommunityGroup
{
public:
	CNtlSLCommunityGroup() {}
	virtual ~CNtlSLCommunityGroup() {}

	virtual	RwBool	Create() = 0;
	virtual VOID	Destroy() = 0;

	// Depending on the community, there may be data held dynamically.
	// Let's override them as functions.
	virtual VOID	ClearMember();
	virtual VOID	Leave()	= 0;

	sCommunityMember* GetMemberbyIndex(RwInt32 iIndex);
	sCommunityMember* GetMemberbyKey(RwUInt32 uiKey);
	sCommunityMember* GetMemberbyName(WCHAR* pwcName);
	RwInt32			GetMemberCount();

	COMMUNITY_ITER	Begin();
	COMMUNITY_ITER	End();

	// After executing Sort, the member structure may be different from the member structure previously received with GetMemberbyIndex().
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	// To receive all member lists and update the GUI all at once
	// To check whether you belong to a group, do not check whether there are members or not.
	// Manage it with separate logic. Example) Does the guild have a name?
	virtual RwBool	IsHaveGroup() = 0;
	RwBool			IsMember(RwUInt32 uiKey);

protected:
	RwBool			AddMember(RwUInt32 uiKey, sCommunityMember* pNewMember);
	VOID			DelMember(RwUInt32 uiKey);

	static RwBool	CompareAscentName(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(pMember->wszMemberName, pMember2->wszMemberName) < 0 );
	}
	static RwBool	CompareDecentName(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(pMember->wszMemberName, pMember2->wszMemberName) > 0 );
	}

protected:
	COMMUNITY_MAP			m_mapMember;			///< Container for search
	COMMUNITY_LIST			m_listMember;			///< Aligned container for GUI
};

#endif