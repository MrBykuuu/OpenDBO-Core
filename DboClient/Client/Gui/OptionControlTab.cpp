#include "precomp_dboclient.h"
// Core
#include "NtlDebug.h"
#include "CEventHandler.h"

// Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// Framework
#include "NtlApplication.h"

// Simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"

// Dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionControlTab.h"

// Logic
#include "DboLogic.h"


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlCategoryNode

/**
* \brief Constructor for COptionControlCategoryNode
*
*Specify the ID that creates the text and Action Group to be displayed in the constructor.
*/
COptionControlCategoryNode::COptionControlCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID)
: CGuiLineTreeNode( pMgr, nID )
, m_pStbTitle( NULL )
, m_pBtnExpand( NULL )
, m_pBtnReduce( NULL )
{
	// Set the size, text, and color of the CategoryNode StaticBox.
	CRectangle rect;
	rect.SetRectWH( dOPTIONCONTROL_CATEGORY_TITLE_X, dOPTIONCONTROL_CATEGORY_TITLE_Y, 
		dOPTIONCONTROL_CATEGORY_TITLE_WIDTH, dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT );
	m_pStbTitle = NTL_NEW gui::CStaticBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pStbTitle->SetText( strTitle.c_str() );
	m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR );
	m_pStbTitle->Enable( false );

	// +button
	rect.SetRectWH(dOPTIONCONTROL_CATEGORY_BUTTON_X, dOPTIONCONTROL_CATEGORY_BUTTON_Y, dOPTIONCONTROL_CATEGORY_BUTTON_WIDTH, dOPTIONCONTROL_CATEGORY_BUTTON_HEIGHT);
	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));

	// -button
	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));

	// Button's signal connection
	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &COptionControlCategoryNode::OnClickBtnExpand);
	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &COptionControlCategoryNode::OnClickBtnReduce);
}

/**
*Destructor of \biref COptionControlCategoryNode
*/
COptionControlCategoryNode::~COptionControlCategoryNode()
{
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pBtnReduce );
	NTL_DELETE( m_pBtnExpand );
}

/**
* \brief Display CategoryNode
*
*Override ShowProc() of CGuiLineTreeNode to recalculate relationships with child nodes as nodes expand.
*Arranges and outputs the current button status as + or -.
*/
void COptionControlCategoryNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pStbTitle->Show(true);
	if (IsExpand())
	{
		m_pBtnReduce->Show(true);		
		m_pBtnExpand->Show(false);
		m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR_DOWN, TRUE );
	}
	else
	{
		m_pBtnExpand->Show(true);
		m_pBtnReduce->Show(false);
		m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR, TRUE );
	}

	// Recalculate the positions of category nodes
	m_pBtnExpand->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCONTROL_CATEGORY_BUTTON_Y);
	m_pBtnReduce->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCONTROL_CATEGORY_BUTTON_Y);
	m_pStbTitle->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_TITLE_X, m_nPosY + dOPTIONCONTROL_CATEGORY_TITLE_Y);
}

/**
* \brief Hide CategoryNode
*/
void COptionControlCategoryNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pStbTitle->Show(false);
	m_pBtnExpand->Show(false);
	m_pBtnReduce->Show(false);
}

/**
* \brief Action when +button is pressed
*/
void COptionControlCategoryNode::OnClickBtnExpand(gui::CComponent* pComponent)
{
	Expand(TRUE);
}

