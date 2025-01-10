/******************************************************************************
*File			: TMQStatusGui.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2007. 10. 29
*Abstract		: 
*****************************************************************************
*Desc			: TMQ progress window
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Share
#include "NtlTimeQuest.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

// Dbo
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "NumberGui.h"

class CLittleQuestContainer;

class CTMQStatusGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sTimeBonus
	{
		RwReal		fElapsed;
		RwBool		bShow;
		CSurfaceGui	srfTimeBonus;
		CNumberGui	TimeBonusNum;
	};

	CTMQStatusGui(const RwChar* pName);
	virtual ~CTMQStatusGui();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTMQStatusGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetTMQPoint(RwInt32 iPoint);
	VOID			ShowBonusTime(RwReal fBonusTime);
	VOID			CheckInfoWindow();

	VOID			OnClicked_MinimizeButton(gui::CComponent* pComponent);
	VOID			OnClicked_SenarioButton(gui::CComponent* pComponent);
	VOID			OnClicked_InfoButton(gui::CComponent* pComponent);
	VOID			OnClicked_NextButton(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnPostPaint();

protected:		
	static CTMQStatusGui*	m_pInstance;

	gui::CSlot			m_slotMinimizeButton;
	gui::CSlot			m_slotSenarioButton;
	gui::CSlot			m_slotStageButton;
	gui::CSlot			m_slotNextButton;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	gui::CStaticBox*	m_pDialogName;					///< Dialog name static
	gui::CStaticBox*	m_pInfo;						///< 'Information'
	gui::CStaticBox*	m_pPoint;						///< 'Point'
	gui::CStaticBox*	m_pCurPoint;					///< Current point
	gui::CStaticBox*	m_pQuest;						///< 'Quest'
	gui::CStaticBox*	m_pQuestTitle;					///< Quest title
	gui::CStaticBox*	m_pQuestAim_1;					///< Quest Goal 1
	gui::CStaticBox*	m_pQuestAim_2;					///< Quest Objective 2
	gui::CStaticBox*	m_pQuestAim_3;					///< Quest Goal 3
	gui::CStaticBox*	m_pQuestAim_4;					///< Quest Goal 4
	
	gui::CButton*		m_pMinimizeButton;				///< Minimize button
	gui::CButton*		m_pStageButton;					///< Stage information
	gui::CButton*		m_pSenarioButton;				///< Scenario button
	gui::CButton*		m_pNextButton;					///< Next quest button

	gui::CFlickerEffect	m_SenarioButtonFlicker;
	gui::CFlickerEffect	m_StageButtonFlicker;

	CSurfaceGui			m_srfGaugeBack;					///< TMQ Point White Paper
	CSurfaceGui			m_srfTMQContourLine_1;			///< Divider line
	CSurfaceGui			m_srfTMQContourLine_2;			///< Divider line
	CSurfaceGui			m_srfTMQContourLine_3;			///< Divider line
	CSurfaceGui			m_srfCouponSlot;				///< Coupon Slot

	gui::CProgressBar*	m_pTMQGauge;					///< TMQ Gauge

	CLittleQuestContainer* m_pTMQQuestContainer;
	CRegularSlotGui			m_CouponSlot;					///< Coupon Slot
};