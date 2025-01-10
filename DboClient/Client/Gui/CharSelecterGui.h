/******************************************************************************
*File           : CharSelecterGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : October 15, 2007
*Abstract		:
*****************************************************************************
*Desc           : GUI for selecting a character to play among your own characters
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Share
#include "NtlSharedDef.h"

// Gui
#include "gui_button.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "Inputhandler.h"

// Dbo
#include "Windowby3.h"


class CCharSelecterGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eCharState
	{
		CHAR_STATE_NORMAL,
		CHAR_STATE_RESERVATED_DEL,

		NUM_CHAR_STATE	// on tw its 3
	};

	struct sCharSlot
	{
		gui::CButton*		pButton[NUM_CHAR_STATE];
		gui::CStaticBox*	pName;
		gui::CStaticBox*	pLevel;

		gui::CStaticBox*	m_pTextStatic;

		CSurfaceGui			srfRace;
		CSurfaceGui			srfNewCharacter;
		CSurfaceGui			srfNewCharacterFoc;

		gui::CSlot			m_slotButton[NUM_CHAR_STATE];
		gui::CSlot			m_slotButtonMove[NUM_CHAR_STATE];
		gui::CSlot			m_slotButtonLeave[NUM_CHAR_STATE];

		RwBool				m_bHaveChar;
		RwUInt8				m_byState;

		sCharSlot() : m_bHaveChar(true), m_byState(CHAR_STATE_NORMAL)
		{}

		VOID SetPosition(RwInt32 iXPos, RwInt32 iYPos)
		{
			srfNewCharacter.SetPosition(iXPos + 36, iYPos + 20);
			srfNewCharacterFoc.SetPosition(iXPos + 36, iYPos + 20);
			srfRace.SetPosition(iXPos + 10, iYPos + 15);
		}

		VOID Render(RwBool bFocus)
		{
			if (m_bHaveChar)
			{
				srfRace.Render();
			}
			else
			{
				if (bFocus)
					srfNewCharacter.Render();
				else
					srfNewCharacterFoc.Render();
			}
		}
	};

	CCharSelecterGui(const RwChar* pName);
	virtual ~CCharSelecterGui();

	virtual RwBool		Create();
	virtual VOID		Update(RwReal fElapsed);
	virtual VOID		Destroy();

	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CCharSelecterGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			SelectSlot(RwUInt8 byIndex);
	VOID			SetDelButton();

	VOID			EnableButtons(bool bEnable);

	VOID			OnClickCharButton(gui::CComponent* pComponent);
	VOID			OnClickDelCharButton(gui::CComponent* pComponent);
	VOID			OnClickRecoveryCharButton(gui::CComponent* pComponent);
	VOID			OnMouseButtonMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseButtonLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnPostPaint();

protected:

	gui::CSlot			m_slotCharDelButton;
	gui::CSlot			m_slotCharRecoveryButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	CWindowby3			m_Background;	///< Top of background

	CSurfaceGui			m_srfInfoBack;		///<

	gui::CButton*		m_pCharDelButton;	///< Character delete button
	gui::CButton*		m_pCharRecoveryButton;///< Character recovery button

	gui::CStaticBox*	m_pTitleStatic;

	gui::CFrame*		m_pPostPaintDummy;

	RwUInt8				m_byFocusIndex;

	sCharSlot			m_CharSlot[NTL_MAX_COUNT_USER_CHAR_SLOT];///< Character slot
};