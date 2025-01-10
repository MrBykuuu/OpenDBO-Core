/******************************************************************************
*File           : GMGuiGroup.h
*Author		    : Haesung, Cho
*Copyright	    : NTL Co., Ltd.
*Date           : November 18, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Group of GUI used when entering GM Mode
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

/**
* \ingroup GMÀÇ Gui Group
*/

class CGMQuickSlotGui;

class CGMGuiGroup : public RWS::CEventHandler
{
public:
	CGMGuiGroup();
	~CGMGuiGroup();

	RwBool	Create();
	VOID	Destroy();

	VOID	HandleEvents( RWS::CMsg& msg );	

protected:
	CGMQuickSlotGui*			m_pGMQuickSlot;
};