/*****************************************************************************
 *
 *File			: NTLAnimLayer.h
 *Author		: HongHoDong
 *Copyright	: (¡÷)NTL
 *Date			: 2006. 4. 11	
 *Abstract		: NTL NtlAnimLayer
 *****************************************************************************
 *Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PL_ANIM_LAYER_H__
#define __NTL_PL_ANIM_LAYER_H__

#include "NtlInstanceAnimData.h"
#include "NtlAnimChannel.h"
#include "NtlCallbackWidget.h"

enum EAnimChannel
{
	ANIM_CHANNEL_CURRENT = 0,
	ANIM_CHANNEL_NEXT,
	ANIM_CHANNEL_END,
};

enum EBlendType
{
	BLEND_MIX,							///< Interpolation is performed by playing Current and Playing Next.
	BLEND_TWEEN							///< Stop Current and interpolate until Start of Next.
};

struct SBlendData
{
	RwReal		fBlendAlpha;			///< Current Blend∞™(0.f -1.f)
	RwReal		fBlendTargetAlpha;		///< Target Blend value
	RwReal		fBlendTargetInterval;	///< change amount
	EBlendType	eBlendType;				///< BlendType

	SBlendData() : fBlendAlpha(0.f), fBlendTargetAlpha(1.f), fBlendTargetInterval(0.1f),
	               eBlendType(BLEND_TWEEN) {}
};

/*!
 *\Animation
 *Class to handle animation blending	
 * 
 */
class CNtlAnimLayer
{
public:
	CNtlAnimLayer();
	virtual ~CNtlAnimLayer() {};

protected:
	RpHAnimHierarchy	*m_pBaseHierarchy;					///< Base Hierarchy Reference
	CNtlAnimChannel		m_AnimChannel[ANIM_CHANNEL_END];	///< Blending: Up to 2
	
	SBlendData			m_CurBlendData;						///< Current Blend value
	SBlendData			m_DefaultBlendData;					///< Default Blend ∞™
	RwBool				m_bBlendChange; 					///< Is it blending?
	RwInt32				m_iLastSendEventIndex;				///< Last sent Event Index
	RwReal				m_fLastSendEventTime;				///< Time of last sent Event

	CNtlCallbackVPParam1	*m_pCallBack;

public:
	RwBool	Create(RpHAnimHierarchy *pBaseHierarchy);
	RwBool	Update(float fElapsed);
	void	Destroy();
	
	RwBool	SetAllAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime = 1.f, RwBool bLoop = TRUE);
	RwBool	SetAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime = 1.f, RwBool bLoop = TRUE, RwBool bBlending = TRUE);
	RwBool	SetBlend(EBlendType eBlendType, RwReal fBlendAlpha = 0.f, RwReal fBlendInterval = 0.1f);
	
	RwReal	GetCurrentAnimTime();
    void    SetCurrentAnimTime(RwReal fCurrTime);				///< Set the current play time of the animation. (By agebreak 2006.05.01)
	RwReal	GetDurationAnimTime();								///< Total time of animation

	RwBool	GetCurrentAnimEnd();

	RwBool	IsBlendEnable(void);

    void    OnAfterAnimEventRun();                              ///< Executes all Anim Events after the current time.
	
	template <class Callbackclass>
	void SetCallBack(Callbackclass *cbclass,int (Callbackclass::*callback)(void* EventData))
	{
		m_pCallBack = NTL_NEW CNtlCallbackVPWidget1<Callbackclass> (cbclass,callback);
	}

};

#endif
