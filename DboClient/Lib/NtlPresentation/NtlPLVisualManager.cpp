#include "precomp_ntlpresentation.h"
#include "NtlPLVisualManager.h"

// Core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlAtomic.h"

#include "NtlWorldInterface.h"

// Sound
#include "NtlSoundManager.h"

// Presentation
#include "NtlPLGlobal.h"
#include "NtlPLEntity.h"
#include "NtlPlEntityFactory.h"
#include "NtlPLRenderGroup.h"
#include "NtlPLDistanceFilterGroup.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLHelpers.h"
#include "NtlSysEvent.h"
#include "NtlPLObject.h"
#include "NtlPLCharacter.h"
#include "NtlPLGuiManager.h"
#include "NtlPLRenderState.h"
#include "NtlPLRenderCallback.h"
#include "NtlPLAtomicSorter.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLCollision.h"
#include "ntleffectsystemfreelist.h"
#include "NtlDNController.h"
#include "NtlPicking.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "ntlworldcommon.h"
#include "NtlPLGlobal.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"
#include "NtlPLBloom.h"
#include "NtlProfiler.h"

#include "NtlPLDojoContainer.h"
#include "NtlPLOptionManager.h"

#include <vector>
using std::vector;

CNtlPLVisualManager::CNtlPLVisualManager()
{
	m_bThreadLoad		= FALSE;
	m_pRWWorldEntity	= NULL;
	m_pWorldEntity		= NULL;
	m_pSkyEntity		= NULL;
	m_pFogEntity		= NULL;
	m_pWaterEntity		= NULL;
	m_pPlantEntity		= NULL;
	m_pPickingObj		= NULL;
	m_pSortContainer	= NULL;

	m_pDojoContainer	= NULL;

	m_bDistFiter		= TRUE;
	m_fDistFilterTime	= 0.0f;
    m_pDistFiterGroup	= NULL;    
    m_pSoundListener    = NULL;

	m_uiAutoDelAlarmFlags	= 0;

	m_ActiveWorldType = AW_HEGITHFIELD;
}

CNtlPLVisualManager::~CNtlPLVisualManager()
{
//	Destroy();
}

RwBool CNtlPLVisualManager::Create(void)
{
	NTL_FUNCTION("CNtlPLVisualManager::Crate");

	if(!CNtlPLGuiManager::GetInstance()->Create(1024,768))
	{
		NTL_RETURN(FALSE);
	}

	m_pSortContainer = NTL_NEW CNtlPLAtomicSorterContainter;
	if(!m_pSortContainer->Crete())
	{
		NTL_RETURN(FALSE);
	}

	GetDnController()->CreateInstance();
	//RegisterMsg (g_EventRenderTick, "g_EventRenderTickStr", "RwReal");
	//LinkMsg(g_EventRenderTick, 0);


	m_pDistFiterGroup = NTL_NEW CNtlPLDistanceFilterGroup;
	if(!m_pDistFiterGroup->Create())
	{
		NTL_RETURN( FALSE );
	}

	m_fDistFilterDetectTime = CNtlPLGlobal::m_fDistFiterFrequency;

	m_pPickingObj = NTL_NEW CNtlPicking;
	if(!m_pPickingObj->Create())
	{
		NTL_RETURN( FALSE );
	}

	m_pDojoContainer = NTL_NEW CNtlPLDojoContainer;
	if (!m_pDojoContainer->Create())
	{
		NTL_RETURN( FALSE );

	}

	NTL_RETURN(TRUE);
}


void CNtlPLVisualManager::Destroy(void)
{
	if ( m_pPickingObj )
	{
		m_pPickingObj->Delete();
		NTL_DELETE( m_pPickingObj );
	}

	// destroy distance filter group
	if(m_pDistFiterGroup)
	{
		m_pDistFiterGroup->Destroy();
		NTL_DELETE(m_pDistFiterGroup);
	}

	CNtlPLRenderGroup *pGroup;
	MapRenderGroup::iterator it;

	std::list<CNtlPLEntity*>::iterator the;
	std::list<CNtlPLEntity*> listEntities;

	// update group
	for(it = m_mapUpdateGroup.begin(); it != m_mapUpdateGroup.end(); ++it)
	{
		pGroup = (*it).second;
		pGroup->Gets(listEntities);
		for(the = listEntities.begin(); the != listEntities.end(); the++)
		{
			DeleteEntity(*the);
		}
		pGroup->Destroy();
		NTL_DELETE(pGroup);
		listEntities.clear();
	}
	m_mapUpdateGroup.clear();

	// rendering group
	for(it = m_mapRenderGroup.begin(); it != m_mapRenderGroup.end(); ++it)
	{
		pGroup = (*it).second;
		pGroup->Gets(listEntities);
		for(the = listEntities.begin(); the != listEntities.end(); the++)
		{
			DeleteEntity(*the);
		}
		pGroup->Destroy();
		NTL_DELETE(pGroup);
		listEntities.clear();
	}
	m_mapRenderGroup.clear();

	// instance group
	for(it = m_mapInstanceGroup.begin(); it != m_mapInstanceGroup.end(); ++it)
	{
		pGroup = (*it).second;
		pGroup->Gets(listEntities);
		for(the = listEntities.begin(); the != listEntities.end(); the++)
		{
			DeleteEntity(*the);
		}
		pGroup->Destroy();
		NTL_DELETE(pGroup);
		listEntities.clear();
	}
	m_mapInstanceGroup.clear();

	// DNController destroy
	GetDnController()->DeleteInstance();

	// gui destroy
	CNtlPLGuiManager::GetInstance()->Destroy(); 

	// sort container destroy
	if(m_pSortContainer)
	{
		m_pSortContainer->Destroy();
		NTL_DELETE(m_pSortContainer);
		m_pSortContainer = NULL;
	}

	if (m_pDojoContainer)
	{
		m_pDojoContainer->Destroy();
		NTL_DELETE(m_pDojoContainer);
		m_pDojoContainer = NULL;
	}
}

void CNtlPLVisualManager::UpdateDistanceFilter(RwReal fElapsed)	
{
	NTL_SPROFILE("CNtlPLVisualManager::UpdateDistanceFilter");

	if(!m_bDistFiter)
	{
		NTL_RPROFILE_VOID()
	}

	if(m_fDistFilterTime > m_fDistFilterDetectTime)
	{
		CNtlPLDistanceFilterGroup::MapSet::const_iterator it;
		const CNtlPLDistanceFilterGroup::MapSet *pSet = m_pDistFiterGroup->GetNextUpdateSet();
		if(pSet)
		{
			CNtlPLObject *pPLObject;

			for(it = pSet->begin(); it != pSet->end(); it++)
			{
				pPLObject = reinterpret_cast<CNtlPLObject*>( (*it).second );
				pPLObject->UpdateFadeSystem();
			}
		}

		m_fDistFilterTime = 0.0f;
	}
	
	m_fDistFilterTime += fElapsed;

	NTL_EPROFILE()
}

