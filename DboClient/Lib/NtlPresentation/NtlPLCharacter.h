/*****************************************************************************
*
* File			: NTLPLCharacter.h
* Author		: HongHoDong
* Copyright	    : (?)NTL
* Date			: 2005. 8. 20.
* Abstract		: NTL PLCharacter
*****************************************************************************
* Desc         :
*
*****************************************************************************/
#ifndef __NTL_PL_CHARACTER_H__
#define __NTL_PL_CHARACTER_H__

#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLResource.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLItem.h"
#include "ceventhandler.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlDefaultItemData.h"
#include "NtlPLLinkEffect.h"
#include "NtlAnimLayer.h"
#include "NtlInstanceAnimData.h"
#include "NtlPLEquipItem.h"
#include "OccluderProxy.h"


#define dCOSTUME_BITSET_INVALID		(0xffffffff)


struct SPLCharacterCreateParam : public SPLEntityCreateParam
{
	RwUInt32 uiSerialId;

	RwBool   bPcFlag;
	RwUInt32 uiClass;
	RwUInt32 uiRace;
	RwUInt32 uiGender;
	RwUInt32 uiHeadType;
	RwUInt32 uiFaceType;
	RwUInt32 uiHeadColorType;
	RwUInt32 uiSkinColorType;
	RwBool	bNotShadingFlag;
    RwBool  bIsAdult;
	SPLCharacterCreateParam() : bNotShadingFlag(FALSE), bIsAdult(FALSE) {}
};

enum ECharacterAnimLayer
{
	CHARACTER_ANIM_LAYER_BASE = 0,
	CHARACTER_ANIM_LAYER_UPPER,
	CHARACTER_ANIM_LAYER_END,
};

/*
*Information on the animation being played
*/
struct SAnimPlayInfo
{
	RwUInt32	m_uiAnimKey;
	std::vector<SEventAnimHit *> vecAnimHit;

	SAnimPlayInfo() : m_uiAnimKey(0) {}
};


/**
* \ingroup NtlPresentation
* \brief Character related classes
* \date 2006-08-21
* \author Hodong
*/

class CNtlPLCharacter : public CNtlPLAttach, public COccluderProxy
{
public:
	CNtlPLCharacter();
	virtual ~CNtlPLCharacter();

protected:
//	RwSphere				m_BSphere;								///< BoundSphere where Position is not calculated
	RwSphere				m_BSphereCur;							///< BoundingSphere where Position was calculated: Updated when GetBoundingSphere is called.
	CNtlPLCharacterProperty	*m_pProperty;							///< Character Property
	CNtlPLResource			*m_pResourceClump;						///< Target Clump Resource Reference
	ENTITY_ATOMIC_VEC	    m_vecAtomicList;                        ///< List of Atomics that currently make up Clump

	SCharScheduleResInfo	m_sScheduleResInfo;
	RwBool					m_bRunTimeAlpha;

	RpHAnimHierarchy		*m_pBaseHierarchy;						///< Base Hierarchy
   
	CNtlAnimLayer			m_AnimLayer[CHARACTER_ANIM_LAYER_END];	///< AnimationLayer (upper, lower body)
	CNtlInstanceAnimTable	m_InstanceAnimTable;					///< Animation Resource InstanceData

	SAnimPlayInfo			m_sBaseAnimPlayInfo;					///< Information about the animation currently playing
	FRAME_MAP				m_mapFrame;								///< Bone Info
	SToonData				m_ToonData;								///< Toon Ink, Toon Paint, Toon Resource
	
    STypeBoneData			*m_pTypeBoneData;						///< Bone Data Reference
	RwInt32				    m_nBoneCount;							///< Number of Bones
    RwV3d                   m_vBoneStretchTargetPos;                ///< Bone Stretch Target Location
    RwChar                  m_szBoneStretchAxisBone[MAX_DEFAULT_NAME];                ///< Bone that becomes the axis of rotation in the Bone Stretch event


	RwV3d					m_vCurPos;								///< Current location of Character
	RwV3d					m_vScale;								///< When changing the scale of a character (Instance)
	RwV3d					m_vAngle;								///< Angle					

	RwBool					m_bAnimUpdate;							///< Do you update animation?
	RwReal					m_fAnimSpeed;							///< Anim Speed
	RwReal					m_fFastMaxExtent;						///< fast max extent

	CNtlPLEquipItem			*m_pEquipItem;							///< Equip Item

	RwRGBA					m_sHeadColor;							///< Head Color
	RwRGBA					m_sSkinColor;							///< Skin Color

