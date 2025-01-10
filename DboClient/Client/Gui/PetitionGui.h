/******************************************************************************
*File: PetitionGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 23, 2008
*Abstract		: 
*****************************************************************************
*Desc: GM consultation request window
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "Windowby3.h"
#include "SurfaceGui.h"

class CPetitionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPetitionGui(const RwChar* pName);
	virtual ~CPetitionGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	// avooo's comment: The GM consultation request window is not part of the game and there are already other dialogs.
	//					 It is impossible to fix a fixed seat in a crowded situation.
	//					 Allow users to change their location at any time.
	virtual void	SetMovable(RwBool bMovable) {}

protected:
	CPetitionGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			EnableControll(bool bEnable);

	VOID			SetTextCategory(RwInt32 iSelect = INVALID_INDEX);
	VOID			SetTextCategory2(RwInt32 iSelect = INVALID_INDEX);

	const WCHAR*	GetCategory1Text(RwInt32 iSelect);
	const WCHAR*	GetCategory2Text(RwInt32 iSelect);

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_SendButton(gui::CComponent* pComponent);

	VOID			OnItemSelect( INT nIndex );
	VOID			OnItemSelect2( INT nIndex );
	
	VOID			OnGotFocus();
	VOID			OnLostFocus();
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotItemSelect;
	gui::CSlot			m_slotItemSelect2;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotSendButton;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pCloseButton;			///< Close button

	gui::CComboBox*		m_pCategory;			///< Main category
	gui::CComboBox*		m_pCategory2;			///< Subcategories
	
	gui::CInputBox*		m_pInput;				///< Contents of complaint
	
	gui::CStaticBox*	m_pDialogName;			///< Dialog title
	gui::CStaticBox*	m_pCategoryStatic;		///< Main category
	gui::CStaticBox*	m_pCategoryStatic2;		///< Subcategories
	gui::CStaticBox*	m_pCategoryMessage;		///< Main category selection message
	gui::CStaticBox*	m_pCategoryMessage2;	///< Subcategory selection message
	
	gui::CButton*		m_pSendButton;			///< Send button

	CWindowby3			m_BackPanel;			///< background
	CSurfaceGui			m_srfCaregoryBack;		///< Category 1 Background
	CSurfaceGui			m_srfCaregoryBack2;		///< Category 2 Background
};