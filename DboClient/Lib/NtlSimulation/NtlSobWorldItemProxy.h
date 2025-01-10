/*****************************************************************************
*
* File			: CNtlSobWorldItemProxy
* Author		: Peessi
* Copyright		: NTL Co., Ltd.
* Date			: 2006. 7. 12	
* Abstract		: Simulation World Item Proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_WORLDITEM_PROXY_H__
#define __NTL_SOB_WORLDITEM_PROXY_H__

class CNtlPLPlayerName;
class CNtlPLDecal;
class CNtlPLItem;

#include "NtlSLDef.h"
#include "NtlSobProxy.h"

class CNtlSobWorldItemProxy : public CNtlSobProxy
{
	DECLEAR_MEMORY_POOL( CNtlSobWorldItemProxy, NTL_DEFAULT_MEMORY_POOL )

private:

	CNtlPLItem*			m_pWorldItem;					/** Item **/
	
protected:

	/** 
	*Character creation
	*/
	VOID CreatePLWorldItem( RWS::CMsg& msg );
	VOID DeletePLWorldItem(VOID);
		
public:

	CNtlSobWorldItemProxy(VOID);
	virtual ~CNtlSobWorldItemProxy(VOID);

	virtual RwBool Create(RwUInt32 uiCompType);
	virtual VOID Destroy(VOID);
	virtual VOID HandleEvents( RWS::CMsg& msg );

public:

	//---------------------------------------------------
	// Functions related to the main presentation entity
	//---------------------------------------------------

	/**
	*Functions related to add and remove in the world.
	*/

	virtual void	AddWorld(void);
	virtual void	RemoveWorld(void);

	/**
	*Function related to transform.
	*/

	virtual void	SetPosition(const RwV3d *pPos);
	virtual void	SetDirection(const RwV3d *pDir);
	virtual void	SetAngleY(RwReal fAngle);


	// Animation
	void			SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f);

	// 
	virtual void	EnableVisible(RwBool bEnable);

	// presentation main entity
	CNtlPLEntity*	GetPLMainEntity(VOID);
};

inline CNtlPLEntity* CNtlSobWorldItemProxy::GetPLMainEntity(VOID) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pWorldItem );
}


#endif