/**
* \brief -Action when button is pressed
*/
void COptionControlCategoryNode::OnClickBtnReduce(gui::CComponent* pComponent)
{
	Expand(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlActionNode

/**
* \brief Constructor for COptionControlActionNode
*
*Creates the Node that will be included as a child of COptionControlCategoryNode, receiving the str of the action and the ID of the Action.
*/
COptionControlActionNode::COptionControlActionNode(CGuiLineTree* pMgr, std::wstring wstrTitle, RwUInt32 nAction, RwBool bFixed)
: CGuiLineTreeNode( pMgr, nAction )
, m_pStbTitle( NULL )
, m_pBtnSetKey( NULL )
{
	// Save initialization value
	m_nAction = nAction;
	m_bFixed = bFixed;

	// Set the Size, Text, and Color of the ActionNode StaticBox.
	CRectangle rect;
	rect.SetRectWH( dOPTIONCONTROL_ACTIONNODE_TITLE_X, dOPTIONCONTROL_ACTIONNODE_TITLE_Y, 
		dOPTIONCONTROL_ACTIONNODE_TITLE_WIDTH, dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT );

	m_pStbTitle = NTL_NEW gui::CStaticBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pStbTitle->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfStaticBGLarge"));
	m_pStbTitle->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbTitle->SetTextColor( dOPTIONCONTROL_ACTIONNODE_TITLE_COLOR );
	m_pStbTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pStbTitle->SetText( wstrTitle.c_str() );
	
	// Key designation button (must be marked according to Action Id)
	// Temporarily use resources of the same size
	rect.SetRectWH( dOPTIONCONTROL_ACTIONNODE_BUTTON_X, dOPTIONCONTROL_ACTIONNODE_BUTTON_Y,
		dOPTIONCONTROL_ACTIONNODE_BUTTON_WIDTH, dOPTIONCONTROL_ACTIONNODE_BUTTON_HEIGHT );
	m_pBtnSetKey = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnSetKey->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfStaticBGSmall"));
	m_pBtnSetKey->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallFocus"));
	m_pBtnSetKey->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallDown"));
	m_pBtnSetKey->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallDisable"));
	m_pBtnSetKey->SetTextStyle(COMP_TEXT_CENTER);
	m_pBtnSetKey->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	// It outputs a string according to the action ID.
	std::wstring wstrKeyName = GetInputActionMap()->GetKeyName( nAction );

	// If it is empty, ¡°Not Specified¡± is output. Otherwise, the name of the key is output as is.
	if( wstrKeyName.empty() )
		m_pBtnSetKey->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_EMPTYKEY" ) );
	else
		m_pBtnSetKey->SetText( wstrKeyName.c_str() );

	// Button's signal connection
	m_slotClickedBtnSetKey = m_pBtnSetKey->SigClicked().Connect(this, &COptionControlActionNode::OnClickBtnSetKey);
}

/**
*Destructor of \biref COptionControlActionNode
*/
COptionControlActionNode::~COptionControlActionNode()
{
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pBtnSetKey );
}

/**
* \brief Show COptionControlActionNode
*/
void COptionControlActionNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pStbTitle->Show(true);
	m_pBtnSetKey->Show(true);
	
	m_pBtnSetKey->SetPosition(m_nPosX + dOPTIONCONTROL_ACTIONNODE_BUTTON_X, m_nPosY + dOPTIONCONTROL_ACTIONNODE_BUTTON_Y);
	m_pStbTitle->SetPosition(m_nPosX + dOPTIONCONTROL_ACTIONNODE_TITLE_X, m_nPosY + dOPTIONCONTROL_ACTIONNODE_TITLE_Y);
}

/**
* \brief Hide COptionControlActionNode
*/
void COptionControlActionNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pStbTitle->Show(false);
	m_pBtnSetKey->Show(false);

	if( GetInputActionMap()->GetInputMode() == (RwUInt32)m_nID )
	{
		GetInputActionMap()->InitInputMode();
		m_pBtnSetKey->Enable( true );
		COptionControlList* pMgr = static_cast<COptionControlList *>(m_pMgr);

		pMgr->GetParentTab()->SetGuideDefault();
	}
}

