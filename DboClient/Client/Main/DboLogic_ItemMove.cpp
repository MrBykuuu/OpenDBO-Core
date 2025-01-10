/*****************************************************************************
*File			: DboLogic_String.h
*Author	    	: Hong sungbock
*Copyright		: NTL Co., Ltd.
*Date			: 2007. 1. 12
*Abstract		: Gather logic related to item movement
*****************************************************************************
*Desc           : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboLogic.h"


// Shared
#include "NtlResultCode.h"
#include "ItemTable.h"
#include "ItemOptionTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Core
#include "NtlDebug.h"

// Sound
#include "GUISoundDefine.h"

// Presention
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobManager.h"
#include "NtlSobItemAttr.h"
#include "NtlInventory.h"
#include "NtlSobItemIcon.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlFSMStateBase.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

// Dbo
#include "DialogManager.h"
#include "ChatGui.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"
#include "IconMoveManager.h"
#include "WarehouseGui.h"
#include "TradeCart.h"


RwBool Logic_ItemMoveProc( SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace,
						   SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount/* = 1 */)
{
	std::string uiResult = "DST_ITEM_CAN_NOT_MOVE_THERE";

	if( eSrcPlace == PLACE_EQUIP )
	{
		if( eDestPlace == PLACE_EQUIP )
			uiResult = Logic_ItemMoveSubProcEquipToEquip( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcEquipToBagSlot( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcEquipToBagChild( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_SCOUTER_SLOT )
			uiResult = Logic_ItemMoveSubProcEquipToEquip( hSrcSerial, ucSrcSlotIdx, EQUIP_SLOT_TYPE_SCOUTER, uiStackCount );
		else if( eDestPlace == PLACE_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcEquipToWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx);
		else if( eDestPlace == PLACE_GUILD_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcEquipToGuildWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx );
	}
	else if( eSrcPlace == PLACE_BAGSLOT )
	{
		if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcBagSlotToBagSlot( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcBagSlotToBagChild( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcBagSlotToWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx);
		else if( eDestPlace == PLACE_GUILD_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcBagSlotToGuildWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx);
	}
	else if( eSrcPlace == PLACE_BAG )
	{
		if( eDestPlace == PLACE_EQUIP )
			uiResult = Logic_ItemMoveSubProcBagChildToEquip( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );		
		else if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcBagChildToBagSlot( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcBagChildToBagChild( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_SCOUTER_SLOT )
			uiResult = Logic_ItemMoveSubProcBagChildToEquip( hSrcSerial, ucSrcSlotIdx, EQUIP_SLOT_TYPE_SCOUTER, uiStackCount );
		else if( eDestPlace == PLACE_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcBagChildToWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount);
		else if( eDestPlace == PLACE_GUILD_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcBagChildToGuildWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount);		
	}
	else if( eSrcPlace == PLACE_SCOUTER_SLOT )
	{
		if( eDestPlace == PLACE_EQUIP )
			uiResult = Logic_ItemMoveSubProcEquipToEquip( hSrcSerial, EQUIP_SLOT_TYPE_SCOUTER, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcEquipToBagSlot( hSrcSerial, EQUIP_SLOT_TYPE_SCOUTER, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcEquipToBagChild( hSrcSerial, EQUIP_SLOT_TYPE_SCOUTER, hDestParentSerial, ucDestSlotIdx, uiStackCount );		
		else if( eDestPlace == PLACE_SCOUTER_SLOT )
		{
			GetIconMoveManager()->IconMoveEnd();
		}
		else if( eDestPlace == PLACE_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcEquipToWarehouse( hSrcSerial, EQUIP_SLOT_TYPE_SCOUTER, hDestParentSerial, ucDestSlotIdx );			
		else if( eDestPlace == PLACE_GUILD_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcEquipToGuildWarehouse( hSrcSerial, EQUIP_SLOT_TYPE_SCOUTER, hDestParentSerial, ucDestSlotIdx );		
	}
	else if( eSrcPlace == PLACE_QUESTBAG )
	{
		if( eDestPlace == PLACE_QUESTBAG )
			uiResult = Logic_ItemMoveSubProcQuestToQuest( ucSrcSlotIdx, ucDestSlotIdx );
	}
	else if( eSrcPlace == PLACE_WAREHOUSE )
	{
		if( eDestPlace == PLACE_EQUIP )
			uiResult = Logic_ItemMoveSubProcWarehouseToEquip( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx);
		else if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcWarehouseToBagSlot( hSrcSerial, ucSrcSlotIdx, ucDestSlotIdx, uiStackCount );
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcWarehouseToBagChild( hSrcSerial, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount);
		else if( eDestPlace == PLACE_SCOUTER_SLOT )
			uiResult = Logic_ItemMoveSubProcWarehouseToEquip( hSrcSerial, ucSrcSlotIdx, EQUIP_SLOT_TYPE_SCOUTER );
		else if( eDestPlace == PLACE_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcWarehouseToWarehouse( hSrcSerial, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount);
	}
	else if( eSrcPlace == PLACE_GUILD_WAREHOUSE )
	{
		if( eDestPlace == PLACE_EQUIP )
		uiResult = Logic_ItemMoveSubProcGuildWarehouseToEquip( hSrcSerial, eSrcPlace, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount);
		else if( eDestPlace == PLACE_BAGSLOT )
			uiResult = Logic_ItemMoveSubProcGuildWarehouseToBagSlot( hSrcSerial, eSrcPlace, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount);
		else if( eDestPlace == PLACE_BAG )
			uiResult = Logic_ItemMoveSubProcGuildWarehouseToBagChild( hSrcSerial, eSrcPlace, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount);
		else if( eDestPlace == PLACE_SCOUTER_SLOT )
			uiResult = Logic_ItemMoveSubProcGuildWarehouseToEquip( hSrcSerial, eSrcPlace, ucSrcSlotIdx, PLACE_EQUIP, hDestParentSerial, EQUIP_SLOT_TYPE_SCOUTER, uiStackCount );
		else if( eDestPlace == PLACE_GUILD_WAREHOUSE )
			uiResult = Logic_ItemMoveSubProcGuildWarehouseToGuildWarehouse( hSrcSerial, ucSrcSlotIdx, hDestParentSerial, ucDestSlotIdx, uiStackCount);
	}
	// Zenny movement related processing.
	else if( eSrcPlace == PLACE_SUB_BAG_ZENNY )
	{
		if( eDestPlace == PLACE_SUB_WAREHOUSE_ZENNY )
			uiResult = Logic_ItemMoveSubProcSubBagZennyToWarehouseZenny( hDestParentSerial, uiStackCount );
		else if( eDestPlace == PLACE_SUB_GUILD_WAREHOUSE_ZENNY )
			uiResult = Logic_ItemMoveSubProcSubBagZennyToGuildWarehouseZenny( hDestParentSerial, uiStackCount );
		else if( eDestPlace == PLACE_SUB_TRADECART_ZENNY )
			uiResult = Logic_ItemMoveSubProcSubBagZennyToTradeCartZenny( hDestParentSerial, uiStackCount );
		else if( eDestPlace == PLACE_SUB_MAILSYSTEM_ZENNY )
			uiResult = Logic_ItemMoveSubProcSubBagZennyToMailSystemZenny( uiStackCount );
	}
	else if( eSrcPlace == PLACE_SUB_WAREHOUSE_ZENNY )
	{
		if( eDestPlace == PLACE_SUB_BAG_ZENNY )
			uiResult = Logic_ItemMoveSubProcWareHouseZennyToSubBagZenny( uiStackCount );
	}
	else if( eSrcPlace == PLACE_SUB_GUILD_WAREHOUSE_ZENNY )
	{
		if( eDestPlace == PLACE_SUB_BAG_ZENNY )
			uiResult = Logic_ItemMoveSubProcGuildWareHouseZennyToSubBagZenny( uiStackCount );
	}
	else if( eSrcPlace == PLACE_SUB_TRADECART_ZENNY )
	{
		if( eDestPlace == PLACE_SUB_BAG_ZENNY )
			uiResult = Logic_ItemMoveSubProcTradeCartZennyToSubBagZenny( uiStackCount );
	}
	else if( eSrcPlace == PLACE_SUB_MAILSYSTEM_ZENNY )
	{
		if( eDestPlace == PLACE_SUB_BAG_ZENNY )
			uiResult = Logic_ItemMoveSubProcMailSystemZennyToSubBagZenny( uiStackCount );
	}
	
	if( uiResult == "" )
		return TRUE;
	else
	{
		GetAlarmManager()->AlarmMessage( uiResult );
		return FALSE;
	}
}

std::string Logic_ItemMoveSubProcEquipToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcEquipToEquip" );
	// Comparison conditions 
	// 1. Cancel if it is the same slot 
	if( ucSrcSlotIdx == ucDestSlotIdx )
	{
		GetIconMoveManager()->IconMoveEnd();
		NTL_RETURN( "" );
	}

	// 2. Only pairs (rings, earrings) can be moved.
	RwInt32 usSrcSlotFlag, usDestSlotFlag;
	usSrcSlotFlag = Logic_ConvertEquipSlotIdxToFlag( ucSrcSlotIdx );
	usDestSlotFlag= Logic_ConvertEquipSlotIdxToFlag( ucDestSlotIdx );
	if( !Logic_EquipSlotTypeFlagCheck( usSrcSlotFlag, usDestSlotFlag ) )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );

	RwUInt8 srcType, destType;
	srcType = (RwUInt8)Logic_ConvertEquipSlotIdxToType( ucSrcSlotIdx );
	destType = (RwUInt8)Logic_ConvertEquipSlotIdxToType( ucDestSlotIdx );

	// 3. Check each usage period.
	SERIAL_HANDLE hDestSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( ucDestSlotIdx );

	if( hSrcSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItemAttr* pSrcItemAttr = Logic_GetItemSobAttrFromSobSerial( hSrcSerial );
		DBO_ASSERT( pSrcItemAttr, "Logic_ItemMoveSubProcEquipToEquip : Src Object is NULL" );

		if( pSrcItemAttr->IsExpired() )
			NTL_RETURN( "DST_ITEM_CAN_NOT_EQUIP_EXPIRED_DURATION" );	
	}	

	if( hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItemAttr* pDestItemAttr = Logic_GetItemSobAttrFromSobSerial( hDestSerial );
		DBO_ASSERT( pDestItemAttr, "Logic_ItemMoveSubProcEquipToEquip : Dest Object is NULL" );

		if( pDestItemAttr->IsExpired() )
			NTL_RETURN( "DST_ITEM_CAN_NOT_EQUIP_EXPIRED_DURATION" );
	}
	
	if( Logic_IsStackMovable( hSrcSerial, hDestSerial, uiStackCount ) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemStackMoveReq( CONTAINER_TYPE_EQUIP, srcType, CONTAINER_TYPE_EQUIP, destType, (RwUInt8)uiStackCount ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( CONTAINER_TYPE_EQUIP, srcType, CONTAINER_TYPE_EQUIP, destType ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

std::string Logic_ItemMoveSubProcEquipToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcEquipToBag" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hDestBagItem = pInventory->GetBagItem( ucDestSlotIdx ); 

	// Comparison conditions 
	// 1. Compare whether the scouter is in use or has parts installed.
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcItem, "Invalid Src Item" );

	// 2. Cancel if DestSlot is empty
	if( hDestBagItem == INVALID_SERIAL_ID )
	{
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG" );
	}

	CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBagItem ) ); 
	// 3. Cancel if the bag¡¯s period limit has expired.
	CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
	if( pDestBagItemAttr->IsExpired() )
	{
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );
	}

	// 4. If the bag exists but there are no empty spaces, cancel.
	if( pDestBagItem->FullChild() )
	{
		NTL_RETURN( "DST_ITEM_BAG_FULL" );
	}		

	RwInt32 nEmptyBagChildSlot = pDestBagItem->FindEmptyChildSlot();

	NTL_RETURN( Logic_ItemMoveSubProcEquipToBagChild( hSrcSerial, ucSrcSlotIdx, hDestBagItem, (RwInt8)nEmptyBagChildSlot, uiStackCount ) );
}

std::string Logic_ItemMoveSubProcEquipToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcEquipToBagChild" );

	// Comparison conditions 
	// 1. Compare whether the scouter is in use or has parts installed.
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcItem, "Invalid Src Item" );

	// 2. Check if it is a net warehouse.
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwInt32 nBagSlotIdx = pInventory->FindBagSlot( hDestParentSerial );
	DBO_ASSERT( ( nBagSlotIdx >= 0 ), "Bag is not found in BagSlot" );	

	// 3. Cancel if the bag¡¯s usage period has expired.
	CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestParentSerial ) );
	DBO_ASSERT( pBagItem, "Invalid DestBagItem" );
	CNtlSobItemAttr* pBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pBagItem->GetSobAttr() );
	DBO_ASSERT( pBagItemAttr, "Invalid DestBagItemAttr" );
	if( pBagItemAttr->IsExpired() )
	{
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );
	}	

	// 4. If the DestSlot is not empty, check whether it is an item that can be moved to Equip.
	if( pBagItem->IsValidChild( ucDestSlotIdx ) )
	{
		CNtlSobItem* pDestItem = pBagItem->GetChildItem( ucDestSlotIdx );
		DBO_ASSERT( pDestItem, "Inccorect ucDestSlotIdx" );
		CNtlSobItemAttr* pDestItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestItem->GetSobAttr() );
		DBO_ASSERT( pDestItemAttr, "Inccorect GetSobAttr" );

		// peessi: If you can't equip it, the message will be that your bag is full.
		if( !pDestItem->IsEquipItem() )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );

		sITEM_TBLDAT* pDestItemTable = pDestItemAttr->GetItemTbl();
		DBO_ASSERT( pDestItemTable, "Equip ItemAttr has no ItemTable" );

		RwUInt16 usSrcSlotFlag = Logic_ConvertEquipSlotIdxToFlag( ucSrcSlotIdx );

		if( !Logic_EquipSlotTypeFlagCheck( usSrcSlotFlag, pDestItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );
	}	

	eCONTAINER_TYPE eBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)nBagSlotIdx );
	eEQUIP_SLOT_TYPE srcType = Logic_ConvertEquipSlotIdxToType( ucSrcSlotIdx );

	SERIAL_HANDLE hDestSerial = pBagItem->GetChildSerial( ucDestSlotIdx );

	if( Logic_IsStackMovable( hSrcSerial, hDestSerial, uiStackCount ) )
	{ 
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemStackMoveReq(CONTAINER_TYPE_EQUIP, (RwUInt8)srcType, (RwUInt8)eBagType, ucDestSlotIdx, (RwUInt8)uiStackCount ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq((RwUInt8)CONTAINER_TYPE_EQUIP, (RwUInt8)srcType, (RwUInt8)eBagType, ucDestSlotIdx ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

std::string Logic_ItemMoveSubProcEquipToWarehouse(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcEquipToWarehouse" );

	// 1. Compare whether the scouter is in use or has parts installed.
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcItem, "Invalid Src Item" );

	// src
	//sITEM_TBLDAT*pITEM_TBLDAT = Logic_GetItemDataFromSob(hSrcSerial);
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	DBO_ASSERT( pItemAttr, "Invalid Src ItemAttr" );

	if( Logic_IsCanSaveWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	SERIAL_HANDLE hCommonWarehouse = pWarehouse->GetSlotSerailID(NTL_COMMON_WAREHOUSE);
	if( hDestParentSerial == hCommonWarehouse &&
		Logic_IsCanSaveCommonWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in shared storage		
		NTL_RETURN( "DST_ITEM_CAN_NOT_SAVE_COMMON_WAREHOUSE" );
	}

	// Dest	
	eCONTAINER_TYPE eDestContainerType = Logic_ConverWarehouseIdxToContainderType(pWarehouse->FindWarehouseSlot(hDestParentSerial));

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, CONTAINER_TYPE_EQUIP, ucSrcSlotIdx, (RwUInt8)eDestContainerType, ucDestSlotIdx ) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcEquipToGuildWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcEquipToGuildWarehouse" );

	// 1. Compare whether the scouter is in use or has parts installed.
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcItem, "Invalid Src Item" );

	//sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hSrcSerial);
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	DBO_ASSERT( pItemAttr, "Invalid Src ItemAttr" );

	if( Logic_IsCanSaveWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

	// hDestParentSerial: received the index of the guild warehouse, not the handle 
	eCONTAINER_TYPE eDestContainerType = Logic_ConverGuildWarehouseIdxToContainderType((RwUInt8)hDestParentSerial);

	if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
		CONTAINER_TYPE_EQUIP, ucSrcSlotIdx, (RwUInt8)eDestContainerType, ucDestSlotIdx) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcBagSlotToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagToBag" );
	// Comparison conditions 
	// 1. Cancel if it is the same seat
	if( ucSrcSlotIdx == ucDestSlotIdx )
	{
		GetIconMoveManager()->IconMoveEnd();
		NTL_RETURN( "" );
	}

	// 2. If the bag you are trying to move is a basic bag, cancel. If the destination is a basic bag, cancel.
	if( ucSrcSlotIdx == 0 || ucDestSlotIdx == 0 )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BASICBAG" );

	CNtlSobItem* pSrcBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcBagItem, "Invalid SrcSerial" );
	CNtlSobItemAttr* pSrcBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcBagItem->GetSobAttr() );
	DBO_ASSERT( pSrcBagItemAttr, "Invalid SrcSerialAttr" );

	// 3. If the period of use of the bag you wish to move has expired, cancel.
	if( pSrcBagItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );

	// 4. Cancel if the bag you are trying to move is not empty.
	if( !pSrcBagItem->EmptyChild() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EMPTY_BAG" );	

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hDestBagSerial = pInventory->GetBagItem( ucDestSlotIdx );

	if( hDestBagSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBagSerial ) );
		DBO_ASSERT( pDestBagItem, "Invalid DestBag" );
		CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
		DBO_ASSERT( pDestBagItemAttr, "Invalid DestBagAttr" );

		// 5. Cancellation if the usage period of the bag at the destination has expired.
		if( pDestBagItemAttr->IsExpired() )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );		

		// 6. Cancellation if the usage period of the bag at the destination has expired.
		if( pDestBagItem->FullChild() )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );
	}

	if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( CONTAINER_TYPE_BAGSLOT, ucSrcSlotIdx, CONTAINER_TYPE_BAGSLOT, ucDestSlotIdx ) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcBagSlotToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagToBagChild" );
	// Comparison conditions 
	// 1. If the bag you want to move is a basic bag, cancel.
	if( ucSrcSlotIdx == 0 )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BASICBAG" );

	CNtlSobItem* pSrcBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcBagItem, "Invalid SrcSerial" );

	// 2. Cancel if the bag you are trying to move is not empty.
	if( !pSrcBagItem->EmptyChild() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EMPTY_BAG" );

	// 3. Cancel to move to self
	if( hDestParentSerial == hSrcSerial )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_THERE" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwInt32 nBagSlotIdx = pInventory->FindBagSlot( hDestParentSerial );
	DBO_ASSERT( ( nBagSlotIdx >= 0 ), "Bag is not found in BagSlot" );

	CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestParentSerial ) );
	NTL_ASSERT( pDestBagItem, "Invalid DestParentSerial" );
	CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
	DBO_ASSERT( pDestBagItemAttr, "Invalid DestBag ItemAttr" );

	// 5. Cancel if the bag you wish to move has expired.
	if( pDestBagItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );	

	// 6. If DestSlot is not empty and the item there is not a bag, cancel.
	if( pDestBagItem->IsValidChild( ucDestSlotIdx ) )
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( pDestBagItem->GetChildItem( ucDestSlotIdx ) );
		DBO_ASSERT( pDestItem, "Must be valid DestItem" );

		if( !pDestItem->IsBagItem() )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG" );
	}	

	eCONTAINER_TYPE eDestBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)pInventory->FindBagSlot( hDestParentSerial ) );
	NTL_ASSERT( eDestBagType != CONTAINER_TYPE_NONE, "Invalid DestSlot Index" );

	if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( CONTAINER_TYPE_BAGSLOT, ucSrcSlotIdx, (RwUInt8)eDestBagType, ucDestSlotIdx ) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcBagSlotToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagSlotToWarehouse" );

	// Src
	CNtlSobItem* pSrcBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcBagItem, "Invalid SrcSerial" );

	//sITEM_TBLDAT*pITEM_TBLDAT = Logic_GetItemDataFromSob(hSrcSerial);
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcBagItem->GetSobAttr() );
	if( Logic_IsCanSaveWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	SERIAL_HANDLE hCommonWarehouse = pWarehouse->GetSlotSerailID(NTL_COMMON_WAREHOUSE);
	if( hDestParentSerial == hCommonWarehouse &&
		Logic_IsCanSaveCommonWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in shared storage
		NTL_RETURN( "DST_ITEM_CAN_NOT_SAVE_COMMON_WAREHOUSE" );
	}	

	// Cancel if bag is not empty.
	if( !pSrcBagItem->EmptyChild() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EMPTY_BAG" );

	// Dest
	eCONTAINER_TYPE eDestContainerType = Logic_ConverWarehouseIdxToContainderType(pWarehouse->FindWarehouseSlot(hDestParentSerial));

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, CONTAINER_TYPE_BAGSLOT, ucSrcSlotIdx,
		(RwUInt8)eDestContainerType, ucDestSlotIdx) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcBagSlotToGuildWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagSlotToGuildWarehouse" );
	
	// Src
	CNtlSobItem* pSrcBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	DBO_ASSERT( pSrcBagItem, "Invalid SrcSerial" );

	//sITEM_TBLDAT*pITEM_TBLDAT = Logic_GetItemDataFromSob(hSrcSerial);
	
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcBagItem->GetSobAttr() );
	if( Logic_IsCanSaveWarehouse( pItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	// Cancel if bag is not empty.
	if( !pSrcBagItem->EmptyChild() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EMPTY_BAG" );

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

	// hDestParentSerial: received the index of the guild warehouse, not the handle 
	eCONTAINER_TYPE eDestContainerType = Logic_ConverGuildWarehouseIdxToContainderType((RwUInt8)hDestParentSerial);

	if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
		CONTAINER_TYPE_BAGSLOT, ucSrcSlotIdx, (RwUInt8)eDestContainerType, ucDestSlotIdx) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcBagChildToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagChildToEquip" );
	// Comparison conditions 
	// 1. Check whether the item can be accessed by DestSlot.
	// 2. Exchange if DestSlot is not empty
	// 3. If the item has expired, it cannot be equipped. 
	// 4. When the DestItem is a scouter, check and check the expiration date of the scouter.
	// peessi Worked only up to this point. Leave it to the server for now, and in case of failure, talk about the message again. Currently, durability is left to the server.
	
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Invalid hSrcSerial" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	NTL_ASSERT( pSrcItemAttr, "Have No Attr" );

	// 1. If the item has expired, it cannot be equipped. 
	if( pSrcItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_EQUIP_EXPIRED_DURATION" );

	sITEM_TBLDAT* pSrcItemTable = pSrcItemAttr->GetItemTbl();
	NTL_ASSERT( pSrcItemTable, "Have No Table" );

	RwInt32 usDestSlotFlag = Logic_ConvertEquipSlotIdxToFlag( ucDestSlotIdx );
	eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)pSrcItem->GetParentItemSlotIdx() );
	SERIAL_HANDLE hDestSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( ucDestSlotIdx );

	// 2. If Dest is a scouter and is already wearing a scouter, check points 3 and 4.
	if( ucDestSlotIdx == EQUIP_SLOT_TYPE_SCOUTER && hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestSerial ) );
		DBO_ASSERT( pDestItem, "Invalid DestItem" );

		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}
	else
	{
		// 5. Check item installation type.
		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}

	if( Logic_IsStackMovable( hSrcSerial, hDestSerial, uiStackCount ) )
	{
		NTL_RETURN("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
	}
	else
	{

		if (pSrcItemTable->byRestrictType != ITEM_RESTRICT_TYPE_NONE && pSrcItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_NONE)
		{
			// open dialog
			sMsgBoxData data;
			data.sMoveEquipInfo.hSerial = hSrcSerial;
			data.sMoveEquipInfo.bySrcPlace = (RwUInt8)eSrcBagType;
			data.sMoveEquipInfo.bySrcPos = ucSrcSlotIdx;
			data.sMoveEquipInfo.byDestPos = ucDestSlotIdx;
			data.sMoveEquipInfo.bBank = false;
			data.sMoveEquipInfo.bGuildBank = false;

			GetAlarmManager()->AlarmMessage("DST_REVERTED_MB_CONFIRM_ACTIVATE", FALSE, &data);

			NTL_RETURN("");
		}

		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( (RwUInt8)eSrcBagType, ucSrcSlotIdx, CONTAINER_TYPE_EQUIP, ucDestSlotIdx ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

std::string Logic_ItemMoveSubProcBagChildToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagChildToBag" );
	
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Invalid hSrcSerial" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	NTL_ASSERT( pSrcItemAttr, "Have No Attr" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hDestBagSerial = pInventory->GetBagItem( ucDestSlotIdx );
	RwInt32 nEmptyBagChildSlot = -1;

	// 1. Check whether DestSlot bag is present or not. 
	if( hDestBagSerial == INVALID_SERIAL_ID )
	{
		// 2. If the src item is a bag.
		if( pSrcItem->IsBagItem() )
		{
			// 3. Check the expiration date of the src bag.
			if( pSrcItemAttr->IsExpired() )
				NTL_RETURN( "DST_ITEM_CAN_NOT_EQUIP_EXPIRED_DURATION" );

			eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)( pSrcItem->GetParentItemSlotIdx() ) );

			if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( (RwUInt8)eSrcBagType, ucSrcSlotIdx, CONTAINER_TYPE_BAGSLOT, ucDestSlotIdx ) )
				NTL_RETURN( "" )	
			else
				NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
		}
		else
		{
			// 4. If it is not a bag and there is no bag at dest, cancel.
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG" );
		}
	}
	else
	{
		SERIAL_HANDLE hSrcBagSerial = pSrcItem->GetParentItemSerial();

		// 5. When the src bag and dest bag are the same. As if the same slot was selected.
		if( hSrcBagSerial == hDestBagSerial )
			NTL_RETURN( "" );

		CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBagSerial ) );
		NTL_ASSERT( pDestBagItem, "Invalid hDestBagSerial" );

		// 6. Cancellation when dest¡¯s bag is full.
		if( pDestBagItem->FullChild() )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );

		nEmptyBagChildSlot = pDestBagItem->FindEmptyChildSlot();

		// 7. In this case, the BagChildToBagChild sublogic. 
		NTL_RETURN( Logic_ItemMoveSubProcBagChildToBagChild( hSrcSerial, ucSrcSlotIdx, hDestBagSerial, (RwUInt8)nEmptyBagChildSlot, uiStackCount ) );	
	}	
}

std::string Logic_ItemMoveSubProcBagChildToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagChildToBagChild" );
	
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Invalid hSrcSerial" );
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	RwUInt8 ucSrcBagIdx = (RwUInt8)pSrcItem->GetParentItemSlotIdx();
	RwUInt8 ucDestBagIdx= (RwUInt8)pInventory->FindBagSlot( hDestParentSerial );

	// 1. Cancellation if same bag and same seat	
	if( ucSrcBagIdx == ucDestBagIdx && ucSrcSlotIdx == ucDestSlotIdx )
	{
		GetIconMoveManager()->IconMoveEnd();
		NTL_RETURN( "" );
	}		

	eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( ucSrcBagIdx );
	eCONTAINER_TYPE eDestBagType= Logic_ConvertBagIdxToContainerType( ucDestBagIdx );

	CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestParentSerial ) );
	DBO_ASSERT( pDestBagItem, "Invalid DestBagItem" );
	CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
	DBO_ASSERT( pDestBagItemAttr, "Invalid DestBagItemAttr" );
	SERIAL_HANDLE hDestSerial = pDestBagItem->GetChildSerial( ucDestSlotIdx );

	// 3. Inspection of expiration date of dest bag
	if( pDestBagItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );

	if( Logic_IsStackMovable( hSrcSerial, hDestSerial, uiStackCount ) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemStackMoveReq( (RwUInt8)eSrcBagType, ucSrcSlotIdx, (RwUInt8)eDestBagType, ucDestSlotIdx, (RwUInt8)uiStackCount ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq( (RwUInt8)eSrcBagType, ucSrcSlotIdx, (RwUInt8)eDestBagType, ucDestSlotIdx ) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}


std::string Logic_ItemMoveSubProcBagChildToWarehouse(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagChildToWarehouse" );

	// Src
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT(pItem, "Logic_ItemMoveSubProcBagChildToWarehouse, Not exist item : " << hSrcSerial);
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "Logic_ItemMoveSubProcBagChildToWarehouse, Not exist item attr : " << hSrcSerial);	

	if( Logic_IsCanSaveWarehouse( pSobItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	SERIAL_HANDLE hCommonWarehouse = pWarehouse->GetSlotSerailID(NTL_COMMON_WAREHOUSE);
	if( hDestParentSerial == hCommonWarehouse && 
		Logic_IsCanSaveCommonWarehouse( pSobItemAttr ) == FALSE )
	{
		// Items that cannot be stored in shared storage
		NTL_RETURN( "DST_ITEM_CAN_NOT_SAVE_COMMON_WAREHOUSE" );
	}

	RwUInt8 ucBagIdx = (RwUInt8)pItem->GetParentItemSlotIdx();
	NTL_ASSERT( ( ucBagIdx >= 0 ), "Bag is not found in BagSlot" );

	eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)ucBagIdx );

	// Dest
	eCONTAINER_TYPE eDestContainerType = Logic_ConverWarehouseIdxToContainderType(pWarehouse->FindWarehouseSlot(hDestParentSerial));

	// Warehouse NPC serial	
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	// for Logic_IsStackMovable
	CNtlSobItem* pDestParentItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hDestParentSerial));

	if( Logic_IsStackMovable(hSrcSerial, pDestParentItem->GetChildSerial(ucDestSlotIdx), uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMoveStack(hNPCHandle, (RwUInt8)eSrcBagType, ucSrcSlotIdx,
			(RwUInt8)eDestContainerType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else 
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, (RwUInt8)eSrcBagType, ucSrcSlotIdx,
			(RwUInt8)eDestContainerType, ucDestSlotIdx) )
			NTL_RETURN( "" )		
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

std::string Logic_ItemMoveSubProcBagChildToGuildWarehouse(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcBagChildToGuildWarehouse" );

	// Src
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT(pItem, "Logic_ItemMoveSubProcBagChildToGuildWarehouse, Not exist item : " << hSrcSerial);
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "Logic_ItemMoveSubProcBagChildToGuildWarehouse, Not exist item attr : " << hSrcSerial);	

	if( Logic_IsCanSaveWarehouse( pSobItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	RwUInt8 ucBagIdx = (RwUInt8)pItem->GetParentItemSlotIdx();
	NTL_ASSERT( ( ucBagIdx >= 0 ), "Bag is not found in BagSlot" );

	eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( (RwUInt8)ucBagIdx );

	// Dest
	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	// hDestParentSerial: received the index of the guild warehouse, not the handle 
	eCONTAINER_TYPE eDestContainerType = Logic_ConverGuildWarehouseIdxToContainderType((RwUInt8)hDestParentSerial);
	sGuildWarehouseSlot* pGuildWarehouseItem = pGuildWarehouse->GetItem((RwUInt8)hDestParentSerial, ucDestSlotIdx);

	if( Logic_IsStackMovable_to_GuildWarehouse(pGuildWarehouseItem, hSrcSerial, uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveStackReq(pGuildWarehouse->GetNPCHandle(),
			(RwUInt8)eSrcBagType, ucSrcSlotIdx, (RwUInt8)eDestContainerType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
			(RwUInt8)eSrcBagType, ucSrcSlotIdx, (RwUInt8)eDestContainerType, ucDestSlotIdx) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

std::string Logic_ItemMoveSubProcQuestToQuest( RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcQuestToQuest" );
	// Comparison conditions
	// 1. Excluding the same seats.
	if( ucSrcSlotIdx == ucDestSlotIdx )
	{
		GetIconMoveManager()->IconMoveEnd();
		NTL_RETURN( "" );
	}

	if( GetDboGlobal()->GetGamePacketGenerator()->SendQuestItemMoveReq( ucSrcSlotIdx, ucDestSlotIdx ) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcWarehouseToEquip( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcWarehouseToEquip" );

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	

	// Src
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSrcSerial));
	DBO_ASSERT( pSrcItem, "Invalid SrcItem" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	DBO_ASSERT( pSrcItemAttr, "Invalid SrcItemAttr" );
	sITEM_TBLDAT* pSrcItemTable = pSrcItemAttr->GetItemTbl();
	NTL_ASSERT( pSrcItemTable, "Have No Table" );

	RwUInt8 bySrcSlot = pWarehouse->FindWarehouseSlot(pSrcItem->GetParentItem()->GetSerialID());
	eCONTAINER_TYPE eSrcContainerType = Logic_ConverWarehouseIdxToContainderType(bySrcSlot);
	
	SERIAL_HANDLE hDestSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( ucDestSlotIdx );
	RwUInt32 usDestSlotFlag = Logic_ConvertEquipSlotIdxToFlag( ucDestSlotIdx );

	// 0. Currently, items that can be stored in the warehouse do not have a usage period limit. No need for testing.

	// 1. If Dest is a scouter and is already wearing a scouter, check points 2 and 3.
	if( ucDestSlotIdx == EQUIP_SLOT_TYPE_SCOUTER && hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestSerial ) );
		DBO_ASSERT( pDestItem, "Invalid DestItem" );

		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}
	else
	{
		// 5. Check item installation type.
		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}	

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	if (pSrcItemTable->byRestrictType != ITEM_RESTRICT_TYPE_NONE && pSrcItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_NONE)
	{
		// open dialog
		sMsgBoxData data;
		data.sMoveEquipInfo.hSerial = hSrcSerial;
		data.sMoveEquipInfo.bySrcPlace = (RwUInt8)eSrcContainerType;
		data.sMoveEquipInfo.bySrcPos = ucSrcSlotIdx;
		data.sMoveEquipInfo.byDestPos = ucDestSlotIdx;
		data.sMoveEquipInfo.bBank = true;
		data.sMoveEquipInfo.bGuildBank = false;
		data.sMoveEquipInfo.hNPCHandle = hNPCHandle;

		GetAlarmManager()->AlarmMessage("DST_REVERTED_MB_CONFIRM_ACTIVATE", FALSE, &data);

		NTL_RETURN("");
	}

	if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx, CONTAINER_TYPE_EQUIP, ucDestSlotIdx) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcWarehouseToBagSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcWarehouseToBagSlot" );

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	

	// Src
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Invalid hSrcSerial" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	NTL_ASSERT( pSrcItemAttr, "Have No Attr" );

	RwUInt8 bySrcSlot = pWarehouse->FindWarehouseSlot(pSrcItem->GetParentItem()->GetSerialID());
	eCONTAINER_TYPE eSrcContainerType = Logic_ConverWarehouseIdxToContainderType(bySrcSlot);

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hDestBagSerial = pInventory->GetBagItem( ucDestSlotIdx );
	RwInt32 nEmptyBagChildSlot = -1;

	// 1. Check whether DestSlot bag is present or not. 
	if( hDestBagSerial == INVALID_SERIAL_ID )
	{
		// 2. If the src item is a bag.
		if( pSrcItem->IsBagItem() )
		{
			// 3. There is no need to check the expiration date of the src bag. There is no limit to the period of use of items stored in warehouses.
			if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove( hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx, CONTAINER_TYPE_BAGSLOT, ucDestSlotIdx ) )
				NTL_RETURN( "" )	
			else
				NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
		}
		else
		{
			// 4. If it is not a bag and there is no bag at dest, cancel.
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG" );
		}
	}
	else
	{
		CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBagSerial ) );
		NTL_ASSERT( pDestBagItem, "Invalid hDestBagSerial" );

		// 4. Cancellation when dest¡¯s bag is full.
		if( pDestBagItem->FullChild() )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );

		nEmptyBagChildSlot = pDestBagItem->FindEmptyChildSlot();

		// 5. In this case, the WarehouseToBagChild sublogic. 
		NTL_RETURN( Logic_ItemMoveSubProcWarehouseToBagChild( hSrcSerial, ucSrcSlotIdx, PLACE_BAG, hDestBagSerial, ucDestSlotIdx, uiStackCount ) );
	}		
}

