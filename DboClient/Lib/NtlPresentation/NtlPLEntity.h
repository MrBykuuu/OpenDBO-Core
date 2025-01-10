/*****************************************************************************
 *
 * File			: NtlPLEntity.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation object entity base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_H__
#define __NTL_PLENTITY_H__

#define dNTL_WORLD_CULLING_NEW

#include <string>
#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"
#include "NtlPLGlobal.h"
#include "NtlPLProperty.h"

#define ENABLE_PLENTITY_FLAGS(flags,check)  (styles & check) 

typedef struct _SPLEntityCreateParam
{
	const RwV3d *pPos;
	RwReal		fAnimSpeed;
	RwReal		fAnimOffset;
	void*		_pUserData;

public:
	inline void operator = (_SPLEntityCreateParam& _EntityCreateParam)
	{
		pPos		= _EntityCreateParam.pPos;
		fAnimSpeed	= _EntityCreateParam.fAnimSpeed;
		fAnimOffset = _EntityCreateParam.fAnimOffset;

		// careful for shallow copy
		_pUserData	= _EntityCreateParam._pUserData;
	}	

	_SPLEntityCreateParam()
	{
		_pUserData = NULL;
	}

}SPLEntityCreateParam;

class CNtlPLResource;
class CNtlElapsedControllerGroup;
class CNtlPLEntityBlendController;


/**
 * \ingroup NtlPresentation
 * Presentation entity base class이다.
 * 
 */
class CNtlPLEntity
{	
protected:
#ifdef dNTL_WORLD_CULLING_NEW
	RwUInt16	m_uiRenderFrame;
#endif

	std::string m_strName;				/**< String describing the presentation entity name */
	RwUInt32 m_uiClassID;				/**< presentation entity class type */
	RwUInt32 m_uiLayer;					/**< presentation entity layer */
	RwUInt32 m_uiMinimapLayer;			/**< presentation entity minimap layer : is flags */
	RwUInt32 m_uiFlags;					/**< presentation entity flags */
	RwUInt32 m_uiSID;					/**< trigger entity id(0xffffffff for default; means triggers are not existed) */
	RwUInt32 m_uiSerialID;				/**< simulaion layer object serial id */
#ifndef dNTL_WORLD_CULLING_NEW
	RwUInt32 m_uiCullIID;				/**< Culling Id */
#endif
	DWORD	 m_ObjNameIdx;				// object name index; not available for client
	RwUInt16 m_byPickOrder;				/**< Picking order */

	RwUInt32 m_uiCullFlags;				/**< presentation entity Culling flags */

	RwReal m_fWeightAlpha;				/**< alpha weight value */
	RwReal m_fWeightElapsedTime;		/**< update weight elapsed time */
	RwReal m_fVisibleCullingDistance;	/**< Object Culling Distance */

	CNtlElapsedControllerGroup *m_pElapController;
	CNtlPLEntityBlendController *m_pBlendController;

public:

	/**
    *  Default constructor for Presentation entity 
    *
    */
	CNtlPLEntity(); 

	/**
    *  Virtual destructor for Presentation entity 
    *
    */
	virtual ~CNtlPLEntity();

	/**
	* Interface function called after the entity is created.
	*  \see Destroy
	*/
	virtual RwBool Create(const SPLEntityCreateParam *pParam = NULL) = 0;

	/**
	* An interface function that is executed after the entity's create function is called.
	*  \see Destroy
	*/
	virtual RwBool PostCreate(void) { return TRUE; }

	/**
	* Interface function called before the entity is deleted.
	*  \see Create
	*/
	virtual void Destroy(void) = 0;

	/**
	*  entity update interface 함수
	*  \param fElapsed update delta time
	*  \see Render
	*/
	virtual RwBool Update(RwReal fElapsed);

	/**
	* entity CullingTest interface 함수
	*  \see SetCull
	*  \see IsCulled
	*/
#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool	CullingTest(RwCamera* pRwCamera);	
#endif
	//virtual RwBool	CullingTest(RwCamera* pRwCamera, const RpAtomic* pAtomic);

    /************************************************************************/
    /* When you exit and delete the current update without deleting the Entity immediately*/
    /* Interface to call                                                  */
    /************************************************************************/
    virtual void   Finish() {}

	/**
    * Entity rendering interface functions
	*  \see Upate
	*/
	virtual RwBool Render(void) = 0;