/**
* \brief What happens when you press the key assignment button
*/
void COptionControlActionNode::OnClickBtnSetKey(gui::CComponent* pComponent)
{
	COptionControlList* pMgr = static_cast<COptionControlList *>(m_pMgr);

	// When InputActionMap is an immutable mdoe
	if( GetInputActionMap()->GetActionMapMode() == ACTIONMAP_MODE_DEFAULT )
	{
		pMgr->GetParentTab()->SetGuideNegativeMode();
		return;
	}

	// If change is not possible, you must inform them that change is not possible.
	if( m_bFixed )
	{
		pMgr->GetParentTab()->SetGuideFixed( m_nID );
	}
	else
	{
		pMgr->GetParentTab()->SetGuideText(dOPTIONCONTROL_STATICBOX_COLOR, GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_READYGUIDE") );
		pMgr->UpdateNode();
		GetInputActionMap()->SetInputMode( m_nAction );
	
		// Disable the button.
		m_pBtnSetKey->Enable(false);
	}
}

void COptionControlActionNode::SetText( std::wstring& strTitle ) 
{
	m_pBtnSetKey->SetText( strTitle );
}

void COptionControlActionNode::Update() 
{
	m_pBtnSetKey->Enable( true );

	// It outputs a string according to the action ID.
	std::wstring wstrKeyName = GetInputActionMap()->GetKeyName( m_nID );

	// If it is empty, ¡°Not Specified¡± is output. Otherwise, the name of the key is output as is.
	if( wstrKeyName.empty() )
		m_pBtnSetKey->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_EMPTYKEY" ) );
	else
		m_pBtnSetKey->SetText( wstrKeyName.c_str() );
}


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlCategoryNode

/**
* \brief Constructor for COptionControlList
*/
COptionControlList::COptionControlList()
{
}

/**
* \brief Destructor of COptionControlList
*/
COptionControlList::~COptionControlList()
{
}

/**
* \brief Function to create CGuiLineTree in COptionControlList
*
*Create a LineTree and set the scrollbar properties.
*
* \param rect (CRectangle&)
*/
RwBool COptionControlList::Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionControl* pOptionControl)
{
	if (!CGuiLineTree::Create(rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth))
		return FALSE;

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->SetButtonSize(dOPTIONCONTROL_LIST_SCROLL_WIDTH, dOPTIONCONTROL_LIST_SCROLL_HEIGHT);
	m_pScroll->SetSliderSize(dOPTIONCONTROL_LIST_SLIDER_WIDTH, dOPTIONCONTROL_LIST_SLIDER_HEIGHT);

	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &COptionControlList::OnCaptureWheelMove );

	m_pOptionControl = pOptionControl;

	return TRUE;
}

/**
* \brief Create items in OptionControl
*/
void COptionControlList::CreateTree() 
{
	//////////////////////////////////////////////////////////////////////////////
	// Avatar related categories
	CGuiLineTreeNode* pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_AVATAR" ) ,	// Category Title
		dOPTIONCONTROL_CATEGORY_AVATAR );												// Category ID
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );							// CGuiLineTree has root as parent
	pNode->Expand( true );

	// The order of enumerations must be maintained. If things aren't in order, things get messed up.
	for(RwUInt32 nActionID = ACTION_AVATAR_FORWARD; nActionID <= ACTION_AVATAR_RIGHTSIDE; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),								// Action title
			nActionID );															// Action ID
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_AVATAR );							// It has a category as its parent.
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Target related categories
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_TARGET") , 
		dOPTIONCONTROL_CATEGORY_TARGET );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	for(RwUInt32 nActionID = ACTION_TARGET_SELF; nActionID <= ACTION_TARGET_4THPARTY; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );
	}

	pNode = NTL_NEW COptionControlActionNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_TARGET_AUTOPARTY" ),
		ACTION_TARGET_AUTOTARGET_PARTY );
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );

	for( RwUInt32 nActionID = ACTION_TARGET_AUTO; nActionID <= ACTION_TARGET_SELECTING_5; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );
	}

	//////////////////////////////////////////////////////////////////////////////
	// Quick Slot Related Categories
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_QUICKSLOT") , 
		dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	for(RwUInt32 nActionID = ACTION_QUICK_1; nActionID <= ACTION_QUICK_PLUS; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	//////////////////////////////////////////////////////////////////////////
	// expansion
	// Reserve empty seats from 80 to 159
	for(RwUInt32 nActionID = ACTION_QUICK_1_EX; nActionID <= ACTION_QUICK_PLUS_EX2; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	// Quick slot operation related
	for(RwUInt32 nActionID = ACTION_QUICK_PREV; nActionID <= ACTION_QUICK_5THCAP; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	//////////////////////////////////////////////////////////////////////////////
	// Windows related categories
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_WINDOW") , 
		dOPTIONCONTROL_CATEGORY_WINDOW );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

		for(RwUInt32 nActionID = ACTION_WINDOW_PROFILE; nActionID <= ACTION_WINDOW_RANKBOARD; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum( nActionID ),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_WINDOW );
	}

	//////////////////////////////////////////////////////////////////////////////
	// Fixed Category
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_FIXED") , 
		dOPTIONCONTROL_CATEGORY_FIXED );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	// Zoom out minimap
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMOUT" ),
		ACTION_MINIMAP_ZOOMOUT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// Zoom in on the minimap
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMIN" ),
		ACTION_MINIMAP_ZOOMIN , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// Chat window previous page
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_CHAT_PGUP" ),
		ACTION_CHAT_PGUP , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// Chat window next page
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_CHAT_PGDN" ),
		ACTION_CHAT_PGDN , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// cancellation
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_CANCLE" ),
		ACTION_GLOBAL_CANCLE , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// screenshot
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_SNAPSHOT" ),
		ACTION_GLOBAL_SNAPSHOT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// Chat input
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_CHAT" ),
		ACTION_GLOBAL_CHAT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );


