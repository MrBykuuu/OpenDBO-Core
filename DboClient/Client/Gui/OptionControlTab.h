/******************************************************************************
*File			: OptionControlTab.h
*Author		    : Cho Hae sung
*Copyright		: (주)NTL
*Date			: 2007. 10. 15 
*Abstract		: 
*****************************************************************************
*Desc			: Tab of Options
*****************************************************************************/

#ifndef _OPTIONCONTROLTAB_H_
#define _OPTIONCONTROLTAB_H_

#pragma once

#include "GuiLineTree.h"

// OptionControlList의 Size
#define dOPTIONCONTROL_LIST_X			0
#define dOPTIONCONTROL_LIST_Y			0
#define dOPTIONCONTROL_LIST_WIDTH		288
#define dOPTIONCONTROL_LIST_HEIGHT		240

// OptionControlList ScrollBar의 Size
#define dOPTIONCONTROL_LIST_SCROLL_WIDTH			12
#define dOPTIONCONTROL_LIST_SCROLL_HEIGHT			19
#define dOPTIONCONTROL_LIST_SLIDER_WIDTH			12
#define dOPTIONCONTROL_LIST_SLIDER_HEIGHT			39

// StaticBox in OptionControl (Explanation)
#define dOPTIONCONTROL_STATICBOX_X		0
#define dOPTIONCONTROL_STATICBOX_Y		0
#define dOPTIONCONTROL_STATICBOX_WIDTH	288
#define dOPTIONCONTROL_STATICBOX_HEIGHT 415
#define dOPTIONCONTROL_STATICBOX_COLOR			RGB(255, 219, 71)
#define dOPTIONCONTROL_STATICBOX_WARNING_COLOR	RGB(255, 219, 71)
#define dOPTIONCONTROL_STATICBOX_GUIDE_COLOR	RGB(255, 219, 71)

//#define dOPTIONCONTROL_STATICBOX_WARNING_COLOR	RGB(255, 0, 0)
//#define dOPTIONCONTROL_STATICBOX_GUIDE_COLOR	RGB(0, 255, 255)

// COptionControlCategoryNode [+] Button의 Size
#define dOPTIONCONTROL_CATEGORY_BUTTON_X		0
#define dOPTIONCONTROL_CATEGORY_BUTTON_Y		0
#define dOPTIONCONTROL_CATEGORY_BUTTON_WIDTH	140
#define dOPTIONCONTROL_CATEGORY_BUTTON_HEIGHT	20

// COptionControlCategoryNode Title의 Size
#define dOPTIONCONTROL_CATEGORY_TITLE_X 30
#define dOPTIONCONTROL_CATEGORY_TITLE_Y 0
#define dOPTIONCONTROL_CATEGORY_TITLE_WIDTH		140
#define dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT	20
#define dOPTIONCONTROL_CATEGORY_TITLE_COLOR			RGB(255, 255, 255)
#define dOPTIONCONTROL_CATEGORY_TITLE_COLOR_DOWN	RGB( 255, 219, 71 )

// COptionControlActionNode StaticBox of Size
#define dOPTIONCONTROL_ACTIONNODE_TITLE_X 0
#define dOPTIONCONTROL_ACTIONNODE_TITLE_Y 0
#define dOPTIONCONTROL_ACTIONNODE_TITLE_WIDTH 147
#define dOPTIONCONTROL_ACTIONNODE_TITLE_HEIGHT 19
#define dOPTIONCONTROL_ACTIONNODE_TITLE_COLOR RGB(255, 255, 255)

// Action Node Statib Box Text of Coordinate
#define dOPTIONCONTROL_ACTIONNODE_TEXT_COORD_X 10
#define dOPTIONCONTROL_ACTIONNODE_TEXT_COORD_Y 3

// COptionControlActionNode Button of Size
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_X 150
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_Y 0
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_WIDTH 91
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_HEIGHT 19
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_COLOR RGB(255, 255, 255)

// Size of items that make up CGuiLineTree
#define dOPTIONCONTROL_LINE_HEIGHT				20
#define dOPTIONCONTROL_LINE_MARGIN				4	
#define dOPTIONCONTROL_CHILD_MARGIN_WIDTH		20

