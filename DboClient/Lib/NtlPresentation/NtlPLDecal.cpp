#include "precomp_ntlpresentation.h"
#include "NtlPLDecal.h"

// Core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlAtomic.h"

// Presentation
#include "NtlPLResourceManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLVisualManager.h"
#include "NtlPLRenderState.h"
#include "NtlDecalManager.h"
#include "NtlEffectSystemFreeList.h"
#include "GraphicDataTable.h"
#include "NtlDNController.h"
#include "NtlPLWorldState.h"


#define DECAL_VB_SMALL_SIZE     1024
#define DECAL_VB_BIG_SIZE       5120

RpIntersection	g_InterZone;

const RwReal CNtlPLDecal::m_cfFadeTime = 0.2f;

void* CNtlPLDecal::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLDecal::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_DECAL));
}

void CNtlPLDecal::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_DECAL, pObj);
}

CNtlPLDecal::CNtlPLDecal()
{
	m_vPos = ZeroAxis;
	m_vOriginPos = ZeroAxis;
	m_vSize = ZeroAxis;
	m_fIntersectionRadius = 3.0f;
	m_fScale = 1.0f;
	m_fVisibleSquaredDist = 1600.f;	//Distance visible from camera (40m)
	m_pTexture = NULL;
	m_pCurrentTexture = NULL;
	m_ppMultiTextures = NULL;
	m_f2DRoateAngle = 0.0f;
	m_bVertexUpdate = TRUE;
	m_fRadiusWorldVertex = 0.0f;
	m_fYOffset = 0.01f;
	m_fDegree = 0.0f;
	m_uiFadeAlpha = 0;
	m_fFadeGap = 0.0f;
	
	m_color.red = m_color.green = m_color.blue = m_color.alpha = 255;
	m_colStart.red = m_colStart.green = m_colStart.blue = m_colStart.alpha = 255;
	m_colEnd.red = m_colEnd.green = m_colEnd.blue = m_colEnd.alpha = 255;
	m_fColorActionTime = 0.0f;

	m_fLifeTime = 10.0f;			
	m_fTotalElapsedTime = 0.0f;
	m_fElapsedTime = 0.0f;
	m_fStartScale = 0.0f;
	m_fEndScale = 0.0f;
	m_fScaleActionTimeGap = 0.0f;
	m_fMultiTextureDeltaTime = 0.0f;
	m_fMultiColorDeltaTime = 0.0f;
	m_nCurrentTexture = 0;
	m_nNextColorIndex = 0;
	m_nNextSizeIndex = 0;
	m_fMultiSizeDeltaTime = 0.0f;
	m_fPrevSize = 0.0f;
	m_fOrgRotateAngle = 0.0f;
	m_fRotateTimeGap = 0.0f;
    m_nMaxVertexCnt = 0;
    m_uiMemoryUseSize = sizeof(CNtlPLDecal);

	m_pIm3DBuffer = NULL;
	m_eDecalType = DECAL_EFFECT;

	m_bBlend = TRUE;
	m_blendSrc = rwBLENDSRCALPHA;
	m_blendDest = rwBLENDONE;
	m_nRenderVertexCnt = 0;	
	RwMatrixSetIdentity(&m_matTexture);
	m_bFirstRotate = TRUE;
	m_eFadeStatus = FADE_STATUS_OUT;
	
	ZeroMemory(&m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
	ZeroMemory(&m_EmitterMultiTextures, sizeof(RpPrtAdvEmtPrtMultiTextures));
	ZeroMemory(&m_EmitterPrtMultiSize, sizeof(SNtlAdvMultiSizeEmitter));

	RwMatrixSetIdentity(&m_matWorld);

	SetLayer(PLENTITY_LAYER_DECAL);
	SetClassType(PLENTITY_DECAL);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_ATTACH);
}

CNtlPLDecal::~CNtlPLDecal()
{
	Destroy();
}

RwBool CNtlPLDecal::Create( const SPLEntityCreateParam * pParam /*= NULL*/ )
{
	if(pParam)
	{
		const SPLDecalCreateParam *pDecalParam = reinterpret_cast<const SPLDecalCreateParam*>(pParam);	
		m_iNameLen = pDecalParam->nTexNameLen;
		m_fScale = pDecalParam->fScale;
		m_fStartScale = m_fScale;
		m_vSize = pDecalParam->vSize;
		m_fVisibleSquaredDist = pDecalParam->fVisibleSquaredDist;				
		m_rwWrapType = pDecalParam->rwWrapType;
		m_fYOffset = pDecalParam->fYOffset;
		m_eDecalType = pDecalParam->eDecalType;

		if(pDecalParam->pPos)
		{
			m_vPos = *pDecalParam->pPos;
		}
		m_nFlagEffect = pDecalParam->nFlagEffect;

		if(strlen(pDecalParam->pTexName) > 0)
		{
			m_pTexture = GetNtlResourceManager()->LoadTexture(pDecalParam->pTexName, pDecalParam->pTexPath);
			DBO_ASSERT(m_pTexture, "Texture load failed.");
			
            if(m_pTexture)
            {
			    RwTextureSetFilterMode(m_pTexture, rwFILTERLINEAR);
			    RwTextureSetAddressing(m_pTexture, m_rwWrapType);				
            }

			m_pCurrentTexture = m_pTexture;
		}
	}

    // Create vertex buffer
    m_pIm3DBuffer = (RwIm3DVertex*)CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_VERTEX_1024);
    m_nMaxVertexCnt = DECAL_VB_SMALL_SIZE;    
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * 1024;

    // Check the fade status at startup.
    if(m_eDecalType == DECAL_EFFECT)
    {
        m_eFadeStatus = FADE_STATUS_IN;      // Effect decals always start from Fade In.        
    }
    else
    {
        if(!IsVisble())
        {
            m_eFadeStatus = FADE_STATUS_OUT;
            m_uiFadeAlpha = 0;
            RwRGBA color;
            color.red = color.green = color.blue = 255;
            color.alpha = 0;
            SetVertexColor(color);
        }
        else
        {
            m_eFadeStatus = FADE_STATUS_IN;            
        }
    }    

	return TRUE;
}

