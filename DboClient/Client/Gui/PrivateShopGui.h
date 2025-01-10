/*****************************************************************************
*File			: PrivateShopGui.h
*Author		    : Jinsung, Kim -> Haesung, Cho
*Copyright		: NTL Co., Ltd.
*Date			: 2007. 7. 4 -> 2007. 12. 19
*Abstract		: DBO personal store
*****************************************************************************
*Desc           : 
*****************************************************************************/

#ifndef __PRIVATE_SHOP_GUI_H__
#define __PRIVATE_SHOP_GUI_H__

#pragma once

#include "Windowby3.h"
#include "SlotGui.h"
#include "NtlItem.h"

// Location of personal store items
#define dPRIVATESHOP_UI_ITEM_TOP				42
#define dPRIVATESHOP_UI_ITEM_LEFT				20
#define dPRIVATESHOP_UI_ITEM_GAP				55

#define dPRIVATESHOP_UI_BGLINE_LEFT				9
#define dPRIVATESHOP_UI_BGLINE_TOP				30
#define dPRIVATESHOP_UI_SHOP_BGLINE_WIDTH		303
#define dPRIVATESHOP_UI_SHOP_BGLINE_HEIGHT		347
#define dPRIVATESHOP_UI_NOTE_BGLINE_WIDTH		303
#define dPRIVATESHOP_UI_NOTE_BGLINE_HEIGHT		375
#define dPRIVATESHOP_UI_BUY_AWAY_LEFT			30
#define dPRIVATESHOP_UI_SELL_AWAY_LEFT			115

#define dPRIVATESHOP_UI_ITEM_HEIGHT				DBOGUI_ICON_SIZE
#define dPRIVATESHOP_UI_ITEM_WIDTH				DBOGUI_ICON_SIZE

#define dPRIVATESHOP_UI_CALC_OFFSET				104


class CPrivateShopItemGui;
class CPrivateShopGui;

/**
* \ingroup client
* \brief Personal store item slot
*/
class CPrivateShopItemGui
{
public:
	CPrivateShopItemGui();
	virtual ~CPrivateShopItemGui();

	// Create & Destroy
	RwBool			Create(const CRectangle& rectArea, CSurfaceGui* pEffectSelect, CSurfaceGui* pEffectFocus, gui::CComponent* pParent, CPrivateShopGui* pPrivateShopGui);
	void			Destroy();

	// Callback
	void			OnPaint();	
	void			OnBgPaint();
	void			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void			OnMouseDown(const CKey& key);
	void			OnMouseUp(const CKey& key);
	void			OnMouseLeave(gui::CComponent* pComponent);
	void			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void			OnClickedBtnPrice(gui::CComponent* pComponent);

	// Interface
	void			Clear();
	void			Show(bool bShow = true)					{ m_pShopItem->Show(bShow); }

	RwUInt8			GetPos()								{ return m_uiPos; }
	void			SetPos(RwUInt8 uiPos)					{ m_uiPos = uiPos; }

	CRegularSlotGui*		GetIcon()								{ return m_pShopItemIcon; }
	void			SetIcon(RwUInt8 uiPos, RwUInt32 uiSerial, 
							RwUInt32 uiCount, RwUInt32 uiPrice, 
							RwUInt8 uiItemState, eREGULAR_SLOT_TYPE eSerialType);

	SERIAL_HANDLE	GetSerial()								{ return m_pShopItemIcon->GetSerial(); }
	RwUInt32		GetCount()								{ return m_pShopItemIcon->GetCount(); }
	RwUInt32		GetPrice()								{ return m_pShopItemIcon->GetPrice(); }
	sITEM_TBLDAT*	GetItemTable()							{ return m_pShopItemIcon->GetItemTable(); }

	void			SetItemState(RwUInt8 uiItemState)		{ m_uiItemState = uiItemState; }
	RwUInt8			GetItemState()							{ return m_uiItemState; }

	CRectangle		GetShopItemRect();
	void			EnablePrice(bool bEnable = true)				{ m_pShopItemPrice->ClickEnable(bEnable); }


protected:
	// Components
	gui::CDialog*		m_pShopItem;				///< Item Dialog
	gui::CPanel*		m_pShopItemBg;				///< Background of Item Slot
	gui::CStaticBox*	m_pShopItemName;			///< Item name
	gui::CButton*		m_pShopItemPrice;			///< Item price
	CRegularSlotGui*			m_pShopItemIcon;			///< Item IconÀÇ Slot

	// Signals
	gui::CSlot			m_slotMove;	
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotBgPaint;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotClickedBtnPrice;			

