/******************************************************************************
*File			: ScouterMenuGui.h
*Author		    : Hong SungBock
*Copyright		: (��)NTL
*Date			: 2006. 10. 30
*Abstract		: 
*****************************************************************************
*Desc			: ScouterMenuGui
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Share
#include "NtlItem.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SlotGui.h"

class CScouterPCInfoViewerGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct PCEquipItem
	{
		CRegularSlotGui			SlotItem;
		sITEM_PROFILE		ItemProfile;
	};

	CScouterPCInfoViewerGui(const RwChar* pName);
	virtual ~CScouterPCInfoViewerGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

protected:
	CScouterPCInfoViewerGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			ShowPCInfo(SERIAL_HANDLE hTarger, RwUInt8 byCount, sITEM_PROFILE* pItemProfile);	///< Show the opponent's equipment
	VOID			CheckInfoWindow();

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnAbsoluteClick(const CKey& key);

protected:		
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;

	gui::CButton*		m_pCloseButton;

	CSurfaceGui			m_srfWeaponBack;
	CSurfaceGui			m_srfDefneceBack;
	CSurfaceGui			m_srfAccessaryBack;
	CSurfaceGui			m_srfSpecialBack;
	CSurfaceGui			m_srfCostumeBack;

	CSurfaceGui			m_srfLine;
	CSurfaceGui			m_srfLine2;
	CSurfaceGui			m_srfLine3;
	CSurfaceGui			m_srfLine4;
	CSurfaceGui			m_srfLine5;

	gui::CStaticBox*	m_pStaticTitle;
	gui::CStaticBox*	m_pStaticWeapon;
	gui::CStaticBox*	m_pStaticDefence;
	gui::CStaticBox*	m_pStaticAccesary;
	gui::CStaticBox*	m_pStaticSpecial;
	gui::CStaticBox*	m_pstbCostume;

	RwInt8				m_byInfoWindowIndex;
	RwReal				m_fElapsed;
	PCEquipItem			m_Item[EQUIP_SLOT_TYPE_COUNT];		///< Item information
};