std::string Logic_ItemMoveSubProcWarehouseToBagChild( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcWarehouseToBag" );

	// Src
	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSrcSerial));
	RwUInt8 bySrcSlot = pWarehouse->FindWarehouseSlot(pSrcItem->GetParentItem()->GetSerialID());
	eCONTAINER_TYPE eSrcContainerType = Logic_ConverWarehouseIdxToContainderType(bySrcSlot);

	// Dest
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt8 ucDestBagIdx = (RwUInt8)pInventory->FindBagSlot( hDestParentSerial );
	eCONTAINER_TYPE eDestBagType = Logic_ConvertBagIdxToContainerType( ucDestBagIdx );

	CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestParentSerial ) );
	DBO_ASSERT( pDestBagItem, "Invalid DestBagItem" );
	CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
	DBO_ASSERT( pDestBagItemAttr, "Invalid DestBagItemAttr" );
	SERIAL_HANDLE hDestSerial = pDestBagItem->GetChildSerial( ucDestSlotIdx );

	// 3. Inspection of expiration date of dest bag
	if( pDestBagItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	if( Logic_IsStackMovable(hSrcSerial, hDestSerial, uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMoveStack(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx,
			(RwUInt8)eDestBagType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx,
			(RwUInt8)eDestBagType, ucDestSlotIdx) )
			NTL_RETURN( "" )		
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}	
}