	// Effects
	CSurfaceGui			m_EffectPick;				///< Picture in picked state
	CSurfaceGui*		m_pEffectSelect;			///< Picture in selected state
	CSurfaceGui*		m_pEffectFocus;				///< Picture in focus state

	CPrivateShopGui*	m_pPrivateShopGui;			///< Parent Class that holds this slot
	RwBool				m_bClick;					///< Was it clicked?
	
	RwUInt8				m_uiItemState;				///< Item status
	CRectangle			m_rectItem;
	RwUInt8				m_uiPos;
};

/**
* \ingroup client
* \brief Personal store GUI
*/
class CPrivateShopGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	
	///< Tab Type of Personal Store UI
	enum eTAB_TYPE
	{
		TAB_SHOP,	///< Store
		TAB_NOTE,	///< Notice
		TAB_NUM,	///< Number of TABs
	};

	///< Item Slot in Personal Store UI
	enum eITEM_SLOT
	{
		SLOT_NUM = 6,	///< Number of slots
	};

	///< Type of personal store UI
	enum ePrivateShopType
	{
		PRIVATESHOP_BUY,	///< Guest
		PRIVATESHOP_SELL,	///< owner
		PRIVATESHOP_NONE,
	};

	// Constructor & Destructor
	CPrivateShopGui(const RwChar* pName);
	virtual ~CPrivateShopGui();

	// Create & Destroy
	RwBool	Create();
	RwBool	CreateTabShop(gui::CDialog* pParent);
	RwBool	CreateTabNote(gui::CDialog* pParent);
	void	Destroy();

	// SwitchDialog & HandleEvents
	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);								///< Event
	void	HandleMsgboxResult(void* pData);							///< Message box result

	void	HandleCalcPopupResult(void* pData);							///< Input result of calculator
	void	HandleSobPrivateShopCommunity(void *pData);					///< When the status of the store changes
	void	HandleRegPrivateShopItem(void* pData);						///< Personal store Item Reg
	void	HandleSobInfoUpdate(void* pData);							///< Sob guest house information update
	void	HandlePickedUpHide(void* pData);							///< When a personal store item is picked up and hidden 

	// HandleEvents : Personal store related
	void	HandlePrivateShopState(void* pData);						///< Private store State
	void	HandlePrivateShopStateVisitor(void *pData);					///< When a visitor comes in
	void	HandlePrivateShopItem(void* pData);							///< Personal store Item
	void	HandlePrivateShopItemDataInfo(void* pData);					///< Personal store Item Update
	void	HandlePrivateShopItemSelect(void* pData);					///< Personal Store Item Select
	void	HandlePrivateShopSelectNfy(void* pData);
	void	HandlePrivateShopItemBuying(void* pData);					///< Personal Store Item Buying
	void	HandlePrivateShopItemState(void* pData);					///< Personal store Item State
	void	HandlePrivateShopNameTag(void* pData);						///< Personal store NameTag Update

	// Event
	void	OnPaintEffect();
	void	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void	OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);
	void	OnTabShopMouseLeave(gui::CComponent* pComponent);
	void	OnClickedBtnClose(gui::CComponent* pComponent);
	void	OnCaptureMouseDown(const CKey& key);

	void	OnGotFocus();
	void	OnLostFocus();

	void	OnClickedBtnNext(gui::CComponent* pComponent);
	void	OnClickedBtnPrev(gui::CComponent* pComponent);
	void	OnClickedBtnMail(gui::CComponent* pComponent);
	void	OnClickedBtnAway(gui::CComponent* pComponent);
	void	OnPaintBtnAway();
	void	OnMovedBtnAway(RwInt32 iOldX, RwInt32 iOldY);
	void	OnKeyUp(gui::CComponent* pComponenet, CInputDevice* pDevice, const CKey& key);
	void	OnNoticeCharChanged(int iLength);
	void	OnClickedBtnSale(gui::CComponent* pComponent);

	// ShopItemGui Event
	void	OnShopItemEnter(CPrivateShopItemGui* pShopItem);
	void	OnShopItemLeave(CPrivateShopItemGui* pShopItem);
	void	OnShopItemClicked(CPrivateShopItemGui* pShopItem, const CKey& Key);
	void	OnShopItemPriceClicked(CPrivateShopItemGui* pShopItem, gui::CComponent* pComponent);
	
	// Gui Setting
	void	SetSale(bool bSale);		// Sales start
	void	SetAway(bool bAway);		// away
	void	SetNotice(const WCHAR* pText);
	void	SetPage(RwUInt32 iPage, bool bCheckMax = true);

	void	SetUserInterface(ePrivateShopType eType, sPRIVATESHOP_SHOP_DATA* pShopData, bool bIsSale);
	void	SetShopItemSelect(CPrivateShopItemGui* pShopItem);
	void	SetShopItemFocus(CPrivateShopItemGui* pShopItem);

	// Get
	RwUInt8	GetShopState() { return m_ShopData.sSummaryPrivateShopData.byShopState; }

	void	Clear();
	void	ClearShopItem();

	void	UpdateShopItem();
	void	UpdateShopItemData();

	void	SetShopItemData(RwUInt8 uiPrivateShopPos, sPRIVATESHOP_ITEM_DATA* pShopItemData, bool bEvent = true);
	void	ShowInfoWindow(RwBool bIsShow, CPrivateShopItemGui* pShopItem = NULL);

	void	CalcNoticeMaxNum();
	void	CheckSoldOut();

