/*****************************************************************************
 *
 * File			: NtlSobGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation global type
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLGLOBAL_H__
#define __NTL_SLGLOBAL_H__


class CNtlSobAvatar;
class CNtlSLPacketGenerator;
class CNtlNetSender;
class CNtlTriggerSyncManager;

#include "NtlCharacter.h"
#include "NtlItem.h"
#include "NtlMail.h"
#include "NtlSkill.h"
#include "NtlSobSkill.h"
#include "NtlWorld.h"
#include "NtlPacketLockManager.h"
#include "NtlQuest.h"
#include "NtlRankBattle.h"
#include "NtlSLTBudokai.h"
#include "NtlTeleport.h"
#include "NtlDojo.h"

/**
* \brief Avatar Info
*/
typedef struct _SAvatarInfo
{
	RwUInt32			uiSerialId;										// avatar serial id
	sPC_PROFILE			sCharPf;										// avatar profile
	sCHARSTATE			sCharState;										// avatar state
	RwUInt16			wItemCount;									//valid item count
	sITEM_PROFILE		sItemPf[NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM];	// item profile
	RwUInt8				bySkillCount;									// Woohyohan skill count
	sSKILL_INFO			sSkillPf[NTL_MAX_PC_HAVE_SKILL];				// skill profile
	RwUInt8				byHTBCount;										// Valid HTB count
	sHTB_SKILL_INFO		sHTBPf[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];			// HTB profile
	RwUInt8				byBuffCount;
	sBUFF_INFO			sBuffPf[DBO_MAX_BUFF_CHARACTER_HAS];
	sWORLD_INFO			sOldWorldInfo;
	sWORLD_INFO			sWorldInfo;
	sQUEST_COMPLETE_INFO sQuestCompleteInfo;								// Quest completion information
	RwUInt8				byQuestProgressInfoCount;							// Quest progress information count
	sQUEST_PROGRESS_INFO sQuestProgressInfo[eMAX_CAN_PROGRESS_QUEST_NUM];	// Quest progress information
	sQUEST_INVENTORY_FULLDATA sQuestInventoryInfo;							// Quest Inventory Information
	RwUInt8				bySetQuickSlotInfo;									// TRUE if quick slot information is received;
	RwUInt8				byQuickSlotInfoCount;								// Quick Slot Information Count
	sQUICK_SLOT_PROFILE	sQuickSlotInfo[NTL_CHAR_QUICK_SLOT_MAX_COUNT];		// Quick Slot Information
    RwUInt8             byMaxRpStock;                                       ///< Maximum RP Stock Number
	
	RwUInt8				byMailCount;										// Number of mails
	RwUInt8				byUnreadMailCountNormal;							// Unread letters: regular mail
	RwUInt8				byUnreadMailCountManager;							// Unread letter: Manager email

	char				acWarFogFlag[NTL_MAX_SIZE_WAR_FOG];					// Warfog Information

	char				abyTitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];

	sRANKBATTLE_SCORE_INFO sRankBattleScoreInfo;							// Ranked battle record information

	RwUInt8				byDojoBriefCount;
	sDBO_DOJO_BRIEF		sDojoBrief[DBO_MAX_COUNT_DOJO_IN_WORLD];

	PORTALID			aPortalID[NTL_PORTAL_MAX_COUNT];	// portals we have registered

	bool				bCombatMode;

}SAvatarInfo;

/**
* \brief Summon information
*/
typedef struct _SSummonPetInfo
{
	RwUInt32				uiSerialId;			// avatar summon pet serial id
	sSUMMON_PET_PROFILE		sCharPf;			// avatar summon pet profile
	sCHARSTATE				sCharState;			// avatar summon pet character state
}SSummonPetInfo;

/**
* \brief Summon skill information
*/
typedef struct _SSummonPetSkillInfo
{
	RwUInt8				bySkillCount;						// avatar summon pet skill count
	sSKILL_INFO_PET		aSkillInfo[NTL_MAX_NPC_HAVE_SKILL];	// avatar summon pet skill info
}SSummonPetSkillInfo;

