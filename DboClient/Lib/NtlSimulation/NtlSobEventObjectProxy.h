/*****************************************************************************
*
* File			: CNtlSobEventObjectProxy
* Author		: agebreak
* Copyright		: (аж)NTL
* Date			: 2007. 9. 4	
* Abstract		: Simulation Event object Proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#pragma once

class CNtlPLObject;

#include "NtlSLDef.h"
#include "NtlSobProxy.h"

class CNtlSobEventObjectProxy : public CNtlSobProxy
{
	DECLEAR_MEMORY_POOL( CNtlSobEventObjectProxy, NTL_DEFAULT_MEMORY_POOL )
public:

    CNtlSobEventObjectProxy(VOID);
    virtual ~CNtlSobEventObjectProxy(VOID);

    virtual RwBool Create(RwUInt32 uiCompType);
    virtual void Destroy(VOID);
    virtual void Update(RwReal fElapsed);
    virtual void HandleEvents( RWS::CMsg& msg );

    //-------------------------------------------------------------------------------
    // event handler
    //-------------------------------------------------------------------------------
    void SobCreateEventHandler(RWS::CMsg &pMsg);

    //---------------------------------------------------
    // Functions related to the main presentation entity 
    //---------------------------------------------------

    /**
    *Function related to transform.
    */

    virtual void SetPosition(const RwV3d *pPos);
    virtual void SetDirection(const RwV3d *pDir);
    virtual void SetAngleY(RwReal fAngle);        

    /**
    *Function regarding entity information.
    */
    CNtlPLEntity*			GetPLMainEntity(void);      ///< presentation main entity    
    virtual RwReal			GetPLEntityWidth(void);     ///< Gets the width of the pl entity.    
    virtual RwReal			GetPLEntityHeight(void);    ///< Gets the height of the pl entity.    
    virtual RwReal			GetPLEntityDepth(void);     ///< Gets the depth of pl entity.

protected:

	/** 
	*Create trigger object name.
	*/
	void    CreatePLObjectName(void);
	void    DeletePLObjectName(void);

protected:
        CNtlPLObject*           m_pPLObject;					/** trigger object */
        RwReal					m_fObjectWidth;					/** trigger object width */
        RwReal					m_fObjectHeight;				/** trigger object height */
        RwReal					m_fObjectDepth;					/** trigger object depth */        
};

inline CNtlPLEntity* CNtlSobEventObjectProxy::GetPLMainEntity(VOID) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pPLObject );
}