	RwRGBA					m_sColor;								///< Base Color
	RwRGBA					m_sAddColor;							///< Add Color

	RwRGBA					m_sInkColor;							///< Ink Color;

	CNtlPLLinkEffect        m_LinkEffectInstance;                   ///< Object that manages LinkEffect Instances
	ListLoopEffect          m_listLoopEffect;                       ///< LoopEffectÀÇ list
	ListSoundHandle         m_listLoopSound;                        ///< LoopSound¡¯s HANDLE List

	RwBool					m_ToonEdgeVisibility;

	// In the case of NPC or MOB, m_pResourceClump is used, but in this case, SimpleMaterial, which only applies to PLItem, cannot be used.
	RwBool m_SimpleMaterial;

	// Skip animation updates to improve performance when the character is far away.
	RwReal m_SkipAnimUpdateTime;
	RwReal m_SkipAnimUpdateElapsedTime;

	static RwBool m_SkipAnimUpdate;
	static RwBool m_SkipSimpleMaterial;
	static RwBool m_SkipAdge;

	// For NPC, MOB, VEHICLE spherical enviroment mapping texture
	RwTexture* m_pSEMTex;

	RwBool m_PetrifySkillActivated;

protected:
	void	CreateToonData(const RwChar *szTexName, const RwChar *szTexPath);
	RwBool	CreateAnim(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool	bLoop = TRUE);

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	void	SetApplyRunTimeAlpha(RwBool bFlag);

 	virtual RwSphere*	GetBoundingSphere();
// 	virtual void		CalcBoundingSphere();

	void	UpdatePreBoneScale();
	void	UpdatePostBoneScale();

	// Loop Effect related
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< Add LoopEffect to the LoopEffect List.
	void    ClearLoopEffect();                                                                 ///< Destroys EffectInstances in the LoopEffect List.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< Check whether there is a LoopEffect of the Name-Bone pair in the LoopEffect List.

	// Loop Sound Related
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< Add SoundHandle to LoopSound List.
	void    ClearLoopSound();                                                                  ///< Destroys LoopSounds in the LoopSound List.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< Check whether the same SoundHandle exists in the LoopSound List.

	// Animation Event Related    
	virtual void   OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);                               ///< Processes the Anim End Event.
	virtual void   OnEventHit(SEventAnimHit* pEventHit);    
	virtual void   OnEventWeightTime(SEventWeightTime* pEventTime);
	virtual void   OnEventVisualSound(SEventSound* pEventSound);
	virtual void   OnEventTraceEffect(SEventTrace* pEventTrace);                                ///< Creates a Trace Effect.
	virtual void   OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);                 ///< Process the Visual Effect Event and create a Visual Effect.
	virtual void   OnEventSubWeapon(SEventSubWeapon* pEventSubWeapon);                          ///< Handles SubWeapon Event.
	virtual void   OnEventPostEffect(SEventPostEffect* pEventPostEffect);						///< Process Post Effect Event.
	virtual void   OnEventSummonPet(SEventSummonPet* pEventSummonPet);							///< Handles Summon Pet Event.
	virtual void   OnEventAlphaFade(SEventAlpha* pEventAlpha);                                  ///< Processes Alpha Fade Event.
	virtual void   OnEventFootStep(SEventFootStep* pEventFootStep);
	virtual void   OnEventDirect(SEventDirect* pEventDirect);                                   ///< Production event
    virtual void   OnEventColorChange(SEventColorChange* pEventColorChange);                    ///< Color change event handling
    virtual void   OnEventStretch(SEventStretch* pEventStretch);                                ///< Bone Increase Event
    virtual void   OnEventTrigger(SEventTrigger* pEventTrigger);                                ///< Trigger event
    virtual void   OnEventSkillCancel(SEventSkillCancel* pEventSkillCancel);                    ///<Skill Cancel Event

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool	Create(const SPLEntityCreateParam *pParam = NULL);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render();
	virtual RwBool	RenderToTexture();
	

	// Related to creating Occluder Proxy.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool	CullingTest(RwCamera* pRwCamera);
