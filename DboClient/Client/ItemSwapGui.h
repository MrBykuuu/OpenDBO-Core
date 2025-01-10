/******************************************************************************
*File			: NPCShop.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2006. 8. 1
*Abstract		:
*****************************************************************************
*Desc			: NPC shop
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

#include "NtlItem.h"

// Gui
#include "eventtimer.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

struct sNPC_TBLDAT;
class CNtlSobActor;

class CItemSwapGui : public CNtlPLGui, public RWS::CEventHandler
{
#define dMAX_ITEM_PANEL		6			///< Number of item panels in Shop

public:
	CItemSwapGui(const RwChar* pName);
	virtual ~CItemSwapGui();

	enum eShopType
	{
		SHOP_TYPE_NORMAL,
		SHOP_TYPE_TENKAICHI,
	};

	struct ItemPanel
	{
		CRegularSlotGui		slot;			///< slot
		gui::CPanel*		pItemPanel;		///< Item panel
		gui::CStaticBox*	pItemName;		///< name
		gui::CStaticBox*	pPoint;			///< Required points (price, world's best prom points, etc...)

		gui::CStaticBox*	pstbNeedItemCount;
		CRegularSlotGui		slotNeedItem;
	};

	struct ShopItem
	{
		SERIAL_HANDLE	hItem;
		std::wstring	wstrItemName;
		RwInt32			iCount;
		RwUInt32		uiPrice;

		std::wstring	wstrNeedItemName;

		sITEM_TBLDAT*	pITEM_DATA;
		sITEM_TBLDAT*	pNeedITEM_DATA;
	};

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);		///< Open/Close in DialogManager

	SERIAL_HANDLE	GetNPCSerial();

protected:

	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			ResetCartTexture();

	VOID			OpenShop(SERIAL_HANDLE hNPC, sNPC_TBLDAT* pNPC_TBLDAT);	///< Open the NPC store
	VOID			CloseShop();						///< Close the NPC store.	

	VOID			ClearShop();						///< Initialize shop information.
	VOID			ClearPanels();						///< Initialize all panel contents.

	VOID			UpdateTabContent(RwUInt8 byIndex);	///< Update the contents that fit the tab
	RwBool			SetPage(RwInt32 iPage);				///< Current page settings
	VOID			SetPanel(RwInt32 iPage);			///< Shows a list of items that fit the current page.
	VOID			SetPageButton();

	VOID			SetZenny();							///< Update your cash information.

	VOID			SetRepairAllTooltip();				///< Calculate the current total repair cost and display the information

	RwUInt8			GetPageCount_of_CurTab();			///< Returns the number of pages belonging to the current tab

	RwBool			IsFirstPage();
	RwBool			IsLastPage();

	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);
	RwInt8			PtinSlot_of_SmallIcon_in_panel(RwInt32 iX, RwInt32 iY);
	VOID			FocusEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			CheckInfoWindow();

	VOID			OnPaint();
	VOID			OnPostPaint();

	VOID			OnSelectChangeTabButton(INT nCurIndex, INT nPrevIndex);	///<Pressed the tab button

	VOID			ClickedPrePageButton(gui::CComponent* pComponent);	///<Pressed the previous button.
	VOID			ClickedNextPageButton(gui::CComponent* pComponent);	///<Pressed the next button.
	VOID			ClickedCloseButton(gui::CComponent* pComponent);	///<Pressed the close button.
	VOID			ClickedRepairButton(gui::CComponent* pComponent);	///<Pressed the repair button.
	VOID			ClickedRepairAllButton(gui::CComponent* pComponent);///< I pressed the full repair button.
	VOID			ClickedIdentifyButton(gui::CComponent* pComponent);	///<Pressed the item appraisal button.

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
	gui::CSlot			m_slotRepairButton;
	gui::CSlot			m_slotRepairAllButton;
	gui::CSlot			m_slotIdentifyButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	RwInt8				m_byInfoWindowIndex;
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
	gui::CButton*		m_pRepairButton;	///< Repair button
	gui::CButton*		m_pRepairAllButton;	///< Full repair button
	gui::CButton*		m_pIdentifyButton;	///< Item appraisal button	
	gui::CButton*		m_pPrePageButton;	///< Skip to previous page button
	gui::CButton*		m_pNextPageButton;	///< Button to go to next page

	gui::CStaticBox*	m_pShopTitle;		///< store name	

	gui::CStaticBox*	m_pPocketMoneytitle;///< Title of possession
	gui::CStaticBox*	m_pPocketMoney;		///< Cash in hand	
	gui::CStaticBox*	m_pCurrentPage;		///< Show current page

	ShopItem			m_aShopItem[NTL_MAX_MERCHANT_TAB_COUNT][NTL_MAX_MERCHANT_COUNT];	/// Information on items that can be sold for each tab

	SERIAL_HANDLE		m_hNPCSerial;		///< NPC number

	RwInt32				m_iCurTab;			///< current tab index
	RwInt32				m_iCurPage;			///< Current page

	RwBool				m_bFocus;			///< Got focus.
};