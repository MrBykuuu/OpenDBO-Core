/*****************************************************************************
*
* File			: NtlPLPinking.h
* Author		: All Author
* Copyright	: (?)NTL
* Date			: 2005. 8. 11	
* Abstract		: Presentation picking.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLPICKING_H__
#define __NTL_PLPICKING_H__

#include <rwcore.h>
#include <rpworld.h>
#include <rtpick.h>
#include <rpcollis.h>
#include "NtlPLEntity.h"

// Picking
typedef struct _SWorldIntersect
{
	RpIntersection Intersection;
	RwReal fMinDist;
	RpAtomic *pAtomic;
	RwV3d vCollPos;
	RwV3d vNormal;
	RwBool bCollision;
}SWorldIntersect;

typedef struct _SWorldPickInfo
{
	RwBool bTerrainExclusion;		// Should we exclude terrain?
	RwBool bCharacterExclusion;		// Should character be excluded?
	RwReal fMinDist;				// Reference variables required for calculations
	RpAtomic *pAtomic;				// Reference variables required for calculations
	CNtlPLEntity *pPLEntity;		// If picking is successful, it is not NULL. If picking fails, it is NULL.
	RwV3d vPickPos;

	_SWorldPickInfo()
	{
		bTerrainExclusion	= FALSE;
		bCharacterExclusion = FALSE;
		pAtomic				= NULL;
		pPLEntity			= NULL;
	}

}SWorldPickInfo;

// Character
#define NTL_MAX_CHAR_INTERSECION	10

typedef struct _SWorldCharIntersectionData
{
	RpAtomic *pAtomic;
	RwBool bCollision;
	RwReal fMinDist;
	RwV3d vCollPos;
	RwV3d vNormal;
}SWorldCharIntersectionData;

typedef struct _SWorldCharIntersectionInfo
{
	RwUInt8 byAtomicCount;							// Number of object atoms with which characters collide.
	RpAtomic *pAtomic[NTL_MAX_CHAR_INTERSECION];	// atomic buffer.
}SWorldCharIntersectionInfo;

// Camera
#define NTL_MAX_CAMERA_INTERSECION	20

typedef struct _SWorldCameraRayAtomicData
{
	RwUInt8 byCollCount;
	RpAtomic *pAtomic[NTL_MAX_CAMERA_INTERSECION];
	RwBool bCollision[NTL_MAX_CAMERA_INTERSECION];
	RwReal fMinDist[NTL_MAX_CAMERA_INTERSECION];
	RwV3d vCollPos[NTL_MAX_CAMERA_INTERSECION];
}SWorldCameraLineAtomicData;

typedef struct _SWorldCameraIntersectionData
{
	RwBool bCollision;
	SWorldCameraLineAtomicData sLineAtomicData;
	RwBool bSphereCollision;
	RwReal fBackwardDist;
	RwBool bBackwardCollision;
}SWorldCameraIntersectionData;

/**
*Check whether collision is possible atomic
**/
RwBool IsCollisionAtomic(RpAtomic* pAtomic);

/**
*Do mouse picking.
*/
void Pick_WorldIntersectionLine(RwLine& Line, SWorldPickInfo& sPickInfo, RwReal fRayDist);

/**
*Optimized for collision logic that determines the height of the colliding height field.
*/
RwBool Collision_HeightFieldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
*Optimized for collision logic that determines the indoor height of collision.
*/
RwBool Collision_IndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
*Optimized for collision logic that calculates the height of colliding objects.
*/
RwBool Collision_WorldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
*Optimized for collision logic that determines the height of colliding mini indoor objects.
*/
RwBool Collision_MiniIndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
*Find colliding atoms among world objects.
*/
RwBool Collision_WorldIntersectionCharacter(RwBBox& box, SWorldCharIntersectionInfo& sCharInfo);

/**
*Find the RpWorldSector(Indoor) Triangle that collides during the world.
*/
RwBool Collision_WorldIntersectionWorldSector(RwLine& Line, SWorldCharIntersectionData& sCharDataInfo);

/**
*Find colliding atoms among world objects.
*/
RwBool Collision_WorldIntersectionCharacterData(RwLine& Line, RpAtomic *pAtomic, SWorldCharIntersectionData& sCharDataInfo);

/**
*Obtain camera collision information in space on the world.
*/
RwBool Collision_WorldIntersectionCameraData(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

/**
*Check whether there is a conflicting object. (For LensFlare. by agebreak)
*/
RwBool Collision_WorldIntersectionObject(const RwLine& line, SWorldPickInfo* pPickInfo );

/**
*Check whether there is a conflicting object. (for targeting)
*/
RwBool Collision_WorldIntersectionObjectForTarget(const RwLine& line, SWorldPickInfo* pPickInfo );

#endif