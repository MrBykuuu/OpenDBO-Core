#include "precomp_dboclient.h"

// Core
#include "NtlDebug.h"
#include "CEventHandler.h"

// Shared
#include "NtlResultCode.h"
#include "NtlItem.h"

// Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// Framework
#include "NtlApplication.h"

// Simulation
#include "InputActionMap.h" 
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobPlayer.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPrivateShop.h"

// Table
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "ChatGui.h"
#include "IconMoveManager.h"
#include "ChatBalloonManager.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"
#include "GUISoundDefine.h"

#include "DumpCommand.h"

// Private shop
#include "PrivateShopGui.h"

// Etc

////////////////////////////////////////////////////////////////////////////////////////////////
// CPrivateShopItem
////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief constructor
*/
CPrivateShopItemGui::CPrivateShopItemGui()
:m_pPrivateShopGui(NULL)
,m_pShopItem(NULL)
,m_pShopItemBg(NULL)
,m_pShopItemName(NULL)
,m_pShopItemPrice(NULL)
,m_pShopItemIcon(NULL)
,m_pEffectSelect(NULL)
,m_pEffectFocus(NULL)
,m_bClick(FALSE)
,m_uiItemState(sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_GENERAL)
,m_uiPos(PRIVATESHOP_ITEMPOS_INVALID)
{
}

/**
* \brief destructor
*/
CPrivateShopItemGui::~CPrivateShopItemGui()
{
}

