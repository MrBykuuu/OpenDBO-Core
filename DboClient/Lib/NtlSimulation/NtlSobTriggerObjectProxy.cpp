#include "precomp_ntlsimulation.h"
#include "NtlSobTriggerObjectProxy.h"

// Shared
#include "ObjectTable.h"
#include "NtlObject.h"

// Core
#include "NtlMath.h"

//// Presentation
#include "NtlPLEvent.h"
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlPLSceneManager.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSLLogicDef.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "DboTSCQAgency.h"
#include "NtlSLLogic.h"
#include "NtlShareTargetMark.h"

DEFINITION_MEMORY_POOL( CNtlSobTriggerObjectProxy )

CNtlSobTriggerObjectProxy::CNtlSobTriggerObjectProxy()
: FADE_RATIO( 255.f / 2.f )
{
	m_pPLObject		= NULL;
	m_pObjName		= NULL;	
	m_pTargetMark	= NULL;	
	m_pQuestMark	= NULL;	
	m_pDojoSealEff	= NULL;
    m_pShareTargetMark = NULL;

	m_fObjectWidth	= 1.0f;
	m_fObjectHeight	= 1.0f;
	m_fObjectDepth	= 1.0f;

	m_byMainState	= TOBJECT_STATE_NONE;
	m_bySubState	= 0;
	m_bPlay			= FALSE;
	m_fPlayTime		= FLT_MAX;

	m_sFadeInOutInfo.fCurAlpha = 255.0f;
	m_sFadeInOutInfo.fDestAlpha = 255.0f;
	m_sFadeInOutInfo.bShow = TRUE;

	m_fTLQTime		= 0.f;
	m_fPIDTime		= 0.f;
}

CNtlSobTriggerObjectProxy::~CNtlSobTriggerObjectProxy()
{
	DeletePLTargetMark();
	DeletePLObjectName();
	DeletePLQuestMark();
	DeleteDojoSealEffect();

    NTL_DELETE(m_pShareTargetMark);
}

void CNtlSobTriggerObjectProxy::CreatePLObjectName(void)
{
	DeletePLObjectName();

	CNtlSobTriggerObject* pSobTObj = dynamic_cast< CNtlSobTriggerObject* > ( m_pSobObj );
	if ( NULL == pSobTObj ) return;
	CNtlSobTriggerObjectAttr* pAttrTObj = dynamic_cast< CNtlSobTriggerObjectAttr* > ( pSobTObj->GetSobAttr() );
	if ( NULL == pAttrTObj ) return;

	const RwBBox *pBox = m_pPLObject->GetBoundingBox();
	if(pBox == NULL)
		return;

	CNtlSobTriggerObjectAttr *pTriggerObjectAttr = dynamic_cast<CNtlSobTriggerObjectAttr*>( m_pSobObj->GetSobAttr() );
	if ( !pTriggerObjectAttr->IsNameCreate() )
		return;

	const WCHAR* pName = pTriggerObjectAttr->GetName();

	m_pObjName = Helper_CreatePlayerName( NULL, NTL_OBJ_NAME_COLOR, pName );

	RwV3d vPos = m_pPLObject->GetPosition();
	vPos.y += (pBox->sup.y - pBox->inf.y)+NTL_NAMEBOX_OFFSET;

	m_pObjName->SetPosition(&vPos);
}

void CNtlSobTriggerObjectProxy::DeletePLObjectName(void)
{
	if(m_pObjName)
	{
		GetSceneManager()->DeleteEntity(m_pObjName); 
		m_pObjName = NULL;
	}
}

