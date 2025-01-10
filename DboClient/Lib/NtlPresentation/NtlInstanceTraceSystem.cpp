#include "precomp_ntlpresentation.h"
#include "NtlInstanceTraceSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"
#include "D3Dx9math.h"
#include "NtlPLCharacter.h"
#include "NtlInstanceEffect.h"

void* CNtlInstanceTraceSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);
    

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE));
}

void CNtlInstanceTraceSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE, pObj);
}

CNtlInstanceTraceSystem::CNtlInstanceTraceSystem(void)
{
    Init();
}

CNtlInstanceTraceSystem::~CNtlInstanceTraceSystem(void)
{
    Delete();
}

void CNtlInstanceTraceSystem::Init()
{
    CNtlInstanceImVertexSystem::Init();

    m_pEventTrace           = NULL;    
    m_pAttach               = NULL;    
    m_nPoolIndex            = 0;
    m_nControlPointIndex    = 0;
    m_poolTraceEdge         = NULL;    
    m_poolControlPoint      = NULL;
    m_fEdgeGapTemp          = 10.0f;                ///< Set a large value to unconditionally generate the first frame.    
    m_bUpdate               = TRUE;
    m_uiMemoryUseSize       = sizeof(CNtlInstanceTraceSystem);
}

void CNtlInstanceTraceSystem::Reset()
{
    Delete();
}

void CNtlInstanceTraceSystem::Delete()
{
    NTL_ARRAY_DELETE(m_poolTraceEdge);
    NTL_ARRAY_DELETE(m_poolControlPoint);    
    NTL_DELETE(m_pEventTrace);

    CNtlInstanceImVertexSystem::Delete();

}

/**
 *Create a trajectory effect.
 * \param pEventTrace Event object containing information about trajectory effects
 * \param pCharacter Pointer to the character to which the trajectory effect will be applied.
 *Whether return is successful or not
 */
RwBool CNtlInstanceTraceSystem::Create(SEventTrace* pEventTrace, CNtlPLAttach* pAttach)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pEventTrace || !pAttach)
        NTL_RETURN(FALSE);

    m_pEventTrace = NTL_NEW SEventTrace();
    memcpy(m_pEventTrace, pEventTrace, sizeof(SEventTrace));
    m_uiMemoryUseSize += sizeof(SEventTrace);
    
    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        m_pEventTrace->fEdgeGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    // Create Vertex Buffer    
    m_pVertices = NTL_NEW RwIm3DVertex[pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount];    
    if(!m_pVertices) NTL_RETURN(FALSE);
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount));
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount);
    
    // Create a pool.
    m_poolTraceEdge = NTL_NEW STraceEdge[pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount];
    ZeroMemory(m_poolTraceEdge, sizeof(STraceEdge) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount));
    m_uiMemoryUseSize += sizeof(STraceEdge) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount);
    
    m_poolControlPoint = NTL_NEW STraceEdge[pEventTrace->nMaxEdgeCount];
    ZeroMemory(m_poolControlPoint, sizeof(STraceEdge) * pEventTrace->nMaxEdgeCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEventTrace->nMaxEdgeCount;

    // Texture settings
    std::string strTextureName = pEventTrace->strTexture;
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(strTextureName);

    // Setting the location pointer of the bone
    SetEdgePoint(pAttach);

    m_pAttach = pAttach;    

    NTL_RETURN(TRUE);
}

/**
 *Set the positions by taking them from two bones.
 * \param pAttach Bone's Entity object to be retrieved (Character Or Item)
 *return 
 */