std::string Logic_ItemMoveSubProcWarehouseToWarehouse( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace,
													SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount )
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcWarehouseToWarehouse" );	

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	

	// Src
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSrcSerial));	
	NTL_ASSERT(pSrcItem, "Logic_ItemMoveSubProcWarehouseToWarehouse, Not exist item : " << hSrcSerial);
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSrcItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "Logic_ItemMoveSubProcWarehouseToWarehouse, Not exist item attr : " << hSrcSerial);	

	if( Logic_IsCanSaveWarehouse( pSobItemAttr ) == FALSE )
	{
		// Items that cannot be stored in the warehouse
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE" );
	}

	SERIAL_HANDLE hCommonWarehouse = pWarehouse->GetSlotSerailID(NTL_COMMON_WAREHOUSE);
	if( hDestParentSerial == hCommonWarehouse &&
		Logic_IsCanSaveCommonWarehouse( pSobItemAttr ) == FALSE )
	{
		// Items that cannot be stored in shared storage		
		NTL_RETURN( "DST_ITEM_CAN_NOT_SAVE_COMMON_WAREHOUSE" );
	}

	RwUInt8 bySrcSlot = pWarehouse->FindWarehouseSlot(pSrcItem->GetParentItem()->GetSerialID());
	eCONTAINER_TYPE eSrcContainerType = Logic_ConverWarehouseIdxToContainderType(bySrcSlot);

	// Dest
	eCONTAINER_TYPE eDestContainerType = Logic_ConverWarehouseIdxToContainderType(pWarehouse->FindWarehouseSlot(hDestParentSerial));

	// Warehouse NPC Serial
	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();

	// Comparison conditions
	// 1. Excluding the same seats.
	if( ucSrcSlotIdx == ucDestSlotIdx && eSrcContainerType == eDestContainerType)
	{
		GetIconMoveManager()->IconMoveEnd();
		NTL_RETURN( "" );
	}

	// for Logic_IsStackMovable
	CNtlSobItem* pDestParentItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hDestParentSerial));

	if( Logic_IsStackMovable(hSrcSerial, pDestParentItem->GetChildSerial(ucDestSlotIdx), uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMoveStack(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx,
			(RwUInt8)eDestContainerType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx,
			(RwUInt8)eDestContainerType, ucDestSlotIdx) )
			NTL_RETURN( "" )		
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
	}
}

