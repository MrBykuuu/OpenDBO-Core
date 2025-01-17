#include "precomp_dboclient.h"
#include "HLShopBuyConfirmGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "DisplayStringManager.h"
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// Table
#include "HLSItemTable.h"



CHLShopBuyConfirmGui::CHLShopBuyConfirmGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CHLShopBuyConfirmGui::~CHLShopBuyConfirmGui()
{
}

void CHLShopBuyConfirmGui::Init()
{
	m_hlsItemTblidx = INVALID_TBLIDX;
	m_bHasEnoughCash = true;
}

RwBool CHLShopBuyConfirmGui::Create()
{
	NTL_FUNCTION("CHLShopBuyConfirmGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLSConfirm.srf", "gui\\HLSFinalConfirm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_srfBackground.SetType(CWindowby3::WT_HORIZONTAL);
	m_srfBackground.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackTop"));
	m_srfBackground.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackMiddle"));
	m_srfBackground.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSConfirm.srf", "srfConfirmBackBottom"));
	m_srfBackground.SetSize(334, 373);
	m_srfBackground.SetPositionfromParent(0, 0);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BUY_TITLE"));

	m_pstbConfirmGuide = (gui::CStaticBox*)GetComponent("stbConfirmGuide");
	m_pstbConfirmGuide->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_RESULT_GUIDE"));

	m_pdlgEachBuy = (gui::CDialog*)GetComponent("dlgEachBuy");

	m_ppnlBlackSlot = (gui::CPanel*)GetComponent("pnlBlackSlot");
	m_surBackSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBackSlot"));
	m_surBackSlot.SetSize(m_ppnlBlackSlot->GetWidth(), m_ppnlBlackSlot->GetHeight());
	m_surBackSlot.SetPositionbyParent(m_ppnlBlackSlot->GetScreenRect().left, m_ppnlBlackSlot->GetScreenRect().top);

	m_pstbItemName = (gui::CStaticBox*)GetComponent("stbItemName");
	m_pstbItemInfo = (gui::CStaticBox*)GetComponent("stbItemInfo");
	m_ppnlCashInfoBack = (gui::CPanel*)GetComponent("pnlCashInfoBack");

	m_pstbRequireCashTitle = (gui::CStaticBox*)GetComponent("stbRequireCashTitle");
	m_pstbRequireCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_NEED_CASH"));

	m_ppnlRequireCashBack = (gui::CPanel*)GetComponent("pnlRequireCashBack");
	m_ppnlCashMark0 = (gui::CPanel*)GetComponent("pnlCashMark0");

	m_pstbRequireCash = (gui::CStaticBox*)GetComponent("stbRequireCash");
	m_pstbRequireCash->SetText(L"0");

	m_pstbHaveCashTitle = (gui::CStaticBox*)GetComponent("stbHaveCashTitle");
	m_pstbHaveCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_HAVE_CASH"));

	m_ppnlHaveCashBack = (gui::CPanel*)GetComponent("pnlHaveCashBack");
	m_ppnlCashMark1 = (gui::CPanel*)GetComponent("pnlCashMark1");
	m_pstbHaveCash = (gui::CStaticBox*)GetComponent("stbHaveCash");
	m_pstbHaveCash->SetText(L"0");

	m_pstbRemainCashTitle = (gui::CStaticBox*)GetComponent("stbRemainCashTitle");
	m_pstbRemainCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_REMAIN_CASH"));

	m_ppnlRemainCashBack = (gui::CPanel*)GetComponent("pnlRemainCashBack");
	m_ppnlCashMark2 = (gui::CPanel*)GetComponent("pnlCashMark2");
	m_pstbRemainCash = (gui::CStaticBox*)GetComponent("stbRemainCash");
	m_pstbRemainCash->SetText(L"0");


	m_pbtnBuy = (gui::CButton*)GetComponent("btnBuy");
	m_pbtnBuy->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_BUY"));
	m_slotClickedBtnBuy = m_pbtnBuy->SigClicked().Connect(this, &CHLShopBuyConfirmGui::OnClickedBtnBuy);

	m_pbtnCancel = (gui::CButton*)GetComponent("btnCancel");
	m_pbtnCancel->SetText(GetDisplayStringManager()->GetString("DST_HLS_CONFIRM_BTN_CANCEL"));
	m_slotClickedBtnCancel = m_pbtnCancel->SigClicked().Connect(this, &CHLShopBuyConfirmGui::OnClickedBtnClose);

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pbtnClose->SigClicked().Connect(this, &CHLShopBuyConfirmGui::OnClickedBtnClose);

	m_slotMove = m_pThis->SigMove().Connect(this, &CHLShopBuyConfirmGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLShopBuyConfirmGui::OnPaint);
	m_slotMouseEnterItem = m_ppnlBlackSlot->SigMouseEnter().Connect(this, &CHLShopBuyConfirmGui::OnMouseEnterItem);
	m_slotMouseLeaveItem = m_ppnlBlackSlot->SigMouseLeave().Connect(this, &CHLShopBuyConfirmGui::OnMouseLeaveItem);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOPBUY);


	LinkMsg(g_EventHLShopEventBuy);
	LinkMsg(g_EventHLShopEventItemBuyRes);

	Show(false);

	NTL_RETURN(TRUE);
}

