/******************************************************************************
*File			: PartyMenu.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Abstract		: 2006. 10. 13
*****************************************************************************
*
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Shared
#include "NtlParty.h"
#include "NtlSkill.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlSLParty.h"

// Dbo
#include "SlotGui.h"


class CPartyMemberGui;


class CPartyMenu : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	struct sMenuShape
	{
		bool				bShowBuff;
		bool				bShowMember;
	};

	CPartyMenu(const RwChar* pName);
	virtual ~CPartyMenu();

	typedef std::list<CPartyMemberGui*>				PARTYMEMBER_LIST;
	typedef std::list<CPartyMemberGui*>::iterator	PARTYMEMBER_ITER;
	
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

	VOID		ResetPartyMenu();					///< Initialize PartyMenu

protected:
	CPartyMenu() {}
	virtual VOID HandleEvents( RWS::CMsg &msg );

	VOID		Clear();

	VOID		AddMember(sPartyMember* pMember);				///< Increase the member window by one.
	VOID		DelMember(SERIAL_HANDLE hSerial);				///< Reduces the member window by one.

	CPartyMemberGui* GetPartyMemberGui(SERIAL_HANDLE hSerial);	///< Returns the member window.
	
	VOID		SetOtherWorld(); // Displays the GUI of members who exist in a world whose rules are different from those of the world where the avatar exists.

	VOID		NoPartyButtons();
	VOID		SpreadButtons();
	VOID		FoldButtons();

	VOID		SwitchMemberGui(bool bOpen);
	VOID		SwitchMemberBuff(bool bOpen);

	VOID		CheckPopup();

	VOID		MemberButton_UpdateTooltip();

	VOID		OnClick_MemberMenuButton(gui::CComponent* pComponent);
	VOID		OnClick_DevideZennyButton(gui::CComponent* pComponent);
	VOID		OnClick_DevideItemButton(gui::CComponent* pComponent);
	VOID		OnClick_DungeonButton(gui::CComponent* pComponent);
	VOID		OnClick_SwitchButton(gui::CComponent* pComponent);

	VOID		OnClick_SpreadMenuButton(gui::CComponent* pComponent);
	VOID		OnClick_FoldMenuButton(gui::CComponent* pComponent);

	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID		OnPaint();

protected:
	gui::CSlot			m_slotSpreadMenuButton;
	gui::CSlot			m_slotFoldMenuButton;
	gui::CSlot			m_slotMemberMenuButton;
	gui::CSlot			m_slotSwitchButton;
	gui::CSlot			m_slotDivideZennyButton;
	gui::CSlot			m_slotDivideItemButton;
	gui::CSlot			m_slotDungeonButton;
	gui::CSlot			m_slotPartyCharmButton;
	gui::CSlot			m_slotChangeMouseMove;
	gui::CSlot			m_slotChangeMouseLeave;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_pPartyName;

	gui::CButton*		m_pSpreadMenuButton;		///< Menu expansion button
	gui::CButton*		m_pFoldMenuButton;			///< Menu Collapse Button

	gui::CButton*		m_pMemberMenuButton;		///< Party member management button	
	gui::CButton*		m_pDivideZennyButton;		///< Zenny distribution method button
	gui::CButton*		m_pDivideItemButton;		///< Item distribution method button
	gui::CButton*		m_pDungeonButton;			///< Dungeon button
	gui::CButton*		m_pSwitchButton;			///< switch button in party menu/member window

	CSurfaceGui			m_srfPartyNameBackground;

	sMenuShape			m_MenuShape;
	
	RwReal				m_fUpdateTimeAbsentee;

	PARTYMEMBER_LIST	m_listPartyMember;			///< Party member window list
};