//RwBool Logic_ItemMoveSubProcWarehouseToScouterSlot( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx)
//{
//	NTL_FUNCTION( "Logic_ItemMoveSubProcWarehouseToScouterSlot" );
//
//	if( Logic_IsMovableEquipedScouter() == FALSE )
//		NTL_RETURN( FALSE );
//
//	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	
//
//	// src
//	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSrcSerial));
//	RwUInt8 bySrcSlot = pWarehouse->FindWarehouseSlot(pSrcItem->GetParentItem()->GetSerialID());
//	eCONTAINER_TYPE eSrcContainerType = Logic_ConverWarehouseIdxToContainderType(bySrcSlot);
//
//	// Ã¢°í NPC Serial
//	CWarehouseBarGui* pWarehouseBarGui = reinterpret_cast<CWarehouseBarGui*>(GetDialogManager()->GetDialog(DIALOG_WAREHOUSEBAR));
//	SERIAL_HANDLE hNPCHandle = pWarehouseBarGui->GetNPCSerial();
//
//	NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(hNPCHandle, (RwUInt8)eSrcContainerType, ucSrcSlotIdx,
//		CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_SCOUTER) );
//}

std::string Logic_ItemMoveSubProcGuildWarehouseToEquip(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcGuildWarehouseToEquip" );

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	sGuildWarehouseSlot* pGuildWarehouseItem = pGuildWarehouse->GetItem(hSrcSerial);
	NTL_ASSERT(pGuildWarehouseItem, "Logic_ItemMoveSubProcGuildWarehouseToBagSlot, Not exist guild warehouse item of handle << " << hSrcSerial);
	sITEM_TBLDAT* pSrcItemTable = pGuildWarehouseItem->pITEM_TBLDAT;
	NTL_ASSERT( pSrcItemTable, "Have No Table" );

	SERIAL_HANDLE hDestSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( ucDestSlotIdx );
	RwInt32 usDestSlotFlag = Logic_ConvertEquipSlotIdxToFlag( ucDestSlotIdx );

	// 0. Currently, items that can be stored in the warehouse do not have a usage period limit. No need for testing.
	// 1. If Dest is a scouter and is already wearing a scouter, check points 2 and 3.
	if( ucDestSlotIdx == EQUIP_SLOT_TYPE_SCOUTER && hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestSerial ) );
		DBO_ASSERT( pDestItem, "Invalid DestItem" );

		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}
	else
	{
		// 5. Check item installation type.
		if( !Logic_EquipSlotTypeFlagCheck( usDestSlotFlag, pSrcItemTable->dwEquip_Slot_Type_Bit_Flag ) )
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE" );
	}
	
	if (pSrcItemTable->byRestrictType != ITEM_RESTRICT_TYPE_NONE && pGuildWarehouseItem->byRestrictState == ITEM_RESTRICT_STATE_TYPE_NONE)
	{
		// open dialog
		sMsgBoxData data;
		data.sMoveEquipInfo.hSerial = hSrcSerial;
		data.sMoveEquipInfo.bySrcPlace = pGuildWarehouseItem->byServerPlace;
		data.sMoveEquipInfo.bySrcPos = pGuildWarehouseItem->byPos;
		data.sMoveEquipInfo.byDestPos = ucDestSlotIdx;
		data.sMoveEquipInfo.bBank = false;
		data.sMoveEquipInfo.bGuildBank = true;
		data.sMoveEquipInfo.hNPCHandle = pGuildWarehouse->GetNPCHandle();

		GetAlarmManager()->AlarmMessage("DST_REVERTED_MB_CONFIRM_ACTIVATE", FALSE, &data);

		NTL_RETURN("");
	}

	if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
		pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, CONTAINER_TYPE_EQUIP, ucDestSlotIdx) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcGuildWarehouseToBagSlot(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcGuildWarehouseToBagSlot" );

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	sGuildWarehouseSlot* pGuildWarehouseItem = pGuildWarehouse->GetItem(hSrcSerial);
	NTL_ASSERT(pGuildWarehouseItem, "Logic_ItemMoveSubProcGuildWarehouseToBagSlot, Not exist guild warehouse item of handle << " << hSrcSerial);
	sITEM_TBLDAT* pSrcItemTable = pGuildWarehouseItem->pITEM_TBLDAT;
	NTL_ASSERT( pSrcItemTable, "Have No Table" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hDestBagSerial = pInventory->GetBagItem( ucDestSlotIdx );
	RwInt32 nEmptyBagChildSlot = -1;

	// 1. Check whether DestSlot bag is present or not. 
	if( hDestBagSerial == INVALID_SERIAL_ID )
	{
		// 2. If the src item is a bag.
		if( pSrcItemTable->byItem_Type == ITEM_TYPE_BAG )
		{
			// 3. There is no need to check the expiration date of the src bag. There is no limit to the period of use of items stored in warehouses.
			if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
				pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, CONTAINER_TYPE_BAGSLOT, ucDestSlotIdx) )
				NTL_RETURN( "" )	
			else
				NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
		}
		else
		{
			// 4. If it is not a bag and there is no bag at dest, cancel.
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG" );
		}
	}
	else
	{
		CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBagSerial ) );
		NTL_ASSERT( pDestBagItem, "Invalid hDestBagSerial" );

		// 4. Cancellation when dest¡¯s bag is full.
		if( pDestBagItem->FullChild() )
			NTL_RETURN( "DST_ITEM_BAG_FULL" );

		nEmptyBagChildSlot = pDestBagItem->FindEmptyChildSlot();

		// 5. In this case, the BagChildToBagChild sublogic. 
		NTL_RETURN( Logic_ItemMoveSubProcGuildWarehouseToBagChild( hSrcSerial, eDestPlace, ucSrcSlotIdx, eDestPlace, hDestParentSerial, ucDestSlotIdx, uiStackCount ) );		
	}	
}

