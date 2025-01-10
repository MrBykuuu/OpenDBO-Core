/*****************************************************************************
 *
 *File			: NtlBehaviorBase.h
 *Author		: HyungSuk, Jang
 *Copyright	: (ÁÖ)NTL
 *Date			: 2006. 2. 13	
 *Abstract		: actor behavior data stuff
 *****************************************************************************
 *Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIORDATA_H__
#define __NTL_BEHAVIORDATA_H__

// Shared 
#include "NtlBattle.h"
#include "NtlSkill.h"

// PL
#include "NtlAnimEventData.h"

// Simulation
#include "NtlSLDef.h"

struct SNtlEventSobMoveSync;

////////////////////////////////////////////////////////////////////////////////////////////////
// attack behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// attack 

//Type of attack success.
#define NTL_ATTACK_RESULT_SUCCESS		0		// attack success
#define NTL_ATTACK_RESULT_BLOCK			1		// attack defense
#define NTL_ATTACK_RESULT_DODGE			2		// attack dodge
#define NTL_ATTACK_RESULT_KNOCKDOWN		3		// attack knockdown
#define NTL_ATTACK_RESULT_SLIDING		4		// °ø°Ý sliding ( NTL_ATTACK_RESULT_KNOCKDOWN | NTL_ATTACK_RESULT_BLOCK )
#define NTL_ATTACK_RESULT_HEALING		5		// heading
#define NTL_ATTACK_RESULT_BUFF			6		// heading
#define NTL_ATTACK_RESULT_STEAL			7		// Steals the attacked target's health. (This flag is for Projectile only)
#define NTL_ATTACK_RESULT_IMMUNE		8		// 
#define NTL_ATTACK_RESULT_RESIST		9		// 


// behavior form
#define NTL_ATTACK_NORMAL				0		// A common form of attack.
#define NTL_ATTACK_PUSH					1		// Push
#define NTL_ATTACK_KNOCKDOWN			2		// knock down
#define NTL_ATTACK_SLIDING				3		// Sliding
#define NTL_ATTACK_TOSS_UP				4		// attack in the air
#define NTL_ATTACK_TOSS_DOWN			5		// An attack that strikes downward from the air.

// form of physical attack
#define NTL_ATTACK_PHYSIC_MELEE			0		// Direct attack.
#define NTL_ATTACK_PHYSIC_PROJECTILE	1		// Projectile attack.
#define NTL_ATTACK_PHYSIC_SPELL			2		// magic attack

// logical form of attack
#define NTL_ATTACK_LOGIC_NORMAL			0		// Normal attack (default attack method)
#define NTL_ATTACK_LOGIC_SKILL			1		// skill attack

// attribute attack form
#define NTL_ATTACK_ATTR_NORMAL			0		// normal attack
#define NTL_ATTACK_ATTR_CRITICAL		1		// critical attack.


struct SAttackDamageStuff
{
    RwInt32 iDamage;        ///< Normal damage
    
    // DD/DOT        
    RwInt32 iAbsorbedDamage;
    RwInt32 iReflectedDamage;

	sDBO_LP_EP_RECOVERED		lpEpRecovered;	// attacked target recover

    // steel skill
    float						LP_EP_STEAL_fLpDamage;
    float						LP_EP_STEAL_fEpDamage;
    float						LP_EP_STEAL_fAbsorbedDamage;
    float						LP_EP_STEAL_fReflectedDamage;
    float						LP_EP_STEAL_fLpGained;
    float						LP_EP_STEAL_fEpGained;
};

typedef union _UAttackAttr
{
	struct 
	{
		RwUInt32 bitResult		: 4;	// Result of successful attack.
		RwUInt32 bitPhysic		: 2;	// form of physical attack
		RwUInt32 bitBehavior	: 4;	// behavior form
		RwUInt32 bitLogic		: 4;	// logical form of attack
		RwUInt32 bitCritical	: 1;	// attribute attack form
		RwUInt32 bitChainAttack : 1;    // chain attack
		RwUInt32 bitHitSeq		: 4;	// hit sequence
		RwUInt32 bitPowerEffect : 1;	// power effect
		RwUInt32 bitBlocking	: 8;	// Type of blocking mode.
		RwUInt32 bitReserved	: 5;        
	};
	RwUInt32 uiFlags;

}UAttackAttr; 

struct SHitStuff
{
	DECLEAR_MEMORY_POOL(SHitStuff, NTL_LARGE_MEMORY_POOL);

public:

	UAttackAttr			uiAttr;									// All attributes of the attack
	SERIAL_HANDLE		hAttackerSerialId;						// attacker serial id
	SERIAL_HANDLE		hDefenderSerialId;						// defender serial id
	SAttackDamageStuff	sDamageStuff;							// damage stuff
	RwUInt32			uiAttrLogicTblId;						// Weapon attack: item id, skill attack: skill idRw
	RwUInt32			uiProjectileBehaviorId;					// If the projectile exists, set the ID.
	RwUInt8				byShotType;								// projectile firing position (character, weapon, sub weapon)			
	RwUInt8				bySubWeaponShotFlag;					// When the projectile firing position is sub weapon. (character, weapon, sub weapon)
	RwReal				fSpeed;									// projectile speed
	RwBool				bLeftHand;								// left or right hand
	RwBool				bCreateHitStuffEffect;					// Should I create Projectile or Target Effect or Word Effect?
	RwChar				chBoneName[NTL_MAX_BONE_NAME];			// bone name
	RwChar				chProjEffName[NTL_MAX_EFFECT_NAME];		// projectile effect name
	
	RwChar				chTargetEffName[NTL_MAX_EFFECT_NAME];	// target effect name
	RwUInt8				byTargetAttach;							// Should I attach to the target?
	RwReal				fTargetLocHeight;						// Height value at which the target effect will explode (persent, 1.0 is 100%)
	RwUInt8				byTargetEffectDir;						// Target effect direction (see character animation event)
	RwChar				chWordEffName[NTL_MAX_EFFECT_NAME];		// target word effect name

	RwV3d				vPushDir;								// push direction.
	RwBool				bCameraShake;							// Shake the camera.
    RwReal              fShakeFactor;                           // camera shake factor
    RwReal              fShakeMaxHeight;                        // Maximum camera shake factor
	SYNCEVENT_HANDLE	hSyncEventId;
	void				*pProjExtData;
    RwBool              bHitSoundEcho;                          // Whether Hit Sound Echo is applied or not

	SHitStuff& operator = (const SHitStuff& stuff)
	{
		uiAttr.uiFlags			= stuff.uiAttr.uiFlags;
		hAttackerSerialId		= stuff.hAttackerSerialId;
	    hDefenderSerialId		= stuff.hDefenderSerialId;
		sDamageStuff			= stuff.sDamageStuff;
		uiAttrLogicTblId		= stuff.uiAttrLogicTblId;
		uiProjectileBehaviorId	= stuff.uiProjectileBehaviorId;
		byShotType				= stuff.byShotType;
		bySubWeaponShotFlag		= stuff.bySubWeaponShotFlag;
		fSpeed					= stuff.fSpeed;
		bLeftHand				= stuff.bLeftHand;
		bCreateHitStuffEffect	= stuff.bCreateHitStuffEffect;
		memcpy(chBoneName, stuff.chBoneName, NTL_MAX_BONE_NAME);
		memcpy(chProjEffName, stuff.chProjEffName, NTL_MAX_EFFECT_NAME);

		memcpy(chTargetEffName, stuff.chTargetEffName, NTL_MAX_EFFECT_NAME);
		byTargetAttach			= stuff.byTargetAttach;
		fTargetLocHeight		= stuff.fTargetLocHeight;
		byTargetEffectDir		= 0;
		memcpy(chWordEffName, stuff.chWordEffName, NTL_MAX_EFFECT_NAME);

		RwV3dAssignMacro(&vPushDir, &stuff.vPushDir);
		bCameraShake			= stuff.bCameraShake;
        fShakeFactor            = stuff.fShakeFactor;
        fShakeMaxHeight         = stuff.fShakeMaxHeight;
		hSyncEventId			= INVALID_SYNCEVENT_ID;
		pProjExtData			= stuff.pProjExtData;
        bHitSoundEcho           = stuff.bHitSoundEcho;
	}
	
	void Reset(void)
	{
		uiAttr.uiFlags					= 0;
		hAttackerSerialId				= INVALID_SERIAL_ID;
		hDefenderSerialId				= INVALID_SERIAL_ID;
		sDamageStuff.iDamage			= 0;
		sDamageStuff.iAbsorbedDamage	= 0;
		sDamageStuff.iReflectedDamage	= 0;
		sDamageStuff.lpEpRecovered.Reset();
		uiAttrLogicTblId				= 0;
		uiProjectileBehaviorId			= 0;
		byShotType						= 0;
		bySubWeaponShotFlag				= 0;
		fSpeed							= 10.0f;
		bLeftHand						= TRUE;
		bCreateHitStuffEffect			= TRUE;
		chBoneName[0]					= 0;
		chProjEffName[0]				= 0;
		chTargetEffName[0]				= 0;
		byTargetAttach					= FALSE;
		fTargetLocHeight				= 0.7f;
		chWordEffName[0]				= 0;
		vPushDir.x						= 0.0f;
		vPushDir.y						= 0.0f;
		vPushDir.z						= 0.0f;
		bCameraShake					= FALSE;
        fShakeFactor                    = CAMERA_SHAKE_FACTOR;
        fShakeMaxHeight                 = CAMERA_SHAKE_MAX_HEIGHT_FACTOR;
		pProjExtData					= NULL; 
        bHitSoundEcho                   = FALSE;
	}
};


struct SAttackStuff
{
	UAttackAttr			uiType;						// All attributes of the attack
	SERIAL_HANDLE		hAttackerSerialId;			// Attacker
	SERIAL_HANDLE		hDefenderSerialId;			// Defender
	RwUInt8				byAttSeq;					// chain attack sequence
	SAttackDamageStuff	sDamage;					// damage stuff
	RwUInt32			uiAttrLogicTblId;			// Weapon attack: item id, skill attack: skill id
	RwV3d				vPushDir;
	RwBool				bAnimSpeedApply;			// Should attack animation speed be applied?
	RwUInt32			uiSyncEventId;
	RwV3d				vFinalLoc;
		
	void Reset(void)
	{
		uiType.uiFlags				= 0;
		hAttackerSerialId			= INVALID_SERIAL_ID;
		hDefenderSerialId			= INVALID_SERIAL_ID;
		byAttSeq					= 0;
		sDamage.iDamage				= 0;
		sDamage.iAbsorbedDamage		= 0;
		sDamage.iReflectedDamage	= 0;
		sDamage.lpEpRecovered.Reset();
		uiAttrLogicTblId			= 0;
		vPushDir.x					= 0.0f;
		vPushDir.y					= 0.0f;
		vPushDir.z					= 0.0f;
		bAnimSpeedApply				= FALSE;
		uiSyncEventId				= INVALID_SYNCEVENT_ID;
		vFinalLoc.x = 0.0f;
		vFinalLoc.y = 0.0f;
		vFinalLoc.z = 0.0f;
	}
};

class CNtlAttackData
{
	DECLEAR_MEMORY_POOL(CNtlAttackData, NTL_DEFAULT_MEMORY_POOL)

private:

	enum { MAX_ATTACK_STUFF = NTL_MAX_NUMBER_OF_SKILL_TARGET};

	RwUInt8 m_byAttCount;
	RwUInt8	m_byAttLogic;
	SAttackStuff m_sStuff[MAX_ATTACK_STUFF];

public:

	void Reset(void)
	{
		m_byAttCount = 0;
		m_byAttLogic = NTL_ATTACK_LOGIC_NORMAL;
	}

	void SetAttackStuffCount(RwUInt8 byAttCount)
	{
		m_byAttCount = byAttCount;
	}

	void SetAttackStuffLogic(RwUInt8 byLogic)
	{
		m_byAttLogic = byLogic;
	}

	RwInt32 GetAttackStuffCount(void) const 
	{
		return m_byAttCount;
	}

	RwUInt8 GetAttackStuffLogic(void) const
	{
		return m_byAttLogic;
	}

	SAttackStuff* GetAttackStuff(RwInt32 iIdx)
	{
		if(iIdx < 0 && iIdx >= MAX_ATTACK_STUFF)
			return NULL;

		return &m_sStuff[iIdx];
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
// move behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////

// move form
#define NTL_MOVEFORM_GROUND			0
#define NTL_MOVEFORM_SWIMMING		1
#define NTL_MOVEFORM_FLY			2
#define NTL_MOVEFORM_SPLINE			3
#define NTL_MOVEFORM_BUS            4               // bus transfer


// move target
#define NTL_MOVETARGET_NONE			0
#define NTL_MOVETARGET_DIR			1	// move keyboard (?)
#define NTL_MOVETARGET_LOC			2	// move dest (mouse) (?)
#define NTL_MOVETARGET_ACTOR		3	// follow (?)
#define NTL_MOVETARGET_DASH			4
#define NTL_MOVETARGET_DASH_ACTOR	5	// dash by skill
#define NTL_MOVETARGET_AIR_DASH		6	// air-jump dash

// move result
#define NTL_MOVE_RESULT_COLLISION	0x01
#define NTL_MOVE_RESULT_FALLING		0x02

// move behavior stuff
struct SMoveStuff
{
	RwUInt8			byType;							// direction, location, actor
	RwUInt8			byForm;							// ground, fly
	RwUInt8			byFormFlag;						// Walking, running, swimming, etc.
	RwUInt8			byMoveFlags;					// By move direction
	RwV3d			vCurrLoc;						// Go to current location.
	RwV3d			vDir;							// In the case of dash, it is used as a direction vector.
	RwV3d			vDest;							// Final destination for mouse and dash movements.
	RwV3d			vSendDest;						// Second shift destination (on bus)
	RwReal			fFollowRange;					// follow range
	SERIAL_HANDLE	hFollowSerialId;				// follow serial
	RwUInt8			byPathNodeNum;					// path node number
	RwV3d			arrPathNode[DBO_MAX_NEXT_DEST_LOC_COUNT];
	RwUInt8			byMoveResult;					// Results after moving.
	RwBool			bSplineGroundCheck;		
	RwBool			bSplineRunMove;					// run move.
	RwChar			chSpline[NTL_MAX_SPLINE_NAME];

	void Reset(void)
	{
		byType			= NTL_MOVETARGET_NONE;
		byForm			= NTL_MOVEFORM_GROUND;
		byFormFlag		= NTL_MOVE_FLAG_WALK;
		hFollowSerialId = INVALID_SERIAL_ID;
		byMoveFlags		= 0;
		byPathNodeNum	= 0;
		byMoveResult	= 0;
	}
};

/// Structure containing Move Sync information (used only by the client)
struct SMoveSyncStuff
{
    std::deque<SNtlEventSobMoveSync*>   m_MoveSyncQ;             ///< Move Sync packet queue
    SNtlEventSobMoveSync*               m_pMoveSyncCurr;         ///< Current Move Sync value    

    SMoveSyncStuff()
    {
        m_pMoveSyncCurr = NULL;
    }

    ~SMoveSyncStuff();

    void    Clear();
    SNtlEventSobMoveSync*  Next();
    SNtlEventSobMoveSync*  Pop();    
};

enum ECharJumpState
{
    JUMPSTATE_NONE,
    JUMPSTATE_START,
    JUMPSTATE_LOOP,
    JUMPSTATE_STAND_LANDING,
    JUMPSTATE_MOVE_LANDING,
    JUMPSTATE_WATER_LANDING,
    JUMPSTATE_END
};


// jump behavior stuff
struct SJumpStuff
{
	RwV3d vJumpDir;
    ECharJumpState eState;
	
	void Reset(void)
	{
		vJumpDir.x = 1.0f;
		vJumpDir.y = 0.0f;
		vJumpDir.z = 0.0f;
        eState = JUMPSTATE_NONE;
	}
};

// skill, item use bahavior stuff
struct SSkillStuff
{
	SERIAL_HANDLE	hTargetSerialId;
	SERIAL_HANDLE	hSkillSerialId;
	RwUInt32		uiSkillTblId;
	RwUInt8			bySlotIdx;
	RwUInt8			byRpBonusType;
	RwReal			fCurrCastingTime;
	RwReal			fMaxCastingTime;
	RwV3d			vFinalLoc;			// caster final location
	RwV3d			vFinalSubjectLoc;	// target final location

	void Reset(void)
	{
		hTargetSerialId		= INVALID_SERIAL_ID;
		hSkillSerialId		= INVALID_SERIAL_ID;
		uiSkillTblId		= 0;
		bySlotIdx			= 0;
		byRpBonusType		= 0xff;
		fCurrCastingTime	= 0.0f;
		fMaxCastingTime		= 0.0f;

		vFinalLoc.x = 0.0f;
		vFinalLoc.y = 0.0f;
		vFinalLoc.z = 0.0f;

		vFinalSubjectLoc.x = 0.0f;
		vFinalSubjectLoc.y = 0.0f;
		vFinalSubjectLoc.z = 0.0f;
	}
};

struct SHTBStuff
{
	SERIAL_HANDLE hAttackSerialId;
	SERIAL_HANDLE hTargetSerialId;
	RwUInt32 uiSkillTblId;
	RwUInt8 bySlotIdx;
	RwUInt8 byCurrStep;
	RwUInt8 byStepCount;
	RwUInt8 byResultCount;
	sHTBSKILL_RESULT sResult[NTL_HTB_MAX_SKILL_COUNT_IN_SET];

	void Reset(void)
	{
		hAttackSerialId = INVALID_SERIAL_ID;
		hTargetSerialId = INVALID_SERIAL_ID;
		uiSkillTblId = 0;
		bySlotIdx = 0;
		byCurrStep = 0;
		byStepCount = 0;
		byResultCount = 0;
	}

	sHTBSKILL_RESULT* FindResult(RwUInt8 byStep)
	{
		for(RwInt8 i = 0; i < byResultCount; ++i)
		{
			if(sResult[i].byStep == byStep)
			{
				return &sResult[i];
			}
		}

		return NULL;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// input control behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////
struct SCtrlStuffFalling
{
	RwUInt8	byMoveDirection;
	RwV3d	vFallingDir;
	RwReal	fSearchHeight;
	RwReal	fSpeed;
};

struct SCtrlStuffFollow
{
	
};

struct SCtrlStuffRotate
{
	RwReal	fDeltaAngleY;

	bool	bSetAngleX;
	RwReal	fDeltaAngleX;
};


struct SCtrlStuffPrivateShop
{
	WCHAR	wchShopName[NTL_MAX_PRIVATESHOP_NAME];	// shop name.
	RwBool	bShopOwnerEmpty;						// shop owner empty.
	RwUInt8	byShopState;							// shop state.
};

struct SCtrlStuffTeleport
{
	RwUInt8			byTeleportType;							// teleport type
	SERIAL_HANDLE	hTeleportNpc;
};

struct SCtrlStuffDirection
{
	RwBool			bAnimUse;
	RwBool			bTimeLoop;
	RwReal			fLoopTime;
	RwUInt8			byDirectType;
	RwUInt32		uiTblId;
	RwUInt32		uiAnimId;
};

struct SCtrlStuffOperate
{
	RwUInt32		hTargetSerialId;
	RwUInt32		uiOperateTime;
	RwUInt32		uiTblId;
};

/**
* \brief Information about gambling NPCs
*/
struct SCtrlStuffGamble
{
	SERIAL_HANDLE	hGambleNpc;
};

