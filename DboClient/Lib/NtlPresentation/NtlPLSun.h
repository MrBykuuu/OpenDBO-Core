#pragma once

/*****************************************************************************
*
* File			: NtlPLSun
* Author		: Agebreak 
* Copyright	: NTL Co., Ltd.
* Date			: 2007. 4. 12	
* Abstract		: Presentation layer Sun & Moon entity class
*****************************************************************************
* Desc         :  Entity to represent the sun and moon. The billboard is flat, and the camera moves while maintaining its distance.
*
*****************************************************************************/

#include "NtlPLAttach.h"
#include "NtlPLLensFlare.h"


enum eSKY_FLOATING_BILLBOARD_TYPE
{
	eSFBT_SUN = 1,
	eSFBT_PLANET,
};


struct SPLSunCreateParam : public SPLEntityCreateParam
{
	std::string		strTexture;						///< Texture name to apply 
	RwBool			bAttachLensFlare;				///< Whether LensFlare is applied or not
	RwReal			Scale;
};


class CNtlPLSun : public CNtlPLAttach
{
public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLSun(void);
	virtual ~CNtlPLSun(void);

	virtual RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy(void);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);


	virtual void	SetPosition(const RwV3d* pPos);
	virtual void	SetColor(RwRGBA color);
	virtual void	SetScale(RwReal fScale);
	virtual void	SetMatrix(RwMatrix& matWorld);
	virtual void	SetTexture(RwTexture* pTexture);
	virtual RwBool	GetVisibility();
	virtual void	SetVisibility(RwBool bVisible);
			
	void	SetFadeEffect(RwBool bFadeIn, RwReal fTime = 1.0f);					///< Apply fade effect. If True, it appears, and if False, it disappears. time is the time it takes
	void	SetFadeEffect(RwRGBA colorFade, RwReal fTime = 1.0f);				///< Apply the Fade effect. Converted to fadeColor over time.

	void	ReFreshLensFlareData();												///< Refresh function for LensFlare Test

protected:
	void	CreateVertexBuffer();
	void	UpdateFadeColor(RwReal fElapsedTime);

protected:
	RwIm3DVertex*	m_pIm3dBuffer;
	RwV3d			m_vPos;
	RwReal			m_fScale;
	RwTexture*		m_pTexture;
	RwMatrix		m_matWorld;
	RwRGBA			m_color;	

	RwBool			m_bVisible;									///< The Sun is rendered between the cloud and the SkyBox, so Entity Visible should not be used.

	// For Fade	
	RwBool			m_bFading;									///<Fade status flag
	RwRGBA			m_FadeColor;
	RwRGBA			m_OriginColor;
	RwReal			m_fFadeTime;
	RwReal			m_fFadingTime;

	// LensFlare object
	CNtlPLLensFlare* m_pLensFlare;
};
