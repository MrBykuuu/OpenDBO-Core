#ifndef _NTL_ANIM_EVENT_DATA_H_
#define _NTL_ANIM_EVENT_DATA_H_

#include "GraphicDataTable.h"
#include "NtlInstanceEffect.h"
#include "NtlSoundDefines.h"
#include "NtlSerializer.h"



// Union to define additional parameters according to Projectile Type
struct SHissidanData        /// Additional data for Hissidan type
{
	RwBool      bApplyAngle;                              ///< Whether to apply/not apply an angle (if not applied, the direction will bend depending on the Hand Type)
	RwV2d       v2dAngle;                                   ///< Launch angle (X-axis, Y-axis)
};

struct SMultiHissidanData   /// Additional data for Multi Hissidan type
{
	RwInt32     nCount;                                   ///< Hissidan count
	RwV2d*      pArrayAngle;                              ///< Array of launch angles (dynamically allocated)
};

struct SHellZoneData        /// Additional data for the magic attack siege type
{
	RwV3d				vTargetEffectOffset;						///< Offset distance from target
	RwReal				fTargetEffectStartWaitTime;					///< Waiting time before flying to Target
	RwReal				fTargetEffectSpeed;							///< Speed ??of flying to target
};

union UEffectTypeExtraData
{
	SHissidanData       hissidanData;
	SMultiHissidanData  multiHissidanData;
	SHellZoneData       hellZoneData;
};

struct SEventAnimHit : public SEventAnim
{
	RwUInt32			uiDamage;
	RwBool				bPowerEffect;
	EAttackType			eAttackType;								///< Attack Type
	ETargetBehavior		eTargetBehavior;
	RwBool              bKB2Push;                                   ///< Flag for whether or not to process with Push if knockdown is not performed
	EHandType			eHandType;									///< HandType
	EProjectileEffectType uiProjectileEffectType;					///< Ball, Beam, Hissidan, hell zone, Multi Hissidan

	EBoneType			eProjectileShotType;						///< Set which bone the projectile is sent from
	RwInt32             nSubWeaponFlag;                             ///< Flag to check which Bone of SubWeapon to exit from

	RwChar				chBoneName[MAX_DEFAULT_NAME];				///< Character Bone Name	
	RwChar              chProjectileEffectName[MAX_DEFAULT_NAME];	///< Flying Effect Name
	RwReal				fProjectileSpeed;							///< projectile speed
	RwBool              bTargetAttach;								///< Whether or not to attach the effect to the target
	RwReal              fTargetHeight;								///< Location where Target Effect will appear (1 is default 100%)

	RwChar              chTargetEffectName[MAX_DEFAULT_NAME];		///< Effect Name to be displayed in Target
	ETargetEffectType	eTargetEffectType;							///< Direction type of Target Effect
	RwChar              chSubTargetEffect[MAX_DEFAULT_NAME];        ///< Effect displayed to nearby targets when attacking in range
	RwChar              chTargetSoundName[MAX_DEFAULT_NAME];		///< Sound File Name that appears when hitting the target
	eChannelGroupType	eSoundType;									///< Sound type(CHANNEL_GROUP_VOICE_SOUND or CHANNEL_GROUP_EFFECT_SOUND)    
	RwBool              bHitSoundEcho;                              ///< Whether the sound will be echoed when hit
	RwBool              bCameraShake;                               ///< Availability of Camera Shake when Target Hits
	RwReal              fCameraShakeFactor;                         ///< Camera shake factor
	RwReal              fCameraShakeMaxHeight;                      ///< Camera shake maximum factor
	RwChar              chWordEffect[MAX_DEFAULT_NAME];             ///< Word Effect that appears when Target Hits (ex. Papak)

	UEffectTypeExtraData uEffectTypeExtraData;                      ///< Additional data union according to Effect Type


