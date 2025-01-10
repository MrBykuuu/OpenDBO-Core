/******************************************************************************
*File           : NetPyShopGui.h
*Author		    : Cho Haesung
*Copyright	    : NTL Co., Ltd.
*Date           : June 25, 2009
*Abstract		: 
*****************************************************************************
*Desc: Class that defines NetPyShop¡¯s GUI and functions among commercial functions
*****************************************************************************/

#ifndef __NETPYSHOP_GUI_H__
#define __NETPYSHOP_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"


// Shared
#include "NtlItem.h"

// Gui
#include "eventtimer.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

struct sNPC_TBLDAT;
class CNtlSobActor;

#define dNETPYSHOP_INVALID_INDEX		(-1)

class CNetPyShopGui : public CNtlPLGui, public RWS::CEventHandler
{
#define dFIRST_PAGE			0
#define dMAX_PAGE			5			///< Maximum pages that can be registered per tab
#define dMAX_ITEM_PANEL		6			///< Number of item panels in Shop
#define dNETPYSHOP_TAB_NUMS 4

public:
	CNetPyShopGui(const RwChar* pName);
	virtual ~CNetPyShopGui();

	enum eShopType
	{
		SHOP_TYPE_NETPY,

		SHOP_TYPE_NUMS,
		SHOP_TYPE_INVALID = 0xFF
	};

	struct ItemPanel
	{
		CRegularSlotGui			slot;			///< slot
		gui::CPanel*		pItemPanel;		///< Item panel
		CSurfaceGui			srfNeedItem;	///< Need Item
		gui::CStaticBox*	pItemName;		///< name
		gui::CStaticBox*	pPoint;			///< Required points (price, world's best prom points, etc...)
		std::wstring		wstrNeedItemName;

		RwBool				bNeedItem;
	};

	struct ShopItem
	{
		SERIAL_HANDLE	hItem;
		std::wstring	wstrItemName;
		RwInt32			iCount;
		RwUInt32		uiPrice;
		RwUInt32		uiPoint;
		sITEM_TBLDAT*	pITEM_DATA;
		sITEM_TBLDAT*	pNeedITEM_DATA;
	};

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);		///< Open/Close in DialogManager
	eShopType		GetShopType();

protected:
	CNetPyShopGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			ResetCartTexture(eShopType eType);

	VOID			OpenShop();
	VOID			CloseShop();						///< Close the NPC store.	

	VOID			ClearShop();						///< Initialize shop information.
	VOID			ClearPanels();						///< Initialize all panel contents.

	VOID			UpdateTabContent(RwUInt8 byIndex);	///< Update the contents that fit the tab
	bool			SetPage(RwInt32 iPage);				///< Current page settings
	VOID			SetPanel(RwInt32 iPage);			///< Shows a list of items that fit the current page.
	VOID			SetPageButton();

	VOID			SelectItem(RwInt32 iPanelIndex);	///< A choice was made to purchase the item.

	VOID			SetZenny();							///< Update your cash information.
	
	RwUInt8			GetPageCount_of_CurTab();			///< Returns the number of pages belonging to the current tab

	RwBool			IsFirstPage();
	RwBool			IsLastPage();

	RwInt32			PtinSlot(RwInt32 iX, RwInt32 iY);
	RwInt32			PtinSlot_of_SmallIcon_in_panel(RwInt32 iX, RwInt32 iY);
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect( RwBool bPush, RwInt32 iSlotIdx = -1 );
	VOID			CheckInfoWindow();

	VOID			OnPaint();
	VOID			OnPostPaint();	

	VOID			OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex );	///<Pressed the tab button

	VOID			ClickedPrePageButton(gui::CComponent* pComponent);	///<Pressed the previous button.
	VOID			ClickedNextPageButton(gui::CComponent* pComponent);	///<Pressed the next button.
	VOID			ClickedCloseButton(gui::CComponent* pComponent);	///<Pressed the close button.
	VOID			ClickedNetStoreButton(gui::CComponent* pComponent);
	
	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);


protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotTab;
	gui::CSlot			m_slotPrePageButton;
	gui::CSlot			m_slotNextPageButton;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotClickedNetStore;

	RwInt32				m_iInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;	///< Index of the slot pressed by the mouse
	RwInt32				m_iSelectedSlot;	///< Select effect slot
	RwInt32				m_iClickEffectedSlot;

	gui::CTabButton*	m_pTabButton;		//< Tab button

	ItemPanel			m_ItemPanel[dMAX_ITEM_PANEL];	///< ItemPanel

	CWindowby3			m_BackLineSurface;	///< Backline

	CSurfaceGui			m_FocusEffect;		///< Slot focus effect
	CSurfaceGui			m_SelectEffect;		///< Slot select effect
	CSurfaceGui			m_MoneyBackPanel;	///< Background of Possessed Money
	CSurfaceGui			m_PageBackPanel;	///< Page background

	gui::CButton*		m_pExitButton;		///< Close window button
	gui::CButton*		m_pPrePageButton;	///< Skip to previous page button
	gui::CButton*		m_pNextPageButton;	///< Button to go to next page
	gui::CButton*		m_pNetStoreButton;

	gui::CStaticBox*	m_pShopTitle;		///< store name	
	gui::CStaticBox*	m_pLargeBuyExplan;	///< Bulk purchase description
	gui::CStaticBox*	m_pPocketMoneytitle;///< Title of possession
	gui::CStaticBox*	m_pPocketMoney;		///< Cash in hand	
	gui::CStaticBox*	m_pCurrentPage;		///< Show current page

	ShopItem			m_aShopItem[NTL_MAX_MERCHANT_TAB_COUNT][NTL_MAX_MERCHANT_COUNT];	/// Information on items that can be sold for each tab

	eShopType			m_eShopType;

	RwInt32				m_iCurTab;			///< current tab index
	RwInt32				m_iCurPage;			///< Current page

	RwBool				m_bFocus;			///< Got focus.

	unsigned int		m_adwNETPYSHOP_MERCHANT_TBLIDX[dNETPYSHOP_TAB_NUMS];
};

#endif