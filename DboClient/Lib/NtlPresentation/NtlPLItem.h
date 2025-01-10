/*****************************************************************************
 *
 * File			: NtlPLItem.h
 * Author		: HongHoDong
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 11. 2.	
 * Abstract		: NTL CNtlPLItem
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_ITEM_
#define _CNTL_PL_ITEM_

#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLResource.h"
#include "NtlPLItemProperty.h"
#include "NtlItem.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlPLLinkEffect.h"
#include "NtlInstanceTraceSystem.h"
#include "NtlAnimLayer.h"
#include "NtlInstanceUpgradeEffectSystem.h"


struct SPLItemCreateParam : public SPLEntityCreateParam
{
	EItemEquipSlotType	eItemEquipSlotType;
	RwUInt8				byUpgrade;

	SPLItemCreateParam() : eItemEquipSlotType(ITEM_EQUIP_NONE), byUpgrade(0) {}
};


class CNtlPLCharacter;
class CNtlPLItemEMUVController;


class CNtlPLItem : public CNtlPLAttach
{
public:
	CNtlPLItem();
	virtual ~CNtlPLItem();

protected:
	CNtlPLItemProperty		*m_pProperty;							///< Item Property
	CNtlPLResource			*m_pResourceClump;						///< Item Resource

	RwMatrix				m_matWorld;								///< World Matrix of Item

	FRAME_MAP				m_mapFrame;								///< Bone Info
	ENTITY_ATOMIC_VEC		m_vecAtomicList;						///< Atomic List

	CNtlPLCharacter			*m_pOwner;								///< Owner in case of Equip Item
	
	SItemScheduleResInfo	m_sScheduleResInfo;						///< Resource information related to Schedule Loading	
	EItemEquipSlotType		m_eInstanceEquipSlotType;				///< Instance Equip Slot Type
	

	RwTexture				*m_pBaseTexture;						///< Base Texture
	RwTexture				*m_pMultiTexture;						///< Multi Texture

	RwRGBA					m_sColor;								///< Color
	RwRGBA					m_sAddColor;
	RwBool					m_bRunTimeAlpha;

	SToonData				m_ToonData;								///< Toon Ink, Toon Paint (Not created by default in Item.)
																	///< Since it only applies to characters, it is created when the character is equipped.

	CNtlPLLinkEffect        m_LinkEffectInstance;					///< Object that manages LinkEffect Instances
	RwBool                  m_bEnableTrace;                         ///< Presence of trajectory effect expression    
    RwBool                  m_bEnableUpgradeEffect;                 ///< Upgrade Effect expression presence/absence flag

    // Item animation related
    RpHAnimHierarchy*       m_pBaseHierarchy;                       ///< Item's Hierarchy
    CNtlAnimLayer*          m_pAnimLayer;                           ///< Animation layer
    CNtlInstanceAnimTable*  m_pInstanceAnimTable;                   ///< Animation Resource Instance Table
    RwUInt32                m_uiCurAnimKey;                         ///< Currently applied AnimKey
    RwBool                  m_bAnimUpdate;                          ///< Availability of Animation Update
	
	RwBool                  m_bVisualEffect;                        ///< Whether VisualEffect is attached or not

    RwInt32                 m_nGrade;                               ///< Grade of Item    
    std::vector<CNtlInstanceEffect*> m_pUpgradeEffects;             ///< Vector of generated Upgrade Effects

	RwTexture				*m_pUVTexture;							///< U,V Texture
	RwMatrix				*m_pUVMatrix;							///< U,V Texture Matrix

	// fake sphere enviroment uv ani effect controller for each upgrade level
	CNtlPLItemEMUVController* m_pEMUVController;

	// DOGI emblem textures
	RwTexture* m_pDogiEmblemTex;

	// DOGI emblem color
	RwRGBA m_DogiBeltClr;

	// DOGI editing color
	RwRGBA m_DogiEditedColor;

	// default render callback
	RpAtomicCallBackRender	m_fnDefRenderCallBack;

	RwBool m_SimpleMaterial;

	RwBool m_PetrifyMaterial;

protected:
	RwBool  CreateAnimData(RwUInt32 uiAnimKey);														///< Set animation-related data.

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);
	void	SetApplyRunTimeAlpha();

	// set enviroment map & uv ani
	VOID SetEMUV();

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);
	
    // Overriding Method
	virtual RwBool Create(const SPLEntityCreateParam *pParam = NULL);
	virtual void   Destroy(void);
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#endif

	virtual void CallSchedulingResource(CNtlPLResource *pResource);

    virtual void   SetVisible(RwBool bVisible);
	
	SItemScheduleResInfo *GetItemScheduleResInfo() { return &m_sScheduleResInfo; }
    virtual RwBool IsSchedulingLoadingComplete() {return m_sScheduleResInfo.bLoadComplete;}

	RwBool	RenderToTexture();

	virtual RwFrame* GetFrame(void);

	virtual VOID SetEMUVController(RwBool _Activation);
	//virtual CNtlPLResource* GetResourceClump() { return m_pResourceClump; }
	//virtual CNtlPLItemEMUVController* GetEMUVController() { return m_pResourceClump; }
	const virtual CNtlPLResource* GetResource(void) const {return m_pResourceClump; }

	virtual RwBool SetProperty(const CNtlPLProperty *pProperty);
    virtual CNtlPLItemProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);
	RwBool	IsExistWorld();

	void SetPosition(const RwV3d *pPos);
	void SetMatrix(RwMatrix& matWorld);
	virtual RwMatrix& GetMatrix() {return m_matWorld;}
	
	RwMatrix* GetBoneMatrix(const RwChar *pszBoneName);
	RwFrame *GetBoneByName(const RwChar *pszBoneName);
	
	EItemResType GetResType() { NTL_ASSERTE(m_pProperty); return m_pProperty->GetResType();}
	
	void SetEquipSlotType(EItemEquipSlotType eEquipSlotType) { m_eInstanceEquipSlotType = eEquipSlotType; }
	EItemEquipSlotType GetEquipSlotType() { return m_eInstanceEquipSlotType;  }
	
	void	SetBaseTexture(RwTexture *pTexture);
	void	SetMultiTexture(RwTexture *pTexture);

	RwTexture *GetBaseTexture() { return m_pBaseTexture; }
	RwTexture *GetMultiTexture() { return m_pMultiTexture; }
	
	RwTexture *GetUVTexture() { return m_pUVTexture; }
	RwMatrix *GetUVMatrix() { return m_pUVMatrix; }

	void	SetAlpha(RwUInt8 byValue);
	void	SetWeightAlpha(RwReal fWeightValue);

	void	SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	void	SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	RwRGBA	*GetColor() { return &m_sColor; }
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	SToonData	*GetToonData();

	virtual void	SetChangeEquip(CNtlPLCharacter *pOwner, RwBool bApplyAttach = TRUE);
	virtual void	SetRemoveEquip();

	virtual void	SetApplyEquipData();
	virtual void	SetApplyEquipDataOwner(RpAtomic *pAtomic, CNtlPLCharacter *pOwner);

	CNtlPLCharacter *GetOwner() { return m_pOwner; }

    RwBool          GetEnableVisualEffect() {return m_bVisualEffect;}                               ///< Returns whether Visual Effect is attached or not.
    RwBool          AttachVisualEffect( CNtlInstanceEffect* pInstanceEffect, 
		                                SEventVisualEffect* pEventVisualEffect);
    
    VecLinkEffect*      GetVecLinkEffect() { return &(m_LinkEffectInstance.m_vecLinkEffect); }	    ///< Returns LinkEffect Vector.
    CNtlInstanceEffect* AttachLinkEffect( SEventLinkEffect* pEventLinkEffect );                     ///< Attach Link Effect.
    RwBool              DetachLinkEffect( CNtlInstanceEffect* pLinkEffect );     

    void                SetEnableTrace(RwBool bEnable) {m_bEnableTrace = bEnable; }					///< Set the presence or absence of trajectory effect expression.
    RwBool              GetEnableTrace() { return m_bEnableTrace; }									///< Returns the presence or absence of trajectory effect expression.
    RwBool              ActiveTraceEffect( SEventTrace* pEventTrace );								///< Expresses the trajectory effect.

    RwBool              SetUpgradeEffect( ENtlPLItemGrade eGrade );									///< Set the Grade Effect of the Item.
    RwInt32             GetUpgradeEffect() {return m_nGrade;}
    void                SetEnableUpgradeEffect( RwBool bEnable );									///< Set whether or not to use the Grade Effect of the Item.
	
	void				SetEffect(ENtlPLItemGrade eGrade, char* effect, RwChar* bone1 = nullptr, RwChar* bone2 = nullptr);

	RwV3d	GetSubWeaponOffset() { return m_pProperty->GetAttachOffset();}

    RwBool	SetAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);		///< Apply animation.
    virtual int CallBackBaseAnim(void* pEventData);												///< CallBack function of Animation Event    
    
	void    SetAnimUpdate(RwBool bAnimUpdate) {m_bAnimUpdate = bAnimUpdate;}						///< Set whether animation is updated or not.
    RwBool  GetAnimUpdate() {return m_bAnimUpdate;}													///< Returns whether animation is updated or not.
    RwReal  GetBaseDurationAnimTime();																///< Returns the Play Time of Animation.
    RwBool  IsHaveAnimData();                                                                       ///< Returns whether animation data is set.

	// DOGI interfaces
	RwTexture*	GetDogiEmblem() { return m_pDogiEmblemTex; }
	RwRGBA*		GetDogiBeltColor() { return &m_DogiBeltClr; }
	RwBool		SetDogiBeltColor(RwRGBA& _BeltRGBA);
	VOID		SetDogiEditedColor(RwRGBA& _DogiEditedColor);
	RwRGBA*		GetDogiEditedColor() { return &m_DogiEditedColor; }
	RwBool		AttachDogiEmblem(RwTexture* _pTexture);
	RwBool		DetachDogiEmblem();
	
	VOID	SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility);

	void	SetSimpleMaterial(RwBool _SimpleMaterial) { m_SimpleMaterial = _SimpleMaterial; }
	RwBool	GetSimpleMaterial() { return m_SimpleMaterial; }
	void	CreateSimpleMaterial();
	void	RestoreSimpleMaterial();

	VOID	SetPetrifyMaterial(RwBool _PetrifyFlag) { m_PetrifyMaterial = _PetrifyFlag; }
	RwBool	GetPetrifyMaterial() { return m_PetrifyMaterial; }
	void	CreatePetrifyMaterial();
	void	RestorePetrifyMaterial();

	RwBool	GetMeshFilePrefixName(string& _MeshFileName);
};

#endif
/*
#pragma once


using std::map;
using std::vector;
#include <map>
#include <vector>


enum eTIME_LINE_LIST_ID
{
	eTLLI_ITEM_UPGRADE_LVL_3 = 0,
	eTLLI_ITEM_UPGRADE_LVL_4,
};


struct sTIME_LINE_BLOCK
{
	// allocated time for this time block
	RwReal _Time;

	// activation switch flag
	RwBool _Switch;

	sTIME_LINE_BLOCK::sTIME_LINE_BLOCK()
	{
		_Time	= 0.0f;
		_Switch	= FALSE;
	}
};

class CTimeLineActor
{
public:
	CTimeLineActor();
	virtual ~CTimeLineActor();

private:
	RwInt32 m_CurTLBIdx;
	RwReal	m_CurTLBAccumulatedTime;
};

class CNtlTimeLine
{
public:
	CNtlTimeLineContainer();
	virtual ~CNtlTimeLineContainer();

private:
	// time line block vector
	typedef std::vector<sTIME_LINE_BLOCK>	tTLB_VEC;
	typedef tTLB_VEC::iterator				tTLB_VEC_ITER;

	// time line map
	typedef std::map<eTIME_LINE_LIST_ID, tTLB_VEC>	tTLL_MAP;
	typedef tTLL_MAP::iterator						tTLL_MAP_ITER;

private:
	VOID RegisterTLL();

public:
	VOID Create();
	VOID Destroy();
};*/