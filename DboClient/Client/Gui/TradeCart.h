/******************************************************************************
*File			: TradeCart.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2006. 8. 1
*Abstract		: 
*****************************************************************************
*Desc			: Shopping cart to use when trading
*                BuySlot's Serial uses the item table
*                SellSlot¡¯s Serial uses Sob¡¯s
*****************************************************************************/

#pragma once

#include "SlotGui.h"
#include "gui_slot.h"
#include "NtlPLGui.h"
#include "NtlSLDef.h"
#include "ceventhandler.h"
#include "eventtimer.h"
#include "gui_button.h"
#include "gui_staticbox.h"
#include "NtlPacketUG.h"
#include "Windowby3.h"

struct sSHOP_BUY_CART;
struct sSHOP_SELL_CART;
struct SDboEventShop;

class CTradeCart : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CTradeCart(const RwChar* pName);
	virtual ~CTradeCart();

	enum eTradeCartDefinedValue
	{		
		MAX_SLOT				= 12,	///< Maximum number of slots provided for Buy and Sell		
	};

	enum
	{
		BUY_SLOT,
		SELL_SLOT,

		SLOTKIND_NUM
	};

	enum eTradeCartTextureType
	{
		TRADECART_TEXTURE_NORMAL,
		TRADECART_TEXTURE_TENKAICHI
	};

	struct MouseDownSlot
	{
		RwInt32		iSlotKind;
		RwInt32		iSlotIndex;

		MouseDownSlot()
		{
			iSlotKind = -1;
			iSlotIndex = -1;
		}
	};

	struct BuySlotInfo
	{
		sSHOP_BUY_CART		NPCShopBuyInfo;		///< Information to be passed on to the server		
		CRegularSlotGui			slot;
		sITEM_DATA			TakeItem;			///< Items received when trading between users
		SERIAL_HANDLE		hGiveHandle;		///< The handle of the item to be received when trading between users (exists on the server, but you do not currently have it)

		BuySlotInfo()
		:hGiveHandle(INVALID_SERIAL_ID)
		{
			memset((void*)&TakeItem, 0, sizeof(TakeItem) );
			TakeItem.itemNo = INVALID_SERIAL_ID;
		}
	};

	struct  SellSlotInfo 
	{		
		sSHOP_SELL_CART		NPCShopSellInfo;	///< Information to be passed on to the server
		CRegularSlotGui			slot;
	};

	RwBool			Create();
	VOID			Destroy();

	VOID			OpenCart(eDialogType eType, RwUInt32 uiSerial);		///< Open the cart UI
	VOID			CloseCart(eDialogType eType);						///< Close the cart.

	RwInt32			SwitchDialog(bool bOpen);			///< Open/Close in DialogManager

	RwUInt32		GetTargetSerial();

	///< Add or subtract items you want to buy or sell.
	///< iSlotKind : BUY_SLOT, SELL_SLOT
	///< iSlotY : Trade Slot Index
	///< iCount : Number
	VOID			AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);
	VOID			SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);