void CNtlPLVisualManager::UpdateSound(RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLVisualManager::UpdateDistanceFilter")

    // If there is a sound listener object, play the sound centered on the camera when not centered around the object
	if(m_pSoundListener && m_pSoundListener->IsSchedulingLoadingComplete())
    {
		// by daneos: disabled at and up to fix issue with sound when moving away from spawn

        RwV3d* pvPos = RwMatrixGetPos(&m_pSoundListener->GetMatrix());
        RwV3d* pvAt  = RwMatrixGetAt(&m_pSoundListener->GetMatrix());
        RwV3d* pvUp  = RwMatrixGetUp(&m_pSoundListener->GetMatrix());

		GetSoundManager()->SetListenerPosition(pvPos->x, pvPos->y, pvPos->z,
			pvAt->x, pvAt->y, pvAt->z,
			pvUp->x, pvUp->y, pvUp->z);
    }
	else
	{
		const RwV3d *pCameraPos = &(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.pos);
		const RwV3d *pCameraAt	= &(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.at);
		const RwV3d *pCameraUp	= &(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.up);

		GetSoundManager()->SetListenerPosition(	pCameraPos->x, pCameraPos->y, pCameraPos->z,
												pCameraAt->x, pCameraAt->y, pCameraAt->z,
												pCameraUp->x, pCameraUp->y, pCameraUp->z);
    }
	
	GetSoundManager()->Update(fElapsed);

	NTL_EPROFILE()
}

void CNtlPLVisualManager::ProcReservedAdd(void)
{
	NTL_SPROFILE("CNtlPLVisualManager::ProcReservedAdd")

	MapReservedUpdate::iterator it;
	for(it = m_mapAddUpdate.begin(); it != m_mapAddUpdate.end(); it++)
	{
		AddReservedUpdate((*it).second);
	}

	m_mapAddUpdate.clear();

	NTL_EPROFILE()
}

void CNtlPLVisualManager::ProcReservedRemove(void)
{
	NTL_SPROFILE("CNtlPLVisualManager::ProcReservedRemove")

	MapReservedUpdate::iterator it;
	for(it = m_mapRemoveUpdate.begin(); it != m_mapRemoveUpdate.end(); it++)
	{
		RemoveReservedUpdate((*it).second);
	}

	m_mapRemoveUpdate.clear();

	NTL_EPROFILE()
}

void CNtlPLVisualManager::AddReservedUpdate(CNtlPLEntity *pEntity)
{
	RwUInt32 uiClassType = pEntity->GetClassType();

	RwUInt32 uiFlags = pEntity->GetFlags();
	uiFlags = uiFlags & ~NTL_PLEFLAG_NOTUPDATE;
	pEntity->SetFlags(uiFlags);

	// update group register
	CNtlPLRenderGroup *pRenderGroup = FindUpdateGroup(uiClassType);

	if(!pRenderGroup)
	{
		pRenderGroup = NTL_NEW CNtlPLRenderGroup;
		RegisterUpdateGroup(uiClassType, pRenderGroup);
	}

	pRenderGroup->AddEntity(pEntity); 
}

void CNtlPLVisualManager::RemoveReservedUpdate(CNtlPLEntity *pEntity)
{
	RwUInt32 uiClassType = pEntity->GetClassType();

	// update remove
	CNtlPLRenderGroup *pRenderGroup = FindUpdateGroup(uiClassType);
	NTL_ASSERTE(pRenderGroup);

	// In AddReservedUpdate, remove the NTL_PLEFLAG_NOTUPDATE Flag.
	// The AddReservedUpdate Update status is maintained even once.
	// Find out if it will be a problem in the future. //Cz
	pRenderGroup->RemoveEntity(pEntity); 
}

void CNtlPLVisualManager::MarkAutoDeleteFlag(ENtlPLEntityType eType)
{
	m_uiAutoDelAlarmFlags = m_uiAutoDelAlarmFlags | (1 << (RwUInt8)eType);
}

void CNtlPLVisualManager::Update(RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLVisualManager::Update")

	// scheduling resource update
	GetNtlResourceManager()->Update(fElapsed);
	GetNtlPLOptionManager()->Update(CNtlPLGlobal::m_RwCamera, fElapsed);

	if(m_bThreadLoad)
	{
		NTL_RPROFILE_VOID()
	}

	// Distance filter processing.
	UpdateDistanceFilter(fElapsed);

	// update sound
	UpdateSound(fElapsed);

	// DN Control
	GetDnController()->Update( fElapsed );

	// Update
	CNtlPLRenderGroup *pGroup, *pInstanceGroup, *pRenderingGroup;
	CNtlPLEntity* pEntity;
	MapRenderGroup::iterator it;

	RwReal fWeightElapsed;

	for(it = m_mapUpdateGroup.begin(); it != m_mapUpdateGroup.end(); ++it)
	{
		pGroup = (*it).second;

		CNtlPLRenderGroup::MapEntity::iterator the;
		CNtlPLRenderGroup::MapEntity *pmapEntity = pGroup->GetEntities();
		for(the = pmapEntity->begin(); the != pmapEntity->end(); )
		{
			pEntity = (*the).second;

			fWeightElapsed = fElapsed;
			if(pEntity->GetFlags() & NTL_PLEFLAG_WEIGHT_ELAPSED_TIME)
				fWeightElapsed = pEntity->GetWeightElapsedTime()*fElapsed;

			if(!pEntity->Update(fWeightElapsed))
			{
				ENtlPLEntityType eType = pEntity->GetClassType();
				if(m_uiAutoDelAlarmFlags & (1 << (RwUInt8)eType))
					AlarmAutoDelete(pEntity);

				pInstanceGroup = FindInstanceGroup(pEntity->GetClassType());
				pInstanceGroup->RemoveEntity(pEntity); 

				pRenderingGroup = FindRenderingGroup(pEntity->GetLayer());
				if(pRenderingGroup)
					pRenderingGroup->RemoveEntity(pEntity); 

				pEntity->Destroy();
				CNtlPLEntityFactory::DeleteEntity(pEntity); 
				the = pmapEntity->erase(the);
			}
			else
			{
				++the;
			}
		}
	}

	ProcReservedRemove();

	ProcReservedAdd();

	// Gui
	CNtlPLGuiManager::GetInstance()->Update( fElapsed ); 

	NTL_EPROFILE()
}


void CNtlPLVisualManager::UpdateBeforeCamera(RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLVisualManager::UpdateBeforeCamera")

	if(m_bThreadLoad)
	{
		NTL_RPROFILE_VOID()
	}

	CNtlPLGuiManager::GetInstance()->UpdateBeforeCamera(fElapsed);

	NTL_EPROFILE()
}