void CNtlInstanceTraceSystem::SetEdgePoint(CNtlPLAttach* pAttach)
{
    if(!pAttach)
        return;

    if(strlen(m_pEventTrace->strStartBoneName) == 0 ||
        strlen(m_pEventTrace->strEndBoneName) == 0)
        return;

    
    RwMatrix *pMatStartBone, *pMatEndBone;

    if(pAttach->GetClassType() == PLENTITY_CHARACTER)
    {
        CNtlPLCharacter* pCharacter = (CNtlPLCharacter*)pAttach;

        pMatStartBone = pCharacter->GetBoneMatrix(m_pEventTrace->strStartBoneName);
        pMatEndBone = pCharacter->GetBoneMatrix(m_pEventTrace->strEndBoneName);
    }
    else if(pAttach->GetClassType() == PLENTITY_ITEM)
    {
        CNtlPLItem* pItem = (CNtlPLItem*)pAttach;

        pMatStartBone = pItem->GetBoneMatrix(m_pEventTrace->strStartBoneName);
        pMatEndBone = pItem->GetBoneMatrix(m_pEventTrace->strEndBoneName);
    }

    if(!pMatStartBone || !pMatEndBone)
        return;

    m_vStartPoint = *RwMatrixGetPos(pMatStartBone);
    m_vEndPoint = *RwMatrixGetPos(pMatEndBone);    

    // Applies if Offset is set.
    if(RwV3dLength(&(m_pEventTrace->v3dStartBoneOffset)) > 0.0f ||
       RwV3dLength(&(m_pEventTrace->v3dEndBoneOffset)) > 0.0f)
    {
        // Get only the rotation value from the Matrix, apply it to Offset, and add it to Point.
        RwMatrix matStartOffset = *pMatStartBone;
        RwMatrix matEndOffset = *pMatEndBone;   
        matStartOffset.pos.x = matStartOffset.pos.y = matStartOffset.pos.z = 0.0f;
        matEndOffset.pos.x = matEndOffset.pos.y = matEndOffset.pos.z = 0.0f;

        RwV3d v3dStartOffset, v3dEndOffset;
        RwV3dTransformPoint(&v3dStartOffset, &(m_pEventTrace->v3dStartBoneOffset), &matStartOffset);
        RwV3dTransformPoint(&v3dEndOffset, &(m_pEventTrace->v3dEndBoneOffset), &matEndOffset);

        m_vStartPoint = m_vStartPoint + v3dStartOffset;
        m_vEndPoint   = m_vEndPoint + v3dEndOffset;
    }
}

RwBool CNtlInstanceTraceSystem::Update(RwReal fElapsedTime)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!m_pEventTrace)
        NTL_RETURN(FALSE);

    m_fLifeTime += fElapsedTime;    

    if(m_fLifeTime >= m_pEventTrace->fLifeTime)
    {
        Stop();
    }

    if(!UpdateVertices(fElapsedTime))
    {
        NTL_RETURN(FALSE);
    }
    
    NTL_RETURN(TRUE);    
}

