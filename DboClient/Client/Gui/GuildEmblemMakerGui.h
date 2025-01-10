/*****************************************************************************
*File           : GuildEmblemMakerGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : March 3, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Guild emblem creation window
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "SurfaceGui.h"

#define dSTUFF_TEXTURE_ROW			3
#define dSTUFF_TEXTURE_COLUMN		4

#define dPALETTE_ROW				4
#define dPALETTE_COLUMN				8

// Presentation
#include "NtlPlEmblemStuffList.h"
#include "NtlPLEmblemMaker.h"

struct sEmblemFactor;

class CGuildEmblemMakerGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sMoustInfo
	{
		RwUInt8			bySlotType;					///< Type of slot where the mouse is located
		RwUInt8			bySlotIndex;				///< Index of the slot where the mouse is located
	};

	struct sStuffSelect
	{
		RwUInt32		bySelectStuffIndex;			///< Index included in the name of the selected material texture
	};

	struct sStuffSlot
	{
		RwUInt8			byStuffTextureIndex;		///< Index included in the name of the material texture
		CSurfaceGui		srfStuff;					///< Surface of material texture

		sStuffSlot()
		:byStuffTextureIndex(dPLEMBLEM_INVALID_FACTOR)
		{
		}
	};

	CGuildEmblemMakerGui(const RwChar* pName);
	virtual ~CGuildEmblemMakerGui();

	virtual RwBool		Create();
	virtual VOID		Destroy();
	VOID				UpdateBeforeCamera(RwReal fElapsed);

	VOID				Active(bool bOpen);

protected:
	CGuildEmblemMakerGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetTextureStuff(RwInt8 byStuffType, RwUInt32 uiStuffTextureIndex);

	RwUInt8			PtInRectStuffEmblem(RwInt8 byRow, RwInt32 nX, RwInt32 nY);
	RwUInt8			PtInRectPalette(RwInt32 nX, RwInt32 nY);

	VOID			OnClicked_LeftAButton(gui::CComponent* pComponent);
	VOID			OnClicked_LeftBButton(gui::CComponent* pComponent);
	VOID			OnClicked_RightAButton(gui::CComponent* pComponent);
	VOID			OnClicked_RightBButton(gui::CComponent* pComponent);

	void			OnClicked_MarkColorButton(gui::CComponent* pComponent);
	void			OnClicked_FrameColorButton(gui::CComponent* pComponent);
	void			OnClicked_BackgroundColorButton(gui::CComponent* pComponent);

	VOID			OnClicked_OKButton(gui::CComponent* pComponent);
	VOID			OnClicked_CancelButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotLeftAButton;
	gui::CSlot			m_slotLeftBButton;
	gui::CSlot			m_slotRightAButton;
	gui::CSlot			m_slotRightBButton;

	gui::CSlot			m_slotMarkColorButton;
	gui::CSlot			m_slotFrameColorButton;
	gui::CSlot			m_slotBackgroundColorButton;

	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;
	gui::CSlot			m_slotCaptureMouseDown;


	gui::CStaticBox*	m_pTitle;
	gui::CStaticBox*	m_pExplain;
	gui::CStaticBox*	m_pExplain2;

	gui::CStaticBox*	m_pstbColorExplain;

	gui::CButton*		m_pLeftAButton;
	gui::CButton*		m_pLeftBButton;
	gui::CButton*		m_pRightAButton;
	gui::CButton*		m_pRightBButton;

	gui::CButton*		m_pbtnMarkColor;
	gui::CButton*		m_pbtnFrameColor;
	gui::CButton*		m_pbtnBackgroundColor;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pOKButton;
	gui::CButton*		m_pCancelButton;

	sMoustInfo			m_MouseInfo;
	sStuffSelect		m_StuffSelect[dSTUFF_TEXTURE_ROW];
	sStuffSlot			m_StuffSlot[dSTUFF_TEXTURE_ROW][dSTUFF_TEXTURE_COLUMN];///< Material texture to create the emblem	
	RwUInt8				m_byCurStuffRow;	///< The row of the material texture currently being modified
	RwUInt8				m_byPaletteFocus;
	RwBool				m_bStuffFocus;

	CSurfaceGui			m_srfEmblemSelect[dSTUFF_TEXTURE_ROW];
	CSurfaceGui			m_srfEmblemFocus;
	CSurfaceGui			m_srfPaletteFocus;
	CSurfaceGui			m_srfEmblem;

	MAP_STUFF*			m_mapStuffTypeA;
	MAP_STUFF*			m_mapStuffTypeB;
	
	CRectangle			m_rtPalette[dPALETTE_ROW * dPALETTE_COLUMN];		///< Area of ??color palette on GUI

	gui::CTexture		m_EmblemTexture;

	sEmblemFactor*		m_pEmblemFactor;
	std::string			m_strEmblemTextureName;
	RwBool				m_bUpdateEmblem;
};