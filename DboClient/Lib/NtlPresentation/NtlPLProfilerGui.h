/******************************************************************************
* File			: NtlPLProfilerGui.h
* Author		: Cho Haesung
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 6. 19
* Abstract		: 
*****************************************************************************
* Desc			: Outputs profiling data.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

#define dPROFILER_GUI_DEFAULT_COLOR		RGB(255, 255, 255)
#define dPROFILER_GUI_HIGHLIGHT_COLOR	RGB(255, 0, 0)
#define dPROFILER_GUI_FOCUS_COLOR		RGB(255, 210, 70)

#define dPROFILER_GUI_WINDOW_WIDTH		640
#define dPROFILER_GUI_WINDOW_HEIGHT		240

#define dPROFILER_GUI_ITEM_WIDTH		620
#define dPROFILER_GUI_ITEM_HEIGHT		20

#define dPROFILER_GUI_UPDATE_TIME		1.0f

#define dPROFILER_GUI_PARENT_NAME_BUFFER	128
#define dPROFILER_GUI_NAME_BUFFER			128

#define dPROFILER_GUI_CURRENT_VIEW_NUMS		9	// Number of items that can be displayed at once

/**
* \brief Structure to output profiler node information
*/
struct SNtlProfileNode
{
	RwUInt32	uiIndex;									///< Index
	RwUInt8		byHighLightField;
	RwChar		acNodeName[dPROFILER_GUI_NAME_BUFFER];		///< Name of Node
	RwReal		fPercentByParent;							///< % from parents
	RwReal		fMSecPerFrame;								///< ms /f
	RwReal		fMSecPerCall;								///< ms /c
	RwReal		fCallPerFrame;								///< calls /f
};

enum eNtlProfileField
{
	FIELD_NODENAME = 0,
	FIELD_PER_BY_PARENT,
	FIELD_MSEC_PER_FRAME,
	FIELD_MSEC_PER_CALL,
	FIELD_CALL_PER_FRAME,

	FILED_NUMS,
	FIELD_INVALID = 0xff,
};

/**
* \ingroup presentation
* \brief Item that displays information about a node in the profiler GUI
*/
class CNtlPLProfilerItem
{
public:
	CNtlPLProfilerItem();
	virtual ~CNtlPLProfilerItem();

	VOID	Create(gui::CComponent* pParentComp, CRectangle& rect );
	VOID	Destroy();
				
	VOID	FocusItem( RwBool bFocus = TRUE );

	CRectangle	GetScreenRect() { return m_pThis->GetScreenRect(); }

	VOID	SetProfileData( SNtlProfileNode& sNode, RwBool bLogged = TRUE );
	VOID	SetSurface( gui::CSurface& surface, RwInt32 nWidth, RwInt32 nHeight,
		RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha );

protected:
	gui::CDialog*		m_pThis;
	
	gui::CStaticBox*	m_pStbItemName;
	gui::CStaticBox*	m_pStbPercentByParent;
	gui::CStaticBox*	m_pStbPercentByTotal;

	gui::CStaticBox*	m_pStbMSecPerFrame;
	gui::CStaticBox*	m_pStbMSecPerCall;
	gui::CStaticBox*	m_pStbCallPerFrame;
};

class CProfileIterator;

/**
* \ingroup presentation
* \brief Profiler GUI (obtains information from Core's NtlProfiler and outputs it)
*/
class CNtlPLProfilerGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CNtlPLProfilerGui( const RwChar* pName );
	virtual ~CNtlPLProfilerGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		HandleEvents( RWS::CMsg& msg );

	VOID		ShowDialog(RwBool bShow = TRUE);

	RwBool		EnterChild( RwInt32 nIdx );
	RwBool		EnterParent();

	VOID		OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );
	VOID		OnMouseDown( const CKey& key );
	VOID		OnMouseUp( const CKey& key );

	VOID		GetCurrentProfileData();
	VOID		CreateProfilerItems(RwUInt32 uiItemsCount);
	VOID		DestroyProfilerItems();
	VOID		UpdateData();
	VOID		UpdatePanel();
	VOID		SetSurface( gui::CSurface& surface, RwInt32 nWidth, RwInt32 nHeight,
		RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha );

	VOID		AdjustPanelToItems();

	VOID		OnClickedBtnUpperExplore( gui::CComponent* pComponent );
	VOID		OnClickedBtnReset( gui::CComponent* pComponent );
	VOID		OnClickedBtnClose( gui::CComponent* pComponent );
	VOID		OnScrollChanged( RwInt32 nNewOffset );

protected:
	// Dialog
	gui::CStaticBox*	m_pStbTitle;
	
	// menu
	gui::CStaticBox*	m_pStbNodeName;
	gui::CStaticBox*	m_pStbParent;
	gui::CStaticBox*	m_pStbMSecPerFrame;
	gui::CStaticBox*	m_pStbMSecPerCall;
	gui::CStaticBox*	m_pStbCallPerFrame;

	// Button
	gui::CButton*		m_pBtnUpperExplore;
	gui::CButton*		m_pBtnReset;
	gui::CButton*		m_pBtnClose;

	// Data Field
	gui::CDialog*		m_pDlgDataField;
	gui::CScrollBar*	m_pScrollBar;
	
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;

	gui::CSlot			m_slotClickedBtnUpperExplore;
	gui::CSlot			m_slotClickedBtnReset;
	gui::CSlot			m_slotClickedBtnClose;

	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;

	// Resource
	gui::CSurface		m_surPanel;
	gui::CSurface		m_surBar;
	gui::CSurface		m_surAnchor;

	// Data
	SNtlProfileNode*	m_pProfileNode;
	CNtlPLProfilerItem*	m_pProfileItem;
	
	RwChar				m_acParentName[dPROFILER_GUI_PARENT_NAME_BUFFER];
	RwReal				m_fElaspedTime;
	RwUInt32			m_uiSibblingCount;
	
	CProfileIterator*	m_pCurrentIter;
	RwUInt32			m_byCurrentFocusItem;
	RwBool				m_bScroll;
};