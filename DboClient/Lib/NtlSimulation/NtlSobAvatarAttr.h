/*****************************************************************************
 *
 * File			: NtlSobAvatarAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30
 * Abstract		: Simulation object avatar attribute base class
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/


#ifndef __NTL_SOBAVATAR_ATTR_H__
#define __NTL_SOBAVATAR_ATTR_H__

#include "NtlSobPlayerAttr.h"

class CNtlSobAvatarAttr : public CNtlSobPlayerAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobAvatarAttr, NTL_DEFAULT_MEMORY_POOL)

public:

	RwUInt32	m_uiExp;		// current exp at current level	
	RwUInt32	m_uiMaxExp;	// max exp at current level

	//////////////////////////////////////////////////////////////////////////

	RwUInt16	m_wApRegen;
	RwUInt16	m_wApSitdownRegen;
	RwUInt16	m_wApBattleRegen;
	RwUInt16	m_wApDegen; // Just hang in air
	RwUInt16	m_wApDegenFly; // When flying
	RwUInt16	m_wApDegenDash; // When dash flying
	RwUInt16	m_wApDegenAccel; // When accel flying

	RwUInt16	m_wRpRegen;			// RP increase speed (/second)
	RwUInt16	m_wRpDimimutionRate;	// RP reduction rate (/second)

	DWORD		m_dwWeightLimit;
	float		m_fItemUpgradeBonusRate;
	BYTE		m_byExpBooster;
	BYTE		m_byQuestDropRate;

	//////////////////////////////////////////////////////////////////////////

	RwUInt32 m_uiZenny;

	RwUInt32 m_uiSp;

	RwUInt32 m_uiHonorPoint;			///< Honor Points
	RwUInt32 m_uiMudosaPoint;			///< Martial Artist Points

	RwBool	 m_bCanChangeClass;			///< Is it possible to change jobs?


	//////////////////////////////////////////////////////////////////////////
	// PC room related
	DWORD    m_dwNetPy;                 ///<
	DWORD    m_dwHlsCash;                 ///<

	//////////////////////////////////////////////////////////////////////////

	bool			m_bInvisibleTitleNameFlag;
	std::wstring	m_wszTitleName;

public:

	CNtlSobAvatarAttr();

	virtual ~CNtlSobAvatarAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_AVATAR); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg& pMsg);

	RwUInt32		GetAP(void);
};


#endif
