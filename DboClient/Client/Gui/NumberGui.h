/******************************************************************************
*File			: CNumberGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2007. 9. 4
*Abstract		: 
*****************************************************************************
*Desc			: Class for displaying numbers composed of images in the GUI level
*The area of ??m_rtRect does not refer to the actual displayed space.
*Among the components of m_rtRect, left and top (top of the image) have numbers.
*The reference coordinates right and bottom according to the alignment from the displayed parent are
*Regardless of the alignment method, the width and height are always indicated from left and top.
*
*Assume that the width and height of the image are all the same
*****************************************************************************/

#pragma once

// Core
#include "NtlDebug.h"
#include "NtlCallbackWidget.h"

// Dbo
#include "SurfaceGui.h"


#define dMAX_CHPHER			10		///< Maximum number of digits that can display a number

#define dNCFLAG_NONE		0x00
#define dNCFLAG_MINUS		0x01
#define dNCFLAG_PLUS		0x02

enum eNumberContent
{
	NC_0,
	NC_1,
	NC_2,
	NC_3,
	NC_4,
	NC_5,
	NC_6,
	NC_7,
	NC_8,
	NC_9,
	NC_PLUS,
	NC_MINUS,

	NUM_NC
};

enum eNumberHoriDirection
{
	ND_LEFT,
	ND_CENTER,
	ND_RIGHT
};

enum eNumberVertDirection
{
	NVD_TOP,
	NVD_CENTER,
	NVD_BOTTOM,
};


class CNumberGui
{
public:
	struct sEffectRate
	{
		RwBool			bEnable;
		RwReal			fStartRate;
		RwReal			fEndRate;
		RwReal			fRemainTime;
		RwReal			fElapsed;

		CNtlCallbackParam0* pCallback;

		sEffectRate()
		:bEnable(false)
		,fStartRate(0.f)
		,fEndRate(0.f)
		,fRemainTime(0.f)
		,fElapsed(0.f)
		,pCallback(NULL)
		{
		}
	};

	typedef VOID* EndEffectRateCallBack();

	CNumberGui();
	virtual ~CNumberGui();
	
	/*	byHoriAlign: Left and right direction in which numbers and markers are aligned at given coordinates
		byVertAlign: Up and down direction in which numbers and markers are aligned at given coordinates
		byNumGap: Gap between numbers and marks
		byFlag: Register the flag of the mark to be used other than numbers.
		byRemainCipher: Number of digits that must be displayed
						  Example) When byRemainCipher = 2, if SetNumber(1), 01 is displayed.
							  byRWhen emainCipher = 4 and SetNumber(1), 0001 is displayed */	
	RwBool			Create(RwUInt8 byHoriAlign = ND_LEFT, RwUInt8 byVertAlign = NVD_TOP, RwInt32 iNumGap = 0, RwUInt8 byFlag = dNCFLAG_NONE, RwUInt8 byRemainCipher = 1);
	VOID			Update(RwReal fElapsed);
	VOID			Destroy();

	VOID			SetNumber(RwInt64 iNumber);
	VOID			SetSurface(RwInt8 byIndex, gui::CSurface& surface);
	VOID			SetPosition(RwInt32 iPosX, RwInt32 iPosY);
	VOID			SetParentPos(RwInt32 iParentX, RwInt32 iParentY);
	VOID			SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha);
	VOID			SetColorOnly(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	VOID			SetAlpha(RwUInt8 byAlpha);
	VOID			SetNumGap(RwInt32 iGap);
	VOID			Show(RwBool bShow);
	VOID			SetHoriDirection(RwUInt8 byHoriDirection);
	VOID			SetVertDirection(RwUInt8 byVertDirection);
	VOID			SetRate(RwReal fRate);	

	RwInt64			GetNumber();
	RwInt32			GetWidth();
	RwInt32			GetContentWidth(RwInt8 byIndex);
	RwInt32			GetHeight();	
	CRectangle		GetPosition();
	CPos			GetParentPos();
	CRectangle		GetRect();
	CRectangle		GetScreenRect();
	RwReal			GetRate();

	// Use the CNumberGui_ZoomHelper class. This function will be deleted in the future
	VOID			EffectRate(RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime);

	VOID			UnsetSurface(RwUInt8 byIndex);
	VOID			UnsetAll();

	RwBool			PtInRect(RwInt32 iPosX, RwInt32 iPosY);
	RwBool			IsShow();
	RwBool			IsEffectRate();

	VOID			Render(bool bRenderTop = false);


	// when finish rate effect
	template <class Callbackclass>	
	VOID			LinkRateEffect(Callbackclass *cbclass,RwInt32 (Callbackclass::*callback)())
	{	
		if(m_EffectRate.pCallback)
		{
			NTL_DELETE(m_EffectRate.pCallback);
		}

		m_EffectRate.pCallback = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
	}

protected:
	VOID			CalcWidth();

protected:
	CSurfaceGui			m_srfNumber[NUM_NC];			///< Number indicating combat power
	
	RwUInt8				m_byFlag;
	RwUInt8				m_byHoriDirection;
	RwUInt8				m_byVertDirection;

	RwInt32				m_iNumGap;		
	RwInt32				m_iX_fromParent, m_iY_fromParent;
	RwInt32				m_iWidth, m_iHeight;
	CPos				m_ParentPos;
	RwReal				m_fRate;

	RwUInt8				m_byCurCipher;					///< current digits
	RwUInt8				m_byCipher[dMAX_CHPHER];		///< Number for each digit
	RwUInt8				m_byRemainCipher;				///< Minimum number of digits to be maintained
	RwInt64				m_i64Number;					///< Number entered

	sEffectRate			m_EffectRate;
};



