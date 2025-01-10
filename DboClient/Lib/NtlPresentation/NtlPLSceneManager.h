/*****************************************************************************
 *
 * File			: NtlPLSceneManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (ÁÖ)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager base class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLSCENEMANAGER_H__
#define __NTL_PLSCENEMANAGER_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLEntity.h"
#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPicking.h"

class	CNtlPLDojoContainer;

class	CNtlPLFog;
class	CNtlPLSky;
class	CNtlPLPlant;
class	CNtlPlLightEvent;
class   CNtlPLWorldEntity;
struct	sNPE_COLLISION_PARAM;

enum EActiveWorldType
{
	AW_NONE,
	AW_RWWORLD,
	AW_HEGITHFIELD
};

/**
 * \ingroup NtlPresentation
 *This is a scene manager abstraction class for presentation.
 *It is composed of functionally focused abstract functions.
 *Dbo client and editor inherit this class and redefine it.
 *
 */
class CNtlPLSceneManager
{
public:

	/**
    *Interface function called after the presentation scene manager is created.
	*  \return Returns true if successful, false if failed.
    *
    */
	virtual RwBool Create(void) = 0;

	/**
    *Interface function called before the presentation scene manager is destroyed.
    *
    */
	virtual void Destroy(void) = 0;


	/**
    * presentation scene manager update ÇÔ¼ö.
    *
    */
	virtual void Update(RwReal fElapsed) = 0;

	/**
    *Function for rendering entities in the UI before presentation scene manager update.
	*Render ware must not overlap camera updates.
	*Since the UI also uses camera texture, the main scene camera and update should not overlap.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed) = 0;


	/**
    *  presentation scene manager render function.
    *
    */
	virtual void Render(void) = 0;
	virtual void Render4RWWorld(void) = 0;

	/**
    *  presentation scene manager postrender functiom.
    *
    */
	virtual void PostRender(void) = 0;

	/**
    * presentation scene active on/off flag.
    *
    */
	virtual void SetThreadLoad(RwBool bThreadLoad) = 0;


	/**
    * reset gui manager
    *
    */
	virtual void ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight) = 0;

	/**
    *Factory interface function that creates an entity.
	*You should consider whether to create a separate entity factory class.
	*  \return If successful, returns the entity base pointer, and if failed, returns NULL.
	*  \see DeleteEntity
    *
    */
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL) = 0;

	/**
    *Factory interface function that deletes an entity.
	*You should consider whether to create a separate entity factory class.
	*  \param pEntity entity pointer to delete
	*  \see CreateEntity
    *
    */
	virtual void DeleteEntity(CNtlPLEntity *pEntity) = 0;

	/**
    *Interface function to add an entity to the scene manager.
	*  \param pEntity Entity pointer to be added to the scene manager.
	*  \see RemovePLEntity
    *
    */
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity) = 0;

	/**
    *Interface function to remove an entity from the scene manager.
	*  \param pEntity The entity pointer to be removed from the scene manager.
	*  \see AddPLEntity
    *
    */
	virtual void RemovePLEntity(CNtlPLEntity *pEntity) = 0;


	/**
    *Interface function that adds an entity to the update of the scene manager.
	*  \param pEntity Entity pointer to be added to the scene manager.
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity) = 0;

	/**
    *Interface function to remove an entity in scene manager update.
	*  \param pEntity The entity pointer to be removed from the scene manager.
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity) = 0;

	/**
    *Add entity to distance filtering manager.
	*  \param pEntity distance The entity pointer to be added to the filtering manager.
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity) = 0;

	/**
    *Delete the entity in the distance filtering manager.
	*  \param pEntity distance The entity pointer to be removed by the filtering manager.
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity) = 0;

	/**
    *Currently active world type (is it renderware world or height field?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void) = 0;

	/**
	*Is the world ready?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void) = 0;

	/**
    *Interface function that obtains the height of the world including the terrain and objects corresponding to the world position.
	*  \return Returns the terrain or object height value.
	*  \param pWorldPos RwV3d pointer corresponding to world position
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f) = 0;

	/**
    *Interface function that calculates the height of only the terrain corresponding to the world position.
	*  \return Returns the terrain height value.
	*  \param pWorldPos RwV3d pointer corresponding to world position
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight) = 0;

	/**
    *Interface function that detects the picked entity during mouse picking.
	*  \param iPosX screen x-coordinate of mouse
	*  \param iPosX screen y-coordinate of mouse
	*
    */
	virtual RwBool PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit) = 0;

	/**
	*Run Ray InterSection on the world's water and terrain.
	*/
	virtual RwBool PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo) = 0;
	virtual RwBool PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo) = 0;

	/**
    *Perform camera collision check and find new collision camera position.
	*\param1 pCameraPos Current camera position
	*\param2 pLookAt current camera look at position
	*\param2 new camera position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos) = 0;

	/**
	*Character conflict handling function.
	*/
