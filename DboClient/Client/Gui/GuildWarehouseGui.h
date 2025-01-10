/******************************************************************************
*File           : CGuildWarehouseGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : February 22, 2008
*Abstract		: 
*****************************************************************************
*Desc           :It does not utilize the existing CWarehouse.
*                In case GuildWarehouse is planned differently in the future, etc.
*                CWarehouse class in case the plan changes in the future.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Share
#include "NtlItem.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SlotGui.h"


struct sGuildWarehouseSlot;

//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseGui
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGuildWarehouseGui(const RwChar* pName);
	virtual ~CGuildWarehouseGui();

	virtual RwBool Create();
	virtual VOID Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	VOID		RegisterWarehouseIndex(RwUInt8 byIndex);
	VOID		SetItem(sGuildWarehouseSlot* pSlotData, RwUInt8 byPos);

protected:
	CGuildWarehouseGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Init();

	VOID			SetupItems();

	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);				///< Find the slot where the mouse entered the area.
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			ShowDisableSlot(RwBool bShow, RwUInt8 bySlot);
	VOID			CheckInfoWindow();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	virtual VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);

	VOID			OnPaint();
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pExitButton;		///< Close window button

	RwInt8				m_byInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;	///< Index of the slot pressed by the mouse
	RwInt32				m_iSelectedSlot;	///< Select effect slot
	RwInt32				m_iClickEffectedSlot;///< Slot with current click effect

	gui::CStaticBox*	m_pDialogName;		///< Dialog name static

	CSurfaceGui			m_FocusEffect;		///< Slot focus effect
	CSurfaceGui			m_SelectEffect;		///< Slot select effect

	RwUInt8				m_byWarehouseIndex;	///<What warehouse is it?

	RwBool				m_bFocus;			///< Got focus.

	CRegularSlotGui			m_Slot[NTL_MAX_GUILD_ITEM_SLOT];	///< Item slot
};


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseAddon
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseAddonGui : public CGuildWarehouseGui
{
public:
	CGuildWarehouseAddonGui(const RwChar* pName);
	virtual ~CGuildWarehouseAddonGui();

	virtual RwBool Create();
	virtual VOID Destroy();

	VOID	SetZenny(RwUInt32 uiZenny);

protected:
	CGuildWarehouseAddonGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			ClickedZennyButton(gui::CComponent* pComponent);

	VOID			OnPostPaint();

protected:
	gui::CSlot			m_slotZennyButton;
	gui::CSlot			m_slotPostPaint;

	gui::CButton*		m_pZennyButton;
	gui::CStaticBox*	m_pZenny;
	gui::CTexture*		m_pMoneyIconTexture;

	CSurfaceGui			m_srfZennySlotDestination;	///< Zenny Slot Destination Image

	RwBool				m_bZennySlotDestination;
};


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseBar
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseBar : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGuildWarehouseBar(const RwChar* pName);
	virtual ~CGuildWarehouseBar();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);		///< Open/Close in DialogManager

protected:
	CGuildWarehouseBar() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Clicked_1_Button(gui::CComponent* pComponent);
	VOID			Clicked_2_Button(gui::CComponent* pComponent);
	VOID			Clicked_3_Button(gui::CComponent* pComponent);
	VOID			ClickedAllButton(gui::CComponent* pComponent);
	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	static CGuildWarehouseBar*	m_pInstance;

	gui::CSlot			m_slotWarehouseBtn[NTL_MAX_GUILD_BANK_COUNT];
	gui::CSlot			m_slot_All_Button;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pWarehouseBtn[NTL_MAX_GUILD_BANK_COUNT];///< warehouse button
	gui::CButton*		m_p_All_Button;		///< All warehouses button
	gui::CButton*		m_pExitButton;		///< Close window button

	CGuildWarehouseGui*		m_pWareHouseGui[NTL_MAX_GUILD_BANK_COUNT];
};