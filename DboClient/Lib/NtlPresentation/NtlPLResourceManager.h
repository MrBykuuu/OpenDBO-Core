/*****************************************************************************
 *
 * File			: NtlPLResourceManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 21	
 * Abstract		: Presentation layer resource manager class
 *****************************************************************************
 * Desc         : 
 * Adjust		: HuunSuk, Jang(2005. 7 29)
 *			        - Added renderware texture reseouce. 
 *			        - Addition of dictionary type to resource (interface for managing texture by group) 
 *
 *****************************************************************************/

#ifndef __NTL_PLRESOURCE_MANAGER_H__
#define __NTL_PLRESOURCE_MANAGER_H__

#include <rwcore.h>
#include <rpworld.h>
#include <string>
#include <map>
#include "NtlPLResource.h"

//RwBool TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize);

class CNtlPLEntity;
class CNtlResourceScheduleManager;

/**
* \ingroup NtlPresentation
* RenderWare resource manager class.
* World, clump, atomic, and texture are provided, and uva file will be reviewed for future provision.
* Note: The module has not yet been verified, so users should be careful.
* You should also consider whether to create clump and atomic resource containers separately.
* 
*/
class CNtlPLResourceManager
{
public:

	enum EResDictType	 
	{
		RES_TEX_DEFAULT_DICT,
		RES_TEX_TERRAIN_DICT,
		RES_TEX_OBJECT_DICT,
		RES_TEX_CHARACTER_DICT,
		RES_TEX_EFFECT_DICT,
		RES_TEX_UI_DICT,
	};

	// TEST
// public:
// 	void SetTextureLoadLevel(RwUInt32 eResDictType, RwUInt32 uiLevel);
// 
// 	struct STextureLoadLevelDirectory
// 	{
// 		std::string strDirectory;
// 		RwUInt32	uiLevelCur;
// 		RwUInt32	uiLevelMax;
// 	};
// 
// 	
// 	typedef std::map<RwUInt32, STextureLoadLevelDirectory>	TextureLoadDirectoryMap;
// 	TextureLoadDirectoryMap m_mapTextureLoadDirectroy;	
// 
// 	std::string m_strImagePath;
// 	RwInt32		m_iImagePathSize;
	//TEST : END

private:
	typedef std::map<std::string, CNtlPLResource*> ResourceMap;
	typedef std::multimap<std::string, CNtlPLResource*> ResourceMultiMap;

	ResourceMap m_mapResTbl; /**< CNtlPlResource의 container*/

	ResourceMap m_mapWorld;
	ResourceMap m_mapClump;
	ResourceMap m_mapAtomic;
	ResourceMultiMap m_mmapCloneClump; 
	ResourceMultiMap m_mmapCloneAtomic;
	ResourceMap m_mapAnim;
	
	RwBool	m_bLoadScheduling;
	RwBool	m_bDeleteScheduling;

	CNtlResourceScheduleManager *m_pScheduleManager;

private:
	
	/**
    *Function that reads RenderWare data.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param uiType RednerWare resource type
	*  \param What if a resource such as file name exists in the bClone container? Should I create it as a clone?
	*  \return resource class pointer
	*/
	CNtlPLResource* LoadDefault(const char *pStrName, const char * pResourcePath, unsigned int uiType);

	/**
    *Function that creates a CNtlPLResource object.
    *
    *  \param pName Resource name is stored in CNtlPLResource.
	*  \param uiType Type of renderware data.
	*  \param bClone clone flag of renderware data.
	*  \param pData Pointer to renderware data.
	*  \return resource class pointer
    */
	CNtlPLResource* CreateResource(const char *pName, unsigned int uiType, bool bClone,	void *pData);

	/**
    *Function to register a CNtlPLResource object in a container.
    *
    *  \param pResource container Resource pointer to register
	*  \see UnRegisterResource
    */
	void RegisterResource(const CNtlPLResource *pResource);