/**
* \brief Create
* \param rectArea Area of ??current slot
* \param pEffectSelect Effect Surface when selected
* \param pEffectFocus Effect Surface when focused
* \param pEffectBusiness Effect Surface during bargaining
* \param pParent Parent Component
* \param pPrivateShopGui The Gui that has the slot.
*/
RwBool CPrivateShopItemGui::Create(const CRectangle& rectArea, 
								   CSurfaceGui* pEffectSelect, 
								   CSurfaceGui* pEffectFocus,
								   gui::CComponent* pParent, 
								   CPrivateShopGui* pPrivateShopGui)
{
	NTL_FUNCTION("CPrivateShopItemGui::Create");

	// The background behind the item slot
	CRectangle rect = rectArea;
	m_pShopItem = NTL_NEW gui::CDialog(rect, pParent, GetNtlGuiManager()->GetSurfaceManager());

	// Picture of the background behind the item ( gui::CPanet )
	rect.SetRectWH(0, 0, 283, 48);
	m_pShopItemBg = NTL_NEW gui::CPanel(rect, m_pShopItem, GetNtlGuiManager()->GetSurfaceManager(),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfBackGroundShopItem"));
	m_pShopItemBg->Enable(false);

	// item name
	rect.SetRectWH(48, 2, 200, 16);
	m_pShopItemName = NTL_NEW gui::CStaticBox(rect, m_pShopItem, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pShopItemName->Enable(false);

	// price 
	rect.SetRectWH(176, 22, 103, 20);
	m_pShopItemPrice = NTL_NEW gui::CButton(rect, L"0",m_pShopItem, GetNtlGuiManager()->GetSurfaceManager());

	m_pShopItemPrice->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfBackGroundMoney" ));
	m_pShopItemPrice->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfBackGroundMoney"));
	m_pShopItemPrice->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfMoneyHighlight"));
	m_pShopItemPrice->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfMoneyDisable"));
	m_pShopItemPrice->GetTextComponent()->SetSize( 87, 20 );
	m_pShopItemPrice->SetTextStyle(COMP_TEXT_RIGHT);
	m_pShopItemPrice->SetTextDownCoordDiff(0, 0);
	m_pShopItemPrice->ClickEnable(false);

	m_pShopItemIcon = NTL_NEW CRegularSlotGui;
	m_pShopItemIcon->Create(m_pShopItem, DIALOG_PRIVATESHOP, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	m_pShopItemIcon->SetPosition_fromParent(7, 6);

	// Setting the Surface when Picked
	m_EffectPick.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfShopItemPick"));
	m_EffectPick.SetPositionfromParent(7, 6);

	// Slot
	m_slotMove			= m_pShopItem->SigMove().Connect(this, &CPrivateShopItemGui::OnMove);
	m_slotPaint			= m_pShopItem->SigPaint().Connect(this, &CPrivateShopItemGui::OnPaint);
	m_slotBgPaint		= m_pShopItemBg->SigPaint().Connect(this, &CPrivateShopItemGui::OnBgPaint);
	m_slotMouseDown		= m_pShopItem->SigMouseDown().Connect(this, &CPrivateShopItemGui::OnMouseDown);
	m_slotMouseUp		= m_pShopItem->SigMouseUp().Connect(this, &CPrivateShopItemGui::OnMouseUp);
	m_slotMouseMove		= m_pShopItem->SigMouseMove().Connect(this, &CPrivateShopItemGui::OnMouseMove);
	m_slotMouseLeave	= m_pShopItem->SigMouseLeave().Connect(this, &CPrivateShopItemGui::OnMouseLeave);
	m_slotClickedBtnPrice	= m_pShopItemPrice->SigClicked().Connect(this, &CPrivateShopItemGui::OnClickedBtnPrice);

	// OwnerClass Setting
	m_pEffectSelect		= pEffectSelect;
	m_pEffectFocus		= pEffectFocus;
	m_pPrivateShopGui	= pPrivateShopGui;

	OnMove( rectArea.left, rectArea.top );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CPrivateShopItemGui::Destroy()
{
	NTL_FUNCTION("CPrivateShopItemGui::Destroy");

	if (m_pShopItemIcon)
	{
		m_pShopItemIcon->Destroy();
		NTL_DELETE( m_pShopItemIcon );
	}

	NTL_DELETE( m_pShopItemBg );
	NTL_DELETE( m_pShopItemName );
	NTL_DELETE( m_pShopItemPrice );
	NTL_DELETE( m_pShopItem );
	
	NTL_RETURNVOID();
}

/**
* \brief clear
*Initializes all item information in the current slot.
*/
void CPrivateShopItemGui::Clear()
{
	m_pShopItem->Show(true);
	m_pShopItemIcon->Clear();
	m_pShopItemName->Clear();
	m_pShopItemPrice->ClearText();
	m_pShopItemPrice->ClickEnable(false);
	m_uiItemState	= sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_GENERAL;
	m_uiPos			= PRIVATESHOP_ITEMPOS_INVALID;
}

void CPrivateShopItemGui::OnPaint()
{
}

void CPrivateShopItemGui::OnBgPaint()
{
	m_pShopItemIcon->Paint();
	
	switch (m_uiItemState)
	{
	case sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_GENERAL:
		break;
	case sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_SELECT_ITEM:
		m_EffectPick.Render();
		break;
	case sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_UNSELECT_ITEM:
		break;
	case sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_INVALID:
		break;
	}
}

void CPrivateShopItemGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect	= m_pShopItem->GetScreenRect();	
	m_pShopItemIcon->SetParentPosition(rect.left, rect.top);
	m_EffectPick.SetPositionbyParent(rect.left, rect.top);
}

void CPrivateShopItemGui::OnMouseDown(const CKey& key)
{
	if (m_pShopItemIcon->PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
	{
		m_bClick	= true;
		m_pShopItemIcon->ClickEffect(true);
		m_pShopItem->CaptureMouse();
	}
}

void CPrivateShopItemGui::OnMouseUp(const CKey& key)
{
	if (m_bClick)
	{
		if (m_pShopItemIcon->PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
		{
			m_bClick	= false;

			if( m_pPrivateShopGui->IsShow() )
				m_pPrivateShopGui->OnShopItemClicked(this, key);
		}
		m_pShopItemIcon->ClickEffect(false);
		m_pShopItem->ReleaseMouse();
	}
}

void CPrivateShopItemGui::OnClickedBtnPrice( gui::CComponent* pComponent ) 
{
	m_pPrivateShopGui->OnShopItemPriceClicked(this, pComponent);
}

void CPrivateShopItemGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_pPrivateShopGui->OnShopItemLeave(this);
}

void CPrivateShopItemGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if (m_pShopItemIcon->PtInRect(nX, nY))
	{
		m_pPrivateShopGui->OnShopItemEnter(this);
	}	
	else
	{
		m_pPrivateShopGui->OnShopItemLeave(this);
	}
}

void CPrivateShopItemGui::SetIcon(RwUInt8 uiPos, RwUInt32 uiSerial, RwUInt32 uiCount, RwUInt32 uiPrice, RwUInt8 uiItemState, eREGULAR_SLOT_TYPE eSerialType)
{
	SetPos(uiPos);
	if (uiSerial != INVALID_SERIAL_ID)
	{
		if (m_pShopItemIcon->GetSerialType() != eSerialType)
		{
			m_pShopItemIcon->Clear();
			m_pShopItemIcon->SetSerialType(eSerialType);
		}

		m_pShopItemIcon->SetIcon(uiSerial, uiCount);
		m_pShopItemIcon->SetPrice(uiPrice);

		if (m_pShopItemIcon->GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION)
		{
			m_pShopItemName->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_ITEM_NOT_IDENTIFICATION"));
		}
		else
		{
			std::wstring strBuf =  API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( m_pShopItemIcon->GetItemTable()->Name );
			m_pShopItemName->SetText( strBuf.c_str() );
		}	

		m_pShopItemPrice->SetText(Logic_FormatZeni(uiPrice));

		m_uiItemState = uiItemState;
	}
	else
	{
		m_pShopItemIcon->Clear();
		m_pShopItemName->Clear();
		m_pShopItemPrice->ClearText();
		
		m_uiItemState = sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_GENERAL;
	}
}

CRectangle CPrivateShopItemGui::GetShopItemRect()
{
	m_rectItem			= m_pShopItem->GetScreenRect();
	m_rectItem.left		= m_rectItem.left + 7;
	m_rectItem.top		= m_rectItem.top + 6;
	m_rectItem.right	= m_rectItem.left + 32;
	m_rectItem.bottom	= m_rectItem.top + 32;

	return m_rectItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// CPrivateShopGui
////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief constructor
*/
CPrivateShopGui::CPrivateShopGui(const RwChar* pName) : CNtlPLGui(pName)
,m_pTitle(NULL)
,m_pCloseButton(NULL)
,m_pTabButton(NULL)
,m_pZennyTitle(NULL)
,m_pZenny(NULL)
,m_pCurrentPage(NULL)
,m_pItemSelect(NULL)
,m_pItemFocus(NULL)
,m_pShopNameTitle(NULL)
,m_pShopNameInput(NULL)
,m_pShopNoticeTitle(NULL)
,m_pShopNoticeInput(NULL)
,m_pShopNoticeDialog(NULL)
,m_pMailButton(NULL)
,m_eShopType(PRIVATESHOP_NONE)
,m_uiPageMax(0)
,m_uiPageCur(0)
,m_pAwayButton(NULL)
,m_pNoticeMaxNum(NULL)
,m_uiItemCount(0)
,m_pSoldOut(NULL)
,m_pStbSoldOut(NULL)
,m_bSale(FALSE)
,m_bAway(FALSE)
{
}

/**
* \brief destructor
*/
CPrivateShopGui::~CPrivateShopGui()
{
}

/**
* \brief Create
*Create a common dialog component and also create a child Tab component.
*/
RwBool CPrivateShopGui::Create()
{
	NTL_FUNCTION("CPrivateShopGui::Create");

	if (!CNtlPLGui::Create("", "gui\\PrivateShop.srf", "gui\\PrivateShop.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis					= (gui::CDialog*)GetComponent("dlgMain");
	m_pTitle				= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pCloseButton			= (gui::CButton*)GetComponent("btnClose");
	m_pTabButton			= (gui::CTabButton*)GetComponent("tabButton");
	m_apDialogTab[TAB_SHOP]	= (gui::CDialog*)GetComponent("dlgShop");
	m_apDialogTab[TAB_NOTE]	= (gui::CDialog*)GetComponent("dlgNote");

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);
	
	// Signal
	m_slotMove					= m_pThis->SigMove().Connect(this, &CPrivateShopGui::OnMove);
	m_slotTabChanged			= m_pTabButton->SigSelectChanged().Connect(this, &CPrivateShopGui::OnTabChanged);
	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnClose);	
	m_slotTabShopMouseLeave		= m_apDialogTab[TAB_SHOP]->SigMouseLeave().Connect(this, &CPrivateShopGui::OnTabShopMouseLeave);
	m_slotCaptureMouseDown		= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CPrivateShopGui::OnCaptureMouseDown);

	// Add Tabs
	for (int i = 0; i < TAB_NUM; ++i)
	{
		m_pTabButton->AddTab(std::string());
	}

	// SetPosition Title
	m_pTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);

	// TAB_SHOP : Create Component
	CreateTabShop(m_apDialogTab[TAB_SHOP]);

	// TAB_NOTE : Create Component
	CreateTabNote(m_apDialogTab[TAB_NOTE]);

	// link
	LinkMsg(g_EventPrivateShopState, 0);
	LinkMsg(g_EventPrivateShopStateVisitor, 0);
	LinkMsg(g_EventPrivateShopItem, 0);	
	LinkMsg(g_EventPrivateShopItemDataInfo, 0);	
	LinkMsg(g_EventCalcPopupResult, 0);
	LinkMsg(g_EventSobPrivateShopCommunity, 0); 
	LinkMsg(g_EventPrivateShopItemSelect, 0);
	LinkMsg(g_EventPrivateShopSelectNfy, 0);
	LinkMsg(g_EventPrivateShopItemBuying, 0);
	LinkMsg(g_EventPrivateShopItemState, 0);
	LinkMsg(g_EventPrivateShopNameTag, 0);

	LinkMsg(g_EventRegPrivateShopItem, 0);
	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventMsgBoxResult, 0);
	LinkMsg(g_EventPickedUpHide, 0);

	Clear();
	Show(false);

	OnMove( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
	NTL_RETURN(TRUE);
}

/**
* \brief Creates the UI of TAB_SHOP.
* \param pParent Parent Dialog
*/
RwBool CPrivateShopGui::CreateTabShop(gui::CDialog* pParent)
{
	// Components
	m_pNextButton			= (gui::CButton*)GetComponent("btnNext");
	m_pPrevButton			= (gui::CButton*)GetComponent("btnPrev");
	m_pSaleButton			= (gui::CButton*)GetComponent("btnSale");
	m_pZenny				= (gui::CStaticBox*)GetComponent("stbPocketMoney");
	m_pZennyTitle			= (gui::CStaticBox*)GetComponent("stbPocketMoneyTitle");
	m_pCurrentPage			= (gui::CStaticBox*)GetComponent("stbCurrentPage");
	m_pSoldOut				= (gui::CPanel*)GetComponent("pnlSoldOut");
	m_pStbSoldOut			= (gui::CStaticBox*)GetComponent("stbSoldOut");

	// Signals
	m_slotNextClicked	= m_pNextButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnNext);
	m_slotPrevClicked	= m_pPrevButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnPrev);
	m_slotEffectPaint	= m_pSaleButton->SigPaint().Connect(this, &CPrivateShopGui::OnPaintEffect);
	m_slotSaleClicked	= m_pSaleButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnSale);

	// effect
	m_EffectFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_EffectSelect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
	
	// Create an Item Slot
	for (int i = 0; i < SLOT_NUM; ++i )
	{
		CRectangle	rect;
		rect.SetRectWH(dPRIVATESHOP_UI_ITEM_LEFT,						// LEFT
			dPRIVATESHOP_UI_ITEM_TOP + (dPRIVATESHOP_UI_ITEM_GAP*i),	// TOP (increased by GAP)
			283,														// WIDTH
			48);														// HEIGHT

		// Create item slot
		m_aShopItem[i].Create(rect, &m_EffectSelect, &m_EffectFocus, pParent, this);
	}

	// Setting
	m_pZennyTitle->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_ZENNY"));
	m_pStbSoldOut->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_EMPTY_SLOT"));
	m_pSaleButton->Raise(); // Raise the Sale button higher than the item slot.
	m_pSoldOut->Raise();
	m_pStbSoldOut->Raise();

	return TRUE;
}