	SEventAnimHit() : eAttackType(ATTACK_TYPE_PHYSICAL),
		eTargetBehavior(TARGET_BEHAVIOR_NONE),
		bKB2Push(FALSE),
		uiDamage(0),
		bPowerEffect(FALSE),
		eHandType(HAND_TYPE_LEFT),
		uiProjectileEffectType(BEID_PROJ_BALL),
		fProjectileSpeed(10.0f),
		eProjectileShotType(BONE_CHARACTER),
		nSubWeaponFlag(0),
		bTargetAttach(FALSE),
		fTargetHeight(1.0f),
		eSoundType(CHANNEL_GROUP_EFFECT_SOUND),
		bCameraShake(FALSE),
		eTargetEffectType(TARGET_EFFECT_TYPE_NONE),
		bHitSoundEcho(FALSE),
		fCameraShakeFactor(CAMERA_SHAKE_FACTOR),
		fCameraShakeMaxHeight(CAMERA_SHAKE_MAX_HEIGHT_FACTOR)
	{
		eEventID = EVENT_ANIM_HIT;
		memset(chBoneName, 0, sizeof(chBoneName));
		ZeroMemory(chProjectileEffectName, sizeof(chProjectileEffectName));
		ZeroMemory(chTargetEffectName, sizeof(chTargetEffectName));
		ZeroMemory(chTargetSoundName, sizeof(chTargetSoundName));
		ZeroMemory(chWordEffect, sizeof(chWordEffect));
		ZeroMemory(&uEffectTypeExtraData, sizeof(UEffectTypeExtraData));
		ZeroMemory(chSubTargetEffect, sizeof(chSubTargetEffect));
	}

	~SEventAnimHit()
	{
		if (uiProjectileEffectType == BEID_PROJ_MULTI_HISSIDAN)
		{
			NTL_ARRAY_DELETE(uEffectTypeExtraData.multiHissidanData.pArrayAngle); // Since it is dynamically allocated, it must be deleted.
		}
	}

	void SaveSerialize(CNtlSerializer& sOut);
	void LoadSerialize(CNtlSerializer& sIn);

};

struct SEventAnimEnd : public SEventAnim
{
	RwUInt32	uiAnimKey;
	SEventAnimEnd() : uiAnimKey(0) { eEventID = EVENT_ANIM_END; }

};

struct SEventVisualEffect : public SEventAnim
{
	RwChar  chEffectName[MAX_DEFAULT_NAME];     ///< Effect Name;
	EBoneType eBoneType;                        ///< Bone Type to which Effect will be attached
	RwChar  chBoneName[MAX_DEFAULT_NAME];       ///< Bone name to be attached
	RwV3d   vOffSetPos;                         //<  Offset;    
	RwBool  bAttach;                            ///< Whether to attach to model or not
	RwBool  bAttachBone;                        ///< Whether to attach to bone (TRUE = Bone Attach, FALSE = Pos Attach)
	RwBool  bApplyScale;                        ///< Whether to apply the scale of the parent entity
	RwBool  bProjectileType;                    ///< Is it a projectile type?

	unsigned int uiUnknown;

	SEventVisualEffect() : eBoneType(BONE_CHARACTER)
	{
		ZeroMemory(chBoneName, sizeof(chBoneName));
		ZeroMemory(chEffectName, sizeof(chEffectName));
		ZeroMemory(&vOffSetPos, sizeof(vOffSetPos));
		bAttach = TRUE;
		bAttachBone = FALSE;
		bApplyScale = TRUE;
		bProjectileType = FALSE;

		uiUnknown = 0;

		eEventID = EVENT_ANIM_VISUAL_EFFECT;
	}

};

struct SEventSound : public SEventAnim
{
	RwChar	chSoundName[MAX_DEFAULT_NAME * 2]; ///< Sound file name (including Sound folder and path name)
	RwChar	chSoundName2[MAX_DEFAULT_NAME * 2]; ///< Sound file name (including Sound folder and path name)
	RwChar	chSoundName3[MAX_DEFAULT_NAME * 2]; ///< Sound file name (including Sound folder and path name)
	RwChar	chSoundName4[MAX_DEFAULT_NAME * 2]; ///< Sound file name (including Sound folder and path name)
	eChannelGroupType eSoundType;				///< Sound Type (CHANNEL_GROUP_VOICE_SOUND or CHANNEL_GROUP_EFFECT_SOUND)    
	RwBool  bLoop;                              ///< Presence of Sound Loop
	RwReal  fSoundVolume;                       ///< Sound Volume
	RwReal  fSoundDist;                         ///< Effective distance
	RwReal  fSoundDecayDist;                    ///< Sound attenuation distance
	RwReal  fSoundPitchMin;                     ///< Minimum sound pitch
	RwReal  fSoundPitchMax;                     ///< Maximum sound pitch

