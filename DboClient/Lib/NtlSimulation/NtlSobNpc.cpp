#include "precomp_ntlsimulation.h"
#include "NtlSobNpc.h"

// Shared
#include "NpcTable.h"
#include "NtlCharacter.h"

// Core
#include "NtlDebug.h"
#include "NtlMath.h"

// Presentation
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"
#include "NtlPLEvent.h"

// Simulation
#include "TableContainer.h"
#include "NtlSobManager.h"
#include "NtlSobAttrFactory.h"
#include "NtlSobNpcAttr.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlFSMCharActLayer.h"
#include "NtlBehaviorData.h"
#include "NtlSobCharProxy.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "DboTSCQAgency.h"
#include "DboTSCTAgency.h"


DEFINITION_MEMORY_POOL(CNtlSobNpc)

CNtlSobNpc::CNtlSobNpc()
{
	m_fTime				= 0.0f;
	m_byQuestMark		= (RwUInt8)EQuestMarkNone;
	m_bActiveQuestMark	= TRUE;
	m_tID				= NTL_TS_T_ID_INVALID;

	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_CAN_SKILLUSE | SLFLAG_LUA_INTERLOCKING | SLFLAG_LUA_TRANSFORM_ENABLE | 
			SLFLAG_TARGET_SELECT_GUI_UPDATE | SLFLAG_TARGET_NEAR_APPROACH);
	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE | SLFLAG_ACTOR_HAVE_MOVEABLE | SLFLAG_ACTOR_HAVE_BATTLE);
}

CNtlSobNpc::~CNtlSobNpc()
{
}

RwBool CNtlSobNpc::Create(void)
{
	NTL_FUNCTION("CNtlSobNpc::Create");

	//proxy settings
	m_pSobProxy = NTL_NEW CNtlSobCharProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	// control data create
	m_pBeData = NTL_NEW CNtlBeCharData; 
	
	if(!CNtlSobFeelingActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	//Set class name.
	SetClassName(SLCLASS_NAME_NPC);

	NTL_RETURN(TRUE);
}

void CNtlSobNpc::Destroy(void)
{
	NTL_FUNCTION("CNtlSobNpc::Destroy");

	// behavior data
	NTL_DELETE( m_pBeData );

	// actor destroy
	CNtlSobFeelingActor::Destroy();
	
	NTL_RETURNVOID();
}

void CNtlSobNpc::Update(RwReal fElapsed)
{
	QeustMarkCheck(fElapsed);

	CNtlSobFeelingActor::Update(fElapsed);
}

CNtlFSMLayer* CNtlSobNpc::CreateFSM(void)
{
	CNtlFSMLayer *pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
	pFSMLayer->SetActor(this);
	pFSMLayer->Create(SLCONTROLLER_SERVER); 
	
	return pFSMLayer;
}

void CNtlSobNpc::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobNpc::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobNpcCreate *pSobCreate = reinterpret_cast<SNtlEventSobNpcCreate*>(pMsg.pData);

		// game attribute data setting.
		GetSobAttr()->HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		//Setting coordinates and direction.
		RwV3d vLoc, vDir;
		RwV3dAssignMacro(&vLoc, &pSobCreate->vLoc); 
		RwV3dAssignMacro(&vDir, &pSobCreate->vDir); 

		if(fabs(vDir.x) < NTL_EPSILON && fabs(vDir.y) < NTL_EPSILON && fabs(vDir.z) < NTL_EPSILON)
		{
			NtlLogFilePrintArgs("CNtlSobNpc create is direction error - table(%d) ", pSobCreate->pNpcBrief->tblidx);
			CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 0.0f, 1.0f);
		}

		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(this, &vLoc, sHStuff);
		vLoc.y = sHStuff.fFinialHeight;
		
		SetPosition(&vLoc);
		SetDirection(&vDir);

		CNtlSobNpcAttr *pSobNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( GetSobAttr() );
		RwReal fScale = pSobNpcAttr->GetScale();
		GetSobProxy()->SetScale(fScale);

		if(pSobCreate->pCharState->sCharStateBase.dwConditionFlag & CHARCOND_FLAG_CLIENT_UI_DISABLE)
		{
			ActiveQuestMark( FALSE );
		}
		else
		{
			ActiveQuestMark( TRUE );
		}

		// Determine whether heading should be changed when attacked.
		CNtlSobNpcAttr *pNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( GetSobAttr() );
		sNPC_TBLDAT *pNpcTable = pNpcAttr->GetNpcTbl();
		if(pNpcTable)
		{
			if(pNpcTable->dwFunc_Bit_Flag & NPC_FUNC_FLAG_TURN_OFF)
			{
				SetFlags(GetFlags() | SLFLAG_HURT_NOT_HEADING_CHANGE);
			}
		}
	}
	else if(pMsg.Id == g_EventSobServerUpdateCondition)
	{
		SNtlEventSobServerUpdateCondition *pSobServerState = reinterpret_cast<SNtlEventSobServerUpdateCondition*>( pMsg.pData );
		if(pSobServerState->dwCondition & CHARCOND_FLAG_CLIENT_UI_DISABLE)
		{
			ActiveQuestMark( FALSE );
		}
		else
		{
			ActiveQuestMark( TRUE );
		}
	}
	else if(pMsg.Id == g_EventQuestMark)
	{
		GetSobProxy()->HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventChangeNickName )
	{
		GetSobAttr()->HandleEvents(pMsg);
	}
    else if(pMsg.Id == g_EventAnimEnd)
    {
        //When using the store, play the event animation and change it to a loop
        SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*)pMsg.pData;
        if(pData->uiSerialId == GetSerialID() && (pData->uiBaseAnimKey == SOC_NO || pData->uiBaseAnimKey == SOC_BYE || pData->uiBaseAnimKey == SOC_AGREE))
        {
            GetSobProxy()->SetBaseAnimation(NML_IDLE_LOOP, TRUE);
        }
    }

	CNtlSobFeelingActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}