	/**
    *Function that releases the CNtlPLResource object from the container.
    *
    *  \param pResource container Resource pointer to release
	*  \see RegisterResource
    */
	void UnRegisterResource(const CNtlPLResource *pResource);

	/**
    *A function that creates data corresponding to the renderware data type.
    *
    *  \param pSteam stream object pointer.
	*  \param iType renderware data type.
    */
	void* CreateRwData(RwStream *pSteam, unsigned int iType);

	/**
    *Create clone data corresponding to renderware data.
	*Compare types and apply appropriate renderware clone function.
    *
    *  \param iType renderware data type.
	*  \param pData renderware data to clone.
	*  \return renderware data pointer
	*  \see DestroyRwData
    */
	void* CreateCloneRwData(unsigned int iType, void *pData); 

	/**
    *Delete appropriately for the type corresponding to the renderware data.
    *
    *  \param iType renderware data type.
	*  \param pData renderware data to delete.
    */
	void  DestroyRwData(unsigned int iType, void *pData);

	/**
    *Function that creates a renderware world.
    *
	*  \param pStream world stream data
	*  \return RenderWare world pointer
	*  \see AtomicStreamRead
	*  \see ClumpStreamRead
    */
	RpWorld* WorldStreamRead (RwStream *pStream);
	
	/**
  *Function that creates a renderware clump.
    *
	*  \param pStream clump stream data
	*  \return RenderWare clump pointer
	*  \see WorldStreamRead
	*  \see AtomicStreamRead
    */
	RpClump* ClumpStreamRead (RwStream *pStream);

	/**
    *A function that creates a renderware atomic.
    *
	*  \param pStream atomic stream data
	*  \return RenderWare atomic pointer
	*  \see WorldStreamRead
	*  \see ClumpStreamRead
    */
	RpAtomic* AtomicStreamRead (RwStream *pStream);

	/**
    *Find CNtlPLResource by name in container.
    *
	*  \param pName name string
	*  \param uiType the resource type
	*  \param uiType the resource dictionary type
	*  \return the resource class pointer corresponding to pName
    */
	CNtlPLResource* Find(const char *pName, unsigned int uiType, EResDictType eDictType = RES_TEX_DEFAULT_DICT);

    /**
    *Function for debugging resources that have not been released when the application is terminated.
	*Enables debugging contents to be displayed as a file or output window.
    *
	*/
	void DebugResource(void);

public:

	CNtlPLResourceManager();

	/**
    *  singleton CNtlPLResourceManager interface 함수.
    *
	*/
	static CNtlPLResourceManager* GetInstance(void);

	/**
    * Initial function.
	*  \return true if initialization of the resource manager was successful, false if it failed.
    *
	*/
	bool Crate(void);

	/**
    *  Terminate function.
    *
	*/
	void Destroy(void);

	/**
    *  Update function.
    *
	*/
	void Update(RwReal fElapsed);

	/**
    *  RenderWare world data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \return Pointer resource class
	*  \see LoadClump	
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadWorld(const char *pStrName, const char * pResourcePath);

	/**
    *Function to read RenderWare clump data.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone Variable that determines whether to create a clump as a clone.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadClump(const char *pStrName, const char * pResourcePath);


	/**
    *Function to read RenderWare clump data. Loading while scheduling at certain times (for replacement of thread loading)
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone Variable that determines whether to create a clump as a clone.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	void LoadSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity);

	/**
    *Function to read RenderWare atomic data.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone Variable that determines whether to create an atomic as a clone.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadClump
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadAtomic(const char *pStrName, const char * pResourcePath);

	
	/**
    *Function that reads RenderWare animation data.
    *
	*  \param pStrName file name
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadClump
	*  \see LoadTexture
	*  \see LoadAtomic
	*  \see UnLoad
	*/
	CNtlPLResource* LoadAnimation(const char *pStrName);

