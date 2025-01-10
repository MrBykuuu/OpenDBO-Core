/*****************************************************************************
 *
 * File			: NtlFSMCharActLayer.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2006. 2. 9	
 * Abstract		: fsm character act layer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_CAHRACT_LAYER_H__
#define __NTL_FSM_CAHRACT_LAYER_H__

#include "NtlFSMLayer.h"

class CNtlBehaviorCharPush;
class CNtlBehaviorCharShock;
class CNtlBehaviorCharKnockDown;

class CNtlFSMCharActLayer : public CNtlFSMLayer
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActLayer, NTL_DEFAULT_MEMORY_POOL)

private:

	CNtlBehaviorCharPush *m_pBePush;
	CNtlBehaviorCharShock *m_pBeShock;
	CNtlBehaviorCharKnockDown *m_pKnownDown;

private:

	void CreateController(RwUInt32 uiController);

public:

	CNtlFSMCharActLayer();
	~CNtlFSMCharActLayer();

	/**
    *Function called immediately after this class is created
    *
    */
	virtual RwBool Create(RwUInt32 uiController);

	/**
    *  A function to call before this class is destroyed.
    *
    */
	virtual void Destroy(void);

	/**
    *  layer update function.
    *
    */
	virtual void Update(RwReal fElapsed);

	/**
    *  layer event handler function.
    *
    */
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

#endif