void CNtlPLVisualManager::Render4RWWorld(void)
{
	NTL_SPROFILE("CNtlPLVisualManager::Render4RWWorld");

	if(RpPVSQuery(CNtlPLGlobal::m_pRpWorld))
	{
		if(!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING))
			CNtlPLGlobal::m_RpWorldSector = RpPVSSetViewPosition(CNtlPLGlobal::m_pRpWorld, RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))));

		RpWorldSectorCallBackRender SectorRenderCallBack = RpWorldGetSectorRenderCallBack(CNtlPLGlobal::m_pRpWorld);

		((CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr())->BeginRenderSector();

		RpWorldSetSectorRenderCallBack(CNtlPLGlobal::m_pRpWorld, NtlWorldSectorRenderCallBack_PVS);

		RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, NtlWorldSectorRenderCallback, NULL);

		RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, NtlWorldSectorAtomicCallback, NULL);

		RpWorldSetSectorRenderCallBack(CNtlPLGlobal::m_pRpWorld, SectorRenderCallBack);

		((CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr())->EndRenderSector();
	}

	NTL_EPROFILE()
}

void CNtlPLVisualManager::Render(void)
{
	NTL_SPROFILE("CNtlPLVisualManager::Render");

	if(m_bThreadLoad)
	{
		NTL_RPROFILE_VOID()
	}

	if( CNtlPLGlobal::m_uiRenderFlag & dFLAG_EXCEPT_GUI )
	{
		NTL_RPROFILE_VOID()
	}

#ifdef _DEBUG
	CNtlPLGlobal::m_uiCullTestCnt		= 0;
	CNtlPLGlobal::m_uiCullTestAtomicCnt	= 0;
	CNtlPLGlobal::m_uiRenderAtomicCnt	= 0;
#endif

	dGET_WORLD_PARAM()->WorldPolyCnt	= 0;

#ifdef dNTL_WORLD_CULLING_NEW

	// renderFrame update
	CNtlPLGlobal::m_uiRenderFrame	= RWSRCGLOBAL(renderFrame);

#else

	m_CullScheduler.Update(g_GetElapsedTime());

    // Entity Curling Check
// 	static RwReal	g_fCullTimeCur	= 0.0f;
// 	static RwReal	g_fCullTime		= 0.05f / 3.0f;
// 	static RwInt32	g_iCullMode		= 0;
// 
// 	g_fCullTimeCur -= g_GetElapsedTime();
// 	if (g_fCullTimeCur < 0.0f)
// 	{
// 		CNtlPLRenderGroup* pNtlPLRenderGroup = NULL;
// 
// 		// Visibility problems arise based on the Frustum Sector in the Object Culling Test.
// 		switch (g_iCullMode)
// 		{
// 		case 0:
// 			{
// 				pNtlPLRenderGroup = FindInstanceGroup(PLENTITY_OBJECT);
// 				break;
// 			}
// 		case 1:
// 			{
// 				pNtlPLRenderGroup = FindInstanceGroup(PLENTITY_CHARACTER);
// 				break;
// 			}
// 		case 2:
// 			{
// 				pNtlPLRenderGroup = FindInstanceGroup(PLENTITY_LAYER_EFFECT);
// 				break;
// 			}
// 		}
// 
// 		if (pNtlPLRenderGroup)
// 		{
// 			CNtlPLRenderGroup::MapEntity* pmapNtlPLRenderGroupEntity	= pNtlPLRenderGroup->GetEntities();
// 			CNtlPLRenderGroup::MapEntity::iterator it					= pmapNtlPLRenderGroupEntity->begin();
// 			CNtlPLRenderGroup::MapEntity::iterator itEnd				= pmapNtlPLRenderGroupEntity->end();
// 
// 			while (it != itEnd)
// 			{
// 				it->second->CullingTest(CNtlPLGlobal::m_RwCamera);
// 				++it;
// 			}
// 		}
// 		if (++g_iCullMode > 2)
// 		{
// 			g_iCullMode = 0;
// 		}
// 
// 		g_fCullTimeCur = g_fCullTime;
// 	}
#endif
		
	// atomic sorter reset
	m_pSortContainer->Reset();

	CNtlPLRenderGroup *pGroup;
	CNtlPLEntity* pEntity;
	MapRenderGroup::iterator it;

	RwFrame *cameraframe = RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
    RwV3d *camPos = RwMatrixGetPos(RwFrameGetLTM(cameraframe));

	for(it = m_mapRenderGroup.begin(); it != m_mapRenderGroup.end(); ++it)
	{	
		pGroup = (*it).second;
		if(pGroup->GetLayer() == PLENTITY_LAYER_RWWORLD)
		{
			switch (GetSceneManager()->GetActiveWorldType())
			{
			case AW_HEGITHFIELD:
				RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, NtlWorldSectorRenderCallback, NULL);
				RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, NtlWorldSectorAtomicCallback, NULL);
				break;
				// Indoor
			case AW_RWWORLD:
				Render4RWWorld();
				break;
			}
		}
		else
		{
			// begin render state setting
			RwUInt32 uiRenderLayer = pGroup->GetLayer();
			CNtlPLRenderGroup::MapEntity::iterator the;
			CNtlPLRenderGroup::MapEntity *pmapEntity = pGroup->GetEntities();
         
			if(uiRenderLayer == PLENTITY_LAYER_PLAYERNAME || uiRenderLayer == PLENTITY_LAYER_DAMAGEBOX ||
				uiRenderLayer == PLENTITY_LAYER_EFFECT || uiRenderLayer == PLENTITY_LAYER_SHORELINE || uiRenderLayer == PLENTITY_LAYER_WEATHER)
			{
				for(the = pmapEntity->begin(); the != pmapEntity->end(); ++the)
				{
					pEntity = (*the).second;
					if(pEntity->IsVisible())
					{
#ifdef dNTL_WORLD_TOOL_MODE
						if (CNtlPLGlobal::m_IsMinimapRendering && !(pEntity->GetMinimapLayer() & CNtlPLGlobal::m_uiMinimapLayer))
						{
							continue;
						}
#endif
						m_pSortContainer->Push(PLSORT_ENTITY, pEntity, camPos); 
					}				
				}
			}
			else
			{
				BeginRenderGroupLayer(uiRenderLayer);
				for(the = pmapEntity->begin(); the != pmapEntity->end(); ++the)
				{
					pEntity = (*the).second;

					if(pEntity->IsVisible())
					{
#ifdef dNTL_WORLD_TOOL_MODE
						if (CNtlPLGlobal::m_IsMinimapRendering && !(pEntity->GetMinimapLayer() & CNtlPLGlobal::m_uiMinimapLayer))
						{
							continue;
						}
#endif
						pEntity->Render();
					}
				}
				EndRenderGroupLayer(uiRenderLayer);

			}
		}
	}

	m_pSortContainer->DepthSort();
	m_pSortContainer->Render();

	NTL_EPROFILE()
}

void CNtlPLVisualManager::PostRender()
{
	NTL_SPROFILE("CNtlPLVisualManager::PostRender")

	if(m_bThreadLoad)
	{
		NTL_RPROFILE_VOID()
	}

	// Gui
	BeginGuiRenderState();
	CNtlPLGuiManager::GetInstance()->Render();
	CNtlPLGuiManager::GetInstance()->PostRender();
	EndGuiRenderState();

	NTL_EPROFILE()
}

void CNtlPLVisualManager::SetThreadLoad(RwBool bThreadLoad)
{
	m_bThreadLoad = bThreadLoad;
}