void CNtlPLDecal::Destroy(void)
{
	if(m_pTexture)
	{
		GetNtlResourceManager()->UnLoadTexture(m_pTexture);
		m_pTexture = NULL;
	}

	if (m_ppMultiTextures != NULL)
	{
		for (RwUInt32 i = 0; i < m_EmitterMultiTextures.numTextures; ++ i)
		{
			if (m_ppMultiTextures[i] != NULL)
			{
				CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_ppMultiTextures[i]);
				m_ppMultiTextures[i] = NULL;
			}
		}
		NTL_ARRAY_DELETE(m_ppMultiTextures);
	}

	if (m_EmitterMultiTextures.list != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterMultiTextures.list);
		m_EmitterMultiTextures.list = NULL;
	}

	NTL_ARRAY_DELETE(m_EmitterPrtMultiCol.list);
	NTL_ARRAY_DELETE(m_EmitterPrtMultiSize.list);

    if(m_pIm3DBuffer)
    {
        if(m_nMaxVertexCnt == DECAL_VB_SMALL_SIZE)
        {
            CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_VERTEX_1024, m_pIm3DBuffer);
        }
        else if(m_nMaxVertexCnt == DECAL_VB_BIG_SIZE)
        {
            CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_VERTEX_5120, m_pIm3DBuffer);

        }

        m_pIm3DBuffer = NULL;
    }
}