void CHLShopBuyConfirmGui::Destroy()
{
	NTL_FUNCTION("CHLShopBuyGui::Destroy");

	UnLinkMsg(g_EventHLShopEventBuy);
	UnLinkMsg(g_EventHLShopEventItemBuyRes);

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CHLShopBuyConfirmGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_BUY_CONFIRM)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_bHasEnoughCash = true;
		m_hlsItemTblidx = INVALID_TBLIDX;
		//m_ItemSlot.Destroy();
	}
	else
	{
	}

	NTL_RETURN(TRUE);
}

void CHLShopBuyConfirmGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventHLShopEventBuy)
	{
		SDboEventHLShopEventBuy* pData = reinterpret_cast<SDboEventHLShopEventBuy*>(msg.pData);
		if (pData->bConfirm == true)
		{
			m_hlsItemTblidx = pData->hlsItemIdx;

			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			// get hls item
			sHLS_ITEM_TBLDAT* pHlsItemTbldat = (sHLS_ITEM_TBLDAT*)API_GetTableContainer()->GetHLSItemTable()->FindData(pData->hlsItemIdx);

			// get item
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItemTbldat->itemTblidx);

			// set icon
			m_ItemSlot.Create(m_ppnlBlackSlot, DIALOG_HLSHOP_BUY_CONFIRM, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
			m_ItemSlot.SetSize(NTL_ITEM_ICON_SIZE);
			m_ItemSlot.SetPosition_fromParent(2, 2);
			m_ItemSlot.SetParentPosition(m_ppnlBlackSlot->GetScreenRect().left, m_ppnlBlackSlot->GetScreenRect().top);
			m_ItemSlot.SetIcon(pItemTbldat->tblidx, pHlsItemTbldat->byStackCount);

			// set item name
			m_pstbItemName->SetText(pItemTextTable->GetText(pItemTbldat->Name).c_str());

			// set item info
			m_pstbItemInfo->SetText(pItemTextTable->GetText(pItemTbldat->Note).c_str());

			int nPrice = (int)pHlsItemTbldat->dwCash;

			// set required cash
			m_pstbRequireCash->SetText(Logic_FormatZeni(nPrice));

			int nHaveCash = (int)Logic_GetHlsCash();

			//set have cash
			m_pstbHaveCash->SetText(Logic_FormatZeni(nHaveCash));

			//set remain cash
			int nRemainCash = nHaveCash - nPrice;
			if (nRemainCash < 0)
			{
				m_bHasEnoughCash = false;

				m_pstbRemainCash->SetText(nRemainCash);
			}
			else
			{
				m_pstbRemainCash->SetText(Logic_FormatZeni(nRemainCash));
			}

			// close buy dialog
			GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY);

			// open the dialog
			GetDialogManager()->OpenDialog(DIALOG_HLSHOP_BUY_CONFIRM);
		}
	}
	else if (msg.Id == g_EventHLShopEventItemBuyRes)
	{
		GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY_CONFIRM);
	}
}

void CHLShopBuyConfirmGui::OnPaint()
{
	m_srfBackground.Render();
	m_surBackSlot.Render();
	m_ItemSlot.Paint();
}

void CHLShopBuyConfirmGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_srfBackground.SetPositionbyParent(rect.left, rect.top);

	CRectangle itemRect = m_ppnlBlackSlot->GetScreenRect();

	m_surBackSlot.SetPositionbyParent(itemRect.left, itemRect.top);
	m_ItemSlot.SetParentPosition(itemRect.left, itemRect.top);
}

void CHLShopBuyConfirmGui::OnClickedBtnBuy(gui::CComponent * pComponent)
{
	if (m_bHasEnoughCash)
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopBuyReq(m_hlsItemTblidx);
	}
	else
	{
		GetAlarmManager()->AlarmMessage("CASHITEM_FAIL_NEED_MORE_CASH", TRUE);
	}
}

void CHLShopBuyConfirmGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY_CONFIRM);
}

void CHLShopBuyConfirmGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(true);
}

void CHLShopBuyConfirmGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	ShowItemInfoWindow(false);
}

void CHLShopBuyConfirmGui::ShowItemInfoWindow(bool bIsShow)
{
	if (bIsShow && m_ItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_ppnlBlackSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, m_ItemSlot.GetItemTable(), DIALOG_HLSHOP_BUY_CONFIRM);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP_BUY_CONFIRM)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}