void CNtlPLVisualManager::ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight)
{
	// gui destroy
	CNtlPLGuiManager::GetInstance()->Destroy(); 

	CNtlPLGuiManager::GetInstance()->Create(iScreenWidth,iScreenHeight);
}

CNtlPLEntity* CNtlPLVisualManager::CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam /*= NULL*/)
{
	NTL_FUNCTION("CNtlPLVisualManager::CreateEntity");

	if( eType != PLENTITY_SKY &&
		eType != PLENTITY_FOG &&
		eType != PLENTITY_WORLD &&
		eType != PLENTITY_WATER &&
		eType != PLENTITY_PLANT &&
		eType != PLENTITY_WEATHER &&
		eType != PLENTITY_DUMMY_WORLD)
	{
		NTL_PRE(CNtlPLGlobal::m_pRpWorld);
	}

    CNtlPLEntity *pEntity = CNtlPLEntityFactory::CreateEntity(eType, pParam);

	if(pEntity)
	{
		RwUInt32 uiFlags = pEntity->GetFlags();
//		if( !(uiFlags & NTL_PLEFLAG_NOT_ADD_WORLD) )
//			NTL_PRE(CNtlPLGlobal::m_pRpWorld);

		if( !(uiFlags & NTL_PLEFLAG_NOT_PROPERTY_USED) )
		{
			CNtlPLProperty *pProperty = CNtlPLPropertyContainer::GetInstance()->GetProperty(eType, pKey);  
            if(pProperty)
            {
                pEntity->SetProperty(pProperty);
            }
            else
            {
				DBO_WARNING_MESSAGE("CNtlPLVisualManager::CreateEntity the property(.xml) find not(" << pKey << ") type " << eType << "");
                // An effect must have an unconditional property.
                if(eType == PLENTITY_EFFECT)
                {
                    NTL_DELETE(pEntity);
					return NULL;
                }
            }
			
		}

		pEntity->Create(pParam);

		pEntity->PostCreate(); 

		pEntity->SetName(pKey); 

		AddPLEntity(pEntity);

		pEntity->AddWorld();
	}

	NTL_RETURN(pEntity);
}

void CNtlPLVisualManager::DeleteEntity(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLVisualManager::DeleteEntity");

	NTL_PRE(pEntity);

	pEntity->Destroy();

	RemovePLEntity(pEntity);

	CNtlPLEntityFactory::DeleteEntity(pEntity); 
	
	NTL_RETURNVOID();
}

RwBool CNtlPLVisualManager::AddPLEntity(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLVisualManager::AddPLEntity");
	NTL_PRE(pEntity->IsValid());

	RwUInt32 uiClassType = pEntity->GetClassType();

	// instance group register
	CNtlPLRenderGroup *pRenderGroup = FindInstanceGroup(uiClassType);

	if(!pRenderGroup)
	{
		pRenderGroup = NTL_NEW CNtlPLRenderGroup;
		RegisterInstanceGroup(uiClassType, pRenderGroup);
	}

	if(uiClassType == PLENTITY_DUMMY_WORLD)
		m_pRWWorldEntity = reinterpret_cast<CNtlPLDummyWorld*>( pEntity );
	else if(uiClassType == PLENTITY_WORLD)
		m_pWorldEntity = reinterpret_cast<CNtlPLWorldEntity*>( pEntity );
	else if(uiClassType == PLENTITY_SKY)
		m_pSkyEntity = reinterpret_cast<CNtlPLSky*>( pEntity );
	else if(uiClassType == PLENTITY_FOG)
		m_pFogEntity = reinterpret_cast<CNtlPLFog*>( pEntity );
	else if(uiClassType == PLENTITY_WATER)
		m_pWaterEntity = reinterpret_cast<CNtlPLWater*>( pEntity );
	else if(uiClassType == PLENTITY_PLANT)
		m_pPlantEntity = reinterpret_cast<CNtlPLPlant*>( pEntity );
	else if(uiClassType == PLENTITY_OBJECT)
    {
		AddDistanceFilter(pEntity);

        // If it is an Event Object, an Event is sent to SL.
        CNtlPLObject* pObject = reinterpret_cast<CNtlPLObject*>(pEntity);
        if(pObject && pObject->GetProperty()->GetObjectType() == E_OBJECT_MILEPOST)
        {
            CNtlPLEventGenerator::CreateEventObject(TRUE, (void*)pObject);
        }
    }

	pRenderGroup->AddEntity(pEntity); 

	// update group register
	pRenderGroup = FindUpdateGroup(uiClassType);

	if(!pRenderGroup)
	{
		pRenderGroup = NTL_NEW CNtlPLRenderGroup;
		RegisterUpdateGroup(uiClassType, pRenderGroup);
	}

	RwUInt32 uiFlags = pEntity->GetFlags();
	if( !(uiFlags & NTL_PLEFLAG_NOTUPDATE) )
		pRenderGroup->AddEntity(pEntity); 

	// rendering group register
	RwUInt32 uiRenderingLayer = pEntity->GetLayer();
	if(uiRenderingLayer != PLENTITY_LAYER_INVALID)
	{
		pRenderGroup = FindRenderingGroup(uiRenderingLayer); 

		if(!pRenderGroup)
		{
			pRenderGroup = NTL_NEW CNtlPLRenderGroup;
			RegisterRenderingGroup(uiRenderingLayer, pRenderGroup);
		}

		pRenderGroup->AddEntity(pEntity); 
	}

	NTL_RETURN(TRUE);
}

void CNtlPLVisualManager::RemovePLEntity(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLVisualManager::RemovePLEntity");
	NTL_PRE(pEntity->IsValid());

	RwUInt32 uiClassType = pEntity->GetClassType();

	// data clear
	if(uiClassType == PLENTITY_DUMMY_WORLD)
		m_pRWWorldEntity = NULL;
	else if(uiClassType == PLENTITY_WORLD)
		m_pWorldEntity = NULL;
	else if(uiClassType == PLENTITY_SKY)
		m_pSkyEntity = NULL;
	else if(uiClassType == PLENTITY_FOG)
		m_pFogEntity = NULL;
	else if(uiClassType == PLENTITY_WATER)
		m_pWaterEntity = NULL;
	else if(uiClassType == PLENTITY_PLANT)
		m_pPlantEntity = NULL;
    else if(uiClassType == PLENTITY_OBJECT)
    {
        // If it is an event object, it drops the delete event on the simulation layer before deleting it (by agebreak)
        CNtlPLObject* pObject = reinterpret_cast<CNtlPLObject*>(pEntity);
        if(pObject->GetProperty()->GetObjectType() != E_OBJECT_NORMAL)
        {
            CNtlPLEventGenerator::CreateEventObject(FALSE, (void*)pEntity);
        }        
    }

	// instance remove
	CNtlPLRenderGroup *pRenderGroup = FindInstanceGroup(uiClassType);
	if(pRenderGroup)
		pRenderGroup->RemoveEntity(pEntity); 

	// update remove
	pRenderGroup = FindUpdateGroup(uiClassType);
	if(pRenderGroup)
		pRenderGroup->RemoveEntity(pEntity); 

	// rendering remove
	RwUInt32 uiRenderingLayer = pEntity->GetLayer();
	pRenderGroup = FindRenderingGroup(uiRenderingLayer);
	if(pRenderGroup)
		pRenderGroup->RemoveEntity(pEntity); 

	// remove from reserved
	MapReservedUpdate::iterator it;
	it = m_mapAddUpdate.find(pEntity);
	if(it != m_mapAddUpdate.end())
		m_mapAddUpdate.erase(it);

	it = m_mapRemoveUpdate.find(pEntity);
	if(it != m_mapRemoveUpdate.end())
		m_mapRemoveUpdate.erase(it);

	// Remove from distane filter group.
	if(m_pDistFiterGroup)
	{
		RemoveDistanceFilter(pEntity);
//		m_pDistFiterGroup->RemoveEntity(pEntity);
	}


	NTL_RETURNVOID();
}

