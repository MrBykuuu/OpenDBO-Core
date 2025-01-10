/*****************************************************************************
*
* File			: NtlSLGuild.h
* Author		: Hong SungBock
* Copyright		: (¡÷)NTL
* Date			: 2007. 4. 11	
* Abstract		: Simulation guild info class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_GUILD_INFO_H__
#define __NTL_GUILD_INFO_H__

// Core
#include "ceventhandler.h"

// Share
#include "NtlGuild.h"

// Simulation
#include "NtlSLCommunityGroup.h"

struct sGuildMember : public sCommunityMember
{
	CHARACTERID		charID;			///< DB character id	
	RwUInt32		uiWorldNameTable;	///< Region name table index
	RwUInt8			byRace;			///< Race
	RwUInt8			byLevel;		///< level
	RwUInt8			byClass;		///< class	
	RwBool			bOnline;		///< Game access status
	RwUInt32        uiSerialID;     ///< Serial ID (handle)
	RwUInt32		uiReputation;	///< Reputation score
	std::wstring	wstrArea;		///< Current location name
};

struct sGuildNotice
{
	WCHAR awcCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR awcNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
};

enum eGuildDataType
{
	COMMUNITY_GUILD_LEVEL	= COMMUNITY_DATA_NEXT_INDEX,
	COMMUNITY_GUILD_AREA,
};

enum eDOJO_TEAM_STATE
{
	DOJO_TEAM_NONE,
	DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE,
	DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE,
};

class CNtlGuild : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
	CNtlGuild();
	virtual ~CNtlGuild();	

	virtual	RwBool	Create(void);
	VOID			Destroy(void);
	VOID			Leave();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	const WCHAR*	GetGuildName();							///< Returns the guild name

	GUILDID			GetGuildID();
	CHARACTERID		GetMasterID();							///< Returns the guild master ID
	CHARACTERID		GetSecondMasterID(RwInt32 iIndex);		///< Returns the guild assistant master ID

	RwInt32			GetCountSecondMaster();					///< Returns the number of guild assistant masters
	RwUInt32		GetReputation();						///< Returns guild reputation points
	RwUInt32		GetReputationEver();					///< Returns the guild°Øs cumulative reputation points
	time_t			GetTimeToDisband();						///< Returns the time remaining until the guild is disbanded	
	sGuildNotice*	GetNotice();							///< Return of Gald notice
	RwInt32			GetOnlineMemberCount();
	RwUInt8			GetGuildDogiColorIndex();
	RwUInt8			GetDojoDogiColorIndex();

	// After executing Sort, the member structure may be different from the member structure previously received with GetMemberbyIndex().
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	virtual RwBool	IsHaveGroup();
	RwBool			IsBeingDisband();						///< Whether the guild is disbanded
	RwBool			IsGuildMaster(CHARACTERID charID);		///< Whether you are a guild leader or not
	RwBool			IsSecondGuildMaster(CHARACTERID charID);///< Whether you are a deputy guild leader or not
	RwBool			IsHaveEmblem();							///< Whether there is a guild emblem
	RwBool			IsChangedGuildDogiColor();
	RwBool			IsChangedDojoDogiColor();
	RwBool			IsExistFunction(eDBO_GUILD_FUNCTION eFunctionIndex);///< Whether the guild function exists

	RwBool			CanGetGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< Whether you can get the guild function
	RwBool			CanGetDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex);		///< Whether you can get the stamp function

	RwBool			EnableUIGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< Whether the guild function menu can be opened
	RwBool			EnableUIDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< Whether the stamp function menu can be opened

protected:
	VOID			AppointSecondMaster(CHARACTERID charID);	///< Appointed as deputy guild leader
	VOID			DismissSecondMaster(CHARACTERID charID);	///< The position of deputy guild leader is removed.

	VOID			ChangeAllMemberDogiColor(RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor);
	VOID			ChangeMemberDogiColor(CHARACTERID charID, RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor);

	static RwBool	CompareAscentLevel(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return reinterpret_cast<sGuildMember*>( pMember )->byLevel < reinterpret_cast<sGuildMember*>( pMember2 )->byLevel;
	}
	static RwBool	CompareDecentLevel(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return reinterpret_cast<sGuildMember*>( pMember )->byLevel > reinterpret_cast<sGuildMember*>( pMember2 )->byLevel;
	}
	static RwBool	CompareAscentArea(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(reinterpret_cast<sGuildMember*>( pMember )->wstrArea.c_str(),
					   reinterpret_cast<sGuildMember*>( pMember2 )->wstrArea.c_str()) < 0 );
	}
	static RwBool	CompareDecentArea(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(reinterpret_cast<sGuildMember*>( pMember )->wstrArea.c_str(),
					   reinterpret_cast<sGuildMember*>( pMember2 )->wstrArea.c_str()) > 0 );
	}

protected:
	WCHAR					m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME+1];	///< Guild name

	GUILDID					m_guildID;
	
	CHARACTERID				m_masterID;					///< Master ID
	CHARACTERID				m_secondMasterID[DBO_MAX_SECOND_MASTER_IN_GUILD];///< Deputy Master ID

	RwUInt32				m_uiReputation;				///< Guild reputation
	RwUInt32				m_uiMaxReputationEver;		///< Guild maximum accumulated reputation points	
	time_t					m_timeToDisband;			///< Time remaining until the guild is disbanded

	RwInt32					m_iOnlineMember;

	RwUInt64				m_ui64Function;				///< Guild function
	sGuildNotice			m_GuildNotice;				///< Guild Notice

	RwUInt8					m_byGuildDogiColorIndex;	///< School uniform color
	RwUInt8					m_byDojoDogiColorIndex;		///< Dobok color
};

#endif