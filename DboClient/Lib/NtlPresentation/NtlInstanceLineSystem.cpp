#include "precomp_ntlpresentation.h"
#include "NtlInstanceLineSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"
#include "NtlInstanceEffect.h"

CNtlInstanceLineSystem::CNtlInstanceLineSystem(void)
{
    Init();
}

CNtlInstanceLineSystem::~CNtlInstanceLineSystem(void)
{
    Delete();
}

void* CNtlInstanceLineSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE));
}

void CNtlInstanceLineSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE, pObj);    
}

void CNtlInstanceLineSystem::Init()
{
    CNtlInstanceImVertexSystem::Init();

    m_pResourceLineSystem = NULL;
    m_poolLineSystemVertex = NULL;
    m_nPoolIndex = 0;
    m_fCreateGapTime = 0.0f;
    m_nRoateSeed = 0;

    m_uiMemoryUseSize += sizeof(CNtlInstanceLineSystem);
}

void CNtlInstanceLineSystem::Delete()
{
    NTL_ARRAY_DELETE(m_poolLineSystemVertex);
	CNtlInstanceImVertexSystem::Delete();
}

void CNtlInstanceLineSystem::Reset()
{
    Delete();
}

RwBool CNtlInstanceLineSystem::Create( CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld )
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pResourceEffect || !pResourceComponentSystem)
        NTL_RETURN(FALSE);

    CNtlInstanceImVertexSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    m_pResourceLineSystem = (CNtlResourceLineSystem*)pResourceComponentSystem;
 
    // Vertex Buffer creation (maximum number of lines *6)
    m_nVertexCount = m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount * 6;    
    m_pVertices = NTL_NEW RwIm3DVertex[m_nVertexCount];
    NTL_ASSERTE(m_pVertices);
    if(!m_pVertices) NTL_RETURN(FALSE);
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * m_nVertexCount);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * m_nVertexCount;

    // Line Pool Creation
    m_poolLineSystemVertex = NTL_NEW SLineSystemVertex[m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount];
    NTL_ASSERTE(m_poolLineSystemVertex);
    if(!m_poolLineSystemVertex) NTL_RETURN(FALSE);
    ZeroMemory(m_poolLineSystemVertex, sizeof(SLineSystemVertex) * m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount);
    m_uiMemoryUseSize += sizeof(SLineSystemVertex) * m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount;
    

    // Texture settings
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceLineSystem->m_strTextureName);

    // Default Emitter Settings
    BuildEmitterStandard();

    m_nRoateSeed = rand();

    NTL_RETURN(TRUE);
}

void CNtlInstanceLineSystem::BuildEmitterStandard()
{
    // Color settings
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
    {
        RwRGBA color;

        RwRGBAFromRwRGBAReal(&color, &m_pResourceLineSystem->m_EmitterPrtCol.prtStartCol);        

        SetVertexColor(color);
    }
}

RwBool CNtlInstanceLineSystem::Update( RwReal fElapsedTime )
{
    CNtlInstanceImVertexSystem::Update(fElapsedTime);

    if(!m_pResourceLineSystem)
        return FALSE;

    m_fLifeTime += fElapsedTime;
    if(m_bStart)
    {
        if(m_pResourceLineSystem->m_fStartTime < m_fLifeTime)
        {
            m_bStart = FALSE;
            m_bReady = TRUE;
            m_bUpdate = TRUE;

            m_fLifeTime = fElapsedTime;
        }
        else
        {
            return TRUE;
        }
    }

    if(m_bUpdate && m_pResourceLineSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceLineSystem->m_fLifeTime)
    {
        Stop();                
    }

    if(m_bReady)
    {
        UpdateVertices(fElapsedTime);

        RwReal fDeltaTime = m_fLifeTime / m_pResourceLineSystem->m_fLifeTime;
        
        // Color Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
        {
            UpdateColor(fDeltaTime);
        }

        // Size Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
        {
            UpdateSize(fDeltaTime);
        }

        // Multi Texture Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
        {
            UpdateMultiTexture(fElapsedTime);
        }

        // Rotate Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
        {
            UpdateRotate(fDeltaTime);
        }
    }

    if((m_bFinish || !m_bUpdate) && m_listLineSystemVertex.size() == 0)
    {
        m_bReady = FALSE;
    }

    return m_bReady;
}