	/**
	* Entity rendering texture interface functions
	*/
	virtual RwBool RenderToTexture(void) { return TRUE; }

	/**
	* entity property setting interface functions
	*  \param pData Property base pointer
	*/
	virtual RwBool SetProperty(const CNtlPLProperty *pData) = 0;

	/**
	*  Add renderware resource to world.
	*  \see RemoveWorld
	*/
	virtual void AddWorld(void)		{}

	/**
	*Delete the renderware resource from the world.
	*  \see AddWorld
	*/
	virtual void RemoveWorld(void)	{}

	/**
	*Notifies before creating a scheduled resource in the resource manager.
	*/
	virtual void CallPreSchedulingResource(void) {}

	/**
	*Notifies the resource scheduled by the resource manager.
	*/
	virtual void CallSchedulingResource(CNtlPLResource *pResource) {}
	virtual void CallSchedulingResourceOnly() {}
    virtual RwBool IsSchedulingLoadingComplete() {return TRUE;}

	/**
	* entity position interface function
	*  \param pPos entity position pointer.
	*  \see GetPosition
	*/
	virtual void SetPosition(const RwV3d *pPos) { }

	/**
	* entity direction interface function
	*  \param pDir entity dir pointer.
	*/
	virtual void SetDirection(const RwV3d *pDir) { }
    virtual RwV3d GetDirection() {RwV3d dir;dir.x = dir.y = 0.0f;dir.z = 1.0f; return dir;}

	/**
	*Interface function that returns entity position.
	*  \return Returns the entity position.
	*  \see SetPosition
	*/
	// by agebreak
	// Would not it be better to use the return type as a pointer or a reference?

	virtual RwV3d GetPosition(void) { return CNtlPLGlobal::m_vZeroV3; }

	/**
	* entity scale function.
	*  \param fScale entity scale value.
	*  \see GetScale
	*/
	virtual void SetScale(RwReal fScale) {}

	/**
	*Function to obtain entity scale value.
	*  \return fScale entity scale value.
	*  \see SetScale
	*/
	virtual RwReal GetScale(void) { return 1.0f; }

	/**
	*Function to obtain entity scale vector.
	*  \return entity scale vector value.
	*  \see SetScale
	*/
	virtual const RwV3d* GetScaleVector(void) const { return NULL; }