RwBool CNtlPLDecal::Update(RwReal fElapsed)
{
	m_fTotalElapsedTime += fElapsed;
	if(m_fTotalElapsedTime < 0.0f)
	{
		m_fTotalElapsedTime = 0.0f;
	}

	m_fElapsedTime = fElapsed;

	if(!CNtlPLGlobal::m_bDecalRender)
		return TRUE;

	if(!GetSceneManager()->IsWorldReady())
		return TRUE;

	if(!IsVisble())
		return TRUE;

	// The scale action must be applied before creating the vertex.
	if(m_nFlagEffect & rpPRTADVEMITTERDATAFLAGMULTISIZE)
	{
		UpdateMultiSize(m_fElapsedTime);
		m_bVertexUpdate = TRUE;
	}
	else if(m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGPRTSIZE)
	{
		UpdateScale(m_fElapsedTime);
		m_bVertexUpdate = TRUE;
	}

	if(m_bVertexUpdate)
	{
		UpdateVertices();
		m_bVertexUpdate = FALSE;
	}

	if((m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGUVANIM) || (m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		UpdateUV();
	}

	if(m_nFlagEffect & rpPRTADVEMITTERDATAFLAGMULTICOLOR)
	{
		UpdateMultiColor(m_fElapsedTime);
	}
	else if(m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGPRTCOLOR)
	{
		UpdateColor(m_fElapsedTime);
	}
	else if((m_nFlagEffect & E_FLAG_DECAL_DAYNIGHT) && CNtlDNController::GetInstance()->IsDNEffectApplied())
	{
		SetVertexColor(m_color);
	}
    else if(GetWeightAlpha() != 1.0f)           // When Weight Alpha is applied
    {
        SetVertexColor(m_color);
    }

	// Fade In/Out
	if(m_eFadeStatus == FADE_STATUS_IN_ING || m_eFadeStatus == FADE_STATUS_OUT_ING)
	{
		UpdateFade(m_fElapsedTime);
	}

	if(m_nFlagEffect & rpPRTADVEMITTERDATAFLAGMULTITEXTURES)
	{
		UpdateMultiTexture(m_fElapsedTime);
	}

	return TRUE;
}

RwBool CNtlPLDecal::Render(void)
{
	if(!CNtlPLGlobal::m_bDecalRender)
		return FALSE;

	if(!GetSceneManager()->IsWorldReady())
		return FALSE;

	if(!IsVisble())
		return FALSE;

	if(m_nRenderVertexCnt <= 0)
		return FALSE;

	BeginEffectDecalSystemState(m_bBlend, m_blendSrc, m_blendDest);

    if(m_pCurrentTexture)
    {
        RwTextureSetAddressing(m_pCurrentTexture, rwTEXTUREADDRESSCLAMP);
        RwD3D9SetTexture(m_pCurrentTexture, 0);
    }
	
	if(RwIm3DTransform(m_pIm3DBuffer, m_nRenderVertexCnt, NULL, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
		RwIm3DEnd();
	}

    //RwTextureSetAddressing(m_pCurrentTexture, rwTEXTUREADDRESSWRAP);

	EndEffectDecalSystemState(m_bBlend);

	return TRUE;
}

RwBool CNtlPLDecal::SetProperty(const CNtlPLProperty *pData)
{
	return TRUE;
}

void CNtlPLDecal::SetPosition(const RwV3d *pPos)
{
	m_vPos = *pPos;	

	if(m_eDecalType == DECAL_TERRAIN)
	{
		RwV3d vPos = m_vPos;
		vPos.y = 0.0f;

		RwReal fSizeMax = (m_vSize.x > m_vSize.z) ? m_vSize.x : m_vSize.z;
		fSizeMax *= m_fScale;
		RwV3d vDist = vPos - m_vOriginPos;
		RwReal fDistance = RwV3dLength(&vDist) + fSizeMax;

		if(fDistance >= m_fRadiusWorldVertex)
		{
			m_bVertexUpdate	= TRUE;
			return;
		}
	}	
	
	// If it is an effect decal or a terrain decal and does not exceed the range of the created vertex, only the UV is updated.
	UpdateUV();
}

// NOTE: Use only when calling from outside and should not be used when fading. (Original Alpha value unrelated to Fade)
void CNtlPLDecal::SetAlpha(RwUInt8 byAlpha)
{
	m_color.alpha = byAlpha;

	SetVertexColor(m_color);
}

// NOTE: Use only when calling from outside and should not be used when fading. (Original Alpha value unrelated to Fade)
void CNtlPLDecal::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_color.red = byRed;
	m_color.green = byGreen;
	m_color.blue = byBlue;	

	SetVertexColor(m_color);	
}

void CNtlPLDecal::SetScale( RwReal fScale ) 
{
	m_bVertexUpdate = TRUE;
	m_fScale = fScale;	
}

void CNtlPLDecal::SetScaleSize( RwReal fScale ) 
{
    m_vSize.x *= fScale;
    m_vSize.z *= fScale;
}

void CNtlPLDecal::SetSize( RwReal fXSize, RwReal fZSize ) 
{
	m_vSize.x = fXSize;
	m_vSize.z = fZSize;
}

void CNtlPLDecal::SetAlphaBlend( RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend ) 
{
	m_bBlend = bBlend;
	m_blendSrc = srcBlend;
	m_blendDest = destBlend;
}

RwBool CNtlPLDecal::UpdateVertices() 
{
	m_nRenderVertexCnt = 0;

	if(m_nFlagEffect & E_FLAG_DECAL_WATER)
	{
		GetWaterDecalVertex();		// Calculate the decal drawn on the water.
	}
	else if(m_nFlagEffect & E_FLAG_DECAL_RENDER_WORLD)
	{
		// Decal calculation of World
		CNtlPLWorldEntity *pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
		if(!pWorldEntity)
			return FALSE;

		// Decal internal and external judgment.
		if (!GetNtlWorldSpace(&m_vPos, NULL, NULL))
		{
			RwV3d vResutSize = m_vSize * m_fScale;

			RwBool bResult = pWorldEntity->GetWorldDecal(m_vPos, vResutSize, m_nMaxVertexCnt, m_nRenderVertexCnt, m_fRadiusWorldVertex, m_pIm3DBuffer, m_fYOffset);		
			if(!bResult)
			{
				// If the buffer is too small to contain everything, change to a larger buffer.
				if(m_nRenderVertexCnt >= m_nMaxVertexCnt && m_nMaxVertexCnt == DECAL_VB_SMALL_SIZE)
				{
					ChangeBigVB();
					bResult = pWorldEntity->GetWorldDecal(m_vPos, vResutSize, m_nMaxVertexCnt, m_nRenderVertexCnt, m_fRadiusWorldVertex, m_pIm3DBuffer, m_fYOffset);
					if(!bResult)
						return FALSE;
				}
				else
				{
					return FALSE;
				}
			}

			// Calculate the midpoint and radius of the created vertex
			m_vOriginPos = m_vPos;
			m_vOriginPos.y = 0.0f;

			m_fRadiusWorldVertex *= 0.6f;
		}		
	}

	if(m_nFlagEffect & E_FLAG_DECAL_RENDER_OBJECT)
	{
		// Decal calculation of objects
		GetObjectDecalVertex();
	}

	SetVertexColor(m_color);
	UpdateUV();

	return TRUE;
}

RwBool CNtlPLDecal::GetWaterDecalVertex() 
{
	RwReal fWaterHeight = GetSceneManager()->GetWorldWaterHeight(m_vPos);
	if(fWaterHeight < -998.0f)			// If it is not water, -999.0f is returned.
		return FALSE;


	RwReal fXHalfSize = m_vSize.x * m_fScale * 0.5f;
	RwReal fZHalfSize = m_vSize.z * m_fScale * 0.5f;
	RwV3d vPos[4];
	vPos[0].x = m_vPos.x + fXHalfSize;
	vPos[0].z = m_vPos.z + fZHalfSize;
	vPos[1].x = m_vPos.x - fXHalfSize;
	vPos[1].z = m_vPos.z + fZHalfSize;
	vPos[2].x = m_vPos.x + fXHalfSize;
	vPos[2].z = m_vPos.z - fZHalfSize;
	vPos[3].x = m_vPos.x - fXHalfSize;
	vPos[3].z = m_vPos.z - fZHalfSize;
	for(int i = 0; i < 4; ++i)
	{
		vPos[i].y = fWaterHeight + m_fYOffset;
	}

	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[0];
	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[2];
	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[3];

	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[0];
	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[3];
	m_pIm3DBuffer[m_nRenderVertexCnt++].objVertex = vPos[1];

	return TRUE;
}

void CNtlPLDecal::SetVertexColor(const RwRGBA& color)
{
	for(int i = 0; i < m_nRenderVertexCnt; ++i)
	{
		// If the flag affected by Day and Night is turned on, it will be affected.
		if(m_nFlagEffect & E_FLAG_DECAL_DAYNIGHT)
		{
			RwRGBA colDayNight = color;
			colDayNight.red		-= GetRValue(dGET_WORLD_PARAM()->ClrDayAndNight);
			colDayNight.green	-= GetGValue(dGET_WORLD_PARAM()->ClrDayAndNight);
			colDayNight.blue	-= GetBValue(dGET_WORLD_PARAM()->ClrDayAndNight);
            colDayNight.alpha   = (RwUInt8)(colDayNight.alpha * GetWeightAlpha());

            RwIm3DVertexSetRGBA(&m_pIm3DBuffer[i], colDayNight.red, colDayNight.green, colDayNight.blue, color.alpha);
		}
		else
		{
			RwIm3DVertexSetRGBA(&m_pIm3DBuffer[i], color.red, color.green, color.blue, (RwUInt8)(color.alpha * GetWeightAlpha()));
		}		
	}	
}

void CNtlPLDecal::UpdateUV() 
{
	if(m_nRenderVertexCnt <= 0)
		return;

	// UV is calculated by calculating the distance from the center point.
	RwReal fU, fV;
	RwV3d v3dPos = m_vPos;
	v3dPos.y = 0.0f;	
	RwV2d v2dRealSize;
	
	v2dRealSize.x = m_vSize.x * m_fScale;
	v2dRealSize.y = m_vSize.z * m_fScale;

	for(int i = 0; i < m_nRenderVertexCnt; ++i)
	{
		RwIm3DVertex pVertex = m_pIm3DBuffer[i];
		pVertex.objVertex.y = 0.0f;
		RwV3d vDist = pVertex.objVertex - v3dPos;
		
		fU = 0.5f - (vDist.x / v2dRealSize.x);
		fV = 0.5f - (vDist.z / v2dRealSize.y);

		m_pIm3DBuffer[i].u = fU;
		m_pIm3DBuffer[i].v = fV;
	}

	RwMatrixSetIdentity(&m_matTexture);

	// Apply the Rotate value. (Rotate on Matrix, not Rotate Action)
	RwMatrixRotate(&m_matTexture, &ZAxis, m_fDegree, rwCOMBINEPOSTCONCAT);	

	// Apply Rotate Action to the newly set UV threshold value.
	if(m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE)
	{
		UpdateRoate(m_fTotalElapsedTime);				
	}

	if(m_nFlagEffect & rpPRTSTDEMITTERDATAFLAGUVANIM)
	{
		UpdateUVAnim(m_fElapsedTime);		
	}

	SetTextureMatrix(m_pIm3DBuffer, m_nRenderVertexCnt, &m_matTexture);	
}

void CalcIntersectionZone(RwV3d *pPos, RwReal fIntersectionRadius, RwReal fHeightRatio = 1.f)
{
	RwV3d v3Box;

	v3Box.x = v3Box.y = v3Box.z = fIntersectionRadius;
	v3Box.y = v3Box.y * fHeightRatio;

	g_InterZone.type = rpINTERSECTBOX;
	
	g_InterZone.t.box.sup = *pPos + v3Box;
	g_InterZone.t.box.sup.y = pPos->y + 1.0f;
	g_InterZone.t.box.inf = *pPos - v3Box;
}

static RpCollisionTriangle *
ShadowRenderAtomicObjectTriangleCB( RpIntersection * intersection __RWUNUSED__,
								   //RpWorldSector * worldSector __RWUNUSED__,
								   RpCollisionTriangle * collTriangle,
								   RwReal distance __RWUNUSED__, void *data)
{
	DecalCallbackParam *pDecalParam = (DecalCallbackParam*) data;

	RwInt32 nBuffCnt = *(pDecalParam->pBufferCnt);  

	// If the maximum number is exceeded, the vertex is canceled without being created.
	if(nBuffCnt + 2 >= pDecalParam->nMaxVertextCnt)			
		return collTriangle;

	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;

	RwV3d v0, v1;
	RwV3dSubMacro(&v0, collTriangle->vertices[1], collTriangle->vertices[0]);
	RwV3dSubMacro(&v1, collTriangle->vertices[2], collTriangle->vertices[0]);
	RwV3dCrossProduct(&pImv->objNormal, &v0, &v1);
	RwV3dNormalize(&pImv->objNormal, &pImv->objNormal);

	if (0.0f < RwV3dDotProduct(&pImv->objNormal, &CNtlPLGlobal::m_vYAxisV3))
	{
		for (int i = 0; i < 3; ++i)
		{
			RwIm3DVertexSetPos(&pImv[i], collTriangle->vertices[i]->x, collTriangle->vertices[i]->y, collTriangle->vertices[i]->z);
			RwV3dTransformPoint(&pImv[i].objVertex, &pImv[i].objVertex, pDecalParam->matAtomicTrans);			
		}

		*(pDecalParam->pBufferCnt) += 3;
	}

// 	DecalCallbackParam *pDecalParam = (DecalCallbackParam*) data;
// 
// 	RwInt32 nBuffCnt = *(pDecalParam->pBufferCnt);  
// 	
// 	// If the maximum number is exceeded, the vertex is canceled without being created.
// 	if(nBuffCnt + 2 >= pDecalParam->nMaxVertextCnt)			
// 		return collTriangle;
// 
// 	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;
// 
// 	RwV3d vIn[3];
// 
// 	vIn[0] = *(collTriangle->vertices[0]);
// 	vIn[1] = *(collTriangle->vertices[1]);
// 	vIn[2] = *(collTriangle->vertices[2]);
// 
// 	RwV3dTransformPoints(&vIn[0], &vIn[0], 3, pDecalParam->matAtomicTrans);
// 
// 	RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
// 	RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
// 	RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);
// 
// 	*(pDecalParam->pBufferCnt) += 3;

	return collTriangle;
}

RpAtomic* ShadowRenderAtomicCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );
	if(pPLEntity && pPLEntity->GetClassType() == PLENTITY_OBJECT)
	{
		if((RpNtlAtomicGetFlag(pAtomic) & NTL_DECAL_VISIBLE) &&  
           !(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE) &&
           pPLEntity->IsVisible())
		{
			RwFrame *pFrame = RpAtomicGetFrame(pAtomic);
			RwMatrix *pMatrix = RwFrameGetLTM(pFrame);

			RwMatrix *pMatInvLtm = RwMatrixCreate();

			RwMatrixInvert(pMatInvLtm, pMatrix);

			DecalCallbackParam *param = (DecalCallbackParam *)pData;

			RwV3d vModelPos;			
			RwV3dTransformPoint(&vModelPos, &param->vPos, pMatInvLtm);
			
			CalcIntersectionZone(&vModelPos, param->fIntersectionRadius, 1.0f);
			

			//CNtlMath::MathRwV3dAssign(&param->vPos, -vModelPos.x+param->fScale/2.0f, -vModelPos.y, -vModelPos.z+param->fScale/2.0f);

			RwMatrix *pMatAtomicLtm = RwMatrixCreate();
			RwMatrixTransform(pMatAtomicLtm, pMatrix, rwCOMBINEREPLACE);
			pMatAtomicLtm->pos.y += param->fYOffset;
			param->matAtomicTrans = pMatAtomicLtm;

			RpCollisionGeometryForAllIntersections(RpAtomicGetGeometry(pAtomic), &g_InterZone, ShadowRenderAtomicObjectTriangleCB, param);

			RwMatrixDestroy(pMatInvLtm);
			RwMatrixDestroy(pMatAtomicLtm);
		}
	}

	return pAtomic;
}

