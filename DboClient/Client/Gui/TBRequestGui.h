/*****************************************************************************
*File			: TBRequestGui.h
*Author		    : Haesung, Cho
*Copyright		:NTL Co., Ltd.
*Date			: 2008. 10. 22
*Abstract		: DBO World's Best Martial Arts Newsletter Renewal
*****************************************************************************
*Desc           : 
*****************************************************************************/

#ifndef __TBREQUESTGUI_H__
#define __TBREQUESTGUI_H__

#pragma once

#include "NumberGui.h"
#include "SurfaceGui.h"

// World's Best Martial Arts Application GUI Script File
#define dTBREQUEST_GUI_FRAMEWORK_FILE		"gui\\TBRequestGui.frm"
#define dTBREQUEST_GUI_SURFACE_FILE			"gui\\TBRequestGui.srf"

#define dTBREQUEST_TEAM_MEMBER				5

/**
* \ingroup client
* \brief World's Best Martial Arts Application GUI
*/
class CTBRequestGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eRequestType
	{
		REQUEST_INDIVIDUAL,
		REQUEST_TEAM,

		REQUEST_NUMS,
		INVALID_REQUEST = 0xFF
	};

	enum eGuiType
	{
		TYPE_REQUEST,
		TYPE_TICKET,

		TYPE_NUMS,
		INVALID_TYPE = 0xFF
	};

public:
	CTBRequestGui(const RwChar* pName);
	virtual ~CTBRequestGui();

	RwBool	Create();
	void	Destroy();

	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);

	void	SetRequestType( RwUInt8 byRequestType );
	
	void	UpdateJoinInfo();
	void	UpdateInterface();

	void	ClearRequestMemberInfo();
	void	ClearTicketMemberInfo();

	void	OnClickedBtnClose(gui::CComponent* pComponent);
	void	OnClickedBtnAccept(gui::CComponent* pComponent);
	/*void	OnClickedBtnCancel(gui::CComponent* pComponent);*/

	void	OnMove( RwInt32 iOldX, RwInt32 iOldY );
	void	OnPaint();

protected:
	RwUInt8				m_byGuiType;
	RwUInt8				m_byRequestType;

	// Common Gui
	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;
	gui::CButton*		m_pBtnAccept;
	gui::CButton*		m_pBtnCancel;

	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnAccept;
	gui::CSlot			m_slotClickedBtnCancel;
	
	// personal application
	gui::CDialog*		m_pDlgIndividual;
	gui::CStaticBox*	m_pStbIndiName;
	gui::CStaticBox*	m_pStbIndiNameDisplay;
	gui::CStaticBox*	m_pStbIndiLevel;
	gui::CStaticBox*	m_pStbIndiLevelDisplay;
	gui::CStaticBox*	m_pStbIndiClass;
	gui::CStaticBox*	m_pStbIndiClassDisplay;
	gui::CStaticBox*	m_pStbIndiRecord;
	gui::CStaticBox*	m_pStbIndiRecordDisplay;
	gui::CStaticBox*	m_pStbIndiRankingPoint;
	gui::CStaticBox*	m_pStbIndiRankingPointDisplay;
	gui::CStaticBox*	m_pStbIndiGuild;
	gui::CStaticBox*	m_pStbIndiGuildDisplay;

	// Team Application
	gui::CDialog*		m_pDlgTeam;
	gui::CStaticBox*	m_pStbTeamName;
	gui::CInputBox*		m_pIpbTeamName;
	gui::CPanel*		m_paMemberClass[dTBREQUEST_TEAM_MEMBER];
	gui::CStaticBox*	m_paMemberName[dTBREQUEST_TEAM_MEMBER];
	gui::CStaticBox*	m_paMemberLevel[dTBREQUEST_TEAM_MEMBER];
	gui::CStaticBox*	m_paMemberRankingPoint[dTBREQUEST_TEAM_MEMBER];

	// personal receipt
	gui::CDialog*		m_pDlgIndiTicket;
	gui::CStaticBox*	m_pStbTicketGuide;
	gui::CStaticBox*	m_pStbTicketNumber;
	CNumberGui			m_numTicket;
	CSurfaceGui			m_surEmblem;
	gui::CTexture		m_texEmblem;
	gui::CStaticBox*	m_pStbDojoRecommned;
	std::string			m_strEmblemTextureName;

	// team receipt
	gui::CDialog*		m_pDlgTeamTicket;
	gui::CStaticBox*	m_pStbTicketTeamGuide;
	gui::CStaticBox*	m_pStbTicketTeamNumber;
	gui::CStaticBox*	m_pStbTicketTeamMember;
	gui::CStaticBox*	m_paTicketTeamMember[dTBREQUEST_TEAM_MEMBER];

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
};

#endif//__TBREQUEST_GUI_H__