std::string Logic_ItemMoveSubProcGuildWarehouseToBagChild(SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx, EPlace eDestPlace, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcGuildWarehouseToBag" );

	// Src
	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	sGuildWarehouseSlot* pGuildWarehouseItem = pGuildWarehouse->GetItem(hSrcSerial);
	NTL_ASSERT(pGuildWarehouseItem, "Logic_ItemMoveSubProcGuildWarehouseToBagSlot, Not exist guild warehouse item of handle << " << hSrcSerial);

	// Dest
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt8 ucDestBagIdx = (RwUInt8)pInventory->FindBagSlot( hDestParentSerial );
	eCONTAINER_TYPE eDestBagType = Logic_ConvertBagIdxToContainerType( ucDestBagIdx );

	CNtlSobItem* pDestBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestParentSerial ) );
	DBO_ASSERT( pDestBagItem, "Invalid DestBagItem" );
	CNtlSobItemAttr* pDestBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestBagItem->GetSobAttr() );
	DBO_ASSERT( pDestBagItemAttr, "Invalid DestBagItemAttr" );
	SERIAL_HANDLE hDestSerial = pDestBagItem->GetChildSerial( ucDestSlotIdx );

	// 3. Inspection of expiration date of dest bag
	if( pDestBagItemAttr->IsExpired() )
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION" );

	if( Logic_IsStackMovable_from_GuildWarehouse(pGuildWarehouseItem, hDestSerial, uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveStackReq(pGuildWarehouse->GetNPCHandle(),
			pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, (RwUInt8)eDestBagType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
			pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, (RwUInt8)eDestBagType, ucDestSlotIdx) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
	}
}