void CNtlPLDecal::GetObjectDecalVertex() 
{
    if(!CNtlPLGlobal::m_pRpWorld)
        return;

	// Get decal		

	DecalCallbackParam param;		
	param.vPos = m_vPos;
	param.pBufferCnt = (RwInt32*)&m_nRenderVertexCnt; ///< Contains the vertex buffer created so far starting from the next.
	param.pVertices = m_pIm3DBuffer;			
	param.fIntersectionRadius =  max(m_vSize.x, m_vSize.z) * m_fScale * 0.5f;
	param.fScale = m_fScale;
	param.nMaxVertextCnt = m_nMaxVertexCnt;
	param.fYOffset = m_fYOffset;

	g_InterZone.t.point = m_vPos;

	RpIntersection InterZone;
	InterZone.type = rpINTERSECTSPHERE;	
	InterZone.t.sphere.center = m_vPos;
	InterZone.t.sphere.radius = param.fIntersectionRadius;

	RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &InterZone, ShadowRenderAtomicCB, (void *) &param);
    
    // If the buffer is too small to contain everything, change to a larger buffer.
    if(m_nMaxVertexCnt == DECAL_VB_SMALL_SIZE && m_nRenderVertexCnt > 1020)
    {
        ChangeBigVB();
    }
}

void CNtlPLDecal::ChangeBigVB() 
{
    RwIm3DVertex* pNewVB = (RwIm3DVertex*)CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_VERTEX_5120);
    ZeroMemory(pNewVB, sizeof(RwIm3DVertex*) * DECAL_VB_BIG_SIZE);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * 5120;

    memcpy_s(pNewVB, sizeof(RwIm3DVertex*) * DECAL_VB_BIG_SIZE, m_pIm3DBuffer, sizeof(RwIm3DVertex*) * DECAL_VB_SMALL_SIZE);
    
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_VERTEX_1024, m_pIm3DBuffer);
    m_uiMemoryUseSize -= sizeof(RwIm3DVertex) * 1024;
    m_pIm3DBuffer = pNewVB;
    m_nMaxVertexCnt = DECAL_VB_BIG_SIZE;
}