RwBool CNtlInstanceTraceSystem::Render()
{
    if(!m_pEventTrace || !m_bVisible || m_listTraceEdge.empty())
        return TRUE;

    // Render the vertices in the list.
    RwD3D9SetTexture(m_pCurrentTexture, 0);

    BeginEffectTraceSystem(TRUE, m_pEventTrace->eSrcBlend, m_pEventTrace->eDestBlend);    

    if(RwIm3DTransform(m_pVertices, m_listTraceEdge.size() * 2, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }

    EndEffectTraceSystem(TRUE);

    return TRUE;
}

RwBool CNtlInstanceTraceSystem::UpdateVertices(RwReal fElapsedTime)
{
    // Add a new edge.
    if(m_fEdgeGapTemp >= m_pEventTrace->fEdgeGap &&
       m_bUpdate)
    {
        CreateEdge();
        m_fEdgeGapTemp = 0.0f;
    }
    else
    {
        m_fEdgeGapTemp += fElapsedTime;
    }

    // Udpate the UV.
    UpdateUV();

    // Update the color.
    UpdateColor();
    
    // Updates the LifeTime of the vertices in the list.    
    int nCount = 0;
    ListTraceEdge::iterator it = m_listTraceEdge.begin();
    while(it != m_listTraceEdge.end())
    {
        (*it)->fLifeTime += fElapsedTime;

        if((*it)->fLifeTime >= m_pEventTrace->fEdgeLifeTime)
        {
            // When the life time expires, it is removed from the list.
            it = m_listTraceEdge.erase(it);
        }
        else
        {
            m_pVertices[nCount++] = (*it)->vVertices[0];
            m_pVertices[nCount++] = (*it)->vVertices[1];            

            ++it;
        }
    }

    if(!m_bUpdate && m_listTraceEdge.size() == 0)
    {
        return FALSE;
    }    

    return TRUE;
}

void CNtlInstanceTraceSystem::CreateEdge() 
{
    // Adds a new edge to the end of the list.    

    // Calculate whether the maximum number of edges has been exceeded.
    if(m_nControlPointIndex >= m_pEventTrace->nMaxEdgeCount)
    {
        // When the index reaches the end of the pool, it returns to the beginning.
        m_nControlPointIndex = 0;        
                
        m_listControlPoint.erase(m_listControlPoint.begin());
    }

    // Calculate whether the maximum length has been exceeded.
    if(m_listTraceEdge.size() > 2)
    {
        RwReal fLength = 0.0f;               

        ListTraceEdge::iterator it = m_listTraceEdge.begin();
        RwV3d  v3dPrevPos = (*it)->vVertices[0].objVertex;
        ++it;

        while(it != m_listTraceEdge.end())
        {            
            RwV3d vLength = (*it)->vVertices[0].objVertex - v3dPrevPos;
            fLength += RwV3dLength(&vLength);

            v3dPrevPos = (*it)->vVertices[0].objVertex;
            ++it;
        }

        if(fLength >= m_pEventTrace->fMaxLength)
        {
            m_listTraceEdge.erase(m_listTraceEdge.begin());            
        }
    }

    // Set the positions of two vertices.
    SetEdgePoint(m_pAttach);

    // Add a new control point to the List.
    m_poolControlPoint[m_nControlPointIndex].vVertices[0].objVertex = m_vStartPoint;
    m_poolControlPoint[m_nControlPointIndex].vVertices[1].objVertex = m_vEndPoint;
    
    m_listControlPoint.push_back(&m_poolControlPoint[m_nControlPointIndex]);
    ++m_nControlPointIndex;

    // Create a Spline Curve Point and add it to the list.
    CreateSplinePath();   
}

/**
 *Create a Spline Path Point and add it to the vertex buffer.
 * \param nIndex Index of the list that created the spline (Creates a Spline Path with four vertices: nIndex, nIndex-1, nIndex-2, and nIndex-3.
 *return 
 */
void CNtlInstanceTraceSystem::CreateSplinePath()
{
    // Create a catmull-rom spline.
    // Use functions in DX.

    if(m_listControlPoint.size() < 4 || m_pEventTrace->nSplinePointCount < 2)
    {
        // The condition for creating a spline is that there must be at least 4 vertices.

        ListTraceEdge::reverse_iterator it = m_listControlPoint.rbegin();
        STraceEdge* pEdge = *it;

        m_poolTraceEdge[m_nPoolIndex].fLifeTime = 0.0f;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex = pEdge->vVertices[0].objVertex;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex = pEdge->vVertices[1].objVertex;

        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[0], m_pEventTrace->colStartColor.red,
                                                                         m_pEventTrace->colStartColor.green,
                                                                         m_pEventTrace->colStartColor.blue,
                                                                         m_pEventTrace->colStartColor.alpha);
        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[1], m_pEventTrace->colEndColor.red,
                                                                         m_pEventTrace->colEndColor.green,
                                                                         m_pEventTrace->colEndColor.blue,
                                                                         m_pEventTrace->colEndColor.alpha);

        ++m_nPoolIndex;

        return;
    }

    D3DXVECTOR3 vStartOut, vEndOut, vStart[4], vEnd[4];
    RwReal fWeight;

    ListTraceEdge::reverse_iterator it = m_listControlPoint.rbegin();
    for(int i = 3; i >= 0 ; --i)
    {
        STraceEdge* pEdge = *it;

        vStart[i].x = pEdge->vVertices[0].objVertex.x;
        vStart[i].y = pEdge->vVertices[0].objVertex.y;
        vStart[i].z = pEdge->vVertices[0].objVertex.z;

        vEnd[i].x = pEdge->vVertices[1].objVertex.x;
        vEnd[i].y = pEdge->vVertices[1].objVertex.y;
        vEnd[i].z = pEdge->vVertices[1].objVertex.z;

        ++it;
    }

    for(int j = 0; j < m_pEventTrace->nSplinePointCount; ++j)
    {
        fWeight = (RwReal)j / (m_pEventTrace->nSplinePointCount - 1) ;

        D3DXVec3CatmullRom(&vStartOut, &vStart[0], &vStart[1], &vStart[2], &vStart[3], fWeight);
        D3DXVec3CatmullRom(&vEndOut, &vEnd[0], &vEnd[1], &vEnd[2], &vEnd[3],fWeight);

        if(m_nPoolIndex >= m_pEventTrace->nMaxEdgeCount)
        {
            m_nPoolIndex = 0;            
        }

        if(m_listTraceEdge.size() >= (RwUInt32)m_pEventTrace->nMaxEdgeCount)
        {
            m_listTraceEdge.erase(m_listTraceEdge.begin());
        }

        m_poolTraceEdge[m_nPoolIndex].fLifeTime = 0.0f;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.x = vStartOut.x;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.y = vStartOut.y;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.z = vStartOut.z;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.x = vEndOut.x;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.y = vEndOut.y;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.z = vEndOut.z;

        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[0], m_pEventTrace->colStartColor.red,
                                                                        m_pEventTrace->colStartColor.green,
                                                                        m_pEventTrace->colStartColor.blue,
                                                                        m_pEventTrace->colStartColor.alpha);
        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[1], m_pEventTrace->colEndColor.red,
                                                                        m_pEventTrace->colEndColor.green,
                                                                        m_pEventTrace->colEndColor.blue,
                                                                        m_pEventTrace->colEndColor.alpha);
                                                                        
        m_listTraceEdge.push_back(&m_poolTraceEdge[m_nPoolIndex]);
        ++m_nPoolIndex;        
    }    
}

