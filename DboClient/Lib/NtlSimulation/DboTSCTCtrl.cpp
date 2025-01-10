#include "precomp_ntlsimulation.h"
#include "DboTSCTCtrl.h"
#include "DboTSCTRecv.h"
#include "DboTSCTAgency.h"
#include "NtlSLApi.h"
#include "NtlClock.h"
#include "NTLResultCode.h"
#include "DboTSDefine.h"
#include "NtlSLPacketGenerator.h"


/**
	Client trigger controller
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCTCtrl, CDboTSTCtrl )


CDboTSCTCtrl::CDboTSCTCtrl( void )
{
}

CDboTSCTCtrl::~CDboTSCTCtrl( void )
{
}

void CDboTSCTCtrl::Update( void )
{
	//In case of termination state
	if ( IsExitState() ) return;

	// Exception Timer Update
	UpdateExceptionTimer();

	// Wait timer update
	UpdateTimeWait();

	// Standby timer operation
	if ( !IsTimeWait() )
	{
		// If the synchronization queue is running, do not proceed to the next step.
		if ( IsContSyncQueueEmpty() )
		{
			// Proceed with the quest
			UpdateTSStep();
		}
	}
}

unsigned int CDboTSCTCtrl::GetCurTime( void )
{
	if ( API_GetGameClock() ) return API_GetGameClock()->GetTime_AvatarTime();
	return 0;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_GCond( CNtlTSCont* pCont )
{
	CDboTSContGCond* pContGCond = (CDboTSContGCond*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContGCond->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContGCond->GetID();

	sCTRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	// General conditions: Container runs on the client first and requests verification from the server if there are no problems.
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the trigger container. Info[%d,%d]. [%s]", tID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// Satisfies the conditions
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGCond->GetYesLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, pContGCond->GetYesLinkID(), TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_ERROR;
		}

		// Ask the server if you can proceed to the next step
		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
	// The conditions are not satisfied now, but the conditions may be satisfied next time.
	// If the conditions are not satisfied now and the conditions will never be satisfied in the future
	else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
	{
		// If there is a link connected to No, run it.
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGCond->GetNoLinkID() );
		if ( pNextCont )
		{
			// Ask the server if you can proceed to the next step
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
		}
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_GAct( CNtlTSCont* pCont )
{
	CDboTSContGAct* pContGAct = (CDboTSContGAct*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContGAct->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContGAct->GetID();

	// A typical action container requests execution from the server and executes when a response is received.
	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGAct->GetNextLinkID() );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, ((CDboTSContGAct*)m_pCurTSP)->GetNextLinkID(), TS_CODE_TRACE() );
		return NTL_TSRESULT_TYPE_ERROR;
	}

	// Ask the server if you can proceed to the next step
	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_UserSel( CNtlTSCont* pCont )
{
	CDboTSContUsrSel* pContUsr = (CDboTSContUsrSel*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContUsr->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContUsr->GetID();

	// Display a user selection window to the user
	sTS_KEY sKey; sKey.Init();
	sKey.tID = tID;
	sKey.tgID = tgID;
	sKey.tcID = tcID;

	TU_ShowUserSelectionDialog( sKey, pContUsr );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_Reward( CNtlTSCont* pCont )
{
	CDboTSContReward* pContRwd = (CDboTSContReward*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContRwd->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContRwd->GetID();

	sCTRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	// The compensation container runs first on the client and requests verification from the server if there are no problems.
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the trigger container. Info[%d,%d,%d]. [%s]", tID, tgID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// Satisfies the conditions
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		// Output dialogue to the user.
		// Additionally, the user must select one of the given rewards and notify the user.
		sTS_KEY sKey; sKey.Init();
		sKey.tID = tID;
		sKey.tgID = tgID;
		sKey.tcID = tcID;

		TU_ShowRewardDialog( sKey, pContRwd );
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_Start( CNtlTSCont* pCont )
{
	CDboTSContStart* pContStart = (CDboTSContStart*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContStart->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContStart->GetID();

	sCTRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	// The starting container runs first on the client and requests verification from the server if there are no problems.
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the trigger container. Info[%d,%d,%d]. [%s]", tID, tgID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// Satisfies the conditions
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContStart->GetYesLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, pContStart->GetYesLinkID(), TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_ERROR;
		}

		// Ask the server if you can proceed to the next step
		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
	// If the startup container cannot proceed to the next step, execute No unconditionally.
	else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
	{
		// If there is a link connected to No, run it.
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContStart->GetNoLinkID() );

		if ( pNextCont )
		{
			// Ask the server if you can proceed to the next step
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
		}
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_End( CNtlTSCont* pCont )
{
	CDboTSContEnd* pContEnd = (CDboTSContEnd*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContEnd->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContEnd->GetID();

	// Ask the server if you can proceed to the next step
	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( tcID, NTL_TS_TC_ID_INVALID, uiParam, GetEventType(), GetEventData() );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_Narration( CNtlTSCont* pCont )
{
	CDboTSContNarration* pContNarration = (CDboTSContNarration*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContNarration->GetParent())->GetID();
	sKey.tcID = pContNarration->GetID();

	// Output narration dialogue to the user.
	TU_ShowNarrationDialog( sKey, pContNarration );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_Proposal( CNtlTSCont* pCont )
{
	CDboTSContProposal* pContProposal = (CDboTSContProposal*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContProposal->GetParent())->GetID();
	sKey.tcID = pContProposal->GetID();

	CNtlTSCont* pRwd = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( pContProposal->GetReward() );

	if ( pRwd && pRwd->IsDerivedClass( "CDboTSContReward" ) )
	{
		TU_ShowProposalDialog( sKey, pContProposal, (CDboTSContReward*)pRwd );
	}
	else
	{
		TU_ShowProposalDialog( sKey, pContProposal, 0 );
	}

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_Switch( CNtlTSCont* pCont )
{
	CDboTSContSwitch* pContSwitch = (CDboTSContSwitch*)pCont;
	NTL_TS_TG_ID tcID = pContSwitch->GetID();

	// Ask the server if you can proceed to the next step
	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( tcID, NTL_TS_TC_ID_INVALID, uiParam, GetEventType(), GetEventData() );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCTCtrl::Cont_UnifiedNarration( CNtlTSCont* pCont )
{
	CDboTSContUnifiedNarration* pContNarration = (CDboTSContUnifiedNarration*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContNarration->GetParent())->GetID();
	sKey.tcID = pContNarration->GetID();

	// Output narration dialogue to the user.
	TU_ShowUnifiedNarrationDialog( sKey, pContNarration );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

void CDboTSCTCtrl::UpdateTSStep( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	// No containers currently in progress means starting
	if ( 0 == m_pCurTSP )
	{
		m_pCurTSP = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );
		if ( 0 == m_pCurTSP )
		{
			SetError();
			CNtlTSLog::Log( "Can not find the start container id. Info[%d]. [%s]", GetTrigger()->GetID(), TS_CODE_TRACE() );
			return;
		}
	}

	switch ( m_pCurTSP->GetEntityType() )
	{
	case DBO_CONT_TYPE_ID_CONT_GCOND:
		{
			if ( Cont_GCond( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_GACT:
		{
			if ( Cont_GAct( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_USERSEL:
		{
			if ( Cont_UserSel( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_REWARD:
		{
			if ( Cont_Reward( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_START:
		{
			if ( Cont_Start( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_END:
		{
			if ( Cont_End( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_NARRATION:
		{
			if ( Cont_Narration( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		{
			if ( Cont_Proposal( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_SWITCH:
		{
			if ( Cont_Switch( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
		{
			if ( Cont_UnifiedNarration( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	}
}

void CDboTSCTCtrl::ChangeTSState( unsigned int uiChangFlag )
{
	if ( eTS_PROG_STATE_PROGRESS_FAILED == uiChangFlag )
	{
		if ( IsFailed() )
		{
			// Send failed setting status to server
			UG_TS_Update_State( eTSSTATE_TYPE_ADD, eTS_SVR_STATE_FAILED );

			// If it is in an error state, it moves to a forced shutdown state in the case of a PC trigger.
			SetExitState();
		}
		else
		{
			UG_TS_Update_State( eTSSTATE_TYPE_REMOVE, eTS_SVR_STATE_FAILED );
		}
	}
	else if ( eTS_PROG_STATE_ERROR_BLOCK == uiChangFlag )
	{
		if ( IsError() )
		{
			// Send error setting status to server
			UG_TS_Update_State( eTSSTATE_TYPE_ADD, eTS_SVR_STATE_ERROR );

			// If it is in an error state, it moves to a forced shutdown state in the case of a PC trigger.
			SetExitState();
		}
		else
		{
			// Send error setting cancellation status to server
			UG_TS_Update_State( eTSSTATE_TYPE_REMOVE, eTS_SVR_STATE_ERROR );
		}
	}
	else if ( eTS_PROG_STATE_EXIT == uiChangFlag )
	{
		if ( IsExitState() )
		{
			// Notify client of trigger termination
			((CDboTSCTAgency*)GetParent())->TU_FinishQuest( TS_TYPE_PC_TRIGGER_CS, GetTrigger()->GetID() );
		}
	}
}

bool CDboTSCTCtrl::IfInterestingDoExceptionGroup( NTL_TS_TG_ID tgId ) const
{
	if ( NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN <= tgId && tgId <= NTL_TS_EXCEPT_CLIENT_GROUP_ID_END ) return true;
	return false;
}

void CDboTSCTCtrl::UG_Avatar_TS_Confirm_Step( NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData )
{
	SetCSComunication();

	((CDboTSCTAgency*)GetParent())->UG_Avatar_TS_Confirm_Step( GetTrigger()->GetID(), tcCurId, tcNextId, uiParam, byEventType, uiEventData );
}

void CDboTSCTCtrl::GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId )
{
	SetCSComunication( false );

	if (wResultCode >= GAME_TS_ERROR_RUN_ERROR && wResultCode <= GAME_TS_ERROR_TRIGGER_OBJECT_INVALID_FUNCFLAG)
	{
		SetError();
		CNtlTSLog::Log( "Trigger confirm step error. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );
		return;
	}
	else if (wResultCode >= GAME_TS_WARNING_NOT_SATISFY_REWARD_CONDITIONS && wResultCode <= GAME_TS_WARNING_NOT_YET_PROCESSED)
	{
		CNtlTSCont* pCurCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcCurId );

		if ( pCurCont->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GACT )
		{
			CDboTSContGAct* pCont = (CDboTSContGAct*)pCurCont;
			CNtlTSCont* pErrorCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( pCont->GetErrorLinkID() );

			if ( pErrorCont )
			{
				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( tcCurId, pErrorCont->GetID(), uiParam, GetEventType(), GetEventData() );
				return;
			}
			else
			{
				SetError();
				CNtlTSLog::Log( "Trigger confirm step error. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );
				return;
			}
		}
		else
		{
			return;
		}
	}
	else if ( RESULT_SUCCESS != wResultCode )
	{
		SetError();
		CNtlTSLog::Log( "Wrong result code. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );
		return;
	}

	CNtlTSCont* pCurCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcCurId );
	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcNextId );

	if ( 0 == pCurCont )
	{
		SetError();
		CNtlTSLog::Log( "Can not progress the trigger. Info[%d,%d]. [%s]", GetTrigger()->GetID(), pCurCont, TS_CODE_TRACE() );
		return;
	}

	// Currently running container
	sCTRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	NTL_TSRESULT tsResult = 0;

	switch ( pCurCont->GetEntityType() )
	{
	case DBO_CONT_TYPE_ID_CONT_GCOND:
		{
			// Since line inspection is performed, there is no need to run the trigger here.
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_GACT:
		{
			// When branching in an error state, no action is executed.
			if ( pNextCont->GetID() != ((CDboTSContGAct*)pCurCont)->GetErrorLinkID() )
			{
				tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, pCurCont->GetID(), GetParent()->GetRecv(), &sParam );
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_USERSEL:
		{
			// There is no entity to execute
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_REWARD:
		{
			// Since line inspection is performed, there is no need to run the trigger here.
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_START:
		{
			// Since line inspection is performed, there is no need to run the trigger here.
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_END:
		{
			// There is no entity to execute

			// Perform termination processing
			SetExitState();
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_NARRATION:
		{
			// There is no entity to execute
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		{
			// There is no entity to execute
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_SWITCH:
		{
			// There is no entity to execute
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
		{
			// There is no entity to execute
		}
		break;
	}

	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		SetError();
		CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), tsResult, TS_CODE_TRACE() );
		return;
	}

	// Move the TSP to the next container
	MoveTSP( pCurCont, pNextCont, false );
}

void CDboTSCTCtrl::GU_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	switch ( byType )
	{
	case eTSSTATE_TYPE_ADD:
		{
			if ( wTSState & eTS_SVR_STATE_ERROR ) SetError( true, false );
			if ( wTSState & eTS_SVR_STATE_FAILED ) SetFailed( true, false );
			if ( wTSState & eTS_SVR_STATE_ESCORT ) SetEscort( true, false );
			if ( wTSState & eTS_SVR_STATE_SVR_EVT_WORLD_RANGE ) { SetSvrEvtWorldRange( true, false ); }
			if ( wTSState & eTS_SVR_STATE_WPS_MODE ) { SetWPSMode( true, false ); }
			if ( wTSState & eTS_SVR_STATE_USER_OUT_FAILED_WPS ) { SetUserOutFailedWPS( true, false ); }
		}
		break;
	case eTSSTATE_TYPE_REMOVE:
		{
			if ( wTSState & eTS_SVR_STATE_ERROR ) SetError( false, false );
			if ( wTSState & eTS_SVR_STATE_FAILED ) SetFailed( false, false );
			if ( wTSState & eTS_SVR_STATE_ESCORT ) SetEscort( false, false );
			if ( wTSState & eTS_SVR_STATE_SVR_EVT_WORLD_RANGE ) { SetSvrEvtWorldRange( false, false ); }
			if ( wTSState & eTS_SVR_STATE_WPS_MODE ) { SetWPSMode( false, false ); }
			if ( wTSState & eTS_SVR_STATE_USER_OUT_FAILED_WPS ) { SetUserOutFailedWPS( false, false ); }
		}
		break;
	}

	// If it is in a failure or error state, it moves to a forced shutdown state in the case of a PC trigger.
	if ( IsFailed() || IsError() )
	{
		SetExitState();
	}
}

void CDboTSCTCtrl::UG_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	((CDboTSCTAgency*)GetParent())->UG_TS_Update_State( GetTrigger()->GetID(), byType, wTSState, uiParam );
}

void CDboTSCTCtrl::UT_ClickObject( unsigned int uiObjId )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_OBJECT );
	SetEventGenId( uiObjId );

	SetEventData( eEVENT_GEN_TYPE_CLICK_OBJECT, uiObjId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_CollisionObject( unsigned int uiObjId )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_COL_OBJECT );
	SetEventGenId( uiObjId );

	SetEventData( eEVENT_GEN_TYPE_COL_OBJECT, uiObjId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ClickNPC( unsigned int uiNPCId )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_NPC );
	SetEventGenId( uiNPCId );

	SetEventData( eEVENT_GEN_TYPE_CLICK_NPC, uiNPCId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ItemUse( unsigned int uiItemIdx, vecdef_ItemTargetList* pdefItemTargetList )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenItemTargetInfo( pdefItemTargetList );

	SetEventData( eEVENT_GEN_TYPE_ITEM_USE, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ItemGet( unsigned int uiItemIdx )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_GET );
	SetEventGenId( uiItemIdx );

	SetEventData( eEVENT_GEN_TYPE_ITEM_GET, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ItemEquip( unsigned int uiItemIdx )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_EQUIP );
	SetEventGenId( uiItemIdx );

	SetEventData( eEVENT_GEN_TYPE_ITEM_EQUIP, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ScoutUse( unsigned int uiItemIdx, SScouterUseTargetInfo* pTargetInfo )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SCOUT_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenScouterUseTargetInfo( pTargetInfo );

	SetEventData( eEVENT_GEN_TYPE_SCOUT_USE, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_RcvSvrEvt( unsigned int uiEvtID )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_RCV_SVR_EVT );
	SetEventGenId( uiEvtID );

	SetEventData( eEVENT_GEN_TYPE_RCV_SVR_EVT, uiEvtID );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_SkillUse( unsigned int uiSkillIdx, SSkillUseInfo* psSkillUseInfo )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SKILL_USE );
	SetEventGenId( uiSkillIdx );
	SetEventGenSkillUseInfo( psSkillUseInfo );

	SetEventData( eEVENT_GEN_TYPE_SKILL_USE, uiSkillIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ColRgn( sCOL_RGN_DATA* pColRgnData )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_COL_REGION );
	SetEventGenColRgnInfo( pColRgnData );

	SetEventData( eEVENT_GEN_TYPE_COL_REGION, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_RB( unsigned int uiWorldIdx, SRB_DATA* pRBData )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_RB );
	SetEventGenId( uiWorldIdx );
	SetEventGenRBData( pRBData );

	SetEventData( eEVENT_GEN_TYPE_RB, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ClickMob( unsigned int uiMobGroupID, SMobTargetInfo* pTargetInfo )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_MOB );
	SetEventGenId( uiMobGroupID );
	SetEventGenMobTargetData( pTargetInfo );

	SetEventData( eEVENT_GEN_TYPE_CLICK_MOB, pTargetInfo->hTarget );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_BindStone( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_BIND_STONE );

	SetEventData( eEVENT_GEN_TYPE_BIND_STONE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_SearchQuest( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SEARCH_QUEST );

	SetEventData( eEVENT_GEN_TYPE_SEARCH_QUEST, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ItemUpgrade( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_UPGRADE );

	SetEventData( eEVENT_GEN_TYPE_ITEM_UPGRADE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_Teleport( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_TELEPORT );

	SetEventData( eEVENT_GEN_TYPE_TELEPORT, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_Budokai( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_BUDOKAI );

	SetEventData( eEVENT_GEN_TYPE_BUDOKAI, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_SlotMachine( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SLOTMACHINE );

	SetEventData( eEVENT_GEN_TYPE_SLOTMACHINE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_HoipoiMix( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_HOIPOIMIX );

	SetEventData( eEVENT_GEN_TYPE_HOIPOIMIX, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_PrivateShop( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_PRIVATESHOP );

	SetEventData( eEVENT_GEN_TYPE_PRIVATESHOP, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_FreeBattle( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_FREEBATTLE );

	SetEventData( eEVENT_GEN_TYPE_FREEBATTLE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ItemIdentity( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEMIDENTITY );

	SetEventData( eEVENT_GEN_TYPE_ITEMIDENTITY, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_UseMail( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_USEMAIL );

	SetEventData( eEVENT_GEN_TYPE_USEMAIL, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_Party( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_PARTY );

	SetEventData( eEVENT_GEN_TYPE_PARTY, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_ClickSideIcon()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_CLICK_SIDEICON);

	SetEventData(eEVENT_GEN_TYPE_CLICK_SIDEICON, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_LevelCheck()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_LEVELCHECK);

	SetEventData(eEVENT_GEN_TYPE_LEVELCHECK, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_Quest()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_QUEST);

	SetEventData(eEVENT_GEN_TYPE_QUEST, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::UT_DialogOpen()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_DIALOGOPEN);

	SetEventData(eEVENT_GEN_TYPE_DIALOGOPEN, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCTCtrl::TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pContRwd )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetClientWait();

	((CDboTSCTAgency*)GetParent())->TU_ShowProposalDialog( sKey, pCont, pContRwd );
}

void CDboTSCTCtrl::UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_PROPOSAL )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bAccept ? ((CDboTSContProposal*) pCont)->GetOkLink() : ((CDboTSContProposal*) pCont)->GetCancelLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCTCtrl::TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSContReward* pCont )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetClientWait();

	((CDboTSCTAgency*)GetParent())->TU_ShowRewardDialog( sKey, this, pCont );
}

void CDboTSCTCtrl::UT_ShowRewardDialog( sTS_KEY& sKey, int nSelRwdIdx, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_REWARD )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	if ( bCancel )
	{
		CNtlTSCont* pCancCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContReward*)pCont)->GetCancelLinkID() );
		if ( pCancCont )
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( m_pCurTSP->GetID(), pCancCont->GetID(), uiParam, GetEventType(), GetEventData() );
		}
	}
	else
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContReward*)pCont)->GetNextLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, ((CDboTSContReward*)m_pCurTSP)->GetNextLinkID(), TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { nSelRwdIdx , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( m_pCurTSP->GetID(), pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
}

void CDboTSCTCtrl::TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSContUsrSel* pCont )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetClientWait();

	((CDboTSCTAgency*)GetParent())->TU_ShowUserSelectionDialog( sKey, this, pCont );
}

void CDboTSCTCtrl::UT_ShowUserSelectionDialog( sTS_KEY& sKey, NTL_TS_TC_ID tcSelId, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_USERSEL )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	if ( bCancel )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContUsrSel*)pCont)->GetCancelLink() );
		if ( NULL == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( sKey.tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
	else if ( ((CDboTSContUsrSel*)m_pCurTSP)->IsNextLink( tcSelId ) )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcSelId );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( sKey.tcID, tcSelId, uiParam, GetEventType(), GetEventData() );
	}
	else
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
		SetError();
	}
}

void CDboTSCTCtrl::TU_ShowNarrationDialog( sTS_KEY& sKey, CDboTSContNarration* pCont )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetClientWait();

	((CDboTSCTAgency*)GetParent())->TU_ShowNarrationDialog( sKey, pCont );
}

void CDboTSCTCtrl::UT_ShowNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_NARRATION )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bCancel ? ((CDboTSContNarration*) pCont)->GetCancelLink() : ((CDboTSContNarration*) pCont)->GetOkLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCTCtrl::TU_ShowUnifiedNarrationDialog( sTS_KEY& sKey, CDboTSContUnifiedNarration* pCont )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetClientWait();

	((CDboTSCTAgency*)GetParent())->TU_ShowUnifiedNarrationDialog( sKey, pCont );
}

void CDboTSCTCtrl::UT_ShowUnifiedNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bCancel ? ((CDboTSContUnifiedNarration*) pCont)->GetCancelLink() : ((CDboTSContUnifiedNarration*) pCont)->GetOkLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCTCtrl::TG_Avatar_TS_ExceptTimerStartNfy( sTS_KEY& sKey, CDboTSActETimerS* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	AttachExceptionTimer( sKey.tcID, sKey.taID, GetCurTime(), pAct->GetTime() );
}

void CDboTSCTCtrl::TG_Avatar_TS_ExceptTimerEndNfy( sTS_KEY& sKey, CDboTSActETimerE* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	DetachExceptionTimer( pAct->GetTCId(), pAct->GetTAId() );
}

void CDboTSCTCtrl::TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( true );

	((CDboTSCTAgency*)GetParent())->TU_ShowNPCConv( sKey, pAct );
}

void CDboTSCTCtrl::UT_ShowNPCConv( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( false );
}

void CDboTSCTCtrl::TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCTAgency*)GetParent())->TU_CameraDirection( sKey, pAct );
}

void CDboTSCTCtrl::UT_CameraDirection( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCTCtrl::TG_WaitTSNfy( sTS_KEY& sKey, CDboTSActTWaitTS* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	StartTimeWait( pAct->GetWaitTime() );
}

void CDboTSCTCtrl::TG_InSSMNfy( sTS_KEY& sKey, CDboTSActInSSM* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SetSSM( pAct->GetSSMId(), pAct->GetValue() );
}

void CDboTSCTCtrl::TG_TSStateNfy( sTS_KEY& sKey, CDboTSActTSState* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	switch ( pAct->GetType() )
	{
	case eTSSTATE_TYPE_ADD:
		if ( pAct->GetTSState() & eTS_SVR_STATE_FAILED ) SetFailed();
		break;
	case eTSSTATE_TYPE_REMOVE:
		if ( pAct->GetTSState() & eTS_SVR_STATE_FAILED ) SetFailed( false );
		break;
	}
}

void CDboTSCTCtrl::TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( true );

	((CDboTSCTAgency*)GetParent())->TU_ShowObjConv( sKey, pAct );
}

void CDboTSCTCtrl::UT_ShowObjConv( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( false );
}

void CDboTSCTCtrl::TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCTAgency*)GetParent())->TU_Hint( sKey, pAct );
}

void CDboTSCTCtrl::UT_Hint( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCTCtrl::TU_Open_Window( sTS_KEY& sKey, CDboTSActOpenWindow* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCTAgency*)GetParent())->TU_Open_Window( sKey, pAct );
}

void CDboTSCTCtrl::UT_Open_Window( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCTCtrl::TU_PC_ConvNfy( sTS_KEY& sKey, CDboTSActPCConv* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	((CDboTSCTAgency*)GetParent())->TU_PC_ConvNfy( sKey, pAct );
}


void CDboTSCTCtrl::TU_TelMudosa( sTS_KEY& sKey, CDboTSActTelMudosa* pAct )
{
	sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCTAgency*)GetParent())->TU_TelMudosa( sKey, pAct );
}

void CDboTSCTCtrl::UT_TelMudosa( sTS_KEY& sKey, RwUInt8 byMudosaIndex, RwBool bCancel )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	if ( !bCancel )
	{
		API_GetSLPacketGenerator()->SendBudokaiMudosaTeleport( byMudosaIndex );
	}
}
