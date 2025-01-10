/*****************************************************************************
 *
 * File			: NtlSobProxy.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 4. 26	
 * Abstract		: Simulation object proxy base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_PROXY_DEF_H__
#define __NTL_SOB_PROXY_DEF_H__




class CNtlPLEntity;
class CNtlPLEntityAlphaWeightBlend;
class CNtlSob;

class CNtlSobProxySystemEffect;
class CNtlSobProxySystemEffectAlphaBlend;
class CNtlSobProxySystemEffectColor;
class CNtlSobProxySystemEffectColorFlickering;
class CNtlSobProxySystemEffectScale;

// proxy component type
#define NTL_SLPROXY_COMP_EQUIP_ITEM			0x00000001

#define NTL_TARGET_MARK_OFFSET				1.0f
#define NTL_ATTACK_MARK_OFFSET				1.0f
#define NTL_QUEST_MARK_OFFSET				1.0f

#define NTL_NAMEBOX_OFFSET					0.5f
#define NTL_DAMAGEBOX_OFFSET				0.2f

#define NTL_INK_THICKNESS					1.2f

#define NTL_SHADOW_GRAY_MIN					175
#define NTL_SHADOW_GRAY_MAX					200

// proxy system flag
#define NTL_PROXYSYSTEM_NOT_VISIBLE			0x00000001		//Used when the GM is not visible to other players.
#define NTL_PROXYSYSTEM_LP_VISIBLE			0x00000002		// LP GUI on/off 


#endif