void CNtlSobTriggerObjectProxy::CreatePLTargetMark(void)
{
	DeletePLTargetMark();

	CNtlSobTriggerObject* pSobTObj = dynamic_cast< CNtlSobTriggerObject* > ( m_pSobObj );
	if ( NULL == pSobTObj ) return;
	CNtlSobTriggerObjectAttr* pAttrTObj = dynamic_cast< CNtlSobTriggerObjectAttr* > ( pSobTObj->GetSobAttr() );
	if ( NULL == pAttrTObj ) return;

    // If a shared target is set, the general target mark does not appear.
    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_TARGET);
        return;
    }

	if ( NULL == m_pTargetMark )
	{
        const RwBBox *pBox = m_pPLObject->GetBoundingBox();
        if(pBox == NULL)
            return;

        RwV3d vPos = m_pPLObject->GetPosition();
        vPos.y += (pBox->sup.y - pBox->inf.y) + NTL_NAMEBOX_OFFSET;

		m_pTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_TARGET);        	
		m_pTargetMark->SetPosition(&vPos);
	}
}

void CNtlSobTriggerObjectProxy::DeletePLTargetMark(void)
{
    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        m_pShareTargetMark->CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_NORMAL);
    }    
    else if(m_pTargetMark)
    {
        GetSceneManager()->DeleteEntity(m_pTargetMark);
        m_pTargetMark = NULL;
    }
	
}

void CNtlSobTriggerObjectProxy::CreatePLQuestMark(const RwChar *pKey)
{
	DeletePLQuestMark();

	CNtlSobTriggerObject* pSobTObj = dynamic_cast< CNtlSobTriggerObject* > ( m_pSobObj );
	if ( NULL == pSobTObj ) return;
	CNtlSobTriggerObjectAttr* pAttrTObj = dynamic_cast< CNtlSobTriggerObjectAttr* > ( pSobTObj->GetSobAttr() );
	if ( NULL == pAttrTObj ) return;

	const RwBBox *pBox = m_pPLObject->GetBoundingBox();
	if(pBox == NULL)
		return;

	RwV3d vPos = m_pPLObject->GetPosition();
	vPos.y += (pBox->sup.y - pBox->inf.y)+NTL_NAMEBOX_OFFSET;

	m_pQuestMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
	m_pQuestMark->SetPosition(&vPos);
}

void CNtlSobTriggerObjectProxy::DeletePLQuestMark(void)
{
	if(m_pQuestMark)
	{
		GetSceneManager()->DeleteEntity(m_pQuestMark);
		m_pQuestMark = NULL;
	}
}

void CNtlSobTriggerObjectProxy::CreateDojoSealEffect(RwUInt8 uiMainState)
{
	DeleteDojoSealEffect();

	std::string strEffectName;
	eDBO_TEAM eDBOTeam = Logic_GetScrambleTeam((eTOBJECT_STATE_TYPE_C)uiMainState);

	switch ( eDBOTeam )
	{
	case DBO_TEAM_NEUTRAILITY:
		{
			strEffectName = NTL_VID_DOJO_SEAL_NO_TEAM;
		}
		break;
	case DBO_TEAM_ENEMY:
		{
			strEffectName = NTL_VID_DOJO_SEAL_ATTACK_TEAM;
		}
		break;
	case DBO_TEAM_MY_TEAM:
		{
			strEffectName = NTL_VID_DOJO_SEAL_DEFENCE_TEAM;
		}
		break;
	}

	if ( !strEffectName.empty() )
	{
		RwV3d vPos = m_pPLObject->GetPosition();

		m_pDojoSealEff = GetSceneManager()->CreateEntity( PLENTITY_EFFECT, strEffectName.c_str() );
		m_pDojoSealEff->SetPosition( &vPos );
	}
}

void CNtlSobTriggerObjectProxy::DeleteDojoSealEffect(void)
{
	if(m_pDojoSealEff)
	{
		GetSceneManager()->DeleteEntity(m_pDojoSealEff);
		m_pDojoSealEff = NULL;
	}
}

RwBool CNtlSobTriggerObjectProxy::Create(RwUInt32 uiCompType)
{
	CNtlSobProxy::Create(uiCompType);

	return TRUE;
}