//		ACTION_DEVUSER_ONLYRENDERGUI = 200, ///< Development mode (only draws GUI. Temporary)
}

/**
* \brief Updates each node.
*
*Calls the Update() function of COptionControlActionNodes.
*/
void COptionControlList::UpdateNode() 
{
	for(int i=ACTION_AVATAR_FORWARD; i != ACTION_NUMS; ++i)
	{
		COptionControlActionNode* pActionNode = 
			static_cast<COptionControlActionNode*>(FindNode( i ));

		if( pActionNode )
			pActionNode->Update();
	}
	
}

void COptionControlList::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos ) 
{
	if( !GetParentGui()->IsVisible() )
		return;
	
	if( GetParentGui()->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 nValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetValue();
	RwInt32 nMaxValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetMaxValue();
	RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->SetValue( nDelta );	
}

COptionControl* COptionControlList::GetParentTab() 
{
	if( m_pOptionControl ) 
		return m_pOptionControl; 
	else 
		return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
// class : COptionControl

COptionControl::COptionControl()
{
	m_pOptionControlList = NULL;	
	m_pDlgControlList = NULL;	
	m_pStbControlTitle = NULL;
	m_pStbControlBack = NULL;
	m_pStbControlText = NULL;	
}

COptionControl::~COptionControl()
{
}

RwBool COptionControl::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionControl::Create");
	COptionBase::Create(pOptionWindow);

	// Operation control title
	m_pStbControlTitle = (gui::CStaticBox*)GetComponent("stbControlTitle");
	m_pStbControlTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_TITLE" ) );

	// Virtual Dialog for using GUI LineTree
	m_pDlgControlList = (gui::CDialog*)GetComponent("dlgControlList");

	// Set the size of the Gui Line Tree to rect.
	CRectangle rect;
	rect.SetRectWH(dOPTIONCONTROL_LIST_X, dOPTIONCONTROL_LIST_Y, 
		dOPTIONCONTROL_LIST_WIDTH, dOPTIONCONTROL_LIST_HEIGHT);

	// Create a GuiLineTree.
	m_pOptionControlList = NTL_NEW COptionControlList;
	if (!m_pOptionControlList->Create(rect, m_pDlgControlList, 
		dOPTIONCONTROL_LINE_HEIGHT,			// height of each line
		dOPTIONCONTROL_LINE_MARGIN,			// spacing of each line
		dOPTIONCONTROL_CHILD_MARGIN_WIDTH,	// spacing between children
		dOPTIONCONTROL_LIST_SLIDER_WIDTH, this))	// width of the slider
		return FALSE;

	// Create items that can be assigned operation keys
	m_pOptionControlList->CreateTree();

	// Create TextBox
	m_pStbControlBack = (gui::CStaticBox*)GetComponent("stbControlBack");
	m_pStbControlText = (gui::CStaticBox*)GetComponent("stbControlText");

	NTL_RETURN(TRUE);
}

void COptionControl::Destroy()
{
	NTL_FUNCTION("COptionControl::Destroy");

	if (m_pOptionControlList)
	{
		m_pOptionControlList->Destroy();
		NTL_DELETE(m_pOptionControlList);
	}

	NTL_RETURNVOID();
}

void COptionControl::Show()
{
	// Do not use ScrollBar of COptionBase.
	m_pScrollBar->Show(false);

	m_pStbControlTitle->Show(true);
	m_pDlgControlList->Show(true);
	m_pStbControlBack->Show(true);
	
	// Basic guidance message settings
	SetGuideDefault(); 
	m_pStbControlText->Show(true);

	COptionBase::Show();
}

void COptionControl::Hide()
{	
	// Using ScrollBar from COptionBase
	m_pScrollBar->Show(true);

	m_pDlgControlList->Show(false);
	m_pStbControlTitle->Show(false);
	m_pStbControlBack->Show(false);
	m_pStbControlText->Show(false);

	GetInputActionMap()->InitInputMode();	   ///< Disable action map input mode
	//GetInputActionMap()->CancleActionMap();	   ///< Load temporarily saved action map
	
	// update of node
	m_pOptionControlList->UpdateNode();
		
	COptionBase::Hide();
}

void COptionControl::OnInit()
{
}

void COptionControl::OnReset()
{
	// Cancels previously changed actions.
	GetInputActionMap()->InitInputMode();
	GetInputActionMap()->InitDefaultActionMap();

	m_pOptionControlList->UpdateNode();
	SetGuideDefault();
}

void COptionControl::OnOk()
{
	GetInputActionMap()->InitInputMode();
	
	// If there are changes and they are applied, update the Node.
	if( GetInputActionMap()->ApplyActionMap() )
	{
		m_pOptionControlList->UpdateNode();
		SetGuideDefault();
	}
	else
		SetGuideText(dOPTIONCONTROL_STATICBOX_WARNING_COLOR, L"DST_OPTION_CONTROL_TEXT_WAITGUIDE" );
}

void COptionControl::OnCancel()
{
	GetInputActionMap()->CancleActionMap();
}

/**
*\letter OnHandleEvents
*/
void COptionControl::OnHandleEvents( RWS::CMsg &pMsg ) 
{
	// Processes information sent from Action Map.
	if( pMsg.Id == g_EventActionMapClientNotify )
	{
		SNtlEventActionMapClientNotify* pData = reinterpret_cast<SNtlEventActionMapClientNotify*>( pMsg.pData );

		m_pOptionControlList->UpdateNode();

		switch( pData->byType )
		{
		case SNtlEventActionMapClientNotify::ACTIONMAP_OK:							// Notification that it has been applied to the server
			SetGuideDefault();
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_RELEASE:							// Notification that the key has changed
			SetGuideChange( pData->wParam1 );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_FIXEDKEY:						// Notify that the key is a fixed key
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_FIXEDKEYGUIDE" );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_NOTCOMBINE:						// Notice that it cannot be specified using a combination key.
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_NOTCOMBINEGUIDE" );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_SAMEKEY:
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_SAMEKEYGUIDE" );
			break;
		default:
			break;
		}
	}
}

void COptionControl::SetGuideText( RwUInt32 uiColor, const WCHAR* pString ) 
{
	m_pStbControlText->SetTextColor( uiColor );
	m_pStbControlText->SetText( pString );
}

/**
* \brief Guide to changing basic shortcut keys
*/
void COptionControl::SetGuideDefault() 
{
	WCHAR awcTemp[512];
	swprintf_s(awcTemp, 512, GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_GUIDE"),
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_EMPTYKEY"));
	
	SetGuideText( dOPTIONCONTROL_STATICBOX_COLOR, awcTemp );
}

/**
* \brief Information on keys that have been released due to duplicate keys not being specified.
*/
void COptionControl::SetGuideChange(RwUInt32 nAction) 
{
	WCHAR awcTemp[512];
	swprintf_s(awcTemp, 512, 
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_CHANGEGUIDE"),
		GetActionDisplayStringNum(nAction),
		GetActionDisplayStringNum(nAction),
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_EMPTYKEY"));

	SetGuideText( dOPTIONCONTROL_STATICBOX_WARNING_COLOR, awcTemp );
}

/**
* \brief Notice that this is an immutable key.
*/
void COptionControl::SetGuideFixed( RwUInt32 nAction ) 
{
	WCHAR awcTemp[512];
	swprintf_s( awcTemp, 512, GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_NOTCHANGEGUIDE"),
		GetActionDisplayStringNum( nAction ) );
	
	SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, awcTemp );
}

/**
* \brief Notice that changes are not possible in the current mode.
*/
void COptionControl::SetGuideNegativeMode()
{
	SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, 
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_NEGATIVEMODEGUIDE") );
}

