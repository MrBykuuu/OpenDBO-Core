#include "precomp_ntlsimulation.h"
#include "NtlSobMonsterAttr.h"

// Shared
#include "MobTable.h"
#include "MobMovePatternTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Core
#include "NtlDebug.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"


DEFINITION_MEMORY_POOL(CNtlSobMonsterAttr)

CNtlSobMonsterAttr::CNtlSobMonsterAttr()
{
	m_pMobTbl			= NULL;
	m_pMovePatternTbl	= NULL;
	m_eDragonBallType = DRAGON_BALL_TYPE_NONE;
}

RwBool CNtlSobMonsterAttr::IsHaveDragonBall()
{
	if( m_eDragonBallType == DRAGON_BALL_TYPE_NONE )
		return false;

	return true;
}

void CNtlSobMonsterAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobMonsterCreate *pSobMonsterCreate = (SNtlEventSobMonsterCreate*)pMsg.pData;
		CMobTable *pMobTbl		= API_GetTableContainer()->GetMobTable();
		CTextTable *pTextTable	= API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
		m_pMobTbl				= (sMOB_TBLDAT*)pMobTbl->FindData(pSobMonsterCreate->pMobBrief->tblidx);

		CMobMovePatternTable *pPatternTbl	= API_GetTableContainer()->GetMobMovePatternTable();
		m_pMovePatternTbl					= (sMOVE_PATTERN_TBLDAT*)pPatternTbl->FindData(pSobMonsterCreate->pMobBrief->actionpatternTblIdx);

		SetLevel(m_pMobTbl->byLevel);
		SetLp((RwInt32)pSobMonsterCreate->pMobBrief->curLp);
		SetMaxLp((RwInt32)pSobMonsterCreate->pMobBrief->maxLp);
		SetEp((RwInt32)pSobMonsterCreate->pMobBrief->wCurEP);
		SetMaxEp((RwInt32)pSobMonsterCreate->pMobBrief->wMaxEP);
		SetRadius(m_pMobTbl->fRadius);
		SetModelName(m_pMobTbl->szModel);

		// If generated by client like cinematic
		// Do not display the name.
        if(pSobMonsterCreate->bIsClientCreate)
        {
            SetNameCreate(FALSE);
        }
        else
        {
            sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( m_pMobTbl->Name ) );
            if(pTextTblData)
                SetName(pTextTblData->wstrText.c_str());
            else
                SetNameCreate(FALSE);

            SetNameColor(NTL_MONSTER_NAME_COLOR);
        }

		SetDefaultWalkSpeed(pSobMonsterCreate->pMobBrief->fLastWalkingSpeed);
		SetDefaultRunSpeed(pSobMonsterCreate->pMobBrief->fLastRunningSpeed);
		SetWalkSpeed(pSobMonsterCreate->pMobBrief->fLastWalkingSpeed);
		SetRunSpeed(pSobMonsterCreate->pMobBrief->fLastRunningSpeed);        		        
        SetRunAnimSpeed(m_pMobTbl->fRun_Speed / m_pMobTbl->fRun_Speed_Origin);
		if(pSobMonsterCreate->pMobBrief->wAttackSpeedRate > 0)
			SetAttackAnimSpeed((RwReal)1000/(RwReal)pSobMonsterCreate->pMobBrief->wAttackSpeedRate);
		SetFlyHeight(m_pMobTbl->fFly_Height);
		SetScale(m_pMobTbl->fScale);
		SetAttackRange(m_pMobTbl->fAttack_Range);

		m_eDragonBallType = (eDRAGON_BALL_TYPE)pSobMonsterCreate->pMobBrief->byBallType;
	}
}

void CNtlSobMonsterAttr::SetMovePatternId(RwUInt8 byPatternId)
{
	m_byMovePatternId = byPatternId;
}

RwUInt8 CNtlSobMonsterAttr::GetMovePatternId(void) const
{
	return m_byMovePatternId;
}