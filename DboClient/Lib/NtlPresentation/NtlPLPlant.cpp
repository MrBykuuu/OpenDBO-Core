#include "precomp_ntlpresentation.h"

#include "ntlworldcommon.h"
#include "ntlworldinterface.h"

#include "NtlPLGlobal.h"
#include "NtlPLEvent.h"
#include "NtlPLRenderState.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLResourceManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"
#include "NtlWorldFieldManager.h"

#include "NtlAtomic.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"

#include "NtlPLPlantProperty.h"
#include "NtlPLPlant.h"

CNtlPLPlant::CNtlPLPlant()
:m_pProperty(NULL)
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD);
	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetClassType(PLENTITY_PLANT);

	m_fClipDistance = 0.0f;
}

CNtlPLPlant::~CNtlPLPlant()
{
}

RwBool CNtlPLPlant::Create(const SPLEntityCreateParam* pParam /* = NULL */)
{
	NTL_FUNCTION("CNtlPLPlant::Create");

	NTL_RETURN(TRUE);
}

void CNtlPLPlant::Destroy()
{
	NTL_FUNCTION("CNtlPLPlant::Destroy");

	NTL_RETURNVOID();
}

void CNtlPLPlant::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlPLPlant::HandleEvents");

	NTL_RETURNVOID();
}

RwBool CNtlPLPlant::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLPlant::Update");

#ifdef dNTL_WORLD_TOOL_MODE
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PLNAT))
	{
		m_mapSector.clear();
		NTL_RETURN(TRUE);
	}
