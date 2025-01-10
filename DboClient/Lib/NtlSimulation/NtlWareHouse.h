/*****************************************************************************
*
*File			: NtlWareHouse.h
*Author		: Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2006. 11. 20	
*Abstract		: Simulation Warehouse class
*****************************************************************************
*Desc         : 
*
*****************************************************************************/

#ifndef __NTL_WARE_HOUSE_H__
#define __NTL_WARE_HOUSE_H__

#include "NtlItem.h"
#include "ceventhandler.h"
#include "NtlSLDef.h"

class CNtlSobItem;
struct SNtlEventSobItemAdd;

class CNtlWarehouse : public RWS::CEventHandler
{
public:
	CNtlWarehouse();
	virtual ~CNtlWarehouse();

	void			Destroy(void);

	SERIAL_HANDLE	GetSlotSerailID(RwUInt8 bySlot);		///< Returns the handle of the slot
	RwUInt8			FindWarehouseSlot(SERIAL_HANDLE hSerial);///< Finds the slot that has the handle received as an argument.
	RwUInt32		GetZenny();								///< Return Jenny from the warehouse

	RwBool			IsHaveCommonWarehouse();				///< Find out if you have a shared warehouse	
	RwBool			IsRecieveData_from_Server();			///< Whether all warehouse data has been received from the server

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

protected:	
	CNtlSobItem* CreateItem(void *pParentSobItem, sITEM_PROFILE* pSobItemCreate);
	CNtlSobItem* CreateItem(void *pParentSobItem, SNtlEventSobItemAdd *pSobItemAdd);

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void DeleteEventHandler(RWS::CMsg &pMsg);
	void ItemMoveEventHandler(RWS::CMsg &pMsg);
	void ItemStackMoveEventHandler(RWS::CMsg &msg);

protected:
	SERIAL_HANDLE	m_hWareHouseSlot[NTL_MAX_BANKSLOT_COUNT];///< Warehouse slots
	RwUInt32		m_uiZenny;								///< Zenny

	RwBool			m_bHaveCommonWarehouse;					///< Ownership of shared warehouse
	RwBool			m_bRecieveData_from_Server;				///< Whether all warehouse data has been received from the server
};

#endif