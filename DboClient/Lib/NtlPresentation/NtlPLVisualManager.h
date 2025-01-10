/*****************************************************************************
 *
 * File			: NtlPLSceneManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager base class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLVISUALMANAGER_H__
#define __NTL_PLVISUALMANAGER_H__

#include <map>
#include "rpcollis.h"
#include "ceventhandler.h"
#include "NtlPLSceneManager.h"
#include "NtlPLRenderGroup.h"
#include "NtlPLCollision.h"
#include "NtlPLCullingScheduling.h"

class CNtlPLRenderGroup;
class CNtlPLDistanceFilterGroup;
class CNtlPLAtomicSorterContainter;
class CNtlPLDojoContainer;

class CNtlPLDummyWorld;
class CNtlPLWorldEntity;
class CNtlPLSky;
class CNtlPLFog;
class CNtlPLWater;
class CNtlPLPlant;
class CNtlPicking;


/**
 * \ingroup NtlPresentation
 *This is a class that manages visual objects in client applications.
 *PL entities have their own layers, and update and render in layer order.
 *Since alpha sorting is not available in renderware, alpha sorting is performed using layers on an object basis.
 *It also provides an interface for creating and destroying PL entities, and creation/destruction management can be hidden. 
 *Manage creation/destruction using factory method.
 *By using the interface function to add/delete PL entity, PL entity object management can be hidden. 
 *Use STL associative array map.
 *
 */

class CNtlPLVisualManager : public CNtlPLSceneManager//, public RWS::CEventHandler
{
protected:

	EActiveWorldType m_ActiveWorldType;

	// Active
	RwBool m_bThreadLoad;

	// Prepare a container for sorting.
	CNtlPLAtomicSorterContainter *m_pSortContainer;

	// Dojo Data
	CNtlPLDojoContainer	*m_pDojoContainer;
	
	CNtlPLDummyWorld	*m_pRWWorldEntity;		
	CNtlPLWorldEntity	*m_pWorldEntity;		/** Reference variables to speed things up. */
	CNtlPLSky			*m_pSkyEntity;			/** Reference variables to speed things up. */
	CNtlPLFog			*m_pFogEntity;			/** Reference variables to speed things up. */	
	CNtlPLPlant			*m_pPlantEntity;		/** Reference variables to speed things up. */	
	CNtlPLWater			*m_pWaterEntity;
	CNtlPicking			*m_pPickingObj;			/** Rendering Picking **/
    CNtlPLEntity        *m_pSoundListener;      /** Listener object that listens to the sound */

	typedef std::map< RwInt32, CNtlPLRenderGroup*, std::less<RwUInt32> > MapRenderGroup;
	MapRenderGroup m_mapInstanceGroup;	/** Save the instance. */
	MapRenderGroup m_mapUpdateGroup;	/** Store the update entity separately. */
	MapRenderGroup m_mapRenderGroup;	/** Store rendering entities separately. */

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapReservedUpdate;
	MapReservedUpdate m_mapAddUpdate;
	MapReservedUpdate m_mapRemoveUpdate;
	
	// distance filtering
	RwBool m_bDistFiter;							/** object distance filtering on/off flag */    
	RwReal m_fDistFilterTime;						/** object distance filter time */
	RwReal m_fDistFilterDetectTime;					/** object distance filter detecting time */
	CNtlPLDistanceFilterGroup *m_pDistFiterGroup;	/** object distance filter group */

	// auto delete call
	RwUInt32	m_uiAutoDelAlarmFlags;

    // For Render Layer Visible Testing 
    std::map<RwUInt32, RwBool> m_mapRenderVisible;

	// Culling Scheduling
	CNtlPLCullingScheduling	m_CullScheduler;

protected:
		
	void RegisterInstanceGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterUpdateGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterRenderingGroup(const RwInt32 uiRenderLayer, CNtlPLRenderGroup *pRenderGroup);


	RwBool GetRWWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetHeightFieldWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetRWTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);

	RwBool GetHeightFieldTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	void UpdateDistanceFilter(RwReal fElapsed);		

	void UpdateSound(RwReal fElapsed);

	void ProcReservedAdd(void);

	void ProcReservedRemove(void);

	void AddReservedUpdate(CNtlPLEntity *pEntity);

	void RemoveReservedUpdate(CNtlPLEntity *pEntity);

	// auto delete marking flag
	void MarkAutoDeleteFlag(ENtlPLEntityType eType);

protected:

	virtual void AlarmAutoDelete(CNtlPLEntity *pEntity) {}

public:

	CNtlPLRenderGroup* FindInstanceGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindUpdateGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindRenderingGroup(const RwInt32 uiRenderLayer);
	
