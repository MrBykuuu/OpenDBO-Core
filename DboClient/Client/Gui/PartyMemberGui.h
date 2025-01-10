/******************************************************************************
*File: PartyMemberGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Abstract		: October 13, 2006
*****************************************************************************
*Desc: CPartyMemberBuffGui: Shows the buffs of party members.
*
*CPartyMemberGui: Shows party member information.
*Two CPartyMemberBuffGui objects
*With this, you can obtain a buff consisting of two floors, top and bottom.
*Shows.
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Shared
#include "NtlParty.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SurfaceGui.h"

class CBuffDispGui;

class CPartyMemberBuffGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPartyMemberBuffGui();
	virtual ~CPartyMemberBuffGui();

	RwBool		Create(SERIAL_HANDLE hHandle);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Show(bool bShow);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);
	CBuffDispGui* GetBuffsDispGui() { return m_pBuff; }; //EXPORT BUFF DISP GUI

protected:
	VOID		HandleEvents(RWS::CMsg &pMsg);	

	VOID		OnPaint();

	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	SERIAL_HANDLE		m_hOwnerHandle;

	CBuffDispGui*		m_pBuff;
};

class CPartyMemberGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPartyMemberGui();
	virtual ~CPartyMemberGui();

	RwBool		Create(SERIAL_HANDLE hSerial, WCHAR* pcText, RwUInt8 byClass);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();
	CPartyMemberBuffGui* GetBuffsGui() { return m_pBuff; }; //Export BUFF GUI

	SERIAL_HANDLE GetHandle();						///< Returns the member's handle

	VOID		SetLP(RwInt32 iValue, RwInt32 iMaxValue);	///< LP settings
	VOID		SetEP(RwInt32 iValue, RwInt32 iMaxValue);	///< EP settings
	VOID		SetLevel(RwInt8 byLevel);			///< Level settings
	VOID		SetClass(RwUInt8 byClass);			///< Class settings
	VOID		SetLeader(RwBool bLeader);			///< Settings for party leader
	VOID		SetOtherWorld(RwBool bOtherWorld);
	VOID		SetCurEmergencyPulse(RwReal fPulse);
	VOID		SetEmergencyState(RwBool bState);

	RwReal		GetCurEmergencyPulse();
	RwBool		GetEmergencyState();

	VOID		ShowBuff(bool bShow);

protected:
	virtual VOID HandleEvents( RWS::CMsg &msg );

	VOID		OnPostPaint();
	
	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMove(RwInt32 nX, RwInt32 nY);
	VOID		OnMouseEnter(gui::CComponent* pComponent);
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	gui::CPanel*		m_pTributarySimbol;			///< Illustration of races and occupations

	gui::CStaticBox*	m_pNameStatic;				///< member name

	gui::CProgressBar*	m_pLPGauge;					///< LP gauge
	/*
	gui::CStaticBox*	m_pLPStatic;				///< LP static
	gui::CStaticBox*	m_pLPSlachStatic;			///< slash static
	gui::CStaticBox*	m_pMaxLPStatic;				///< MaxLP static
*/
	gui::CProgressBar*	m_pEPGauge;					///< EP gauge
	/*
	gui::CStaticBox*	m_pEPStatic;				///< EP Static
	gui::CStaticBox*	m_pEPSlachStatic;			///< slash static
	gui::CStaticBox*	m_pMaxEPStatic;				///< MAxEP static
*/
	gui::CStaticBox*	m_pLevel;					///< Level

	CSurfaceGui			m_srfOtherWorldPanel;		///< Panel of party members in another world
	CSurfaceGui			m_srfDeathPanel;			///< Panel upon death

	CSurfaceGui			m_srfLeaderMark;			///< Leader Mark
	CSurfaceGui			m_srfDeathMark;				///< Death Mark
	CSurfaceGui			m_srfTooFarMark;			///< Party member far away

	gui::CFlickerEffect	m_feEmergency;				///< LP warning light

	SERIAL_HANDLE		m_hSerial;					///< Member's cereal
	RwUInt8				m_byClass;					///< class
	RwInt8				m_byMouse;					///< Whether the member window was pressed

	RwUInt8				m_byDisplayFlag;

	CPartyMemberBuffGui*	m_pBuff;				///< Buff Window
};