/// stun type
struct SCtrlStuffStun
{
    BYTE    byStunType;
};

// Information about buses and vehicles
struct SCtrlStuffRide
{
    RwUInt32        hTargetSerialId;
};

/// Pushing status information
struct SCtrlStuffPushing
{
    BYTE    byAttackResult;
	WORD    wAttackResultValue;
    RwV3d   vDestPos;
};

// Information about Vehicle

enum eVehicleEngineMessgeType
{
	VEHICLE_READY,
	VEHICLE_ENGINE_ASK_REG_FROM_BAG,
	VEHICLE_ENGINE_REG_FUEL,
	VEHICLE_ENGINE_UNREG_FUEL,
	VEHICLE_ENGINE_UPDATE_FUEL,
	VEHICLE_ENGINE_END,
};

struct SCtrlStuffVehicle
{
	RwBool	bCreateUpdate;
	RwBool	bRideOn;
//	RwBool	bEngineOn; //engine disabled by daneos
	RwV3d	vSpawnPos;
	SERIAL_HANDLE hVehicleItem;
	TBLIDX	idxVehicleItem;
};

union UCtrlStuffExtra
{
	SCtrlStuffFalling		sFalling;
	SCtrlStuffFollow		sFollow;
	SCtrlStuffRotate		sRotate;
	SCtrlStuffPrivateShop	sPrivateShop;
	SCtrlStuffTeleport		sTeleport;
	SCtrlStuffDirection		sDirection;
	SCtrlStuffOperate		sOperate;    
	SCtrlStuffGamble		sGamble;
    SCtrlStuffStun          sStun;
    SCtrlStuffPushing       sPushing;
};