	/**
	* entity rotate interface function.
	*  \param fXAngle x-axis rotate.
	*  \param fXAngle y-axis rotate.
	*  \param fXAngle z-axis rotate.
	*/
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle) {;}

	/**
	*Function to obtain the rotated entity vector.
	*  \return entity rotate value.
	*  \see SetRotate
	*/
	virtual const RwV3d* GetRotate(void) const { return NULL;} 

	/**
	*Function to obtain the entity bounding sphere.
	*  \return bounding sphere pointer.
	*/
	virtual const RwSphere* GetBoundingSphere(void) { return NULL; }

	/**
	*Function to get the entity bounding box.
	*  \return bounding box pointer.
	*/
	virtual const RwBBox* GetBoundingBox(void) { return NULL; }

	/**
	*Function for setting entity matrix.
	*  \param matWorld RwMatrix variable.
	*/
	virtual void SetMatrix(RwMatrix& matWorld) {;}

	/**
	*Function for setting entity matrix.
	*  \return matrix 
	*/
	virtual RwMatrix& GetMatrix(void) { return CNtlPLGlobal::m_matIden; } 

	/**
	*Set entity alpha value.
	*/
	virtual void SetAlpha(RwUInt8 byValue) { }

	/**
	*Setting the entity color value.
	*/
	virtual void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) { }

	/**
	*Setting the entity Addcolor value.
	*/
	virtual void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) { }

	/**
	*Setting entity distance culling value.
	*/
	virtual void SetVisibleCullingDistance(RwReal fDistance);

	/** 
	* entity current distance culling value
	*  \return culling value
	*/
	virtual RwReal	GetVisibleCullingDistance(void) const { return m_fVisibleCullingDistance; }

	/**
	*Setting entity weight alpha value.
	*/
	virtual void SetWeightAlpha(RwReal fWeightValue);

	/** 
	* entity current weight alpha
	*  \return weight alpha
	*/
	RwReal GetWeightAlpha(void) const;

	/**
	*Setting atomic weight alpha value.
	*/
	virtual void SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue) {}

	/** 
	* entity current weight alpha
	*  \return weight alpha
	*/
	CNtlPLEntityBlendController* GetAlphaBlendController(void);

	/**
	*Setting the weight elapsed time value.
	*/
	void SetWeightElapsedTime(RwReal fWeightValue);

	/** 
	* entity current weight elapsed time
	*  \return weight elapsed time
	*/
	RwReal GetWeightElapsedTime(void) const;

	/** 
	*Create entity current weight elapsed controller.
	*/
	virtual void CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue); 

	/** 
	*Entity current weight elapsed controller disappears.
	*/
	virtual void DeleteWeightElapsedController(void);

	/** 
	*Function that turns entity rendering on/off.
	*  \param rendering on/off flag variable.
	*/
	virtual void	SetVisible(RwBool bVisible);	
	/** 
	*Is the entity currently being rendered?
	*  \param CullFlags to be operated on.
	*  \return TRUE if rendering is on, FALSE if rendering is off
	*/
	virtual RwBool	IsVisible(RwUInt32 uiFlags = 0xFFFFFFFF);

	/** 
	*Visible check function that exists for entity camera collision
	*Function that does not consider culling to improve performance
	*  \return TRUE if rendering is in progress, otherwise FALSE
	*/
	//virtual RwBool	IsVisibleForCameraCollision(void);

	/** 
	*Function that sets entity cullflags.
	*  \param cullflags check bit variable.
	*  \param cullflasgs on/off variable.
	*/
	virtual void	SetCullFlags(RwUInt32 uiFlag, RwBool bCulled);
	virtual void	SetCullFlags(RwUInt32 uiFlag);

	virtual RwUInt32 GetCullFlags();

	/** 
	*Function that turns entity picking on/off.
	*  \param picking on/off flag variable.
	*/
	virtual void SetPicking(RwBool bPicking);

	/** 
	*Entity picking order definition function.
	*  \param picking order variable.
	*/
	virtual RwUInt16 GetPickingOrder(void);

	virtual void SetPickingOrder(RwUInt16 byPickOrder);

	/** 
	*Function that turns the entity weight elapsed time flag on/off.
	*/
	virtual void EnableWeightElapsedTime(RwBool bEnable);
  
	/**
	* entity name setting
	*  \see GetName
	*/
	void SetName(const RwChar *pName);

	/**
	*Function to obtain entity name.
	*  \return Returns the entity name.
	*  \see SetName
	*/
	const RwChar* GetName(void); 

	/**
	*entity class id setting.
	*As a replacement for RTTI, the entity type is determined by storing the class id.
	*  \see GetClassID
	*/
	void SetClassType(const ENtlPLEntityType eType);

	/**
	*Function to obtain entity class id.
	*  \return Returns the class id.
	*  \see SetClassID
	*/
	ENtlPLEntityType GetClassType(void) const;

	/**
	*Setting entity layer
	*Using the entity layer, you can control the rendering order.
	*The entity layer is defined as an enum in NtlPLEntity.h, and can be added as needed.
	*  \see GetLayer
	*/
	void SetLayer(RwUInt32 wLayer);

	/**
	*Function to obtain the entity layer.
	*  \return The layer value of the entity.
	*  \see SetLayer
	*/
	RwUInt32 GetLayer(void) const;

	/**
	*Setting entity flag.
	*  \see GetFlags
	*/
	virtual void SetFlags(RwUInt32 uiFlags);

	/**
	*Get the entity flag.
	*  \see SetFlags
	*/
	RwUInt32 GetFlags(void);

	/**
	*Setting entity minimap layer
	*Uses entity minimap layer, currently only used for minimap layer output.
	*The entity minimap layer is defined as an enum in NtlPLEntity.h, and can be added as needed.
	*  \see GetMinimapLayer
	*/
	void SetMinimapLayer(RwUInt32 uiLayerFlags);

	/**
	*Function to obtain the entity layer.
	*  \return the entity's minimap layer flags value.
	*  \see SetMinimapLayer
	*/
	RwUInt32 GetMinimapLayer(void) const;

	/**
	*Set simulation object serial ID.
	*  \see GetSerialID
	*/
	void SetSerialID(RwUInt32 uiSerialID);

	/**
	*Obtain simulation object serial id.
	*  \see SetSerialID
	*/
	RwUInt32 GetSerialID(void);

	VOID		SetSID(RwUInt32 _uiSID);
	RwUInt32	GetSID(VOID);
	VOID		SetObjNameIdx(DWORD _Idx);
	DWORD		GetObjNameIdx();

