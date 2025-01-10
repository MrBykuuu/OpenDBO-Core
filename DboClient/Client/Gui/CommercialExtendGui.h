#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "surfaceGui.h"
#include "Windowby3.h"
#include "NtlItem.h"
#include "NtlSobItem.h"
/**
* \ingroup Client
* \brief Processing of extension of period related to commercialization, period extension confirmation 3 (Zenny, Netpy, Cash) + Purchase confirmation (Cash)
*Size change: When the dialog cache is used, the size is default, Zenny, and netfuser size are reduced.
*
*CCommercialExtendGui is basically a Gui for processing period extensions.
*	
*The most important entry point that handles the first purchase event of a limited-term item
*
* 
* \date 2009-08-14
* \author woosung_test
*/

class sITEM_TBLDAT;

class CCommercialExtendMode
{
public:
	enum	GUI_SIZE_MODE		{ SIZE_DEFAULT, SIZE_DETAIL_DESC_DELETED	};
	enum	GUI_ARRANGE_MODE	{ DEFAULT_BUTTONS, CASH_EXTEND_BUTTONS		};

	class CAttributeControlSet
	{
	public:
		GUI_SIZE_MODE		eSizeMode;
		GUI_ARRANGE_MODE	eArrangeMode;
		int					iDefaultWndHeight;
		int					iDefaultDescript1Top;
		int					iDefaultCancelButtLeft;
		/// external link
		gui::CDialog*		pDialog;						// change dialog size
		gui::CStaticBox*	pStatic_stbTitle;				// title can be changed
		gui::CStaticBox*	pStatic_stbConsumeGoldTitle;
		gui::CStaticBox*	pStatic_stbMyGoldTitle;
		gui::CStaticBox*	pStatic_stbRemainTime;
		gui::CStaticBox*	pStatic_stbAddableTime;
		gui::CStaticBox*	pStatic_stbConsumeGold;
		CSurfaceGui*		pSurConsumeGoldIcon;			// Texture may change
		gui::CStaticBox*	pStatic_stbMyGold;
		CSurfaceGui*		pSurMyGoldIcon;					// Texture may change
		gui::CPanel*		pPanel_pnlPanel2;
		gui::COutputBox*		pInput_opbDescriptDetail;
		gui::CStaticBox*	pStatic_stbDescript1;			// change
		gui::CStaticBox*	pStatic_stbDescript2;			// change
		gui::CButton*		pButt_btnConfirm;				// Can be hidden
		gui::CButton*		pButt_btnBuyDuration;			// hidden
		gui::CButton*		pButt_btnRefillCash;			// hidden
		gui::CButton*		pButt_btnCancel;				// change location
	};
protected:
	CAttributeControlSet	m_AttributeControlSet;
	CNtlSobItem*			m_pTargetNtlSobItem;			// Target item for extension

	RwUInt32				m_uiConsumeGold;
	RwUInt32				m_uiMyGold;

public:
	virtual void			Init() = 0;
	virtual RwBool			ConFirm() = 0;
	virtual void			Cancel() = 0;

	virtual void			CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	virtual void			SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);

	void					InitShowComponent();
};

class CCommercialExtendZennyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendNetphyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendCashMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					Buy();
	void					Refill();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendCashBuyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum	GUI_EXTEND_MODE		{ ZENNY_EXTEND, NETPY_EXTEND,    CASH_EXTEND, CASH_BUY,	NUM_EXTEND_MODE };

protected:

	GUI_EXTEND_MODE				m_eCurrExtendGuiMode;	/// This GUI operates in 4 modes.
	CCommercialExtendMode*		m_pArrayCommercialExtendMode[NUM_EXTEND_MODE];

	RwUInt32					m_uiDurItemIdx;				/// Temporary items to trade
		
	CNtlSobItem*				m_pTargetNtlSobItem;		/// Items to be extended
	sITEM_TBLDAT*				m_pCurrITEM_TBLDA;			/// Item information ref for extension of period


	class CDataSendBuyDurItemNPC
	{
	public:
		RwUInt32				m_uiNPCSerial;					/// NPC Shop		/// ZENNY_EXTEND	
		sSHOP_BUY_CART			m_ShopBuyCart;										/// ZENNY_EXTEND
	};
	class CDataSendBuyDurItemNetpy
	{
	public:
		RwUInt32				m_uiMerchantTblidx;				/// NETPY Shop		/// NETPY_EXTEND	
		BYTE					m_byMerchanSlotPos;									/// NETPY_EXTEND	
	};
	class CDataSendMoveDurItemFromYardrat
	{
	public:
		RwUInt64				m_uiProductId;					/// Yardrat Ã¢°í¿ë		/// CASH_EXTEND, CASH_BUY
	};
	typedef union
	{
		CDataSendBuyDurItemNPC			DataSendBuyDurItemNPC;
		CDataSendBuyDurItemNetpy		DataSendBuyDurItemNetpy;
		CDataSendMoveDurItemFromYardrat	DataSendMoveDurItemFromYardrat;
	} UNI_NET_SEND;

	UNI_NET_SEND				m_NetSendData;		