void CNtlPLVisualManager::AddUpdate(CNtlPLEntity *pEntity)
{
	m_mapAddUpdate[pEntity] = pEntity;
}

void CNtlPLVisualManager::RemoveUpdate(CNtlPLEntity *pEntity)
{
	m_mapRemoveUpdate[pEntity] = pEntity;
}

void CNtlPLVisualManager::AddDistanceFilter(CNtlPLEntity *pEntity)
{
	m_pDistFiterGroup->AddEntity(pEntity);

	RwInt32 iSetCount = m_pDistFiterGroup->GetFilterSetCount();
	if(iSetCount > 0)
		m_fDistFilterDetectTime = CNtlPLGlobal::m_fDistFiterFrequency/(RwReal)iSetCount;
}

void CNtlPLVisualManager::RemoveDistanceFilter(CNtlPLEntity *pEntity)
{
	m_pDistFiterGroup->RemoveEntity(pEntity);

	RwInt32 iSetCount = m_pDistFiterGroup->GetFilterSetCount();
	if(iSetCount > 0)
		m_fDistFilterDetectTime = CNtlPLGlobal::m_fDistFiterFrequency/(RwReal)iSetCount;
}

EActiveWorldType CNtlPLVisualManager::GetActiveWorldType(void)
{
	/*
	우택
	if(m_pRWWorldEntity)
		return AW_RWWORLD;
	else if(m_pWorldEntity)
		return AW_HEGITHFIELD;
	else 
		return AW_NONE;
		*/
	if(m_pWorldEntity)
	{
		return m_pWorldEntity->GetActiveWorldType();
	}
	else
	{
		return AW_NONE;
	}
}

void CNtlPLVisualManager::RegisterInstanceGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup)
{
	m_mapInstanceGroup[uiClassType] = pRenderGroup;
}

void CNtlPLVisualManager::RegisterUpdateGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup)
{
	m_mapUpdateGroup[uiClassType] = pRenderGroup;
}

void CNtlPLVisualManager::RegisterRenderingGroup(const RwInt32 uiRenderLayer, CNtlPLRenderGroup *pRenderGroup)
{
	pRenderGroup->SetLayer(uiRenderLayer);
	m_mapRenderGroup[uiRenderLayer] = pRenderGroup;
}

CNtlPLRenderGroup* CNtlPLVisualManager::FindInstanceGroup(const RwInt32 uiClassType)
{
	MapRenderGroup::iterator it;
	it = m_mapInstanceGroup.find(uiClassType);
	if(it != m_mapInstanceGroup.end())
		return (*it).second;

	return NULL;
}

CNtlPLRenderGroup* CNtlPLVisualManager::FindUpdateGroup(const RwInt32 uiClassType)
{
	MapRenderGroup::iterator it;
	it = m_mapUpdateGroup.find(uiClassType);
	if(it != m_mapUpdateGroup.end())
		return (*it).second;

	return NULL;
}

CNtlPLRenderGroup* CNtlPLVisualManager::FindRenderingGroup(const RwInt32 uiRenderLayer)
{
	MapRenderGroup::iterator it;
	it = m_mapRenderGroup.find(uiRenderLayer);
	if(it != m_mapRenderGroup.end())
		return (*it).second;

	return NULL;
}


RwBool CNtlPLVisualManager::GetRWWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen)
{
	CNtlPLGlobal::m_pWHEntity = NULL;

	if(!GetWorld()->GetWorldReady())
	{
		fHeight = 0.0f;
		return FALSE;
	}

	// Get object line collision height.
	RwLine Line;
	RwV3dAssign(&Line.start, pWorldPos);
	RwV3dAssign(&Line.end, pWorldPos);
	Line.start.y += 2.0f;
	Line.end.y -= fLineLen;

	static RwBool bObjColl = FALSE;
	static SWorldIntersect sObjInter;
	bObjColl = Collision_WorldIntersectionLineTopDown(Line, sObjInter);
	if (bObjColl && (GetWorldAttribute(*pWorldPos) & dNMAP_MINI_INDOOR_TILE_FLAG))
	{
		CNtlPLEntity* pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
		if (pEntity && pEntity->GetClassType() == PLENTITY_OBJECT)
		{
			CNtlPLObject* pObject = (CNtlPLObject*)pEntity;
			if (pObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				pObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				fHeight = sObjInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sObjInter.vNormal);
				CNtlPLGlobal::m_pWHEntity = pEntity;

				return TRUE;
			}
		}
	}

	// height field get height
	static SWorldIntersect sHFInter;
	static RwBool bHFColl = FALSE;
	bHFColl = Collision_IndoorIntersectionLineTopDown(Line, sHFInter);

	if(bObjColl)
	{
		if(bHFColl)
		{
			if(sHFInter.vCollPos.y >= sObjInter.vCollPos.y)
			{
				fHeight = sHFInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sHFInter.vNormal);

			}
			else
			{
				fHeight = sObjInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sObjInter.vNormal);
				CNtlPLGlobal::m_pWHEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
			}
		}
		else
		{
			fHeight = sObjInter.vCollPos.y;
			if(pNormal)
				RwV3dAssign(pNormal, &sObjInter.vNormal);
			CNtlPLGlobal::m_pWHEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
		}
	}
	else
	{
		if(bHFColl)
		{
			fHeight = sHFInter.vCollPos.y;
			if(pNormal)
				RwV3dAssign(pNormal, &sHFInter.vNormal);
		}
		else
		{
			// If nothing conflicts, start it up again.
			Line.start.y += fLineLen; // existing 200.0f;
			bHFColl = Collision_IndoorIntersectionLineTopDown(Line, sHFInter);
			if(bHFColl)
			{
				fHeight = sHFInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sHFInter.vNormal);
			}
			else
			{
				GetRWTerrainHeight(pWorldPos, fHeight);
				//fHeight = pWorldPos->y;
				if(pNormal)
					RwV3dAssign(pNormal, &CNtlPLGlobal::m_vYAxisV3);
			}
		}
	}

	if (!bObjColl && !bHFColl)
	{
		DBO_TRACE(bObjColl || bHFColl, "GetWorldHeight : Failed : IN(" << pWorldPos->x << "," << pWorldPos->y << "," << pWorldPos->z << "), OUT_HEIGHT(" << fHeight << ")");
	}

	return TRUE;
}

