#include "precomp_ntlsimulation.h"
#include "NtlWareHouse.h"

#include "NtlDebug.h"
#include "ItemTable.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobManager.h"

#include "NtlSLLogic.h"

CNtlWarehouse::CNtlWarehouse()
:m_uiZenny(0)
,m_bHaveCommonWarehouse(FALSE)
,m_bRecieveData_from_Server(FALSE)
{	
	for(RwInt32 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
		m_hWareHouseSlot[i] = INVALID_SERIAL_ID;
}

CNtlWarehouse::~CNtlWarehouse()
{

}

void CNtlWarehouse::Destroy(void)
{
	NTL_FUNCTION("CNtlWarehouse::Destroy");

	NTL_RETURNVOID();
}

SERIAL_HANDLE CNtlWarehouse::GetSlotSerailID(RwUInt8 bySlot)
{
	if( FALSE == Logic_IsUsableIndex(bySlot, NTL_MAX_BANKSLOT_COUNT) )
		return INVALID_SERIAL_ID;

	return m_hWareHouseSlot[bySlot];
}

RwUInt8 CNtlWarehouse::FindWarehouseSlot(SERIAL_HANDLE hSerial)
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i )
	{
		if(m_hWareHouseSlot[i] == hSerial)
		{
			return i;
		}
	}

	return INVALID_BYTE;
}

RwUInt32 CNtlWarehouse::GetZenny()
{
	return m_uiZenny;
}

RwBool CNtlWarehouse::IsHaveCommonWarehouse()
{
	return m_bHaveCommonWarehouse;
}

RwBool CNtlWarehouse::IsRecieveData_from_Server()
{
	return m_bRecieveData_from_Server;
}

VOID CNtlWarehouse::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlWarehouse::HandleEvents");

	if(pMsg.Id == g_EventSobWarehouseUpdate)
	{
		SNtlEventSobWarehouseUpdate *pSobWarehouseUpdate = reinterpret_cast<SNtlEventSobWarehouseUpdate*>( pMsg.pData );

		switch(pSobWarehouseUpdate->uiMessageType)
		{
		case NESWUT_RECIEVE_DATA:
			{
				// I received warehouse data for the first time during the game.
				m_bRecieveData_from_Server = TRUE;
				break;
			}
		case NESWUT_WAREHOUSE_START:
			{
				// Opened a warehouse for the first time in the game.
				break;
			}
		case NESWUT_ADD_ZENNY:
			{
				// There are more Zennys in the warehouse
				m_uiZenny += pSobWarehouseUpdate->uiValue;
				break;
			}
		case NESWUT_SUB_ZENNY:
			{
				// Zenny has decreased in the warehouse.
				m_uiZenny -= pSobWarehouseUpdate->uiValue;
				break;
			}
		case NESWUT_CLOSE:
			{
				break;
			}
		}
	}
	else if(pMsg.Id == g_EventSobWarehouseItemCreate)
	{
		CreateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobWarehouseItemAdd)
	{
		AddEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobWarehouseItemDelete)
	{
		DeleteEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobWarehouseItemMove)
	{
		ItemMoveEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobWarehouseItemStackMove)
	{
		ItemStackMoveEventHandler(pMsg);
	}

	NTL_RETURNVOID();
}

CNtlSobItem* CNtlWarehouse::CreateItem(void *pParentSobItem, sITEM_PROFILE* pItemProfile)
{
	if( !pItemProfile )
	{
		DBO_FAIL("Null item profile pointer");
		return NULL;
	}

	void *pSobObj = CNtlSLEventGenerator::SobSlotItemCreate(pParentSobItem, SLCLASS_SLOT_ITEM, pItemProfile->handle, 
		pItemProfile->tblidx, pItemProfile->sOptionSet, pItemProfile->byPlace,
		pItemProfile->byPos, pItemProfile->byStackcount, pItemProfile->byRank,
		pItemProfile->byGrade, pItemProfile->byCurDur, pItemProfile->bNeedToIdentify,
		FALSE, pItemProfile->byBattleAttribute, pItemProfile->byRestrictState, pItemProfile->awchMaker,
		pItemProfile->byDurationType, pItemProfile->nUseStartTime, pItemProfile->nUseEndTime);   

	if( !pSobObj )
	{
		DBO_FAIL("Fail to create sob item");
		return NULL;
	}

	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(pSobObj); 
	pSobItem->SetOwnerID(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID()); 

	return pSobItem;
}