#endif

	RwCamera*	pCamera = CNtlPLGlobal::m_RwCamera;
	RwV3d		vNormal;
	RwV3d		vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pCamera)));

	// Calculates the clip plane.
	RwV3dNegate(&vNormal, RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(pCamera))));
	RwV3dIncrementScaled(&vPos, RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(pCamera))), m_fClipDistance);
	m_planeCilp.PlaneFromPointNormal(&vPos, &vNormal);

	m_fWeightElapsedTime += fElapsed;

	if (m_fWeightElapsedTime < 0.125f)
	{
		NTL_RETURN(TRUE);
	}
	m_fWeightElapsedTime = 0.0f;

	m_mapSector.clear();

	RwInt32				iCurIdx		= GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pCamera))));
	CNtlWorldSector*	pSectors	= GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors;

	if (iCurIdx == -1)
	{
		NTL_RETURN(TRUE);
	}

	std::vector<RwInt32> vecNeighborSectorIndices;
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNeighborSectors(iCurIdx, vecNeighborSectorIndices);
	vecNeighborSectorIndices.push_back(iCurIdx);

	// Searches surrounding sectors to obtain a list of objects included in the visible area.
	RwInt32 iNeighborSectorSize = vecNeighborSectorIndices.size();
	if (vecNeighborSectorIndices.empty())
	{
		NTL_RETURN(TRUE);
	}

	RwInt32 iNeighborSector = 0;
	do 
	{		
		// Let's not forget. Sector may be invalid.
		if (vecNeighborSectorIndices.at(iNeighborSector) == -1)
		{
			continue;
		}

		CNtlWorldSector* pSector = &pSectors[vecNeighborSectorIndices.at(iNeighborSector)];
		if (!PlantFrustumTestSector(pSector))
		{
			continue;
		}

		std::vector<sSECTOR_PLANT_SET_ATTR*>::iterator itPlantSet		= pSector->m_vecPlantSet.begin();
		std::vector<sSECTOR_PLANT_SET_ATTR*>::iterator itPlantSetEnd	= pSector->m_vecPlantSet.end();

		if (itPlantSet == itPlantSetEnd)
		{
			continue;
		}

		do
		{
			sSECTOR_PLANT_SET_ATTR*					pPlantSet	= *itPlantSet;
			CNtlPLPlantProperty::PROPERTY_MAP_IT	itProp		= m_pProperty->m_mapProperty.find(pPlantSet->iSetIdx);

			DBO_ASSERT(itProp != m_pProperty->m_mapProperty.end(), "Property find failed.");

			sSECTOR_PLANT_SET_ATTR::PLANT_OBJ_VEC_IT itPlantObj		= pPlantSet->vecPlantObj.begin();
			sSECTOR_PLANT_SET_ATTR::PLANT_OBJ_VEC_IT itPlantObjEnd	= pPlantSet->vecPlantObj.end();

			if (itPlantObj == itPlantObjEnd)
			{
				continue;
			}

			CNtlPLPlantProperty::sPLANT_PROP&	sProperty = itProp->second;
			RwV3d								vTemp;

			// Find the center point of the first tile.
			vTemp.x = pSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2) + (dGET_WORLD_PARAM()->WorldSectorTileSize / 2);
			vTemp.y = 0.0f;
			vTemp.z = pSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2) + (dGET_WORLD_PARAM()->WorldSectorTileSize / 2);

			do
			{
				sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR*	pPlantObj = *itPlantObj;
				RwSphere										sphereTile;

				// Calculates the center point of the object.
				sphereTile.radius	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				sphereTile.center.x = vTemp.x + (RwReal)(((pPlantObj->iTileIdx % dGET_WORLD_PARAM()->WorldSectorTileNum)) * dGET_WORLD_PARAM()->WorldSectorTileSize);
				sphereTile.center.y = 0.0f;
				sphereTile.center.z = vTemp.z + (RwReal)(((pPlantObj->iTileIdx / dGET_WORLD_PARAM()->WorldSectorTileNum)) * dGET_WORLD_PARAM()->WorldSectorTileSize);

				// Find the height value of the current location.
				GetSceneManager()->GetTerrainHeight(&sphereTile.center, sphereTile.center.y);

				// Check whether the current tile is within the visible area.
				if (!PlantFrustumTestSphere(&sphereTile))
				{
					continue;
				}

				sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_OBJ_VEC::iterator itPlantObjMat		= pPlantObj->vecObjMatrix.begin();
				sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_OBJ_VEC::iterator itPlantObjMatEnd	= pPlantObj->vecObjMatrix.end();

				if (itPlantObjMat == itPlantObjMatEnd)
				{
					continue;
				}

				RwInt32 iObjIdx = 0;
				do 
				{
					std::string&													strResourceName	= sProperty.vecstrResourceName.at(iObjIdx);						
					sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC*	pvecTransform	= pPlantObj->vecObjMatrix.at(iObjIdx);

					// The pointer of the object list of the corresponding tile is included in the list as a whole.
					if (!pvecTransform->empty())
					{
						m_mapSector[pSector][strResourceName].push_back(pvecTransform);
					}
				} while(++iObjIdx, ++itPlantObjMat != itPlantObjMatEnd);
			} while(++itPlantObj != itPlantObjEnd);
		} while(++itPlantSet != itPlantSetEnd);					
	} while(++iNeighborSector < iNeighborSectorSize);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPlant::PlantFrustumTestSphere(const RwSphere* pSphere)
{
	// bacamera.c(void CameraBuildPerspClipPlanes(RwCamera) Reference)
	RwCamera* pCamera = CNtlPLGlobal::m_RwCamera;

	// Far :: Different from the existing visible area check using the clip plane.
	/*
	*The normals of the clip planes in renderware are pointing outward.
	*To set the DX renderstate, you must face the inside of the clip plane for normal processing.
	*Because the clip plane distance is different and the direction is different, FarClipPlane is processed differently.
	*/
	if (RwV3dDotProduct(&pSphere->center, &m_planeCilp.GetNormal()) +
		m_planeCilp.GetConstant() < -pSphere->radius)
	{
		return FALSE;
	}
	
	// Left
	if (RwV3dDotProduct(&pSphere->center, &pCamera->frustumPlanes[2].plane.normal) -
		pCamera->frustumPlanes[2].plane.distance > pSphere->radius)
	{
		return FALSE;
	}

	// Right
	if (RwV3dDotProduct(&pSphere->center, &pCamera->frustumPlanes[4].plane.normal) -
		pCamera->frustumPlanes[4].plane.distance > pSphere->radius)
	{
		return FALSE;
	}
 
	// Near
 	if (RwV3dDotProduct(&pSphere->center, &pCamera->frustumPlanes[1].plane.normal) -
	  	pCamera->frustumPlanes[1].plane.distance > pSphere->radius)
	{
  		return FALSE;
	}
 
 	// Top
 	if (RwV3dDotProduct(&pSphere->center, &pCamera->frustumPlanes[3].plane.normal) -
 		pCamera->frustumPlanes[3].plane.distance > pSphere->radius)
 	{
 		return FALSE;
 	}
 
 	// Bottom
 	if (RwV3dDotProduct(&pSphere->center, &pCamera->frustumPlanes[5].plane.normal) -
 		pCamera->frustumPlanes[5].plane.distance > pSphere->radius)
 	{
 		return FALSE;
 	}

	return TRUE;
}