RwBool CNtlInstanceLineSystem::Render()
{
    if(!m_bReady || !m_bVisible || !m_pResourceLineSystem )
        return TRUE;

	if(m_listLineSystemVertex.empty())
		return TRUE;
    
    // Render vertices.
    if(m_pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable == FALSE)
    {
        RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    }
    
    BeginRenderState(m_pResourceLineSystem);
    
    if(RwIm3DTransform(m_pVertices, (RwInt32)m_listLineSystemVertex.size() * 6,  &m_matRender, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
        RwIm3DEnd();
    }
    
    EndRenderState(m_pResourceLineSystem);

    if(m_pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable == FALSE)
    {
        RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    }


    NTL_RETURN(TRUE);
}

void CNtlInstanceLineSystem::UpdateVertices( RwReal fElapsedTime )
{
    NTL_FUNCTION(__FUNCTION__);

    if(!m_pResourceLineSystem)
        NTL_RETURNVOID();

    m_fCreateGapTime += fElapsedTime;

    // If the gap time has passed, a new line is created.
    RwReal fEmitterGap = m_pResourceLineSystem->m_EmitterStandard.m_fEmitterGap;
    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        fEmitterGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    if(m_fCreateGapTime >= fEmitterGap && m_bUpdate && !m_bFinish)
    {
        for(int i = 0; i < m_pResourceLineSystem->m_EmitterStandard.m_nEmitterCount; ++i)
        {
            CreateLine();
        }
        
        m_fCreateGapTime = 0.0f;
    }

    int nCount = 0;
    ListLineSystemVertex::iterator it = m_listLineSystemVertex.begin();
    while(it != m_listLineSystemVertex.end())
    {
        SLineSystemVertex* pLineSystemVertex = (SLineSystemVertex*)(*it);
        if(pLineSystemVertex)
        {
            pLineSystemVertex->fLifeTime += fElapsedTime;

            if(pLineSystemVertex->fLifeTime < m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime)
            {
                // Move the end point in the set direction.
                pLineSystemVertex->vUpdatePoint += pLineSystemVertex->vDir * m_pResourceLineSystem->m_EmitterStandard.m_fVelocity * fElapsedTime;

                // If the line becomes longer than the specified length, the StartPoint also moves.
                if(m_pResourceLineSystem->m_EmitterStandard.m_bMoveLine)
                {
                    RwV3d vLenth = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;

                    if(RwV3dLength(&vLenth) >= pLineSystemVertex->vLineSize.y)
                    {
                        pLineSystemVertex->vStrartPoint += pLineSystemVertex->vDir * m_pResourceLineSystem->m_EmitterStandard.m_fVelocity * fElapsedTime;
                    }
                }

                // If the Shake option is on, it shakes randomly.
                if(m_pResourceLineSystem->m_EmitterStandard.m_bShake)
                {
                    UpdateShake(pLineSystemVertex);
                }

                // Create a Z-axis billboard surface using two specified points.
                if(m_pResourceLineSystem->m_EmitterStandard.m_bZBiilBoard)
                {
                    UpdateZBillBoard(pLineSystemVertex);
                }
                else
                {
                    UpdateNoneZBillBoard(pLineSystemVertex);
                }
                

                // Put the vertices in the list into the Vertex Buffer.
                for(int i = 0; i < 6; ++i)
                {
                    m_pVertices[nCount++] = pLineSystemVertex->imVertices[i];                    
                }

                ++it;
            }
            else    // When Life Time expires, it is removed from the list.
            {
                it = m_listLineSystemVertex.erase(it);

            }            
        }
    }
}

/**
 *Add a new line and add it to the List. 
 */
void CNtlInstanceLineSystem::CreateLine()
{
    if(m_listLineSystemVertex.size() >= (RwUInt32)m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount)
    {
        return;
    }

    if(m_nPoolIndex >= (RwInt32)m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount)
    {
        m_nPoolIndex = 0;
    }

    // Determine the location within the emitter.
    RwReal fRadian = NtlRandomNumber(0.0f, (2.0f * rwPI));        
    
    RwV3d vStartPoint;
    RwV3d vDir;
    
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
    {
        vStartPoint.x = (RwReal)RwCos(fRadian);
        vStartPoint.y = (RwReal)RwSin(fRadian);  
        vStartPoint.z = 0.0f;

        RwMatrix matRot;
        RwMatrixSetIdentity(&matRot);
        RwMatrixRotate(&matRot, &VEC_Y, NtlRandomNumber(0.0f, 360.0f), rwCOMBINEPOSTCONCAT);
        RwV3dTransformVector(&vStartPoint, &vStartPoint, &matRot);

        RwV3dNormalize(&vStartPoint, &vStartPoint);
        vDir = vStartPoint;        

        vStartPoint.x = vStartPoint.x * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.x;
        vStartPoint.y = vStartPoint.y * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.y;
        vStartPoint.z = vStartPoint.z * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.z;
        
    }
    else
    {
        vStartPoint.x = (RwReal)RwCos(fRadian);
        vStartPoint.y = (RwReal)RwSin(fRadian);    
        vStartPoint.z = 0.0f;    

        // Determines the direction of progress of the line.
        vDir = vStartPoint * m_pResourceLineSystem->m_EmitterStandard.m_fTargetRadius;    
        vDir.z = -1.0f;
        RwV3dNormalize(&vDir, &vDir);
    }

    m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint = vStartPoint * m_pResourceLineSystem->m_EmitterStandard.m_fEmitterRaduis;
    m_poolLineSystemVertex[m_nPoolIndex].vUpdatePoint = m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint;

    m_poolLineSystemVertex[m_nPoolIndex].vDir = vDir;    

    // Set Emitter Bias.
    if(m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias != 0.0f)
    {
        RwReal fEmitterBias = NtlRandomNumber(-(m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias), m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias);
        vDir = vDir * fEmitterBias;
        m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint += vDir;
    }

    // UV settings
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[0].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[0].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[1].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[1].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[2].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[2].v = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[3].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[3].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[4].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[4].v = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[5].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[5].v = 1.0f;

    // Color settings
    for(int i = 0; i < 6; ++i)
    {
        RwIm3DVertexSetRGBA(&m_poolLineSystemVertex[m_nPoolIndex].imVertices[i], m_sColor.red, m_sColor.green, m_sColor.blue, m_sColor.alpha);
    }

    // Size settings    
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
    {
        // When Size Action is set, the default Size is ignored. (Z value of Size Action is not used.)
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize.x = m_pResourceLineSystem->m_EmitterPrtSize.prtStartSize.x;        
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize.y = m_pResourceLineSystem->m_EmitterPrtSize.prtStartSize.y;
    }
    else
    {
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize = m_pResourceLineSystem->m_EmitterStandard.m_sizeLine;        
    }

    m_poolLineSystemVertex[m_nPoolIndex].fLifeTime = 0.0f;

    m_listLineSystemVertex.push_back(&m_poolLineSystemVertex[m_nPoolIndex]);

    ++m_nPoolIndex;
}

void CNtlInstanceLineSystem::UpdateZBillBoard( SLineSystemVertex* pLineSystemVertex )
{
    if(!pLineSystemVertex)
        return;

    const RwV3d* pViewPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

    RwV3d vAxisZ = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;
    if(RwV3dLength(&vAxisZ) == 0.0f)
        return;

    RwV3dNormalize(&vAxisZ, &vAxisZ);

    RwV3d vAxisStartX = *pViewPos - pLineSystemVertex->vStrartPoint;
    RwV3dNormalize(&vAxisStartX, &vAxisStartX);

    RwV3d vAxisEndX = *pViewPos - pLineSystemVertex->vUpdatePoint;
    RwV3dNormalize(&vAxisEndX, &vAxisEndX);

    RwV3d vAxisStartY;
    RwV3dCrossProduct(&vAxisStartY, &vAxisStartX, &vAxisZ);
    RwV3dNormalize(&vAxisStartY, &vAxisStartY);    

    RwV3d vAxisEndY;
    RwV3dCrossProduct(&vAxisEndY, &vAxisEndX, &vAxisZ);
    RwV3dNormalize(&vAxisEndY, &vAxisEndY);
    
    CreatePlane(pLineSystemVertex, vAxisStartY, vAxisEndY);
}

void CNtlInstanceLineSystem::UpdateNoneZBillBoard( SLineSystemVertex* pLineSystemVertex ) 
{
    // Creates a surface to which ZBillBoard is not applied.
    // The surface is stretched in the direction of the vector that is the cross product of the line's Dir vector and the vector between the line position and the origin.

    if(!pLineSystemVertex)
        return;

    // progress vector
    RwV3d vDir = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;
    if(RwV3dLength(&vDir) == 0.0f)
        return;

    RwV3dNormalize(&vDir, &vDir);

    // vector with origin
    RwV3d vLinePos = pLineSystemVertex->vUpdatePoint;
    vLinePos.z = m_pResourceLineSystem->m_vPosition.z;

    RwV3d vOrigin = vLinePos - m_pResourceLineSystem->m_vPosition;
    RwV3dNormalize(&vOrigin, &vOrigin);

    // The direction of the face is determined by cross producting the two vectors.
    RwV3d vAxisWidth;
    RwV3dCrossProduct(&vAxisWidth, &vDir, &vOrigin);
    RwV3dNormalize(&vAxisWidth, &vAxisWidth);

    CreatePlane(pLineSystemVertex, vAxisWidth, vAxisWidth);
}


/**
 *Create a plane with line information.
 * \param pLineSystemVertex Pointer to a structure containing information.
 * \param vStartAxis Vector of the plane to be constructed from the starting point (unit vector)
 * \param vEndAxis Vector of the side to be constructed from the endpoints (unit vector) 
 */
void CNtlInstanceLineSystem::CreatePlane(SLineSystemVertex* pLineSystemVertex, const RwV3d& vStartAxis, const RwV3d& vEndAxis)
{
    if(!pLineSystemVertex)
        return;
    
    RwV3d vStartWidth = vStartAxis * pLineSystemVertex->vLineSize.x * 0.5f;
    RwV3d vEndWidth = vEndAxis * pLineSystemVertex->vLineSize.x * 0.5f;

    pLineSystemVertex->imVertices[0].objVertex = pLineSystemVertex->vStrartPoint + vStartWidth;
    pLineSystemVertex->imVertices[1].objVertex = pLineSystemVertex->vUpdatePoint + vEndWidth;
    pLineSystemVertex->imVertices[2].objVertex = pLineSystemVertex->vUpdatePoint - vEndWidth;

    pLineSystemVertex->imVertices[3].objVertex = pLineSystemVertex->imVertices[0].objVertex;
    pLineSystemVertex->imVertices[4].objVertex = pLineSystemVertex->imVertices[2].objVertex;
    pLineSystemVertex->imVertices[5].objVertex = pLineSystemVertex->vStrartPoint - vStartWidth;
}

void CNtlInstanceLineSystem::UpdateShake( SLineSystemVertex* pLineSystemVertex )
{
    if(!pLineSystemVertex)
        return;

    RwReal fShakeRatio = NtlRandomNumber(0.0f, m_pResourceLineSystem->m_EmitterStandard.m_fShakeBias);

    RwV3d fShakeDir;
    fShakeDir.x = NtlRandomNumber(-1.0f, 1.0f);
    fShakeDir.y = NtlRandomNumber(-1.0f, 1.0f);
    fShakeDir.z = NtlRandomNumber(-1.0f, 1.0f);
    RwV3dNormalize(&fShakeDir, &fShakeDir);

    fShakeDir = fShakeDir * fShakeRatio;

    // Shake is applied only to the end line.
    pLineSystemVertex->vUpdatePoint = pLineSystemVertex->vUpdatePoint + fShakeDir;
}

void CNtlInstanceLineSystem::UpdateRotate( RwReal fDeltaTime ) 
{
    RwV3d vRotate;
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
    {
        RebuildMultiRotate(vRotate, m_nRoateSeed, fDeltaTime, &m_pResourceLineSystem->m_EmitterPrtRotate, &m_pResourceLineSystem->m_EmitterPrtMultiRotate);
    }
    else
    {
        RebuildRotate(vRotate, m_nRoateSeed, fDeltaTime, &m_pResourceLineSystem->m_EmitterPrtRotate);
    }

    RwMatrixSetIdentity(&m_matRotate);
    RwMatrixRotate(&m_matRotate, &VEC_X, vRotate.x, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate(&m_matRotate, &VEC_Y, vRotate.y, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate(&m_matRotate, &VEC_Z, vRotate.z, rwCOMBINEPOSTCONCAT);

    SetWorldMatrix(m_matWorld);
}

void CNtlInstanceLineSystem::UpdateColor( RwReal fDeltaTime ) 
{
    // Update the color for each line.
    RwRGBA color;

    for each(SLineSystemVertex* pLineSystemVertex in m_listLineSystemVertex)
    {
        RwReal fLineDeltaTime = pLineSystemVertex->fLifeTime / m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime;
        
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
        {
            RebuildMultiColor(color, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtCol, &m_pResourceLineSystem->m_EmitterPrtMultiCol);
        }
        else
        {
            RebuildColor(color, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtCol);            
        }

        for(int i = 0; i < 6; ++i)
        {
            RwIm3DVertexSetRGBA(&pLineSystemVertex->imVertices[i], color.red, color.green, color.blue, color.alpha);
        }
    }
}
void CNtlInstanceLineSystem::UpdateSize( RwReal fDeltaTime ) 
{
    // Update the size for each line.
    RwV3d size;

    for each(SLineSystemVertex* pLineSystemVertex in m_listLineSystemVertex)
    {
        RwReal fLineDeltaTime = pLineSystemVertex->fLifeTime / m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime;

        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
        {
            RebuildMultiSize(size, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtSize, &m_pResourceLineSystem->m_EmitterPrtMultiSize);
        }
        else
        {
            RebuildSize(size, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtSize);
        }

        pLineSystemVertex->vLineSize.x = size.x;
        pLineSystemVertex->vLineSize.y = size.y;
    }
}