#ifndef dNTL_WORLD_CULLING_NEW
	void		SetCullingID(RwUInt32 uiCullID);
	RwUInt32	GetCullingID(void);
#endif

	/**
	*Determine whether the layer, class id, and layer in the entity have valid data, and ensure that the entity contains accurate information.
	*A function that checks whether it exists.
	*  \return true if class id and layer contain valid information, false otherwise.
	*/
	RwBool IsValid(void) const;

	/**
	*Is this an entity that is automatically deleted at update time?
	*/
	virtual RwBool IsAutoDelete(void);


//-------------------------------------------------------------------------------
// event function
//-------------------------------------------------------------------------------

	// Function performed when summoning the Dragon God
	virtual void OnDragonSkyAppearence(RwBool _FlgDragonAppeared) {}

	/**
	*Occurs when an entity is clicked.
	*/
	virtual void	OnClicked(void) {}

	/**
	*Occurs when an entity is double clicked.
	*/
	virtual void	OnDbClicked(void) {}

	/**
	*Occurs when mouse focus is placed on an entity.
	*/
	virtual void	OnGotFocus(void) {}

	/**
	*Called when mouse focus is lost on an entity.
	*/
	virtual void	OnLostFocus(void) {}

//-------------------------------------------------------------------------------
// Interface to retrieve trigger information for World Editor (by agebreak 2006-11-28)
//-------------------------------------------------------------------------------
    virtual RwBBox  GetTriggerAABBInfo() {RwBBox bbbox; ZeroMemory(&bbbox, sizeof(RwBBox)); return bbbox;}                                  ///< Returns AABB information for use in world editor trigger information.
    virtual RwBBox  GetTriggerAABBInfo(const RwV3d& vPos, const RwV3d& vRotate, const RwV3d& vScale) {RwBBox bbbox; ZeroMemory(&bbbox, sizeof(RwBBox)); return bbbox;}

};

inline VOID	CNtlPLEntity::SetObjNameIdx(DWORD _Idx)
{
	m_ObjNameIdx = _Idx;
}

inline DWORD CNtlPLEntity::GetObjNameIdx()
{
	return m_ObjNameIdx;
}

#ifndef dNTL_WORLD_CULLING_NEW
inline void CNtlPLEntity::SetCullingID(RwUInt32 uiCullID)
{
	m_uiCullIID = uiCullID;
}

inline RwUInt32 CNtlPLEntity::GetCullingID(void)
{
	return m_uiCullIID;
}
#endif

inline void CNtlPLEntity::SetVisible(RwBool bVisible)
{
	if(bVisible)
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_NOT_VISIBLE);
	else
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_NOT_VISIBLE;
}

inline void CNtlPLEntity::SetCullFlags(RwUInt32 uiFlag, RwBool bCulled)
{
	if(bCulled)
		m_uiCullFlags = m_uiCullFlags | uiFlag;
	else
		m_uiCullFlags = m_uiCullFlags & (~uiFlag);
}

inline void CNtlPLEntity::SetCullFlags(RwUInt32 uiFlag)
{
	m_uiCullFlags = uiFlag;
}

inline RwUInt32 CNtlPLEntity::GetCullFlags()
{
	return m_uiCullFlags;
}

