#include "precomp_ntlsimulation.h"
#include "DBODirectorIndicate.h"

#include "WorldSoundDefine.h"
#include "NtlPLCharacter.h"
#include "NtlInstanceEffect.h"
#include "NtlPLSceneManager.h"
#include "NtlSLEvent.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLEventFunc.h"

#define DESTORY_DIST 5.0f                                      ///< Effect extinction distance
#define DIRECTOR_FADE_TIME 1.0f

CDBODirectorIndicate::CDBODirectorIndicate(CNtlPLCharacter* pPLCharacter)
{
    m_pCharacter = pPLCharacter;        
    m_pDirectorEffect = NULL;
    m_pTargetEffect = NULL;    
    m_bUpdate = FALSE;

    ZeroMemory(&m_vTargetPos, sizeof(m_vTargetPos));
}

CDBODirectorIndicate::~CDBODirectorIndicate(void)
{
    Destory();
}

RwBool CDBODirectorIndicate::Create() 
{
    if(!m_pCharacter)
        return FALSE;

    return TRUE;
}

VOID CDBODirectorIndicate::Destory() 
{
    if(m_pDirectorEffect)
    {
        m_pDirectorEffect->Destroy();        
        m_pDirectorEffect = NULL;
    }

    if(m_pTargetEffect)
    {
        m_pTargetEffect->Destroy();        
        m_pTargetEffect = NULL;
    }

    m_bUpdate = FALSE;
}

RwBool CDBODirectorIndicate::Update(RwReal fElapsedTime)
{
    if(!m_pCharacter || !m_pDirectorEffect || !m_bUpdate)
        return FALSE;

    UpdateDirection();
    CalcDistance();    

    return TRUE;
}

VOID CDBODirectorIndicate::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventDirectionNfy)
    {
        SNtlEventDirectionNfy* pData = (SNtlEventDirectionNfy*)pMsg.pData;

        if(pData->bCreate)
        {
            if(m_pTargetEffect)
            {
                CNtlSLEventGenerator::SLDirection_Nfy(FALSE, m_vTargetPos);
            }

            CreateDirector();    
            CreateTargetEffect();
            m_vTargetPos = pData->vLoc;
            m_vTargetPos.y = 0.0f;
            SetTargetEffectPos(m_vTargetPos);
            m_bUpdate = TRUE;            
        }
        else    //Effect delete event
        {
            Destory();
        }
    }   
    else if(pMsg.Id == g_EventTMQCleintState)
    {
        SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( pMsg.pData );

        if( pEvent->uiState == WORLD_STATE_TMQ_CLOSE || pEvent->uiState == WORLD_STATE_TMQ_FAIL)
            Destory();
    }
    else if(pMsg.Id == g_EventWorldChange)
    {
        SNtlEventWorldChange* pWorldChange = reinterpret_cast<SNtlEventWorldChange*>(pMsg.pData);

        if(pWorldChange->bWorldChange)      
        {
            Destory();
        }
    }
}

// Creates a direction indication effect.
RwBool CDBODirectorIndicate::CreateDirector()
{
    if(!m_pCharacter)
        return FALSE;

    if(!m_pDirectorEffect)
    {
        m_pDirectorEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_DIRECTOR_EFFECT);        
    }

    return TRUE;
}

RwBool CDBODirectorIndicate::CreateTargetEffect()
{
    if(!m_pTargetEffect)
    {
        m_pTargetEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_DIRECTOR_MARK);
    }

    return TRUE;
}

//Delete the effect when the distance to the target becomes closer than the specified distance.
RwBool CDBODirectorIndicate::CalcDistance() 
{
    if(!m_pCharacter)
        return FALSE;

        // NOTE: The target's position does not have a Y value. (it is not sent in the event) (이벤트에서 안날려준다)
    RwV3d vCharPos = m_pCharacter->GetPosition();
    vCharPos.y = 0.0f;
    RwV3d vDistance = m_vTargetPos - vCharPos;
    RwReal fDist = CNtlMath::MathRwV3dSquaredLength(&vDistance);
    if(fDist < DESTORY_DIST * DESTORY_DIST)
    {
        // A delete event is sent to remove the minimap display.
        CNtlSLEventGenerator::SLDirection_Nfy(FALSE, m_vTargetPos);

		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
		tSoundParam.pcFileName		= WORLD_SOUND_CHECKPOINT;
		tSoundParam.fXPos			= m_vTargetPos.x;
		tSoundParam.fYPos			= m_vTargetPos.y;
		tSoundParam.fZPos			= m_vTargetPos.z;

		GetSoundManager()->Play(&tSoundParam);
    }

    return TRUE;
}

VOID CDBODirectorIndicate::UpdateDirection() 
{
    if(!m_pCharacter)
        return;

    RwV3d vTargetDir = m_vTargetPos - m_pCharacter->GetPosition();
    RwV3dNormalize(&vTargetDir, &vTargetDir);
    RwV3d vRight;
    RwV3dCrossProduct(&vRight, &vTargetDir, &YAxis);

    RwMatrix matDir;
    RwMatrixSetIdentity(&matDir);    
    *RwMatrixGetRight(&matDir) = vRight;
    *RwMatrixGetUp(&matDir) = YAxis;
    *RwMatrixGetAt(&matDir) = vTargetDir;
    *RwMatrixGetPos(&matDir) = m_pCharacter->GetPosition();
    RwMatrixUpdate(&matDir);
    
    m_pDirectorEffect->SetMatrix(matDir);    
}

VOID CDBODirectorIndicate::SetTargetEffectPos(RwV3d vPos ) 
{
    if(!m_pTargetEffect)
        return;

    // The coordinates coming down from the server have no y-coordinate. The client calculates them directly.
    RwReal fHeight = 0.0f;
    RwV3d  vNormal = ZeroAxis;
    GetSceneManager()->GetWorldHeight(&vPos, fHeight, &vNormal);
    vPos.y = fHeight;

    m_pTargetEffect->SetPosition(&vPos);    
}