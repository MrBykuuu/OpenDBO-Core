/******************************************************************************
*File           : PetitionSatisFactionGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 23, 2008
*Abstract		: 
*****************************************************************************
*Desc           : Satisfaction survey GUI on consultation with GM
*****************************************************************************/

#pragma once

// Presetation
#include "NtlPLGui.h"

// Share
#include "NtlSharedType.h"
#include "NtlPetition.h"

#define dTEMP_GG	5

class CPetitionSatisfaction : public CNtlPLGui
{
public:
	CPetitionSatisfaction(const RwChar* pName);
	virtual ~CPetitionSatisfaction();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPetitionSatisfaction() {}

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_RadioToggleButton(gui::CComponent* pComponent, bool bToggled);
	VOID			OnClicked_SendButton(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRadioButton[dTEMP_GG];
	gui::CSlot			m_slotSendButton;

	gui::CStaticBox*	m_pDialogName;					///< Dialog title
	gui::CStaticBox*	m_pMessage;						///< Information message
	gui::CStaticBox*	m_pToggleText[dTEMP_GG];		///< Satisfaction text

	gui::CButton*		m_pCloseButton;					///< Close button
	gui::CButton*		m_pRadioButton[dTEMP_GG];		///< Satisfaction selection radio button
	gui::CButton*		m_pSendButton;					///< Send satisfaction button

	RwInt32				m_iSelectedIndex;				///< Selected satisfaction index

	PETITIONID			m_petitionID;
	ACCOUNTID			m_GMID;
};