//***********************************************************************************
//	File		:	NtlInstanceParticleSystem.h
//	Desc		:	
//	Begin		:	2006. 7.28
//	Copyright	:	? 2006 by NTL CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstancecomponentsystem.h"

#include ".\ntlresourceparticlesystem.h"

#include "NtlInstanceTrailSystem.h"
/**
 * \ingroup NtlPresentation
 * \brief Class that constitutes Particle Instance
 * \date 2006-08-28
 * \author agebreak
 */
class CNtlInstanceParticleSystem : public CNtlInstanceComponentSystem
{
public:
	CNtlResourceParticleSystem* m_pResourceParticleSystem;

	RpPrtStdEmitterClass*		m_pEmitterClass;
	RpPrtStdParticleClass*		m_pParticleClass;

	RpPrtStdEmitter*			m_pEmitter;
	RpPrtStdEmitterStandard*	m_pEmitterStandard;    

	RpAtomic*					m_pAtomic;
	RpPrtStdEmitterPTank*		m_pTankProperties;

	RwTexture*					m_pStandardTexture;

	RwInt32						m_nVelocityOffset;
	// Hurricane
	RwReal						m_fHurricaneLifeTime;
	RwReal						m_fHurricaneDeltaTime;
    
    // Trail System (Trail Action)
    CNtlInstanceTrailSystem*    m_pTrailSystem;

protected:
    // Rotation related when Matrix Action is applied
    RpPrtStdEmitterPrtMatrix*   m_pEmitterPrtMatrix;
    RwV3d                       m_vOrgAt;           ///< Originally set At value
    RwV3d                       m_vOrgUp;           ///<Originally set Up value    

    // Related to Follow Particle (particles that follow particle creation objects)
    RwV3d                       m_vPosPrev;         ///< Previous location of created object
    RwV3d                       m_vPosDelta;        ///< Location changed from previous location    

public:
	CNtlInstanceParticleSystem(void);
	virtual ~CNtlInstanceParticleSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);
	virtual void	Stop();    

			RpPrtStdEmitter*	SubEmitterCreate(CNtlResourceParticleSystem* pResourceSystem);

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);
	virtual void	SetScale(RwReal fScale);					///< Set the scale of the particles.
    virtual void    SetAlpha(RwUInt8 byValue);                  ///< Set the Alpha of the particle.

			void	BuildHurricanePoint(RwV3d& vPosition, RwReal fLifeTime);

protected:
            void    UpdateFollow();                             ///< Updates the position of the Follow particle.    
            void    UpdateBillboard();                          ///< If a billboard is set, the billboard is applied.
            void    UpdateRotate();                             ///< Apply the Emitter¡¯s Rotate value.
};
