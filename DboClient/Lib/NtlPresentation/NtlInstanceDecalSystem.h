//***********************************************************************************
//	File		:	NtlInstanceDecalSystem.h
//	Desc		:	
//	Begin		:	2007. 3.19
//	Copyright	:	
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstanceImVertexsystem.h"
#include ".\ntlresourcedecalsystem.h"
#include "NtlPLDecal.h"

struct DecalParam
{
	RwV3d vPos;
	RwV3d vSize;
	RwInt32 *pBufferCnt;
	RwIm3DVertex *pVertices;
};

class CNtlInstanceDecalSystem : public CNtlInstanceComponentSystem
{
	// It uses Vertex, but because it uses CNtlDecal, it does not inherit from ImvertexSystem.
public:
	CNtlResourceDecalSystem*		m_pResourceDecalSystem;			///< Resource object containing property information
	RwBool							m_bUpdateVertex;
	CNtlPLDecal*					m_pDecalSystem;					///< Decal object that draws the actual decal

public:
	CNtlInstanceDecalSystem(void);
	virtual ~CNtlInstanceDecalSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

			void	SetVertexUv();
    virtual void    SetAlpha(RwUInt8 byValue);
    virtual void	SetScale(RwReal fScale);									 ///< Set the scale of the effect. (Currently only valid for particles)

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

			void	BuildEmitterStandard(SNtlDecalEmitterStandard* pEmitterStandard);

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);

	RwInt32	GetPolyCount();											///< Returns the number of polygons currently being drawn. (Vertex count /3)
};

