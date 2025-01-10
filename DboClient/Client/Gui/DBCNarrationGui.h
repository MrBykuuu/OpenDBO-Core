////////////////////////////////////////////////////////////////////////////////
// File: DBCNarrationGui.h
// Desc: DBCNarrationGui
//		 Narration °ü·Ã
//
// 2006.12.5 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DBC_NARRATION_GUI_H__
#define __DBC_NARRATION_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSLEvent.h"

class CNtlPLCinematicObject;

class CDBCNarrationGui : public RWS::CEventHandler
{
public:
	enum STATE { STATE_NONE, STATE_BEGIN, STATE_ING, STATE_IDLE, STATE_END };

	//! Constructor & Destructor
	CDBCNarrationGui(VOID);
	~CDBCNarrationGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	INT		StateUpdate( RwUInt32 uiElapsedTime );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	//! Singleton
	static VOID	CreateInstance(VOID);
	static VOID DeleteInstance(VOID);

private:
	//! Implement	
	VOID	SetUIData( const RwChar* szTextureName, const WCHAR* wszText, RwInt32 eNarrationState );
	
	VOID	SetState( STATE eState );

	VOID	BeginStateDirection(VOID);
	VOID	IngStateDirection(VOID);
	VOID	EndStateDirection(VOID);
	
	//! Call back
	VOID	OnMouseDown( const CKey& key );

	gui::CSlot	m_slotOnMouseDown;

	//! Variable
	STATE				m_eUIState;					// State of the UI itself
	RwInt32				m_eNarrationState;			// NarrationÀÇ State	

	CNtlPLCinematicObject*	m_pCinematicObject;

public:
	static CDBCNarrationGui* m_pInstance;			// Singleton Instance
};

#endif//__DBC_NARRATION_GUI_H__