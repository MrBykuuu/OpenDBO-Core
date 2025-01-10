/*****************************************************************************
 *
 *File			: DBOTitle.h
 *Author	    : Hong Sungbock
 *Copyright  	: (аж)NTL
 *Date			: 2009. 4. 2
 *Abstract		: 
 *****************************************************************************
 *Desc          : DBO Title screen when starting game
 *
 *****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

class CDBOTitleGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDBOTitleGui(const RwChar *pName);
	~CDBOTitleGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();
	
protected:
	CDBOTitleGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SwitchEnableButtons(bool bEnable);	///< Availability settings for buttons other than the Exit button

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			OnClicked_CinemaButton(gui::CComponent* pComponent);
	VOID			OnClicked_ContractButton(gui::CComponent* pComponent);
	VOID			OnClicked_ConnectGameServerButton(gui::CComponent* pComponent);
	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);

	VOID			OnPaint();

	int				CallBackMoviePlayStop();

	VOID			LogInStageEnterEventHandler(RWS::CMsg &msg);
	VOID			LogInStageExitEventHandler(RWS::CMsg &msg);
	VOID			LogInStageTimeOutEventHandler(RWS::CMsg &msg);
	
protected:
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCinemaButton;
	gui::CSlot			m_slotContractButton;
	gui::CSlot			m_slotConnectGameServerButton;
	gui::CSlot			m_slotExitButton;

	gui::CButton*		m_pCinemaButton;			///< Video play button
	gui::CButton*		m_pContractButton;			///< View terms and conditions button
	gui::CButton*		m_pConnectGameServerButton;	///< Game server connection button
	gui::CButton*		m_pExitButton;				///< Exit button

	gui::CStaticBox*	m_pVersion;					///< Display Dbo version

	gui::CFlash*		m_pFlashBackground;

	CSurfaceGui			m_srfTitle;					///< Title Surface
};