void CNtlSobTriggerObjectProxy::Destroy(VOID)
{
	CNtlSobProxy::Destroy();
}


void CNtlSobTriggerObjectProxy::Update(RwReal fElapsed)
{
	if(m_pObjName)
	{
		const RwBBox *pBox = m_pPLObject->GetBoundingBox();
		RwV3d vPos = m_pPLObject->GetPosition();
		vPos.y += (pBox->sup.y - pBox->inf.y)+NTL_NAMEBOX_OFFSET;
		
		m_pObjName->SetPosition(&vPos);
	}

	if(m_pPLObject->IsActiveUpdate() && m_bPlay)
	{
		m_bPlay = FALSE;
		PlayAnimation(m_fPlayTime);
	}

	UpdateFadeInOut( fElapsed );

	UpdateTLQObject( fElapsed );

	UpdatePIDObject( fElapsed );

    CNtlSobProxy::Update(fElapsed);
}

void CNtlSobTriggerObjectProxy::HandleEvents( RWS::CMsg &pMsg )
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SobCreateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobTargetSelect)
	{
		SobTargetSelectEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobAttackSelect)
	{
		SobAttackSelectEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobTargetSelectRelease)
	{
		SobTargetSelectReleaseEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobGotFocus)
	{
		m_pPLObject->SetAddColor( 40, 40, 40 );
	}
	else if(pMsg.Id == g_EventSobLostFocus)
	{
		m_pPLObject->SetAddColor( 0, 0, 0 );
	}
	else if(pMsg.Id == g_EventQuestMark)
	{
		SobQuestMark(pMsg);
	}
	else if(pMsg.Id == g_EventTObjectUpdateState)
	{
		SobUpdateState(pMsg);
	}
	else if(pMsg.Id == g_EventAnimEnd)
	{
		SobAnimEnd(pMsg);
	}
    else if(pMsg.Id == g_EventShareTargetSelect)
    {
        SobShareTargetSelectHandler(pMsg);
    }
    else if(pMsg.Id == g_EventShareTargetRelease)
    {
        SobShareTargetReleaseHandler(pMsg);
    }
}

void CNtlSobTriggerObjectProxy::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobTriggerObjectCreate *pSobCreate = reinterpret_cast<SNtlEventSobTriggerObjectCreate*>(pMsg.pData);
	m_pPLObject = reinterpret_cast<CNtlPLObject*>( pSobCreate->pPLEntity );

	m_pPLObject->SetSerialID(m_pSobObj->GetSerialID());

	// For trigger objects, check whether they are Visible and Fading In/Out.
	// Controlled externally -in the case of games
	RwUInt32 uiFlags = m_pPLObject->GetFlags();
	uiFlags |= NTL_PLEFLAG_OBJECT_FORCE_VISIBLE;
	uiFlags &= ~NTL_PLEFLAG_FADE;
	m_pPLObject->SetFlags( uiFlags );

	// width, height, depth
	const RwBBox *pBox = m_pPLObject->GetBoundingBox();
	if(pBox)
	{
		m_fObjectWidth	= pBox->sup.x - pBox->inf.x;
		m_fObjectHeight = pBox->sup.y - pBox->inf.y;
		m_fObjectDepth	= pBox->sup.z - pBox->inf.z;
	}

	CreatePLObjectName();

	// Process the initial state
	sOBJECT_TBLDAT* pObjTbl = ((CNtlSobTriggerObjectAttr*)m_pSobObj->GetSobAttr())->GetTriggerObjectTbl();
	InitState( pObjTbl->byDefMainState, pObjTbl->byDefSubState);
}

void CNtlSobTriggerObjectProxy::SobTargetSelectEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>( pMsg.pData );
	if(m_pSobObj->GetSerialID() == pSobTargetSelect->hSerialId)
	{
		CreatePLTargetMark();
	}
}

