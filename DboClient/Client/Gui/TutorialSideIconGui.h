/******************************************************************************
* File			: TutorialSideIconGui.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 1. 22
* Abstract		: 
*****************************************************************************
* Desc			: When you press the tutorial side icon, the side view is displayed.
*                 Instead, a message box pops up asking if you want to end the tutorial.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "SideIconGui.h"

class CTutorialSideIconGui : public CSideIconBase
{
public:
	CTutorialSideIconGui(const RwChar* pName);
	virtual ~CTutorialSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);	

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};


class CTutorialSideViewGui : public CSideViewBase
{
public:
	CTutorialSideViewGui(const RwChar* pName);
	virtual ~CTutorialSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	
};