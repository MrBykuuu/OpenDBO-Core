/*****************************************************************************
 *
 * File			: NtlPLEventGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 9. 29	
 * Abstract		: Presentation layer event function
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLEVENT_GENERATOR_H__
#define __NTL_PLEVENT_GENERATOR_H__

#include <rwcore.h>


class CNtlPLEventGenerator
{
public:
	//Anim Event
	static void AnimEnd(RwUInt32 uiSerialId, RwUInt32 uiBaseAnimKey, RwUInt32 uiUpperAnimKey);
	static void AnimHit(RwUInt32 uiSerialId, void *pData);
	static void AnimFootStep(RwUInt32 uiSerialId, void* pData);
	static void AnimWeightTime(RwUInt32 uiSeiralId, void *pData);
	static void AnimEventSubWeaponActive(RwUInt32 uiSerialId);
	static void AnimEventSubWeaponDeActive(RwUInt32 uiSerialId);
	static void AnimEventSummonActive(RwUInt32 uiSerialId);
    static void AnimEventAlpha(RwUInt32 uiSerialId, void* pData);           ///< Generates the alpha event of the animation
    static void AnimEventTMQ(RwUInt32 uiSerialID, RwBool bIn);              ///< Generates a TMQ entry event.
    static void AnimEventExplosion(RwUInt32 uiSerialID, void* pData);       ///< Generates an explosion event.
    static void AnimEventPostEffect(RwUInt32 uiSerialID, void* pData);      ///< Generates a post-effect event.    
    static void AnimEventDirect(RwUInt32 uiSerialID, void* pData);          ///< Generates a production event.
    static void AnimEventColorChange(RwUInt32 uiSerialID, void* pData);     ///< Generates a color change event.
    static void AnimEventStretch(RwUInt32 uiSerialID, void* pData);         ///< Arm stretching event occurs.
    static void AnimEventTrigger(RwUInt32 uiSerialID, void* pData);         ///< Trigger event
	static void AnimEventSkillCancel(RwUInt32 uiSerialID);

	// world field switching effects
	static void IsAnotherFieldChanged(void* pData);
	static void IsAnotherFieldChangedWithoutDelay(void* pData);
	static void IsAnotherBlockChanged(void* pData);
	static void IsAnotherBlockChangedWithoutDelay(void* pData);
	static void IsAnotherObjectChanged(void* pData);
	static void IsAnotherObjectChangedWithoutDelay(void* pData);
	static void IsAnotherSectorChangedWithoutDelay(void* pData);
	static void OnMapNameChange(void* pData);

	// trigger doodads
	static void OnTriDoodadsEvent(void* pData);

    // Event Object
    static void CreateEventObject(RwBool bCreate, void* pData);
    static void CreateEventThreadLoading(RwUInt32 uiSerialID);

	// gui event
	static void Resize(RwInt32 iWidht, RwInt32 iHeight, RwBool bRestore_from_Minimize);

	static void CinematicViewShow(RwBool bShow);
	static void CinematicBalloonShow(RwUInt32 hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwUInt8 byPlayType);
	static void CinematicBalloonHide(void);
	static void CinematicFlashShow( RwChar* pFilename, RwBool bUseTime,	RwReal fLifeTime, void* pEchoData,
									RwReal fBeginAlphaTime = 0.0f, RwReal fEndAlphaTime = 0.0f, RwUInt32 flagPlayTimeType = 0 );
	static void CinematicFlashHide(void);
	static void CinematicEcho(void* hCinematic);
	static void ResetCinematic(RwUInt32 eResetDemandObject);

	static void CinematicFadeIn(RwUInt8 byType, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byEndAlpha);
	static void CinematicFadeOut(RwUInt8 byType, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byEndAlpha);

	static void CinematicBreak(void);

	static void ShowProfilerGui(RwBool bShow);

	static void ShowSpectatorBalloon( RwV3d& vPos, WCHAR* szMessage, RwReal fLifeTime, RwReal fFadeInTime = 0.0f, RwUInt8 byBalloonType = 0 );
	static void HideAllSpectatorBalloon(void);

	static void ShowPlayerName(RwUInt32 hSerialId, RwBool bShow);

	static void EventDirectPlayCancel(RwUInt32 hSerialId);
};  


#endif