RwBool CNtlPLDecal::IsVisble() 
{
	RwReal fMaxSize = (m_vSize.x > m_vSize.y) ? m_vSize.x * m_fScale : m_vSize.y * m_fScale;
	RwSphere sphere;
	sphere.center = m_vPos;
	sphere.radius = fMaxSize;

	RwFrustumTestResult ret = RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere);
	if(ret == rwSPHEREOUTSIDE)
	{
		return FALSE;
	}
	else
	{
		RwReal fCameraDist;
		RwV3d vDist = sphere.center - RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.pos;				
		fCameraDist = CNtlMath::MathRwV3dSquaredLength(&vDist);
		if(fCameraDist > m_fVisibleSquaredDist)
		{
			if(m_eFadeStatus == FADE_STATUS_IN || m_eFadeStatus == FADE_STATUS_IN_ING)
			{
				m_eFadeStatus = FADE_STATUS_OUT_ING;
				m_uiFadeAlpha = m_color.alpha;
				m_fFadeGap = (RwReal)m_uiFadeAlpha / m_cfFadeTime;
				return TRUE;
			}
			else if(m_eFadeStatus == FADE_STATUS_OUT_ING)
			{				
				return TRUE;
			}
			else
			{
				return FALSE;
			}			
		}
        else
        {
            if(m_eFadeStatus == FADE_STATUS_OUT || m_eFadeStatus == FADE_STATUS_OUT_ING)
            {
                m_uiFadeAlpha = 0;
                m_fFadeGap = 255.0f / m_cfFadeTime;
                m_eFadeStatus = FADE_STATUS_IN_ING;
            }
        }
	}

	return TRUE;
}