CNtlSobItem* CNtlWarehouse::CreateItem(void *pParentSobItem, SNtlEventSobItemAdd *pSobItemAdd)
{
	if( !pSobItemAdd )
	{
		DBO_FAIL("Null pointer");
		return NULL;
	}

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	void *pSobObj = CNtlSLEventGenerator::SobSlotItemCreate(pParentSobItem, SLCLASS_SLOT_ITEM, pSobItemAdd->hItemSerialId, 
		pSobItemAdd->uiItemTblId, pSobItemAdd->sOptionSet, pSobItemAdd->byPlace,
		pSobItemAdd->bySlotIdx, pSobItemAdd->byStackCount, pSobItemAdd->byRank,
		pSobItemAdd->byGrade, pSobItemAdd->byDur, pSobItemAdd->bNeedToIdentify, 
		FALSE, pSobItemAdd->byBattleAttribute, pSobItemAdd->byRestrictState, pSobItemAdd->wszMaker,
		pSobItemAdd->byDurationType, pSobItemAdd->StartTime, pSobItemAdd->EndTime);   

	if( !pSobObj )
	{
		DBO_FAIL("Fail to create sob item");
		return NULL;
	}

	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(pSobObj); 
	pSobItem->SetOwnerID(pSobAvatar->GetSerialID()); 

	return pSobItem;
}


void CNtlWarehouse::CreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlWarehouse::CreateEventHandler");

	SNtlEventSobWarehouseItemCreate* pSobItemCreate = reinterpret_cast<SNtlEventSobWarehouseItemCreate*>(pMsg.pData);
	sITEM_PROFILE* pItemProfile = reinterpret_cast<sITEM_PROFILE*>(pSobItemCreate->pData);

	for(RwInt32 i = 0; i < pSobItemCreate->byItemCount ; ++i)
	{
		RwUInt8 byPlace = pItemProfile[i].byPlace;
		if(byPlace == CONTAINER_TYPE_BANKSLOT)
		{
			if( FALSE == Logic_IsUsableIndex(pItemProfile[i].byPos, NTL_MAX_BANKSLOT_COUNT) )
			{
				DBO_FAIL("CONTAINER_TYPE_BANKSLOT Bag Slot idx is invalid : " << pItemProfile[i].byPos);
				NTL_RETURNVOID();
			}


			CreateItem(NULL, &pItemProfile[i]);

			m_hWareHouseSlot[pItemProfile[i].byPos] = pItemProfile[i].handle;

			RwInt8 bySlot = (RwInt8)(pItemProfile[i].byPos + CONTAINER_TYPE_BANK1);
			if(bySlot == CONTAINER_TYPE_BANK4)
			{
				// Purchased a shared warehouse
				m_bHaveCommonWarehouse = TRUE;
			}
			else if(bySlot >= CONTAINER_TYPE_BANK1 && bySlot <= CONTAINER_TYPE_BANK3)
			{
				// Purchased a general warehouse
			}

			// Notify the client that a warehouse has been purchased			
			CNtlSLEventGenerator::SobUpdate(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), EVENT_AIUT_WAREHOUSE, NESWUT_BUY_SLOT);
		}
		else if(byPlace >= CONTAINER_TYPE_BANK1 && byPlace <= CONTAINER_TYPE_BANK4)
		{
			RwUInt8 byIndex = (RwUInt8)(byPlace - CONTAINER_TYPE_BANK1);

			if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_BANKSLOT_COUNT) )
			{
				NTL_RETURNVOID();
			}

			CreateItem(GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[byIndex]), &pItemProfile[i]);
		}
	}

	NTL_RETURNVOID();
}

