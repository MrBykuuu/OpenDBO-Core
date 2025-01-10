/******************************************************************************
*File			: LoadingGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2008. 6. 22
*Abstract		: 
*****************************************************************************
*Desc			: 
*****************************************************************************/

#pragma once

// Shard
#include "NtlSharedType.h"

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"


enum eLOADING_PAGE_TYPE
{
	LOADING_PAGE_FIRST_STEP,
	LOADING_PAGE_SECOND_STEP,
	LOADING_PAGE_THIRD_STEP,

	INVALID_LOADING_PAGE
};


class CLoadingGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CLoadingGui(const RwChar* pName);
	virtual ~CLoadingGui();

	virtual RwBool		Create();
	virtual VOID		Update(RwReal fElapsed);
	virtual VOID		Render();
	virtual VOID		Destroy();

	VOID			Load_LoadingPage(eLOADING_PAGE_TYPE eType);

protected:
	CLoadingGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool			LoadPage_Job();
	RwBool			LoadPage_Region();
	RwBool			LoadPage_GameTip();

	VOID			SetGameTipText();

	RwBool			LoadFlash(const char* pcFileName, gui::CFlash* pFlash);

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			OnMovieEnd(gui::CComponent* pComponent);
	VOID			OnMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pFlash;

	gui::CStaticBox*	m_pTipTitle;
	gui::CStaticBox*	m_pTipMessage;

	eLOADING_PAGE_TYPE	m_eLoadingPageType;

	TBLIDX				m_idxStart;			///< Starting index to search for table that explains game tips
	TBLIDX				m_idxEnd;			///< Last index to search for table explaining game tips
};