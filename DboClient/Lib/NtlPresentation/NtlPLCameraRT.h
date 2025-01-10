/******************************************************************************
* File			: NtlPLCameraRT.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 3. 7
* Abstract		: 
*****************************************************************************
* Desc			: Camera render texture creation order
*				  1. Rendering if CameraBeginUpdate() is true
*				  2. Call CameraEndUpdate()
*				  3. Return texture with GetTexture()
* Caution) A new frame buffer is assigned to CameraBeginUpdate() each time. thus
* Create a texture with CameraBeginUpdate() and make sure to retrieve the texture with GetTexture().
* You must obtain it and apply a texture to it.
*****************************************************************************/
#pragma once

class CNtlPLCameraRT
{
public:
	CNtlPLCameraRT();
	CNtlPLCameraRT(const CNtlPLCameraRT& camera) {}
	virtual ~CNtlPLCameraRT();

	VOID			Create(RwInt32 iWidth, RwInt32 iHeigth);
	VOID			Destroy();

	RwBool			CameraBeginUpdate();			///< t : Ready to render to camera texture
	VOID			CameraEndUpdate();

	RwTexture*		GetTexture();

protected:
	RwCamera*				m_pCamera;				///< Raster camera
	RwTexture*				m_pRenderTexture;		///< Render target texture

	RwInt32					m_iWidth, m_iHeigth;
};