RwBool CNtlPLVisualManager::GetHeightFieldWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen)
{
	CNtlPLGlobal::m_pWHEntity = NULL;

	if(!GetWorld()->GetWorldReady())
	{
		fHeight = 0.0f;
		return FALSE;
	}

	// Get object line collision height.
	RwLine Line;
	RwV3dAssign(&Line.start, pWorldPos);
	RwV3dAssign(&Line.end, pWorldPos);
	Line.start.y += 2.0f;
	Line.end.y -= fLineLen;

	static RwBool bObjColl = FALSE;
	static SWorldIntersect sObjInter;
	bObjColl = Collision_WorldIntersectionLineTopDown(Line, sObjInter);
	if (bObjColl && (GetWorldAttribute(*pWorldPos) & dNMAP_MINI_INDOOR_TILE_FLAG))
	{
		CNtlPLEntity* pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
		if (pEntity && pEntity->GetClassType() == PLENTITY_OBJECT)
		{
			CNtlPLObject* pObject = (CNtlPLObject*)pEntity;
			if (pObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				pObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				fHeight = sObjInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sObjInter.vNormal);
				CNtlPLGlobal::m_pWHEntity = pEntity;

				return TRUE;
			}
		}
	}

	// height field get height
	static SWorldIntersect sHFInter;
	static RwBool bHFColl = FALSE;
	bHFColl = Collision_HeightFieldIntersectionLineTopDown(Line, sHFInter);

	if(bObjColl)
	{
		if(bHFColl)
		{
			if(sHFInter.vCollPos.y >= sObjInter.vCollPos.y)
			{
				fHeight = sHFInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sHFInter.vNormal);
				
			}
			else
			{
				fHeight = sObjInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sObjInter.vNormal);
				CNtlPLGlobal::m_pWHEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
			}
		}
		else
		{
			fHeight = sObjInter.vCollPos.y;
			if(pNormal)
				RwV3dAssign(pNormal, &sObjInter.vNormal);
			CNtlPLGlobal::m_pWHEntity = (CNtlPLEntity*)RpNtlAtomicGetData(sObjInter.pAtomic);
		}
	}
	else
	{
		if(bHFColl)
		{
			fHeight = sHFInter.vCollPos.y;
			if(pNormal)
				RwV3dAssign(pNormal, &sHFInter.vNormal);
		}
		else
		{
			// If nothing crashes... Raise the start position and try again.
			Line.start.y += fLineLen; // existing 200.0f;
			bHFColl = Collision_HeightFieldIntersectionLineTopDown(Line, sHFInter);
			if(bHFColl)
			{
				fHeight = sHFInter.vCollPos.y;
				if(pNormal)
					RwV3dAssign(pNormal, &sHFInter.vNormal);
			}
			else
			{
				GetHeightFieldTerrainHeight(pWorldPos, fHeight);
				//fHeight = pWorldPos->y;
				if(pNormal)
					RwV3dAssign(pNormal, &CNtlPLGlobal::m_vYAxisV3);
			}
		}
	}

	if (!bObjColl && !bHFColl)
	{
		DBO_TRACE(bObjColl || bHFColl, "GetWorldHeight : Failed : IN(" << pWorldPos->x << "," << pWorldPos->y << "," << pWorldPos->z << "), OUT_HEIGHT(" << fHeight << ")");
	}

	return TRUE;
}

RwBool CNtlPLVisualManager::GetRWTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight)
{
	if(!GetWorld()->GetWorldReady())
	{
		fHeight = -999.0f;
		return FALSE;
	}

	// height field get height
	RwV3d vTmp;
	CNtlMath::MathRwV3dAssign(&vTmp, pWorldPos->x, pWorldPos->y, pWorldPos->z);
	fHeight = GetWorld()->GetWorldSectorHeight(vTmp);

	return TRUE;
}

RwBool CNtlPLVisualManager::GetHeightFieldTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight)
{
	if(!GetWorld()->GetWorldReady())
	{
		fHeight = -999.0f;
		return FALSE;
	}

	// height field get height
	RwV3d vTmp;
	CNtlMath::MathRwV3dAssign(&vTmp, pWorldPos->x, pWorldPos->y, pWorldPos->z);
	fHeight = GetWorld()->GetWorldSectorHeight(vTmp);

	return TRUE;
}

RwBool CNtlPLVisualManager::IsWorldReady(void)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
	{
		return FALSE;
	}

	if(GetWorld() && GetWorld()->GetWorldReady())
	{
		return TRUE;
	}

	return FALSE;
}

RwBool CNtlPLVisualManager::GetWorldHeight( const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen /*= 1000.0f*/)
{
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return FALSE;
	
	if(eWorldType == AW_RWWORLD)
		return GetRWWorldHeight(pWorldPos, fHeight, pNormal, fLineLen);
	else
		return GetHeightFieldWorldHeight(pWorldPos, fHeight, pNormal, fLineLen);
}

/**
*Interface function that calculates the height of only the terrain corresponding to the world position.
*  \return Returns the terrain height value.
*  \param pWorldPos RwV3d pointer corresponding to world position
*
*/
RwBool CNtlPLVisualManager::GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight)
{
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return FALSE;

	if(eWorldType == AW_RWWORLD)
		return GetRWTerrainHeight(pWorldPos, fHeight);
	else
		return GetHeightFieldTerrainHeight(pWorldPos, fHeight);
}


RpWorld* CNtlPLVisualManager::GetWorldPtr(void)
{
	return CNtlPLGlobal::m_pRpWorld;
}


/**
*Find the polygon picked in the world.
*Pick the characters... find the distance between the picked characters and select the closest one,
*Compare the distance with the terrain again, and if the terrain is close, add sPickInfo.pPLEntity = GetTerrain(),
*If the character is close, just put sPickInfo.pPLEntity = CNtlPLCharacter.
*  \return The coordinates of the driven polygon.
*
*/

RwBool CNtlPLVisualManager::PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit)
{
//	return PickWorld_Old( iPosX, iPosY, sPickInfo, fPickTerrainLimit, fPickObjLimit );
	return PickWorld_New( iPosX, iPosY, sPickInfo, fPickTerrainLimit, fPickObjLimit );
}

RwBool CNtlPLVisualManager::PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo)
{
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return FALSE;
	
	sPickInfo.pPLEntity = NULL;
	if (GetWorld()->PickTerrain(vStartPos, vEndPos, sPickInfo.vPickPos))
	{
		sPickInfo.pPLEntity = GetWorld();
		return TRUE;
	}

	return FALSE;
}

RwBool CNtlPLVisualManager::PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo)
{
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return FALSE;

	sPickInfo.pPLEntity = NULL;
	if (GetWorld()->PickTerrainAndWater(vStartPos, vEndPos, sPickInfo.vPickPos))
	{
		sPickInfo.pPLEntity = GetWorld();
		return TRUE;
	}

	return FALSE;
}