/**
* \brief Information about the status of the World's Best Martial Arts Club
*/
typedef struct _STenkaichiBudokaiInfo
{
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;			///< World's Best Martial Arts Main State
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sMatchStateInfo[MAX_BUDOKAI_STATE];	///< Match status
	sBUDOKAI_JOIN_INFO					sJoinInfo;							///< Player's application status
} STenkaichiBudokaiInfo;

/**
* \brief !! AdjustMoney!!
*Saved to match Jenny usage timing between server and client. (Party invitations, emails, etc.)
*/
struct SAdjustZennyInfo
{
	enum ZENNYADJUSTTYPE { USER_TRADE, PARTY_CHARM, MAIL_BOX, PARTY_INVEN, NUM_TYPE };
	
	RwInt32	m_aiAdjustZenny[NUM_TYPE];	

	void	Init(void);
	void	SetAdjustZenny(RwInt32 eType, RwInt32 nValue);	
	RwInt32	GetAdjustZenny(RwInt32 eType);
};

struct SAvatarRecipeInfo
{
	RwUInt16			wCount;
	sRECIPE_DATA		asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];
};

/**
* \brief !! PassiveSkillEffect!!
*Save skill changes according to passive skills. 
*/
struct SPassiveSkillEffect
{
	RwUInt32				SkillTblidx;
	RwUInt8					bySlotidx;
	sDBO_UPDATED_SKILL_DATA	sUpdatedSkillData;
};

typedef std::list<SPassiveSkillEffect>				LIST_PASSIVE_SKILL_EFFECT;
typedef std::list<SPassiveSkillEffect>::iterator	LIST_PASSIVE_SKILL_EFFECT_ITER;	

/**
* \brief GameManiaTime
*Save GMT information
*/
struct SGameManiaTimeInfo
{
	RwUInt8 byGMTResetCount;
	sGAME_MANIA_TIME_DATA sCurrent;
	sGAME_MANIA_TIME_DATA sNext;
};

/**
* \ingroup Simulation
* \brief Global data structure that can be freely accessed from the Simulation layer
*/
class CNtlSLGlobal
{
private:

	static CNtlSLGlobal		*m_pInstance;
	
	SAvatarInfo				m_sAvatarInfo;						///< Data structure containing avatar information when entering the game
	STenkaichiBudokaiInfo	m_sTenkaichiBudokaiInfo;			///< Data structure that contains World's Best Martial Arts information when entering the game
	SSummonPetInfo			m_sSummonPetInfo;					///< Information sent when an avatar summons a summon pet
	SSummonPetSkillInfo		m_sSummonPetSkillInfo;				///< Information (skill) sent when the avatar summons a summon pet
	SAdjustZennyInfo		m_sAdjustZennyInfo;					///< Corrects problems caused by differences in Zeny usage timing between servers and clients.
	SAvatarRecipeInfo		m_sAvatarRecipeInfo;				///< Data structure that contains the avatar°Øs recipe data when entering the game
	SGameManiaTimeInfo		m_sGameManiaTimeInfo;
	LIST_PASSIVE_SKILL_EFFECT m_listPassveSkillEffectInfo;		///< Passive effect application information for each skill provided before avatar creation.

	CNtlSobAvatar			*m_pSobAvatar;						///< avatar object instance pointer
	RwBool					m_bWorldUpdateAvaterPos;
	DBOTIME					m_ServerTimeOffset;					///< (server time -client), used in addition to client time.

	HOBJECT					m_hNpcFacingHandle;

public:

	static bool				m_bSelectSkillMode;
	static SERIAL_HANDLE	m_hSelectSkillModeHandle;
	
public:

	CNtlSLGlobal();
	~CNtlSLGlobal();