void CNtlPLDecal::UpdateRoate(RwReal fElapsedTime)
{
	RwReal fElapsedAngle = m_f2DRoateAngle * fElapsedTime;	
	SetRotate(fElapsedAngle);
}

void CNtlPLDecal::SetTextureMatrix(RwIm3DVertex* pVertices, RwInt32 nVertCnt, RwMatrix* pMatrix)
{
	if(!pVertices || !pMatrix || nVertCnt <= 0)
		return;

	/*
		| rx ry - | (right)
		| ux uy - | (up)
		| -  -  - | (at)
		| px py - | (pos) 

		u' = rx * u + ux * v + px
		v' = ry * u + uy * v + py 
	*/

	RwV3d* pvRight = RwMatrixGetRight(pMatrix);
	RwV3d* pvUp = RwMatrixGetUp(pMatrix);
	RwV3d* pvPos = RwMatrixGetPos(pMatrix);

	for(int i = 0; i < nVertCnt; ++i)
	{
		
		// Since the texture's origin (0,0) and the desired origin (0.5, 0.5) are different, 0.5 is added to the uv coordinates.
		// However, since the calculation must be based on the original origin (0,0), 0.5 is subtracted again before calculation.		
		// Caution) If this is not done, the texture will rotate based on the origin (0, 0). 
		// Note: You can keep the original value separately and use the result, but to save memory, just calculate it.
		pVertices[i].u -= 0.5f;
		pVertices[i].v -= 0.5f;		

		RwReal fU = pvRight->x * pVertices[i].u + pvUp->x * pVertices[i].v + pvPos->x;
		RwReal fV = pvRight->y * pVertices[i].u + pvUp->y * pVertices[i].v + pvPos->y;

		pVertices[i].u = fU + 0.5f;
		pVertices[i].v = fV + 0.5f;
	}
	
}

void CNtlPLDecal::Set2DRoateAction(RwReal fAngle, RwReal fTimeGap /* = 1.0f */)
{
	if(fTimeGap == 0.0f)
		return;

	m_nFlagEffect |= rpPRTSTDEMITTERDATAFLAGPRT2DROTATE;

	m_f2DRoateAngle = fAngle / fTimeGap;

	m_fOrgRotateAngle = fAngle;
	m_fRotateTimeGap = fTimeGap;
}

void CNtlPLDecal::SetColorAction(const RwRGBA& colStart, const RwRGBA& colEnd, RwReal fTimeGap)
{
	if(fTimeGap == 0.0f)
		return;

	m_nFlagEffect |= rpPRTSTDEMITTERDATAFLAGPRTCOLOR;

	m_colStart = colStart;
	m_colEnd = colEnd;
	m_fColorActionTime = fTimeGap;	
}

void CNtlPLDecal::UpdateColor(RwReal fElapsedTime)
{
	RwReal fDeltaTime = fmod(m_fTotalElapsedTime, m_fColorActionTime) / m_fColorActionTime;
	RwInt32 odd = ((RwInt32)m_fTotalElapsedTime / (RwInt32)m_fColorActionTime) % 2;

	RwRGBA color;
	if(odd == 0) // If the divided number is even, interpolate toward the end.
	{
		color = CNtlMath::Interpolation(m_colStart, m_colEnd, fDeltaTime);
	}
	else // If the raw number is odd, interpolate toward start.
	{
		color = CNtlMath::Interpolation(m_colEnd, m_colStart, fDeltaTime);
	}

	SetVertexColor(color);
}

void CNtlPLDecal::UpdateScale(RwReal fElapsedTime)
{
	RwReal fDeltaTime = fmod(m_fTotalElapsedTime, m_fScaleActionTimeGap) / m_fScaleActionTimeGap;
	RwInt32 odd = (RwInt32)(m_fTotalElapsedTime / m_fScaleActionTimeGap) % 2;

	RwReal fScale = 1.0f;
	if(odd == 0)
	{
		fScale = m_fStartScale + (m_fEndScale - m_fStartScale) * fDeltaTime;
	}
	else
	{
		fScale = m_fEndScale + (m_fStartScale - m_fEndScale) * fDeltaTime;
	}

	SetScale(fScale);
}

void CNtlPLDecal::SetSizeAction( RwReal fStartScale, RwReal fEndScale, RwReal fTimeGap ) 
{
	if(fTimeGap == 0.0f)
		return;

	m_nFlagEffect |= rpPRTSTDEMITTERDATAFLAGPRTSIZE;
	
	m_fStartScale = fStartScale;
	m_fEndScale = fEndScale;
	m_fScaleActionTimeGap = fTimeGap;
}

void CNtlPLDecal::SetUVAnimAction( const SNtlPrtStdEmitterPrtUVAnim& emitterUVAnim ) 
{
	m_nFlagEffect |= rpPRTSTDEMITTERDATAFLAGUVANIM;

	m_EmitterPrtUVAnim = emitterUVAnim;
	m_EmitterPrtUVAnim.vDir.y = emitterUVAnim.vDir.z;
	m_EmitterPrtUVAnim.vDir.z = 0.0f;
	RwV3dNormalize(&m_EmitterPrtUVAnim.vDir, &m_EmitterPrtUVAnim.vDir);
}

