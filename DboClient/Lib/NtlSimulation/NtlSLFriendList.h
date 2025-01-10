#pragma once

#include "NtlSLCommunityGroup.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"

struct sFriendMember : public sCommunityMember
{
	CHARACTERID		charID;			///< DB character id
	RwUInt32		mapTblIdx;		///< map table index
	RwUInt8			byLevel;		///< level
	RwUInt8			byClass;		///< class	
	RwBool			bOnline;		///< Game access status
	RwUInt32        uiSerialID;     ///< Serial ID (handle)
	RwUInt8			byChannel;      ///< channel
};

enum FriendDataType
{
	COMMUNITY_FRIEND_LEVEL	= COMMUNITY_DATA_NEXT_INDEX,
	COMMUNITY_FRIEND_ONLINE,
};

/// Class that manages friend list data
/// by agebreak 2007.12.14
class CNtlSLFriendList : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
    CNtlSLFriendList(void);
    virtual ~CNtlSLFriendList(void);

    virtual	RwBool	Create();
    virtual VOID	Destroy();
    virtual VOID	Leave();
    virtual	void	HandleEvents(RWS::CMsg &pMsg);

	// After executing Sort, the member structure may be different from the member structure previously received with GetMemberbyIndex().
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);
    virtual RwBool	IsHaveGroup() {return TRUE;}

protected:
	VOID    OnFriendAddRes(SNtlEventFriendAddRes* pData);
	VOID    OnFriendDelRes(SNtlEventFriendDelRes* pData);
	VOID    OnFriendMoveRes(SNtlEventFriendMoveRes* pData);
	VOID    OnFriendListInfo(SNtlEventFriendListInfo* pData);
	VOID    OnFriendInfo(SNtlEventFriendInfo* pData);
	VOID    OnFriendInfoChange(SNtlEventFriendInfoChange* pData);
	void    OnFriendNameChange(SNtlEventChangeCharName* pData);

    static RwBool CompareAscentLevel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentLevel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareAscentLoc(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentLoc(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareAscentChannel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentChannel(sCommunityMember* pMember1, sCommunityMember* pMember2);
protected:
};
