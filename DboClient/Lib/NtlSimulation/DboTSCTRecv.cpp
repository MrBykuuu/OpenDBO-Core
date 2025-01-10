#include "precomp_ntlsimulation.h"
#include "DboTSCTRecv.h"
#include "DboTSCTCtrl.h"
#include "DboTSCTAgency.h"
#include "DboTSCQAgency.h"
#include "DboTSCMain.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobManager.h"
#include "TableContainer.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "QuestItemTable.h"
#include "ObjectTable.h"


/** 
	Client trigger receiver
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCTRecv, CDboTSTRecv )


NTL_TSRESULT CDboTSCTRecv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTRecv::Run( CNtlTSEntity* pEntity, void* pParam )
{
	sCTRUN_PARAM* pTParam = (sCTRUN_PARAM*)pParam;

	NTL_TSRESULT tsResult = NTL_TSRESULT_TYPE_SUCCESS;

	switch ( pEntity->GetEntityType() )
	{

//////////////////////////////////////////////////////////////////////////
//
//	Event
//	In the client, line checking is processed only for events, so here
//	Do not run tests
//
//////////////////////////////////////////////////////////////////////////

	case DBO_EVENT_TYPE_ID_CLICK_NPC:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_CLICK_NPC &&
				 ((CDboTSClickNPC*)pEntity)->GetNPCIdx() == pTParam->GetCtrl()->GetEventGenId() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_CLICK_OBJECT:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_CLICK_OBJECT &&
				 ((CDboTSClickObject*)pEntity)->GetWorldIdx() == GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx && 
				 ((CDboTSClickObject*)pEntity)->HasObjectIdx( pTParam->GetCtrl()->GetEventGenId() ) ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_COL_OBJECT:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_COL_OBJECT &&
				 ((CDboTSColObject*)pEntity)->GetWorldIdx() == GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx && 
				 ((CDboTSColObject*)pEntity)->HasObjectIdx( pTParam->GetCtrl()->GetEventGenId() ) ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_ITEM_USE:
		{
			if ( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_ITEM_USE )
			{
				if ( ((CDboTSItemUse*)pEntity)->GetItemIdx() == pTParam->GetCtrl()->GetEventGenId() )
				{
					eEVENT_ITEM_TARGET_TYPE eTargetType = ((CDboTSItemUse*)pEntity)->GetItemTargetType();
					if ( eEVENT_ITEM_TARGET_TYPE_ALL == eTargetType )
					{
						break;
					}

					bool bFind = false;
					vecdef_ItemTargetList& defItemTargetList = pTParam->GetCtrl()->GetEventGenItemTargetInfo();
					vecdef_ItemTargetList::iterator it = defItemTargetList.begin();
					for ( ; it != defItemTargetList.end(); ++it )
					{
						if ( eEVENT_ITEM_TARGET_TYPE_NPC == eTargetType &&
							 SLCLASS_NPC == (*it).uiClassID &&
							 (*it).tblIdx == ((CDboTSItemUse*)pEntity)->GetItemTargetIdx() )
						{
							bFind = true;
							break;
						}

						if ( eEVENT_ITEM_TARGET_TYPE_MOB == eTargetType &&
							 SLCLASS_MONSTER == (*it).uiClassID &&
							 (*it).tblIdx == ((CDboTSItemUse*)pEntity)->GetItemTargetIdx() )
						{
							bFind = true;
							break;
						}

						if ( eEVENT_ITEM_TARGET_TYPE_AVATAR == eTargetType &&
							 SLCLASS_AVATAR == (*it).uiClassID )
						{
							bFind = true;
							break;
						}
					}

					if ( !bFind )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				else
				{
					tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_ITEM_GET:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_ITEM_GET &&
				 ((CDboTSItemGet*)pEntity)->GetItemIdx() == pTParam->GetCtrl()->GetEventGenId() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_ITEM_EQUIP:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_ITEM_EQUIP &&
				 ((CDboTSItemEquip*)pEntity)->GetItemIdx() == pTParam->GetCtrl()->GetEventGenId() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_SCOUT_USE:
		{
			if ( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_SCOUT_USE )
			{
				if ( ((CDboTSScoutUse*)pEntity)->GetItemIdx() == pTParam->GetCtrl()->GetEventGenId() )
				{
					eEVENT_SCOUTER_TARGET_TYPE eTargetType = ((CDboTSScoutUse*)pEntity)->GetTargetType();
					if ( eEVENT_SCOUTER_TARGET_TYPE_ALL == eTargetType )
					{
						break;
					}

					SScouterUseTargetInfo& sTargetInfo = pTParam->GetCtrl()->GetEventGenScouterUseTargetInfo();

					bool bFind = false;

					if ( eEVENT_SCOUTER_TARGET_TYPE_NPC == eTargetType &&
						 SLCLASS_NPC == sTargetInfo.uiClassID &&
						 sTargetInfo.tblIdx == ((CDboTSScoutUse*)pEntity)->GetTargetIdx() )
					{
						bFind = true;
					}

					if ( eEVENT_SCOUTER_TARGET_TYPE_MOB == eTargetType &&
						 SLCLASS_MONSTER == sTargetInfo.uiClassID &&
						 sTargetInfo.tblIdx == ((CDboTSScoutUse*)pEntity)->GetTargetIdx() )
					{
						bFind = true;
					}

					if ( eEVENT_SCOUTER_TARGET_TYPE_AVATAR == eTargetType &&
						 SLCLASS_AVATAR == sTargetInfo.uiClassID )
					{
						bFind = true;
					}

					if ( !bFind )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				else
				{
					tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_RCV_SVR_EVT:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_RCV_SVR_EVT &&
				 ((CDboTSRcvSvrEvt*)pEntity)->GetEvtID() == pTParam->GetCtrl()->GetEventGenId() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_SKILL_USE:
		{
			if ( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_SKILL_USE )
			{
				CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();

				// Skill type test
				if ( eEVENT_SKILL_TYPE_SKILLIDX == pTParam->GetCtrl()->GetEventGenSkillUseInfo().eSkillType )
				{
					if ( 0xffffffff == ((CDboTSSkillUse*)pEntity)->GetSkillIdx() || ( pSkillTable->FindBasicSkillTblidx( ((CDboTSSkillUse*)pEntity)->GetSkillIdx() ) == pSkillTable->FindBasicSkillTblidx( pTParam->GetCtrl()->GetEventGenId() ) ) )
					{
						// Skill target inspection
						eEVENT_SKILL_TARGET_TYPE eTargetType = ((CDboTSSkillUse*)pEntity)->GetSkillTargetType();
						if ( eEVENT_SKILL_TARGET_TYPE_ALL != eTargetType )
						{
							bool bFind = false;
							vecdef_SkillTargetList& defSkillTargetList = pTParam->GetCtrl()->GetEventGenSkillUseInfo().defSkillTargetList;
							vecdef_SkillTargetList::iterator it = defSkillTargetList.begin();
							for ( ; it != defSkillTargetList.end(); ++it )
							{
								if ( eEVENT_SKILL_TARGET_TYPE_NPC == eTargetType &&
									SLCLASS_NPC == (*it).uiClassID &&
									(*it).tblIdx == ((CDboTSSkillUse*)pEntity)->GetSkillTargetIdx() )
								{
									bFind = true;
									break;
								}

								if ( eEVENT_SKILL_TARGET_TYPE_MOB == eTargetType &&
									SLCLASS_MONSTER == (*it).uiClassID &&
									(*it).tblIdx == ((CDboTSSkillUse*)pEntity)->GetSkillTargetIdx() )
								{
									bFind = true;
									break;
								}
							}

							if ( !bFind )
							{
								tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								break;
							}
						}
					}
				}
				else
				{
					if ( ((CDboTSSkillUse*)pEntity)->GetSkillType() != pTParam->GetCtrl()->GetEventGenSkillUseInfo().eSkillType )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
						break;
					}
				}

				// Skill RP Check
				{
					unsigned int uiSkillRPFlags = ((CDboTSSkillUse*)pEntity)->GetSkillRPFlags();

					// If uiSkillRPFlags == 0, RP check is not performed.
					if ( 0 != uiSkillRPFlags )
					{
						unsigned char byRpBonusType = pTParam->GetCtrl()->GetEventGenSkillUseInfo().byRpBonusType;

						switch ( byRpBonusType )
						{
						case DBO_RP_BONUS_TYPE_KNOCKDOWN:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_KNOCKDOWN )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_RESULT_PLUS:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_POWER )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_EP_MINUS:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_EP_MINUS )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_KEEP_TIME_PLUS )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_CASTING_TIME_MINUS )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_COOL_TIME_MINUS:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_COOL_TIME_NINUS )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						case DBO_RP_BONUS_TYPE_GUARD_CRASH:
							{
								if ( !(uiSkillRPFlags & (1<<eEVENT_SKILL_RP_GUARD_CRASH )) )
								{
									tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
								}
							}
							break;
						default:
							{
								tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
							}
							break;
						}

						if ( tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS )
						{
							break;
						}
					}
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_COL_RGN:
		{
			if ( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_COL_REGION )
			{
				CDboTSColRgn* pColRgnEntity = (CDboTSColRgn*)pEntity;
				sCOL_RGN_DATA& sColRgnData = pTParam->GetCtrl()->GetEventGenColRgnInfo();

				if ( 0xffffffff == pColRgnEntity->GetWorldIdx() )
				{
					tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
					break;
				}

				if ( pColRgnEntity->GetWorldIdx() == sColRgnData.uiWorldIdx )
				{
					switch ( sColRgnData.eWorldChangeType )
					{
					case sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_ENTER:
						{
							if ( !(eCOL_RESULT_TYPE_ENTER & pColRgnEntity->GetColCheckType()) ||
								 !pColRgnEntity->InCheck( sColRgnData.fBeginX, sColRgnData.fBeginZ ) )
							{
								tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
							}
						}
						break;

					case sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_LEAVE:
						{
							if ( !(eCOL_RESULT_TYPE_LEAVE & pColRgnEntity->GetColCheckType()) ||
								 !pColRgnEntity->InCheck( sColRgnData.fBeginX, sColRgnData.fBeginZ ) )
							{
								tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
							}
						}
						break;

					case sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_NO:
						{
							if ( !pColRgnEntity->ColCheck( sColRgnData.fBeginX, sColRgnData.fBeginZ, sColRgnData.fEndX, sColRgnData.fEndZ ) )
							{
								tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
							}
						}
						break;

					default:
						{
							tsResult |= NTL_TSRESULT_TYPE_ERROR;
						}
						break;
					}
				}
				else
				{
					tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_RB:
		{
			if ( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_RB )
			{
				SRB_DATA& sRBData = pTParam->GetCtrl()->GetEventGenRBData();

				CDboTSRB* pRB = (CDboTSRB*)pEntity;
				unsigned int uiRBTblIdx = pRB->GetRBTblIdx();
				unsigned int uiRBCondition = pRB->GetRBCondition();

				if ( 0xffffffff == uiRBTblIdx )
				{
					if ( eRB_COND_FLAG_INVALID == (uiRBCondition & sRBData.eRBCondFlag) )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				else
				{
					if ( pTParam->GetCtrl()->GetEventGenId() == uiRBTblIdx )
					{
						if ( eRB_COND_FLAG_INVALID == (uiRBCondition & sRBData.eRBCondFlag) )
						{
							tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
						}
					}
					else
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;
	case DBO_EVENT_TYPE_ID_CLICK_MOB:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_CLICK_MOB &&
				 ((CDboTSClickMob*)pEntity)->GetMobGroupIdx() == pTParam->GetCtrl()->GetEventGenId() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_BIND_STONE:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_BIND_STONE ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_SEARCH_QUEST:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_SEARCH_QUEST ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_ITEM_UPGRADE:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_ITEM_UPGRADE ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_TELEPORT:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_TELEPORT ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_BUDOKAI:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_BUDOKAI ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_SLOTMACHINE:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_SLOTMACHINE ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_HOIPOIMIX:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_HOIPOIMIX ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_PRIVATESHOP:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_PRIVATESHOP ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_FREEBATTLE:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_FREEBATTLE ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_ITEMIDENTITY:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_ITEMIDENTITY ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_EVENT_TYPE_ID_USERMAIL:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_USEMAIL ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

		case DBO_EVENT_TYPE_ID_PARTY:
		{
			if ( !( pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_PARTY ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

		case DBO_EVENT_TYPE_ID_CLICK_SIDEICON:
		{
			if (!(pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_CLICK_SIDEICON))
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

		case DBO_EVENT_TYPE_ID_DIALOGOPEN:
		{
			if (!(pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_DIALOGOPEN))
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

		case DBO_EVENT_TYPE_ID_QUEST:
		{
			if (!(pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_QUEST))
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

		case DBO_EVENT_TYPE_ID_LEVELCHECK:
		{
			if (!(pTParam->GetCtrl()->GetEventGenType() == eEVENT_GEN_TYPE_LEVELCHECK))
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

//////////////////////////////////////////////////////////////////////////
//
//	Condition
//	Must be handled in client module
//
//////////////////////////////////////////////////////////////////////////

	case DBO_COND_TYPE_ID_CHECK_LVL:
		{
			int nLvl = Logic_GetLevel( GetNtlSLGlobal()->GetSobAvatar() );
			CDboTSCheckLvl* pLvl = (CDboTSCheckLvl*)pEntity;

			if ( pLvl->GetMinLvl() > nLvl )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
			else if ( pLvl->GetMaxLvl() < nLvl )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_PCCLS:
		{
			unsigned int uiCls = (unsigned int)((CNtlSobPlayerAttr*)GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr())->GetClass();

			if ( !(((CDboTSCheckPCCls*)pEntity)->GetClsFlags() & (1<<uiCls)) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_PCRACE:
		{
			unsigned int uiRace = (unsigned int)((CNtlSobPlayerAttr*)GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr())->GetRace();

			if ( !(((CDboTSCheckPCRace*)pEntity)->GetRaceFlags() & (1<<uiRace)) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_QITEM:
		{
			CDboTSCheckQItem* pQItem = (CDboTSCheckQItem*)pEntity;

			int nSum = 0;

			CNtlQuestInventory* pQuestInventory = GetNtlSLGlobal()->GetSobAvatar()->GetQuestInventory();
			for ( int i = 0; i < MAX_QUEST_INVENTORY_SLOT; ++i )
			{
				CNtlSobQuestItem* pSobQItem = pQuestInventory->GetQuestItemFromIdx( i );
				if ( pSobQItem )
				{
					CNtlSobQuestItemAttr* pSobQItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>(pSobQItem->GetSobAttr());
					if ( pSobQItemAttr->GetQuestItemTbl()->tblidx == pQItem->GetItemIdx() )
					{
						nSum += pSobQItemAttr->GetStackNum();
					}
				}
			}

			switch ( pQItem->GetCheckType() )
			{
			case eQUEST_ITEM_CHECK_TYPE_EQUAL:
				{
					if ( nSum != pQItem->GetItemCnt() )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				break;
			case eQUEST_ITEM_CHECK_TYPE_MORE:
				{
					if ( nSum < pQItem->GetItemCnt() )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				break;
			case eQUEST_ITEM_CHECK_TYPE_LESS:
				{
					if ( nSum > pQItem->GetItemCnt() )
					{
						tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
					}
				}
				break;

			default:
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_CLRQST:
		{
			CDboTSCheckClrQst* pClrQst = (CDboTSCheckClrQst*)pEntity;
			std::vector<NTL_TS_T_ID>::const_iterator it;

			const std::vector<NTL_TS_T_ID>& AndList = pClrQst->GetAndIdList();
			if ( !AndList.empty() )
			{
				bool bCleared = true;
				for ( it = AndList.begin(); it != AndList.end(); ++it )
				{
					bCleared &= ((CDboTSCMain*)pTParam->GetAgency()->GetParent())->GetQAgency()->GetCompletedQInfo()->HasBeenClearQuest( *it );
				}
				if ( !bCleared ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}

			const std::vector<NTL_TS_T_ID>& OrList = pClrQst->GetOrIdList();
			if ( !OrList.empty() )
			{
				bool bCleared = false;
				for ( it = OrList.begin(); it != OrList.end(); ++it )
				{
					bCleared |= ((CDboTSCMain*)pTParam->GetAgency()->GetParent())->GetQAgency()->GetCompletedQInfo()->HasBeenClearQuest( *it );
					if ( bCleared ) break;
				}
				if ( !bCleared ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_STOCEVT:
		{
			// This condition is for quest use only, so no check is performed in the trigger.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_ITEM:
		{
			CDboTSCheckItem* pItem = (CDboTSCheckItem*)pEntity;

			CNtlInventory *pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

			int nSumCnt = 0;

			// Search Equip window
			for ( int i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i )
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetEquipItem( i ) ) );
				if ( 0 == pSobItem ) continue;

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
				if ( pSobItemAttr->GetTblIdx() == pItem->GetItemIdx() )
				{
					nSumCnt += pSobItemAttr->GetStackNum();
				}
			}

			// Search Bag window
			for ( int i = 0; i < NTL_MAX_BAGSLOT_COUNT; ++i )
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( i ) ) );
				if ( 0 == pSobItem ) continue;

				int nChildCnt = pSobItem->GetChildNum();
				for ( int j = 0; j < nChildCnt; ++j )
				{
					CNtlSobItem* pChildSobItem = pSobItem->GetChildItem( j );
					if ( 0 == pChildSobItem ) continue;

					CNtlSobItemAttr* pChildSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pChildSobItem->GetSobAttr());
					if ( pChildSobItemAttr->GetTblIdx() == pItem->GetItemIdx() )
					{
						nSumCnt += pChildSobItemAttr->GetStackNum();
					}
				}

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
				if ( pSobItemAttr->GetTblIdx() == pItem->GetItemIdx() )
				{
					nSumCnt += pSobItemAttr->GetStackNum();
				}
			}

			if ( pItem->GetItemCnt() > nSumCnt )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_WITEM:
		{
			CDboTSCheckItem* pItem = (CDboTSCheckItem*)pEntity;

			CNtlInventory *pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

			bool bFind = false;

			// Search Equip window
			for ( int i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i )
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetEquipItem( i ) ) );
				if ( 0 == pSobItem ) continue;

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
				if ( pSobItemAttr->GetTblIdx() == pItem->GetItemIdx() &&
					pSobItemAttr->GetStackNum() >= pItem->GetItemCnt() )
				{
					bFind = true;
					break;
				}
			}

			if ( !bFind )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_SSM:
		{
			CDboTSCheckSSM* pSSM = (CDboTSCheckSSM*)pEntity;

			switch ( pSSM->GetOPType() )
			{
			case eSSM_OP_TYPE_EQUAL:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			case eSSM_OP_TYPE_NOT_EQUAL:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			case eSSM_OP_TYPE_LESS:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			case eSSM_OP_TYPE_LESS_EQUAL:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			case eSSM_OP_TYPE_MORE:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			case eSSM_OP_TYPE_MORE_EQUAL:
				if ( pSSM->GetVariable() != pTParam->GetCtrl()->GetSSM( pSSM->GetSSMId() ) ) tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				break;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_OBJITEM:
		{
			// The condition to check whether Object can give an item is
			// Doesn't work in quests
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );
			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_IN_NPC:
		{
			CDboTSCheckInNPC* pInNPC = (CDboTSCheckInNPC*)pEntity;

			RwV3d v3AvatarPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( GetNtlSobManager()->GetNpcTriggerSerialFromId( pInNPC->GetNPCIndex() ) );
			if ( pSobObj )
			{
				RwV3d v3DestPos = pSobObj->GetPosition();

				float fSDist = (v3DestPos.x-v3AvatarPos.x)*(v3DestPos.x-v3AvatarPos.x) + (v3DestPos.z-v3AvatarPos.z)*(v3DestPos.z-v3AvatarPos.z);
				if ( fSDist - (pInNPC->GetRadius()*pInNPC->GetRadius()) > 0.f )
				{
					tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
				}
			}
			else
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_OBJ_STATE:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_IN_WORLD:
		{
			CDboTSCheckInWorld* pInWorld = (CDboTSCheckInWorld*)pEntity;

			if ( pInWorld->GetWorldTblIdx() != GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_OPERATEOBJECT:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_HASCOUPON:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_NPCDEAD:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_DIST_WITH_NPC:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_STOC_DELIVERTY:
		{
			// Doesn't work with triggers
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_ATTACH_OBJ:
		{
			// Condition that operates only on object trigger
			CNtlTSLog::Log( "Not supported condition. Info[%d,%d,%d]. [%s]",
							((CDboTSCTCtrl*)pTParam->GetCtrl())->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							TS_CODE_TRACE() );

			tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_PROG_QUEST:
		{
			CDboTSCheckProgQuest* pProgQuest = (CDboTSCheckProgQuest*) pEntity;

			if ( NULL == ((CDboTSCMain*)pTParam->GetAgency()->GetParent())->GetQAgency()->FindProgressTrigger( pProgQuest->GetQuestID() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_REPUTATION:
		{
			RwUInt32 uiRT = Logic_GetReputation( GetNtlSLGlobal()->GetSobAvatar() );
			CDboTSCheckReputation* pRT = (CDboTSCheckReputation*)pEntity;

			if ( pRT->GetMinReputation() > uiRT )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
			else if ( pRT->GetMaxReputation() < uiRT )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_BUDOKAI_STATE:
		{
			CDboTSCheckBudokaiState* pBudokai = (CDboTSCheckBudokaiState*)pEntity;
			if ( !pBudokai->IsConditionYes( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) )
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CANT_PROGRESS;
			}
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_CUSTOMEVENT:
		{
			// Server-only condition
		}
		break;

	case DBO_COND_TYPE_ID_CHECK_AVATARDEAD:
		{
			// Server-only condition
		}
		break;

		case DBO_COND_TYPE_ID_CHECK_OUTRGN:
		{
			// Server-only condition
		}
		break;

		case DBO_COND_TYPE_ID_LEVEL_CHECK:
		{
			int nLvl = GetNtlSLGlobal()->GetAvatarInfo()->sCharPf.byLevel;
			CDboTSCheckLvl* pLvl = (CDboTSCheckLvl*)pEntity;

			if (nLvl < pLvl->GetMinLvl() || nLvl > pLvl->GetMaxLvl()) //check if pc lower than min lv or higher than max lv
			{
				tsResult |= NTL_TSRESULT_TYPE_COND_CAN_PROGRESS;
			}
		}
		break;

//////////////////////////////////////////////////////////////////////////
//
//	Action
//
//////////////////////////////////////////////////////////////////////////

	case DBO_ACT_TYPE_ID_ACT_ITEM:
		// Server-only action that does not work on the client
		break;

	case DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_S:
		{
			CDboTSActETimerS* pTimer = (CDboTSActETimerS*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pTimer->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pTimer->GetParent())->GetID();
			sKey.taID = pTimer->GetActionId();

			pTParam->GetCtrl()->TG_Avatar_TS_ExceptTimerStartNfy( sKey, pTimer );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_E:
		{
			CDboTSActETimerE* pTimer = (CDboTSActETimerE*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pTimer->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pTimer->GetParent())->GetID();
			sKey.taID = pTimer->GetActionId();

			pTParam->GetCtrl()->TG_Avatar_TS_ExceptTimerEndNfy( sKey, pTimer );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_STOCEVT:
		// This action is for quest use only and is not used in triggers.
		CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
						pTParam->GetCtrl()->GetTrigger()->GetID(),
						((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
						((CNtlTSCont*)pEntity->GetParent())->GetID(),
						((CNtlTSAction*)pEntity)->GetActionId(),
						TS_CODE_TRACE() );
		break;

	case DBO_ACT_TYPE_ID_ACT_QITEM:
		// Server-only action that does not work on the client
		break;

	case DBO_ACT_TYPE_ID_ACT_NPCCONV:
		{
			CDboTSActNPCConv* pConv = (CDboTSActNPCConv*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pConv->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pConv->GetParent())->GetID();
			sKey.taID = pConv->GetActionId();

			pTParam->GetCtrl()->TU_ShowNPCConv( sKey, pConv );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_REGQINFO:
		// This action is for quest use only and is not used in triggers.
		CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
						pTParam->GetCtrl()->GetTrigger()->GetID(),
						((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
						((CNtlTSCont*)pEntity->GetParent())->GetID(),
						((CNtlTSAction*)pEntity)->GetActionId(),
						TS_CODE_TRACE() );
		break;

	case DBO_ACT_TYPE_ID_ACT_DIR:
		{
			// Does nothing on the client
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OPCAM:
		{
			CDboTSActOPCam* pCam = (CDboTSActOPCam*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pCam->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pCam->GetParent())->GetID();
			sKey.taID = pCam->GetActionId();

			pTParam->GetCtrl()->TU_CameraDirection( sKey, pCam );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TWAITTS:
		{
			CDboTSActTWaitTS* pWaitTS = (CDboTSActTWaitTS*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pWaitTS->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pWaitTS->GetParent())->GetID();
			sKey.taID = pWaitTS->GetActionId();

			pTParam->GetCtrl()->TG_WaitTSNfy( sKey, pWaitTS );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_INSSM:
		{
			CDboTSActInSSM* pSSM = (CDboTSActInSSM*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pSSM->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pSSM->GetParent())->GetID();
			sKey.taID = pSSM->GetActionId();

			pTParam->GetCtrl()->TG_InSSMNfy( sKey, pSSM );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TSSTATE:
		{
			CDboTSActTSState* pTSState = (CDboTSActTSState*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pTSState->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pTSState->GetParent())->GetID();
			sKey.taID = pTSState->GetActionId();

			pTParam->GetCtrl()->TG_TSStateNfy( sKey, pTSState );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_PORTAL:
		{
			// Portal does not work on the client
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OBJSTATE:
		{
			// Code that changes the state of an object on the client does not work.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_CONC_CHECK:
		{
			// The client does not perform simultaneous checks.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OBJCONV:
		{
			CDboTSActObjConv* pConv = (CDboTSActObjConv*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pConv->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pConv->GetParent())->GetID();
			sKey.taID = pConv->GetActionId();

			pTParam->GetCtrl()->TU_ShowObjConv( sKey, pConv );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_HINT:
		{
			CDboTSActHint* pHint = (CDboTSActHint*)pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pHint->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pHint->GetParent())->GetID();
			sKey.taID = pHint->GetActionId();

			pTParam->GetCtrl()->TU_Hint( sKey, pHint );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_SEND_SVR_EVT:
	case DBO_ACT_TYPE_ID_ACT_TMQ_STAGE:
		{
			// Server-only action that does not work on the client
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OPEN_WINDOW:
		{
			CDboTSActOpenWindow* pAct = (CDboTSActOpenWindow*) pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pAct->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pAct->GetParent())->GetID();
			sKey.taID = pAct->GetActionId();

			pTParam->GetCtrl()->TU_Open_Window( sKey, pAct );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TMQ_TIMEBONUS:
		{
			// The time bonus does not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TELECAST:
		{
			// The client does not perform broadcasting operations.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_DIRINDICATOR:
		{
			// The client does not perform any action for direction indication.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OPERATEOBJECT:
		{
			// No action is taken on object operations on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_DROP:
		{
			// There is no action on the drop from the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_RMV_COUPON:
		{
			// Coupons do not work in the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_ESCORT:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TMQINFOTYPE:
		{
			// TMQ info type does not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_WORLDPLAYSCRIPT:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_SWPROBSF:
		{
			// Switch probability success fail does not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_PCCONV:
		{
			CDboTSActPCConv* pAct = (CDboTSActPCConv*) pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pAct->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pAct->GetParent())->GetID();
			sKey.taID = pAct->GetActionId();

			pTParam->GetCtrl()->TU_PC_ConvNfy( sKey, pAct );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_CUSTOMEVT:
		{
			// It does not work with custom events on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TELMUDOSA:
		{
			CDboTSActTelMudosa* pAct = (CDboTSActTelMudosa*) pEntity;

			sTS_KEY sKey; sKey.Init();
			sKey.tID = pTParam->GetCtrl()->GetTrigger()->GetID();
			sKey.tgID = ((CNtlTSGroup*)pAct->GetParent()->GetParent())->GetID();
			sKey.tcID = ((CNtlTSCont*)pAct->GetParent())->GetID();
			sKey.taID = pAct->GetActionId();

			pTParam->GetCtrl()->TU_TelMudosa( sKey, pAct );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TELMINORMATCH:
		{
			// It does not work in relation to teleport minor matches on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_PIDGN:
		{
			// It does not work in relation to Party instance dungeon on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TLQ:
		{
			// Client does not work on Time leap quest dungeon
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_DO_SKILL:
		{
			// It does not work regarding Do skill on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_TOBJ_FRIENDLY:
		{
			// Trigger object friendly does not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_BROAD_MSG:
		{
			// Broad messages do not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_MINI_NARRATION:
		{
			// Mini narration does not work on the client.
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_REG_C_TIMING:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_EXC_C_GROUP:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_SKIP_CONT:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

	case DBO_ACT_TYPE_ID_ACT_OBJ_WPS:
		{
			// This action is for quest use only and is not used in triggers.
			CNtlTSLog::Log( "It must be only used in the quest trigger. Info[%d,%d,%d,%d]. [%s]",
							pTParam->GetCtrl()->GetTrigger()->GetID(),
							((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID(),
							((CNtlTSCont*)pEntity->GetParent())->GetID(),
							((CNtlTSAction*)pEntity)->GetActionId(),
							TS_CODE_TRACE() );
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_DOJO:
		{
			// Client does not work with Dojo
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_OUT_MSG:
		{
			// to do by daneos
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_WPSFD:
		{
			// to do by daneos
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_MAIL:
		{
			// to do by daneos
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_AVATARDEAD:
		{
			// to do by daneos
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_SKYDGN:
		{
			// to do by daneos
		}
		break;
	}

	return tsResult;
}