public:

	CCommercialExtendGui( const RwChar* pName );
	~CCommercialExtendGui(VOID);

	//! Operation
	VOID						Init(VOID);
	virtual RwBool				Create(VOID);
	virtual VOID				Destroy(VOID); 

	VOID						UpdateSurfacePos();

	RwInt32						SwitchDialog(bool bOpen);
	
	VOID						OpenDialog(GUI_EXTEND_MODE eMode, sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiItemIdx);
	VOID						OpenDialog();

	VOID						StartBuyDurItemProcessNPC( RwUInt32 uiItemIdx, RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart ); /// NPC, 
	VOID						StartBuyDurItemProcessNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos ); /// Netpy Shop
	sITEM_TBLDAT*				StartBuyDurItemProcessCommonForItem( RwUInt32 uiItemIdx );
	VOID						StartMoveDurItemProcessCash( RwUInt32 uiItemIdx, RwUInt32 uiProductId );						/// Yardrat
	// Net
	VOID						NetSendBuyDurItemNPC();				/// NPC Shop
	VOID						NetSendBuyDurItemNetpy();			/// Netpy Shop
	VOID						NetSendMoveDurItemFromYardrat();	/// Yardrat

	//! Event
	virtual VOID				HandleEvents( RWS::CMsg& msg );
	VOID						HandleEventsSubMsgBox( RWS::CMsg& msg );

	//! Access
	VOID						SetDurationItem(RwUInt32 uiItemIdx);
	VOID						SetExtendTargetItem(CNtlSobItem* pNtlSobItem);
	VOID						SetCurrItemTbl(RwUInt32 uiItemIdx);
	VOID						SetCurrItemTbl(sITEM_TBLDAT* pITEM_TBLDAT);	/// All item related information setting
																			/// GUI_EXTEND_MODE must be determined before setting
protected:
	//! Implementation
	VOID						ChangeExtendMode(GUI_EXTEND_MODE eMode);
	RwBool						SetIcon(const RwChar* pcIconName);

	VOID						OnMouseEnter_pnlItemButt(gui::CComponent* pComponent);			
	VOID						OnMouseLeave_pnlItemButt(gui::CComponent* pComponent);
	VOID						OnClicked_btnConfirm(gui::CComponent* pComponent);
	VOID						OnClicked_btnCancel(gui::CComponent* pComponent);
	VOID						OnClicked_btnBuyDuration(gui::CComponent* pComponent);
	VOID						OnClicked_btnRefillCash(gui::CComponent* pComponent);
	VOID						OnClicked_btnClose(gui::CComponent* pComponent);
	VOID						OnPaint(VOID);
	VOID						OnMove(int nX,int nY);

	// Gui Components
	gui::CStaticBox*			m_pStatic_stbTitle;					// title can be changed
	gui::CPanel*				m_pPanel_pnlItemButt;
	//gui::CPanel*				m_pPanel_pnlPanel1;
	gui::CStaticBox*			m_pStatic_stbRemainTimeTitle;
	gui::CStaticBox*			m_pStatic_stbAddableTimeTitle;
	gui::CStaticBox*			m_pStatic_stbConsumeGoldTitle;
	gui::CStaticBox*			m_pStatic_stbMyGoldTitle;
	gui::CStaticBox*			m_pStatic_stbRemainTime;
	gui::CStaticBox*			m_pStatic_stbAddableTime;
	gui::CStaticBox*			m_pStatic_stbConsumeGold;
	gui::CPanel*				m_pPanel_pnlPanelConsumeGoldIcon;	
	gui::CStaticBox*			m_pStatic_stbMyGold;
	gui::CPanel*				m_pPanel_pnlPanelMyGoldIcon;		

	gui::CPanel*				m_pPanel_pnlPanel2;					// Can be hidden
	gui::COutputBox*			m_pInput_opbDescriptDetail;

	gui::CPanel*				m_pPanel_pnlPanelName;
	gui::CStaticBox*			m_pStatic_stbItemName;
	gui::CStaticBox*			m_pStatic_stbDescript1;				// change
	gui::CStaticBox*			m_pStatic_stbDescript2;				// change
	gui::CButton*				m_pButt_btnConfirm;					// Can be hidden
	gui::CButton*				m_pButt_btnCancel;					// Change location
	gui::CButton*				m_pButt_btnBuyDuration;				// hidden
	gui::CButton*				m_pButt_btnRefillCash;				// hidden
	gui::CButton*				m_pButt_btnClose;

	// Variable
	CSurfaceGui					m_surItemIconSlot;
	CSurfaceGui					m_surEnterSlot;
	CSurfaceGui					m_surConsumeGoldIcon;				// Texture may change
	CSurfaceGui					m_surMyGoldIcon;					// Texture may change

	CWindowby3					m_DialogSurface;					///< Dialog image whose size can be changed

	// Gui Signals
	gui::CSlot					m_slotGotFocus_pnlItemButt;
	gui::CSlot					m_slotLostFocus_pnlItemButt;
	gui::CSlot					m_slotClicked_btnConfirm;
	gui::CSlot					m_slotClicked_btnCancel;
	gui::CSlot					m_slotClicked_btnBuyDuration;
	gui::CSlot					m_slotClicked_btnRefillCash;
	gui::CSlot					m_slotClicked_btnClose;

	gui::CSlot					m_slotPaint_dlgMain;
	gui::CSlot					m_slotMove_dlgMain;

};