#endif
    virtual RwBool  IsCullingTestAllAtomic();                                       ///< Returns whether the currently set animation is a Culling Atomic check.

	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy();

	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	SCharScheduleResInfo *GetCharScheduleResInfo() { return &m_sScheduleResInfo; }
	virtual RwBool IsSchedulingLoadingComplete() {return m_sScheduleResInfo.bLoadComplete;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);
	RwBool	IsExistWorld();

	virtual void SetVisible(RwBool bVisible);    
	virtual void SetPicking(RwBool bPicking);

	virtual void SetCullFlags(RwUInt32 uiFlag, RwBool bCulled);
	virtual void SetCullFlags(RwUInt32 uiFlag);

	virtual RwFrame*    GetFrame();
	virtual RwMatrix&   GetMatrix();
	virtual void        SetMatrix(RwMatrix& matWorld);    
    RpClump*            GetClump();
    RpAtomic*           GetAtomic(const std::string& strName);
    RpHAnimHierarchy*   GetBaseHierarchy();
    void                UpdateMaterialSkinInfo();                                               ///< Update Atomic material information. (Used when pure mine)

    // Bone related
    RwMatrix*       GetBoneMatrix(const RwChar *pBoneName);
    RwFrame *       GetBoneByName(const RwChar *pBoneName);
    RwUInt32        GetBoneIndex(const RwChar* pBoneName);
    STypeBoneData*  GetBoneData() {return m_pTypeBoneData;}
    RwChar*         GetBoneName(int nBoneIndex);                                                ///< Returns the name of the bone.
    void            SetSBoneStretchTargetPos(const RwV3d& vTargetPos, const RwChar* szAxisBone);///< Set the target of the Bone Stretch event

	virtual void SetPosition(const RwV3d *pPos);
	RwV3d	GetPosition();

	virtual void SetDirection(const RwV3d *pDir);
	RwV3d	GetDirection();
	RwV3d	GetRight();    
	RwV3d   GetUp();

	void	SetAngleDelta(const RwReal fAngleDeltaY);

	void	SetAngleY(const RwReal fAngleY);
	RwReal	GetAngleY() { return m_vAngle.y; }

	void    SetAngleX(const RwReal fAngleX);
	RwReal	GetAngleX() { return m_vAngle.x; }

	virtual void   SetScale(RwReal fScale);														///< Randomly change scale (Instance)
	virtual RwReal GetScale();                                                                  ///< Returns a randomly adjusted scale

	void	SetBaseScale(RwReal fBaseScale);													///< Change BaseScale
	RwReal  GetBaseScale();                                                                     ///< Returns the set BaseScale.
	void	SetPosOffset(RwV3d *pPos);															///< Function to compensate if the foot falls when using Bone Scale

	RwBool	SetAllAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool	bLoop = TRUE);  ///< Set All Animation
	RwBool	SetBaseAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool bLoop = TRUE); ///< Set Base Animation (Client only)
	RwUInt32 GetCurBaseAnimKey() {return m_sBaseAnimPlayInfo.m_uiAnimKey;};                     ///< Returns the Anim Key currently being played.    
	RwBool	IsExistAnim(RwUInt32 uiAnimKey);													///< Is there an Animation of uiAnimKey?
	RwBool	SetBlend(EBlendType eBlendType, RwReal fBlendAllpha = 0.f, RwReal fBlendInterval = 0.1f);///< Animation Blend (If fBlendAlpha = 1.f, Blend does not work.)
	void	SetAnimSpeed(RwReal fSpeed);														///< AnimationÀÇ Speed Default(1.f)
	RwReal  GetAnimSpeed();                                                                     ///< Returns the speed of animation.
	void	SetAnimUpdate(RwBool bAnimUpdate);													///< Whether to update animation or not
	RwBool	GetAnimUpdate();																	///< Function to check whether animation is updated
	RwReal	GetBaseCurrentAnimTime();															///< Function to get the time of the current Base Layer Animation
	RwReal	GetBaseDurationAnimTime();															///< Function to get the total time of the current Base Layer Animation
	void    SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< A function to force the time is also needed.
	RwBool	GetBaseCurrentAnimEnd();
	SAnimPlayInfo	*GetBaseAnimPlayInfo();														///< Obtain brief information about the current animation.
	RwReal  GetDurationAnimTime(RwUInt32 uiAnimKey);											///< Obtain the animation time corresponding to the Anim Key.
    void    OnAfterAnimEventRun();                                                              ///< Executes all Anim Events after the current time.

	RwReal	GetWidth();																			///< Character Width (x)
	RwReal	GetHeight();																		///< Character Height (y)
	RwReal	GetDepth();																			///< Character Depth (z)	
	RwReal	GetMaxExtent();
	RwReal	GetFastMaxExtent();

	RwBool SetChangeEquipCItem(CNtlPLItem* _pItem, RwBool _bAttach, RwUInt32 _CBitSet);
	RwBool SetRemoveEquipCItem(CNtlPLItem* _pItem, RwUInt32 _CBitSet);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. pItem		: This is the Item you want to Change or Remove.
	// 2. bAttach	: Subweapon related flag. In the case of subweapon, this flag is FALSE, meaning it is not actually attached.
	// 3. _CBitSet	: Costume-related BitFlag Set and must be entered only when it is a Costume.
	//				  If the actual BitFlag Set value is 0xFFFFFFFF, the existing method is followed. If it is not 0xFFFFFFFF, it means that something is attached or detached.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <Caution>
	// Things to keep in mind when using the current interface are as follows.
	// All meshes corresponding to BitSet are simply attached and detached internally in Lib. Therefore, this function should always be executed last.
	// This is because if you run this function first and then put on or take off the item in the traditional way, the default item part will be put on or taken off again because of that item.
	//  Costume-related dressing interfaces are always executed last. Conversely, when taking it off, always run it first. It's the same reason.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual RwBool	SetChangeEquipItem(CNtlPLItem *pItem, RwBool bAttach = TRUE, RwUInt32 _CBitSet = dCOSTUME_BITSET_INVALID);
	virtual RwBool	SetRemoveEquipItem(CNtlPLItem *pItem, RwUInt32 _CBitSet = dCOSTUME_BITSET_INVALID);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function to determine toon edge visibility
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// When called, Toon edge rendering can be turned off or on.
	// When this function is called, a flag will be placed in Atomic internally, so care must be taken for maintenance.
	// This function should be called depending on the appearance state of the character, such as when putting on or taking off a partial mesh from the server with Edge turned off.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VOID SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility);

	void UpdateCharacterAtomicList();

	CNtlPLEquipItem* GetEquipItem() {return m_pEquipItem;}
	RwBool  SetItemGradeEffect(ENtlPLItemGrade eGrade);                                         ///< Applies the Grade Effect to the currently equipped weapon.

	SToonData	*GetToonData() { return &m_ToonData; }											///< Obtain Toon Data.							

	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< Base Color
	RwRGBA	*GetColor() { return &m_sColor; }

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA *GetAddColor() { return &m_sAddColor; }

	virtual void SetAlpha(RwUInt8 byValue);																///< Base Alpha
	virtual void SetWeightAlpha(RwReal fWeightValue);													///< Weight Alpha
	virtual void SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);                  ///< Set Atomic Weight Alpha Value.

	virtual void SetSkinColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Skin Color
	RwRGBA	*GetSkinColor() { return &m_sSkinColor; }		

	virtual void SetHeadColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Head Color
	RwRGBA *GetHeadColor() { return &m_sHeadColor; }

	virtual void SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Ink Color
	virtual void SetInkThickness(RwReal fThickness = DEFAULT_INK_THICKNESS);					///< Ink Thickness

	virtual int	CallBackBaseAnim(void* pAnimEvent);											///< Callback function that receives Animation Event
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

	// Link Effect related
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< Returns LinkEffect Vector.
    CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Attach Link Effect.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);                      ///< Detach the Link Effect.

	RwV3d  GetFaceCameraPos() { return m_pProperty->GetFaceCameraPos();}							///< Get Face Camera Position
	RwV3d  GetFaceCameraLookAt() { return m_pProperty->GetFaceCameraLookAt();}					///< Set Face Camera LookAt Height 
    RwReal GetAlphaDistance();                              

	RwBool IsApplyGroundPitchAngle(void);

	static void fpRenderCB(void* _pParam);

	// Problems may occur when skipping animation update. ex) When world transformation is not possible in the tutorial and only processed with char animation, etc.
	// Set and release the sections where problems occur; It can be used in the same concept as setting and restoring renderstate.
	static void SetSkipAnimUpdate(RwBool _Skip = TRUE);

	// This is a function to determine the presence or absence of the game option check in the simple material part.
	static void SetSkipSimpleMaterial(RwBool _Skip = TRUE);
	static RwBool GetSkipSimpleMaterial(void);

	// Skip the edge. (Fluorite)
	static void SetSkipEdge(RwBool _Skip = FALSE);
	static RwBool GetSkipEdge(void);

	void	SetDXT1AlphaCheck(RwBool _DXT1AlphaCheck);

	// simple material; simplify textures
	RwBool	GetSimpleMaterial() { return m_SimpleMaterial; }
	VOID	SetSimpleMaterial(RwBool _SimpleMaterial);

	// Petrify
	RwBool	GetPetrifyMaterial() { return m_PetrifySkillActivated; }
	VOID	SetPetrifyMaterial(RwBool _PetrifyFlag);

	// Spherical Enviroment Map
	RwTexture* GetSEM() { return m_pSEMTex; }

	// enviroment map uv ani toggling
	static VOID ToggleEMUVAni(RwBool _Visibility);
};

#endif