/*
RwBool CNtlPLVisualManager::PickWorld_Old(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit)
{
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return FALSE;

aaaa
	sPickInfo.pPLEntity = NULL;

	// 지형을 제외하지 않으면.
	RwBool bPickTerrain = FALSE;
	RwV3d vTerrain;
	if(!sPickInfo.bTerrainExclusion)
	{
		if(eWorldType == AW_RWWORLD)
		{
		}
		else
		{
			GetWorld()->Pick(iPosX, iPosY, sPickInfo);
			if(sPickInfo.pPLEntity)
			{
				RwV3dAssign(&vTerrain, &sPickInfo.vPickPos);
				bPickTerrain = TRUE;
			}
		}
	}

	// object를 picking 한다.
	
	sPickInfo.pPLEntity = NULL;

	RwV2d vScreenPos;
	vScreenPos.x = (RwReal)iPosX;
	vScreenPos.y = (RwReal)iPosY;

	RwLine CameraRay;
	RwCameraCalcPixelRay( CNtlPLGlobal::m_RwCamera, &CameraRay, &vScreenPos );
	Pick_WorldIntersectionLine(CameraRay, sPickInfo, fPickObjLimit);

	// object에서 picking 된것이 존재하면.
	if(sPickInfo.pPLEntity)
	{
		if(bPickTerrain)
		{
			RwV3d vSub;
			RwV3dSubMacro(&vSub, &vTerrain, &CameraRay.start);
			RwReal fTerrainLen = CNtlMath::MathRwV3dSquaredLength(&vSub);
			RwV3dSubMacro(&vSub, &sPickInfo.vPickPos, &CameraRay.start);
			RwReal fObjLen = CNtlMath::MathRwV3dSquaredLength(&vSub);
			if(fTerrainLen < fObjLen)
			{
				sPickInfo.pPLEntity = GetWorld();
				RwV3dAssign(&sPickInfo.vPickPos, &vTerrain);
			}
		}
	}
	else
	{
		if(bPickTerrain)
		{
			RwV3d vSub;
			RwV3dSubMacro(&vSub, &vTerrain, &CameraRay.start);
			RwReal fTerrainLen = RwV3dLength(&vSub);
			if(fTerrainLen < fPickTerrainLimit)
			{
				sPickInfo.pPLEntity = GetWorld();
				RwV3dAssign(&sPickInfo.vPickPos, &vTerrain);
			}
		}
	}

	if(sPickInfo.pPLEntity)
		return TRUE;

	return FALSE;
}
*/
RwBool CNtlPLVisualManager::PickWorld_New(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit)
{
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Terrain Picking
	//
	//////////////////////////////////////////////////////////////////////////
	RwBool bPickTerrain = FALSE;
	RwV3d vTerrain;
	if(!sPickInfo.bTerrainExclusion)
	{
		sPickInfo.pPLEntity = NULL;

		if(GetWorld()->Pick(iPosX, iPosY, sPickInfo.vPickPos))
		{
			sPickInfo.pPLEntity = GetWorld();
			RwV3dAssign(&vTerrain, &sPickInfo.vPickPos);
			bPickTerrain = TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Atomic Picking
	//
	//////////////////////////////////////////////////////////////////////////
	
	RwV2d vScreenPos;
	vScreenPos.x = (RwReal)iPosX;
	vScreenPos.y = (RwReal)iPosY;

	sPickInfo.pPLEntity = NULL;
	m_pPickingObj->Pick(CNtlPLGlobal::m_RwCamera, CNtlPLGlobal::m_pRpWorld, vScreenPos, fPickObjLimit, sPickInfo);

	//////////////////////////////////////////////////////////////////////////
	//
	// Determine collisions
	//
	//////////////////////////////////////////////////////////////////////////
	RwLine CameraRay;
	RwCameraCalcPixelRay( CNtlPLGlobal::m_RwCamera, &CameraRay, &vScreenPos );

	if(sPickInfo.pPLEntity)
	{
		if(bPickTerrain)
		{
			RwV3d vSub;
			RwV3dSubMacro(&vSub, &vTerrain, &CameraRay.start);
			RwReal fTerrainLen = CNtlMath::MathRwV3dSquaredLength(&vSub);
			RwV3dSubMacro(&vSub, &sPickInfo.vPickPos, &CameraRay.start);
			RwReal fObjLen = CNtlMath::MathRwV3dSquaredLength(&vSub);
			if(fTerrainLen < fObjLen)
			{
				sPickInfo.pPLEntity = GetWorld();
				RwV3dAssign(&sPickInfo.vPickPos, &vTerrain);
			}
		}
	}
	else
	{
		if(bPickTerrain)
		{
			RwV3d vSub;
			RwV3dSubMacro(&vSub, &vTerrain, &CameraRay.start);
			RwReal fTerrainLen = RwV3dLength(&vSub);
			if(fTerrainLen < fPickTerrainLimit)
			{
				sPickInfo.pPLEntity = GetWorld();
				RwV3dAssign(&sPickInfo.vPickPos, &vTerrain);
			}
		}
	}

	if(sPickInfo.pPLEntity)
		return TRUE;

	return FALSE;
}

RwBool CNtlPLVisualManager::GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos)
{
	return Collision_WorldIntersectionCameraData(pCameraPos, pLookAt, fRadius, vNewPos);
}

/*
RwBool CNtlPLVisualManager::CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam)
{
	NTL_FUNCTION( "CNtlPLVisualManager::CTLine2Sphere" );

	RpIntersection Intersection;
	Intersection.type = rpINTERSECTLINE;
	::CopyMemory(&Intersection.t.line, _pLine, sizeof(RwLine));

	RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, CNtlPLCollision::WorldIntersection4AtomicCB_CTLine2Sphere, &sNPECollisionParam);

	if(sNPECollisionParam._vecNPECollisionParam.size())
	{
		NTL_RETURN(TRUE);
	}

	NTL_RETURN(FALSE);
}

RwBool CNtlPLVisualManager::CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam)
{
	NTL_FUNCTION("CNtlPLVisualManager::ChrCollisionTest");

	CNtlPLObject* pNtlPLObj	= static_cast<CNtlPLObject*>(sNPECollisionParam._pNtlPLEntity);
	NTL_PRE(pNtlPLObj);

	RpClump* pClump = pNtlPLObj->GetClump();
	NTL_PRE(pClump);

	if(pNtlPLObj->GetBoundingSphere())
	{
		NTL_RETURN(FALSE);
	}

	RpIntersection	Intersection;
	Intersection.type	= rpINTERSECTSPHERE;
	::CopyMemory(&Intersection.t.sphere, pNtlPLObj->GetBoundingSphere(), sizeof(RwSphere));
	
	RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, CNtlPLCollision::WorldIntersection4AtomicCB_CTChar2Poly, &sNPECollisionParam);

	if(sNPECollisionParam._vecNPECollisionParam.size())
	{
		NTL_RETURN(TRUE);
	}

	NTL_RETURN(FALSE);
}
*/
/**
 *Used to set the presence or absence of the Object Fade effect in the Map Tool.
 * \param bEnable Fade effect or not 
 */
void CNtlPLVisualManager::SetDistanceFilter(RwBool bEnable)
{
	m_bDistFiter = bEnable;    

    CNtlPLRenderGroup *pInstanceGroup = FindInstanceGroup(PLENTITY_OBJECT);
    if(pInstanceGroup)
    {
        CNtlPLRenderGroup::MapEntity::iterator the;
        CNtlPLRenderGroup::MapEntity *pmapEntity = pInstanceGroup->GetEntities();

        CNtlPLObject *pPLObject;

        for(the = pmapEntity->begin(); the != pmapEntity->end(); the++)
        {
            pPLObject = reinterpret_cast<CNtlPLObject*>( (*the).second );
            pPLObject->SetFadeEnable(bEnable);
        }
    }
}

DWORD CNtlPLVisualManager::GetWorldAttribute(RwV3d vPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return 0;

	return GetWorld()->GetMapTileProp(vPos);
}

DWORD CNtlPLVisualManager::GetWorldNormalAttribute(RwV3d vPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return 0;

	DWORD dwAttr = GetWorld()->GetMapTileProp(vPos);

	return dwAttr;
	//return (dwAttr >> 16);
}

DWORD CNtlPLVisualManager::GetWorldSpecialAttribute(RwV3d vPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();

	if(eWorldType == AW_NONE)
		return 0;

	DWORD dwAttr = GetWorld()->GetMapTileProp(vPos);
	//return (dwAttr & 0x0000ffff);
	return (dwAttr >> 16);
}

BYTE CNtlPLVisualManager::GetWorldMaterialAttribute(RwV3d vPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return 0;

	return GetWorld()->GetMaterialTileProp(vPos);
}

RwReal CNtlPLVisualManager::GetWorldWaterHeight(RwV3d vPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return -9999.0f;

	return GetWorld()->GetWaterHeight(vPos);
}


RwReal CNtlPLVisualManager::GetActiveBloomFactor(void) 
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return dMONO_POWER_DEFAULT;

	CNtlPLWorldEntity *pWorldEntity = GetWorld();
	if(pWorldEntity == NULL)
		return dMONO_POWER_DEFAULT;

	return GetWorld()->GetMonoFactor();
}

