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


#ifndef __NTL_SOB_PROXY_H__
#define __NTL_SOB_PROXY_H__

#include "ceventhandler.h"
#include "NtlMath.h"
#include "NtlSobProxyDef.h" 


class CNtlSobProxy : public RWS::CEventHandler
{
public:

	typedef std::list<CNtlPLEntity*>						ListPLEntity;
	typedef std::list<CNtlSobProxySystemEffect*>		    ListProxySystemEffect;	

protected:

	RwUInt32						m_uiSystemFlags;
	RwUInt32						m_uiCompType;
	CNtlSob							*m_pSobObj;
	ListPLEntity					m_listPLChild;
	
	RwUInt8							m_byAlpha;
	CNtlSobProxySystemEffectColor	*m_pWorldLight;
	ListProxySystemEffect			m_listProxySystemEffect;	
    
    RwBool                          m_bEnableCameraAlpha;
    CNtlSobProxySystemEffect*       m_pCameraAlphaEffect;

	bool							m_bDisableAniChange;

protected:

	void							ResetRemoveSystemEffectAlpha(void);

	void							UpdateSystemEffectColor(RwReal fElapsed);
	void							UpdateSystemEffectAlpha(RwReal fElapsed);	
    void							UpdateSystemEffectScale(RwReal fElapsed);

public:

	CNtlSobProxy();
	virtual ~CNtlSobProxy();

	virtual RwBool	Create(RwUInt32 uiCompType);
	virtual void	Destroy(void);
	virtual void	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg) {}

	
	
public:

	void			SetDisableAniChange(bool bFlag) { m_bDisableAniChange = bFlag; }
	bool			GetDisableAniChange() { return m_bDisableAniChange; }

//---------------------------------------------------
	//Function related to proxy system flag settings.
	//---------------------------------------------------

	virtual	void	EnableProxySystemVisible(RwBool bVisible);
	RwBool			IsProxySystemVisible(void);

	virtual void	EnableProxySystemGUILPVisible(RwBool bVisible);
	RwBool			IsProxySystemGUILPVisible(void);

//---------------------------------------------------
	//Function related to owner simulation object settings.
	//---------------------------------------------------

	void			SetSobObj(CNtlSob *pSobObj);
	void			SetCompType(RwUInt32 uiCompType);
	RwUInt32		GetCompType(void) const;

	virtual void	NotifyStateEnter(RwUInt32 uiStateId)	{}
	virtual void	NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)		{}

//---------------------------------------------------
	//Presentation entity creation function.                                                                      
	//---------------------------------------------------

// Create effect without attaching.
	CNtlPLEntity*			CreatePLEffect(const RwChar *pKey, const RwV3d *pPos = NULL, RwBool bIgnoreVisible = FALSE );

// Create effect without attaching
	//Add to child list.
	CNtlPLEntity*			CreatePLChildEffect(const RwChar *pKey, RwBool bIgnoreVisible = FALSE);
	
// Create effect while attaching bone.
    virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset = ZeroAxis, RwBool bIgnoreVisible = FALSE) {return NULL;}

// Create effect by attaching to offset position.
	virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible = FALSE, RwBool bApplyRotate = FALSE)  { return NULL; }

// Create effect by attaching to offset position.
	CNtlPLEntity*			CreatePLDamageBox(RwV3d vPos, RwUInt32 uiDamage, RwBool bHealing = FALSE);

// Delete the effect created as a child.
	virtual void			DeletePLChildEffect(CNtlPLEntity *pPLEntity);

//---------------------------------------------------
	//Functions related to the main presentation entity 
	//---------------------------------------------------

/**
	*Functions related to add and remove in world.
	*/

	virtual void			AddWorld(void)		{}
	virtual void			RemoveWorld(void)	{}

/**
	* Function related to transform.
	*/

	virtual void			SetPosition(const RwV3d *pPos)	{}
	virtual RwV3d			GetPosition(void);

	virtual void			SetDirection(const RwV3d *pDir) {}
	virtual RwV3d			GetDirection(void);

	virtual void			SetAngleY(RwReal fAngle)		{}
	virtual void			SetDeltaAngleY(RwReal fAngle)	{}
	virtual void			SetAngleX(RwReal fAngle) {}
	virtual void			SetDeltaAngleX(RwReal fAngle) {}

	virtual void			SetScale(RwReal fScale)			{}

/**
	*Function implemented in material.
	*/

	virtual void			SetAlpha(RwUInt8 byAlpha);
	
	RwUInt8					GetAlpha(void);

	void					AddAlpha(RwUInt8 byAddValue);

	virtual void			SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) {} 

	void					EnableWorldLight(RwBool bEnable, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	virtual	void			SetInkThicknessWeight(RwReal fWeightValue = 1.0f) {}

	virtual void			SetInkThickness(RwReal fValue = NTL_INK_THICKNESS) {}

	virtual void			SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) {}
	
/**
	*Functions related to animation.
	*/
	
// animation setting
	virtual void			SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f) {}

	virtual RwBool			IsExistBaseAnimation(RwUInt32 uiAnimKey) { return FALSE; }

// Retrieves the currently playing animation key.
	virtual RwUInt32		GetBaseAnimationKey(void)		{ return 0; }

	virtual RwBool			IsBaseAnimationEnd(void)		{ return TRUE; }

	virtual void			SetAnimSpeed(RwReal fAniSpeed)	{}
    virtual RwReal          GetAnimSpeed()					{return 1.0f;}

	virtual void			SetAnimBlendNoneOneTime(void)	{}

