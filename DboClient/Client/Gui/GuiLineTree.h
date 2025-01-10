////////////////////////////////////////////////////////////////////////////////
// File: GuiLineTree.h
// Desc: GuiLineTree -Module to be used in QuestList, Quest Window, etc.
//		 Only the tree structure and LineID are in charge, and coordinates, rendering, and interactivity are implemented through inheritance.
//
// 2006.10.16 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_LINE_TREE_H__
#define __GUI_LINE_TREE_H__

#include "NtlPLGui.h"

class CGuiLineTree;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CGuiLineTreeNode
{
public:
	//! Typedef
	typedef std::list<CGuiLineTreeNode*>	LIST_LINENODE;

	CGuiLineTreeNode( CGuiLineTree* pMgr, RwInt32 nID );
	virtual ~CGuiLineTreeNode(VOID);

	//! Attribute
	CGuiLineTreeNode*	GetParentNode(VOID);
	RwInt32				GetID(VOID);
	RwInt32				GetPosX(VOID);
	RwBool				IsExpand(VOID);
	RwBool				IsRootNode(VOID);
	RwBool				IsChildExist(VOID);
	RwBool				IsVisible(VOID);

	VOID				SetParentNode( CGuiLineTreeNode* pParentNode );
	VOID				SetID( RwInt32 nID );
	VOID				SetPosX( RwInt32 nPosX );
	
	//! Operation
	VOID AddChildNode( CGuiLineTreeNode* pNode );		
	VOID DeleteChildNode( RwInt32 nID );	
	VOID ClearChildNodes(VOID);

	CGuiLineTreeNode*	FindChildNode( RwInt32 nID );
	LIST_LINENODE*		GetChildNodeAll(VOID) { return &m_listpChild; }
	
	RwInt32	RecursiveSetLineID( RwInt32 nLineID );
	VOID	RecursiveChildHide(VOID);
		
	//! Virtual
	virtual VOID		Expand( RwBool bExpand );
	virtual VOID		ExpandChild( RwBool bExpand );
	virtual VOID		ShowProc(VOID);		// Overrides Show, coordinate designation, etc. of included components.
	virtual VOID		HideProc(VOID);		// Override with Hide, etc. of the included component.			

protected:
	CGuiLineTree*		m_pMgr;		
	CGuiLineTreeNode*	m_pParent;
	LIST_LINENODE		m_listpChild;
	RwInt32				m_nID;			// unique ID
	RwInt32				m_nLineID;		// line id
	RwInt32				m_nPosX;		// starting point
	RwInt32				m_nPosY;		
	RwBool				m_bIsExpand;	
	RwBool				m_bShow;		// Is this node a visible node?
};

inline CGuiLineTreeNode* CGuiLineTreeNode::GetParentNode(VOID)
{
	return m_pParent;
}

inline RwInt32 CGuiLineTreeNode::GetID(VOID)
{
	return m_nID;
}

inline RwInt32 CGuiLineTreeNode::GetPosX(VOID)
{
	return m_nPosX;
}

inline RwBool CGuiLineTreeNode::IsExpand(VOID)
{
	return m_bIsExpand;
}

inline RwBool CGuiLineTreeNode::IsRootNode(VOID)
{
	return ( m_pParent ) ? FALSE : TRUE ;
}

inline RwBool CGuiLineTreeNode::IsChildExist(VOID)
{
	return ( m_listpChild.empty() ) ? FALSE : TRUE ;
}

inline RwBool CGuiLineTreeNode::IsVisible(VOID)
{
	return m_bShow;
}

inline VOID CGuiLineTreeNode::SetParentNode( CGuiLineTreeNode* pParentNode )
{
	m_pParent = pParentNode;
}

inline VOID CGuiLineTreeNode::SetID( RwInt32 nID )
{
	m_nID = nID;
}

inline VOID CGuiLineTreeNode::SetPosX( RwInt32 nPosX )
{
	m_nPosX = nPosX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GUILINETREE_ROOTNODE_ID				-1
#define GUILINETREE_DEFAULT_HEIGHT			20
#define GUILINETREE_DEFAULT_MARGIN			3
#define GUILINETREE_DEFAULT_CHILDMARGINX	10
#define GUILINETREE_DEFAULT_SCROLLBARWIDTH	16

class CGuiLineTree
{
public:
	CGuiLineTree(VOID);
	virtual ~CGuiLineTree(VOID);

	//! Operation
	virtual RwBool	Create( CRectangle& rect, gui::CComponent* pParent, 
							RwInt32 nLineHeight = GUILINETREE_DEFAULT_HEIGHT,
							RwInt32 nLineMargin = GUILINETREE_DEFAULT_MARGIN,
							RwInt32 nChildXMargin = GUILINETREE_DEFAULT_CHILDMARGINX,
							RwInt32 nScrollBarWidth = GUILINETREE_DEFAULT_SCROLLBARWIDTH );	// Override when adding pictures, etc. 		
	virtual	VOID	Destroy(VOID);		
	
	virtual VOID	AddNode( CGuiLineTreeNode* pNode, RwInt32 nParentID = GUILINETREE_ROOTNODE_ID ); // If there is a standard to apply when adding or deleting, you must override or overload.
	virtual VOID	DeleteNode( RwInt32 nID );
	virtual VOID	ClearNodes(VOID);			// The root node is not deleted.

	CGuiLineTreeNode*	FindNode( RwInt32 nID );
	VOID				RecalcLineID(VOID);

	//! Attribute
	CGuiLineTreeNode*	GetRootNode(VOID);
	RwInt32				GetMaxLine(VOID);
	RwInt32				GetLineHeight(VOID);
	RwInt32				GetLineMargin(VOID);
	RwInt32				GetScrollOffset(VOID);
	RwInt32				GetChildXMargin(VOID);
	gui::CComponent*	GetParentGui(VOID);
	gui::CScrollBar*	GetScrollBar(VOID);

protected:
	// CallBack
	VOID	OnScrollChanged( RwInt32 nNewOffset );
	
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;

	// Variable
	RwInt32				m_nLineHeight;	// Line width.
	RwInt32				m_nLineMargin;	// Margin between lines.
	RwInt32				m_nScrollOffset;// Scroll offset.
	RwInt32				m_nMaxLine;		// Maximum number of lines.
	RwInt32				m_nChildXMargin;// The indentation value of the child node.
	gui::CScrollBar*	m_pScroll;		// scroll bar
	gui::CDialog*		m_pLineTree;	// Line Tree Dialog

	CGuiLineTreeNode*	m_pRootNode;		
};

inline CGuiLineTreeNode* CGuiLineTree::GetRootNode(VOID)
{
	return m_pRootNode;
}

inline RwInt32 CGuiLineTree::GetMaxLine(VOID)
{
	return m_nMaxLine;
}

inline RwInt32 CGuiLineTree::GetLineHeight(VOID)
{
	return m_nLineHeight;
}

inline RwInt32 CGuiLineTree::GetLineMargin(VOID)
{
	return m_nLineMargin;
}

inline RwInt32 CGuiLineTree::GetScrollOffset(VOID)
{
	return m_nScrollOffset;
}

inline RwInt32 CGuiLineTree::GetChildXMargin(VOID)
{
	return m_nChildXMargin;
}

inline gui::CComponent* CGuiLineTree::GetParentGui(VOID)
{
	return m_pLineTree;
}

inline gui::CScrollBar* CGuiLineTree::GetScrollBar(VOID)
{
	return m_pScroll;
}

#endif//__GUI_LINE_TREE_H__