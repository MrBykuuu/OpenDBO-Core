/******************************************************************************
* File			: ScrambleBalloonHelper.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2009. 6. 18
* Abstract		: 
*****************************************************************************
* Desc			: Supervises the creation and destruction of pre-painting speech bubble managers.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Simulation
#include "NtlSLDef.h"


class ScrambleBalloonHelper : public RWS::CEventHandler
{
	enum eBATTLE_BALLOON_STATE
	{
		BATTLE_BALLOON_STATE_NOT_CREATE,
		BATTLE_BALLOON_STATE_CREATED,
		BATTLE_BALLOON_STATE_FAIL_CREATE,
	};

	struct sBALLOON_INFO
	{
		eBATTLE_BALLOON_STATE	eBattleBalloonState;
		RwUInt8					byTickCount;
	};

	typedef std::map<SERIAL_HANDLE, sBALLOON_INFO>		MAP_PLAYER_BALLOON;	

public:
	ScrambleBalloonHelper();
	virtual ~ScrambleBalloonHelper();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );
	VOID			ClearBalloon();

protected:
	MAP_PLAYER_BALLOON		m_mapPlayerBalloon;
};