	SEventSound()
	{
		eEventID = EVENT_ANIM_VISUAL_SOUND;
		ZeroMemory(chSoundName, sizeof(chSoundName));
		ZeroMemory(chSoundName2, sizeof(chSoundName2));
		ZeroMemory(chSoundName3, sizeof(chSoundName3));
		ZeroMemory(chSoundName4, sizeof(chSoundName4));
		eSoundType = CHANNEL_GROUP_EFFECT_SOUND;
		bLoop = FALSE;
		fSoundVolume = 100.0f;
		fSoundDist = 15.0f;
		fSoundDecayDist = 30.0f;
		fSoundPitchMin = fSoundPitchMax = dNTLSOUND_PITCH_DEFAULT;
	}
};

struct SEventFootStep : public SEventAnim
{
	EFootStepType eFootStepType;
	EFootStepMobType eFootStepMobType;

	SEventFootStep()
	{
		eEventID = EVENT_ANIM_FOOT_STEP;
		eFootStepType = FOOT_LEFT;
		eFootStepMobType = FOOT_TYPE_NORMAL;
	}
};

struct SEventLinkEffect : public SEventAnim
{
	RwChar  chEffectName[MAX_DEFAULT_NAME];
	RwChar  chBoneName[MAX_DEFAULT_NAME];
	RwV3d   vOffsetPos;
	RwBool  bAttachBone;

	SEventLinkEffect()
	{
		eEventID = EVENT_ANIM_LINK_EFFECT;
		ZeroMemory(chEffectName, sizeof(chEffectName));
		ZeroMemory(chBoneName, sizeof(chBoneName));
		ZeroMemory(&vOffsetPos, sizeof(vOffsetPos));
		bAttachBone = FALSE;
	}

};

///////////////////////////////////////////////////////////////////////////
// Slow Time Event
//////////////////////////////////////////////////////////////////////////
struct SEventWeightTime : public SEventAnim
{
	RwReal fLifeTime;       ///< Time for the Slow effect to take effect
	RwReal fWeightValue;    ///< Weight value for how much it will slow down

	SEventWeightTime()
	{
		eEventID = EVENT_ANIM_WEIGHT_TIME;
		fLifeTime = 0.0f;
		fWeightValue = 1.0f;
	}

};

struct SEventTrace : public SEventAnim
{
	/// Type of where the trace will be attached
	enum EAttachType
	{
		CHARACTER_BONE,     ///< Attached to the character bone.
		WEAPONE_BONE,       ///< Attaches to the weapon pattern.
		SUB_WEAPON_BONE,    ///< Attaches to secondary weapon model.
	};

	/// Determine whether trajectory settings follow event settings or weapon settings.
	enum ETraceKind
	{
		EVENT_TRACE,        ///< Follows Event settings.
		ITEM_TRACE,         ///< Follows the item settings.
	};

	RwReal  fLifeTime;           ///< Display time of trajectory
	RwReal  fEdgeLifeTime;       ///< EdgeÀÇ Life Time    
	EAttachType eAttachType;     ///< Where will the trajectory end up? 
	ETraceKind  eTraceKind;      ///< Whether the trajectory will follow the Item settings
	RwChar  strStartBoneName[MAX_DEFAULT_NAME];    ///< Name of the first bone
	RwChar  strEndBoneName[MAX_DEFAULT_NAME];      ///< Name of the second bone
	RwChar  strTexture[MAX_DEFAULT_NAME];          ///< Texture File Name
	RwV3d   v3dStartBoneOffset;  ///< Offset of the first bone
	RwV3d   v3dEndBoneOffset;    ///< Offset of the second bone
	RwReal  fEdgeGap;             ///< Value of how many seconds to create an edge
	RwInt32 nSplinePointCount;   ///< Number of vertices created with spline (number of vertices created between two edges)
	RwInt32 nMaxEdgeCount;       ///< Maximum Edge Count
	RwReal  fMaxLength;          ///< maximum length of trajectory
	RwBlendFunction eSrcBlend;  ///< Src Blend State
	RwBlendFunction eDestBlend; ///< Dest Blend State
	RwRGBA  colStartColor;       ///< Starting color value
	RwRGBA  colEndColor;         ///< End color value   

	SEventTrace() : fLifeTime(1.0f),
		fEdgeLifeTime(0.3f),
		fEdgeGap(0.03f),
		nSplinePointCount(10),
		nMaxEdgeCount(500),
		fMaxLength(1.0f)
	{
		eEventID = EVENT_ANIM_TRACE_EFFECT;
		eSrcBlend = rwBLENDSRCALPHA;
		eDestBlend = rwBLENDONE;
		eAttachType = CHARACTER_BONE;
		eTraceKind = EVENT_TRACE;
		ZeroMemory(strStartBoneName, sizeof(strStartBoneName));
		ZeroMemory(strEndBoneName, sizeof(strEndBoneName));
		ZeroMemory(strTexture, sizeof(strTexture));
		ZeroMemory(&v3dStartBoneOffset, sizeof(RwV3d));
		ZeroMemory(&v3dEndBoneOffset, sizeof(RwV3d));
		colStartColor.red = colStartColor.green = colStartColor.blue = 255;
		colEndColor.red = colEndColor.green = colEndColor.blue = 255;
		colStartColor.alpha = 255;
		colEndColor.alpha = 0;
	}

};