protected:
	// Gui Common Components
	gui::CStaticBox*			m_pTitle;
	gui::CButton*				m_pCloseButton;
	gui::CTabButton*			m_pTabButton;
	gui::CDialog*				m_apDialogTab[TAB_NUM];

	// Gui Common Signals
	gui::CSlot					m_slotMove;
	gui::CSlot					m_slotTabChanged;
	gui::CSlot					m_slotCloseButtonClicked;
	gui::CSlot					m_slotTabShopMouseLeave;
	gui::CSlot					m_slotCaptureMouseDown;

	// TAB_SHOP (Shop) Components
	gui::CButton*				m_pNextButton;
	gui::CButton*				m_pPrevButton;
	gui::CButton*				m_pSaleButton;
	gui::CStaticBox*			m_pZenny;
	gui::CStaticBox*			m_pZennyTitle;
	gui::CStaticBox*			m_pCurrentPage;
	gui::CPanel*				m_pSoldOut;
	gui::CStaticBox*			m_pStbSoldOut;

	gui::CSlot					m_slotNextClicked;
	gui::CSlot					m_slotPrevClicked;
	gui::CSlot					m_slotSaleClicked;
	gui::CSlot					m_slotEffectPaint;

	// TAB_SHOP (Shop) Signals
	CPrivateShopItemGui			m_aShopItem[SLOT_NUM];		///< Personal store items
	CSurfaceGui					m_EffectSelect;				///< Picture in selected state
	CSurfaceGui					m_EffectFocus;				///< Picture in focus state

	// TAB_SHOP (Shop) State & Effect
	ePrivateShopType			m_eShopType;				///<Status of the store
	sPRIVATESHOP_ITEM_DATA		m_aShopItemData[NTL_MAX_PRIVATESHOP_INVENTORY];
	sPRIVATESHOP_SHOP_DATA		m_ShopData;

	CPrivateShopItemGui*		m_pItemSelect;		///< Selected item
	CPrivateShopItemGui*		m_pItemFocus;		///< Focused item

	RwUInt32					m_uiItemCount;		///< Number of items
	RwUInt32					m_uiPageMax;		///< maximum pages 
	RwUInt32					m_uiPageCur;		///< Current page
	bool						m_bAway;			///< Away
	bool						m_bSale;			///< On sale

	// TAB_NOTE (Notice) Components
	gui::CStaticBox*			m_pShopNameTitle;
	gui::CInputBox*				m_pShopNameInput;
	gui::CStaticBox*			m_pShopNoticeTitle;
	gui::CInputBox*				m_pShopNoticeInput;
	gui::CDialog*				m_pShopNoticeDialog;
	gui::CButton*				m_pAwayButton;
	gui::CButton*				m_pMailButton;
	gui::CStaticBox*			m_pNoticeMaxNum;

	// TAB_NOTE (Notice) Signals
	gui::CSlot					m_slotNameGotFocus;
	gui::CSlot					m_slotNameLostFocus;
	gui::CSlot					m_slotNoticeGotFocus;
	gui::CSlot					m_slotNoticeLostFocus;
	gui::CSlot					m_slotMailClicked;
	gui::CSlot					m_slotAwayClicked;
	gui::CSlot					m_slotAwayPaint;
	gui::CSlot					m_slotAwayMove;
	gui::CSlot					m_slotNameKeyUp;
	gui::CSlot					m_slotNoticeKeyUp;
	gui::CSlot					m_slotNoticeCharChanged;

	CSurfaceGui					m_surAwayCheck;
	CSurfaceGui					m_surAwayUnCheck;
	
};

#endif//__PRIVATE_SHOP_GUI_H__