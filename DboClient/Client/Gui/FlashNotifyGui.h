/******************************************************************************
*File: FlashNotifyGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : November 21, 2007
*Abstract		: 
*****************************************************************************
*Desc           : Reads and displays the flash file specified according to the notification message.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

class CFlashNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CFlashNotifyGui(const RwChar* pName);
	virtual ~CFlashNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CFlashNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	RwBool			LoadFlash(const char* acFileName, RwBool bFrameCallback = FALSE);
	VOID			LocateComponent();
	VOID			FinishFlash();

	VOID			OnMovieEnd(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pFlashBackground;

	std::string			m_strSecondFile;		///< Name of the file to be played second
	RwBool				m_bFinishNotify;
};