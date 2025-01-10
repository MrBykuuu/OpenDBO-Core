/*****************************************************************************
*File			: DboPackethandler_GUHoipoiMix.cpp
*Author		    : HaeSung, Cho
*Copyright		: NTL Co., Ltd.
*Date			: 2009. 1. 8
*Abstract		: Hoipoi mix capsule related handler
*****************************************************************************
*Desc           : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// Shared
#include "ItemTable.h"
#include "TextAllTable.h"

// Simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"

// Client
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"


void PacketHandler_GUHoipoiMixRecipeRegNfy( void* pPacket )
{
	sGU_HOIPOIMIX_RECIPE_REG_NFY* pNotify = (sGU_HOIPOIMIX_RECIPE_REG_NFY*)pPacket;

	GetAlarmManager()->FormattedAlarmMessage( "DST_HOIPOIMIX_REGISTER_RECIPE", FALSE, NULL, Logic_GetHoipoiMixRecipeName( pNotify->recipeTblidx ) );
	CNtlSLEventGenerator::HoipoiMixRecipeReg( pNotify->recipeTblidx );
}

void PacketHandler_GUHoipoiMixItemMakeRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_HOIPOIMIX_ITEM_CREATE_RES);

	sGU_HOIPOIMIX_ITEM_CREATE_RES* pResult = (sGU_HOIPOIMIX_ITEM_CREATE_RES*)pPacket;

	CDboEventGenerator::HoipoiMixItemMake( pResult->wResultCode, pResult->itemTblidx, pResult->objHandle, pResult->recipeTblidx,
		pResult->itemHandle );

	CNtlSLEventGenerator::TSHoipoiMix();
}

void PacketHandelr_GUHoipoiMixItemRecipeInfo( void* pPacket )
{
	sGU_HOIPOIMIX_ITEM_RECIPE_INFO* pInfo = (sGU_HOIPOIMIX_ITEM_RECIPE_INFO*)pPacket;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( pSobAvatar )
		CNtlSLEventGenerator::HoipoiMixItemRecipeInfo( pInfo->wCount, reinterpret_cast<void*>(pInfo->asRecipeData) );
	else
	{
		SAvatarRecipeInfo* pRecipeInfo = GetNtlSLGlobal()->GetAvatarRecipeInfo();
		memcpy( pRecipeInfo->asRecipeData, pInfo->asRecipeData, sizeof(sRECIPE_DATA) * pInfo->wCount);
		pRecipeInfo->wCount = pInfo->wCount;
	}
}

void PacketHandler_GUHoipoiMixItemMakeExpNfy( void* pPacket )
{
	sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY* pNotify = (sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY*)pPacket;

	CNtlSLEventGenerator::HoipoiMixItemMakeExp( pNotify->dwCurExp, pNotify->byCurLevel, pNotify->dwExpGained );
}
