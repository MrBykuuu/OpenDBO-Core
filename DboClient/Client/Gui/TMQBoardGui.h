/******************************************************************************
*File			: TMQBoardGui.h
*Author		    : Hong SungBock
*Copyright		: (��)NTL
*Date			: 2007. 7. 3
*Update		: 1/24/2019
*Abstract		: 
*****************************************************************************
*Desc			: Shows a list of scheduled time machine quests.
*****************************************************************************/
#pragma once

// Core
#include "ceventhandler.h"

// Share
#include "NtlTimeQuest.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SurfaceGui.h"
#include "SlotGui.h"

struct sTIMEQUEST_TBLDAT;

class CTMQBoardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CTMQBoardGui(const RwChar* pName);
	virtual ~CTMQBoardGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		Clear();

	SERIAL_HANDLE	GetNPCHandle();

	int			SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

protected:

	virtual VOID	HandleEvents( RWS::CMsg &msg );

	void			CheckInfoWindow();

	void			ClickedCloseButton(gui::CComponent* pComponent);
	void			ClickedRequestPartyMatchingButton(gui::CComponent* pComponent);
	void			ClickedDirectEnterButton(gui::CComponent* pComponent);

	void			OnPaint();
	void			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void			OnMouseLeave(gui::CComponent* pComponent);
	void			OnSelectDifficulty(INT nSelectIndex);

private:

	bool			LoadTmqInfo(HOBJECT hNPCSerial);
	void			LoadItem();

protected:

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCloseButton;	
	gui::CSlot			m_slotbtnRequestPartyMatching;
	gui::CSlot			m_slotbtnDirectEnter;
	gui::CSlot			m_slotbtnBottomClose;
	gui::CSlot			m_slotbtnDifficultyCombo;

	gui::CStaticBox*	m_pTitle;

	gui::CHtmlBox*		m_phtbQuestSenario;

	gui::CStaticBox*	m_pLimitedLevel;
	gui::CStaticBox*	m_pLimitedLevelValue;

	gui::CStaticBox*	m_pstbQuestName;

	gui::CStaticBox*	m_pstbDifficult;

	gui::CStaticBox*	m_pstbLimitedTime;

	gui::CStaticBox*	m_pstbFatigability;

	gui::CStaticBox*	m_pstbLimitedTimeValue;

	gui::CStaticBox*	m_pstbFatigabilityValue;

	gui::CComboBox*		m_pbtnDifficultyCombo;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pbtnRequestPartyMatching;
	gui::CButton*		m_pbtnDirectEnter;
	gui::CButton*		m_pbtnBottomClose;

	CRegularSlotGui		m_NeedItem;

	SERIAL_HANDLE		m_hNPCSerial;
	BYTE				m_byDifficulty;

	sTIMEQUEST_TBLDAT*	m_pTIMEQUEST_TBLDAT;
};