void CNtlPLDecal::SetMultiTextureAction( const RpPrtAdvEmtPrtMultiTextures& emitterMultiTexture ) 
{
	if(emitterMultiTexture.numTextures <= 0)
		return;

	m_nFlagEffect |= rpPRTADVEMITTERDATAFLAGMULTITEXTURES;

	memcpy(&m_EmitterMultiTextures, &emitterMultiTexture, sizeof(RpPrtAdvEmtPrtMultiTextures));
	if (m_EmitterMultiTextures.numTextures > 0)
	{
		m_EmitterMultiTextures.list = NTL_NEW RpPrtAdvEmtPrtTextureItem[m_EmitterMultiTextures.numTextures];
        m_uiMemoryUseSize += sizeof(RpPrtAdvEmtPrtTextureItem) * m_EmitterMultiTextures.numTextures;
		for (RwUInt32 i = 0;i < m_EmitterMultiTextures.numTextures; ++ i)
		{
			m_EmitterMultiTextures.list[i].time = emitterMultiTexture.list[i].time;
			m_EmitterMultiTextures.list[i].timeBias = emitterMultiTexture.list[i].timeBias;
			m_EmitterMultiTextures.list[i].strTexture = m_EmitterMultiTextures.list[i].strTexture.c_str();
		}
	}

	m_ppMultiTextures = NTL_NEW RwTexture*[emitterMultiTexture.numTextures];
    m_uiMemoryUseSize += sizeof(RwTexture*) * emitterMultiTexture.numTextures;

	for(RwUInt32 i = 0; i < emitterMultiTexture.numTextures; ++i)
	{
		m_ppMultiTextures[i] = NULL;
		if(!emitterMultiTexture.list[i].strTexture.empty())
		{
			m_ppMultiTextures[i] = CNtlEffectSystemFreeList	::FindTexture(emitterMultiTexture.list[i].strTexture.c_str());	
			if(m_ppMultiTextures[i])
			{
				RwTextureSetFilterMode(m_ppMultiTextures[i], rwFILTERLINEAR);
				RwTextureSetAddressing(m_ppMultiTextures[i], m_rwWrapType);
			}
		}
	}
}

void CNtlPLDecal::SetMultiColorAction( const RpPrtAdvPrtMultiColorEmitter& emitterPrtMulticol ) 
{
	if(emitterPrtMulticol.numCol <= 0)
		return;

	m_nFlagEffect |= rpPRTADVEMITTERDATAFLAGMULTICOLOR;
	memcpy(&m_EmitterPrtMultiCol, &emitterPrtMulticol, sizeof(RpPrtAdvPrtMultiColorEmitter));
	m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[m_EmitterPrtMultiCol.numCol];
    m_uiMemoryUseSize += sizeof(RpPrtAdvEmtPrtColorItem) * m_EmitterPrtMultiCol.numCol;
	memcpy(m_EmitterPrtMultiCol.list, emitterPrtMulticol.list, sizeof(RpPrtAdvEmtPrtColorItem) * m_EmitterPrtMultiCol.numCol);
}

void CNtlPLDecal::SetMultiSizeAction( const SNtlAdvMultiSizeEmitter& emitterPrtMultiSize ) 
{
	if(emitterPrtMultiSize.numSize <= 0)
		return;

	m_nFlagEffect |= rpPRTADVEMITTERDATAFLAGMULTISIZE;
	memcpy(&m_EmitterPrtMultiSize, &emitterPrtMultiSize, sizeof(SNtlAdvMultiSizeEmitter));		
	m_EmitterPrtMultiSize.list = NTL_NEW SNtlAdvEmtPrtSizeItem[m_EmitterPrtMultiSize.numSize];
    m_uiMemoryUseSize += sizeof(SNtlAdvEmtPrtSizeItem) * m_EmitterPrtMultiSize.numSize;
	memcpy(m_EmitterPrtMultiSize.list, emitterPrtMultiSize.list, sizeof(SNtlAdvEmtPrtSizeItem) * m_EmitterPrtMultiSize.numSize);	

	m_fPrevSize = m_fStartScale;
}

void CNtlPLDecal::UpdateMultiTexture(RwReal fElapsedTime)
{
	m_fMultiTextureDeltaTime += fElapsedTime;

	RwReal fTextureTime = m_EmitterMultiTextures.list[m_nCurrentTexture].time;
	if(m_fMultiTextureDeltaTime > fTextureTime)
	{
		++m_nCurrentTexture;
		m_fMultiTextureDeltaTime -= fTextureTime;

		if(m_nCurrentTexture >= m_EmitterMultiTextures.numTextures)
		{
			if(m_EmitterMultiTextures.bLoop)
			{
				m_nCurrentTexture = 0;
			}
			else	// Unless it is a loop, the last texture is continuously displayed.
			{
				--m_nCurrentTexture;
			}			
		}

		m_pCurrentTexture = m_ppMultiTextures[m_nCurrentTexture];
	}
}

