/*****************************************************************************
 *
 * File			: NtlPLCameraRenderTexture.h
 * Author		: HongHoDong
 * Copyright	: (?)NTL
 * Date			: 2006. 8. 19.	
 * Abstract		: NtlPLCameraRenderTexture
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

/**
 * \ingroup NtlPresentation
 *This is a class that wraps RenderWare¡¯s RenderToTexture function.
 *Since texture is created and used, the user receives the texture and renders it.
 Just do *.
 *When each CameraRenderTexture is created, the Camera's ZBuffer is continuously created.
 *Memory for ZBuffer is likely to be used more.
 */

#ifndef __NTL_CAMERA_RENDER_TEXTURE_
#define __NTL_CAMERA_RENDER_TEXTURE_

#include "NtlDebug.h"
#include "NtlPLEntity.h"

//#define RENDER_TEXTURE_ASPECTRATIO (4.0f/3.0f)

class CNtlPLCameraRenderTexture
{
protected:
	RwCamera	*m_pCamera;																///< Camera for RenderToTexture
	RwTexture	*m_pRenderTexture;														///< Texture for RenderToTexture
	RwBool		m_IsCamInWorld;
	
protected:
	void SetCalcFov(RwReal fFov, RwInt32 nWidth, RwInt32 nHeight);

public:
	CNtlPLCameraRenderTexture() : m_pCamera(NULL), m_pRenderTexture(NULL) {} ;
	~CNtlPLCameraRenderTexture() {};

	void Create( RwInt32	nTexWidth,														///< Create RenderTexture (nWidth: horizontal, nHeight; vertical) 
		         RwInt32	nTexHeight, 
				 RwInt32	nRealWidth, 
				 RwInt32	nRealHeight,
				 RwReal		fFov = 45.f,
				 RwBool		IsCamInWorld = FALSE);

	void SetNear(RwReal _Near);
	void SetFar(RwReal _Far);
	RwCamera* GetCamera() { return m_pCamera; }
																						///< What if there is an area to be photographed separately from the Texture Size?
	void Destroy();
	
	void RenderTexture();																// Render NTL World
	void RenderTexture(CNtlPLEntity *pEntity);											///< Render the entity to texture.
	void RenderTexture(std::list<CNtlPLEntity*>& listEntity);
																						///< How to timing rendering to texture?
																						///< There is no problem with rendering when updating the UI.
	
	void Render(RwCamera *pRenderTarget, RwV2d fPos, RwV2d fSize);	    				///< Rendering m_pRenderTexture for Test
	
	RwTexture *GetTexture() { NTL_ASSERTE(m_pRenderTexture); return m_pRenderTexture; } ///< Texture Return (I'm thinking about managing references)
	void SetCameraPosition(const RwV3d *pCameraPos, const RwV3d *pLookAt);				///< Camera Transform
};

#endif