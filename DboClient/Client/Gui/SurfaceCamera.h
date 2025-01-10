/******************************************************************************
*File			: SurfaceCamera.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2007. 1. 21
*Abstract		: 
*****************************************************************************
*Desc			: SurfaceGui¿ë Render texture camera
*****************************************************************************/
#pragma once

#include "SurfaceGui.h"

class CSurfaceCamera
{
public:
	CSurfaceCamera();
	CSurfaceCamera(const CSurfaceCamera& camera) {}
	virtual ~CSurfaceCamera();

	VOID			Create(RwInt32 iWidth, RwInt32 iHeight);
	VOID			Destroy();

	RwBool			CameraBeginUpdate();			///< t : Ready to render to camera texture
	VOID			CameraEndUpdate();

	VOID			CreateTexture();				///< Creates gui::CTexture
	gui::CTexture*	GetTexture();

protected:
	RwCamera*				m_pCamera;				///< Raster camera
	RwTexture*				m_pRenderTexture;		///< Render target texture

	gui::CTexture			m_GUITexture;			///< Dbo Texture
};