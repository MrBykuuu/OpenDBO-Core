#pragma once

#include "NtlDebug.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "ButtonList.h"
#include "DragonBallRewardTable.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "SurfaceGui.h"
#include "DragonBallTable.h"
#include "DragonBallRewardTable.h"

class CTextTable;
class CNtlWorldConceptDBC;

#define REWARD_LIST_MAX_VISIBLE		4

/// Reward GUI status
enum ERewardUIStatus
{
	E_REWARD_STATUS_SELECT,			///< Reward selection step
	E_REWARD_STATUS_CONFIRM,		///< Reward confirmation step
};


/// Nodes that make up the tree structure of reward items
struct SRewardItem
{
	sDRAGONBALL_REWARD_TBLDAT* pData;			///< Reward Table Data
	SRewardItem* pParent;
	std::vector<SRewardItem*> itemChild;	///< Child Items

	SRewardItem()
	{
		pData = NULL;
		pParent = NULL;
	}
	~SRewardItem()
	{
		for each(SRewardItem* pItem in itemChild)
		{
			NTL_DELETE(pItem);
		}
	}		
};


/**
 * \ingroup Client
*  \brief Wish reward GUI after summoning the Dragon God
 * \date 2007-06-24
 * \author agebreak
 */
class CDBCRewardGUI : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDBCRewardGUI(void);
	CDBCRewardGUI(const RwChar* pName);
	virtual ~CDBCRewardGUI(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);
	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	 // create/remove singleton
	static	void	CreateInstance();
	static	void	DeleteInstance();

	VOID			AdjustDialog();							///< Align dialog position

protected:
	// CallBack
	VOID	OnClickBackBtn(gui::CComponent* pComponent);
	VOID	OnClickOkBtn(gui::CComponent* pComponent);
	VOID	OnClickCancelBtn(gui::CComponent* pComponent);
	VOID	OnPaint();
	VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID	OnMouseLeave(gui::CComponent* pComponent);
	VOID	OnScrollBarChanged(RwInt32 iValue);
    VOID	OnCaptureMouseDown(const CKey& key);
    VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	
	// Select Item CallBack
	gui::CButton*	OnCreateItem();
	void			OnDestroyItem(gui::CButton* pButton);
	void			OnClickedItem(RwInt32 iIndex);


	VOID			SetStatus(ERewardUIStatus eStatus);		///< Set the current state of the reward window UI.
	
	void			ShowList(SRewardItem* pItem, RwInt32 nPos);
	void			CreateRewardTree(SRewardItem* pParent, RwInt32 nDepth);						///< Build reward data into a tree.
	void			ShowItemInfo(SRewardItem* pItem);		//< Displays reward item information.
	void			UnLockDragonBall();						///< Unlocks all locked dragon balls.
	RwBool			IsExistSkill(RwUInt32 nTblIdx);			///< Check if the skill is already available.

protected:
	static	CDBCRewardGUI*	m_pInstance;			///< Singleton instance object

	WCHAR					m_pBuf[20];
	RwBool					m_bDisplayHurryUp;		///< Flag to display the message that there is no time
	RwBool					m_bDelete;				///< Flag to delete
	DWORD					m_nDepthIdx;			///< Index value of the depth of the selected item
	DWORD					m_nPrevDepthIdx;		///< Previous item Depth index value
    RwInt32                 m_nBtnCount;            ///< number of buttons created	
	
	CTextTable*				m_pDBRewardTextTable;	///< Reward related text table
	CDragonBallTable*		m_pDragonBallTable;		///< Dragon Ball table
	CDragonBallRewardTable* m_pDragonBallRewardTable; ///< Reward table
	
	SRewardItem				m_RewardItemRoot;		///< Root node of the reward item tree
	SRewardItem*			m_pPrevItem;			///< One step up tree node
	SRewardItem*			m_pRewardItem;			///< Final reward item
	sTBLDAT*				m_pRewardItemTblData;	///< Table data of reward item

	CNtlWorldConceptDBC*	m_pWorldConceptDBC;		///< DBC Concept controller

	// gui
    gui::CPanel*            m_pPnlSelectMenu;
    gui::CPanel*            m_pPnlSelectItem;
	gui::CButton*			m_pBtnBack;
	gui::CButton*			m_pBtnOK;
	gui::CButton*			m_pBtnCancel;	
	gui::CStaticBox*		m_pSttTime;
	gui::CStaticBox*		m_pSttItemName;
	gui::CStaticBox*		m_pSttItemText;	
	gui::CScrollBar*		m_pScrollBar;
	CSurfaceGui				m_sufItem;				/// < Surface to display the item's Icon

	CButtonList<CDBCRewardGUI, SRewardItem*>	m_ButtonList;	///< Item Button List

	// slot
	gui::CSlot				m_slotClickExitBtn;
	gui::CSlot				m_slotClickBackBtn;
	gui::CSlot				m_slotClickOKBtn;
	gui::CSlot				m_slotClickCancelBtn;
	gui::CSlot				m_slotPaint;
	gui::CSlot				m_slotMove;
	gui::CSlot				m_slotMouseMove;
	gui::CSlot				m_slotMouseLeave;
	gui::CSlot				m_slotSliderMove;
	gui::CSlot				m_slotSliderChanged;
    gui::CSlot  			m_slotCaptureMouseDown;
    gui::CSlot              m_slotListWheelMove[REWARD_LIST_MAX_VISIBLE];
};	