RwBool CNtlPLPlant::PlantFrustumTestSector(const CNtlWorldSector* pSector)
{
	RwV3d*	pvCameara		= RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwReal	fSectorHalfSize	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);

	if ((pSector->DatumPoint.x - pvCameara->x) * (pSector->DatumPoint.x - pvCameara->x) +
		(pSector->DatumPoint.z - pvCameara->z) * (pSector->DatumPoint.z - pvCameara->z) - fSectorHalfSize * fSectorHalfSize > m_fClipDistance * m_fClipDistance)
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlPLPlant::Render()
{
	NTL_FUNCTION("CNtlPLPlant::Render");
	NTL_RETURN(TRUE);
}

RwBool CNtlPLPlant::RenderSector(CNtlWorldSector* pSector)
{
	NTL_FUNCTION("CNtlPLPlant::RenderSector");

#ifdef dNTL_WORLD_TOOL_MODE
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PLNAT))
	{
		NTL_RETURN(TRUE);
	}
#endif

  	SECTOR_MAP_IT itSector = m_mapSector.find(pSector);
	if (itSector == m_mapSector.end())
	{
		NTL_RETURN(TRUE);
	}
	/*
	*All PLANT OBJs decided to use the same RenderState
	*Ignore the flag set in the existing Atomic and set the RenderState externally.
	*/
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0x90);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

 	RwD3D9SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	RwD3D9SetClipPlane(0, &m_planeCilp.v4d);

	RENDER_OBJ_MAP& mapRenderObj = itSector->second;

	RENDER_OBJ_MAP_IT itRender		= mapRenderObj.begin();
	RENDER_OBJ_MAP_IT itRenderEnd	= mapRenderObj.end();

	if (itRender == itRenderEnd)
	{
		NTL_RETURN(TRUE);
	}

	do 
	{
		RES_MAP_IT itRes = m_mapResource.find(itRender->first);
		if (itRes == m_mapResource.end())
		{
			continue;
		}

		TRANSFORM_VEC_IT itTransform	= itRender->second.begin();
		TRANSFORM_VEC_IT itTransformEnd = itRender->second.end();

		if (itTransform == itTransformEnd)
		{
			continue;
		}

		CNtlPLResource* pRes	= itRes->second.pPLResource;
		RpClump*		pClump	= pRes->GetClump();
		RwFrame*		pFrame	= RpClumpGetFrame(pClump);
		
		do 
		{
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC*			pvecTransform	= *itTransform;
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC::iterator itMat			= pvecTransform->begin();
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC::iterator itMatEnd		= pvecTransform->end();

			if (itMat == itMatEnd)
			{
				continue;
			}

			do 
			{
				RwFrameTransform(pFrame, &*itMat, rwCOMBINEREPLACE);
				RpClumpRender(pClump);
			} while(++itMat != itMatEnd);
		} while(++itTransform != itTransformEnd);
	} while(++itRender != itRenderEnd);

	RwD3D9SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);

	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATER);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPlant::SetProperty(const CNtlPLProperty *pProperty)
{
	NTL_FUNCTION("CNtlPLPlant::SetProperty");

	m_pProperty		= reinterpret_cast<CNtlPLPlantProperty*>(const_cast<CNtlPLProperty*>(pProperty));
	m_fClipDistance	= m_pProperty->GetClipDistance();

	NTL_RETURN(TRUE);
}