std::string Logic_ItemMoveSubProcGuildWarehouseToGuildWarehouse(SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx, SERIAL_HANDLE hDestParentSerial, RwUInt8 ucDestSlotIdx, RwUInt32 uiStackCount)
{
	// Src
	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	sGuildWarehouseSlot* pGuildWarehouseItem = pGuildWarehouse->GetItem(hSrcSerial);
	RwUInt8 bySrcIndex = (RwUInt8)(pGuildWarehouseItem->byServerPlace - CONTAINER_TYPE_GUILD_FIRST);
	NTL_ASSERT(pGuildWarehouseItem, "Logic_ItemMoveSubProcGuildWarehouseToBagSlot, Not exist guild warehouse item of handle << " << hSrcSerial);

	// Same slots do not move
	if( bySrcIndex == hDestParentSerial )
	{
		if( ucSrcSlotIdx == ucDestSlotIdx )
		{
			GetIconMoveManager()->IconMoveEnd();
			NTL_RETURN( "" );
		}
	}

	// Dest
	// hDestParentSerial: received the index of the guild warehouse, not the handle 
	sGuildWarehouseSlot* pGuildWarehouseDestItem = pGuildWarehouse->GetItem((RwUInt8)hDestParentSerial, ucDestSlotIdx);
	eCONTAINER_TYPE eDestContainerType = Logic_ConverGuildWarehouseIdxToContainderType((RwUInt8)hDestParentSerial);

	if( Logic_IsStackMovable_from_GuildWarehouse(pGuildWarehouseItem, pGuildWarehouseDestItem, uiStackCount) )
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveStackReq(pGuildWarehouse->GetNPCHandle(),
			pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, (RwUInt8)eDestContainerType, ucDestSlotIdx, (RwUInt8)uiStackCount) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
	}
	else
	{
		if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pGuildWarehouse->GetNPCHandle(),
			pGuildWarehouseItem->byServerPlace, pGuildWarehouseItem->byPos, (RwUInt8)eDestContainerType, ucDestSlotIdx) )
			NTL_RETURN( "" )
		else
			NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Zenny moving related

