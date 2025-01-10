/******************************************************************************
*File: CounterGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : November 8, 2008
*Abstract		: 
*Update: 
*****************************************************************************
*Desc           : GUI that counts counters during the game
*                 1. The count only decreases and does not increase
*                 2. The clock can make a sound every second.
*                 3. Counting can be temporarily stopped
*                 4. Can be positioned in 4 directions based on the designated Pibot
*****************************************************************************/

#pragma once

// Util
#include "NtlBitFlag.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "SurfaceGui.h"
#include "NumberGui.h"


enum eCounterDisplayType
{
	COUNTER_DISPLAY_1,		// 2D backgrounds can have different image sizes per hour
	COUNTER_DISPLAY_2,		// Flash background
};

enum eCounterQuadrant		// SetPosition The space to be sorted from the assignment specified by the function
{							//						||
	COUNTER_QUADRANT_1,		//			Quadrant 2 || 1st quadrant
	COUNTER_QUADRANT_2,		//	================== Pivot ==================
	COUNTER_QUADRANT_3,		//			Quadrant 3 || 4 quadrants
	COUNTER_QUADRANT_4		//						||
};

#define dCOUNTER_DISPLAY_FLAG_BIGGER	0x01		///< The GUI grows at certain times (COUNTER_DISPLAY_2 does not apply)


class CCounterGui : public CNtlPLGui
{
public:
	CCounterGui(const RwChar* pName);
	virtual ~CCounterGui();

	RwBool			Create(eCounterDisplayType eDisplayType, RwReal fSecond, eCounterQuadrant eQuadrant = COUNTER_QUADRANT_4);
	virtual VOID	Update(RwReal fElapsed);
	virtual VOID	Destroy();

	VOID			SetTime(RwReal fSecond);				///< Sets the current time of CCounterGui
	RwReal			GetTime();

	VOID			SetQuadrant(eCounterQuadrant eQuadrantType);

	///< Makes a "ticking" sound every second at a time less than fUsableSecond
	VOID			AlarmSound(RwBool bActive, RwReal fUsableSecond = 10.f);

	VOID			SetDisplayFlag(RwUInt8 byFlag);
	VOID			UnsetDisplayFlag(RwUInt8 byFlag);

	VOID			Pause(RwBool bPause);
	RwBool			IsPause();

protected:
	CCounterGui() {}

	VOID			SetSizeRate(RwReal fRate);

	VOID			RelocatePosition_1();
	VOID			RelocatePosition_2();

	VOID			LoadSurface_1();
	VOID			LoadSurface_2();

	VOID			LoadNumberSurface();

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnPaintDisplay_1();
	VOID			OnPaintDisplay_2();

	VOID			OnFrameCallback_Display2(const char* pcParam, const char* pcParam2);

protected:
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaintDisplay_1;
	gui::CSlot			m_slotPaintDisplay_2;
	gui::CSlot			m_slotFrameCallbackDisplay2;

	gui::CFlash*		m_pflashBackground;

	CNumberGui			m_MinuteNumberGui;		///< Display in minutes
	CNumberGui			m_SecondNumberGui;		///< Display in seconds

	CSurfaceGui			m_srfNumberBack;

	eCounterDisplayType	m_eDisplayType;
	eCounterQuadrant	m_eQuadrant;

	RwReal				m_fElapsed;				///< Count remaining time
	RwReal				m_fUsableAlarmTime;		///< When m_fElapsed becomes less than this time, a ticking sound is heard.
	RwReal				m_fSizeRate;			///< Size ratio of Number

	RwInt32				m_iAlarmElapsed;		///< The time ticked

	RwUInt8				m_byDisplayFlag;
	
	CPos				m_OriginalNumberBackGuiSize;

	RwBool				m_bPause;
	RwBool				m_bAlarmSound;
	RwBool				m_bRender;
};


inline VOID CCounterGui::SetTime(RwReal fSecond)
{
	m_fElapsed		= fSecond;
	m_iAlarmElapsed	= (RwInt32)fSecond;
}

inline RwReal CCounterGui::GetTime()
{
	return m_fElapsed;
}

inline VOID CCounterGui::AlarmSound(RwBool bActive, RwReal fUsableSecond /* = 10.f */)
{
	m_bAlarmSound		= bActive;
	m_fUsableAlarmTime	= fUsableSecond;
	m_iAlarmElapsed		= (RwInt32)fUsableSecond;
}

inline VOID CCounterGui::SetDisplayFlag(RwUInt8 byFlag)
{
	BIT_FLAG_SET(m_byDisplayFlag, byFlag);
}

inline VOID CCounterGui::UnsetDisplayFlag(RwUInt8 byFlag)
{
	BIT_FLAG_UNSET(m_byDisplayFlag, byFlag);
}

inline VOID CCounterGui::Pause(RwBool bPause)
{
	m_bPause = bPause;
}

inline RwBool CCounterGui::IsPause()
{
	return m_bPause;
}