/**
* \brief Create UI for TAB_NOTE
* \param pParent parent dialog
*/
RwBool CPrivateShopGui::CreateTabNote(gui::CDialog* pParent)
{
	// Components
	m_pShopNameTitle		= (gui::CStaticBox*)GetComponent("stbShopNameTitle");
	m_pShopNameInput		= (gui::CInputBox*)GetComponent("ipbShopName");
	m_pShopNoticeTitle		= (gui::CStaticBox*)GetComponent("stbShopNoticeTitle");
	m_pShopNoticeInput		= (gui::CInputBox*)GetComponent("ipbShopNotice");
	m_pShopNoticeDialog		= (gui::CDialog*)GetComponent("dlgShopNotice");
	m_pAwayButton			= (gui::CButton*)GetComponent("btnAway");
	m_pMailButton			= (gui::CButton*)GetComponent("btnMail");
	m_pNoticeMaxNum			= (gui::CStaticBox*)GetComponent("stbMaxNumText");

	// slot
	m_slotNameGotFocus			= m_pShopNameInput->SigGotFocus().Connect(this, &CPrivateShopGui::OnGotFocus);
	m_slotNameLostFocus			= m_pShopNameInput->SigLostFocus().Connect(this, &CPrivateShopGui::OnLostFocus);
	m_slotNoticeGotFocus		= m_pShopNoticeInput->SigGotFocus().Connect(this, &CPrivateShopGui::OnGotFocus);
	m_slotNoticeLostFocus		= m_pShopNoticeInput->SigLostFocus().Connect(this, &CPrivateShopGui::OnLostFocus);
	m_slotMailClicked			= m_pMailButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnMail);
	m_slotAwayClicked			= m_pAwayButton->SigClicked().Connect(this, &CPrivateShopGui::OnClickedBtnAway);
	m_slotAwayPaint				= m_pAwayButton->SigPaint().Connect(this, &CPrivateShopGui::OnPaintBtnAway);
	m_slotAwayMove				= m_pAwayButton->SigMove().Connect(this, &CPrivateShopGui::OnMovedBtnAway);
	m_slotNameKeyUp				= m_pShopNameInput->SigKeyUp().Connect(this, &CPrivateShopGui::OnKeyUp);
	m_slotNoticeKeyUp			= m_pShopNoticeInput->SigKeyUp().Connect(this, &CPrivateShopGui::OnKeyUp);
	m_slotNoticeCharChanged		= m_pShopNoticeInput->SigCharChanged().Connect(this, &CPrivateShopGui::OnNoticeCharChanged);

	// Surface
	m_surAwayCheck.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfAwayCheck"));
	m_surAwayCheck.SetPositionfromParent(0, 0);
	m_surAwayUnCheck.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PrivateShop.srf", "srfAwayUnCheck"));
	m_surAwayUnCheck.SetPositionfromParent(0, 0);

	// Setting
	m_pShopNameTitle->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_SHOPNAME"));
	m_pShopNoticeTitle->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NOTICE"));
	m_pAwayButton->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_AWAY"));
	m_pShopNameInput->SetMaxLength(NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE);
	m_pShopNoticeInput->SetMultilineMode(TRUE);
	m_pShopNoticeInput->SetMaxLength(NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE);
	
	return TRUE;
}

/**
* \brief Destroy
*/ 
void CPrivateShopGui::Destroy()
{
	NTL_FUNCTION("CPrivateShopGui::Destroy");

	for (int i = 0; i < SLOT_NUM; ++i)
	{
		m_aShopItem[i].Destroy();		
	}

	UnLinkMsg(g_EventPrivateShopState);
	UnLinkMsg(g_EventPrivateShopStateVisitor);
	UnLinkMsg(g_EventPrivateShopItem);
	UnLinkMsg(g_EventPrivateShopItemDataInfo);	
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventSobPrivateShopCommunity); 
	UnLinkMsg(g_EventPrivateShopItemSelect); 
	UnLinkMsg(g_EventPrivateShopSelectNfy);
	UnLinkMsg(g_EventPrivateShopItemBuying);
	UnLinkMsg(g_EventPrivateShopItemState); 
	UnLinkMsg(g_EventPrivateShopNameTag);

	UnLinkMsg(g_EventRegPrivateShopItem);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventPickedUpHide);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
*\breif SwitchDialog
* \param bOpen Whether to open or not
*The Show(bOpen) part was changed to the bottom because it asks about the game rule concept.
*/
RwInt32 CPrivateShopGui::SwitchDialog(bool bOpen)
{
	// If the dialog is closed, it means that the customer, the owner, or the individual store has been turned off.
	if ( bOpen == FALSE )
	{  
		if (GetIconMoveManager()->GetSrcPlace() == PLACE_PRIVATESHOP)
		{
			GetIconMoveManager()->IconMoveEnd();
		}

		// Sending packets that terminate depending on the type of store.
		switch (m_eShopType)
		{
		case PRIVATESHOP_NONE:
			break;
		case PRIVATESHOP_BUY:	// customer
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopLeaveReq(m_ShopData.hOwner);
			break;
		case PRIVATESHOP_SELL:	// master
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopExitReq();
			}
			break;
		}
	}

	Show(bOpen);

	return TRUE;
}

/**
* \brief HandleEvents
*Todo
*Must be divided into related events
*/
void CPrivateShopGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CPrivateShopGui::HandleEvents");

	// Handle Events: Comment is in the class definition part...

	if (msg.Id == g_EventMsgBoxResult)
		HandleMsgboxResult( msg.pData );
	else if (msg.Id == g_EventCalcPopupResult)
		HandleCalcPopupResult( msg.pData );
	else if (msg.Id == g_EventSobPrivateShopCommunity)
		HandleSobPrivateShopCommunity( msg.pData );
	else if (msg.Id == g_EventRegPrivateShopItem && !m_bSale)
		HandleRegPrivateShopItem( msg.pData );
	else if (msg.Id == g_EventSobInfoUpdate)
		HandleSobInfoUpdate( msg.pData );
	else if (msg.Id == g_EventPickedUpHide)
		HandlePickedUpHide( msg.pData );
	else if (msg.Id == g_EventPrivateShopState)
		HandlePrivateShopState( msg.pData );
	else if (msg.Id == g_EventPrivateShopStateVisitor)
		HandlePrivateShopStateVisitor( msg.pData );
	else if (msg.Id == g_EventPrivateShopItem)
		HandlePrivateShopItem( msg.pData );
	else if (msg.Id == g_EventPrivateShopItemDataInfo)
		HandlePrivateShopItemDataInfo( msg.pData );
	else if (msg.Id == g_EventPrivateShopItemSelect)
		HandlePrivateShopItemSelect( msg.pData );
	else if (msg.Id == g_EventPrivateShopSelectNfy )
		HandlePrivateShopSelectNfy( msg.pData );
	else if (msg.Id == g_EventPrivateShopItemBuying)
		HandlePrivateShopItemBuying( msg.pData );
	else if (msg.Id == g_EventPrivateShopItemState)
		HandlePrivateShopItemState( msg.pData );
	else if (msg.Id == g_EventPrivateShopNameTag)
		HandlePrivateShopNameTag( msg.pData );

	NTL_RETURNVOID();
}

