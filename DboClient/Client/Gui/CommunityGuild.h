/******************************************************************************
*File: CommunityGuild.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : April 19, 2007
*Abstract		: 
*****************************************************************************
*Desc: Guild Community Window
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlSharedType.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "CommunityBase.h"
#include "SurfaceGui.h"
#include "Windowby3.h"
#include "RwTextureGui.h"

struct sGuildMember;
class CGuildEmblemMakerGui;
class CGuildNoticeGui;
class CNtlGuild;

class CGuildMemberGui
{
public:
	CGuildMemberGui(CNtlPLGui* pParent);
	virtual ~CGuildMemberGui();

	VOID	Destory();
	VOID	Clear();

	VOID	SetPosition(RwInt32 iParentiX, RwInt32 iParentiY, RwInt32 iX, RwInt32 iY);
	VOID	GetPosition(CRectangle& rect);

	VOID	SetMember(sGuildMember* pGuildMember);

	CHARACTERID GetCharID();

	VOID	Show(bool bShow);
	RwBool	PtInRect(RwInt32 iXPos, RwInt32 iYPos);

protected:
	VOID	SetMasterColor();
	VOID	SetSecondMasterColor();
	VOID	SetNormalColor();
	VOID	SetOffLineColor();

	VOID	ClickedMemberButton(gui::CComponent* pComponent);

public:
	gui::CSlot			m_slotMemberMenuButton;

	CNtlPLGui*			m_pParent;

	gui::CPanel*		pTributarySimbol;///< Illustration of races and occupations
	gui::CStaticBox*	m_pName;
	gui::CStaticBox*	m_pLevel;
	gui::CStaticBox*	m_pArea;
	gui::CButton*		m_pMemberMenuButton;

	CRectangle			m_rtPos;

	CHARACTERID			m_charID;
};

#define dVISIBLE_LIST_COUNT					8

class CCommunityGuild : public CCommunityBase
{
public:
	CCommunityGuild();
	virtual ~CCommunityGuild();

	virtual RwBool		Create(CNtlPLGui* pParent);
	virtual VOID		Destroy();

	virtual VOID		SwitchDialog(bool bOpen);

	virtual VOID		Clear();	
	virtual VOID		ActiveTab(bool bActive);

	virtual VOID		HandleEvents( RWS::CMsg &msg );	

	virtual VOID		OnMouseDown(const CKey& key);
	virtual VOID		OnMouseUp(const CKey& key);
	virtual VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent);
	VOID				OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID				OnCaptureMouseDown(const CKey& key);

	virtual VOID		OnPaint();

	SERIAL_HANDLE		GetSeletedID();

protected:
	VOID		GuildLeave();

	VOID		AddMember(CHARACTERID charID);
	VOID		DelMember(CHARACTERID charID);

	VOID		SetOnlineMemberCount(CNtlGuild* pGuild);
	VOID		RefreshList(RwInt32 iOffSet);

	VOID		EmblemButton_Change();
	VOID		EmblemButton_Reset();

	VOID		ClearEmblemMaker();
	VOID		ClearNoticeGui();
	VOID		CheckInfoWindow();

	// Signals
	VOID		OnScrollChanged(RwInt32 iOffset);

	VOID		ClickedEmblemButton(gui::CComponent* pComponent);
	VOID		ClickedLeaveButton(gui::CComponent* pComponent);
	VOID		ClickedGhymButton(gui::CComponent* pComponent);

	VOID		ClickedNameButton(gui::CComponent* pComponent);
	VOID		ClickedLevelButton(gui::CComponent* pComponent);
	VOID		ClickedAreaButton(gui::CComponent* pComponent);

	VOID		ClickedNoticeButton(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotEmblemButton;
	gui::CSlot			m_slotEmblemModifyButton;
	gui::CSlot			m_slotLeaveButton;
	gui::CSlot			m_slotGhymButton;
	gui::CSlot			m_slotNameButton;
	gui::CSlot			m_slotLevelButton;
	gui::CSlot			m_slotAreaButton;	
	gui::CSlot			m_slotNoticeButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;

	CNtlPLGui*			m_pParent;					///< Owner class	

	gui::CButton*		m_pEmblemButton;			///< Guild emblem button
	gui::CButton*		m_pEmblemModifyButton;		///< Guild crest edit button
	gui::CButton*		m_pLeaveButton;				///< Guild withdrawal button
	gui::CButton*		m_pGhymButton;				///< Guild seal button
	gui::CButton*		m_pNameButton;				///< Name sort button
	gui::CButton*		m_pLevelButton;				///< Level sort button
	gui::CButton*		m_pAreaButton;				///< Region sort button
	gui::CButton*		m_pNoticeButton;			///< Notice button

	gui::CStaticBox*	m_pGuildName;				///< Guild name

	gui::CStaticBox*	m_pCurGuildPointText;		///< Current guild points
	gui::CStaticBox*	m_pMaxGuildPointText;		///< Max guild points

	gui::CStaticBox*	m_pCurGuildPoint;			///< Current guild points
	gui::CStaticBox*	m_pMaxGuildPoint;			///< Max guild points


	gui::CStaticBox*	m_pGuildNum;				///< Guild members
	gui::CStaticBox*	m_pGuildNoticeStatic;		///< ¡®Guild Notice¡¯
	gui::CStaticBox*	m_pGuildNoticeTime;			///< Notice date
	gui::CStaticBox*	m_pGuildNoticer;			///< Announcer
	gui::COutputBox*	m_pNotice;					///<Notice contents

	gui::CScrollBar*	m_pScrollbar;

	CRwTextureGui*		m_pEmblemButtonImage;

	CWindowby3			m_GuildMemBackSurface;		///< Guild member background
	CWindowby3			m_NoticeSurface;			///< Guild notice background

	CSurfaceGui			m_srfHumanShapeSurface;		///< human shape
	CSurfaceGui			m_srfNoticeBar;				///< Guild Notice Bar	

	RwBool				m_bNameAscendingSort;		///< Whether the names are in ascending order
	RwBool				m_bLevelAscendingSort;		///< Whether the level is in ascending order
	RwBool				m_bAreaAscendingSort;		///< Is the region in ascending order?	

	RwInt32				m_iOnlineMember;			///< Number of guild members currently online

	CHARACTERID			m_SelectedCharID;			///< Currently selected guild member ID

	RwInt32				m_iVisibleStart;			///< The first index of MemGUI shown on the screen
	RwInt8				m_byInfoMemberIndex;	

	CGuildMemberGui*	m_aGuildMemberGui[dVISIBLE_LIST_COUNT];

	CGuildEmblemMakerGui*	m_pEmblemMakerGui;
	CGuildNoticeGui*		m_pNoticeGui;
};