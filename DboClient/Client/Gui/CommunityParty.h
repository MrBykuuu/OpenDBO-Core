/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (?)NTL
* Date			: 2006. 9. 13S
* Abstract		: 
*****************************************************************************
*Desc: Community-related dialog template
*The content of each page is set according to PageType
*Messages and events are also delivered to the activated Page.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlParty.h"

// Gui

#include "gui_panel.h"

// Simulation
#include "NtlSLParty.h"

// Dbo
#include "CommunityBase.h"
#include "SlotGui.h"
#include "Windowby3.h"

class CCommunityParty : public CCommunityBase
{
public:
	enum eZennyDivType
	{
		ZDT_PICKUP_PERSON,		///< The person who gave it owns it.
		ZDT_EQUAL,				///< joint distribution

		NUM_ZDT
	};

	enum eItemDivType
	{
		IDT_PICKUP_PERSON,		///< The person who gave it owns it.
		IDT_ORDER,				///< in order
		IDT_GRADE_ITEM,			///< Contribution distribution according to item grade
		IDT_CONTIBUTION,		///< Contribution distribution

		NUM_IDT
	};

	struct PartyMemberUI
	{
		SERIAL_HANDLE		hSerial;	///< Member's cereal
		gui::CStaticBox*	pName;		///< Party member name
		gui::CStaticBox*	pScore;		///< Party member contribution points
		gui::CPanel*		pTributarySimbol;///< Illustration of races and occupations

		PartyMemberUI()
		:pName(NULL)
		,pScore(NULL)
		{
		}
	};

	///< 3-part window divided into left/center/right and enlarged respectively
	struct Windowby3						
	{
		CSurfaceGui			Left;
		CSurfaceGui			Center;
		CSurfaceGui			Right;

		VOID	Render()
		{
			Left.Render();
			Center.Render();
			Right.Render();
		}
	};

	CCommunityParty();
	virtual ~CCommunityParty();

	virtual RwBool		Create(CNtlPLGui* pParent);
	virtual VOID		Update(RwReal fElapsed);
	virtual VOID		Destroy();

	virtual VOID		SwitchDialog(bool bOpen);

	virtual VOID		Clear();
	virtual VOID		ActiveTab(bool bActive);

	virtual VOID		HandleEvents( RWS::CMsg &msg );	
	
	virtual VOID		OnMouseDown(const CKey& key);
	virtual VOID		OnMouseUp(const CKey& key);
	virtual VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent);

	virtual VOID		OnPaint();

protected:	
	VOID		PartyLeave();

	VOID		SetZennyDivision(RwUInt8 byIndex);
	VOID		SetItemDivision(RwUInt8 byIndex);

	VOID		LockZennyButton(bool bLock);
	VOID		LockItemButton(bool bLock);

	VOID		SetMember(sPartyMember* pMember, RwUInt8 byIndex);
	VOID		DelMember(RwUInt8 byIndex);

	VOID		CheckInfoWindow();

	VOID		OnGotFocus();
	VOID		OnLostFocus();	

	VOID		OnReturn_PartyNameInput();

	VOID		ClickedPartyCreateButton(gui::CComponent* pComponent);
	VOID		ClickedSelectDivButton(gui::CComponent* pComponent);
	VOID		ClickedLockButton(gui::CComponent* pComponent);

	VOID		OnClicked_InvenButton(gui::CComponent* pComponent);
	VOID		OnClicked_CharmButton(gui::CComponent* pComponent);
	VOID		OnClicked_CPButton(gui::CComponent* pComponent);
	VOID		OnClicked_ItemtoCPButton(gui::CComponent* pComponent);

	VOID		LocateLeaderMark(RwInt32 iIndex);	///< Mark the position of the leader. If iIndex <= 0, it is not displayed.

	VOID		CalcSurfacePosition();

	VOID		AdjustSurface(gui::CSurface& surface);	///< Move the surface to the coordinates to be used in the button

