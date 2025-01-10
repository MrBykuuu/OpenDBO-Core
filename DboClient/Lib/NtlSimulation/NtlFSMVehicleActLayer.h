/*****************************************************************************
 *
 * File			: NtlFSMVehicleActLayer.h
 * Author		: JeongHo, Rho
 * Copyright	: NTL Co., Ltd.
 * Date			: 2008. 11. 12	
 * Abstract		: fsm vehicle act layer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_VEHICLE_LAYER_H__
#define __NTL_FSM_VEHICLE_LAYER_H__


#include "NtlFSMLayer.h"


class CNtlFSMVehicleActLayer : public CNtlFSMLayer
{
	DECLEAR_MEMORY_POOL( CNtlFSMVehicleActLayer, NTL_DEFAULT_MEMORY_POOL )

private:

	void CreateController( RwUInt32 uiController );

public:

	CNtlFSMVehicleActLayer( void );

	/**
    *Function called immediately after this class is created
    *
    */
	virtual RwBool Create( RwUInt32 uiController );

	/**
    *Function called before this class is destroyed
    *
    */
	virtual void Destroy( void );

	/**
    *layer update function.
    *
    */
	virtual void Update( RwReal fElapsed );

	/**
    *Layer event handler function.
    *
    */
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );
};


#endif