/**
	*Functions related to decoration
	*/

//Change the name color at the top of the entity
	virtual void			SetNameColor(const WCHAR* pwcName, COLORREF color) {}

	virtual void			SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor) {}

	virtual void			SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
											RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
											RwUInt8 byTypeC, RwUInt8 byTypeCColor) {}

	virtual void			SetDecorationProxyAlpha(RwUInt8 byAlpha) {}
	virtual void			EnableDecorationProxyVisible(RwBool bShow) {}

/**
	* Function regarding entity information.
	*/

// Function to get the main entity.
	virtual CNtlPLEntity*	GetPLMainEntity(void) { return NULL; }

// Get the bone position.
	virtual RwV3d			GetBonePosition(const RwChar *pBoneName);

// bone position of the equipped weapon
	virtual RwV3d			GetWeaponBonePosition(const RwChar *pBoneName);

// Bone position of the mounted sub weapon
	virtual RwV3d			GetSubWeaponBonePosition(const RwChar *pBoneName);

//Get the width of pl entity.
	virtual RwReal			GetPLEntityWidth(void); 

// Get the height of pl entity.
	virtual RwReal			GetPLEntityHeight(void);

// Get the depth of pl entity.
	virtual RwReal			GetPLEntityDepth(void); 

// Obtain the base scale of pl entity.
	virtual RwReal			GetPLEntityBaseScale(void);

// Obtains a randomly adjusted scale.
    virtual RwReal          GetScale();

/**
	*Functions related to child presentation entity  
	*/

//Add child entity.
	void					AddPLChildEntity(CNtlPLEntity *pPLEntity);

// Get the number of child entities.
	RwInt32					GetPLChildEntityCount(void) const;

// child entity transform.
	void					TransformPLChildEntity(RwMatrix& mat);

/**
	* dynamic effect control.
	*/
	CNtlSobProxySystemEffect*	AddVisualSystemEffectAlphaBlend(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE);	
	CNtlSobProxySystemEffect*	AddVisualSystemEffectAlphaBlend(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE);	
	void						RemoveVisualSystemEffectAlpha(CNtlSobProxySystemEffect *pSystemEffect);	

    CNtlSobProxySystemEffect*   AddVisualSystemEffectColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwBool bAddColor = FALSE);
    void                        RemoveVisualSystemEffectColor(CNtlSobProxySystemEffect* pSystemEffect);

	CNtlSobProxySystemEffect*	AddVisualSystemEffectColorFlickering(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fFlickerTime);
	void						RemoveVisualSystemEffectColorFlickering(CNtlSobProxySystemEffect *pSystemEffect);	

    CNtlSobProxySystemEffect*   AddVisualSystemEffectScale(RwReal fEndScale, RwReal fTime);    

    RwBool                      IsExitSystemEffect(CNtlSobProxySystemEffect* pSystemEffect);
	
//////////////////////////////////////////////////////////////////////////
	virtual void EnableVisible(RwBool bEnable);
	virtual void EnablePicking(RwBool bEnable) {}
	virtual void EnableShadowRendering(RwBool bEnable) {}
	virtual void EnableHighlight(RwBool bEnable) {}	
	virtual void CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue);
	virtual void DeleteElapsedController(void);
    virtual void EnableCameraAlpha(RwBool bEnable);             ///< Apply alpha according to camera distance.

//------------------------------------------------------------------------------------------------------------
// Function related to event.  
//------------------------------------------------------------------------------------------------------------

public:

	
// When mouse focus is received.
	virtual void SetFocus(void) {}

// When mouse focus is read.
	virtual void ReleaseFocus(void) {}

};

inline RwBool CNtlSobProxy::IsProxySystemVisible(void)
{
	if(m_uiSystemFlags & NTL_PROXYSYSTEM_NOT_VISIBLE)
		return FALSE;

	return TRUE;
}

inline RwBool CNtlSobProxy::IsProxySystemGUILPVisible(void)
{
	if(m_uiSystemFlags & NTL_PROXYSYSTEM_LP_VISIBLE)
		return FALSE;

	return TRUE;
}

inline void CNtlSobProxy::SetSobObj(CNtlSob *pSobObj)
{
	m_pSobObj = pSobObj;
}

inline void CNtlSobProxy::SetCompType(RwUInt32 uiCompType)
{
	m_uiCompType = uiCompType;
}

inline RwUInt32 CNtlSobProxy::GetCompType(void) const
{
	return m_uiCompType;
}

inline RwInt32 CNtlSobProxy::GetPLChildEntityCount(void) const
{
	return m_listPLChild.size();
}

// Get the width of pl entity.
inline RwReal CNtlSobProxy::GetPLEntityWidth(void)
{
	return 1.0f;
}

inline RwReal CNtlSobProxy::GetPLEntityHeight(void)
{
	return 2.0f;
}

// Retrieves the depth of pl entity.
inline RwReal CNtlSobProxy::GetPLEntityDepth(void)
{
	return 1.0f;
}

inline RwReal CNtlSobProxy::GetPLEntityBaseScale(void)
{
	return 1.0f;
}
inline RwReal CNtlSobProxy::GetScale()
{
    return 1.0f;
}


#endif