// 	virtual RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam) = 0;
// 	virtual RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam) = 0;

	/**
    *Interface function to obtain RpWorld pointer.
	*
    */
	virtual RpWorld* GetWorldPtr(void) = 0;

	virtual void SetDistanceFilter(RwBool bEnable) = 0;

    virtual RwBool GetDistanceFilterEnable() = 0;

	/**
	*world attribute
	* \return Returns the attribute value of the world.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos) = 0;

	/**
	*world normal attribute
	* \return Returns the normal attribute value of the world.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos) = 0;

	/**
	*world special attribute (divided into general attributes and special attributes)
	* \return Returns the special attribute value of the world.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos) = 0;

	/**
	*world material attribute
	* \return Returns the material attribute value of the world.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos) = 0;

	/**
	*In general, check the normal attribute of the world and use it only when it is water.
	* \return The water height in the world.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos) = 0;



	/**
	*Obtains the Bloom effect power factor of the current location.
	* \return if TRUE, the value is valid, and if FALSE, it is invalid.
	*/
	virtual RwReal						GetActiveBloomFactor(void) = 0; 

	virtual VOID						SetActiveSky(CNtlPLSky* _pNtlPLSky) = 0;

	virtual	CNtlPLDojoContainer*		GetDojoContainer(void) = 0; 

	virtual CNtlPLSky*					GetSky(void) = 0;
	virtual CNtlPLFog*					GetFog(void) = 0;
	virtual CNtlPLPlant*				GetPlant(void) = 0;
	virtual CNtlPLWorldEntity*			GetWorld(void) = 0;

	// Checks whether it is possible to move to actual world coordinates.
	virtual RwBool	GetAvailablePos(RwV3d& _CurPos) = 0;

	// When you want to decide whether to use PVS for background rendering in real time in the game.
	virtual VOID	SetWorldPVSActivation(RwBool _Flag) = 0;

	// Obtain a rotation metric perpendicular to the terrain in real world coordinates.
	virtual RwBool	GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen = 1000.0f) = 0;

    // Set a listener object that listens to the sound. (If set to NULL, the sound is heard centered on the camera position)
    virtual VOID	SetSoundListener(CNtlPLEntity* pEntity) = 0;

	// Use when you want to get the shadow color of a specified location
	virtual RwBool	GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor) = 0;

	// Used when you want to know the calculated world color at a specified location.
	virtual RwBool	GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow) = 0;

    virtual void	SetEntityVisible(ENtlPLEntityType eLayer, RwBool bVisible) = 0;             ///< Turns rendering of the render layer on/off. (for testing)

	// PLENTITY_SUN Change Entity Visible.
	virtual void	SetVisiblePlanet(RwBool bVisible) = 0;

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime) = 0;

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime) = 0;
};

/**
*Scene manager¡¯s instance function.
*
*/
inline CNtlPLSceneManager* GetSceneManager(void)
{
	return CNtlPLSceneManagerFactory::GetInstance(); 
}

#endif