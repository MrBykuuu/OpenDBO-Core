/*****************************************************************************
*
* File			: NtlPLDef.h
* Author		: HyungSuk, Jang
* Copyright	: (?)NTL
* Date			: 2005. 8. 01	
* Abstract		: Presentation layer standard define
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#ifndef __NTL_PLDEF_H__
#define __NTL_PLDEF_H__

#include "NtlCharacter.h"

enum ENtlPLEntityLayer
{
	PLENTITY_LAYER_INVALID,
	PLENTITY_LAYER_NONE,
	PLENTITY_LAYER_FOG,
	PLENTITY_LAYER_SKY,	
	PLENTITY_LAYER_RWWORLD,				// All entities included as renderware (woody1019, jhssugi)
	PLENTITY_LAYER_DECAL,
	PLENTITY_LAYER_PLAYERNAME,
	PLENTITY_LAYER_DAMAGEBOX,
	PLENTITY_LAYER_SOUND,
	PLENTITY_LAYER_EFFECT,
	PLENTITY_LAYER_LIGHT,
	PLENTITY_LAYER_SHORELINE,
	PLENTITY_LAYER_WEATHER,
	PLENTITY_LAYER_OCCLUDER,
	PLENTITY_LAYER_HEATHAZE,
	PLENTITY_LAYER_TOOL,
	PLENTITY_LAYER_GUI,
	PLENTITY_LAYER_MAX
};


enum ENtlPLEntityType
{
	PLENTITY_SKY,
	PLENTITY_WORLD,			// Woody1019
	PLENTITY_DUMMY_WORLD,	// Jhssugi
	PLENTITY_OCCLUDER,
	PLENTITY_WATER,
	PLENTITY_SHORELINE,
	PLENTITY_OBJECT,
	PLENTITY_CHARACTER,
	PLENTITY_ITEM,			//by HoDong
	PLENTITY_EFFECT,	// 10 in tw?
	PLENTITY_SPLINE,		// by agebreak
	PLENTITY_DECAL,			// by jhssugi
	PLENTITY_PLAYER_NAME,
	PLENTITY_DAMAGE_BOX,
	PLENTITY_LIGHT,
	// CZ-SS
	//PLENTITY_SPOT_LIGHT,
	PLENTITY_FOG,
	PLENTITY_GUI,
	PLENTITY_SOUND,
	PLENTITY_SOUND_BGM,
	PLENTITY_BLOOM,
	PLENTITY_SUN,			// by agebreak
	PLENTITY_SUN_PLANET,	// Added to distinguish class types due to FreeList problem.
	PLENTITY_PLANT,
	PLENTITY_WORLD_LIGHT,
	PLENTITY_WEATHER,
	PLENTITY_HEATHAZE,
	PLENTITY_LIGHT_OBJECT,	// 28 in tw?
	PLENTITY_GAME_PROPERTY,
	PLENTITY_DECAL_VOLUME,
	PLENTITY_DOJO,

	PLENTITY_END,
	PLENTITY_INVALID_TYPE = 0xffffffff
};

// PLEN => Abbreviation for presentation layer entity name.
#define NTL_PLEN_SKY			"PLSky"
#define NTL_PLEN_WORLD			"PLWorld"		// Woody1019
#define NTL_PLEN_DUMMY_WORLD	"PLDummyWorld"	// Jhssugi
#define NTL_PLEN_WATER			"PLWater"
#define NTL_PLEN_SHORELINE		"PLShoreLIne"
#define NTL_PLEN_OBJECT			"PLObject"
#define NTL_PLEN_CHARACTER		"PLCharacter"
#define NTL_PLEN_ITEM			"PLItem"
#define NTL_PLEN_EFFECT			"PLEffect"
#define NTL_PLEN_SPLINE			"PLSpline"
#define NTL_PLEN_DECAL			"PLDecal"
#define NTL_PLEN_PLAYER_NAME	"PLPlayerName"
#define NTL_PLEN_DAMAGE_BOX		"PLDamageBox"
#define NTL_PLEN_LIGHT			"PLLight"
// CZ-SS
//#define NTL_PLEN_SPOT_LIGHT		"PLSpotLight"
#define NTL_PLEN_FOG			"PLFog"
#define NTL_PLEN_GUI			"PLGui"
#define NTL_PLEN_SOUND			"PLSound"
#define NTL_PLEN_SOUND_BGM		"PLSoundBGM"
#define NTL_PLEN_BLOOM			"PLBloom"
#define NTL_PLEN_PLANT			"PLPlant"
#define NTL_PLEN_WORLD_LIGHT	"PLWorldLight"
#define NTL_PLEN_WEATHER		"PLWeather"
#define NTL_PLEN_HEATHAZE		"PLHeatHaze"
#define NTL_PLEN_LIGHT_OBJECT	"PLLightObject"
#define NTL_PLEN_GAME_PROPERTY	"PLGameProperty"
#define NTL_PLEN_DECAL_VOLUME	"PLDecalVolume"
#define NTL_PLEN_DOJO			"PLDojo"

/// entity flag
// CNtlPLEntity::SetFlags(RwUInt32 uiFlags); Set it using a function.
#define NTL_PLEFLAG_NOTUPDATE					0x00000001	// Set when update is not necessary among entities (e.g. object without animation).
#define NTL_PLEFLAG_FRUSTUMOUT_NOTUPDATE		0x00000002	// This setting is used when updates are not necessary for entities that are outside the camera frustum.
#define NTL_PLEFLAG_ALPHA						0x00000004	// Set the alpha flag on the entity.
#define NTL_PLEFLAG_PICKING						0x00000008	// Among the entities, there are those that can mouse pick (for speed...)
#define NTL_PLEFLAG_NOT_ADD_WORLD				0x00000010  // In the case of entities that do not need to be added to RpWorld.
#define NTL_PLEFLAG_NOT_PROPERTY_USED			0x00000020	// In the case of entities that do not use CNtlPLProperty at all.
#define NTL_PLEFLAG_NOT_VISIBLE					0x00000040	// entityµÈ¡ﬂ rendering on/off flags.
#define NTL_PLEFLAG_ATTACH						0x00000080	// The current entity is an attachable entity.
#define NTL_PLEFLAG_COLLISION					0x00000100  // Currently, the entity is capable of collision and is capable of going up.
#define NTL_PLEFLAG_WEIGHT_ELAPSED_TIME			0x00000200  // The current entity controls the update elapsed time rate.
#define NTL_PLEFLAG_SELECTION					0x00000400	// Determines whether or not objects with triggers are selected.
#define NTL_PLEFLAG_TRIGGER					    0x00000800	// Determine whether there is a trigger or not.
#define NTL_PLEFLAG_ALTERABLE					0x00001000	// Determine whether the object has dynamic deformation.
#define NTL_PLEFLAG_PORTAL						0x00002000	// Determine whether there is a portal function or not.
#define NTL_PLEFLAG_PECOLLISION					0x00004000	// Determine whether to use 2D collision mesh for path engine.
#define NTL_PLEFLAG_FADE                        0x00008000  // Fade application status flag
#define NTL_PLEFLAG_SHADOW                      0x00010000  // Shadow visibility flag for Entity
#define NTL_PLEFLAG_NEST_MEMBER                 0x00020000  // Nest Spawn Member
#define NTL_PLEFLAG_RESIZE_PICKING_BBOX         0x00040000  // Flag to dynamically bound the bounding box for picking
#define NTL_PLEFLAG_TOOL_PICK					0x00080000	// In tools, you may place derby objects while moving them. In this case, a conflict with the picker may occur, and this flag is used to prevent this. This is handled by NTLPicking so that picking occurs only when this flag is not present.
#define NTL_PLEFLAG_OBJECT_FORCE_VISIBLE		0x00100000	// Flag for adjusting Visible and Alpha values ??only from the outside without the object being affected by internal processing (Fade, Distance Culling...) (used to process the state of the trigger object) -Does not apply to map tools
#define NTL_PLEFLAG_SHADOW_PROP					0x00200000	// When to apply the terrain property.
#define NTL_PLEFLAG_DUMMY						0X00400000	// ex. Used when a GUI-only character is created.

// entity cull flags: The higher the priority, the lower the number of bits.
#define NTL_PLEFLAG_CULLED_PVS							0x00000001 // pvs test
#define NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT			0x00000002 // camera frustum out
#define NTL_PLEFLAG_CULLED_OCCLUDER						0x00000004 // occluder culled

// Pick order
#define NTL_PICK_ORDER_NORMAL					(0)
#define NTL_PICK_ORDER_HIGH						(1)


#define NTL_PLEFLAG_MINIMAP_LAYER_NONE				(0x00000000)
#define NTL_PLEFLAG_MINIMAP_LAYER_ONE				(0x00000001)
#define NTL_PLEFLAG_MINIMAP_LAYER_TWO				(0x00000002)

// Item

// Item's Grade Enum
enum ENtlPLItemGrade
{
	ITEM_GRADE_NONE = 0,
	ITEM_GRADE_1,
	ITEM_GRADE_2,
	ITEM_GRADE_3,
	ITEM_GRADE_4,
	ITEM_GRADE_5,
	ITEM_GRADE_6,
	ITEM_GRADE_7,
	ITEM_GRADE_8,
	ITEM_GRADE_9,
	ITEM_GRADE_10,
	ITEM_GRADE_11,
	ITEM_GRADE_12,
	ITEM_GRADE_13,
	ITEM_GRADE_14,
	ITEM_GRADE_15,
};

// emblem type
enum eEmblemType
{
	EMBLEMTYPE_A,
	EMBLEMTYPE_B,
	EMBLEMTYPE_C,

	NUM_EMBLEMTYPE
};

#define NTL_ITEM_ICON_SIZE				32	
#define NTL_ITEM_SMALL_ICON_SIZE		16	


// icon name
#define MONEYICON_NAME				"I_SYS_Zenny.png"
#define UNIDENTIFIED_ICON_NAME		"i_ucap.png"
#define SEALED_ICON_NAME			"i_lot_blu.png"
#define UNABLE_SLOT_NAME			"Unable_Slot.png"

// effect key name

#define _COMPILE_OPTION_USE_HEIGHT_MAP_				// Use Height Map


#define VISIBLE_BOUND		( 2 )
#define INTERPOLATE_OFFSET	( 1.0f )

#define rwID_WORLD_PLUG_IN_ID	( 0xF3 )

// Path
#define PATH_COLLISION_OBJECT   "Object\\col_mesh\\"
#define PATH_EFFECT_RESOURCE	"Effect\\"
#define PATH_EFFECT_TEXTURE		".\\Texture\\ntlwe\\Planet\\;.\\Texture\\Effect\\;.\\Texture\\character\\;.\\Texture\\Effect\\text\\;.\\Texture\\Effect\\qest_icon\\;"

// XML encryption password
#define ENC_DATA_PASSWORD				"@#)!agebreak"

// Rendering flag
#define dFLAG_EXCEPT_GUI			0x00000001

typedef unsigned int BITFLAG;                   // Type for bit flag



#endif


