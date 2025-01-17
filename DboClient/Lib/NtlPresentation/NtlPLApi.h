/*****************************************************************************
 *
 * File			: NtlPLApi.h
 * Author		: All Author
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer api function.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLAPI_H__
#define __NTL_PLAPI_H__

#include <rwcore.h>
#include <rpworld.h>

#define DISPATCHSIZE (1000)

RwBool API_PLInit(void);
void API_PLTerminate(void);

#define API_PL_CLAMP(x, min, max) (x = (x < min ? min : x < max ? x : max))

#define E2I(val) *((int*)&val)					///< Change Enum type to Int type.


RpAtomic*	API_PL_LoadAtomic(const RwChar*	filename, const RwChar* pTexturePath);
RpClump*	API_PL_LoadClump(const RwChar* filename);

RtDict*		API_PL_LoadUVAnimDict(const RwChar* strFullName);

void		API_PL_PathnameDestroy(RwChar* buffer);
RwChar*		API_PL_PathnameCreate(const RwChar*	srcBuffer);

RwBool		API_PL_DeleteAtomic(RpAtomic*	pAtomic);
RwBool		API_PL_DeleteTexture(RwTexture* pTexture);

RwBool		API_PL_TranslationAtomic(RpAtomic*	pAtomic, RwV3d& Pos, RwOpCombineType OpCombineType);
RwBool		API_PL_RotationAtomic(RpAtomic*	pAtomic, RwReal Rot, RwOpCombineType OpCombineType);

RwBool		API_PL_AtomicSetTexture(RpAtomic* pAtomic, RwTexture* pTexture);
RwTexture*	API_PL_AtomicGetTexture(RpAtomic* pAtomic);
void		API_PL_AtomicSetGeoFlags(RpAtomic* pAtomic, RwUInt32);
RpMaterial*	API_PL_AtomicGetMaterial(RpAtomic* pAtomic);
void		API_PL_AtomicSetMaterialSetAlpha(RpAtomic* pAtomic, RwUInt8 AlphaValue);

void		API_PL_8BColorClamp(RwUInt8* pDst, RwInt32 Value);	// 8bits color clamping from the value
void		API_PL_1BColorClamp(RwReal* pDst, RwReal Value);	// 1.0f based color clamping from the value


void		API_PL_RasterSaveImage(const RwRaster *pRaster, const RwChar *pszFileName);    ///< Save Raster as Image (.png, .bmp)
void		API_PL_RasterSaveImage(const RwRaster *pRaster, const RwChar *pszFileName, RwInt32 _Width, RwInt32 _Height);    ///< Save Raster as Image (.png, .bmp)

void		API_PL_CameraLookAt( RwCamera* pCamera, 
								 const RwV3d* pSetCameraPos, 
								 const RwV3d* pTarget, 
								 const RwV3d* pSentInUp = NULL,
								 RwReal fAtAxisRot = 0.0f);   ///< Convert Camra Matrix with LookAt

void		API_PL_CameraMatrixLookAt( RwMatrix *pMat, 
									   const RwV3d* pSetCameraPos, 
									   const RwV3d* pTarget);  

RwV2d       API_PL_Calc3DPosTo2D( RwV3d* v3DPos, RwInt32 nWidth, RwInt32 nHeight, RwBool bOutSideReturn = FALSE); ///< Convert 3D coordinates to 2D coordinates

RpAtomic*	API_PL_RenderWireMesh(RpAtomic* atomic, void* data);				   ///< Display Atomic as Wireframe.


#endif
