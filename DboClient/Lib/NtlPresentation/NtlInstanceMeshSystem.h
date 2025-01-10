//***********************************************************************************
//	File		:	NtlInstanceMeshSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	? 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once


#include ".\ntlinstancecomponentsystem.h"
#include "NtlResourceMeshSystem.h"

#include "NtlPLResourceManager.h"
#include "NtlPLEntityRenderHelpers.h"

#include "NtlInstanceTrailSystem.h"

#include <rpuvanim.h>
#include <rtdict.h>

class CNtlInstanceMeshSystem : public CNtlInstanceComponentSystem
{
public:
	CNtlInstanceMeshSystem(void);
	virtual ~CNtlInstanceMeshSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

	RwBool		    Finish();                                                       ///< Processes the deletion of the effect.

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);
	virtual void    SetAlpha(RwUInt8 byValue);                                      ///< Apply the Alpha value of the effect.
    virtual void	SetScale(RwReal fScale);

    static void    CreateToonData(RpClump* pClump, SToonData* pToonData);
protected:
    RtDict* UVAnimDictLoad(const RwChar* strFullName);
    void	BuildEmitterStandard(SNtlMeshEmitterStandard* pEmitterStandard);
    void	UpdateSize(RwReal fDeltaTime);
    void	UpdateColor(RwReal fDeltaTime);
    void	UpdateRotate(RwReal fDeltaTime);
    virtual RwBool  UpdateFinish(RwReal fElapsedTime);          ///< Finish Processes the task.
    void    CalcBoundingSphere();
    

protected:
    CNtlResourceMeshSystem*			m_pResourceMeshSystem;	
    CNtlPLResource*					m_pClumpResource;
    CNtlPLResource*					m_pAnimationResource;

    RtDict*							m_pUvAnimDict;
    RwSList*						m_pAnimationMaterialList;

    RwRGBA							m_Color;   

    RwInt32							m_nSizeSeed;

    RwBool							m_bDefaultRotate;

    RwBool							m_bUpdateRenderMatrix;

    RwMatrix*                       m_pMatEmitter;                   ///< Matrix of the first emitter when Attach Option is False
    RwMatrix                        m_matInvScale;                   ///< Inverse matrix of the Scale matrix. Used when calculating spline.
    RpHAnimHierarchy*               m_pAnimhierarchy;                ///< Clump Hierarchy (only used when animation is present)

    CNtlInstanceTrailSystem*        m_pTrailSystem;                  ///< Trail System (Trail Action)
    SToonData*                      m_pToonData;                     ///< Toon information for meshes with toons applied
};
