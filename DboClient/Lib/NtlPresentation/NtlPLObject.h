/*****************************************************************************
*
* File			: NtlPLObject.h
* Author		: HyungSuk Jang
* Copyright	    : NTL Co., Ltd.
* Date			: 2005. 8. 11	
* Abstract		: Presentation layer object entity class
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#ifndef __NTL_PLOBJECT_H__
#define __NTL_PLOBJECT_H__

// Commented out the fluorite for a moment.
// #define FADE_OBJECT_TIME 2.0f ///< Time it takes for an object to fade in/out 
#define FADE_EFFECT_GAP_TIME    1.0f                ///< Gap of how long to display the object after the effect is displayed

#include "NtlPLAttach.h"
#include "NtlPLGlobal.h"
#include "NtlAtomic.h"
#include "NtlPLResource.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLUVAnim.h"
#include "NtlPLLinkEffect.h"
#include "NtlAnimLayer.h"
#include "NtlPLEntityRenderHelpers.h"
#include "OccluderProxy.h"
#include "NtlPLObjectType.h"

// #include <vector>
// using std::vector;

typedef union
{
	RwTexCoords uv;
	RwUInt32    intUV[2];
}
LtMapIntUV_;

struct sSCHEDULING_LM_ATOM_PROP
{
	sSCHEDULING_LM_ATOM_PROP()
	{
		_pTexCoords = NULL;
	}

	~sSCHEDULING_LM_ATOM_PROP()
	{
		NTL_ARRAY_DELETE(_pTexCoords);
	}

	RwChar			_szAtomName[64];
	RwChar			_szTexName[64];
	RwInt32			_UVCnt;
	RwTexCoords*	_pTexCoords;
};

struct sSCHEDULING_LM_PROP
{
	RwUInt32	_uiLtMapObjectFlags;
	RwUInt32	_uiLtMapMaterialFlags;

	vector<sSCHEDULING_LM_ATOM_PROP*> _vecSchedulingLMAtomProp;
};

/// Structure containing information needed for scheduling loading
struct SObjectSchedulingInfo
{
    RwBool      bLoadComplete;
    RwUInt32    uiAnimKey;
    RwReal      fAnimStartTime;
    RwBool      bAnimLoop;

    SObjectSchedulingInfo()
    {
        bLoadComplete = FALSE;
        uiAnimKey = 0;
        fAnimStartTime = 0.0f;
        bAnimLoop = FALSE;
    }
};

class CNtlPLEntityAlphaWeightBlend;

struct SPLObjectCreateParam : public SPLEntityCreateParam
{
	RwBool bLoadMap;            // True when loading the map (when fade should not be applied), False otherwise.
};

struct SCollisionInfo;
struct RpHAnimHierarchy;
struct RwFrame;
struct SPLLightObjectCreateParam;

class CNtlPLResource;
class CNtlWorldShadow;

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CNtlPLObject : public CNtlPLAttach, public COccluderProxy
{
public:
	/// Flag value to be used in Fade System
	enum EObjectFadeState
	{
		FADE_VISIBLE,
		FADE_IN_OBJECT,
		FADE_IN_EFFECT,
		FADE_OUT_OBJECT,
		FADE_OUT_EFFECT,
		FADE_NOT_VISIBLE,
	};

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLObject();
	virtual ~CNtlPLObject();

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);

	virtual RwBool Render(void);
	virtual RwBool RenderToTexture();

	// Related to creating Occluder Proxy.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool CullingTest(RwCamera* pRwCamera);
#endif
	virtual RwBool  IsCullingTestAllAtomic();                                       ///< Returns whether the currently set animation is a Culling Atomic check.

	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual CNtlPLObjectProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);

	virtual void CallPreSchedulingResource(void);
	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	virtual void CallSchedulingResourceOnly();

	virtual void SetPosition(const RwV3d *pPos);
	virtual RwV3d GetPosition(void);

	virtual void SetScale( const RwV3d * pScale );
	virtual const RwV3d * GetScale( void ) const;
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	virtual const RwV3d* GetRotate(void) const;    
	virtual RwV3d GetDirection();

	virtual RwReal GetVisibleCullingDistance(void);

	virtual void  SetVisible(RwBool bVisible);													///< Sets the presence or absence of object expression.
	void SetEffectVisible(RwBool bVisible);														///< Set the presence or absence of effect expression.

	void SetAlpha(RwUInt8 byValue);																///< Sets the Alpha of the object.
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< Set the color of the object.
	RwRGBA	*GetColor() { return &m_sColor; }													///< Get the color of the object.

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	virtual RwFrame*	GetFrame(void);

	void				SetMatrix( RwMatrix & matWorld );
	RwMatrix&			GetMatrix(void);
	RwMatrix*			GetBoneMatrix(const RwChar *pBoneName);

	void				GetBoneName(std::vector<std::string>& vecBoneName);

	virtual RwSphere*	GetBoundingSphere();
	virtual void		CalcBoundingSphere();

	RwBool			IsActiveUpdate( void ) { return m_bActiveUpdate; }
	RwBool          IsSchedulingLoadingComplete() {return m_sScheduleInfo.bLoadComplete;}       ///< Schedule loading completed

	RpClump *GetClump( void ) const;

	RwBool          SetUVAnim(const RwChar* szUVAnimFileName);									///< Set UVAnim.
	CNtlPLUVAnim*   GetUVAnim() {return m_pUVAnim;}												///< Returns a UVAnim object.

	RwBool          SetAnimation(const RwChar* szAnimFileName);									///< Apply animation.
	virtual RwBool	SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);				///< Play Trigger Animation.
	virtual int		CallBackBaseAnim(void* pEventData);										///< CallBack function of Animation Event    
	void            SetAnimUpdate(RwBool bPlay) {m_bAnimPlay = bPlay;}								///< Apply animation play/stop.
	RwBool			GetPlayAnim() { return m_bAnimPlay; }										///< Can the animation play?
	RwBool			SetPlayAnimTime(RwReal fStartAnimTime);										///< Animation Play start time (0.f -1.f)
	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);										///< Animaiton Speed (1.f: Default )
	RwReal			GetBaseCurrentAnimTime();															///< Function to get the time of the current Base Layer Animation
	RwReal			GetAnimPlayTime(RwUInt32 uiAnimKey);										///< Returns the Play Time of the Anim corresponding to the Key.
	RwReal			GetBaseDurationAnimTime();													///< Returns the Play Time of Animation.
	void			SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< A function to force the time is also needed.

	RwBool			GetEnableShadow();															///< Should I make a Shadow?

	// Doodads PSM
	RwBool				GetEnableGenShadowMap();													///< Should I create a ShadowMap?
	RwBool				AreThereObjPSMap();
	VOID				DeleteDoodadShadow();
	ENTITY_ATOMIC_VEC*	GetAtomicList();															///< Do Atomic List Return.	
	VOID				SetAtomicPSMap();

	virtual const RwBBox* GetBoundingBox(void) {return &m_bbox;}								///< Returns Bounding Box.

	// Link Effect related
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< Returns LinkEffect Vector.
	CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Attach Link Effect.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);						///< Detach the Link Effect.

	// Functions for World Editor
	virtual RwBBox  GetTriggerAABBInfo();														///< Returns AABB information for use in world editor trigger information.
	virtual RwBBox  GetTriggerAABBInfo( const RwV3d& vPos, 
		const RwV3d& vRotate, 
		const RwV3d& vScale);	

	// Fade System related
	RwBool          UpdateFadeSystem();                                                         ///< Fade the object according to the set Culling Distance.
	void            SetFadeEnable(RwBool bEnable);                                              ///< Set the presence or absence of Fade Enable

	void    SetTransform(void);

	// Milepost related properties
	RwUInt32        GetMilepostID() {return m_uiMilepostTextID;}                                ///< Returns Milepost Text ID.
	void            SetMilepostID(RwUInt32 uiMilepostTextID) {m_uiMilepostTextID = uiMilepostTextID;} ///< Set Milepost Text ID.

	// Toon related
	void            CheckToonData();                                                            ///< Check if Toon can be applied.
	SToonData*      GetToonData() {return m_pToonData;}

	void		ResetUV();
	RwBool		CheckUVsSetUpForLightMapping();
	RwUInt32	GetObjectSerialID()								{ return m_uiObjectSerialID; }
	void		SetObjectSerialID(RwUInt32 uiObjectSerialID) { m_uiObjectSerialID = uiObjectSerialID; }
	void		DeleteLightmapFile();
	void		CheckLightmapFlags();

	// attach light
	virtual void		AttachLightObject();	

	RwBBox				GetDefaultAABB();

	// object type
	RwUInt32			GetObjectType();
	void				SetObjectType(RwUInt32 uiObjectType);
	CNtlPLObjectType*	GetObjectTypePtr() { return m_pObjectType; }

protected:

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	RwBool  CreateAnim();																		///< Creates Anim-related information and objects.
	RwBBox  CreateDefaultAABB();																///< Generates basic AABB information when there is no AABB information in the property.	
	RwBool  UpdateFading(RwReal fElapsedTime);                                                  ///< Fading.

	// Loop Effect related
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< Add LoopEffect to the LoopEffect List.
	void    ClearLoopEffect();                                                                 ///< Destroys EffectInstances in the LoopEffect List.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< Check whether there is a LoopEffect of the Name-Bone pair in the LoopEffect List.

	// Loop Sound Related
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< Add SoundHandle to LoopSound List.
	void    ClearLoopSound();                                                                  ///< Destroys LoopSounds in the LoopSound List.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< Check whether the same SoundHandle exists in the LoopSound List.
	void    UpdateLoopSoundPos();                                                              ///< Update LoopSound location.

	// Animation Event Related
	virtual void OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);
	virtual void OnEventVisualSound(SEventSound* pEventSound);									
	virtual void OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);    
	virtual void OnEventAlphaFade(SEventAlpha* pEventAlpha);
	virtual void OnEventTMQ(SEventAnimCinematic* pEventTMQ);
	virtual void OnEventExplosion(SEventExplosion* pEventExplosion);

	void    SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);               ///< Set Atomic Weight Alpha Value.

	void	AddSceneUpdate();
	void	RemoveSceneUpdate();

public:
	void			SetLightMapMaterialFlags(RwUInt32 uiFlags);
	void			SetLightMapObjectFlags(RwUInt32 uiFlags);
	RwUInt32		GetLightMapMaterialFlags();
	RwUInt32		GetLightMapObjectFlags();
	void			FreeSchedulingLTProp();
	void			AllocSchedulingLTProp();
	RwBool			IsRpWorldSectorList(RpWorldSector* pRpWorldSector);
	void			UpdateRpSectorOBBCheck();

	// attach dogi gym. emblem texture. careful for Destroy()
	RwBool			SetAtomicEmblemMap(RwTexture* _pEmblemMap);
	RwBool			GetEmblemMarkEnabled();
	void			DestroyEmblemMap();

	// Dojo	
	CNtlPLEntity*	GetDojo()							{ return m_pDojoEntity; }
	void			SetDojo(CNtlPLEntity* pDojoEntity)	{ m_pDojoEntity = pDojoEntity; }

public:
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

public:
	//static void					SetFarPlane(RwReal fFarDist);

protected:
	//static RwPlane				g_planeFar;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif

protected:
	sSCHEDULING_LM_PROP*		m_pSchedulingLMProp;
	SObjectSchedulingInfo       m_sScheduleInfo;        ///< Information about scheduling loading 
	RwUInt32					m_uiObjectSerialID;

	CNtlPLObjectType*			m_pObjectType;

	RwBool						m_bActiveUpdate;
	RwBool						m_bLoadMap;

	CNtlPLResource *			m_pClumpResource;
	ENTITY_ATOMIC_VEC	        m_vecAtomicList;         ///< List of Atomics that currently make up Clump
	FRAME_MAP				    m_mapFrame;				 ///< Bone Info

	RwV3d						m_vModelAngle;
	RwV3d						m_vModelScale;
	RwV3d						m_vWorldPosition;

	CNtlPLObjectProperty*       m_pProperty;            ///< object property object	

	CNtlPLUVAnim*               m_pUVAnim;              ///< UVAnim file object                
	RpHAnimHierarchy*           m_pBaseHierarchy;		///< Base Hierarchy
	CNtlPLResource*             m_pAnimResource;        ///< Anim Resource
	RwBool                      m_bAnimPlay;            ///< Availability of Anim Play
	RwBool						m_bHaveAnim;			///< Presence or absence of Anim.

	RwReal						m_fAnimSpeed;			///< Anim Speed

	RwRGBA						m_sColor;				///< Color				
	RwRGBA						m_sAddColor;			///< AddColor

	RwSphere					m_BSphere;				///< BoundSphere where position is not calculated
	RwSphere					m_BSphereCur;			///< BoundingSphere where Position was calculated: Updated when GetBoundingSphere is called.

	CNtlPLLinkEffect            m_LinkEffectInstance;   ///< Object that manages LinkEffect Instances

	RwBBox                      m_bbox;                 ///< Object의 Bounding Box

	// Fade System related
	EObjectFadeState            m_eFadeState;           ///< Fade State of current Object
	RwReal                      m_fPrevCameraDistance;  ///< Distance from previous camera
	RwReal                      m_fFadeTime;            ///< Fade Time    
	CNtlPLEntityAlphaWeightBlend* m_pFadeBlend;         ///< Alpha blending object for Fade Blend settings

	// Animation related to Trigger Object
	CNtlAnimLayer*				m_pAnimLayer;			///< Animation layer
	CNtlInstanceAnimTable*		m_pInstanceAnimTable;	///< Animation Resource Instance Table
	RwUInt32					m_uiCurAnimKey;			///< Currently applied AnimKey

	ListSoundHandle				m_listLoopSound;        ///< LoopSound’s HANDLE List
	ListLoopEffect				m_listLoopEffect;       ///< LoopEffect의 list

	// Terrain object shadow related
	vector<CNtlWorldShadow*>    m_vecNtlWorldShadow;

	RwUInt32                    m_uiMilepostTextID;      ///< ID of the text table to be connected when used as a milestone

	// Toon related
	SToonData*				    m_pToonData;			///< Toon Ink, Toon Paint, Toon Resource

	// Indoor
	CNtlOBB						m_OBB;					///< Used in indoor, to retrieve the RpWorldSector.
	vector<RpWorldSector*>		m_vecRpWorldSector;		///< used indoors, to retrieve the RpWorldSectorlist it contains

	// Dojo
	CNtlPLEntity*				m_pDojoEntity;

	// attach light
	struct SLightObjectAttachData
	{
		RwV3d						vPos;
		RwChar						chBoneName[MAX_ATTR_BONE_NAME];
		SPLLightObjectCreateParam*	pPLLightObjectCreateParam;
	};
	std::vector<SLightObjectAttachData*> m_vecAttachLightParam;

public:
	virtual RwBool		LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	virtual RwBool		SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	static	RwInt32		SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);

	static	BYTE*		SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*		SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	static	BYTE*		SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*		SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
	static	BYTE*		SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);

	// Projection Shadow
	virtual RwBool		SavePSMap(FILE* pFile);
	virtual RwBool		LoadPSMap(FILE* pFile);
	static RwInt32		SkipPSMap(FILE* pFile);

	static	BYTE*		SavePSMapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*		SkipPSMapToFileMem(BYTE* pFileMem);

	// lightmap related
	virtual RwBool		SaveLightmap(FILE* pFile);
	virtual RwBool		LoadLightmap(FILE* pFile);
	static  RwInt32		SkipLightmap(FILE* pFile);

	static  BYTE*		SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightmapToFileMem(BYTE* pFileMem);

	virtual RwBool		LoadSchedulingLightmapProp(FILE* pFile);

	// Attach LightObject
	virtual RwBool		LoadLightObjectAttachData(FILE* pFile);	
	virtual RwBool		SaveLightObjectAttachData(FILE* pFile);
	static  RwInt32		SkipLightObjectAttachData(FILE* pFile);

	static  BYTE*		SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightObjectAttachDataToFileMem(BYTE* pFileMem);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CNtlPLObject : public CNtlPLAttach, public COccluderProxy
{
public:
	/// Flag value to be used in Fade System
	enum EObjectFadeState
	{
		FADE_VISIBLE,
		FADE_IN_OBJECT,
		FADE_IN_EFFECT,
		FADE_OUT_OBJECT,
		FADE_OUT_EFFECT,
		FADE_NOT_VISIBLE,
	};

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLObject();
	virtual ~CNtlPLObject();

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);

	virtual RwBool Render(void);
	virtual RwBool RenderToTexture();

	// Related to creating Occluder Proxy.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool CullingTest(RwCamera* pRwCamera);
#endif
    virtual RwBool  IsCullingTestAllAtomic();                                       ///< Returns whether the currently set animation is a Culling Atomic check.

	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual CNtlPLObjectProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);

	virtual void CallPreSchedulingResource(void);
	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	virtual void CallSchedulingResourceOnly();

	virtual void SetPosition(const RwV3d *pPos);
	virtual RwV3d GetPosition(void);

	virtual void SetScale( const RwV3d * pScale );
	virtual const RwV3d * GetScale( void ) const;
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	virtual const RwV3d* GetRotate(void) const;    
	virtual RwV3d GetDirection();

	virtual RwReal GetVisibleCullingDistance(void);

	virtual void  SetVisible(RwBool bVisible);													///< Sets the presence or absence of object expression.
	void SetEffectVisible(RwBool bVisible);														///< Set the presence or absence of effect expression.

	void SetAlpha(RwUInt8 byValue);																///< Sets the Alpha of the object.
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< Set the color of the object.
	RwRGBA	*GetColor() { return &m_sColor; }													///< Get the color of the object.

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	virtual RwFrame*	GetFrame(void);

	void				SetMatrix( RwMatrix & matWorld );
	RwMatrix&			GetMatrix(void);
	RwMatrix*			GetBoneMatrix(const RwChar *pBoneName);

	void				GetBoneName(std::vector<std::string>& vecBoneName);

	virtual RwSphere*	GetBoundingSphere();
	virtual void		CalcBoundingSphere();

	RwBool			IsActiveUpdate( void ) { return m_bActiveUpdate; }
    RwBool          IsSchedulingLoadingComplete() {return m_sScheduleInfo.bLoadComplete;}       ///< Schedule loading completed

	RpClump *GetClump( void ) const;

	RwBool          SetUVAnim(const RwChar* szUVAnimFileName);									///< Set UVAnim.
	CNtlPLUVAnim*   GetUVAnim() {return m_pUVAnim;}												///< Returns a UVAnim object.

	RwBool          SetAnimation(const RwChar* szAnimFileName);									///< Apply animation.
	virtual RwBool	SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);				///< Play Trigger Animation.
	virtual int		CallBackBaseAnim(void* pEventData);										///< CallBack function of Animation Event    
	void            SetAnimUpdate(RwBool bPlay) {m_bAnimPlay = bPlay;}								///< Apply animation play/stop.
	RwBool			GetPlayAnim() { return m_bAnimPlay; }										///< Can the animation play?
	RwBool			SetPlayAnimTime(RwReal fStartAnimTime);										///< Animation Play start time (0.f -1.f)
	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);										///< Animaiton Speed (1.f: Default )
	RwReal			GetBaseCurrentAnimTime();															///< Function to get the time of the current Base Layer Animation
	RwReal			GetAnimPlayTime(RwUInt32 uiAnimKey);										///< Returns the Play Time of the Anim corresponding to the Key.
	RwReal			GetBaseDurationAnimTime();													///< Returns the Play Time of Animation.
	void			SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< A function to force the time is also needed.

	RwBool			GetEnableShadow();															///< Should I make a Shadow?

	// Doodads PSM
	RwBool				GetEnableGenShadowMap();													///< Do I need to create a ShadowMap?
	RwBool				AreThereObjPSMap();
	VOID				DeleteDoodadShadow();
	ENTITY_ATOMIC_VEC*	GetAtomicList();															///< Do Atomic List Return.
	VOID				SavePSMap(FILE* _pFile);
	VOID				LoadPSMap(FILE* _pFile);
	VOID				SetAtomicPSMap();

	virtual const RwBBox* GetBoundingBox(void) {return &m_bbox;}								///< Returns Bounding Box.

	// Link Effect related
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< Returns LinkEffect Vector.
	CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Attach Link Effect.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);						///< Detach the Link Effect.

	// Functions for World Editor
	virtual RwBBox  GetTriggerAABBInfo();														///< Returns AABB information for use in world editor trigger information.
	virtual RwBBox  GetTriggerAABBInfo( const RwV3d& vPos, 
		const RwV3d& vRotate, 
		const RwV3d& vScale);	

	// Fade System related
	RwBool          UpdateFadeSystem();                                                         ///< Fade the object according to the set Culling Distance.
	void            SetFadeEnable(RwBool bEnable);                                              ///< Set the presence or absence of Fade Enable

	void    SetTransform(void);

	// Milepost related properties
	RwUInt32        GetMilepostID() {return m_uiMilepostTextID;}                                ///< Returns Milepost Text ID.
	void            SetMilepostID(RwUInt32 uiMilepostTextID) {m_uiMilepostTextID = uiMilepostTextID;} ///< Set Milepost Text ID.

    // Toon related
    void            CheckToonData();                                                            ///< Check if Toon can be applied.
    SToonData*      GetToonData() {return m_pToonData;}

	// lightmap related
	virtual RwBool		SaveLightmap(FILE* pFile);
	virtual RwBool		LoadLightmap(FILE* pFile);
	static  RwInt32		SkipLightmap(FILE* pFile);

	static  BYTE*		SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightmapToFileMem(BYTE* pFileMem);

	virtual RwBool		LoadSchedulingLightmapProp(FILE* pFile);

	void		ResetUV();
	RwBool		CheckUVsSetUpForLightMapping();
	RwUInt32	GetObjectSerialID()								{ return m_uiObjectSerialID; }
    void		SetObjectSerialID(RwUInt32 uiObjectSerialID) { m_uiObjectSerialID = uiObjectSerialID; }
	void		DeleteLightmapFile();
	void		CheckLightmapFlags();

	// attach light
	virtual void		AttachLightObject();
	virtual RwBool		LoadLightObjectAttachData(FILE* pFile);	
	virtual RwBool		SaveLightObjectAttachData(FILE* pFile);
	static  RwInt32		SkipLightObjectAttachData(FILE* pFile);
		
	static  BYTE*		SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightObjectAttachDataToFileMem(BYTE* pFileMem);

	RwBBox		GetDefaultAABB();

	// object type
	RwUInt32			GetObjectType();
	void				SetObjectType(RwUInt32 uiObjectType);
	CNtlPLObjectType*	GetObjectTypePtr() { return m_pObjectType; }

protected:

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	RwBool  CreateAnim();																		///< Creates Anim-related information and objects.
	RwBBox  CreateDefaultAABB();																///< Generates basic AABB information when there is no AABB information in the property.	
	RwBool  UpdateFading(RwReal fElapsedTime);                                                  ///< Fading.

	// Loop Effect related
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< Add LoopEffect to the LoopEffect List.
	void    ClearLoopEffect();                                                                 ///< Destroys EffectInstances in the LoopEffect List.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< Check whether there is a LoopEffect of the Name-Bone pair in the LoopEffect List.

	// Loop Sound Related
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< Add SoundHandle to LoopSound List.
	void    ClearLoopSound();                                                                  ///< Destroys LoopSounds in the LoopSound List.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< Check whether the same SoundHandle exists in the LoopSound List.
	void    UpdateLoopSoundPos();                                                              ///< Update LoopSound location.

	// Animation Event Related
	virtual void OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);
	virtual void OnEventVisualSound(SEventSound* pEventSound);									
	virtual void OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);    
	virtual void OnEventAlphaFade(SEventAlpha* pEventAlpha);
	virtual void OnEventTMQ(SEventAnimCinematic* pEventTMQ);
	virtual void OnEventExplosion(SEventExplosion* pEventExplosion);

	void    SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);               ///< Set Atomic Weight Alpha Value.

	void	AddSceneUpdate();
	void	RemoveSceneUpdate();

public:
	void			SetLightMapMaterialFlags(RwUInt32 uiFlags);
	void			SetLightMapObjectFlags(RwUInt32 uiFlags);
	RwUInt32		GetLightMapMaterialFlags();
	RwUInt32		GetLightMapObjectFlags();
	void			FreeSchedulingLTProp();
	void			AllocSchedulingLTProp();
	RwBool			IsRpWorldSectorList(RpWorldSector* pRpWorldSector);
	void			UpdateRpSectorOBBCheck();

	// attach dogi gym. emblem texture. careful for Destroy()
	RwBool			SetAtomicEmblemMap(RwTexture* _pEmblemMap);
	RwBool			GetEmblemMarkEnabled();
	void			DestroyEmblemMap();

	// Dojo	
	CNtlPLEntity*	GetDojo()							{ return m_pDojoEntity; }
	void			SetDojo(CNtlPLEntity* pDojoEntity)	{ m_pDojoEntity = pDojoEntity; }

public:
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

public:
	//static void					SetFarPlane(RwReal fFarDist);

protected:
	//static RwPlane				g_planeFar;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif
	
protected:
	sSCHEDULING_LM_PROP*		m_pSchedulingLMProp;
    SObjectSchedulingInfo       m_sScheduleInfo;        ///< Information about scheduling loading 
	RwUInt32					m_uiObjectSerialID;

	CNtlPLObjectType*			m_pObjectType;

	RwBool						m_bActiveUpdate;
	RwBool						m_bLoadMap;

	CNtlPLResource *			m_pClumpResource;
	ENTITY_ATOMIC_VEC	        m_vecAtomicList;         ///< List of Atomics that currently make up Clump
	FRAME_MAP				    m_mapFrame;				 ///< Bone Info

	RwV3d						m_vModelAngle;
	RwV3d						m_vModelScale;
	RwV3d						m_vWorldPosition;

	CNtlPLObjectProperty*       m_pProperty;            ///< object property object	

	CNtlPLUVAnim*               m_pUVAnim;              ///< UVAnim file object                
	RpHAnimHierarchy*           m_pBaseHierarchy;		///< Base Hierarchy
	CNtlPLResource*             m_pAnimResource;        ///< Anim Resource
	RwBool                      m_bAnimPlay;            ///< Availability of Anim Play
	RwBool						m_bHaveAnim;			///< Presence or absence of Anim.

	RwReal						m_fAnimSpeed;			///< Anim Speed

	RwRGBA						m_sColor;				///< Color				
	RwRGBA						m_sAddColor;			///< AddColor

	RwSphere					m_BSphere;				///< BoundSphere where position is not calculated
	RwSphere					m_BSphereCur;			///< BoundingSphere where Position was calculated: Updated when GetBoundingSphere is called.

	CNtlPLLinkEffect            m_LinkEffectInstance;   ///< Object that manages LinkEffect Instances

	RwBBox                      m_bbox;                 ///< Object의 Bounding Box

	// Fade System related
	EObjectFadeState            m_eFadeState;           ///< Fade State of current Object
	RwReal                      m_fPrevCameraDistance;  ///< Distance from previous camera
	RwReal                      m_fFadeTime;            ///< Fade Time    
	CNtlPLEntityAlphaWeightBlend* m_pFadeBlend;         ///< Alpha blending object for Fade Blend settings

	// Animation related to Trigger Object
	CNtlAnimLayer*				m_pAnimLayer;			///< Animation layer
	CNtlInstanceAnimTable*		m_pInstanceAnimTable;	///< Animation Resource Instance Table
	RwUInt32					m_uiCurAnimKey;			///< Currently applied AnimKey

	ListSoundHandle				m_listLoopSound;        ///< LoopSound’s HANDLE List
	ListLoopEffect				m_listLoopEffect;       ///< LoopEffect의 list

	// Terrain object shadow related
	vector<CNtlWorldShadow*>    m_vecNtlWorldShadow;

	RwUInt32                    m_uiMilepostTextID;      ///< ID of the text table to be connected when used as a milestone

    // Toon related
    SToonData*				    m_pToonData;			///< Toon Ink, Toon Paint, Toon Resource

	// Indoor
	CNtlOBB						m_OBB;					///< used in indoors, to search for RpWorldSectors.
	vector<RpWorldSector*>		m_vecRpWorldSector;		///< RpWorldSectorlist, used in indoors, contains the RpWorldSector

	// Dojo
	CNtlPLEntity*				m_pDojoEntity;

	// attach light
	struct SLightObjectAttachData
	{
		RwV3d						vPos;
		RwChar						chBoneName[MAX_ATTR_BONE_NAME];
		SPLLightObjectCreateParam*	pPLLightObjectCreateParam;
	};
	std::vector<SLightObjectAttachData*> m_vecAttachLightParam;

public:
	virtual RwBool	LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	virtual RwBool	SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	static	RwInt32	SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*	SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*	SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
	static	BYTE*	SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif

#endif