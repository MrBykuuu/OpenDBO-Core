/******************************************************************************
*File           : NetPyShopCartGui.h
*Author		    : Cho Haesung
*Copyright	    : NTL Co., Ltd.
*Date           : June 25, 2009
*Abstract		: 
*****************************************************************************
*Desc: Class that defines NetPyShop¡¯s Cart GUI and functions among commercial functions
*****************************************************************************/

#ifndef __NETPYSHOP_CART_GUI_H__
#define __NETPYSHOP_CART_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlPacketUG.h"

// Gui
#include "eventtimer.h"
#include "gui_slot.h"
#include "gui_button.h"
#include "gui_staticbox.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SlotGui.h"
#include "Windowby3.h"

struct sSHOP_BUY_CART;
struct SDboEventNetPyShopEvent;

#define dNETPYSHOP_CART_INVALID_INDEX		(-1)

class CNetPyShopCartGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CNetPyShopCartGui(const RwChar* pName);
	virtual ~CNetPyShopCartGui();

	enum eNetPyShopCartDefinedValue
	{		
		MAX_SLOT				= 12,	///< Maximum number of slots provided for Buy and Sell		
	};

	struct MouseDownSlot
	{
		RwInt32		iSlotKind;
		RwInt32		iSlotIndex;

		MouseDownSlot()
		{
			iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
			iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;
		}
	};

	enum
	{
		BUY_SLOT,

		SLOTKIND_NUM
	};

	struct BuySlotInfo
	{
		sSHOP_BUY_CART		NPCShopBuyInfo;		///< Information to be passed on to the server		
		CRegularSlotGui			slot;
	};

	RwBool			Create();
	VOID			Destroy();

	VOID			OpenCart();		///< Open the cart UI
	VOID			CloseCart();						///< Close the cart.

	RwInt32			SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

	///< Add or subtract items you want to buy or sell.
	///< iSlotKind : BUY_SLOT
	///< iSlotY : Trade Slot Index
	///< iCount: Number
	VOID			AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount);
	VOID			SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount);

protected:
	CNetPyShopCartGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &pMsg );

	VOID			Clear();									///< Delete trade cart information.
	VOID			ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot);///< Deletes the contents of the slot.

	VOID			AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount);	///< Adds or subtracts the number of items in a slot.
	VOID			UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount);

	VOID			CalcTotalBuyPrice();						///< Calculate total purchase amount

	///< Categorized according to how you upload items from NPCShop or Inven
	VOID			RegBuyItemByDrag(RwInt32 iSlot);	
	VOID			RegBuyItemByEvent(RwInt32 iSlot, SDboEventNetPyShopEvent& TradeInfo);	

	RwInt32			FindEmptySlot(RwInt32 iSlotType);			///< Find the earliest empty slot by slot type.
	RwInt32			FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial);
	RwInt32			PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();							///< Close the info window depending on the situation.

	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotKind = 0, RwInt32 iSlotIdx = -1 );		///< Slot focus effect

	VOID			OnPaint();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedBuyButton(gui::CComponent* pComponent);		///< Buy the selected items.
	
	VOID			ClickUpButton(gui::CComponent* pComponent);	///<Pressed the item number plus button
	VOID			ClickDownButton(gui::CComponent* pComponent);	///<Pressed the item count subtraction button

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouse;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotBtnZenny;
	gui::CSlot			m_slotClickedBuy;
	gui::CSlot			m_slotUpButton[SLOTKIND_NUM][MAX_SLOT];
	gui::CSlot			m_slotDownButton[SLOTKIND_NUM][MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;
	MouseDownSlot		m_MouseDownSlot;			///< Slot pressed with mouse

	gui::CTexture*		m_pMoneyIconTexture;

	gui::CButton*		m_pExitButton;				///< Close window button
	gui::CButton*		m_pBuyButton;				///< Purchase button

	gui::CButton*		m_pUpButton[SLOTKIND_NUM][MAX_SLOT];	///< Item number plus button
	gui::CButton*		m_pDownButton[SLOTKIND_NUM][MAX_SLOT];	///< Item count minus button

	gui::CStaticBox*	m_pDialogName;				///< Dialog name static
	gui::CStaticBox*	m_pTotalBuyMoney;			///< Total purchase amount
	gui::CStaticBox*	m_pTotalSellMoney;			///< Total sales amount

	CSurfaceGui			m_FocusEffect;				///< Slot focus effect
	CSurfaceGui			m_BuyBar;					///< fraud surface

	RwBool				m_bFocus;					///< Got focus.	

	RwInt32				m_iCurTabIndex;				///< current tab index

	BuySlotInfo			m_BuySlotInfo[MAX_SLOT];	///< Left slot information
	
	RwUInt32			m_uiTotalBuyPrice;			///< Total purchase amount
};


#endif