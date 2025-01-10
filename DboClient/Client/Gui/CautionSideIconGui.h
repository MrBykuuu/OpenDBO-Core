/******************************************************************************
* File			: CautionSideIconGui.h
* Author		: Cho Haesung
* Copyright		: (주)NTL
* Date			: 2008. 4. 3
* Abstract		: 
*****************************************************************************
* Desc			:  When warning messages are accumulated and output, they are stored in CautionSideIcon.
*
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Client
#include "SideIconGui.h"
#include "windowby3.h"

// Maximum number of warning messages to be displayed
#define dMAX_CAUTION_SIDE_VIEW_UNIT			10

#define dCAUTION_UNIT_WIDTH					230

#define dCAUTION_UNIT_HEIGHT				15
#define dCAUTION_UNIT_HEIGHT_OFFSET_TOP		30
#define dCAUTION_UNIT_HEIGHT_OFFSET_BOTTOM	40

#define dCAUTION_UNIT_MARGIN_X_FROM_VIEW	10

#define dCAUTION_UNIT_TEXT_COLOR			RGB( 255, 71, 57 )

/**
* \brief Structure containing unit information held by SideView
*Used when passing information from SideView -> SideIcon.
*/
struct SCautionSideViewUnits
{
	RwInt32 nSize;
};

/**
* \ingroup client
* \brief Unit expressing one warning message
*/
class CCautionSideViewUnit
{
public:
	CCautionSideViewUnit(gui::CComponent* pParentGui, std::string& uiStringID, const WCHAR* pwcString, RwReal fLimitTime);
	virtual ~CCautionSideViewUnit();

	void			Update(RwReal fElapsed);
	RwBool			IsLive()					{ return m_bLife; }

	void			SetPositionFromParent(RwInt32 nPosX, RwInt32 nPosY);
	RwInt32			GetHeight()					{ return m_pStbSideViewUnit->GetHeight(); }

	std::string&		GetStringID()				{ return m_uiStringID; }

protected:
	gui::CStaticBox*	m_pStbSideViewUnit;

	std::string			m_uiStringID;
	RwReal				m_fCurrentTime;
	RwReal				m_fLimitTime;
	RwBool				m_bLife;
};

/**
* \ingroup client
* \brief Caution Side Icon
*Define the Side Icon and transmit the function of opening and closing the Caution Side View and the Unit to the Side View.
*/
class CCautionSideIconGui : public CSideIconBase
{
public:
	CCautionSideIconGui(const RwChar* pName);
	virtual ~CCautionSideIconGui();

	RwBool	Create();
	void	Destroy();

	virtual void	OnSideIconReceive(void* pData);
	virtual void	OnSideViewClosed();
	virtual void	OnIconButtonClicked(gui::CComponent* pComponent);

	void			OnMove(RwInt32 nOldX, RwInt32 nOldY);

protected:
	gui::CButton*	m_pIconButton;
	gui::CSlot		m_slotIconButtonClicked;

	gui::CSlot		m_slotMove;

	RwInt32			m_nSize;
};

/**
* \ingroup	client
* \brief	Caution Side View
*/
class CCautionSideViewGui : public CSideViewBase, RWS::CEventHandler
{
public:
	CCautionSideViewGui(const RwChar* pName);
	virtual ~CCautionSideViewGui();

	RwBool	Create();
	void	Destroy();
	
	void	Update(RwReal fElapsed);
	
	void	OnPaint();
	void	OnMove(RwInt32 nOldX, RwInt32 nOldY);
	void	OnCloseButtonClicked(gui::CComponent* pComponent);

	void	HandleEvents(RWS::CMsg& msg);

	virtual void	OnPressESC();
	virtual void	OnSideViewOpen(const void* pData);
	virtual void	OnSideViewReceive(void* pData);
	virtual void	OnSideViewClose();
	virtual void	OnSideViewLocate(const CRectangle& rectSideIcon);

protected:
	typedef std::list< CCautionSideViewUnit* >				ListUnits;
	typedef std::list< CCautionSideViewUnit* >::iterator	ListUnitsIt;

	ListUnits m_listUnit;

	RwBool	CreateUnit( std::string& uiStringID, const WCHAR* pString, RwReal fLifeTime );
	RwBool	DestroyUnit( CCautionSideViewUnit* pUnit );
	RwBool	DestroyUnit(std::string& uiStringID );

	void	SendUnitSizeToIcon();

protected:
	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;
	
	CWindowby3			m_winBackPanel;

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotClickedClose;

	CRectangle			m_rectIcon;
};