inline VOID CNumberGui::SetSurface(RwInt8 byIndex, gui::CSurface& surface)
{
	m_srfNumber[byIndex].SetSurface(surface);
}

inline VOID CNumberGui::SetParentPos(RwInt32 iParentX, RwInt32 iParentY)
{
	m_ParentPos.x = iParentX;
	m_ParentPos.y = iParentY;
}

inline VOID CNumberGui::SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha )
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetColor(byRed, byGreen, byBlue, byAlpha);
}

inline VOID CNumberGui::SetColorOnly( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue )
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetColorOnly(byRed, byGreen, byBlue);
}

inline VOID CNumberGui::SetAlpha(RwUInt8 byAlpha)
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetAlpha(byAlpha);
}

inline VOID CNumberGui::SetNumGap(RwInt32 iGap)
{
	m_iNumGap = iGap;
}

inline VOID CNumberGui::Show(RwBool bShow)
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].Show(bShow);
}

inline VOID CNumberGui::SetHoriDirection(RwUInt8 byHoriDirection)
{
	m_byHoriDirection = byHoriDirection;
}

inline VOID CNumberGui::SetVertDirection(RwUInt8 byVertDirection)
{
	m_byVertDirection = byVertDirection;
}

inline RwInt64 CNumberGui::GetNumber()
{
	return m_i64Number;
}

inline RwInt32 CNumberGui::GetWidth()
{
	return m_iWidth;
}

inline RwInt32 CNumberGui::GetContentWidth(RwInt8 byIndex)
{
	return m_srfNumber[byIndex].GetWidth();
}

inline RwInt32 CNumberGui::GetHeight()
{
	return m_iHeight;
}

inline CRectangle CNumberGui::GetPosition()
{
	// Just refer to the reference coordinates, area, and height from the parent.
	CRectangle rtRect;

	rtRect.left		= m_iX_fromParent;
	rtRect.top		= m_iY_fromParent;
	rtRect.right	= rtRect.left + m_iX_fromParent;
	rtRect.bottom	= rtRect.top + m_iY_fromParent;

	return rtRect;
}

inline CPos CNumberGui::GetParentPos()
{
	return m_ParentPos;
}

inline VOID CNumberGui::UnsetSurface(RwUInt8 byIndex)
{
	m_srfNumber[byIndex].UnsetTexture();
}

inline VOID CNumberGui::UnsetAll()
{
	for( RwUInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].UnsetTexture();
}

inline RwBool CNumberGui::PtInRect(RwInt32 iPosX, RwInt32 iPosY)
{
	// You must enter coordinates from the parent, not screen coordinates.
	CRectangle rtRect = GetRect();

	return rtRect.PtInRect(iPosX, iPosY);
}

inline RwBool CNumberGui::IsShow()
{
	return m_srfNumber[NC_0].IsShow();
}

inline RwBool CNumberGui::IsEffectRate()
{
	return m_EffectRate.bEnable;
}

inline RwReal CNumberGui::GetRate()
{
	return m_fRate;
}