std::string Logic_ItemMoveSubProcSubBagZennyToWarehouseZenny(SERIAL_HANDLE hNPCHandle, RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcSubBagMoneyToWarehouse" );

	// Put money into a warehouse (bank)
	if( GetDboGlobal()->GetGamePacketGenerator()->SendBankZenny(hNPCHandle, uiZenny, true) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
}

std::string Logic_ItemMoveSubProcSubBagZennyToGuildWarehouseZenny(SERIAL_HANDLE hNPCHandle, RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcSubBagMoneyToGuildWarehouse" );

	// Put money into the guild warehouse (bank)
	if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseZennyReq(hNPCHandle, uiZenny, true) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
}

std::string Logic_ItemMoveSubProcSubBagZennyToTradeCartZenny(SERIAL_HANDLE hTargetHandle, RwUInt32 uiZenny)
{	
	NTL_FUNCTION( "Logic_ItemMoveSubProcSubBagZennyToTradeCartZenny" );

	RwUInt32 uiTradeZenny = -1 * ( GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::USER_TRADE ) - uiZenny);

	if( GetDboGlobal()->GetGamePacketGenerator()->SendTradeZennyUpdateReq(hTargetHandle, uiTradeZenny) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
}

std::string Logic_ItemMoveSubProcSubBagZennyToMailSystemZenny(RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcSubBagZennyToMailSystemZenny" );

	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::MAIL_BOX ) - uiZenny );
	
	GetIconMoveManager()->IconMoveEnd();
	NTL_RETURN( "" );
}

std::string Logic_ItemMoveSubProcWareHouseZennyToSubBagZenny(RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcWareHouseZennyToSubBagZenny" );

	CWarehouseCommonGui* pWarehouseCommonGui = reinterpret_cast<CWarehouseCommonGui*>( GetDialogManager()->GetDialog(DIALOG_WAREHOUSE_COMMON) );

	// Withdraw money from the warehouse (bank)
	if( GetDboGlobal()->GetGamePacketGenerator()->SendBankZenny(pWarehouseCommonGui->GetNPCHandle(), uiZenny, false) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
}

std::string Logic_ItemMoveSubProcGuildWareHouseZennyToSubBagZenny(RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcGuildWareHouseZennyToSubBagZenny" );

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

	// Deduct money from guild warehouse (bank)
	if( GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseZennyReq(pGuildWarehouse->GetNPCHandle(), uiZenny, false) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )			
}

std::string Logic_ItemMoveSubProcTradeCartZennyToSubBagZenny(RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcTradeCartZennyToSubBagZenny" );

	RwUInt32 uiTradeZenny = -1 * (GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::USER_TRADE ) + uiZenny);
	CTradeCart* pGui = reinterpret_cast<CTradeCart*>( GetDialogManager()->GetDialog(DIALOG_TRADECART) );

	if( !pGui )
		NTL_RETURN( "DST_ZENNY_CAN_NOT_MOVE_TO_TRADECART_NOT_VALID" );

	RwUInt32 uiTargetSerial = pGui->GetTargetSerial();

	if( GetDboGlobal()->GetGamePacketGenerator()->SendTradeZennyUpdateReq(uiTargetSerial, uiTradeZenny) )
		NTL_RETURN( "" )
	else
		NTL_RETURN( "DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND" )
}

std::string Logic_ItemMoveSubProcMailSystemZennyToSubBagZenny(RwUInt32 uiZenny)
{
	NTL_FUNCTION( "Logic_ItemMoveSubProcMailSystemZennyToSubBagZenny" );

	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::MAIL_BOX ) + uiZenny );
	GetIconMoveManager()->IconMoveEnd();
	NTL_RETURN( "" );
}

