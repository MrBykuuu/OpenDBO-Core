/******************************************************************************
* File			: CBackboardGui.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2007. 8. 8
* Abstract		: 
*****************************************************************************
* Desc			: Draws a rectangle that covers the entire screen according to the specified color and transparency.
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SurfaceGui.h"


class CBackboardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CBackboardGui(const RwChar* pName);
	virtual ~CBackboardGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	VOID		SetAlpha(RwUInt8 byAlpha);

	RwInt32		SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

protected:
	CBackboardGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:	
	gui::CSlot			m_slotPaint;

	CSurfaceGui			m_srfBack;
};