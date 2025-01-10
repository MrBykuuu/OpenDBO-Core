//***********************************************************************************
//	File		:	NtlInstancePostEffectSystem.h
//	Desc		:	
//	Begin		:	2007. 1.17
//	Copyright	:	? 2007 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstancecomponentsystem.h"
#include "NtlResourcePostEffectSystem.h"

class CNtlInstancePostEffectSystem : public CNtlInstanceComponentSystem
{
public:
    // comic effect
    enum COMIC_STATE
    {
        COMIC_STATE_IDLE = 0,
        COMIC_STATE_FADE_OUT,
        COMIC_STATE_FADE_IN,

        COMIC_STATE_MAX_COUNT,
    };

public:
	CNtlInstancePostEffectSystem(void);
	virtual ~CNtlInstancePostEffectSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();
            RwBool  RenderPostEffect();
    virtual void    SetWorldMatrix(const RwMatrix& matWorld);

            COMIC_STATE GetComicState() {return m_nComicState;}
            void    SetCenterFixEnable(RwBool bEnable);                     ///< Turns the center point fixation function on/off.

protected:
            void	BuildEmitterStandard(SNtlPostEffectEmitterStandard* pEmitterStandard);
            void    SetComicVertex();
            void    SetComicIndex();
            void    RebuildPostEffectVertex();
            void    UpdateVertex(RwReal fElapsedTime);
            RwRGBA  UpdateColor(RwReal fDeltaTime);            

protected:
    CNtlResourcePostEffectSystem*			m_pResourcePostEffectSystem;    
    RwInt32                                 m_nVertexCount;    
    RwInt32                                 m_nIndexCount;                  ///< Number of Indexes
    RwInt32			                    	m_nCurLoopCount;        
    RwBool                  				m_bHalfIndexType;    
    COMIC_STATE			                    m_nComicState;
    RwReal				                    m_fComicAlpha;    
    RwV2d                                   m_v2dCenterPos;                 ///< Coordinates of the center point
    RwBool                                  m_bCenterFix;                   ///< Center point fixation function use flag
    RwBool                                  m_bUpdateCenter;                ///< Flag whether the position of the center point needs to be changed

    RwIm2DVertex*		                    m_pComicVertices;    
    RwImVertexIndex*	                    m_pComicHalfIndex;        
};
