/******************************************************************************
*File			: DiceGui.h
*Author	    	: Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2008. 5. 26
*Abstract		: 
*****************************************************************************
*Desc			: 
*****************************************************************************/

#pragma once

// Presetation
#include "NtlPLGui.h"

// Dbo
#include "SlotGui.h"
#include "DiceManager.h"

struct sITEM_PROFILE;

struct sWaitDiceDisappear
{
	RwReal		fElapsedDisappear;	///< Time until the dice disappear
	RwBool		bWaitDisappear;		///< Whether to wait for the dice to disappear

	sWaitDiceDisappear()
	:fElapsedDisappear(0.f), bWaitDisappear(false)
	{
	}
};

class CDiceGui : public CNtlPLGui
{
public:
	CDiceGui(const RwChar* pName, RwUInt8 byWorkID)
	:CNtlPLGui(pName)
	,m_byWorkID(byWorkID)
	,m_byResultNum(0)
	,m_bGenerateResultEvent(false)
	,m_byFunctionFlag(dDICE_FUNCTION_NONE)
	{}

	virtual ~CDiceGui() {}

	virtual RwBool	Create(RwUInt8 byFunctionFlag) = 0;
	virtual VOID	Destroy() = 0;

	virtual RwBool	ReadyDice() = 0;

	RwUInt8			GetWorkID() { return m_byWorkID; }

	virtual VOID	SetResultNum(RwUInt8 byNum) { m_byResultNum = byNum; }
	virtual VOID	Switch(bool bOpen) = 0;

protected:
	CDiceGui() {}

protected:
	RwUInt8		m_byWorkID;
	RwUInt8		m_byResultNum;
	RwBool		m_bGenerateResultEvent;
	RwUInt8		m_byFunctionFlag;
};


class CActionDiceGui : public CDiceGui
{
public:
	CActionDiceGui(const RwChar* pName, RwUInt8 byWorkID)
	:CDiceGui(pName, byWorkID) {}
	virtual ~CActionDiceGui();

	RwBool		Create(RwUInt8 byFunctionFlag);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Switch(bool bOpen);

	virtual RwBool	ReadyDice();
	virtual VOID	SetResultNum(RwUInt8 byNum);

	VOID			SetExplain(const WCHAR* pwcText);
	VOID			SetExplain2(const WCHAR* pwcText);

protected:
	CActionDiceGui() {}
	CActionDiceGui(const RwChar* pName) {}

	VOID			CheckInfoWindow();

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnMovieEnd(gui::CComponent* pComponent);
	VOID			OnFSCallback(const char* pcString, const char* pcString2);

protected:	
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotFSCallback;
	gui::CSlot			m_slotMovieEnd;
	
	gui::CButton*		m_pExitButton;			///< Close window button
	gui::CFlash*		m_pflashDice;			///< Dice Flash

	gui::CStaticBox*	m_pActionDiceExplain;	///< Action dice description
	gui::CStaticBox*	m_pActionDiceExplain2;	///< Action dice description 2
	gui::CStaticBox*	m_pResultNum;			///< Number rolled on the dice
};


class CItemDiceGui : public CDiceGui
{
public:	
	CItemDiceGui(const RwChar* pName, RwUInt8 byWorkID);
	virtual ~CItemDiceGui();

	RwBool		Create(RwUInt8 byFunctionFlag);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Switch(bool bOpen);

	virtual RwBool	ReadyDice();
	virtual VOID	SetResultNum(RwUInt8 byNum);

	VOID			SetExplain(const WCHAR* pwcText);
	
	VOID			SetItemData(void* pItemData);

protected:
	CItemDiceGui() {}
	CItemDiceGui(const RwChar* pName) {}

	VOID			CheckInfoWindow();

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnMovieEnd(gui::CComponent* pComponent);
	VOID			OnFSCallback(const char* pcString, const char* pcString2);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:	
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotFSCallback;
	gui::CSlot			m_slotMovieEnd;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pExitButton;			///< Close window button
	gui::CFlash*		m_pflashDice;			///< Dice Flash

	gui::CStaticBox*	m_pAutoDiceExplain;		///< Description of automatic dice
	gui::CStaticBox*	m_pResultNum;			///< Number rolled on the dice

	gui::CProgressBar*	m_pTimeGauge;			///< Dice rolling time limit

	CSurfaceGui			m_srfItemSlot;			///< Item slot background
	CSurfaceGui			m_srfTimeGaugeBack;		///< Dice rolling time limit background

	CRegularSlotGui			m_slot;					///< Item slot

	RwReal				m_fElapsed;				///< Limited time to roll the dice
	
	sWaitDiceDisappear	m_WaitDiceDisappear;	///< Information needed until the dice disappear

	sITEM_PROFILE*		m_pItemProfile;	
};