// Standards for grouping actions
#define dOPTIONCONTROL_CATEGORY_AVATAR			-2	// Avatar related
#define dOPTIONCONTROL_CATEGORY_TARGET			-3	// target behavior
#define dOPTIONCONTROL_CATEGORY_QUICKSLOT		-4	// Quick Slot Action
#define dOPTIONCONTROL_CATEGORY_WINDOW			-5	// Windows-related behavior
#define dOPTIONCONTROL_CATEGORY_FIXED			-6	// Fixed (shortcut key cannot be changed)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : COptionControlCategoryNode, COptionControlActionNode, COptionControlList, COptionControl
// Modified by Kell ( 2007. 10. 01 )
//
// COptionControlList(CGuiLineTree)의 구성
// (RootNode)
//     ├ COptionControlCategoryNode(CGuiLineTreeNode) (e.g. [+] related to avatar behavior )
//     │ ├ COptionControlActionNode(CGuiLineTreeNode)  (Example: [Avatar Forward] [W] )
//     │ ├ COptionControlActionNode(CGuiLineTreeNode)
//     │ └ ...
//     ├ COptionControlCategoryNode(CGuiLineTreeNode)
//     └ ...
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \ingroup Client
*NODE that displays the Action Group in the operation key option window
*/
class COptionControlCategoryNode : public CGuiLineTreeNode
{
public:
	COptionControlCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID);
	virtual ~COptionControlCategoryNode();

	virtual void ShowProc();				///< Override in CGuiLineTreeNode
	virtual void HideProc();				///< Override in CGuiLineTreeNode

	void OnClickBtnExpand(gui::CComponent* pComponent);		///< Function executed when the + button is clicked
	void OnClickBtnReduce(gui::CComponent* pComponent);		///< -Function executed when the button is clicked

protected:
	gui::CStaticBox* m_pStbTitle;			///< Action Group의 Title
	
	gui::CButton* m_pBtnExpand;				// + Button
	gui::CButton* m_pBtnReduce;				// - Button

	gui::CSlot m_slotClickedBtnExpand;		// + Button's signal
	gui::CSlot m_slotClickedBtnReduce;		// - Button's signal
};

/**
* \ingroup Client
*Node that displays items in the operation key option window Action Group
*/
class COptionControlActionNode : public CGuiLineTreeNode
{
public:
	COptionControlActionNode(CGuiLineTree* pMgr, std::wstring strTitle, RwUInt32 nAction, RwBool bFixed = false);
	virtual ~COptionControlActionNode();

	virtual void ShowProc();
	virtual void HideProc();

	///< Button with key name
	void OnClickBtnSetKey(gui::CComponent* pComponent);

	///< Specifies the name of the key button
	void SetText( std::wstring& strTitle );
	
	void Update();

	void RestoreBtnSetKey() { m_pBtnSetKey->Enable(); }

protected:
	gui::CStaticBox* m_pStbTitle;			///< name of action
	
	gui::CButton* m_pBtnSetKey;				///< Key assignment button
	
	gui::CSlot m_slotClickedBtnSetKey;		///< signal of key assignment button

	RwUInt32 m_nAction;						///< Contains action value.
	RwBool m_bFixed;
};

/**
* \ingroup Client
*To manage Nodes related to Windows operation keys that inherit CGuiLineTreeNode
*Manager class that inherits CGuiLineTree
*/
class COptionControlList : public CGuiLineTree
{
public:
	COptionControlList();
	virtual ~COptionControlList();

	RwBool	Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionControl* pOptionControl = NULL);

	void CreateTree();									///< Build a tree.

	///< Update the tree.
	void UpdateNode();

	///< To operate the scroll bar with a wheel mouse
	void OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	///< Calls the tab to which the current COptionControlList belongs.
	COptionControl* GetParentTab(); 

protected:
	COptionControl*	m_pOptionControl;

	gui::CSlot m_slotCaptureWheelMove;
};

/**
* \ingroup Client
*Inherit COptionBase to configure the operation key-related tab, one of the tabs in the option window.
*Can receive operation key input designation, including CGuiLineTree
*/
class COptionControl : public COptionBase
{
public:
	COptionControl();
	virtual ~COptionControl();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();
		
	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	// Information message to be printed
public:

	void SetGuideText( RwUInt32 uiColor, const WCHAR* pString );

	void SetGuideDefault();						///< Basic information
	void SetGuideChange(RwUInt32 nAction);		///< Redundant with nAction. nAction is not specified...
	void SetGuideFixed(RwUInt32 nAction);		///< %s cannot be changed.
	void SetGuideNegativeMode();				///< Changes are not possible in the current mode.
	

protected:
	COptionControlList* m_pOptionControlList;		///< Manager of Line Tree
	gui::CDialog*		m_pDlgControlList;			///< Dialog that will be the parent of the line tree

	gui::CStaticBox*	m_pStbControlTitle;			///< Title of the operation key change window
	gui::CStaticBox*	m_pStbControlBack;			///< Background of text output window in option window
	gui::CStaticBox*	m_pStbControlText;			///<Text output window in options window
};

#endif//OPTIONREFACTOR