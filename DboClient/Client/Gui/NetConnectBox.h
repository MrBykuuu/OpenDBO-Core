///******************************************************************************
//* File			: NetConnectBox.h
//* Author		    : Hong SungBock
//* Copyright		: (аж)NTL
//* Date			: 2007. 1. 4
//* Update		    : 2007. 10. 15
//* Abstract		: box dedicated to network connect
//*****************************************************************************
//* Desc			: CNetConnectBox
//*****************************************************************************/
//
//#pragma once
//
//// Core
//#include "ceventhandler.h"
//
//// Gui
//#include "gui_button.h"
//
//// presentation
//#include "NtlPLGui.h"
//
//// Dbo
//#include "DboDef.h"
//#include "Windowby3.h"
//
//class CNetConnectBox : public CNtlPLGui , public RWS::CEventHandler
//{
//public:
//	CNetConnectBox(const RwChar* pName);
//	virtual ~CNetConnectBox();
//
//	RwBool		Create();
//	VOID		Destroy();
//	VOID		Update(RwReal fElapsed);
//
//protected:
//	CNetConnectBox() {}
//	VOID			HandleEvents( RWS::CMsg &msg );	
//	
//	VOID			CalcBoxSize();				///< Creates a box that matches the given message
//	VOID			SetText();
//	VOID			ProcessCancel();
//	VOID			SwitchDialog(bool bSwitch);
//
//	VOID			OnClickedOkButton(gui::CComponent* pComponent);
//	VOID			OnClickedCancelButton(gui::CComponent* pComponent);	
//	VOID			OnPaint();	
//
//protected:
//
//	gui::CSlot			m_slotPaint;
//	gui::CSlot			m_slotOk;
//	gui::CSlot			m_slotCencle;
//
//	CWindowby3			m_Backboard;			///< background
//	CWindowby3			m_Panel;				///< Text background frame
//
//	gui::CButton*		m_pOkBtn;
//	gui::CButton*		m_pCancelBtn;
//
//	gui::CStaticBox*	m_pMessage;				///< message content
//
//	std::wstring		m_wstrString;			///< Original message
//	RwUInt8				m_byMsgBoxType;			///< message box shape type
//	RwUInt32			m_uiMsgProcessType;		///< Message processing type
//	RwReal				m_fRemainTime;			///< update time
//	sMsgBoxData			m_ExData;				///< message extension data
//
//	RwBool				m_bRemainTime;			///< Whether there is a timeout for displaying the message box
//	const WCHAR*		m_pwcText;				///< Last shown string table index
//};