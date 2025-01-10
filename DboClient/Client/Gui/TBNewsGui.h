/*****************************************************************************
*File			: TBNewsGui.h
*Author		    : Haesung, Cho
*Copyright		: (аж)NTL
*Date			: 2008. 10. 21
*Abstract		:DBO World's Best Martial Arts Newsletter Renewal
*****************************************************************************
*Desc           : 
*****************************************************************************/

#ifndef __TBNEWSGUI_H__
#define __TBNEWSGUI_H__

#pragma once

// World's Best Martial Arts GUI script file
#define dTBNEWS_SCRIPT_FILE_FOLDER		L"script\\tenkaichibudokai\\"
#define dTBNEWS_GUI_FRAMEWORK_FILE		"gui\\TBNewsGui.frm"
#define dTBNEWS_GUI_SURFACE_FILE		"gui\\TBNewsGui.srf"

// Number of scrolling static boxes in World's Best Martial Arts Club
#define dTBNEWS_INFO_BOX_NUMS						2

// Minimum size of World's Best Martial Arts scroll box
#define dTBNEWS_INFO_BOX_MIN_WIDTH					245

// World's Best Martial Arts Scroll Box Spacing
#define dTBNEWS_INFO_BOX_MARGIN						10

/**
* \ingroup client
* \brief World's Best Martial Arts Newsletter GUI
*/
class CTBNewsGui : public CNtlPLGui, public RWS::CEventHandler
{
	enum eNewsType
	{
		TYPE_INDIVIDIAUL = 0,			///< Solo exhibition newsletter
		TYPE_TEAM,					///< Team Battle Newsletter

		TYPE_NUMS,
		INVALID_TYPE = 0xFF
	};

	enum eStaticInfo
	{
		INFO_STATE = 0,					///< Individual participation information
		INFO_INDITIME,				///< Individual exhibition time information
		INFO_TEAMTIME,				///< Pre-party time information
		
		INFO_NUMS,
		INVALID_INFO = 0xFF
	};

	enum eTextLine
	{
		TL_1 = 0,
		TL_2,
		TL_3,
		TL_4,

		TL_NUMS
	};

	enum eNoticeType
	{
		NIT_STATE_GUIDE,
		NIT_STATE_END,
		
		NIT_NUMS,
		INVALID_NIT = 0xFF
	};

public:
	CTBNewsGui(const RwChar* pName);
	virtual ~CTBNewsGui();

	RwBool	Create();
	void	Destroy();

	void	Update(RwReal fElapsed);

	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);

public:
	void	UpdateInterface();
	void	SetNews( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState );
	void	SetMatchNotice( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState );
	void	SetPrizeText( eNoticeType eNitType, RwUInt8 byMatchType, gui::CStaticBox* pComponent, const WCHAR* awcWinner, const WCHAR* awcSecondWinner );
	void	UpdateNoticeImage( eNoticeType eNitType );
	void	UpdateButtons();

	void	OnClickedBtnClose( gui::CComponent* pComponent );
	void	OnClickedBtnSoloRequest( gui::CComponent* pComponent );
	void	OnClickedBtnPartyRequest( gui::CComponent* pComponent );
	void	OnClickedBtnLicense( gui::CComponent* pComponent );
	void	OnClickedBtnTournenment( gui::CComponent* pComponent );

	// Info
public:
	void	InitInfoBox();
	WCHAR*	GetNextStepString( RwUInt8 byMatchType );			///< Message output for the following status
	void	SetInfoBox( RwUInt8 byInfoBoxNum, gui::CStaticBox* pStbInfoBox );
	void	UpdateInstanceInfo();


protected:
	gui::CStaticBox*	m_pStbTitle;			///< Name of newsletter
	gui::CStaticBox*	m_pStbHeadLine;			///< Headline of the newsletter
	gui::CStaticBox*	m_pStbNewsMark;			///< Mark in the newsletter

	gui::CPanel*		m_pPnlImage;			///< Image inserted into the newsletter
	gui::CHtmlBox*		m_pHtmlNews;			///< Html box where the newsletter will be printed

	gui::CStaticBox*	m_pStbNoticeText[TL_NUMS];	///< Text to be printed in the newsletter
		
	gui::CButton*	m_pBtnClose;			///< Close button
	gui::CButton*	m_pBtnSoloRequest;		///< Button for applying for solo exhibition
	gui::CButton*	m_pBtnPartyRequest;		///< Button for applying before party
	gui::CButton*	m_pBtnLicense;			///< Button on receipt
	gui::CButton*	m_pBtnTournement;		///< Bracket button

	// Scroll infotext
	gui::CDialog*		m_pDlgInfo;			///< Clipping dialog
	gui::CStaticBox*	m_paStbInfoBox[dTBNEWS_INFO_BOX_NUMS];
	RwUInt8				m_aBoxPresentInfo[dTBNEWS_INFO_BOX_NUMS];
	RwReal				m_fBoxPresentPosX[dTBNEWS_INFO_BOX_NUMS];

	RwUInt8				m_byCurFirstBox;	///< Number of the box you are currently ahead of

	// Slot
	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnSoloRequest;
	gui::CSlot			m_slotClickedBtnPartyRequest;
	gui::CSlot			m_slotClickedBtnLicense;
	gui::CSlot			m_slotClickedBtnTournement;

	RwBool				m_bMudosaUpdate;
	RwUInt32			m_uiMudosaReaminTime;
};

#endif//__TBNEWS_GUI_H__