/*
*Operates when the sector is loaded.
*Terrain sector vertex information must exist before this method is executed.
*Load resources (directly) based on sector information.
*/
void CNtlPLPlant::OnLoad(CNtlWorldSector* pSector)
{
	for (int i = 0; i < (int)pSector->m_vecPlantSet.size(); ++i)
	{
		LoadResourceSet(pSector->m_vecPlantSet.at(i)->iSetIdx);
		for (int j = 0; j < (int)pSector->m_vecPlantSet.at(i)->vecPlantObj.size(); ++j)
		{
			OnGenerator(pSector, pSector->m_vecPlantSet.at(i), pSector->m_vecPlantSet.at(i)->vecPlantObj.at(j));
		}
	}
}

/*
*Operates when a sector is unloaded.
*Delete resources based on sector information.
*/
void CNtlPLPlant::OnUnload(CNtlWorldSector* pSector)
{
	for (int i = 0; i < (int)pSector->m_vecPlantSet.size(); ++i)
	{
		UnloadResourceSet(pSector->m_vecPlantSet.at(i)->iSetIdx);
	}

	SECTOR_MAP_IT it = m_mapSector.find(pSector);
	if (it != m_mapSector.end())
	{
		m_mapSector.erase(it);
	}
}

/*
*Since the sector information only has the tile index value, here in this method
*Constructs a matrix.
*/
void CNtlPLPlant::OnGenerator(CNtlWorldSector* pSector, sSECTOR_PLANT_SET_ATTR* pPlantSet, sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj)
{
	CNtlPLPlantProperty::PROPERTY_MAP_IT itProp = m_pProperty->m_mapProperty.find(pPlantSet->iSetIdx);
	DBO_ASSERT(itProp != m_pProperty->m_mapProperty.end(), "Property find failed.");
	
	CNtlPLPlantProperty::sPLANT_PROP& sProperty = itProp->second;

	RwInt32		iObj	= sProperty.vecstrResourceName.size();
	RwInt32		iNum	= NtlRandomNumber(sProperty.iNumMin, sProperty.iNumMax);
	RwV3d		vScale;
	RwMatrix	matScale;

	CNtlMath::MathRwV3dAssign(&vScale, 1.0f, NtlRandomNumber(sProperty.fScaleMin, sProperty.fScaleMax), 1.0f);
	RwMatrixScale(&matScale, &vScale, rwCOMBINEREPLACE);

	for (int i = 0; i < (int)pPlantObj->vecObjMatrix.size(); ++i)
	{
		sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC* pvecTransform = pPlantObj->vecObjMatrix.at(i);
		NTL_DELETE(pvecTransform);
	}
	pPlantObj->vecObjMatrix.clear();
	for (int i = 0; i < iObj; ++i)
	{
		sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC* pvecTransform = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC;
		pPlantObj->vecObjMatrix.push_back(pvecTransform);
	}
	
	RwV3d	vSPos;
	RwReal	fSectorHalfSize = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	
	vSPos.x = pSector->DatumPoint.x - fSectorHalfSize + 
		(RwReal)(((pPlantObj->iTileIdx % dGET_WORLD_PARAM()->WorldSectorTileNum)) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	vSPos.y = 0.0f;
	vSPos.z = pSector->DatumPoint.z - fSectorHalfSize +
		(RwReal)(((pPlantObj->iTileIdx / dGET_WORLD_PARAM()->WorldSectorTileNum)) * dGET_WORLD_PARAM()->WorldSectorTileSize);

	for (int i = 0; i < iNum; ++i)
	{
		RwMatrix	matTransform;
		RwMatrix	matRotate;
		RwMatrix*	pmatRotate = &CNtlPLGlobal::m_matIden;
		RwV3d		vPos;

		vPos.x = vSPos.x + NtlRandomNumber(0.0f, (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize);
		vPos.y = 0.0f;
		vPos.z = vSPos.z + NtlRandomNumber(0.0f, (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize);
		
		if (!GetSceneManager()->GetWorldTerrainRotate(&matRotate, &vPos, TRUE))
		{
			continue;
		}

		// If the flag is checked to be perpendicular to the terrain, calculations are made based on this information.
		if (sProperty.bIsRotationTerrain)
		{
			pmatRotate = &matRotate;
		}

		RwMatrixMultiply(&matTransform, &matScale, pmatRotate);
		RwMatrixRotate(&matTransform, RwMatrixGetUp(pmatRotate), NtlRandomNumber(0.0f, 1.0f), rwCOMBINEPOSTCONCAT);
		RwMatrixTranslate(&matTransform, &vPos, rwCOMBINEPOSTCONCAT);			
		RwMatrixUpdate(&matTransform);

		pPlantObj->vecObjMatrix.at(NtlRandomNumber(0, iObj - 1))->push_back(matTransform);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLPlant::OnCreate(CNtlWorldSector* pSector, RwInt32 iTileIdx, RwInt32 iSetIdx)
{
	if (iSetIdx == -1)
	{
		return;
	}
	OnDelete(pSector, iTileIdx);

	sSECTOR_PLANT_SET_ATTR* pPlantSet = GetPlantSetAttrFromSector(pSector, iSetIdx);
	if (!pPlantSet)
	{
		pPlantSet = NTL_NEW sSECTOR_PLANT_SET_ATTR;
		pPlantSet->iSetIdx = iSetIdx;
		pSector->m_vecPlantSet.push_back(pPlantSet);
		LoadResourceSet(pPlantSet->iSetIdx);
	}
	
	sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR;
	pPlantObj->iTileIdx = iTileIdx;
	pPlantSet->vecPlantObj.push_back(pPlantObj);

	OnGenerator(pSector, pPlantSet, pPlantObj);
}
#endif

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLPlant::OnObjectCount(CNtlWorldSector* pSector, RwInt32 iCount)
{
	RwInt32 iCountObj	= 0;
	for (std::vector<sSECTOR_PLANT_SET_ATTR*>::iterator itSet = pSector->m_vecPlantSet.begin(); itSet != pSector->m_vecPlantSet.end(); ++itSet)
	{
		sSECTOR_PLANT_SET_ATTR* pPlantSet = *itSet;
		
		iCountObj += pPlantSet->vecPlantObj.size();
	}

	RwInt32 iNumDelete = iCountObj - iCount;
	for (int i = 0; i < iNumDelete; ++i)
	{
		RwInt32 iSet = NtlRandomNumber(0, pSector->m_vecPlantSet.size() - 1);
		RwInt32 iObj = NtlRandomNumber(0, pSector->m_vecPlantSet.at(iSet)->vecPlantObj.size() - 1);
		OnDelete(pSector, pSector->m_vecPlantSet.at(iSet)->vecPlantObj.at(iObj)->iTileIdx);
	}
}
#endif

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLPlant::OnDelete(CNtlWorldSector* pSector, RwInt32 iTileIdx)
{
	for (std::vector<sSECTOR_PLANT_SET_ATTR*>::iterator itSet = pSector->m_vecPlantSet.begin(); itSet != pSector->m_vecPlantSet.end(); ++itSet)
	{
		sSECTOR_PLANT_SET_ATTR* pPlantSet = *itSet;
		for (sSECTOR_PLANT_SET_ATTR::PLANT_OBJ_VEC_IT itObj = pPlantSet->vecPlantObj.begin(); itObj != pPlantSet->vecPlantObj.end(); ++itObj)
		{
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = *itObj;
			if (pPlantObj->iTileIdx == iTileIdx)
			{
				NTL_DELETE(pPlantObj);
				pPlantSet->vecPlantObj.erase(itObj);

				if (pPlantSet->vecPlantObj.empty())
				{
					UnloadResourceSet(pPlantSet->iSetIdx);
					NTL_DELETE(pPlantSet);
					pSector->m_vecPlantSet.erase(itSet);
				}
				return;
			}
		}
	}
}
#endif

sSECTOR_PLANT_SET_ATTR* CNtlPLPlant::GetPlantSetAttrFromSector(CNtlWorldSector* pSector, RwInt32 iSetIdx)
{
	if (pSector)
	{
		for (int i = 0; i < (int)pSector->m_vecPlantSet.size(); ++i)
		{
			sSECTOR_PLANT_SET_ATTR* pPlantSet = pSector->m_vecPlantSet.at(i);
			if (pPlantSet->iSetIdx == iSetIdx)
			{
				return pPlantSet;
			}
		}
	}

	return NULL;
}

sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* CNtlPLPlant::GetPlantObjFromPlantSet(sSECTOR_PLANT_SET_ATTR* pPlantSet, RwInt32 iTileIdx)
{
	if (pPlantSet)
	{
		for (int i = 0; i < (int)pPlantSet->vecPlantObj.size(); ++i)
		{
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = pPlantSet->vecPlantObj.at(i);
			if (pPlantObj->iTileIdx == iTileIdx)
			{
				return pPlantObj;
			}
		}
	}

	return NULL;
}

void CNtlPLPlant::LoadResourceSet(RwInt32 iSetIdx)
{
	CNtlPLPlantProperty::PROPERTY_MAP_IT itProp = m_pProperty->m_mapProperty.find(iSetIdx);
	DBO_ASSERT(itProp != m_pProperty->m_mapProperty.end(), "Property find failed.");

	CNtlPLPlantProperty::sPLANT_PROP& sProp = itProp->second;
	for (int i = 0; i < (int)sProp.vecstrResourceName.size(); ++i)
	{
		sNTLPLPlantResource& sPlantResource = m_mapResource[sProp.vecstrResourceName.at(i)];

		if (++sPlantResource.iRef == 1)
		{
			std::string strResPath(m_pProperty->GetResourceFilePath()); strResPath += sProp.vecstrResourceName.at(i);
			sPlantResource.pPLResource = CNtlPLResourceManager::GetInstance()->LoadClump(strResPath.c_str(), m_pProperty->GetTextureFilePath());
		}
		DBO_ASSERT(sPlantResource.pPLResource, "Resource load failed.");
		
	}
}

void CNtlPLPlant::UnloadResourceSet(RwInt32 iSetIdx)
{
	CNtlPLPlantProperty::PROPERTY_MAP_IT itProp = m_pProperty->m_mapProperty.find(iSetIdx);
	DBO_ASSERT(itProp != m_pProperty->m_mapProperty.end(), "Property find failed.");

	CNtlPLPlantProperty::sPLANT_PROP& sProp = itProp->second;
	for (int i = 0; i < (int)sProp.vecstrResourceName.size(); ++i)
	{
		sNTLPLPlantResource& sPlantResource = m_mapResource[sProp.vecstrResourceName.at(i)];
		if (--sPlantResource.iRef == 0)
		{
			CNtlPLResourceManager::GetInstance()->UnLoad(sPlantResource.pPLResource);
		}
		if (sPlantResource.iRef <= 0)
		{
			m_mapResource.erase(sProp.vecstrResourceName.at(i));
		}
	}
}