inline RwBool CNtlPLEntity::IsVisible(RwUInt32 uiFlags)
{
	// USER VISIBLE CHECK
	if ((m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE))
	{
		return FALSE;
	}

    if(CNtlPLGlobal::m_bEntityNotCulling)
	{
        return TRUE;
	}

#ifdef dNTL_WORLD_CULLING_NEW
	// If m_uiRenderFrame is equal to the initial value (-1), it is an entity that does not perform CullingTest.
	if (uiFlags)
	{
		uiFlags &= m_uiCullFlags;
		// PVS Flags are owned by every Entitiy. PVS Flags exist even if CullingTest is not performed.
		if (((uiFlags & NTL_PLEFLAG_CULLED_PVS) && CNtlPLGlobal::m_UseTerrainPVSMode))
		{
			return FALSE;
		}		
		// Except for PVS, the Flag should have the same RenderFrame but is up-to-date and not different.
		// However, if the RenderFrame is -1 and the FrameTest is not used, the Flag information has no value.
		if (m_uiRenderFrame == static_cast<RwUInt16>(-1))
		{
			return TRUE;
		}
		
		if ((CNtlPLGlobal::m_uiRenderFrame != m_uiRenderFrame) ||
			(uiFlags & (NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT | NTL_PLEFLAG_CULLED_OCCLUDER)))
		{
			return FALSE;
		}
	}
	return TRUE;
#endif

#ifndef dNTL_WORLD_CULLING_NEW
	// CULL VISIBLE CHECK
	if (uiFlags)
	{
		uiFlags &= m_uiCullFlags;
		if (((uiFlags & NTL_PLEFLAG_CULLED_PVS) && CNtlPLGlobal::m_UseTerrainPVSMode) ||
			(uiFlags & NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT) ||
			(uiFlags & NTL_PLEFLAG_CULLED_OCCLUDER))
		{
			return FALSE;
		}
	}

	return TRUE;
#endif
}

/*inline RwBool CNtlPLEntity::IsVisibleForCameraCollision(void)
{
	if ( m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE )
	{
		return FALSE;
	}
	return TRUE;
}*/

inline void CNtlPLEntity::SetPicking(RwBool bPicking)
{
	if(bPicking)
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_PICKING;
	else
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_PICKING);
}

inline RwUInt16 CNtlPLEntity::GetPickingOrder(void)
{
	return m_byPickOrder;
}

inline void CNtlPLEntity::SetPickingOrder(RwUInt16 byPickOrder)
{
	m_byPickOrder = byPickOrder;
}

inline void CNtlPLEntity::EnableWeightElapsedTime(RwBool bEnable)
{
	if(bEnable)
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_WEIGHT_ELAPSED_TIME;
	else
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_WEIGHT_ELAPSED_TIME);
}


inline void CNtlPLEntity::SetName(const RwChar *pName)
{
	if(pName)
	{
		m_strName = pName;
	}
}

inline const RwChar* CNtlPLEntity::GetName(void)
{
	return m_strName.c_str();
}


inline void CNtlPLEntity::SetClassType(const ENtlPLEntityType eType)
{
	m_uiClassID = eType;
}

inline ENtlPLEntityType CNtlPLEntity::GetClassType(void) const
{
	return (ENtlPLEntityType)m_uiClassID;
}

inline void CNtlPLEntity::SetLayer(RwUInt32 uiLayer)
{
	m_uiLayer = uiLayer;
}

inline RwUInt32 CNtlPLEntity::GetLayer(void) const
{
	return m_uiLayer;
}

inline void CNtlPLEntity::SetMinimapLayer(RwUInt32 uiLayerFlags)
{
	m_uiMinimapLayer = uiLayerFlags;
}

inline RwUInt32 CNtlPLEntity::GetMinimapLayer(void) const
{
	return m_uiMinimapLayer;
}


inline void CNtlPLEntity::SetFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

inline RwUInt32 CNtlPLEntity::GetFlags(void)
{
	return m_uiFlags;
}

inline void CNtlPLEntity::SetSerialID(RwUInt32 uiSerialID)
{
	m_uiSerialID = uiSerialID;
}

inline RwUInt32 CNtlPLEntity::GetSerialID(void)
{
	return m_uiSerialID;
}

inline void CNtlPLEntity::SetSID(RwUInt32 _uiSID)
{
	m_uiSID = _uiSID;
}

inline RwUInt32 CNtlPLEntity::GetSID(void)
{
	return m_uiSID;
}


inline RwReal CNtlPLEntity::GetWeightAlpha(void) const
{
	return m_fWeightAlpha;
}


inline void CNtlPLEntity::SetWeightElapsedTime(RwReal fWeightValue)
{
	m_fWeightElapsedTime = fWeightValue;
}


inline RwReal CNtlPLEntity::GetWeightElapsedTime(void) const
{
	return m_fWeightElapsedTime;
}

inline RwBool CNtlPLEntity::IsValid(void) const
{
	return (m_uiLayer == PLENTITY_LAYER_INVALID || m_uiClassID == PLENTITY_INVALID_TYPE)  ? FALSE : TRUE;
}


inline RwBool CNtlPLEntity::IsAutoDelete(void)
{
	return FALSE;
}

#endif