/**
* \brief Processes the Result of the MessageBox related to the personal store.
* \param pMsgBoxResult message box result structure
*/
void CPrivateShopGui::HandleMsgboxResult( void* pData ) 
{
	SDboEventMsgBoxResult* pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>(pData);

	// TO DO

	if (pMsgBoxResult->strID == "DST_PRIVATESHOP_MSG_SALE_OPEN")
	{
		if (pMsgBoxResult->eResult == MBR_OK)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopOpenReq(m_pShopNameInput->GetText(), m_pShopNoticeInput->GetText(), m_bAway);
		}
	}
	else if (pMsgBoxResult->strID == "DST_PRIVATESHOP_MSG_SALE_CLOSE")
	{
		if (pMsgBoxResult->eResult == MBR_OK)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopCloseReq();
		}
	}
}

/**
* \brief When numbers are entered using a calculator
*/
void CPrivateShopGui::HandleCalcPopupResult( void* pData ) 
{
	SDboEventCalcPopupResult* pPopupResult = reinterpret_cast<SDboEventCalcPopupResult*>(pData);
	if (pPopupResult->nSrcPlace == PLACE_PRIVATESHOP)
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemUpdateReq((RwUInt8)pPopupResult->uiSerial, pPopupResult->uiValue);
	}
}

/**
* \brief Notifies you of the status of personal stores around the avatar when they are updated.
*/
void CPrivateShopGui::HandleSobPrivateShopCommunity( void *pData ) 
{
	SNtlEventSobPrivateShopCommunity* pPrivateShopCommunity = reinterpret_cast<SNtlEventSobPrivateShopCommunity*>(pData);

	switch( pPrivateShopCommunity->byShopState )
	{
		case PRIVATESHOP_STATE_OPEN:				// open
		{
			if (pPrivateShopCommunity->hSerialId != INVALID_SERIAL_ID)
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopEnterReq(pPrivateShopCommunity->hSerialId);
			}
		}
		break;

		default:
		break;
	}	
}

/**
* \brief When registering an item from a personal store
*/
void CPrivateShopGui::HandleRegPrivateShopItem( void* pData ) 
{
	SDboEventRegPrivateShopItem*	pRegPrivateShopItem = reinterpret_cast<SDboEventRegPrivateShopItem*>(pData);
	CNtlPrivateShop*				pPrivateShop		= GetNtlSLGlobal()->GetSobAvatar()->GetPrivateShop();

	if (pRegPrivateShopItem->uiPlace == PLACE_BAG)
	{
		RwUInt8 uiPrivateShopPos	= pPrivateShop->GetPrivateShopEmptyPos();
		RwUInt8 uiInventoryPlace	= (RwUInt8)Logic_ConvertBagIdxToContainerType((RwUInt8)pRegPrivateShopItem->uiIndex);

		if (uiPrivateShopPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemInsertReq(uiPrivateShopPos, uiInventoryPlace, (RwUInt8)pRegPrivateShopItem->uiPos);
		}
	}		
}

/**
* \brief Information on SOB required for personal stores has been updated
*/
void CPrivateShopGui::HandleSobInfoUpdate( void* pData ) 
{
	SNtlEventSobInfoUpdate*	pSobInfoUpdate	= reinterpret_cast<SNtlEventSobInfoUpdate*>(pData);
	if (pSobInfoUpdate->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
	{
		if (pSobInfoUpdate->uiUpdateType == EVENT_AIUT_ZENNY)
		{
			m_pZenny->SetText(Logic_FormatZeni(Logic_GetZenny()));
		}
	}
}

/**
* \brief When a personal store item is picked up and hidden 
*/
void CPrivateShopGui::HandlePickedUpHide( void* pData ) 
{
	if ((RwInt32)pData == PLACE_PRIVATESHOP)
	{
		SetShopItemSelect(NULL);
	}
}

/**
* \brief Change of store status
*/
void CPrivateShopGui::HandlePrivateShopState( void* pData ) 
{
	SNtlEventPrivateShopState*	pShopState	= reinterpret_cast<SNtlEventPrivateShopState*>( pData );
	sPRIVATESHOP_SHOP_DATA*		pShopData	= pShopState->pPrivateShopData;

	// Return if the avatar is not in personal store status
	if (!pShopState->IsAvatarPrivateShop)
	{
		NTL_RETURNVOID();
	}

	// Change in store status
	switch (pShopState->uiEventType)
	{
	case PRIVATESHOP_EVENT_CREATE:	// Start a store
		{
			// Break if DATA LOADING
			if( pShopState->uiPrivateShopState == PRIVATESHOP_STATE_DATALOADING )
				break;

			//GetNtlSLGlobal()->GetSobAvatar()->GetPrivateShop()->

			UpdateShopItemData();
			SetUserInterface(PRIVATESHOP_SELL, pShopData, false);
			if (GetDialogManager()->IsOpenDialog(DIALOG_PRIVATESHOP))
			{
				GetDialogManager()->CloseDialog(DIALOG_PRIVATESHOP);
			}
			GetDialogManager()->OpenDialog(DIALOG_PRIVATESHOP);
		}
		break;
	case PRIVATESHOP_EVENT_EXIT:	// end of store
		Clear();
		GetDialogManager()->CloseDialog(DIALOG_PRIVATESHOP);
		break;
	case PRIVATESHOP_EVENT_OPEN:	// Sales start
		{
			SetUserInterface(PRIVATESHOP_SELL, pShopData, true);
			CNtlSLEventGenerator::TSPrivateShop();
		}
		break;
	case PRIVATESHOP_EVENT_CLOSE:	// stop selling
		SetSale(false);
		break;
	}
	UpdateShopItem();
}

/**
* \brief When a guest visits
*/
void CPrivateShopGui::HandlePrivateShopStateVisitor( void *pData ) 
{
	SNtlEventPrivateShopStateVisitor*	pShopStateVisitor	= reinterpret_cast<SNtlEventPrivateShopStateVisitor*>(pData);
	sPRIVATESHOP_SHOP_DATA*				pShopData			= pShopStateVisitor->pPrivateShopData;

	switch (pShopStateVisitor->uiEventType)
	{
		case PRIVATESHOP_EVENT_ENTER:
		{
			SetUserInterface(PRIVATESHOP_BUY, pShopData, true);

			if (GetDialogManager()->IsOpenDialog(DIALOG_PRIVATESHOP))
			{
				GetDialogManager()->CloseDialog(DIALOG_PRIVATESHOP);
			}

			{
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_PRIVATESHOP_CART);
				pPLGui->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);
				Link(GetDialogManager()->GetDialog(DIALOG_PRIVATESHOP_CART)->GetDialog());
				GetDialogManager()->OpenDialog(DIALOG_PRIVATESHOP);
			}
		}
		break;
		case PRIVATESHOP_EVENT_LEAVE:
		{
			Clear();
			GetDialogManager()->CloseDialog(DIALOG_PRIVATESHOP);
			UnLink(GetDialogManager()->GetDialog(DIALOG_PRIVATESHOP_CART)->GetDialog());
		}
		break;
	}

	UpdateShopItem();
}