void CNtlSobTriggerObjectProxy::SobAttackSelectEventHandler(RWS::CMsg &pMsg)
{
	CreatePLTargetMark();
}

void CNtlSobTriggerObjectProxy::SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg)
{
	DeletePLTargetMark();
}

void CNtlSobTriggerObjectProxy::SobQuestMark(RWS::CMsg &pMsg)
{
	SNtlEventQuestMark *pQuestMark = reinterpret_cast<SNtlEventQuestMark*>( pMsg.pData );
	if(pQuestMark->byMarkType == EQuestMarkNew)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_NEW);
	}
	else if(pQuestMark->byMarkType == EQuestMarkProgress)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_PROGRESS);
	}
	else if(pQuestMark->byMarkType == EQuestMarkReward)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_REWARD);
	}
	else if(pQuestMark->byMarkType == EQuestMarkVisit)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_VISIT);
	}
	else
	{
		DeletePLQuestMark();
	}
}

void CNtlSobTriggerObjectProxy::SobUpdateState(RWS::CMsg &pMsg)
{
	// Perform updated state processing
	SNtlEventTObjectUpdateState *pSobTObjUpdateState = reinterpret_cast<SNtlEventTObjectUpdateState*>(pMsg.pData);
	UpdateState( pSobTObjUpdateState->byMainState, pSobTObjUpdateState->bySubState, pSobTObjUpdateState->uiStateTime );
}

void CNtlSobTriggerObjectProxy::SobAnimEnd(RWS::CMsg &pMsg)
{
	m_bPlay = TRUE;
	m_fPlayTime = 0.f;
}

void CNtlSobTriggerObjectProxy::SetPosition(const RwV3d *pPos)
{
	if(m_pPLObject)
	{
		m_pPLObject->SetPosition(pPos);
	}
}

