/******************************************************************************
*File			: TutorialGui.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2008. 5. 11
*Abstract		: 
*****************************************************************************
*Desc			: Appears when the user achieves the goal during the tutorial.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"


class CTutorialConfirmGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	struct sShowHide
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwBool				bShow;
	};

	CTutorialConfirmGui(const RwChar* pName);
	virtual ~CTutorialConfirmGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTutorialConfirmGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool			LoadFlash(const char* acFileName);
	VOID			LocateComponent();
	VOID			FinishFlash();

protected:
	gui::CFlash*		m_pflashConfirm;

	sShowHide			m_ShowHide;
	RwInt32				m_iOffsetX, m_iOffsetY;
};