void CNtlWarehouse::AddEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlWarehouse::AddEventHandler");

	SNtlEventSobItemAdd *pSobItemAdd = reinterpret_cast<SNtlEventSobItemAdd*>( pMsg.pData );

	RwUInt8 byPlace = pSobItemAdd->byPlace;
	if(byPlace == CONTAINER_TYPE_BANK1)
	{	// Since one warehouse is given by default, this case is not actually implemented.
		CreateItem(GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[0]), pSobItemAdd);
	}
	else if(byPlace == CONTAINER_TYPE_BANK2)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[1]), pSobItemAdd);
	}
	else if(byPlace == CONTAINER_TYPE_BANK3)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[2]), pSobItemAdd);
	}
	else if(byPlace == CONTAINER_TYPE_BANK4)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[3]), pSobItemAdd);
	}

	NTL_RETURNVOID();
}

void CNtlWarehouse::DeleteEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlWarehouse::DeleteEventHandler");

	SNtlEventSobItemDelete *pSobItemDelete = reinterpret_cast<SNtlEventSobItemDelete*>( pMsg.pData );

	if( pSobItemDelete->byPlace == CONTAINER_TYPE_BANKSLOT )
	{
		if( FALSE == Logic_IsUsableIndex(pSobItemDelete->bySlotIdx, NTL_MAX_BANKSLOT_COUNT) )
		{
			DBO_FAIL("Invalid index : " << pSobItemDelete->bySlotIdx);
			NTL_RETURNVOID();
		}

		m_hWareHouseSlot[pSobItemDelete->bySlotIdx] = INVALID_SERIAL_ID;
	}
	else if( pSobItemDelete->byPlace > CONTAINER_TYPE_BANK_FIRST && pSobItemDelete->byPlace <= CONTAINER_TYPE_BANK_LAST )
	{
		CNtlSLEventGenerator::SobDelete( pSobItemDelete->hItemSerialId );

		// delete for parent bag
		RwUInt8 bySrcParentSlotIdx = (RwUInt8)(pSobItemDelete->byPlace - CONTAINER_TYPE_BANK1);

		if( FALSE == Logic_IsUsableIndex(bySrcParentSlotIdx, NTL_MAX_BANKSLOT_COUNT) )
		{
			DBO_FAIL("Invalid index : " << bySrcParentSlotIdx);
			NTL_RETURNVOID();
		}

		CNtlSobItem* pSrcParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[bySrcParentSlotIdx]) );
		if( !pSrcParentBagSobItem )
		{
			DBO_FAIL("src parent item instance is null !!!. src item handle is" << m_hWareHouseSlot[bySrcParentSlotIdx]);
			NTL_RETURNVOID();
		}
		
		pSrcParentBagSobItem->SetChildSerial(pSobItemDelete->bySlotIdx, INVALID_SERIAL_ID);
	}

	NTL_RETURNVOID();
}