void CNtlSobNpc::QeustMarkCheck(RwReal fElapsed)
{
	if(!m_bActiveQuestMark)
		return;

	m_fTime += fElapsed;
	if(m_fTime < 1.0f)
		return;

	CNtlSobNpcAttr *pSobNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( GetSobAttr() );
	sNPC_TBLDAT *pNpcTblData = pSobNpcAttr->GetNpcTbl();
	if(pNpcTblData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_QUEST_GRANTER)
	{
		RwUInt8 byQuestMark = (RwUInt8)EQuestMarkNone;

		CDboTSCQAgency *pQuestAgency = API_GetQuestAgency();
		vecdef_QUEST_INFO_LIST *pQuestInfoList = NULL;

		sQUEST_INFO* pQuestInfo = NULL;
		RwBool		bFirstNewQuest = TRUE;
		RwUInt16	tID = NTL_TS_T_ID_INVALID;

		if(pQuestAgency)
		{
			vecdef_QUEST_INFO_LIST::iterator it;
			SGET_QUEST_INFO* pGetQuestInfoList =  pQuestAgency->GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_NPC, Logic_GetNPCTriggerId(this));
			pQuestInfoList = &pGetQuestInfoList->QuestInfoList;
						
			for(it = pQuestInfoList->begin(); it != pQuestInfoList->end(); ++it)
			{
				if ( (*it).bVisitOP )
				{
					byQuestMark = byQuestMark | (1 << EQuestMarkVisit );
				}

				if((*it).eProgType == eQUEST_PROGRESS_TYPE_NEW)
				{
					byQuestMark = byQuestMark | (1 << EQuestMarkNew);
					if( bFirstNewQuest )
					{
						tID = (*it).sKey.tID;
						pQuestInfo = &(*it);
						bFirstNewQuest = FALSE;
					}
				}
				else if((*it).eProgType == eQUEST_PROGRESS_TYPE_PROGRESS)
					byQuestMark = byQuestMark | (1 << EQuestMarkProgress);
				else
					byQuestMark = byQuestMark | (1 << EQuestMarkReward);
			}

			if(byQuestMark & EQuestMarkFlagReward)
			{
				byQuestMark = EQuestMarkReward;
			}
			else if(byQuestMark & EQuestMarkFlagProgress)
			{
				byQuestMark = EQuestMarkProgress;
			}
			else if(byQuestMark & EQuestMarkFlagNew)
			{
				RwReal fDist = Logic_GetQuestNpcDistanceFromAvatar(this);
				if( fDist >= NTL_QUEST_MARK_BALLOON_DIST ||
					pQuestInfo->dwQuestStartDialog == INVALID_TBLIDX )
					byQuestMark = EQuestMarkNew;
				else
					byQuestMark = EQuestMarkBalloonNew;
			}
			else if(byQuestMark & EQuestMarkFlagVisit)
			{
				byQuestMark = EQuestMarkVisit;
			}
			else
			{
				byQuestMark = EQuestMarkNone;
			}

			if( ( m_byQuestMark != byQuestMark ) ||
				( byQuestMark == EQuestMarkBalloonNew && m_tID != tID ) )
			{
				//event occurs.
				m_byQuestMark = byQuestMark;
				m_tID = tID;
				CNtlSLEventGenerator::QuestMark(GetSerialID(), m_byQuestMark, (void*)pQuestInfo);
			}		
		}
	}

	m_fTime = 0.0f;
}


void CNtlSobNpc::ActiveQuestMark(RwBool bQuestMark)
{
	if(m_bActiveQuestMark && !bQuestMark)
	{
		if(m_byQuestMark != EQuestMarkNone)
		{
			//Send none.
			m_byQuestMark = EQuestMarkNone;
			CNtlSLEventGenerator::QuestMark(GetSerialID(), m_byQuestMark, NULL);
		}
	}
	else if(!m_bActiveQuestMark && bQuestMark)
	{
	}

	m_fTime = 0.0f;
	m_bActiveQuestMark = bQuestMark;
}