/*****************************************************************************
*
* File			: NtlParty.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2006. 10. 20	
* Abstract		: Simulation party info class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PARTY_INFO_H__
#define __NTL_PARTY_INFO_H__

// Core
#include "ceventhandler.h"

// Share
#include "NtlParty.h"
#include "NtlPartyDungeon.h"

// Simulation
#include "NtlSharedDef.h"
#include "NtlSLDef.h"
#include "NtlSLCommunityGroup.h"

#define dINVALID_CHARM_BUFF_INDEX	NTL_PARTY_CHARM_INVENTORY_COUNT

class CNtlFakeBuffContainer;
struct sITEM_TBLDAT;
struct sCHARM_TBLDAT;

struct sPartyMember : public sCommunityMember
{
	SERIAL_HANDLE	hSerial;		///< member handle
	RwUInt8			byRace;			///< Race
	RwUInt8			byClass;		///< Occupation
	RwUInt8			byLevel;		///< level
	RwUInt16		wCurLP;			///< Current LP
	RwUInt16		wMaxLP;			///< Max LP
	RwUInt16		wCurEP;			///< Current EP
	RwUInt16		wMaxEP;			///< Max EP
	RwUInt32		uiWorldID;		///< World ID
	RwUInt32		uiWorldTbl;		///< World Table Index
	RwV3d			vPos;			///< Location (receives packets when out of line of sight)
	SERIAL_HANDLE	hBusSerial;		///< Steering wheel of the bus you are riding

	CNtlFakeBuffContainer*	pFakeBuffContainer;		///< Save buff information of members except yourself
};

enum ePartyMemberDataType
{
	PARTY_MEMBER_DATA_RACE,

	NUM_PARTY_MEMBER_DATA
};

class CNtlParty : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
	CNtlParty();
	virtual ~CNtlParty();

	virtual	RwBool	Create(void);
	VOID			Update(RwReal fElapsed);
	virtual VOID	Destroy(void);

	virtual VOID	ClearMember();
	virtual VOID	Leave();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	VOID			SetPartyName(WCHAR* pcName);			///< Party name setting

	const WCHAR*	GetPartyName();							///< Returns the party name
	SERIAL_HANDLE	GetLeaderHandle();						///< returns the leader handle
	RwInt32			GetAvatarIndex();

	// Distribution method return
	RwUInt8				 GetZennyLootingMethod();
	RwUInt8				 GetItemLootingMethod();

	ePARTY_DUNGEON_STATE GetPartyDungeonState();

	// After executing Sort, it may be different from the member structure previously received with GetMemberbyIndex ()
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	virtual RwBool	IsHaveGroup();

    sSHARETARGET_INFO*  GetShareTargetInfos() {return m_aShareTargetInfo;}      ///< Returns party shared target information

protected:
    void            PartyShareTargetResHandler(RWS::CMsg& pMsg);         ///< Party sharing target response processing
    void            PartyShareTargetNfyHandler(RWS::CMsg& pMsg);         ///< Party sharing target notification processing 

protected:
	WCHAR					m_pwPartyName[NTL_MAX_SIZE_PARTY_NAME+1];	///< Party name

	SERIAL_HANDLE			m_hLeaderHandle;			///< Party leader handle

	sSHARETARGET_INFO       m_aShareTargetInfo[NTL_MAX_SHARETARGET_COUNT];        // Party shared target information, the index of the array is the shared target number

	RwUInt8					m_byZennyLootingMethod;		///< Zenny distribution method
	RwUInt8					m_byItemLootingMethod;		///< Item distribution method

	ePARTY_DUNGEON_STATE	m_eDungeonState;			///< Status of party instance dungeon
};

inline RwUInt8 CNtlParty::GetZennyLootingMethod()
{
	return m_byZennyLootingMethod;
}

inline RwUInt8 CNtlParty::GetItemLootingMethod()
{
	return m_byItemLootingMethod;
}

inline ePARTY_DUNGEON_STATE CNtlParty::GetPartyDungeonState()
{
	return m_eDungeonState;
}

#endif