public:

	/**
    *  Default constructor for Presentation visual manager 
    *
    */
	CNtlPLVisualManager();

	/**
    *  Virtual destructor for Presentation vitual manager 
    *
    */
	~CNtlPLVisualManager();

	/**
	*A function called after the visual manager object is created.
	*  \see Destroy
	*/
	virtual RwBool Create(void);
	
	/**
	*A function called before visual manager is deleted.
	*  \see Create
	*/
	virtual void Destroy(void);

	/**
	* vitual manager update interface ÇÔ¼ö
	*  \param fElapsed update delta time
	*/
	virtual void Update(RwReal fElapsed);

	/**
    *Function for rendering entities in the UI before presentation scene manager update.
	*Render ware must not overlap camera updates.
	*Since the UI also uses camera texture, the main scene camera and update should not overlap.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed);

	/**
	*virtual manager render interface function
	*/
	virtual void Render(void);
	virtual void Render4RWWorld(void);

	/**
    *visual manager postrender function.
    *
    */
	virtual void PostRender(void);

	/**
    *  presentation scene active on/off flag.
    *
    */
	virtual void SetThreadLoad(RwBool bThreadLoad);

	/**
    *  reset gui manager
    *
    */
	virtual void ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight);

	/**
	*PL entity creation interface function.
	*  \param pEntityName is the entity name.
	*  \param pPropertyName is the property name to be applied to the entity.
	*  \return the created entity pointer
	*  \see DeleteEntity
	*/
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL);

	/**
	*PL entity destruction interface function.
	*  \param pEntity is a valid entity instance pointer.
	*  \see CreateEntity
	*/
	virtual void DeleteEntity(CNtlPLEntity *pEntity);

	/**
	*Interface function that adds a PL entity to an entity container.
	*  \param pEntity is a valid entity instance pointer.
	*  \return TRUE if successful, FALSE if failed
	*  \see RemovePLEntity
	*/
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity);

	/**
	*Interface function that removes the PL entity from the entity container.
	*  \param pEntity is a valid entity instance pointer.
	*  \see RemovePLEntity
	*/
	virtual void RemovePLEntity(CNtlPLEntity *pEntity);

	/**
    *Interface function that adds an entity to the update of the scene manager.
	*  \param pEntity Entity pointer to be added to the scene manager.
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity);

	/**
    *Interface function to remove an entity in scene manager update.
	*  \param pEntity The entity pointer to be removed from the scene manager.
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity);

	/**
    *Add entity to distance filtering manager.
	*  \param pEntity distance The entity pointer to be added to the filtering manager.
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *Delete the entity in the distance filtering manager.
	*  \param pEntity distance The entity pointer to be removed by the filtering manager.
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *Currently active world type (is it renderware world or height field?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void);

	/**
	*Is the world ready?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void);

	/**
    *Interface function that calculates the height of the terrain corresponding to the world position.
	*  \return Returns the terrain height value.
	*  \param pWorldPos RwV3d pointer corresponding to world position
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f);

	/**
    *Interface function that calculates the height of only the terrain corresponding to the world position.
	*  \return Returns the terrain height value.
	*  \param pWorldPos RwV3d pointer corresponding to world position
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	/**
    *Find the polygon picked in the world.
	*  \return The coordinates of the driven polygon.
    *
    */

	virtual RwBool PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	//RwBool PickWorld_Old(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);
	RwBool PickWorld_New(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	virtual RwBool PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);
	virtual RwBool PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);

	/**
    *Perform camera collision check and find new collision camera position.
	*\param1 pCameraPos Current camera position
	*\param2 pCameraDir Current camera direction
	*\param2 new camera position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

	/**
	*Interface function to obtain RpWorld pointer.
	*
	*/
	virtual RpWorld* GetWorldPtr(void);

	// woody1019
	// 	RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam);
    // 	RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam);

	/**
	*  visual manager event handler
	*  \param pMsg is the message sent when an event occurs.
	*/
	//virtual void HandleEvents(RWS::CMsg &pMsg);

    // Fade-related functions of Object
	virtual void	SetDistanceFilter(RwBool bEnable);        ///< Turns the fade function of the object off or on. (Used by Map Tool)
    virtual RwBool  GetDistanceFilterEnable() {return m_bDistFiter;}                        ///< Returns the current Fade On/Off status.

	/**
	* world attribute
	* \return Returns the attribute value of the world.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos);

	/**
	* world normal attribute
	* \return Returns the normal attribute value of the world.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos);

	/**
	* world special attribute (divided into general attributes and special attributes)
	* \return Returns the special attribute value of the world.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos);

	/**
	* world material attribute
	* \return Returns the material attribute value of the world.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos);

	/**
	*In general, check the normal attribute of the world and use it only when it is water.
	* \return The water height in the world.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos);

	/**
	*Obtains the Bloom effect power factor of the current location.
	* \return if TRUE, the value is valid, and if FALSE, it is invalid.
	*/
	virtual RwReal					GetActiveBloomFactor(void); 

	virtual VOID					SetActiveSky(CNtlPLSky* _pNtlPLSky) { m_pSkyEntity = _pNtlPLSky; }

	virtual CNtlPLDojoContainer*	GetDojoContainer(void)		{ return m_pDojoContainer; } 

	virtual CNtlPLDummyWorld*		GetRwWorld(void)			{ return m_pRWWorldEntity; }
	virtual CNtlPLWorldEntity*		GetWorld(void)				{ return m_pWorldEntity; }
	virtual CNtlPLSky*				GetSky(void)				{ return m_pSkyEntity; }
	virtual CNtlPLFog*				GetFog(void)				{ return m_pFogEntity; }
	virtual CNtlPLPlant*			GetPlant(void)				{ return m_pPlantEntity; }
	virtual CNtlPLWater*			GetWater(void)				{ return m_pWaterEntity; }
	virtual CNtlPicking*			GetPickingObject( void )	{ return m_pPickingObj; }

	virtual RwBool GetAvailablePos(RwV3d& _CurPos);

	virtual VOID SetWorldPVSActivation(RwBool _Flag);

	virtual RwBool GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen = 1000.0f);

    virtual VOID SetSoundListener(CNtlPLEntity* pEntity);

	virtual RwBool GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor);

	virtual RwBool GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow);

    virtual void   SetEntityVisible(ENtlPLEntityType eType, RwBool bVisible);             ///< Turns rendering of the render layer on/off. (for testing)

	virtual void	SetVisiblePlanet(RwBool bVisible);

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime);

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime);
};

#endif