	/**
    *Function that reads RenderWare texture data.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param eFilterMode The filtering mode of the texture. See renderware RwTextureFilterMode.
	*  \param EResDictType When managing texture by dividing it into groups, becomes the group ID.
	*The reason for managing by group is to speed up search.     
	*In general, you can think of groups such as object, character, terrain, texture, and UI.
	*  \return Pointer resource class
	*  \see UnLoadTexture
	*/
	RwTexture* LoadTexture(const char *pStrName, const char * pResourcePath, EResDictType eType = RES_TEX_DEFAULT_DICT, const char *pStrMaskName = NULL);

	/**
    *A function that forcibly creates a RenderWare texture.
    *
	*  \param pStrName file name.
	*  \param iWidth texture width.
	*  \param iHeight texture height.
	*  \param iDepth pixel depth.
	*  \param eFilterMode texture filter mode.
	*  \param eFormat raster format.
	*  \param eType texture dictionary type. (using EResDictType of CNtlPLResourceManager).
	*  \return Pointer renderware texture pointer
	*  \see LoadTexture
	*  \see UnLoadTexture
	*/

	RwTexture* CreateTexture(const char *pStrName, 
							RwInt32 iWidth, 
							RwInt32 iHeight,
							RwInt32 iDepth,
							RwRasterFormat eFormat, 
							EResDictType eType = RES_TEX_DEFAULT_DICT);

	/**
    * Unload resources.
    *
	*  \param pResource unload할 resource pointer
	*  \see LoadWorld	
	*  \see LoadClump	
	*  \see LoadAtomic
	*  \see LoadTexture
	*/
	void UnLoad(CNtlPLResource *pResource);

	
	/**
    *Delete the scheduling node corresponding to the entity registered in scheduling.
    *
	*/
	void UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource);

	/**
    *Unload the texture.
    *
	*  \param pResource resource pointer to unload
	*  \see LoadTexture	
	*/
	void UnLoadTexture(RwTexture *pTexture);

	// woody1019
	//RwBool SetAlphaValue(RwTexture *pTexture, RwUInt8 Value, RwV2d Pos);
	RwBool	SetColor(RwTexture* _pTexture, DWORD _FillColor);
	RwBool	SetColor(RwTexture *pTexture, RwReal OpaLvl, POINT& TexelPos);
	RwBool	SetColor(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaValueInc(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaValueDec(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaBits(RwTexture *pTexture, RwUInt8 Value, RwV2d Pos);
	RwBool	SaveAlphaBits(RwTexture *pTexture, FILE *pFile);
	RwBool	SaveTexture(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTexture(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTexture(RwTexture *pTexture, BYTE* pBuff);
	RwBool	LoadAlphaBits(RwTexture *pTexture, RwUInt8 *pPreparedBits);
	RwBool	LoadAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile, RwRGBA* pShdowClr);
	RwBool	SaveAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile);
	RwBool	SaveTextureShadowMap(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTextureShadowMap(RwTexture *pTexture, FILE *pFile);

	RwBool	CopyRaster(RwTexture *pSrcTex, RwTexture *pDstTex);


	void	SetLoadScheduling(RwBool bSchedule); 
	RwBool	IsLoadScheduling();

	void	SetDeleteScheduling(RwBool bSchedule);
	RwBool	IsDeleteScheduling();

	RwBool	IsEmptyLoadScheduling(void);

	RwReal	GetAlphaAverageDensity(RwTexture* _pTex);
	RwReal	GetAlphaAverageDensity(BYTE* _pAlphaBits, RwInt32 _Cnt);

	RwBool	TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize);
};


static CNtlPLResourceManager* GetNtlResourceManager(void)
{
	return CNtlPLResourceManager::GetInstance();
}

inline void	CNtlPLResourceManager::SetLoadScheduling(RwBool bSchedule)
{
	m_bLoadScheduling = bSchedule;
}

inline void	CNtlPLResourceManager::SetDeleteScheduling(RwBool bSchedule)
{
	m_bDeleteScheduling = bSchedule;
}

inline RwBool CNtlPLResourceManager::IsLoadScheduling()
{
	return m_bLoadScheduling;
}

#endif
