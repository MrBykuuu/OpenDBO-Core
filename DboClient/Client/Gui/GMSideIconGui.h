/******************************************************************************
*File           : GMSideIconGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : November 17, 2008
*Abstract		: 
*****************************************************************************
*Desc           : GM side icon
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "SideIconGui.h"

class CGMSideIconGui : public CSideIconBase
{
public:
	CGMSideIconGui(const RwChar* pName);
	virtual ~CGMSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);	

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};


class CGMSideViewGui : public CSideViewBase
{
public:
	CGMSideViewGui(const RwChar* pName);
	virtual ~CGMSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	
};