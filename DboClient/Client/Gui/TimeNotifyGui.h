/******************************************************************************
*File			: TimeNotifyGui.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2008. 1. 2
*Abstract		: 
*****************************************************************************
*Desc			: Announce information related to time
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Trigger
#include "DboTSCoreDefine.h"

// Dbo
#include "NumberGui.h"


class CTimeNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eTimeNotifyType
	{
		TIME_TYPE_TMQ_QUEST_LIMIT_TIME,
		TIME_TYPE_RANK_BATTLE_LIMIT_TIME,
		TIME_TYPE_BUDOKAI_LIMIT_TIME,

		TIME_TYPE_INVALID
	};

	enum eDisplayType
	{
		DISPLAY_TYPE_NORMAL,
		DISPLAY_TYPE_WARNING,

		NUM_DISPLAY_TYPE
	};

	struct sTimeInfo
	{
		RwUInt8			byType;
		RwReal			fElapsed;
		
		sTS_KEY					sTSKey;				///< TS key
		RwUInt32				uiSortType;			///< Quest type
	};

	CTimeNotifyGui(const RwChar* pName);
	virtual ~CTimeNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTimeNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetDisplayType();
	VOID			LocateComponent();

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CNumberGui			m_MinuteNumber[NUM_DISPLAY_TYPE];
	CNumberGui			m_SecondNumber[NUM_DISPLAY_TYPE];

	CSurfaceGui			m_srfColon[NUM_DISPLAY_TYPE];

	sTimeInfo			m_TimeInfo;

	RwUInt8				m_byDisplayType;
};