RwBool CNtlPLVisualManager::GetAvailablePos(RwV3d& _CurPos)
{
	// Indoor
	EActiveWorldType eWorldType = GetActiveWorldType();
	if(eWorldType == AW_NONE)
		return TRUE;

// #ifdef dNTL_WORLD_TOOL_MODE
// 	RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - dGET_WORLD_PARAM()->WorldSectorSize);
// 	RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + dGET_WORLD_PARAM()->WorldSectorSize);
// #else
// 	RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - 1);
// 	RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + 1);
// #endif

#ifdef dNTL_WORLD_TOOL_MODE
	RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - dGET_WORLD_PARAM()->WorldSectorSize);
	RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + dGET_WORLD_PARAM()->WorldSectorSize);
#else
	RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
	RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
#endif

	if(_CurPos.x > Max || _CurPos.x < Min || _CurPos.z > Max || _CurPos.z < Min)
	{
		return FALSE;
	}

	return TRUE;
}

VOID CNtlPLVisualManager::SetWorldPVSActivation(RwBool _Flag)
{
	// It is used with a different meaning in Indoor. Request for correction. As it is, it has no meaning in Indoor.

	CNtlPLGlobal::m_UseTerrainPVSMode = _Flag;
}

RwBool CNtlPLVisualManager::GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen)
{
// 	EActiveWorldType eWorldType = GetActiveWorldType();
// 	if(eWorldType != AW_HEGITHFIELD)
// 	{
// 		return FALSE;
// 	}

	RwV3d	vNormal;
	RwReal	fHeight;
	RwLine	Line;

	RwV3dAssign(&Line.start, pvPos);
	RwV3dAssign(&Line.end, pvPos);
	Line.start.y += 2.0f;
	Line.end.y -= fLineLen;

	static SWorldIntersect sHFInter;
	static RwBool bHFColl = FALSE;

	switch (GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		bHFColl = Collision_HeightFieldIntersectionLineTopDown(Line, sHFInter);
		break;
	case AW_RWWORLD:
		bHFColl = Collision_IndoorIntersectionLineTopDown(Line, sHFInter);
		break;
	}
	if(bHFColl)
	{
		fHeight = sHFInter.vCollPos.y;
		RwV3dAssign(&vNormal, &sHFInter.vNormal);
	}
	else
	{
		Line.start.y += 200.0f;
		switch (GetActiveWorldType())
		{
		case AW_HEGITHFIELD:
			bHFColl = Collision_HeightFieldIntersectionLineTopDown(Line, sHFInter);
			break;
		case AW_RWWORLD:
			bHFColl = Collision_IndoorIntersectionLineTopDown(Line, sHFInter);
			break;
		}
		if(bHFColl)
		{
			fHeight = sHFInter.vCollPos.y;
			RwV3dAssign(&vNormal, &sHFInter.vNormal);
		}
		else
		{
			fHeight = pvPos->y;
			RwV3dAssign(&vNormal, &CNtlPLGlobal::m_vYAxisV3);
		}
	}
	
	CNtlMath::MathGetRotationMatrix(pmatOut, &CNtlPLGlobal::m_vYAxisV3, &vNormal);		
	
	if (bPosHeightChange)
	{
		pvPos->y = fHeight;
	}

	return bHFColl;
}

VOID CNtlPLVisualManager::SetSoundListener( CNtlPLEntity* pEntity ) 
{
    m_pSoundListener = pEntity;
}

RwBool CNtlPLVisualManager::GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor)
{
	return GetWorld()->GetWorldShadowColor(vPos, pOutColor);
}

RwBool CNtlPLVisualManager::GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow)
{
	return GetWorld()->GetWorldLightColor(vPos, pOutColor, bCalcShadow);
}

void CNtlPLVisualManager::SetEntityVisible( ENtlPLEntityType eType, RwBool bVisible ) 
{
    m_mapRenderVisible[eType] = bVisible;
}

void CNtlPLVisualManager::SetVisiblePlanet(RwBool bVisible)
{
	GetWorld()->SetVisiblePlanet(bVisible);
}

void CNtlPLVisualManager::RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime)
{
	GetWorld()->GetWorldFieldMgr()->GetNtlPLBloom()->RunableLightning(fBloomShaderFadeInTime, fBloomShaderFadeOutTime);
}

void CNtlPLVisualManager::RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime)
{
	GetSky()->RunableLightning(fSkyFadeInTime, fSkyFadeOutTime);
}


/*

void CNtlPLVisualManager::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlPLVisualManager::HandleEvents");

	if(pMsg.Id == g_EventRenderTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		Update(fElapsed);
		Render();
	}

	NTL_RETURNVOID();
}

*/