void CNtlSobTriggerObjectProxy::SetDirection(const RwV3d *pDir)
{
	if(m_pPLObject)
	{
		RwReal fAngle = CNtlMath::LineToAngleY(pDir);
		m_pPLObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

RwV3d CNtlSobTriggerObjectProxy::GetDirection(void)
{
	if(m_pPLObject)
	{
		return m_pPLObject->GetDirection();
	}

	return CNtlSobProxy::GetDirection();
}

void CNtlSobTriggerObjectProxy::SetAngleY(RwReal fAngle)
{
	if(m_pPLObject)
	{
		m_pPLObject->SetRotate( 0.0f, fAngle, 0.0f );
	}
}

void CNtlSobTriggerObjectProxy::EnableVisible(RwBool bEnable)
{
	Fade(bEnable);
}

RwReal CNtlSobTriggerObjectProxy::GetPLEntityWidth(void)
{
	return m_fObjectWidth;
}


RwReal CNtlSobTriggerObjectProxy::GetPLEntityHeight(void)
{
	return m_fObjectHeight;
}


RwReal CNtlSobTriggerObjectProxy::GetPLEntityDepth(void)
{
	return m_fObjectDepth;
}

void CNtlSobTriggerObjectProxy::EnableNameVisible(RwBool bVisible)
{
	if ( m_pObjName )
	{
		m_pObjName->SetVisible(bVisible);
	}
}

void CNtlSobTriggerObjectProxy::SetNameColor(const WCHAR* pwcName, COLORREF color)
{
	if ( m_pObjName )
	{
		m_pObjName->SetNameColor(pwcName, color);
	}
}

RwBool CNtlSobTriggerObjectProxy::IsScouterObject(void)
{
	return ((CNtlSobTriggerObject*)m_pSobObj)->IsScouterObject();
}

RwBool CNtlSobTriggerObjectProxy::IsTLQObject(void)
{
	return ((CNtlSobTriggerObject*)m_pSobObj)->IsTLQObject();
}

RwBool CNtlSobTriggerObjectProxy::IsPIDObject(void)
{
	return ((CNtlSobTriggerObject*)m_pSobObj)->IsPIDObject();
}

RwBool CNtlSobTriggerObjectProxy::IsDojoSealObject(void)
{
	return ((CNtlSobTriggerObject*)m_pSobObj)->IsDojoSealObject();
}

void CNtlSobTriggerObjectProxy::InitState(RwUInt8 byMainState, RwUInt8 bySubState)
{
    if(byMainState >= DBO_MAX_OBJECT_STATE)
        return;

	// Main state process

	// If it is a PID object, the control for the Visible will be set to the current avatar's party
	// The state is determined by the type of the party instance.
	if ( IsPIDObject() )
	{
		m_byMainState = byMainState;

		UpdatePIDObject( PID_STAND_TIME + 1.f );
	}
	else
	{
		if ( m_byMainState != byMainState )
		{
			m_byMainState = byMainState;

			if ( IsDojoSealObject() )
			{
				CreateDojoSealEffect( m_byMainState );
			}

			m_AnimChain.clear();

			sOBJECT_TBLDAT* pObjTbl = ((CNtlSobTriggerObjectAttr*)m_pSobObj->GetSobAttr())->GetTriggerObjectTbl();

			RwInt32 i = 0;

			while ( i < DBO_MAX_OBJECT_STATE_ANIMATION && pObjTbl->abyState[byMainState][i] != 255 )
			{
				m_AnimChain.push_back( pObjTbl->abyState[byMainState][i] );
				++i;
			}

			if ( !m_AnimChain.empty() )
			{
				// In the initial state, only the animation at the end of the animation chain is output.
				while ( m_AnimChain.size() != 1 )
				{
					m_AnimChain.pop_front();
				}

				m_bPlay = TRUE;
			}
			else
			{
				DBO_WARNING_MESSAGE("Trigger object animation chain empty. " << m_pPLObject->GetName() << " m_byMainState: " << (int)m_byMainState);

				m_bPlay = FALSE;
			}

			m_fPlayTime = 0.f;
		}
	}

	// Sub state processing
	m_bySubState = bySubState;

	// Whether the object is Show /Hide

	// If the object is a Scouter object, the control on the Visible is invisible regardless of CS interaction
	// will start
	if ( IsScouterObject() )
	{
		EnableVisible( FALSE );
	}
	// In case of TLQ object, the control for Visible is not related to CS
	// Visibility is determined by quest progress
	else if ( IsTLQObject() )
	{
		UpdateTLQObject( TLQ_STAND_TIME + 1.f );
	}
	else
	{
		if( m_byMainState == TOBJECT_STATE_NONE )
		{
			EnableVisible( TRUE );
		}
		else
		{
			if((m_bySubState & (1<<TOBJECT_SUBSTATE_SHOW)) == (1<<TOBJECT_SUBSTATE_SHOW))
			{
				EnableVisible( TRUE );
			}
			else if((m_bySubState & (1<<TOBJECT_SUBSTATE_SHOW)) != (1<<TOBJECT_SUBSTATE_SHOW))
			{
				EnableVisible( FALSE );
			}
		}
	}

	if ( (m_bySubState & (1<<TOBJECT_SUBSTATE_UNSELECT)) == (1<<TOBJECT_SUBSTATE_UNSELECT) )
	{
		DeletePLTargetMark();
		m_pPLObject->SetAddColor( 0, 0, 0 );
	}
}

void CNtlSobTriggerObjectProxy::UpdateState(RwUInt8 byMainState, RwUInt8 bySubState, RwUInt32 uiStateTime)
{
    if(byMainState >= DBO_MAX_OBJECT_STATE)
        return;

	// Main state processing

	// In the case of a PID object, control over Visible is performed by the current avatar's party regardless of CS linkage status.
	// Status is determined by party instance type.
	if ( IsPIDObject() )
	{
		m_byMainState = byMainState;

		// Animation chain must remain initialized
	}
	else
	{
		// Update the main state only when the state changes.
		if ( m_byMainState != byMainState )
		{
			m_byMainState = byMainState;

			if ( IsDojoSealObject() )
			{
				CreateDojoSealEffect( m_byMainState );
			}

			m_AnimChain.clear();

			sOBJECT_TBLDAT* pObjTbl = ((CNtlSobTriggerObjectAttr*)m_pSobObj->GetSobAttr())->GetTriggerObjectTbl();

			RwReal fProgTime = uiStateTime / 1000.f;
			RwReal fPlayTime = FLT_MAX;
			RwInt32 i = 0;

			while ( i < DBO_MAX_OBJECT_STATE_ANIMATION && pObjTbl->abyState[byMainState][i] != 255 )
			{
				fProgTime -= m_pPLObject->GetAnimPlayTime( pObjTbl->abyState[byMainState][i] );

				// Animation is skipped according to the state progress time of the trigger object.

				if ( fProgTime < 0.f )
				{
					if ( FLT_MAX == fPlayTime )
					{
						// There seemed to be a case where events were skipped, so I decided to start from the beginning instead of using the comment code below.
						fPlayTime = 0.f;	// fProgTime + m_pPLObject->GetAnimPlayTime( pObjTbl->abyState[byMainState][i] );
					}

					m_AnimChain.push_back( pObjTbl->abyState[byMainState][i] );
				}

				++i;
			}

			// When the state progress time of the trigger object on the server is long for all animations
			// Print the last animation.
			if ( m_AnimChain.empty() && i > 0 )
			{
				fPlayTime = 0.f;
				m_AnimChain.push_back( pObjTbl->abyState[byMainState][i-1] );
			}

			if ( FLT_MAX == fPlayTime )
			{
				NTL_ASSERTFAIL( "Invalid play time." << m_pPLObject->GetName() << fPlayTime << "\n" );

				m_bPlay = FALSE;
				m_fPlayTime = 0.f;
			}
			else
			{
				m_bPlay = TRUE;
				m_fPlayTime = fPlayTime;
			}
		}
	}

	// Secondary state handling
	m_bySubState = bySubState;

	// Object Show/Hide

	// In the case of a Scouter object, control over Visible is handled regardless of the CS integration status.
	// In other words, Visibility is not synchronized with the state interconnected between CSs.
	if ( IsScouterObject() )
	{
		// do nothing
	}
	// In the case of a TLQ object, control over Visible is handled regardless of the CS linkage status.
	// In other words, Visibility is not synchronized with the state interconnected between CSs.
	else if ( IsTLQObject() )
	{
		// do nothing
	}
	else
	{
		if((m_bySubState & (1<<TOBJECT_SUBSTATE_SHOW)) == (1<<TOBJECT_SUBSTATE_SHOW))
		{
			EnableVisible( TRUE );
		}
		else if((m_bySubState & (1<<TOBJECT_SUBSTATE_SHOW)) != (1<<TOBJECT_SUBSTATE_SHOW))
		{
			EnableVisible( FALSE );
		}
	}

	if ( (m_bySubState & (1<<TOBJECT_SUBSTATE_UNSELECT)) == (1<<TOBJECT_SUBSTATE_UNSELECT) )
	{
		DeletePLTargetMark();
		m_pPLObject->SetAddColor( 0, 0, 0 );
	}
}

void CNtlSobTriggerObjectProxy::UpdateTLQObject(RwReal fElapsed)
{
	// Trigger objects are created before the avatar
	// Exception handling for avatars not yet created l
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if ( NULL == pAvatar )
	{
		return;
	}

	CDboTSCQAgency* pQAgency = pAvatar->GetQuestAgency();
	if ( NULL == pQAgency )
	{
		return;
	}

	if ( IsTLQObject() )
	{
		m_fTLQTime -= fElapsed;
		if ( m_fTLQTime > 0.f ) return;
		m_fTLQTime = TLQ_STAND_TIME;

		sOBJECT_TBLDAT* pObjTbl = ((CNtlSobTriggerObjectAttr*)((CNtlSobTriggerObject*)m_pSobObj)->GetSobAttr())->GetTriggerObjectTbl();

		bool bRet = false;

		if ( pObjTbl->minQuestId != NTL_TS_T_ID_INVALID &&
			 pObjTbl->maxQuestId != NTL_TS_T_ID_INVALID )
		{
			for ( NTL_TS_T_ID tID = pObjTbl->minQuestId; tID <= pObjTbl->maxQuestId; ++tID )
			{
				CDboTSTCtrl* pCtrl = pQAgency->FindProgressTrigger( tID );

				if ( pCtrl )
				{
					bRet |= (!pCtrl->IsError() && !pCtrl->IsFailed() ? true : false);
				}
			}
		}

		if ( bRet )
		{
			EnableVisible( TRUE );
		}
		else
		{
			EnableVisible( FALSE );
		}
	}
}

void CNtlSobTriggerObjectProxy::UpdatePIDObject(RwReal fElapsed)
{
	// In the case of trigger objects, they are created before the avatar.
	// Exception handling when the avatar has not been created yet
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if ( NULL == pAvatar )
	{
		return;
	}

	CNtlParty* pParty = pAvatar->GetParty();
	if ( NULL == pParty )
	{
		return;
	}

	// Construct a new animation chain only if the animation chain is empty.
	if ( IsPIDObject() && m_AnimChain.empty() )
	{
		RwUInt8 byDngState;
		if ( pParty->GetLeaderHandle() != INVALID_SERIAL_ID )
		{
			byDngState = (RwUInt8)pParty->GetPartyDungeonState();
		}
		else
		{
			byDngState = PARTY_DUNGEON_STATE_NORMAL;
		}

		sOBJECT_TBLDAT* pObjTbl = ((CNtlSobTriggerObjectAttr*)m_pSobObj->GetSobAttr())->GetTriggerObjectTbl();

		RwInt32 i = 0;

		while ( i < DBO_MAX_OBJECT_STATE_ANIMATION && pObjTbl->abyState[byDngState][i] != 255 )
		{
			m_AnimChain.push_back( pObjTbl->abyState[byDngState][i] );
			++i;
		}

		// In the initial state, only the animation at the end of the animation chain is output.
		if ( !m_AnimChain.empty() )
		{
			while ( m_AnimChain.size() != 1 )
			{
				m_AnimChain.pop_front();
			}

			m_bPlay = TRUE;
			m_fPlayTime = 0.f;
		}
		else
		{
			NTL_ASSERTFAIL( "Trigger object animation chain empty." << m_pPLObject->GetName() << byDngState << "\n" );

			m_bPlay = FALSE;
			m_fPlayTime = 0.f;
		}
	}
}

void CNtlSobTriggerObjectProxy::PlayAnimation(RwReal fPlayTime)
{
	if(!m_AnimChain.empty() && m_pPLObject)
	{
		RwInt32 iAniID = m_AnimChain.front();
		m_AnimChain.pop_front();

		// In the animation chain, only the last animation works as LOOP
		if ( !m_pPLObject->SetTriggerAnimation( iAniID, fPlayTime, m_AnimChain.empty() ? TRUE : FALSE ) )
		{
			//DBO_WARNING_MESSAGE( "Trigger object animation play failed " << m_pPLObject->GetName() << iAniID << fPlayTime << "\n" );
		}
	}
}

void CNtlSobTriggerObjectProxy::Fade(RwBool bFadeIn)
{
	if(bFadeIn)
	{
		m_sFadeInOutInfo.fDestAlpha = 255.0f;
	}
	else
	{
		m_sFadeInOutInfo.fDestAlpha = 0.0f;
	}
}

void CNtlSobTriggerObjectProxy::UpdateFadeInOut(RwReal fElapsed)
{
	// Determination of current Alpha value
	m_sFadeInOutInfo.fCurAlpha = CNtlMath::Range( m_sFadeInOutInfo.fCurAlpha, 0.0f, 255.0f );
	m_sFadeInOutInfo.fDestAlpha = CNtlMath::Range( m_sFadeInOutInfo.fDestAlpha, 0.0f, 255.0f );

	if ( (RwReal)m_sFadeInOutInfo.fDestAlpha != (RwReal)m_sFadeInOutInfo.fCurAlpha )
	{
		RwBool bFadeIn = ((RwReal)m_sFadeInOutInfo.fDestAlpha - (RwReal)m_sFadeInOutInfo.fCurAlpha > 0) ? TRUE : FALSE;

		// Alpha determination for child objects
		RwReal fAlpha = m_sFadeInOutInfo.fCurAlpha + ( bFadeIn ? FADE_RATIO * fElapsed : -1 * FADE_RATIO * fElapsed );
		m_sFadeInOutInfo.fCurAlpha = CNtlMath::Range( fAlpha, 0.0f, 255.0f );
	}

	// Visual decisions about child objects
	m_sFadeInOutInfo.bShow = (m_sFadeInOutInfo.fCurAlpha != 0.0f) ? TRUE : FALSE;

	// Alpha and Visual settings for child objects
	CNtlSobProxy::EnableVisible( m_sFadeInOutInfo.bShow );

	if( m_pPLObject )
	{
		m_pPLObject->SetVisible( m_sFadeInOutInfo.bShow );
		m_pPLObject->SetAlpha((RwUInt8)m_sFadeInOutInfo.fCurAlpha );
	}

	if( m_pObjName )
	{
		m_pObjName->SetVisible( m_sFadeInOutInfo.bShow );
		m_pObjName->SetAlpha((RwInt8)m_sFadeInOutInfo.fCurAlpha );
	}

	if( m_pTargetMark )
	{
		m_pTargetMark->SetVisible( m_sFadeInOutInfo.bShow );
	}

	if( m_pQuestMark )
	{
		m_pQuestMark->SetVisible( m_sFadeInOutInfo.bShow );
	}

	if ( m_pDojoSealEff )
	{
		m_pDojoSealEff->SetVisible( m_sFadeInOutInfo.bShow );
	}
}

void CNtlSobTriggerObjectProxy::SetAlpha( RwUInt8 byAlpha ) 
{
    CNtlSobProxy::SetAlpha(byAlpha);

    if(m_pPLObject)
    {
        m_pPLObject->SetAlpha(byAlpha);
    }
}

void CNtlSobTriggerObjectProxy::CreateShareTargetMark(RwUInt8 bySlot, RwInt32 type)
{
    if(!m_pShareTargetMark)
    {
        m_pShareTargetMark = NTL_NEW CNtlShareTargetMark(m_pSobObj);
    }

    m_pShareTargetMark->CreateShareTargetMark(bySlot, type);
}

void CNtlSobTriggerObjectProxy::DeleteShareTargetMark()
{
    if(!m_pShareTargetMark)
        return;

    m_pShareTargetMark->DeleteShareTargetMark();
}

void CNtlSobTriggerObjectProxy::SobShareTargetSelectHandler( RWS::CMsg& msg )
{
    SNtlEventShareTargetSelect* pData = (SNtlEventShareTargetSelect*)msg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        // Save the current state before deleting the existing target mark.        
        RwBool bTargetMode = m_pTargetMark ? TRUE : FALSE;

        DeletePLTargetMark();

        if(bTargetMode)
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_TARGET);
        }
        else
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_NORMAL);
        }        
    }
}

void CNtlSobTriggerObjectProxy::SobShareTargetReleaseHandler( RWS::CMsg& msg )
{
    SNtlEventShareTargetRelease* pData = (SNtlEventShareTargetRelease*)msg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        DeleteShareTargetMark();
    }
}