#ifndef __NTL_PL_ENTITY_HELPERS_H__
#define __NTL_PL_ENTITY_HELPERS_H__

#include "rptoon.h"
#include "rphanim.h"

///< For Bone Data
typedef	std::map<std::string, RwFrame *>			FRAME_MAP;
typedef	std::map<std::string, RwFrame *>::iterator	FRAME_MAP_ITER;

#define DEFAULT_INK_THICKNESS		1.f
struct SToonData
{
	RpToonInk		*pToonInk;
	RpToonPaint		*pToonPaint;
	RwTexture		*pTexture;

	SToonData() : pToonInk(NULL), pToonPaint(NULL), pTexture(NULL) {}
};

struct SEntityAtomicData
{
	RpAtomic	*pData;
	RwBool		bClone;
	RwFrame		*pFrame;
	
	SEntityAtomicData() : pData(NULL), bClone(FALSE), pFrame(NULL) { }
};

typedef	std::vector< SEntityAtomicData *>				ENTITY_ATOMIC_VEC;
typedef	std::vector< SEntityAtomicData *>::iterator		ENTITY_ATOMIC_VEC_ITER;

struct SEntityMaterialData
{
	RpMaterial	*pMaterial;

	SEntityMaterialData() : pMaterial(NULL) { }
};

void Helper_SetToonClump(RpClump *pClump, SToonData *pToonData);					///< Set Toon Data.
void Helper_SetToonAtomic(RpAtomic *pAtomic, SToonData *pToonData);					///< Set Toon Data.
void Helper_SetToonAtomicRender(RpAtomic *pAtomic, SToonData *pToonData);			///< Set Toon Data (Ink, Paint) when rendering.
RwBool Helper_IsToonClump(RpClump* pClump);                                         ///< Check if the toon is applied to the clump.

RpAtomic* Helper_GetEnvMapAtomic(RpClump* pClump);                                  ///< Returns Atomic with environment map applied.


void Helper_RemoveClumpAllAtomics(RpClump *pClump);									///< Remove all Atomics in Clump.
void Helper_SetAtomicAllMaterialSkinInfo(RpAtomic *pAtomic);						///< If skin material exists, skin information in the material


void Helper_SetClumpAllAtomics( RpClump *pClump, 
							    ENTITY_ATOMIC_VEC *pVecAtomicList, 
								bool bClone = false);								///< Obtains Atomic List.
void Helper_DestroyAllAtomics( ENTITY_ATOMIC_VEC *pVecAtomicList);					///< Destroy the Atomic List.

void Helper_SetHierarchyClumpSkin(RpClump *pClump, RpHAnimHierarchy *pHierarchy);	///< Add Hierarchy to all Atomic Skins of Clump.
RpHAnimHierarchy *Helper_GetHierarchyClump(RpClump *pClump);						///< Obtain Clump¡¯s Hierarchy.

RwChar *Helper_BoneName(RwFrame *pFrame);											///< Return the name of the bone.
RwChar *Helper_AtomicName(RpAtomic *pAtomic);										///< Return the name of Atomic.
RwChar *Helper_MaterialName(RpMaterial *pMaterial);									///< Return the name of the material.

RwBool	Helper_GetBoneList(RpClump *pClump, FRAME_MAP *pFrame);						///< Get the Bone List.
RwBool	Helper_AtomicTestAlpha(RpAtomic *pAtomic);									///< Function to find out if Alpha is present in Atomic
RwBool	Helper_IsArmorType(RpClump *pClump);


void Helper_ClumpCalcBoundingSphere(RpClump *pClump, RwSphere *pSphere);			///< Obtain Clump¡¯s Bounding Sphere. (Object Space)
void Helper_ClumpBoundingSphereRender(RpClump *pClump, RwSphere *pSphere);
void Helper_ClumpWorldBoundingSphereRender(RwSphere *pSphere, RwUInt8 byR = 196, RwUInt8 byG = 196, RwUInt8 byB = 0, RwUInt8 byA = 255 );

void Helper_SetClumpAllAtomicsEMUV(RpClump *pClump, RwTexture* _pTexMap, RwMatrix* _pTexMat, RwBool _EMUVWorking);
void Helper_ResetClumpAllAtomicsEMUV(RpClump *pClump);

void Helper_SetClumpAllAtomicsSimpleMaterial(RpClump* pClump, RwBool _SimpleMaterial);

#endif