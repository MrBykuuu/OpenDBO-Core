/******************************************************************************
*File: LittleQuestContainer.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : February 20, 2007
*Abstract		: 
*****************************************************************************
*Desc: Instance quest container of a specific type
*****************************************************************************/

#pragma once

#include <list>

// Core
#include "ceventhandler.h"

// Share
#include "NtlTimeQuest.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

// Dbo
#include "SurfaceGui.h"

class CLittleQuestContainer : public RWS::CEventHandler
{
#define dMAX_QUEST_AIM			4	///< Maximum number of goals per quest

	struct sQuestData
	{
		sTS_KEY					sTSKey;				///< TS key
		RwUInt32				uiSortType;			///< Quest type
		RwUInt32				uiQuestTitle;		///< Quest title
		eSTOC_EVT_DATA_TYPE		eEvtInfoType;		///< Server event information -data type
		uSTOC_EVT_DATA			uEvtInfoData;		///< Server event information -data
		RwUInt32				uiTimeLimit;		///< Time limit information
	};

public:
	CLittleQuestContainer();
	virtual ~CLittleQuestContainer();

	typedef std::list<sQuestData*>				LIST_QUEST;
	typedef std::list<sQuestData*>::iterator	LIST_QUEST_ITER;

	RwBool		Create(RwUInt32 uiQuestType, gui::CStaticBox* pQuestTitle, gui::CStaticBox* pAim_1, gui::CStaticBox* pAim_2,
					   gui::CStaticBox* pAim_3, gui::CStaticBox* pAim_4);
	VOID		Destroy();

	VOID		NextQuest();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			InitQuestData();
	VOID			SetIndicator(sQuestData* pQuestData);
	sQuestData*		FindData(NTL_TS_T_ID tID);
	sQuestData*		NextData(NTL_TS_T_ID tID_current);
	VOID			DelData(NTL_TS_T_ID& tID);

protected:		
	gui::CStaticBox*	m_pQuestTitle;				///< Quest title
	gui::CStaticBox*	m_pQuestAim[dMAX_QUEST_AIM];///< Quest Goal

	RwUInt32			m_uiQuestType;
	NTL_TS_T_ID			m_tID;						///<Currently displayed quest ID
	LIST_QUEST			m_listQuestData;
};