void CNtlWarehouse::ItemMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlWarehouse::ItemMoveEventHandler");

	SNtlEventSobItemMove *pItemMove = reinterpret_cast<SNtlEventSobItemMove*>(pMsg.pData); 

	// source item handle event
	CNtlSobItem *pSrcSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pItemMove->hSrcSerial) );
	if( !pSrcSobItem )
	{
		DBO_FAIL("source item instance is null !!!. src item handle : " << pItemMove->hSrcSerial);
		NTL_RETURNVOID();
	}

	if(pItemMove->byDestPlace > CONTAINER_TYPE_BANK_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BANK_LAST)
	{	
		// When putting an item into the warehouse

		RwUInt8 byDestParentSlotIdx = (RwUInt8)(pItemMove->byDestPlace - CONTAINER_TYPE_BANK1);

		if( FALSE == Logic_IsUsableIndex(byDestParentSlotIdx, NTL_MAX_BANKSLOT_COUNT) )
		{
			DBO_FAIL("Invalid index : " << byDestParentSlotIdx);
			NTL_RETURNVOID();
		}

		CNtlSobItem *pDestParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hWareHouseSlot[byDestParentSlotIdx] ) );
		if( !pDestParentBagSobItem )
		{
			DBO_FAIL("dest parent item instance is null !!!. dest item handle : " << m_hWareHouseSlot[byDestParentSlotIdx]);
			NTL_RETURNVOID();
		}		

		// Go to source.
		pSrcSobItem->SetParentItemSerial(m_hWareHouseSlot[byDestParentSlotIdx]);
		pDestParentBagSobItem->SetChildSerial(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);

		if(pItemMove->bySrcPlace > CONTAINER_TYPE_BANK_FIRST && pItemMove->bySrcPlace <= CONTAINER_TYPE_BANK_LAST)
		{				
			RwUInt8 bySrcParentSlotIdx = (RwUInt8)(pItemMove->bySrcPlace - CONTAINER_TYPE_BANK1);

			if( FALSE == Logic_IsUsableIndex(bySrcParentSlotIdx, NTL_MAX_BANKSLOT_COUNT) )
			{
				DBO_FAIL("Invalid index : " << bySrcParentSlotIdx);
				NTL_RETURNVOID();
			}

			CNtlSobItem* pSrcParentBagSobItem = reinterpret_cast<CNtlSobItem*>((GetNtlSobManager()->GetSobObject(m_hWareHouseSlot[bySrcParentSlotIdx]) ) );
			if( !pSrcParentBagSobItem )
			{
				DBO_FAIL("src parent item instance is null !!!. src item handle is " << m_hWareHouseSlot[bySrcParentSlotIdx]);
				NTL_RETURNVOID();
			}

			// move to dest.															
			if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
			{
				CNtlSobItem* pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
				if( !pDestSobItem )
				{
					DBO_FAIL("Dest item instance is null !!!. dest item handle is " << pItemMove->hDestSerial);
					NTL_RETURNVOID();
				}

				pDestSobItem->SetParentItemSerial(pSrcParentBagSobItem->GetSerialID());				
			}			

			pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}
		else if(pItemMove->bySrcPlace == CONTAINER_TYPE_BAGSLOT)
		{	
			// move to dest.
			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
			pInventory->SetBagItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}
		else if(pItemMove->bySrcPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->bySrcPlace <= CONTAINER_TYPE_BAG_LAST)
		{	
			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();			
			RwUInt8 bySrcParentSlotIdx = (RwUInt8)(pItemMove->bySrcPlace - CONTAINER_TYPE_BAG1);
			SERIAL_HANDLE hSrcItemHandle = pInventory->GetBagItem(bySrcParentSlotIdx);
			CNtlSobItem *pSrcParentBagSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSrcItemHandle) );
			if( !pSrcParentBagSobItem )
			{
				DBO_FAIL("src parent item instance is null !!!. src item handle is " << hSrcItemHandle);
				NTL_RETURNVOID();
			}

			// move to dest.
			if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
			{
				CNtlSobItem *pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
				if( !pDestSobItem )
				{
					DBO_FAIL("Dest item instance is null !!!. dest item handle is " << pItemMove->hDestSerial);
					NTL_RETURNVOID();
				}

				pDestSobItem->SetParentItemSerial(pSrcParentBagSobItem->GetSerialID());				
			}	

			pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}
		else if(pItemMove->bySrcPlace == CONTAINER_TYPE_EQUIP)
		{	
			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

			// move to dest.						
			if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
			{
				CNtlSobItem *pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
				if( !pDestSobItem )
				{
					DBO_FAIL("Dest item instance is null !!!. dest item handle is " << pItemMove->hDestSerial);
					NTL_RETURNVOID();
				}

				pDestSobItem->SetParentItemSerial(INVALID_SERIAL_ID);
			}	

			pInventory->SetEquipItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}
	}
	else if(pItemMove->bySrcPlace > CONTAINER_TYPE_BANK_FIRST && pItemMove->bySrcPlace <= CONTAINER_TYPE_BANK_LAST)
	{
		// When moving items from a warehouse to another container
		
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		RwUInt8 bySrcParentSlotIdx = (RwUInt8)(pItemMove->bySrcPlace - CONTAINER_TYPE_BANK1);

		if( FALSE == Logic_IsUsableIndex(bySrcParentSlotIdx, NTL_MAX_BANKSLOT_COUNT) )
		{
			DBO_FAIL("Invalid index : " << bySrcParentSlotIdx);
			NTL_RETURNVOID();
		}

		CNtlSobItem *pSrcParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hWareHouseSlot[bySrcParentSlotIdx] ) );
		if( !pSrcParentBagSobItem )
		{
			DBO_FAIL("source parent item instance is null !!!. src item handle is " << m_hWareHouseSlot[bySrcParentSlotIdx]);
			NTL_RETURNVOID();
		}

		// move to dest.
		if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
		{
			CNtlSobItem* pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
			if( !pDestSobItem )
			{
				DBO_FAIL("Dest item instance is null !!!. dest item handle is " << pItemMove->hDestSerial );
				NTL_RETURNVOID();
			}

			pDestSobItem->SetParentItemSerial(pSrcParentBagSobItem->GetSerialID());
		}

		pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);

		if(pItemMove->byDestPlace == CONTAINER_TYPE_BAGSLOT)
		{
			// Go to source.
			pSrcSobItem->SetParentItemSerial(INVALID_SERIAL_ID);
			pInventory->SetBagItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);			
		}
		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			// Go to source.
			RwUInt8 byDestParentSlotIdx = (RwUInt8)(pItemMove->byDestPlace - CONTAINER_TYPE_BAG1);
			SERIAL_HANDLE hDestItemHandle = pInventory->GetBagItem(byDestParentSlotIdx);
			CNtlSobItem *pDestParentBagSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject( hDestItemHandle ) );
			if( !pSrcParentBagSobItem )
			{
				DBO_FAIL("dest parent item instance is null !!!. dest item handle is " << hDestItemHandle);
				NTL_RETURNVOID();
			}

			pSrcSobItem->SetParentItemSerial(pInventory->GetBagItem(byDestParentSlotIdx));
			pDestParentBagSobItem->SetChildSerial(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);			
		}
		else if(pItemMove->byDestPlace == CONTAINER_TYPE_EQUIP)
		{
			// Go to source.
			pSrcSobItem->SetParentItemSerial(INVALID_SERIAL_ID);
			pInventory->SetEquipItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
		}
	}

	// source item handle event
	pSrcSobItem->HandleEvents(pMsg); 

	// dest item handle event
	if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
	{
		CNtlSobItem *pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pItemMove->hDestSerial) );
		if( !pDestSobItem )
		{
			DBO_FAIL("dest item instance is null !!!. dest item handle is " << pItemMove->hDestSerial);
			NTL_RETURNVOID();
		}

		pDestSobItem->HandleEvents(pMsg); 
	}

	NTL_RETURNVOID();
}