/**
* \brief When an item is updated
*/
void CPrivateShopGui::HandlePrivateShopItem( void* pData ) 
{
	SNtlEventPrivateShopItem*	pShopItem		= reinterpret_cast<SNtlEventPrivateShopItem*>(pData);
	sPRIVATESHOP_ITEM_DATA&		sShopItemData	= pShopItem->sPrivateShopItemData;


	switch (pShopItem->uiEventType)
	{
	case PRIVATESHOP_EVENT_ITEM_INSERT: // Goods registration 
		SetShopItemData(sShopItemData.byPrivateShopInventorySlotPos, &sShopItemData);
		break;
	case PRIVATESHOP_EVENT_ITEM_UPDATE: // Change product information
		m_aShopItemData[sShopItemData.byPrivateShopInventorySlotPos].dwZenny = sShopItemData.dwZenny;
		break;
	case PRIVATESHOP_EVENT_ITEM_DELETE: // Delete item
		SetShopItemData(sShopItemData.byPrivateShopInventorySlotPos, NULL);
		break;
	}
	UpdateShopItem();
}

/**
* \brief When updating personal store item data
*/
void CPrivateShopGui::HandlePrivateShopItemDataInfo( void* pData ) 
{
	SNtlEventPrivateShopItemDataInfo* pShopItemDataInfo = reinterpret_cast<SNtlEventPrivateShopItemDataInfo*>(pData);

	if (pShopItemDataInfo->hOwnerSerial != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
	{
		SetShopItemData(pShopItemDataInfo->sPrivateShopItemData.byPrivateShopInventorySlotPos, &pShopItemDataInfo->sPrivateShopItemData, false);
	}
	UpdateShopItem();
}

/**
* \brief When someone has added an item from a personal store to their wishlist
*/
void CPrivateShopGui::HandlePrivateShopItemSelect( void* pData ) 
{
	SNtlEventPrivateShopItemSelect* pPrivateShopItemSelect = reinterpret_cast<SNtlEventPrivateShopItemSelect*>(pData);

	// Change the state of an item
	m_aShopItemData[pPrivateShopItemSelect->uiPrivateShopPos].byItemState = pPrivateShopItemSelect->uiItemState;
	UpdateShopItem();
}

/**
* \brief When someone has added an item from a personal store to their wishlist
*/
void CPrivateShopGui::HandlePrivateShopSelectNfy( void* pData ) 
{
	SDboEventPrivateShopSelectNfy* pNotify = reinterpret_cast<SDboEventPrivateShopSelectNfy*>(pData);

	if( pNotify->uiItemState == sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_SELECT_ITEM )
	{
		sPRIVATESHOP_ITEM_DATA *pShopItem = &m_aShopItemData[pNotify->uiPrivateShopPos];
		CDboEventGenerator::RegPrivateShopCartItem(pShopItem->sItem.itemNo, PLACE_PRIVATESHOP, pShopItem->byPrivateShopInventorySlotPos,
			pShopItem->sItem.byStackcount, pShopItem->dwZenny, (void*)&pShopItem->sItem);
	}
}

/**
* \brief When someone purchases an item from a private store
*/
void CPrivateShopGui::HandlePrivateShopItemBuying( void* pData ) 
{
	SNtlEventPrivateShopItemBuying* pPrivateShopItemBuying = reinterpret_cast<SNtlEventPrivateShopItemBuying*>(pData);

	for (int i = 0; i < pPrivateShopItemBuying->uiBuyCount; ++i)
	{
		sPRIVATESHOP_ITEM_DATA& sPrivateShopItemData = pPrivateShopItemBuying->asPrivateShopItemData[i];
		if (sPrivateShopItemData.byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			SetShopItemData(sPrivateShopItemData.byPrivateShopInventorySlotPos, NULL);
		}
	}
	
	SetPage(m_uiPageCur);
	UpdateShopItem();
}

/**
* \brief When the status of an item in a personal store changes
*/
void CPrivateShopGui::HandlePrivateShopItemState( void* pData ) 
{
	SNtlEventPrivateShopItemState* pPrivateShopItemSelect = reinterpret_cast<SNtlEventPrivateShopItemState*>(pData);

	for (int i = 0; i < pPrivateShopItemSelect->iStateCount; ++i)
	{
		m_aShopItemData[i].byItemState = pPrivateShopItemSelect->auiItemState[i];
	}	
	UpdateShopItem();
}



/**
* \brief NameTag update of personal store
*/
void CPrivateShopGui::HandlePrivateShopNameTag( void* pData ) 
{
	SNtlEventPrivateShopNameTag* pPrivateShopNameTag = reinterpret_cast<SNtlEventPrivateShopNameTag*>(pData);

	if (pPrivateShopNameTag->hOwnerSerial != INVALID_SERIAL_ID)
	{		
		CNtlSob* pSobObject = GetNtlSobManager()->GetSobObject(pPrivateShopNameTag->hOwnerSerial);

		switch (pPrivateShopNameTag->uiPrivateShopState)
		{
		case PRIVATESHOP_STATE_CLOSE:
			GetBalloonManager()->AddPrivateShopBalloon(pSobObject, GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NAMETAG_READY"), TRUE);
			break;
		case PRIVATESHOP_STATE_DATALOADING:
			GetBalloonManager()->AddPrivateShopBalloon(pSobObject, GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NAMETAG_READY"), TRUE);
			break;
		case PRIVATESHOP_STATE_OPEN:
			{
				WCHAR awcBuffer[256];
				if (lstrlenW(pPrivateShopNameTag->pwcPrivateShopName) > 0)
				{
					swprintf_s(awcBuffer, L"%s%s", pPrivateShopNameTag->pwcPrivateShopName, (pPrivateShopNameTag->bIsOwnerEmpty ? GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NAMETAG_AWAY") : L""));
				}
				else
				{
					swprintf_s(awcBuffer, L"%s%s%s", Logic_GetName(GetNtlSobManager()->GetSobObject(pPrivateShopNameTag->hOwnerSerial)),
						GetDisplayStringManager()->GetString("DST_PRIVATESHOP_SHOPNAME_EMPTY"),
						(pPrivateShopNameTag->bIsOwnerEmpty ? GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NAMETAG_AWAY") : L""));
				}
				GetBalloonManager()->AddPrivateShopBalloon(pSobObject, awcBuffer, FALSE);
			}
			break;
		case PRIVATESHOP_STATE_NONE:
			GetBalloonManager()->FadeOutBalloon(pSobObject, CBalloonGui::TYPE_PRIVATESHOP);
			break;
		}
	}
}

void CPrivateShopGui::SetShopItemData(RwUInt8 uiPrivateShopPos, sPRIVATESHOP_ITEM_DATA* pShopItemData, bool bEvent)
{
	//DUMP_DEBUG_MSG( MakeStr( "SetShopItemData Pos = %d ShopItemData = %d", uiPrivateShopPos, pShopItemData ) );

	if (pShopItemData)
	{
		sPRIVATESHOP_ITEM_DATA& shopItemData = *pShopItemData;
		if (bEvent && shopItemData.hItem != INVALID_SERIAL_ID && !shopItemData.bIsSaveItem)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_PRIVATESHOP, PLACE_BAG, 
				Logic_ConvertContainerTypeToBagIdx(shopItemData.sItem.byPlace), shopItemData.sItem.byPosition, shopItemData.hItem);
		}
		if (m_aShopItemData[uiPrivateShopPos].byPrivateShopInventorySlotPos == PRIVATESHOP_ITEMPOS_INVALID &&
			pShopItemData->byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			++m_uiItemCount;

		/*	WCHAR awcBuffer[256];
			_itow_s(m_uiItemCount, awcBuffer, 10);
			CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, L"", (int)wcslen(awcBuffer), awcBuffer);*/
		}
		m_aShopItemData[uiPrivateShopPos] = *pShopItemData;
	}
	else
	{
		sPRIVATESHOP_ITEM_DATA& shopItemData = m_aShopItemData[uiPrivateShopPos];
		if (bEvent && shopItemData.hItem != INVALID_SERIAL_ID && !shopItemData.bIsSaveItem)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_PRIVATESHOP, PLACE_BAG, 
				Logic_ConvertContainerTypeToBagIdx(shopItemData.sItem.byPlace), shopItemData.sItem.byPosition, shopItemData.hItem);
		}
		if (m_aShopItemData[uiPrivateShopPos].byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			--m_uiItemCount;

			/*WCHAR awcBuffer[256];
			_itow_s(m_uiItemCount, awcBuffer, 10);
			CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, L"", (int)wcslen(awcBuffer), awcBuffer);*/
		}
		m_aShopItemData[uiPrivateShopPos] = sPRIVATESHOP_ITEM_DATA();
	}
}

/**
* \brief Render the effect.
*Draws the appropriate state for each item slot.
*m_pItemSelect: Selected state
*m_pItemFocus: Focused state
*/
void CPrivateShopGui::OnPaintEffect()
{
	if (m_apDialogTab[TAB_SHOP]->IsVisible())
	{
		if (m_pItemSelect != NULL)
		{
			m_EffectSelect.Render();
		}
		if (m_pItemFocus != NULL)
		{
			m_EffectFocus.Render();
		}
	}
}

void CPrivateShopGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	//m_BgLine.SetPositionbyParent(rect.left, rect.top);

	SetShopItemSelect(m_pItemSelect);
	SetShopItemFocus(m_pItemFocus);

	MoveLinkedPLGui(rect.left - iOldX, rect.top - iOldY);
}

void CPrivateShopGui::OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex)
{
	bool bBool = false;
	
	CRectangle rect = GetPosition();
	if (nIndex == TAB_SHOP)
	{
		bBool = true;		
	}
	else if (nIndex == TAB_NOTE)
	{
		if (!m_bSale)
		{
			m_pShopNameInput->SetFocus();
		}
		bBool = false;
	}

	m_apDialogTab[TAB_SHOP]->Show(bBool);
	m_apDialogTab[TAB_SHOP]->Enable(bBool);
	m_apDialogTab[TAB_NOTE]->Show(!bBool);
	m_apDialogTab[TAB_NOTE]->Enable(!bBool);
}

void CPrivateShopGui::OnClickedBtnClose(gui::CComponent* pComponent)
{
	switch (m_eShopType)
	{
	case PRIVATESHOP_BUY:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopLeaveReq(m_ShopData.hOwner);
			break;
		}
	case PRIVATESHOP_SELL:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopExitReq();
			break;
		}
	}
}

void CPrivateShopGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_PRIVATESHOP, key.m_fX, key.m_fY);
}

void CPrivateShopGui::OnTabShopMouseLeave(gui::CComponent* pComponent)
{
	//SetShopItemSelect(NULL);
}

void CPrivateShopGui::OnClickedBtnNext(gui::CComponent* pComponent)
{
	// If you are holding something
	if (GetIconMoveManager()->GetSrcPlace() == PLACE_PRIVATESHOP)
		GetIconMoveManager()->IconMoveEnd();


	if (m_uiPageCur < m_uiPageMax)
	{

		SetPage(m_uiPageCur + 1);
	}
	UpdateShopItem();
}

void CPrivateShopGui::OnClickedBtnPrev(gui::CComponent* pComponent)
{
	if (GetIconMoveManager()->GetSrcPlace() == PLACE_PRIVATESHOP)
		GetIconMoveManager()->IconMoveEnd();

	if (m_uiPageCur != 0)
	{
		
		SetPage(m_uiPageCur - 1);
	}
	UpdateShopItem();
}

void CPrivateShopGui::OnClickedBtnSale( gui::CComponent* pComponent ) 
{
	if (m_bSale)
	{
		//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_MSG_SALE_CLOSE), MBW_PRIVATESHOP_SALE_STOP, MBTF_OK | MBTF_CANCEL, TRUE);
		GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_SALE_CLOSE" );
	}
	else
	{
		if (GetChattingFilter()->IsSlang(m_pShopNameInput->GetText()))
		{
			//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_MSG_SHOPNAME_SLANGDER), MBW_NONE, MBTF_OK, TRUE);
			GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_SHOPNAME_SLANGDER" );
			return;
		}
		else if (GetChattingFilter()->IsSlang(m_pShopNoticeInput->GetText()))
		{
			//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_MSG_NOTICE_SLANGDER), MBW_NONE, MBTF_OK, TRUE);
			GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_NOTICE_SLANGDER" );
			return;
		}
		if (m_uiItemCount)
		{
			//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_MSG_SALE_OPEN), MBW_PRIVATESHOP_SALE_START, MBTF_OK | MBTF_CANCEL, TRUE);
			GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_SALE_OPEN" );
		}
		else
		{
			//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_MSG_SALE_ITEM), MBW_NONE, MBTF_OK, FALSE);
			GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_SALE_ITEM" );
		}
	}
}

void CPrivateShopGui::OnShopItemEnter(CPrivateShopItemGui* pShopItem)
{
	SetShopItemFocus(pShopItem);
	ShowInfoWindow(TRUE, pShopItem);
}

