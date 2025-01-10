/******************************************************************************
*File			: TutorialMessageGui.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2008. 4. 23
*Abstract		: 
*****************************************************************************
*Desc			: Displays a message while the tutorial is in progress.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "Windowby3.h"


class CTutorialMessageGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sShowHide
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwBool				bShow;
	};

	struct sChangeColor
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwRGBA				srcColor;
		RwRGBA				destColor;
	};

	struct sDefaultMessageStyle
	{
		std::string			strFontName;
		RwRGBA				FontColor;
		RwRGBA				FontBackgroundColor;
		RwUInt8				byFontSize;
		RwUInt8				byAlign;
		RwInt32				iOffsetX;
		RwInt32				iOffsetY;
		RwInt32				iOffsetBackgroundX;
		RwInt32				iOffsetBackgroundY;
	};

	CTutorialMessageGui(const RwChar* pName);
	virtual ~CTutorialMessageGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTutorialMessageGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnClick_SkipButton(gui::CComponent* pComponent);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	gui::CSlot			m_slotSkipButton;
	gui::CSlot			m_slotMove;

	gui::CFlash*		m_pflashBackground;
	gui::CHtmlBox*		m_pMessage;
	gui::CButton*		m_pSkipButton;

	sShowHide			m_ShowHideMessage;	
	sChangeColor		m_ChangeColor;
	sDefaultMessageStyle m_DefaultMessageStyle;
};