/*****************************************************************************
 *
 * File			: NTLAnimSet.h
 * Author		: HongHoDong
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 20	
 * Abstract		: NTL PLAnimSet
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_ANIM_SET_
#define _CNTL_PL_ANIM_SET_

/*
#include <rwcore.h>
#include <rtanim.h>
#include <rpworld.h> 
#include <rtquat.h> 
#include <rtanim.h>
#include <rphanim.h>
#include <vector>
*/

#include "NtlDebug.h"
#include "NtlPLCharacterAnimTable.h"

/**
 * \ingroup Client
 *This is a class that handles animation-related processing.
 * 
 */

enum EAnimChannel
{
	ANIM_CHANNEL_CURRENT = 0,
	ANIM_CHANNEL_NEXT,
	ANIM_CHANNEL_END,
};

enum EBlendType
{
	BLEND_MIX,		//The previous animation is played, and the current animation is also played while interpolating.
	BLEND_TWEEN		//The previous animation stops and interpolation is performed up to the start of the current animation.
};


struct SAnimChannel
{
	RpHAnimHierarchy	*pHierarchy;
	RwBool				bLoop;				//Is it an animation that needs to operate in a loop?
	RwBool				bNotLoopEnd;		//Is the animation, not a loop, finished?
	RwReal				fAnimSpeed;         //Animation Speed
	unsigned int		uiAnimKey;

	SAnimData			*pAnimData;

	SAnimChannel() 
	{ 
		pHierarchy      = NULL;
		bLoop			= FALSE;
		pAnimData		= NULL;
		bNotLoopEnd     = FALSE;
		fAnimSpeed      = 1.f;
	};
};

class CNtlPLCharacter;
class CNtlPLAnimSet
{
public:
	CNtlPLAnimSet();
	~CNtlPLAnimSet();
private:
	RpClump	*m_pTargetClump;						 //Target Clump
	CNtlPLCharacter *m_pOwner;                       //Class that currently owns the Class

	RpHAnimHierarchy *m_pMainHierarchy;				 //Character Main Hierarchy

	
	SAnimChannel		m_Channel[ANIM_CHANNEL_END]; //Anim Channel
	EBlendType			m_eBlendType;

	RwReal	m_fBlendAlpha;							 // Blend value (0.. 1)
	RwReal	m_fBlendTargetAlpha;					 // Target Blend
	RwReal	m_fBlendTargetInterval;					 // Time to blend (in seconds)

	RwBool	m_bAnimChanging;
	
	RwReal	m_fDefaultBlendAlpha;
	RwReal  m_fDefaultBlendTargetAlpha;
	RwReal	m_fDefaultBlendTargetInterval;
	
	RwBool	m_bActiveFlag;
	
	RwReal	m_fOldAnimTime;
private:
	void SendAnimEnd();
	void SendAnimHit();
	void SendFootStep();

public:
	void Create(RpClump *pClump, RtAnimAnimation *pDefaultAnim, CNtlPLCharacter *pOwner);
	void CreateAnimation(RtAnimAnimation *pDefaultAnim);


	void Destroy();
	void Update(float fElapsed);
	
	void	SetChangeAnimation(SAnimData *pAnimData, RwBool bLoop, unsigned int uiAnimKey);
	void	SetChangeChannelAnimation(EAnimChannel eChannel, RtAnimAnimation *pAnim);

	RwBool	SetChangeBlend(EBlendType blendType, RwReal fBlendTargetAlpha = 1.f, RwReal fBlendTargetInterval = 0.1f);
	RwBool	SetChangeAnimSpeed(RwReal fSpeed = 1.f);

	RpHAnimHierarchy *GetHierarchy() { NTL_ASSERTE(m_pMainHierarchy != NULL); return m_pMainHierarchy; }
};

#endif