void CPrivateShopGui::ShowInfoWindow(RwBool bIsShow, CPrivateShopItemGui* pShopItem)
{
	if (bIsShow && pShopItem && pShopItem->GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle	rect		= pShopItem->GetShopItemRect();
		sITEM_DATA&	sItemData	= m_aShopItemData[pShopItem->GetPos()].sItem;
		if (sItemData.bNeedToIdentify)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, NULL, DIALOG_PRIVATESHOP);

		} 
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM_DATA, rect.left, rect.top, &sItemData, DIALOG_PRIVATESHOP);
		}		
	}
	else
	{
		if( DIALOG_PRIVATESHOP == GetInfoWndManager()->GetRequestGui() )
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CPrivateShopGui::OnShopItemLeave(CPrivateShopItemGui* pShopItem)
{
	if (pShopItem == pShopItem)
	{
		SetShopItemFocus(NULL);
		ShowInfoWindow(FALSE);
	}
}

void CPrivateShopGui::OnShopItemClicked(CPrivateShopItemGui* pShopItem, const CKey& Key)
{
	switch (m_eShopType)
	{
	case PRIVATESHOP_BUY:
		{
			// Currently, it can only be used with the right mouse button.
			if (Key.m_nID == UD_LEFT_BUTTON)
			{
				if (!GetIconMoveManager()->IsActive())
				{
					if (pShopItem->GetSerial() != INVALID_SERIAL_ID)
					{
						GetIconMoveManager()->IconMovePickUp(pShopItem->GetSerial(), PLACE_PRIVATESHOP,
							pShopItem->GetPos(), pShopItem->GetCount(), pShopItem->GetIcon()->GetTexture(), pShopItem->GetPrice(), (RwUInt32)&m_aShopItemData[pShopItem->GetPos()].sItem);
						SetShopItemSelect(pShopItem);
					}					
				}
				/*GetAlarmManager()->AlarmMessage( DST_PRIVATESHOP_MSG_BUY_PROCESS );*/
			}
			else if (Key.m_nID == UD_RIGHT_BUTTON)
			{
				if (pShopItem->GetSerial() != INVALID_SERIAL_ID)
				{
					/*CDboEventGenerator::RegPrivateShopCartItem(pShopItem->GetSerial(), PLACE_PRIVATESHOP, pShopItem->GetPos(),
						pShopItem->GetCount(), pShopItem->GetPrice(), (void*)&m_aShopItemData[pShopItem->GetPos()].sItem);*/

					GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemSelectReq(m_ShopData.hOwner, pShopItem->GetPos(), true);
				}
			}
			break;
		}
	case PRIVATESHOP_SELL:
		{
			if (m_bSale)
			{
				return;
			}

			if (Key.m_nID == UD_LEFT_BUTTON)
			{
				if (GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
				{
					// peessi: Logic changed due to addition of usage period limit attribute
					CNtlSobItemAttr* pSobItemAttr = Logic_GetItemSobAttrFromSobSerial( GetIconMoveManager()->GetSrcSerial() );
					sITEM_TBLDAT* pTlbData = NULL;

					if( pSobItemAttr )
						pTlbData = pSobItemAttr->GetItemTbl();

					if (!pTlbData ||
						(Logic_IsCanSellItem(pSobItemAttr) && Logic_IsCanUserTradeItem(pSobItemAttr) &&
						Logic_IsCanSaveWarehouse(pSobItemAttr) && Logic_IsCanSaveCommonWarehouse(pSobItemAttr)))
					{
						// Cz To Kell: Edit the source below
						//if (pShopItem->GetSerial() != INVALID_SERIAL_ID)
						if (GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
						{
							//CNtlSobItem*		pSobItem	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
							//CNtlSobItemAttr*	pSobItemAttr= reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

							if (pSobItemAttr->GetStackNum() == GetIconMoveManager()->GetStackCount())
							{
								RwUInt8 uiPrivateShopPos	= (RwUInt8)pShopItem->GetPos();
								RwUInt8 uiInventoryPlace	= (RwUInt8)Logic_WhichBagHasItem(GetIconMoveManager()->GetSrcSerial());
								RwUInt8 uiInventoryPos		= (RwUInt8)GetIconMoveManager()->GetSrcSlotIdx();

								GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemInsertReq(
									uiPrivateShopPos, uiInventoryPlace, uiInventoryPos);
							}
						}
					}
					else
					{
						const WCHAR* pwcString = GetDisplayStringManager()->GetString("DST_PRIVATESHOP_MSG_ITEM_NOT_REG");
						CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, L"", (RwUInt16)wcslen(pwcString), pwcString);
					}
				}
			}
			else if (Key.m_nID == UD_RIGHT_BUTTON)
			{
				if (pShopItem->GetSerial() != INVALID_SERIAL_ID)
				{
					RwUInt8 uiPrivateShopPos= pShopItem->GetPos();
					GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopItemDeleteReq(uiPrivateShopPos);

				}
			}
			break;
		}
	}
	ShowInfoWindow(FALSE);
}

/**
* \brief When the price button is pressed
* \param pShopItem The item of the personal shop that was pressed
* \param pComponent Clicked component
*/
void CPrivateShopGui::OnShopItemPriceClicked(CPrivateShopItemGui* pShopItem, gui::CComponent* pComponent)
{
	// Launches a calculator to adjust the price.
	CRectangle rect = pComponent->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, pShopItem->GetPos(), PLACE_PRIVATESHOP,
		rect.left - dPRIVATESHOP_UI_CALC_OFFSET, rect.top, NTL_MAX_USE_ZENI);

	
}


void CPrivateShopGui::SetUserInterface(ePrivateShopType eType, sPRIVATESHOP_SHOP_DATA* pShopData, bool bIsSale)
{
	m_eShopType	= eType;
	memcpy(&m_ShopData, pShopData, sizeof(sPRIVATESHOP_SHOP_DATA));

	// Buy : Sell
	bool bBool = false;
	switch(eType)
	{
	case PRIVATESHOP_BUY:
		{			
			m_pTabButton->ChangeTabText(TAB_SHOP, std::wstring(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_TAB_BUY_1")));
			m_pTabButton->ChangeTabText(TAB_NOTE, std::wstring(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_TAB_BUY_2")));

			m_pAwayButton->SetPosition(dPRIVATESHOP_UI_BUY_AWAY_LEFT, 413);
			bBool = false;
			break;
		}
	case PRIVATESHOP_SELL:
		{
			m_pTabButton->ChangeTabText(TAB_SHOP, std::wstring(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_TAB_SELL_1")));
			m_pTabButton->ChangeTabText(TAB_NOTE, std::wstring(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_TAB_SELL_2")));

			m_pAwayButton->SetPosition(dPRIVATESHOP_UI_SELL_AWAY_LEFT, 413);
			bBool = true;
			break;
		}
	}

	//Show & Enable
	m_pSaleButton->Show(bBool);
	m_pMailButton->Show(!bBool);
	
	// Common information
	SetAway(pShopData->sSummaryPrivateShopData.bIsOwnerEmpty);
	m_pZenny->SetText(Logic_FormatZeni(Logic_GetZenny()));
	
	if (lstrlenW(pShopData->sSummaryPrivateShopData.wcPrivateShopName) > 0)
	{
		m_pTitle->SetText(pShopData->sSummaryPrivateShopData.wcPrivateShopName);
		m_pShopNameInput->SetText(pShopData->sSummaryPrivateShopData.wcPrivateShopName);
	}
	else
	{
		WCHAR awcBuffer[256];
		swprintf_s(awcBuffer, L"%s%s", Logic_GetName(GetNtlSobManager()->GetSobObject(pShopData->hOwner)), GetDisplayStringManager()->GetString("DST_PRIVATESHOP_SHOPNAME_EMPTY"));
		
		m_pTitle->SetText(awcBuffer);
		m_pShopNameInput->SetText(awcBuffer);
	}

	if (lstrlenW(pShopData->wcNotice) > 0)
	{
		SetNotice(pShopData->wcNotice);
	}
	else
	{
		SetNotice(NULL);
	}
	SetPage(m_uiPageCur);
	SetSale(bIsSale);
}

void CPrivateShopGui::SetShopItemSelect(CPrivateShopItemGui* pShopItem)
{
	m_pItemSelect = pShopItem;
	if (m_pItemSelect != NULL)
	{
		m_EffectSelect.SetRect(m_pItemSelect->GetShopItemRect());
	}
}

void CPrivateShopGui::SetShopItemFocus(CPrivateShopItemGui* pShopItem)
{
	m_pItemFocus = pShopItem;
	if (m_pItemFocus != NULL)
	{
		m_EffectFocus.SetRect(m_pItemFocus->GetShopItemRect());
	}
}

void CPrivateShopGui::Clear()
{
	SetNotice(NULL);
	SetSale(false);
	SetShopItemFocus(NULL);
	SetShopItemSelect(NULL);
	
	// Tab Setting
	m_pTabButton->SelectTab(TAB_SHOP, false);
	m_apDialogTab[TAB_SHOP]->Show(TRUE);
	m_apDialogTab[TAB_SHOP]->Enable(TRUE);
	m_apDialogTab[TAB_NOTE]->Show(FALSE);
	m_apDialogTab[TAB_NOTE]->Enable(FALSE);

	ClearShopItem();
	for (RwUInt8 i = 0; i < NTL_MAX_PRIVATESHOP_INVENTORY; ++i)
	{
		SetShopItemData(i, NULL);
	}

	m_uiPageCur = 0;
	m_eShopType	= PRIVATESHOP_NONE;
	m_ShopData	= sPRIVATESHOP_SHOP_DATA();
}

void CPrivateShopGui::ClearShopItem()
{
	for (int i = 0; i < SLOT_NUM; ++i)
	{
		m_aShopItem[i].Clear();
	}
}

/**
* \brief Update
*Checks whether the status of the item has changed.
*/
void CPrivateShopGui::UpdateShopItem()
{
	CheckSoldOut();
	
	switch (m_eShopType)
	{
	case PRIVATESHOP_BUY:
		{
			RwInt32 iTopIndex	= m_uiPageCur * SLOT_NUM;
			RwInt32 iPos		= 0;
			RwInt32 iItemCount	= 0;

			for (int i = 0; i < SLOT_NUM; ++i)
			{
				for (int j = iPos; j < NTL_MAX_PRIVATESHOP_INVENTORY; ++j)
				{
					sPRIVATESHOP_ITEM_DATA& sPrivateShopItem = m_aShopItemData[j];
					if (sPrivateShopItem.sItem.itemNo != 0)
					{
						if (++iItemCount > iTopIndex)
						{
							iPos = j;
							break;
						}
					}
				}

				if (iPos < NTL_MAX_PRIVATESHOP_INVENTORY && m_aShopItemData[iPos].sItem.itemNo != 0)
				{
					sPRIVATESHOP_ITEM_DATA& sPrivateShopItem = m_aShopItemData[iPos];
					m_aShopItem[i].SetIcon((RwUInt8)iPos, sPrivateShopItem.sItem.itemNo,
						sPrivateShopItem.sItem.byStackcount, sPrivateShopItem.dwZenny, sPrivateShopItem.byItemState,
						(sPrivateShopItem.sItem.bNeedToIdentify ? REGULAR_SLOT_ITEM_NOT_IDENTIFICATION : REGULAR_SLOT_ITEM_TABLE));
					m_aShopItem[i].Show(TRUE);
					++iPos;
					continue;
				}

				// The buyer sees a bargain button.
				m_aShopItem[i].Show(FALSE);
			}
			break;
		}
	case PRIVATESHOP_SELL:
		{
			RwInt32 iTopIndex = m_uiPageCur * SLOT_NUM;

			for (int i = 0; i < SLOT_NUM; ++i)
			{
				sPRIVATESHOP_ITEM_DATA& sPrivateShopItem = m_aShopItemData[iTopIndex + i];

				m_aShopItem[i].SetIcon((RwUInt8)(iTopIndex + i), 
					(sPrivateShopItem.sItem.itemNo == 0 ? INVALID_SERIAL_ID : sPrivateShopItem.sItem.itemNo),
					sPrivateShopItem.sItem.byStackcount, sPrivateShopItem.dwZenny, sPrivateShopItem.byItemState,
					(sPrivateShopItem.sItem.bNeedToIdentify ? REGULAR_SLOT_ITEM_NOT_IDENTIFICATION : REGULAR_SLOT_ITEM_TABLE));

				if (m_bSale || m_aShopItem[i].GetSerial() == INVALID_SERIAL_ID) 
				{
					m_aShopItem[i].EnablePrice(FALSE);
				}
				else
				{
					m_aShopItem[i].EnablePrice(TRUE);
				}
			}	
			break;
		}
	}
}

void CPrivateShopGui::UpdateShopItemData()
{
	CNtlPrivateShop* pPrivateShop = GetNtlSLGlobal()->GetSobAvatar()->GetPrivateShop();

	for (BYTE i = 0; i < NTL_MAX_PRIVATESHOP_INVENTORY; ++i)
	{
		sPRIVATESHOP_ITEM_DATA* pShopItem = pPrivateShop->GetPrivateShopItemData(i);
		if( !pShopItem->bIsSaveItem )
			continue;

		m_aShopItemData[i] = *pShopItem;

		if (m_aShopItemData[i].byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
		{
			++m_uiItemCount;
		}
	}
}

void CPrivateShopGui::SetPage(RwUInt32 iPage, bool bCheckMax)
{
	if (bCheckMax)
	{
		switch (m_eShopType)
		{
		case PRIVATESHOP_BUY:
			{
				int iNum	= 0;
				m_uiPageMax	= 1;
				for (int i = 0; i < NTL_MAX_PRIVATESHOP_INVENTORY; ++i)
				{
					if (m_aShopItemData[i].sItem.itemNo != 0)
					{
						if (++iNum > SLOT_NUM)
						{
							iNum = 1;
							++m_uiPageMax;
						}				
					}
				}
				break;
			}
		case PRIVATESHOP_SELL:
			{
				m_uiPageMax = NTL_MAX_PRIVATESHOP_INVENTORY / SLOT_NUM;
			}
		}
	}

	if (iPage >= m_uiPageMax)
	{
		m_uiPageCur = m_uiPageMax - 1;
	}
	else
	{
		m_uiPageCur = iPage;
	}
	
	m_pCurrentPage->Format(L"%u / %u", m_uiPageCur + 1, m_uiPageMax);
}

void CPrivateShopGui::SetSale(bool bSale)
{
	m_bSale = bSale;
	
	//m_pSaleButton->SetDown(m_bSale);
	m_pShopNameInput->Enable(!m_bSale);

	m_pShopNoticeInput->Enable(!m_bSale);
	m_pAwayButton->ClickEnable(!m_bSale);

	if (m_bSale)
	{
		m_pSaleButton->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_SALE_STOP"));
		m_pNoticeMaxNum->Format(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_NOTICE_FOOT"), Logic_GetName(GetNtlSobManager()->GetSobObject(m_ShopData.hOwner)));
	}
	else
	{
		m_pSaleButton->SetText(GetDisplayStringManager()->GetString("DST_PRIVATESHOP_SALE_START"));
		CalcNoticeMaxNum();
	}
}

void CPrivateShopGui::SetAway(bool bAway)
{
	m_bAway = bAway;	
}

void CPrivateShopGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

void CPrivateShopGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

void CPrivateShopGui::OnClickedBtnMail(gui::CComponent* pComponent)
{
	if (m_ShopData.hOwner != INVALID_SERIAL_ID)
	{
		CNtlSobPlayer*		pPlayer		= reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject(m_ShopData.hOwner));
		CNtlSobPlayerAttr*	pPlayerAttr	= reinterpret_cast<CNtlSobPlayerAttr*>(pPlayer->GetSobAttr());

		CDboEventGenerator::OpenSendMail(pPlayerAttr->GetName());
	}
}

void CPrivateShopGui::OnClickedBtnAway(gui::CComponent* pComponent)
{
	SetAway(!m_bAway);
}

void CPrivateShopGui::OnPaintBtnAway()
{
	if (m_bAway)
	{
		m_surAwayCheck.Render();
	}
	else
	{
		m_surAwayUnCheck.Render();
	}
}

void CPrivateShopGui::OnMovedBtnAway(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pAwayButton->GetScreenRect();
	m_surAwayCheck.SetPositionbyParent(rect.left, rect.top);
	m_surAwayUnCheck.SetPositionbyParent(rect.left, rect.top);
}

void CPrivateShopGui::OnKeyUp(gui::CComponent* pComponenet, CInputDevice* pDevice, const CKey& key)
{
	if (key.m_dwVKey == VK_TAB)
	{
		if (pComponenet == m_pShopNoticeInput)
		{
			m_pShopNameInput->SetFocus();
		}
		else if (pComponenet == m_pShopNameInput)
		{
			m_pShopNoticeInput->SetFocus();
		}
	}
}

void CPrivateShopGui::OnNoticeCharChanged(int iLength)
{
	// Printed when the maximum number of characters in the input box is exceeded.
	CalcNoticeMaxNum();	
	if( iLength >= m_pShopNoticeInput->GetMaxLength() )
	{
		GetAlarmManager()->AlarmMessage( "DST_PRIVATESHOP_MSG_NOTICE_BUFFEROVER" );
	}
}


void CPrivateShopGui::SetNotice(const WCHAR* pText)
{
	if (pText)
	{
		m_pShopNoticeInput->SetText(pText);
	}
	else
	{
		m_pShopNoticeInput->Clear();
	}
}

void CPrivateShopGui::CalcNoticeMaxNum()
{
	m_pNoticeMaxNum->Format("(%u/%u)", m_pShopNoticeInput->GetLength(),m_pShopNoticeInput->GetMaxLength());
}

void CPrivateShopGui::CheckSoldOut()
{
	bool bSoldOut = false;
	if (m_uiItemCount == 0 && m_bSale)
	{
		bSoldOut = true;
	}
	
	if (m_pSoldOut->IsVisible() != bSoldOut)
	{
		m_pSoldOut->Show(bSoldOut);
	}
	
	if(m_pStbSoldOut->IsVisible() != bSoldOut )
	{
		m_pStbSoldOut->Show(bSoldOut);
	}

}
