#pragma once

#include <map>

// Core
#include "ceventhandler.h"

// Share
#include "NtlSharedDef.h"

// Simulation
#include "NtlSLDef.h"
#include "DboTSCoreDefine.h"
#include "NtlSLGlobal.h"

struct sNPC_TBLDAT;

#define dINVALID_WARFOG_INDEX				0xffff

struct sNextQuest
{
	RwV3d				v3RealPosition;
	eQMI_TARGET_TYPE	eTargetType;
	QM_KEY				QMKey;
	WORLDID				WorldID;
	TBLIDX				Tblidx;
	RwBool				bShow;
	std::wstring		wstrName;

	union
	{
		sNPC_TBLDAT*	pNPC_TBLDAT;
	};
};

typedef std::multimap<QM_KEY, sNextQuest>				MAP_NEXTQUEST;
typedef std::multimap<QM_KEY, sNextQuest>::iterator		MAP_NEXTQUEST_ITER;


struct sBindInfo
{
	RwUInt8				byBindType;					///< Bind type (eDBO_BIND_TYPE)
	WORLDID				BindedWorldID;				///< ID of the bound world
	RwUInt32			uiBindedObejcTblIdx;		///< Bound object table index
};


enum eBUS_SHAPE_TYPE
{
	BUS_SHAPE_HUMAN,
	BUS_SHAPE_NAMEK,
	BUS_SHAPE_MAJIN,

	NUM_BUS_SHAPE_TYPE,

	INVALID_BUS_SHAPE_TYPE
};

struct sBusRoute
{
	SERIAL_HANDLE		hBus;
	sNPC_TBLDAT*		pNPC_TBLDAT;
	eBUS_SHAPE_TYPE		eBusShapeType;
	RwV3d				v3Pos;
	RwV3d				v3Dir;
};

typedef std::map<SERIAL_HANDLE, sBusRoute>				MAP_BUS_ROUTE;
typedef std::map<SERIAL_HANDLE, sBusRoute>::iterator	MAP_BUS_ROUTE_ITER;

class CNtlOtherParam : public RWS::CEventHandler
{
public:
	CNtlOtherParam();
	virtual ~CNtlOtherParam();

	virtual	RwBool	Create(void);
	VOID			Destroy(void);
	
	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	MAP_NEXTQUEST_ITER GetNextQuestBegin();
	MAP_NEXTQUEST_ITER GetNextQuestEnd();

	const sBusRoute*   GetBusRoute(SERIAL_HANDLE hHandle);
	MAP_BUS_ROUTE_ITER GetBusRouteBegin();
	MAP_BUS_ROUTE_ITER GetBusRouteEnd();

	RwUInt32			GetHelpHintConditionCheck() { return m_uiHelpHint; }
	sBindInfo*			GetBindInfo() { return &m_BindInfo; }

	void				SetWarFolgValue(RwInt32 iFlagIndex);
	char*				GetWarFomFlag();

	void				SetCharTitleValue(RwInt32 iFlagIndex);
	void				UnSetCharTitleValue(RwInt32 iFlagIndex);
	char*				GetCharTitleFlag();

	void				SetRegisterFuelItem(SERIAL_HANDLE hItem);
	void				SetRegisterFuelItemTableIndex(TBLIDX idx);
	SERIAL_HANDLE		GetRegisterFuelItem();
	TBLIDX				GetRegisterFuelItemTableIndex();

	void				SetScouterOn(bool bFlag);
	bool				IsScouterOn();

	RwBool				IsExistNextQuest_of_NPC(TBLIDX idNPCTableIndex);
	RwBool				IsExistWarFog_of_Index(RwInt32 iFlagIndex);

protected:
	RwUInt32		m_uiHelpHint;

	SERIAL_HANDLE	m_hRegisteredFuelItem;		///< Only update the correct handle when using the vehicle
	TBLIDX			m_idxRegisteredFuelItem;

	MAP_NEXTQUEST	m_multimapNextQuest;
	MAP_BUS_ROUTE	m_mapBusRoute;
	sBindInfo		m_BindInfo;	

	bool			m_bScouterOn;
};