/// Sub Weapon Activation Event
struct SEventSubWeapon : SEventAnim
{
	ESubWeaponActiveFlag    eSubWeaponActiveFlag;

	SEventSubWeapon() : eSubWeaponActiveFlag(SUB_WEAPON_DEACTIVE)
	{
		eEventID = EVENT_ANIM_SUB_WEAPON;
	}

};

/// Post Effect event (center line display event on screen)
struct SEventPostEffect : SEventAnim
{
	RwChar                   szPostEffectName[MAX_DEFAULT_NAME];          ///< Name of PostEffect to be used
	EPostEffectTypeFlag      eTarget;                                     ///< Target type of PostEffect
	RwV3d                    v3dOffset;                                   ///< Offset value of the location where the PostEffect will be attached
	RwBool                   bCenterFixEnable;                            ///< Center point fixation function
	RwReal                   fTargetHeight;                               ///< If the object is set as a target, the height value (bounding box relative value)
	RwChar                   szPCBoneName[MAX_DEFAULT_NAME];              ///< If the target is set to PC, the name of the bone to attach

	SEventPostEffect()
	{
		eEventID = EVENT_ANIM_POST_EFFECT;

		ZeroMemory(szPostEffectName, sizeof(szPostEffectName));
		ZeroMemory(&v3dOffset, sizeof(v3dOffset));
		ZeroMemory(szPCBoneName, sizeof(szPCBoneName));
		eTarget = POST_EFFECT_TARGET_TYPE_SELF;
		bCenterFixEnable = TRUE;
		fTargetHeight = 1.0f;
	}

};

// Summon Pet Event
struct SEventSummonPet : SEventAnim
{
	SEventSummonPet()
	{
		eEventID = EVENT_ANIM_SUMMON_PET;
	}
};

struct SEventAnimCinematic : public SEventAnim
{
	EAnimCinematicEventType eAnimCinematicEventType;

	SEventAnimCinematic()
	{
		eEventID = EVENT_ANIM_TMQ;
		eAnimCinematicEventType = E_ANIM_CINEMATIC_TMQ_IN;
	}
};

// Alpha Event 
struct SEventAlpha : SEventAnim
{
	// Alpha Event Apply Type
	enum EAlphaEventType
	{
		E_ALPHA_EVENT_CLUMP,
		E_ALPHA_EVENT_ATOMIC,
	};
	//////////////////////////////////////////////////////////////////////////

	RwInt32       nStartAlpha;              ///< Fade start alpha value (0~255)
	RwInt32       nDestAlpha;               ///< Fade Dest alpha value (0~255)    
	RwReal        fFadeTime;                ///< Fade time
	RwReal        fLifeTime;                ///< Life Time for which events are maintained
	EAlphaEventType eAlphaEventType;        ///< Alpha event application type
	BITFLAG       bfAtomicList;             ///< Index flag of the atomics to which alpha will be applied


	SEventAlpha()
	{
		eEventID = EVENT_ANIM_ALPHA;
		nStartAlpha = 255;
		nDestAlpha = 0;
		fFadeTime = 1.0f;
		fLifeTime = 1.0f;
		eAlphaEventType = E_ALPHA_EVENT_CLUMP;
		bfAtomicList = 0;
	}
};

// explosion event
struct SEventExplosion : SEventAnim
{
	ENtlPLExplosionEventType  eType;

	SEventExplosion()
	{
		eEventID = EVENT_ANIM_EXPLOSION;
		eType = EXPLOSION_EVENT_TYPE_NORMAL;
	}
};

// Directed event
struct SEventDirect : SEventAnim
{
	ENtlPLDirectEventType eType;

	SEventDirect()
	{
		eEventID = EVENT_ANIM_DIRECT;
		eType = DIRECT_EVENT_TYPE_CAMERA_SHAKE;
	}
};

// Color change event
struct SEventColorChange : SEventAnim
{
	EColorChangeType eType;
	RwRGBA           colorEdge;
	RwRGBA           colorBody;
	RwRGBA           colorAdd;