struct SCtrlStuff
{
	RwReal			fTime;
	SERIAL_HANDLE	hTargetSerialId;						// attack serial.
	RwReal			fFollowRange;

	UCtrlStuffExtra	        uExtra;
    SCtrlStuffRide          sRide;
	SCtrlStuffVehicle		sVehicle;
		
	void Reset(void)
	{
		fTime				= 0.0f;
		hTargetSerialId		= INVALID_SERIAL_ID;
        sRide.hTargetSerialId = INVALID_SERIAL_ID;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// time sequence
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlTimeSequence
{
public:

	CNtlTimeSequence();
	virtual ~CNtlTimeSequence();
    
    void SetTime(RwReal fTime) {m_fTime = fTime;}
    RwReal GetTime(void) const {return m_fTime;}

	void Add(CNtlTimeSequence *pSeq);
	void Remove(CNtlTimeSequence *pSeq);
	
	void SetPrev(CNtlTimeSequence *pSeq);
	CNtlTimeSequence* GetPrev(void) const;
	
	void SetNext(CNtlTimeSequence *pSeq);
	CNtlTimeSequence* GetNext(void) const;

	virtual void* GetData(RwReal fTime) { return NULL; }
	virtual void* GetDeltaData(RwReal fPrevTime, RwReal fCurrTime) { return NULL; }

protected:
    CNtlTimeSequence *m_pPrev;
    CNtlTimeSequence *m_pNext;
    RwReal m_fTime;
};


class CNtlDistTimeSequence : public CNtlTimeSequence
{
	DECLEAR_MEMORY_POOL(CNtlDistTimeSequence, NTL_DEFAULT_MEMORY_POOL)

private:

	RwReal m_fDist;

	static RwReal m_fCurrDist;
	static RwReal m_fOldDist;
	static RwReal m_fDeltaDist;
	static RwBool m_bCalcOld;
	static RwInt32 m_iOldTimeNode;

public:

	CNtlDistTimeSequence();
	~CNtlDistTimeSequence();

	void SetDistance(RwReal fDist); 
	RwReal GetDistance(void) const;

	RwReal RecursiveData(RwReal fTime);
	RwReal RecursiveDeltaData(RwReal fOldTime, RwReal fCurrTime);

	virtual void* GetData(RwReal fTime);
	virtual void* GetDeltaData(RwReal fOldTime, RwReal fCurrTime);
};



class CNtl2DTimeSequence : public CNtlTimeSequence
{
	DECLEAR_MEMORY_POOL(CNtl2DTimeSequence, NTL_DEFAULT_MEMORY_POOL)

public:

	typedef struct _S2DDist
	{
		RwReal fX;
		RwReal fY;
	}S2DDist;

private:
	S2DDist m_sDist;	
	static S2DDist m_sCurrDist;

public:
	CNtl2DTimeSequence();
	~CNtl2DTimeSequence();

	void SetDistance(RwReal fDistX, RwReal fDistY); 
	S2DDist GetDistance(void) const;

	S2DDist RecursiveData(RwReal fTime);

	virtual void* GetData(RwReal fTime);
};

// Time Sequence class in 3D coordinate system (by agebreak)
class CNtl3DTimeSequence : public CNtlTimeSequence
{
    DECLEAR_MEMORY_POOL(CNtl3DTimeSequence, NTL_DEFAULT_MEMORY_POOL);

public:
    CNtl3DTimeSequence();
    ~CNtl3DTimeSequence();

    virtual void*   GetData(RwReal fTime);
    RwV3d           GetHaveData() {return m_vData;}
    void            SetData(const RwV3d& vData) {m_vData = vData;}

protected:
    RwV3d           RecursiveData(RwReal fTime);

protected:
    RwV3d m_vData;                      ///< Data of current sequence

    static RwV3d m_svCurrData;          ///< Static data that must be held for pointer type return 
};


////////////////////////////////////////////////////////////////////////////////////////////////
// behavior data class
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBeCharData
{
	DECLEAR_MEMORY_POOL(CNtlBeCharData, NTL_DEFAULT_MEMORY_POOL)

private:

	SCtrlStuff	m_sCtrlStuff;
	SMoveStuff	m_sMoveStuff;
    SMoveSyncStuff m_sMoveSyncStuff;                ///< Structure containing Move Sync information
	SJumpStuff	m_sJumpStuff;
	SHitStuff	m_sHitStuff;
	SSkillStuff m_sSkillStuff;    
	SHTBStuff	m_sHTBStuff;
	CNtlAttackData m_AttData;
		
public:

	CNtlBeCharData()
	{
		m_sCtrlStuff.Reset();
		m_sMoveStuff.Reset();
	}
	
	SCtrlStuff* GetCtrlStuff(void) { return &m_sCtrlStuff; }
	SMoveStuff* GetMoveStuff(void) { return &m_sMoveStuff; }
    SMoveSyncStuff* GetMoveSyncStuff() {return &m_sMoveSyncStuff;} // no longer used
	SJumpStuff* GetJumpStuff(void) { return &m_sJumpStuff; }
	SHitStuff* GetHitStuff(void) { return &m_sHitStuff; }
	SSkillStuff* GetSkillStuff(void) { return &m_sSkillStuff; }
	SHTBStuff* GetHTBStuff(void) { return &m_sHTBStuff; }
    
	CNtlAttackData* GetAttackData(void) { return &m_AttData; }

};




////////////////////////////////////////////////////////////////////////////////////////////////
// behavior logic function
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobActor;

// Function related to actor coordinate correction.
void Behavior_AdjustPositionJumping(CNtlSobActor *pActor, const RwV3d *pTarLoc);



#endif
