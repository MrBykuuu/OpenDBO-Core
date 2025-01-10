/*****************************************************************************
*
* File			: NtlPlLensFlare
* Author		: agebreak
* Copyright 	: NTL Co., Ltd.
* Date			: 2007. 4. 19	
* Abstract		: Presentation layer LensFlare entity class
*****************************************************************************
* Desc          : A class that implements Lensflare, which is affected by the position of the sun.
*
*****************************************************************************/

#pragma once

#include "NtlPLAttach.h"
#include "NtlDebug.h"
#include "NtlEffectDefine.h"

#define	LENSFLARE_FADE_VELOCITY		4.0f
#define	LENSFLARE_PICK_TICK			0.3f

///< Structure containing information about the spots that make up LensFlare
struct SNtlPLLensFlareSpot
{
	RwTexture*	m_pTexture;					///< Texture of Spot
	RwReal		m_fSize;					///< Spot Size (0.0 ~ 1.0)
	RwReal		m_fLinePos;					///< Location of Spot (1 ~ -1)
	RwRGBAReal	m_color;					///< SpotÀÇ Vertex Color	
	RwIm2DVertex* m_pIm2dBuffer;			///< Vertex buffer that constitutes the spot

	SNtlPLLensFlareSpot()
	{
		m_pTexture = NULL;
		m_fSize = 1.0f;
		m_fLinePos = 0.5f;
		m_color.red = m_color.green = m_color.blue = m_color.alpha = 1.0f;
		m_pIm2dBuffer = NULL;
	}

	SNtlPLLensFlareSpot(RwTexture* pTexture, RwReal fSize, RwReal fLinePos, RwReal red, RwReal green, RwReal blue, RwReal alpha)
	{
		m_pTexture = pTexture;
		m_fSize = fSize;
		m_fLinePos = fLinePos;		
		m_color.red = red;
		m_color.green = green;
		m_color.blue = blue;
		m_color.alpha = alpha;
		m_pIm2dBuffer = NULL;
	}

	~SNtlPLLensFlareSpot()
	{
		if(m_pIm2dBuffer)
		{
			NTL_ARRAY_DELETE(m_pIm2dBuffer);
		}
	}
};

class CNtlPLLensFlare : public CNtlPLAttach
{
public:
	CNtlPLLensFlare(void);
	virtual ~CNtlPLLensFlare(void);

	virtual RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy(void);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);

	virtual void	SetMatrix(RwMatrix& matWorld);

	void	AddSpot(const SNtlPLLensFlareSpot& spot);				///< Add Spot.
	void	AddSpotForTest(const std::string& strXMLFileName);		///< Read and set spot information from XML file

	void	SetMaxIntensity(RwReal fMaxIntensity) {m_fIntensity = fMaxIntensity;}	///< Set maximum intensity. (between 0.0 and 1.0)
	RwReal  GetMaxIntensity() {return m_fIntensity;}				///< Returns the maximum intensity.

protected:
	void	AddSpots();												///< Add spots to the vector.
	void	UpdateVertices(RwReal fElapsedTime);					///< Update vertex information.
	void	SetRhwVertex(RwIm2DVertex* pVertex, RwReal nStartX, RwReal nStartY, RwInt32 width, RwInt32 height, RwRGBAReal color);		///< Set 2D Vertexes
	RwBool	CalcPicking(const RwV2d& posLight);						///< Check the Object and Terrain Collision of the light source to determine whether it will be visible or invisible.

protected:
	std::vector<SNtlPLLensFlareSpot>		m_vecSpots;
	std::vector<RwTexture*>					m_vecTex;	
	RwReal									m_fIntensity;			///< Maximum intensity of Flare
	RwReal									m_fIntensityBorder;		///< Range of Flare (affects intensity)	
	RwMatrix								m_matWorld;				///< World Matrix (used to set Pos)
	RwBool									m_bRender;				///< presence of render
	RwReal									m_fFadeIntensity;		///< Variables for Fade In/Out
	EFadeStatus								m_eFadeStatus;

	// Variables for Pick Tick setting
	RwBool									m_bPrevPickingResult;	///< Picking results saved in the previous tick
	RwReal									m_fPickingTickTime;		///< Time variable for specifying Tick in Pick									
};	
