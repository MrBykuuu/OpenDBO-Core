/*****************************************************************************
 *
 * File			: NtlMatExt
 * Author		: HongHoDong
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 11. 27	
 * Abstract		: NTl Mat Extension
 *****************************************************************************
 * Desc         : Material Extension
 *
 *****************************************************************************/
#ifndef _NTL_MATERIAL_EXT_CORE_H_
#define _NTL_MATERIAL_EXT_CORE_H_

#include <rwcore.h>
#include <rpworld.h>
#include "../Ntl_Plugin_Toon/toon/NtlMaterialExt.h"

RwBool		NtlMatExtPluginAttach(void);

RwBool		NtlMatExtSetMultiTexture(RpMaterial *pMaterial, RwTexture *pTexture);	/**<Set MultiTexture Currently only supports 1 image */
RwTexture  *NtlMatExtGetMultiTexture(RpMaterial *pMaterial);						/**<Get MultiTexture currently only supports 1 image */

void		NtlMatExtSetFlag(RpMaterial *pMaterial, RwUInt32 bFlag);				/**<Set Material property Flag Refer to NtlMaterialExt.h in Ntl_PluginToon */
RwUInt32	NtlMatExtGetFlag(RpMaterial *pMaterial);								/**<Get Material property Flag Refer to NtlMaterialExt.h in Ntl_PluginToon */

void		NtlMatExtSetPipeline(RpAtomic *pAtomic);

#endif