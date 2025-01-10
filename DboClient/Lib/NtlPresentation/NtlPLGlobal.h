/*****************************************************************************
 *
 * File			: NtlPLGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer global variable class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLGLOBAL_H__
#define __NTL_PLGLOBAL_H__

#include <rwcore.h>
#include <rpworld.h>

#include <string>

using std::string;

extern RwReal g_fCharScheduleTime;

/**
 * \ingroup NtlPresentation
 * Global data data structure.
 * renderware RpWorld and RwCamera are used in many places, so they can be referenced globally.
 *
 */

class CNtlPLEntity;
class CNtlPLGlobal
{
public:

	static RpWorld*					m_pRpWorld;						/* Currently active RpWorld pointer **/
	static RwCamera*				m_RwCamera;						/* Currently active RwCamera pointer **/

	// Indoor
	static RpWorldSector*			m_RpWorldSector;				/* Currently active RpWorldSector pointer **/

	static RwV3d					m_vZeroV3;						/* Initialized vector variable **/
	static RwMatrix					m_matIden;						/* Initialized matrix variable **/
	static RwV3d					m_vIdenV3;
	static RwV3d					m_vXAxisV3;
	static RwV3d					m_vYAxisV3;
	static RwV3d					m_vZAxisV3;
	static RwMatrix					m_matNameBox;					/* name box matrix variable */

	static std::string				m_strAppPath;
	static RwBool					m_bCollObjVisible;				/* collision object rendering on/off flag */
	static RwReal					m_fDistFiterFrequency;			/* visual manager distance filtering update frequency */
	static RwReal					m_fDistFiterObjFrequency;		/* object distance fade in/out filtering update frequency */
	static RwBool					m_bWorldTerrainVisible;			/* world terrain visible on/off */
	static RwBool					m_bWorldSkyVisible;				/* world Sky visible on/off */
	static RwBool					m_bDecalRender;					// Decal Render on/Off	 -Decal Test ¿ë
    static RwBool					m_bCharacterUpdateVisible;		///< Flag that updates only when the character is Visible (used in map tool)
	static RwReal					m_fLensFlareForDNRatio;			///< Multiplier of how much Lens Flare will be affected when D&N is activated
	
	
	// Cz : 08.1.31
	static CNtlPLEntity*			m_pWHEntity;					// Is the world height above the object? //Change to Entity instead of Bool type.
	
	
	static RwBool					m_UseTerrainPVSMode;			// Whether or not to use terrain PVS data
	static RwUInt32					m_uiRenderFlag;
	static RwBool					m_IsMinimapRendering;
	static RwUInt32					m_uiMinimapLayer;
	static std::vector<std::string> m_vecPathMinimapObject;			// List of objects to be removed when outputting the minimap. Use temporarily.

    static RwBool					m_bObjectRender;				///< Object Render presence flag

	static D3DMATRIX				m_mD3D9IdentityMatrix;
    static RwBool					m_bRenderScouter;				///< Scouter rendering presence flag
    static RwBool					m_bEntityNotCulling;            ///< Flag used when not culling entities

	static RwReal					m_fIndoorPlaneHeight;			///< Terrain plane height used by Indoor.

	static RwUInt16					m_uiRenderFrame;				///< RenderFrame used in Culling

	static RwUInt32					m_uiCullTestCnt;
	static RwUInt32					m_uiCullTestAtomicCnt;
	static RwUInt32					m_uiRenderAtomicCnt;

	// Test PVS Multi Layer
	static RwInt32					m_iCurrentPVSLayer;

	static RwBool					m_bItemDropDisplay;
   
public:

	/**
	*Initialize function.
	*  \see Terminate
	*/
	static void Init(void);

	/**
	*Terminate function.
	*  \seeInit
	*/
	static void Terminate(void);

	/**
	*Function for setting RpWorld.
	*  \param pRpWorld 
	*/
	static void SetActiveWorld(const RpWorld *pRpWorld);

	/**
	*Function for setting RwCamera.
	*  \param pRwCamera
	*/
	static void SetActiveCamera(const RwCamera *pRwCamera);

	/**
	*Load the Object List that should not be output when outputting Minimap in SABER.
	*This function and the m_strPathMiniMapObject variable may be deleted in the future depending on Minimap planning.
	*/
	static void	LoadMinimapPassObjectList(const RwChar* pFilename);
};


#endif