void CNtlPLDecal::UpdateMultiColor( RwReal fElapsedTime ) 
{
	if(m_EmitterPrtMultiCol.numCol <= 0)
		return;

	m_fMultiColorDeltaTime += fElapsedTime;

	RwReal fNextTime = m_EmitterPrtMultiCol.list[m_nNextColorIndex].time;
	if(m_fMultiColorDeltaTime >= fNextTime)
	{
		m_fMultiColorDeltaTime -= fNextTime;
		m_colPrev.red	= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.red;
		m_colPrev.green = (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.green;
		m_colPrev.blue	= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.blue;
		m_colPrev.alpha = (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.alpha;

		++m_nNextColorIndex;
		if(m_nNextColorIndex >= m_EmitterPrtMultiCol.numCol)
		{
			m_nNextColorIndex = 0;
		}
	}

	RwRGBA colNext;
	colNext.red		= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.red;
	colNext.green	= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.green;
	colNext.blue	= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.blue;
	colNext.alpha	= (RwUInt8)m_EmitterPrtMultiCol.list[m_nNextColorIndex].midCol.alpha;

	RwReal fDeltaTime = m_fMultiColorDeltaTime / m_EmitterPrtMultiCol.list[m_nNextColorIndex].time;
	RwRGBA color = CNtlMath::Interpolation(m_colPrev, colNext, fDeltaTime);
	SetVertexColor(color);
}


void CNtlPLDecal::UpdateMultiSize( RwReal fElapsedTime ) 
{
	if(m_EmitterPrtMultiSize.numSize <= 0)
		return ;

	// Multi Size Time refers to the gap time between each item. 
	// Terrain Decal does not have an EndTime, so it cannot be expressed as a percentage of time like a particle, so it was used as a Gap.
	// That is, [1] 0.5 seconds, [2] 0.3 seconds, and so on, changing to the first item over 0.5 seconds, and then over 0.3 seconds.
	// This is done by changing to the second item. (by agebreak 2007.03.26)

	m_fMultiSizeDeltaTime += fElapsedTime;

	RwReal fNextSize = 1.0f;
	RwReal fNextTime = m_EmitterPrtMultiSize.list[m_nNextSizeIndex].time;
	if(m_fMultiSizeDeltaTime >= fNextTime)
	{
		m_fMultiSizeDeltaTime -= fNextTime;

		m_fPrevSize = m_EmitterPrtMultiSize.list[m_nNextSizeIndex].midSize.x;
		
		++m_nNextSizeIndex;
		if(m_nNextSizeIndex >= m_EmitterPrtMultiSize.numSize)
		{
			m_nNextSizeIndex = 0;
		}		
	}	

	fNextSize = m_EmitterPrtMultiSize.list[m_nNextSizeIndex].midSize.x;
	
	RwReal fDeltaTime = m_fMultiSizeDeltaTime / m_EmitterPrtMultiSize.list[m_nNextSizeIndex].time;    
    fDeltaTime = CNtlMath::Range(fDeltaTime, 0.0f, 1.0f);	
    RwReal fSize = CNtlMath::Interpolation(m_fPrevSize, fNextSize, fDeltaTime);
	SetScale(fSize);
}

void CNtlPLDecal::SetRotate( RwReal fDegree ) 
{
	m_bVertexUpdate = TRUE;

	m_fDegree = fDegree;	
}

void CNtlPLDecal::SetMatrix(RwMatrix& matWorld ) 
{
	m_matWorld = matWorld;

// NOTE: The Rotate value will be applied on subsequent UpdateUV() calls.

	RwV3d* pPos = RwMatrixGetPos(&m_matWorld);		
	SetPosition(pPos);

	m_fDegree = CNtlMath::LineToAngleY(RwMatrixGetAt(&m_matWorld));		
}

void CNtlPLDecal::UpdateUVAnim( RwReal fElapsedTime ) 
{
	RwV3d vPos;
	RwReal fVelocity = 0.0f;
	if(m_EmitterPrtUVAnim.fMaxOffset > 0.0f)
	{
		RwReal fOffsetTime = m_EmitterPrtUVAnim.fMaxOffset / m_EmitterPrtUVAnim.fVelocity;
		RwInt32 nOdd = (RwInt32)(m_fTotalElapsedTime / fOffsetTime);
		RwReal fTime = fmod(m_fTotalElapsedTime, fOffsetTime);
		
		if(nOdd % 2 == 0)	// Advance
		{
			fVelocity = m_EmitterPrtUVAnim.fVelocity * fTime;
		}
		else				// Reverse
		{
			fVelocity = m_EmitterPrtUVAnim.fVelocity * (fOffsetTime - fTime);
		}		
	}
	else
	{
		fVelocity = m_EmitterPrtUVAnim.fVelocity * m_fTotalElapsedTime;	
	}

	vPos = m_EmitterPrtUVAnim.vDir * fVelocity;
	
	RwMatrixTranslate(&m_matTexture, &vPos, rwCOMBINEPOSTCONCAT);
}

void CNtlPLDecal::UpdateFade( RwReal fElapsedTime ) 
{
	if(m_eFadeStatus == FADE_STATUS_IN_ING)
	{
		m_uiFadeAlpha += (RwUInt8)(m_fFadeGap * fElapsedTime);		
		if(m_uiFadeAlpha > m_color.alpha)
		{
			m_uiFadeAlpha = m_color.alpha;
			m_eFadeStatus = FADE_STATUS_IN;
		}
	}
	else if(m_eFadeStatus == FADE_STATUS_OUT_ING)
	{
		m_uiFadeAlpha -= (RwUInt8)(m_fFadeGap * fElapsedTime);
		if(m_uiFadeAlpha < 0)
		{
			m_uiFadeAlpha = 0;
			m_eFadeStatus = FADE_STATUS_OUT;
		}
	
	}

	RwRGBA color = m_color;
	color.alpha = m_uiFadeAlpha;
	SetVertexColor(color);
}

void CNtlPLDecal::GetRotateActionValue( RwReal& fAngle, RwReal& fTimeGap ) 
{
	fAngle = m_fOrgRotateAngle;
	fTimeGap = m_fRotateTimeGap;
}

void CNtlPLDecal::GetScaleActionValue(RwReal& _StartScale, RwReal& _EndScale, RwReal& _TimeGap)
{
	_StartScale				= m_fStartScale;
	_EndScale				= m_fEndScale;
	_TimeGap				= m_fScaleActionTimeGap;
}

