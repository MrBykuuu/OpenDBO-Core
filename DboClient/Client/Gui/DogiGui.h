/******************************************************************************
*File			: DogiGui.h
*Author		    : Hong SungBock
*Copyright		: NTL Co., Ltd.
*Date			: 2009. 4. 6
*Abstract		: 
*****************************************************************************
*Desc			: µµº¹ GUI
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SlotGui.h"


class CDogiGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
#define dMAX_COLOR_ROW				(4)
#define dMAX_COLOR_COLUMN			(8)
#define dMAX_PALETTE				(dMAX_COLOR_ROW*dMAX_COLOR_COLUMN)

	struct sUSE_COLOR_INFO
	{
		RwBool				bUseAllColor;
		RwUInt8				byEnableColorRow;
	};

public:
	CDogiGui(const RwChar* pName);
	virtual ~CDogiGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CDogiGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Clear();
	VOID			UpdateBeforeCamera( RwReal fElapsed );
	RwBool			SetDogi(SERIAL_HANDLE hDogi);
	VOID			UnsetDogi();
	VOID			SelectColor(RwUInt8 byIndex);

	VOID			CheckInfoWindow();
	RwUInt8			PtinSlot(RwInt32 iX, RwInt32 iY);

	VOID			RegisterDogiItem( TBLIDX tblIdx, RwUInt8 byGrade );
	VOID			UnregisterDogiIem( void );

	VOID			OnClicked_HelpButton( gui::CComponent* pComponent );
	VOID			OnClicked_ExitButton( gui::CComponent* pComponent );
	VOID			OnPressed_LeftArrowButton( gui::CComponent* pComponent );
	VOID			OnPressed_RightArrowButton( gui::CComponent* pComponent );
	VOID			OnReleased_LeftArrowButton( gui::CComponent* pComponent );
	VOID			OnReleased_RightArrowButton( gui::CComponent* pComponent );
	VOID			OnClicked_OKButton( gui::CComponent* pComponent );

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotHelpButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotLeftArrowButtonPress;
	gui::CSlot			m_slotLeftArrowButtonRelease;
	gui::CSlot			m_slotRightArrowButtonPress;
	gui::CSlot			m_slotRightArrowButtonRelease;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pBtnHelp;
	gui::CButton*		m_pBtnExit;
	gui::CButton*		m_pBtnLeftArrow;
	gui::CButton*		m_pBtnRightArrow;
	gui::CButton*		m_pBtnOK;
	gui::CButton*		m_pBtnCancel;

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pPreviewDogiStatic;				///< ¡®Preview of uniform¡¯
	gui::CStaticBox*	m_pSetupDogiStatic;					///< 'Drum settings'

	CSurfaceGui			m_srfPreviewDogiBar;
	CSurfaceGui			m_srfSetupDogiBar;
	CSurfaceGui			m_srfCharacterBackground;
	CSurfaceGui			m_srfDogiBackground;
	CSurfaceGui			m_srfColorDisable[dMAX_COLOR_ROW];
	CSurfaceGui			m_srfCharacter;						///< Surface on which the character is rendered
	CSurfaceGui			m_srfSlotFocus;

	gui::CTexture		m_texCharacter;

	CRegularSlotGui			m_DogiSlot;

	RwBool				m_bSlotFocus;
	RwBool				m_bSlotDest;
	RwBool				m_bExpectServerAnswer;

	RwUInt8				m_byLeftMouseDown;
	RwUInt8				m_byRightMouseDown;	
	SERIAL_HANDLE		m_hNPCSerial;
	RwUInt8				m_bySelectedColorIndex;
	RwUInt8				m_byOriginalColorIndex;

	RwUInt32			m_uiDojoTableIndex;

	sUSE_COLOR_INFO		m_tUseColorInfo;

	TBLIDX				m_tblDogiItem;
	RwUInt8				m_byDogiGrade;
};