protected:	
	gui::CSlot			m_slotReturnInput;
	gui::CSlot			m_slotSelectZennyDivButton;
	gui::CSlot			m_slotSelectItemDivButton;
	gui::CSlot			m_slotZennyLockButton;
	gui::CSlot			m_slotItemLockButton;
	gui::CSlot			m_slotPartyCreateButton;
	gui::CSlot			m_slotInvenButton;
	gui::CSlot			m_slotCharmButton;
	gui::CSlot			m_slotCPButton;
	gui::CSlot			m_slotItemtoCPButton;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;

	CNtlPLGui*			m_pParent;					///< Owner class	

	gui::CStaticBox*	m_pPartyName;				///< 'Party name'
	CSurfaceGui			m_PartyNameBack;			///< Party Name Backboard

	gui::CStaticBox*	m_pMyPartyNameStatic;		///< Current party name
	gui::CInputBox*		m_pPartNameInput;			///< Party name input field
	gui::CButton*		m_pPartyButtonCreate;		///< Create party



	CSurfaceGui			m_ZennySurface;				///< 'Zenny' surface
	CSurfaceGui			m_ZennyDivSurface[NUM_ZDT];	///< Zenny distribution method
	gui::CButton*		m_pSelectZennyDivButton;	///< Zenny distribution method change button
	CSurfaceGui			m_ZennyExplainPanel;		///< Zenny distribution explanation background
	gui::CStaticBox*	m_pZennyDivExplain;			///< Zenny distribution explanation
	gui::CButton*		m_pZennyLockButton;			///< Zenny dispense lock button

	CSurfaceGui			m_ItemSurface;				///< 'Item' surface
	CSurfaceGui			m_ItemDivSurface[NUM_IDT];	///< Item distribution method
	gui::CButton*		m_pSelectItemDivButton;		///< Item distribution method change button
	CSurfaceGui			m_ItemExplainPanel;			///< Item distribution explanation background
	gui::CStaticBox*	m_pItemDivExplain;			///< Description of item distribution
	gui::CButton*		m_pItemLockButton;			///< Item distribution lock button


	CSurfaceGui			m_MemberHeadLine;			///< Party member headline
	gui::CStaticBox*	m_MemberStatic;				///< 'Party member'
	gui::CStaticBox*	m_ScoreStatic;				///< ¡®Contribution score¡¯
	CSurfaceGui			m_LeaderMark;				///< Party Leader Mark
	CSurfaceGui			m_srfMemberboard;			///< Member list background
	PartyMemberUI		m_PartyMemberInfo[NTL_MAX_MEMBER_IN_PARTY];	///< Party member information	


	gui::CButton*		m_PartyInvenButton;			///< Party inventory button		
	gui::CButton*		m_pCharmButton;				///< Amulet button
	CSlotGui			m_PartyCharmSlot[NTL_PARTY_CHARM_INVENTORY_COUNT];	///< Party talisman slot
	CSurfaceGui			m_CharmBackpanel;			///< Amulet panel
	
	CSurfaceGui			m_CPHeadLine;				///< CP Headline
	gui::CStaticBox*	m_CPStatic;					///< 'CP conversion'
	gui::CStaticBox*	m_CPPoint;					///<Actual CP held
	gui::CStaticBox*	m_CPExplain;				///< CP Description
	gui::CButton*		m_pCPButton;				///< CP button
	gui::CButton*		m_pItemtoCPButton;			///< Convert item to CP button
	
	RwInt32				m_iLeaderIndex;				///< Party leader index
	RwInt8				m_byInfoWindowIndex;

	RwUInt8				m_byItemDivisionType;		///<Currently drawn item distribution method
	RwUInt8				m_byZennyDivisionType;		///< Current Zenny distribution method

	RwUInt8				m_bySelectedItemDivisionType;	///< Current item distribution method
	RwUInt8				m_bySelectedZennyDivisionType;	///< Current Zenny distribution method
};