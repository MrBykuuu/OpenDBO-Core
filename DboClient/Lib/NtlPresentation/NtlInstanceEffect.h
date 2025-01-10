//***********************************************************************************
//	File		:	NtlInstanceEffect.h
//	Desc		:	
//	Begin		:	2005. 7.28
//                  2006. 8.17
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//                  HoDong
//	Update		:	
//***********************************************************************************
#ifndef __NTL_INSTANCE_EFFECT__
#define __NTL_INSTANCE_EFFECT__

#include "rwcore.h"
#include "NtlPLAttach.h"
#include "NtlEffectDefine.h"
#include "NtlResourceEffect.h"
#include "NtlInstanceComponentSystem.h"
#include "NtlPLResourceManager.h"
#include "NtlPLSound.h"

/**
 * \ingroup Effect
 * Effect entity class (the container class for NtlInstanceComponentSystem).
 * 
 */
class CNtlInstanceEffect : public CNtlPLAttach
{
public:
	typedef std::vector<CNtlInstanceComponentSystem*>	svdef_InstanceComponentSystem;
	svdef_InstanceComponentSystem						m_svSystem;

	CNtlResourceEffect* m_pResourceEffect;

	RwMatrix			m_matWorld;						///< Effect World Matrix (m_vModelScale *m_vModelAngle *m_vWorldPosition)
														///< When directly entering the Matrix value with SetMatrix(), only the m_vWorldPosition value can be mixed.
														///< Rotation and scale may conflict with the values ??entered in the Matrix.

	RwV3d				m_vModelScale;					///< Model Scale
	RwV3d				m_vModelAngle;					///< Model Angle (Degree -360 -360)
	RwV3d				m_vWorldPosition;				///< World Position    

	RwReal				m_fLifeTime;
	RwBool				m_bReady;
	RwBool				m_bShow;
	RwBool				m_bAutoDelete;
    RwBool              m_bApplyScale;

    // Sound related instance
    CNtlPLSound         m_PLSound;                      ///< Sound object   

protected:
    RwReal						m_fAnimSpeed;			///< Anim Speed
    CNtlPLResource *			m_pVisibleBVClump;		///< Bounding Volume Clump //Only for picking from Tool	
    RwSphere                    m_BoundingSphere;       ///< Bounding Sphere
    RwBool                      m_bIsCulling;           ///< Flag whether culled by bounding sphere or not
    RwReal                      m_fNotUpdateTime;       ///< Time not updated due to culling or Not Visible    
    
    static  RwBool              m_bLowSpec;             ///< Effect low specification application status flag
    static  RwReal              m_fLowSpecRatio;        ///< Ratio for effect low specs
    static  RwBool              m_bRenderMeshSystem;    ///< Mesh system rendering On/Off flag
    static  RwBool              m_bRenderDecalSystem;   ///< Decal system rendering On/Off flag

public:
	CNtlInstanceEffect(void);
	~CNtlInstanceEffect(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool	Create(const SPLEntityCreateParam *pParam);
	virtual void	Destroy();
    virtual void    Finish();                                   ///< Method used when calling a termination operation instead of immediately ending the effect 	    
	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();
	virtual RwBool  RenderToTexture(void);
    virtual RwBool	SetProperty(const CNtlPLProperty *pData);
    virtual void    SetAttachParent(CNtlPLAttach *pPLAttachEntity);

    void    AddComponentSystem(CNtlInstanceComponentSystem* pComponentSystem);

	virtual RwBool IsAutoDelete()	{ return m_bAutoDelete; }						///< Is this an effect that is automatically deleted?
	
	void	SetMatrix(RwMatrix& matWorld);											///< Set the World Matrix of the Effect.
	RwMatrix &GetMatrix(void);														///< Obtain the World Matrix of the Effect.
	
	void	SetPosition(const RwV3d *pPos);											///< Move the Effect to the location of the World.
	RwV3d	GetPosition(void);														///< Get the location of the Effect¡¯s World.
	
	void	SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);				///< Model space rotation value (degree) of Effect
	const	RwV3d* GetRotate(void) const;											///< Obtain Model space rotation value of Effect

	void	SetScale( RwReal fScale );												///< Model space scale value of Effect
	RwReal	GetScale(void);															///< Obtain model space scale value of effect
    void    SetApplyScale(RwBool bApply);                                           ///< Set whether to apply scale.

    inline RwSphere* GetBoundingSphere();                                           ///< Returns the bounding sphere of the effect. (If there is no bounding sphere, NULL is returned)
    void			 RenderBoundingSphere();                                        ///< Renders the shape of the bounding sphere. (Venus only)

    virtual void	SetAlpha(RwUInt8 byValue);                                      ///< Set the Alpha value of the Effect.
    virtual void	SetVisible(RwBool bVisible);                                    ///< Set whether the effect is visible or not
    virtual RwBool	IsVisible(RwUInt32 uiFlags = 0xFFFFFFFF);                       ///< Returns whether the effect is visible or not.

	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);									///< Animaiton Speed (1.f: Default )
	void			SetVisibleBoundingVolume(RwBool bVisible);								///< World Editor¿ë Picking Bounding Volume
    void			SetPostEffect_CenterFixEnable(RwBool bEnable);                          ///< Set the Center Fix function of the included Post Effect.
    void			SetCulling(RwBool bCulling) {m_bIsCulling = bCulling;}                  ///< Forcefully sets the presence or absence of culling.
    RwBool			IsCulling() {return m_bIsCulling;}                                      ///< Returns the culling presence or absence flag.
    RwUInt32        GetMemoryUseSize();                                                     ///< Returns the memory usage of the effect.
    
#ifndef dNTL_WORLD_CULLING_NEW
	virtual RwBool  CullingTest(RwCamera* pCamera);                                         ///< Check for curling.
#endif

    //--Flag settings for Low Spec
    static void     SetLowSpecEnable(RwBool bEnable) {m_bLowSpec = bEnable;}        ///< Set whether low specification is applied or not.
    static RwBool   GetLowSpecEnable() {return m_bLowSpec;}
    static void     SetLowSpecRatio(RwReal fRatio) {m_fLowSpecRatio = fRatio;}      ///< Apply the low spec ratio.   
    static RwReal   GetLowSpecRatio() {return m_fLowSpecRatio;}
    static void     SetRenderMeshSystem(RwBool bEnable) {m_bRenderMeshSystem = bEnable;}    ///< Mesh System Rendering On /Off
    static RwBool   GetRenderMeshSystem() {return m_bRenderMeshSystem;}
    static void     SetRenderDecalSystem(RwBool bEnable) {m_bRenderDecalSystem = bEnable;}  ///< Decal System Rendering On /Off
    static RwBool   GetRenderDecalSystem() {return m_bRenderDecalSystem;}

protected:
    void	SetTransform();
    void	CreateSystem(CNtlResourceEffect* pResourceEffect, const RwMatrix& matRender);
    void    CalcBoundingSphere();                                                   ///< Calculate the bounding sphere.

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif

public:
	virtual RwBool	LoadFromFile(FILE* pFile);
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32 SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);

};

RwSphere* CNtlInstanceEffect::GetBoundingSphere() 
{
    if(m_BoundingSphere.radius != 0.0f)
        return &m_BoundingSphere;

    return NULL;
}

#endif