	static CNtlSLGlobal* GetInstance(void);

	RwBool Create(RwBool bVirtualNet);
	void Destroy(void);

	void			Reset(void);
	void			ResetTenkaichiBudokaiInfo(void);
	
	SAvatarInfo*	GetAvatarInfo(void);

	SSummonPetInfo*			GetAvatarSummonPetInfo(void);
	SSummonPetSkillInfo*	GetAvatarSummonPetSkillInfo(void);
	STenkaichiBudokaiInfo*	GetTBudokaiStateInfo(void);
	SAdjustZennyInfo*		GetAdjustZennyInfo(void);
	SAvatarRecipeInfo*		GetAvatarRecipeInfo(void);
	SGameManiaTimeInfo*		GetGameManiaTimeInfo(void);
	LIST_PASSIVE_SKILL_EFFECT* GetPassiveSkillEffectInfo(void);
	
	void			SetSobAvatar(CNtlSobAvatar *pSobAvatar);
	CNtlSobAvatar*	GetSobAvatar(void);

	bool			IsSkillSelectMode();
	SERIAL_HANDLE	GetSkillSelectModeHandle();
	
	void			SetWorldUpdateAvatarPosition(RwBool bAvatarPos);
	RwBool			IsWorldUpdateAvaterPosition(void);

	void			SetServerTime( DBOTIME serverTime );
	DBOTIME			GetServerTime(void);

	void			SetNpcFacingHandle(HOBJECT hHandle) { m_hNpcFacingHandle = hHandle; }
	HOBJECT			GetNpcFacingHandle() { return m_hNpcFacingHandle; }
};

static CNtlSLGlobal* GetNtlSLGlobal(void)
{
	return CNtlSLGlobal::GetInstance(); 
}

// Inline ----------
inline SAvatarInfo* CNtlSLGlobal::GetAvatarInfo(void)
{
	return &m_sAvatarInfo;
}

inline SSummonPetInfo* CNtlSLGlobal::GetAvatarSummonPetInfo(void)
{
	return &m_sSummonPetInfo;
}

inline SSummonPetSkillInfo* CNtlSLGlobal::GetAvatarSummonPetSkillInfo(void)
{
	return &m_sSummonPetSkillInfo;
}

inline SAvatarRecipeInfo* CNtlSLGlobal::GetAvatarRecipeInfo(void)
{
	return &m_sAvatarRecipeInfo;
}

inline void CNtlSLGlobal::SetSobAvatar(CNtlSobAvatar *pSobAvatar)
{
	m_pSobAvatar = pSobAvatar;
}

inline void	CNtlSLGlobal::SetWorldUpdateAvatarPosition(RwBool bAvatarPos)
{
	m_bWorldUpdateAvaterPos = bAvatarPos;
}

inline RwBool CNtlSLGlobal::IsWorldUpdateAvaterPosition(void)
{
	return m_bWorldUpdateAvaterPos;
}

inline STenkaichiBudokaiInfo* CNtlSLGlobal::GetTBudokaiStateInfo( void )
{
	return &m_sTenkaichiBudokaiInfo;
}

inline SAdjustZennyInfo* CNtlSLGlobal::GetAdjustZennyInfo( void )
{
	return &m_sAdjustZennyInfo;
}

inline LIST_PASSIVE_SKILL_EFFECT* CNtlSLGlobal::GetPassiveSkillEffectInfo( void )
{
	return &m_listPassveSkillEffectInfo;
}

inline SGameManiaTimeInfo* CNtlSLGlobal::GetGameManiaTimeInfo( void )
{
	return &m_sGameManiaTimeInfo;
}

inline void CNtlSLGlobal::SetServerTime( DBOTIME serverTime )
{
	m_ServerTimeOffset = serverTime - time(NULL);
}

inline DBOTIME CNtlSLGlobal::GetServerTime(void)
{
	return time(NULL) + m_ServerTimeOffset;
}


#endif