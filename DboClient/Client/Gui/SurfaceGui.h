/*****************************************************************************
 *
 * File			: SurfaceGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 10. 25	
 * Abstract		: DBO surface gui.
 *****************************************************************************
 * Desc         : Move the surface to the specified location using one of two methods.
 *
 *                SetPosition(): Enters the coordinates on the client screen.
 *
 *                SetPositionfromParent(): Enters the coordinates from the parent dialog.
 *                SetPositionbyParent(): Parent dialog on the client screen
 *                Enter coordinates
 *****************************************************************************/

#ifndef __SURFACE_GUI_H__
#define __SURFACE_GUI_H__

#include "NtlPLGui.h"

class CSurfaceGui
{
protected:

	RwBool m_bShow;
	RwBool m_bCreateTexture;
	gui::CSurface m_surPaint;
	CPos	m_Pos_from_Parent;
	
	RwBool		m_bClipping;
	CRectangle	m_rtClipping;

public:

	CSurfaceGui();
	virtual ~CSurfaceGui();

	RwBool IsCreatedTexture(VOID) { return m_bCreateTexture; }
	gui::CTexture* GetTexture(VOID) { return m_surPaint.m_pTexture; }

	void Show( RwBool bShow ) { m_bShow = bShow; }
	RwBool IsShow(VOID) { return m_bShow; }

	void SetRect(RwInt32 iLeft, RwInt32 iTop, RwInt32 iRight, RwInt32 iBottom);
	void SetRect(CRectangle& rect);
	void SetRectWH(RwInt32 iLeft, RwInt32 iTop, RwInt32 iWidth, RwInt32 iHeight);

	void SetWidth(RwInt32 iWidth);
	void SetHeight(RwInt32 iHeight);
	void SetSize(RwInt32 iWidth, RwInt32 iHeight);

	void SetPosition(RwInt32 iPosX, RwInt32 iPosY);
	void SetCenterPosition(RwInt32 iPosX, RwInt32 iPosY);
	void SetPositionfromParent(RwInt32 iPosX_from_Parent, RwInt32 iPosY_from_Parent);
	void SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY);

	void SetClippingRect( CRectangle& rtClipping );
	void SetClippingMode( RwBool bClipping );
	
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha); 
	void SetColorOnly(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	void SetAlpha(RwUInt8 byAlpha);
	void SetZRot(RwReal fTheta);
	RwBool SetTexture(const RwChar *pName);
	void SetTexture(const gui::CTexture *pTexture,RwBool bCreated = FALSE);
	void UnsetTexture(VOID);
	void SetSurface(gui::CSurface& surface);

	RwReal GetZRot();
	void GetRect(CRectangle& rect);	
	CPos GetPosfromParent();
	RwInt32 GetWidth();
	RwInt32 GetHeight();
	RwUInt8 GetAlpha();
	gui::CSurface* GetSurface() { return &m_surPaint; }

	RwBool PtInRect(int iX, int iY);

	virtual void Render(bool bRenderTop = false);
};

/*****************************************************************************
*
* File			: FilteredSurfaceGui.h
* Author		: Peessi
* Copyright		: (аж)NTL
* Date			: 2006. 9. 12	
* Abstract		: DBO surface gui.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

class CFilteredSurfaceGui : public CSurfaceGui
{
protected:
	gui::CTexture*	m_ptexFilter;

	RwIm2DVertex	m_2dVertices[4];
	
	// Implementation
	VOID BeginFilterRenderState(VOID);
	VOID EndFilterRenderState(VOID);

public:
	CFilteredSurfaceGui();
	virtual ~CFilteredSurfaceGui();

	void SetFilterTexture( const gui::CTexture* ptexFilter );
	void UnsetFilterTexture(VOID);
	
	gui::CTexture* GetFilterTexture(void) { return m_ptexFilter; }
	
	void Render(bool bRenderTop = false);
};

/*****************************************************************************
*
* File			: CCameraRenderTargetSurface.h
* Author		: hong sungbock
* Copyright		: (аж)NTL
* Date			: 2007. 1. 24	
* Abstract		: DBO surface gui for render target
*****************************************************************************
* Desc         : Regardless of the size of the application, depending on the camera size
*                Surface to create texture
*
*****************************************************************************/

class CCameraRenderTargetSurface : public CSurfaceGui
{
public:
	void Render(bool bRenderTop = false);

protected:
	RwIm2DVertex m_2dVertices[4];
};

#endif
