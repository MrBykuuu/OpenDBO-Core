/*****************************************************************************
 *
 * File			: NtlSobEventObject.h
 * Author		: agebreak
 * Copyright	: NTL Co., Ltd.
 * Date			: 2007. 9. 4	
 * Abstract		: Simulation Event object
 *****************************************************************************
 * Desc         : Creates an object with a special event other than a Trigger Object.
 * (e.g. milestone -displays UI when focused)
 * It is not synchronized with the server and is only used for client display. 
 *****************************************************************************/

#pragma once

#include "NtlSobActor.h"


class CNtlSobEventObject : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobEventObject, NTL_DEFAULT_MEMORY_POOL )

public:
	CNtlSobEventObject(VOID);
	virtual ~CNtlSobEventObject(VOID);

	virtual RwBool  Create(VOID);
	virtual void    Destroy(VOID);
	virtual void    Update( RwReal fElapsed );
	virtual void    HandleEvents( RWS::CMsg& pMsg );

protected:	
};