	SEventColorChange()
	{
		eEventID = EVENT_ANIM_COLOR_CHANGE;
		eType = COLOR_CHANGE_TYPE_START;
		colorEdge.red = colorEdge.green = colorEdge.blue = 0;
		colorBody.red = colorBody.green = colorBody.blue = 255;
		colorAdd.red = colorAdd.green = colorAdd.blue = 0;
		colorEdge.alpha = colorBody.alpha = colorAdd.alpha = 255;
	}
};

/// Information about increasing BONE
struct StretchBoneInfo
{
	RwChar szBoneName[MAX_DEFAULT_NAME];
	RwReal fStretchSpeed;
	RwReal fWidth;

	StretchBoneInfo()
	{
		ZeroMemory(szBoneName, sizeof(szBoneName));
		fStretchSpeed = 100.0f;
		fWidth = 1.0f;
	}
};

/// Event to increase BONE
struct SEventStretch : SEventAnim
{
	EStretchEventType eType;
	RwChar          szBoneName[2][MAX_DEFAULT_NAME];
	RwReal          fStretchSpeed;
	RwReal          fWidth;
	RwReal          fAccel;
	RwChar          szScaleBone[MAX_DEFAULT_NAME];
	RwReal          fScaleSize;
	RwChar          szAxisBone[MAX_DEFAULT_NAME];
	RwChar          szTargetEffect[MAX_DEFAULT_NAME];

	SEventStretch()
	{
		eEventID = EVENT_ANIM_STRETCH;
		fScaleSize = 1.0f;
		fStretchSpeed = 10.0f;
		fWidth = 1.0f;
		fAccel = 25.0f;
		sprintf_s(szBoneName[0], "Bip01 L UpperArm");
		sprintf_s(szBoneName[1], "Bip01 L Forearm");
		sprintf_s(szScaleBone, "Bip01 L Hand");
		sprintf_s(szAxisBone, "Bip01 L Clavicle");
		ZeroMemory(szTargetEffect, sizeof(szTargetEffect));
		eType = E_STRETCH_PULLING;
	}
};

/// Trigger events used in skills, etc.
struct SEventTrigger : SEventAnim
{
	SEventTrigger()
	{
		eEventID = EVENT_ANIM_TRIGGER;
	}
};

/// Event used for skill cancellation
struct SEventSkillCancel : SEventAnim
{
	SEventSkillCancel()
	{
		eEventID = EVENT_ANIM_SKILL_CANCEL;
	}
};

/// A function that returns the size of each event structure.
static RwUInt32 GetAnimEventSize(EAnimEventType eType)
{
    switch(eType)
    {
    case EVENT_ANIM_HIT:
        return sizeof(SEventAnimHit);
    case EVENT_ANIM_VISUAL_EFFECT:
        return sizeof(SEventVisualEffect);
    case EVENT_ANIM_VISUAL_SOUND:
        return sizeof(SEventSound);
    case EVENT_ANIM_FOOT_STEP:
        return sizeof(SEventFootStep);
    case EVENT_ANIM_LINK_EFFECT:
        return sizeof(SEventLinkEffect);
    case EVENT_ANIM_WEIGHT_TIME:
        return sizeof(SEventWeightTime);
    case EVENT_ANIM_TRACE_EFFECT:
        return sizeof(SEventTrace);
    case EVENT_ANIM_SUB_WEAPON:
        return sizeof(SEventSubWeapon);
    case EVENT_ANIM_POST_EFFECT:
        return sizeof(SEventPostEffect);
    case EVENT_ANIM_SUMMON_PET:
        return sizeof(SEventSummonPet);
    case EVENT_ANIM_TMQ:
        return sizeof(SEventAnimCinematic);
    case EVENT_ANIM_ALPHA:
        return sizeof(SEventAlpha);
    case EVENT_ANIM_EXPLOSION:
        return sizeof(SEventExplosion);
    case EVENT_ANIM_DIRECT:
        return sizeof(SEventDirect);
    case EVENT_ANIM_COLOR_CHANGE:
        return sizeof(SEventColorChange);
    case EVENT_ANIM_STRETCH:
        return sizeof(SEventStretch);
    case EVENT_ANIM_TRIGGER:
        return sizeof(SEventTrigger);
    case EVENT_ANIM_SKILL_CANCEL:
        return sizeof(SEventSkillCancel);        
    }

    return 0;
}


#endif