void CNtlInstanceTraceSystem::UpdateUV() 
{
    RwUInt32 uiCount = 0;
    RwUInt32 uiListSize = m_listTraceEdge.size();
    
    for each(STraceEdge* pTraceEdge in m_listTraceEdge)
    {
        if(!pTraceEdge)
            continue;

        RwReal fUPos = (RwReal)uiCount / (RwReal)uiListSize;

        pTraceEdge->vVertices[0].u = fUPos;
        pTraceEdge->vVertices[0].v = 0.0f;

        pTraceEdge->vVertices[1].u = fUPos;
        pTraceEdge->vVertices[1].v = 1.0f;        

        ++uiCount;
    }
}

void CNtlInstanceTraceSystem::UpdateColor()
{
    if(m_pEventTrace->colStartColor.red == m_pEventTrace->colEndColor.red &&
       m_pEventTrace->colStartColor.green == m_pEventTrace->colEndColor.green &&
       m_pEventTrace->colStartColor.blue == m_pEventTrace->colEndColor.blue &&
       m_pEventTrace->colStartColor.alpha == m_pEventTrace->colEndColor.alpha)
        return;

    RwRGBA color;
    RwReal fDeltaTime = 0.0f;

    for each(STraceEdge* pTraceEdge in m_listTraceEdge)
    {
        if(!pTraceEdge)
            continue;

        fDeltaTime = pTraceEdge->fLifeTime / m_pEventTrace->fEdgeLifeTime;
        if(fDeltaTime > 1.0f)
        {
            fDeltaTime = 1.0f;
        }

        color = CNtlMath::Interpolation(m_pEventTrace->colStartColor, m_pEventTrace->colEndColor, fDeltaTime);        
        RwIm3DVertexSetRGBA(&pTraceEdge->vVertices[0], color.red, color.green, color.blue, color.alpha);
        RwIm3DVertexSetRGBA(&pTraceEdge->vVertices[1], color.red, color.green, color.blue, color.alpha);
    }
}