void CNtlWarehouse::ItemStackMoveEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION("CNtlWarehouse::ItemStackMoveEventHandler");

	SNtlEventSobItemStackMove *pItemStackMove = reinterpret_cast<SNtlEventSobItemStackMove*>(msg.pData); 

	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMove->hSrcSerial ) );
	if( !pSrcItem )
	{
		DBO_FAIL("SrcItem is Null!! item handle : " << pItemStackMove->hDestSerial);
		NTL_RETURNVOID();
	}	


	SERIAL_HANDLE hDestBag;

	if(pItemStackMove->byDestPlace >= CONTAINER_TYPE_BANK1 && pItemStackMove->byDestPlace <= CONTAINER_TYPE_BANK_LAST)
	{
		RwUInt32 uiIndex = (RwUInt32)pItemStackMove->byDestPlace - (RwUInt32)CONTAINER_TYPE_BANK1;

		if( FALSE == Logic_IsUsableIndex(uiIndex, NTL_MAX_BANKSLOT_COUNT) )
		{
			DBO_FAIL("Invalid index : " << uiIndex);
			NTL_RETURNVOID();
		}

		hDestBag = m_hWareHouseSlot[uiIndex];
	}
	else if(pItemStackMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemStackMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		hDestBag = pInventory->GetBagItem(pItemStackMove->byDestPlace - CONTAINER_TYPE_BAG1);
	}
	else
	{
		NTL_RETURNVOID();
	}

	CNtlSobItem* pDestBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBag ) );
	if( !pDestBag )
	{
		DBO_FAIL("Dest Bag is Null!!. dest item handle is " << hDestBag);
		NTL_RETURNVOID();
	}	

	// When divided in two
	if( pDestBag->GetChildSerial( pItemStackMove->byDestSlotIdx ) == INVALID_SERIAL_ID )
	{		
		CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
		sITEM_TBLDAT* pItemData = pSrcItemAttr->GetItemTbl();
		if( !pItemData )
		{
			DBO_FAIL("SrcItem Table is NULL!. srd item handle : " << pItemStackMove->hSrcSerial);
			NTL_RETURNVOID();
		}

		// Item Create
		if(pItemStackMove->byDestPlace >= CONTAINER_TYPE_BAGSLOT && pItemStackMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			CNtlSLEventGenerator::SobItemAdd( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
				pItemStackMove->hDestSerial, pItemData->tblidx, pSrcItemAttr->GetOptionSet(),
				pItemStackMove->byDestPlace, pItemStackMove->byDestSlotIdx, pItemStackMove->byDestStackCount,
				pSrcItemAttr->GetRank(), pSrcItemAttr->GetGrade(), pSrcItemAttr->GetDur(), pSrcItemAttr->GetMaxDur(), FALSE, pSrcItemAttr->GetBattleAttribute(),
				pSrcItemAttr->GetRestrictState(), pSrcItemAttr->GetMaker(), pSrcItemAttr->GetDurationType(),
				pSrcItemAttr->GetStartTime(), pSrcItemAttr->GetEndTime());
		}
		else
		{
			CNtlSLEventGenerator::SobWarehouseItemAdd( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
				pItemStackMove->hDestSerial, pItemData->tblidx, pSrcItemAttr->GetOptionSet(),
				pItemStackMove->byDestPlace, pItemStackMove->byDestSlotIdx, pItemStackMove->byDestStackCount,
				pSrcItemAttr->GetRank(), pSrcItemAttr->GetGrade(), pSrcItemAttr->GetDur(), pSrcItemAttr->GetMaxDur(), FALSE, pSrcItemAttr->GetBattleAttribute(),
				pSrcItemAttr->GetRestrictState(), pSrcItemAttr->GetMaker(), pSrcItemAttr->GetDurationType(),
				pSrcItemAttr->GetStartTime(), pSrcItemAttr->GetEndTime());
		}		

		pSrcItem->HandleEvents( msg );
	}

	//  When we merge into one
	else if( pItemStackMove->bySrcStackCount == 0 )
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMove->hDestSerial ) );
		if( !pDestItem )
		{
			DBO_FAIL("DestItem is Null!! item handle : " << pItemStackMove->hDestSerial);
			NTL_RETURNVOID();
		}

		// Item Delete
		if(pItemStackMove->bySrcPlace >= CONTAINER_TYPE_BAGSLOT && pItemStackMove->bySrcPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			CNtlSLEventGenerator::SobItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
				pItemStackMove->hSrcSerial, pItemStackMove->bySrcPlace, pItemStackMove->bySrcSlotIdx );
		}
		else
		{
			CNtlSLEventGenerator::SobWarehouseItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
				pItemStackMove->hSrcSerial, pItemStackMove->bySrcPlace, pItemStackMove->bySrcSlotIdx );
		}		

		// Stack application
		pDestItem->HandleEvents( msg );
	}

	// When both exist
	else
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMove->hDestSerial ) );
		if( !pDestItem )
		{
			DBO_FAIL("DestItem is Null!! item handle : " << pItemStackMove->hDestSerial);
			NTL_RETURNVOID();
		}

		pSrcItem->HandleEvents( msg );
		pDestItem->HandleEvents( msg );
	}

	NTL_RETURNVOID();
}