/******************************************************************************
* File			: BroadCastGui.h
* Author		: Haesung Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 27
* Abstract		: 
*****************************************************************************
* Desc			: GUI responsible for game TS message, server emergency, and mini narration
*****************************************************************************/

#ifndef __BROAD_CAST_GUI_H__
#define __BROAD_CAST_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Client UI Config
#include "DBOUIConfig.h"

// Broad cast
#include "BroadCastUnit.h"
#include "NtlPLAccelController.h"

// Client gui
#include "SurfaceGui.h"

#define dBROAD_BALLOON_SHAPE_NUMS		3
#define dBROAD_BALLOON_BTS_DEFAULT_IMG	"BTS_Default.png"
#define dBROAD_BALLOON_BE_DEFAULT_IMG	"BE_Default.png"
#define dBROAD_HTML_TEXT_STANDARD		33
#define dBROAD_HTML_TEXT_LARGE_WIDTH	286
#define dBROAD_HTML_TEXT_SMALL_WIDTH	143
#define dBROAD_HTML_TEXT_HEIGHT			55
#define dBROAD_HTML_TEXT_LARGE_POSITION_X	-325
#define dBROAD_HTML_TEXT_SMALL_POSITION_X	-185
#define dBROAD_HTML_TEXT_POSITION_Y			60

// forward declaration
class CNtlPLAccelController;
class CBroadCastUnit;

// unit deque define
typedef std::deque< CBroadCastUnit > deqBroadUnit;

/**
* \ingroup client
*/
class CBroadCastGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	// Status of broadcast GUI
	enum eBroadState
	{
		STATE_DIRECTION_IN_START,	// Natanna begins directing
		STATE_DIRECTION_IN,			// Directing it to appear
		STATE_DIRECTION_IN_END,		// At the end of the presentation that appears -in the case of a slide, a speech bubble appears.
		STATE_OPEN,					// open state
		STATE_DIRECTION_OUT_START,	// Start the disappearing presentation -In the case of a slide, close the speech bubble.
		STATE_DIRECTION_OUT,		// Directing the disappearance
		STATE_DIRECTION_OUT_END,	// The end of the disappearing production
		STATE_CLOSE,				// closed state

		STATE_NUMS,
		INVALID_STATE = 0xFF
	};

	CBroadCastGui(const RwChar* pName);
	virtual ~CBroadCastGui();

	RwBool		Create();
	VOID		Destroy();

	// Updates
	VOID		Update( RwReal fElapsed );
	VOID		UpdateDirectionInStart( RwReal fElapsed );
	VOID		UpdateDirectionIn( RwReal fElapsed );
	VOID		UpdateDirectionInEnd( RwReal fElapsed );
	VOID		UpdateOpen( RwReal fElapsed );
	VOID		UpdateDirectionOutStart( RwReal fElapsed );
	VOID		UpdateDirectionOut( RwReal fElapsed );
	VOID		UpdateDirectionOutEnd( RwReal fElapsed );
	VOID		UpdateClose( RwReal fElapsed );

	// HandleEvents
	VOID		HandleEvents( RWS::CMsg &msg );
	RwInt32		SwitchDialog( bool bOpen );

	// Interfaces
	VOID		SetState( RwUInt8 byState );
	VOID		SetCurUnitData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecList );
	VOID		SetCurMsgData( CBroadCastUnit& unit );
	VOID		SetHtmlString( const WCHAR* pString, RwInt32 nSize );
	VOID		SaveCurUnitData();
	VOID		DeleteDeqUnit( RwInt8 byMsgType );
	VOID		DeleteDeqUnitAll();
	VOID		ShowBalloon( RwUInt8 byBalloonShape, RwBool bSmall = FALSE, RwBool bShow = TRUE);
	VOID		HideAllBalloon();

	VOID		SetIllust( const RwChar* pIllustName );
	VOID		SetIllust( RwUInt32 uiOwnerTblIdx, RwUInt32 uiOwnerCondition );
	VOID		SetillustPos( RwInt32 nX, RwInt32 nY );

	VOID		Play( RwBool bPlay );
	RwBool		IsPlay();

	// Signal back
	VOID		OnPaint();
	VOID		OnMove( RwInt32 nOldX, RwInt32 nOldY );


	// Gui
protected:
	gui::CPanel*		m_pPnlBack;
	gui::CStaticBox*	m_pStbName;
	gui::CFlash*		m_paFlaBalloon[dBROAD_BALLOON_SHAPE_NUMS];
	gui::CFlash*		m_paFlaBalloonSmall[dBROAD_BALLOON_SHAPE_NUMS];
	gui::CHtmlBox*		m_pHtmlText;
	gui::CPanel*		m_pPnlIllust;
	CSurfaceGui			m_surIllust;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

protected:
	// Function pointer for update function
	typedef				VOID (CBroadCastGui::*UpdateState)( RwReal );
	UpdateState			m_aCallUpdateState[STATE_NUMS];	///< Update function pointer

	RwUInt8				m_byState;			///< Current state of UI
	deqBroadUnit		m_deqUnit;			///< Queue of Units
	
	CBroadCastUnit		m_CurUnit;			///< Data of the unit currently being output
	RwBool				m_bCurUnit;			///< Whether the data of the unit currently being output is valid/invalid

	sBROAD_MSG_DATA		m_sCurData;			///< Data currently being output (Setting)	
	RwUInt8				m_uSlidingFadeAlpha;		///< Current Alpha (only used when Sliding/Fade)
	RwBool				m_bSmallText;		///< Whether the text currently being printed is Large

	RwReal				m_fElapsed;			///< Time currently consumed in the GUI

	RwReal				m_fShakeTime;		///< Time variables used in Illust Shake
	CRectangle			m_rectBackIllust;	///< Coordinates of the center of shaking of the illustration

	CNtlPLAccelController	m_conAccel;		///< AccelController
	CNtlPLAccelController	m_conAlpha;		///< Alpha AccelController

	RwBool				m_bPlay;			///< Is it currently playing?

	SUIConfigBroadCast*	m_pBroadCastConfig;	///< Options related to Broad Cast production
};

#endif//__BROAD_CAST_GUI_H__