protected:
	CTradeCart() {}
	virtual VOID	HandleEvents( RWS::CMsg &pMsg );

	VOID			ResetCartTexture(eTradeCartTextureType eType);

	VOID			Clear();									///< Delete trade cart information.
	VOID			ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot);///< Deletes the contents of the slot.

	VOID			AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount, RwUInt32 itemSerial);	///< Adds or subtracts the number of items in a slot.
	VOID			UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount);

	VOID			CalcTotalBuyPrice();						///< Calculate total purchase amount
	VOID			CalcTotalSellPrice();						///< Calculate total sales

	///< Categorized according to how you upload items from NPCShop or Inven
	VOID			RegBuyItemByDrag(RwInt32 iSlot);
	VOID			RegSellItemByDrag(RwInt32 iSlot);

	VOID			RegBuyItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo);	
	VOID			RegSellItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo);

	// When registering an item in user trade
	VOID			RegTakeItemByPacket(RwInt32 iSlot, SERIAL_HANDLE hTakeHandle, sITEM_DATA* pITEM_DATA, RwUInt8 byCount);
	VOID			RegGiveItemByPacket(RwInt32 iSlot, RwUInt32 uiSerial, RwUInt8 byCount);

	RwInt32			FindEmptySlot(RwInt32 iSlotType);			///< Find the earliest empty slot by slot type.
	RwUInt8			FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial);
	RwInt32			PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();							///< Close the info window depending on the situation.
	
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotKind = 0, RwInt32 iSlotIdx = -1 );		///< Slot focus effect

	VOID			OnPaint();
	VOID			OnPostPaint();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			ClickedZennyButton(gui::CComponent* pComponent);

	VOID			ClickedBuyButton(gui::CComponent* pComponent);		///< Buy the selected items.
	VOID			ClickedSellButton(gui::CComponent* pComponent);		///< Sell the selected items.

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
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRightLockButton;
	gui::CSlot			m_slotRightUnlockButton;
	gui::CSlot			m_slotBtnZenny;
	gui::CSlot			m_slotClickedBuy;
	gui::CSlot			m_slotClickedSell;
	gui::CSlot			m_slotUpButton[SLOTKIND_NUM][MAX_SLOT];
	gui::CSlot			m_slotDownButton[SLOTKIND_NUM][MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;
	MouseDownSlot		m_MouseDownSlot;			///< Slot pressed with mouse

	gui::CTexture*		m_pMoneyIconTexture;
	
	gui::CButton*		m_pExitButton;				///< Close window button
	gui::CButton*		m_pZennyButton;				///< Zenny button (right Zenny button for user trading)
	gui::CButton*		m_pBuyButton;				///< Purchase button
	gui::CButton*		m_pSellButton;				///< Sell Button

	gui::CButton*		m_pUpButton[SLOTKIND_NUM][MAX_SLOT];	///< Item number plus button
	gui::CButton*		m_pDownButton[SLOTKIND_NUM][MAX_SLOT];	///< Item count minus button

	gui::CStaticBox*	m_pDialogName;				///< Dialog name static
	gui::CStaticBox*	m_pTotalBuyMoney;			///< Total purchase amount
	gui::CStaticBox*	m_pTotalSellMoney;			///< Total sales amount

	CSurfaceGui			m_FocusEffect;				///< Slot focus effect
	CSurfaceGui			m_BuyBar;					///< fraud surface
	CSurfaceGui			m_SellBar;					///< sell surface
	CSurfaceGui			m_GiveBar;					///< Get Surface
	CSurfaceGui			m_TakeBar;					///< Periodic surface
	CSurfaceGui			m_srfZennySlotDestination;	///< Zenny Slot Destination Image

	CWindowby3			m_LeftLockSurface;			///< Left locked surface
	CWindowby3			m_RightLockSurface;			///< Right locked surface

	RwBool				m_bFocus;					///< Got focus.
	bool				m_bLeftLock;				///< Left item registration locked
	bool				m_bRightLock;				///< Right item registration locked
	RwBool				m_bZennySlotDestination;

	eTradeCartTextureType m_eTextureType;			///< Cart texture type
	RwUInt32			m_uiSubjectSerial;			///< Serial to TradeCart
	eDialogType			m_eDialogType;				///< What kind of transaction is this cart for?
	RwInt32				m_iCurTabIndex;				///< current tab index

	BuySlotInfo			m_BuySlotInfo[MAX_SLOT];	///< Left slot information
	SellSlotInfo		m_SellSlotInfo[MAX_SLOT];	///< Right slot information

	RwUInt32			m_uiTotalSellPrice;			///< Total sales amount

	RwUInt8				m_byOthersEmptySlot;		///< When trading between users, the number of empty slots available for trading by the other party
	RwUInt32			m_uiUserTradePacketCount;	///< Number of packets updated during user-to-user transactions
};
