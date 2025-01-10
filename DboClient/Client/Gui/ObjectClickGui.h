/******************************************************************************
*File			: ObjectClickGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2007. 11. 14
*Abstract		: 
*****************************************************************************
*Desc			: Shows the event when the trigger object is clicked
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Share
#include "NtlTimeQuest.h"

// Sound
#include "GUISoundDefine.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SurfaceGui.h"

class CObjectClickGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CObjectClickGui(const RwChar* pName);
	virtual ~CObjectClickGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

protected:
	CObjectClickGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotPaint;

	SOUND_HANDLE		m_hSound;

	gui::CRadarEffect	m_TimeEffect;

	CSurfaceGui			m_srfNeedle;				///< Needle picture
	CSurfaceGui			m_srfReflectedLight;		///< Reflected light	

	RwReal				m_fLastTime;
	RwReal				m_fRemainTime;

	RwBool				m_bActive;
	RwBool				m_bVisible;
};