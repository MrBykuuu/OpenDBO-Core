/******************************************************************************
*File           : DojoInfo_Base.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : January 16, 2009
*Abstract		: 
*****************************************************************************
*Desc           : Tab parent class of stamp information window
*****************************************************************************/

#pragma once

// Shared
#include "NtlSharedType.h"

// Core
#include "ceventhandler.h"

// Presentatione
#include "NtlPLGui.h"

class CDojoInfo_Base
{
public:
	virtual ~CDojoInfo_Base(VOID) {}

	virtual RwBool		Create(CNtlPLGui* pParent) = 0;
	virtual VOID		Update(RwReal fElapsed) = 0;
	virtual VOID		Destroy() = 0;

	virtual VOID		Clear() = 0;
	virtual VOID		ActiveTab(bool bActive) = 0;	

	virtual VOID		OnMouseDown(const CKey& key) = 0;
	virtual VOID		OnMouseUp(const CKey& key) = 0;
	virtual VOID		OnMove(RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent) = 0;

	virtual VOID		OnPaint() = 0;

	virtual VOID		SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex = INVALID_TBLIDX) = 0;
	virtual VOID		HandleEvents( RWS::CMsg &msg ) = 0;
};