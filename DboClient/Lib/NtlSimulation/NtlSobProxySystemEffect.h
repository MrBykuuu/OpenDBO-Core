#pragma once

#include "NtlSoundDefines.h"

class CNtlSobProxy;
class CNtlPLEntityAlphaWeightBlend;

//////////////////////////////////////////////////////////////////////////
class CNtlSobProxySystemEffect
{
public:
    enum EProxySystemEffectType
    {
        PROXY_SYSTEM_EFFECT_ALPHA_BLEND,
        PROXY_SYSTEM_EFFECT_COLOR,
        PROXY_SYSTEM_EFFECT_COLOR_FLICKER,
        PROXY_SYSTEM_EFFECT_COLOR_ADD,
        PROXY_SYSTEM_EFFECT_SCALE,        

        PROXY_SYSTEM_EFFECT_COUNT,
    };

public:
    CNtlSobProxySystemEffect();
    CNtlSobProxySystemEffect(CNtlSobProxy *pSobProxy);
    virtual ~CNtlSobProxySystemEffect();

    virtual void	Enter(void) {}
    virtual void	Exit(void) {}
    virtual void	Update(RwReal fElapsed) {}

    virtual RwBool	IsFinish(void)				{ return FALSE; }
    virtual RwBool	IsCalcWeightValue(void)		{ return FALSE; }

    virtual EProxySystemEffectType GetType()    {return m_eType;}

    void            SetExtraSound(SOUND_HANDLE hSound) {m_hSound = hSound;}            ///< Sets additional sound handles.

protected:
    CNtlSobProxy*           m_pSobProxy;
    EProxySystemEffectType  m_eType; 
    SOUND_HANDLE            m_hSound;                          ///< Additional sound handles
};

//////////////////////////////////////////////////////////////////////////
class CNtlSobProxySystemEffectAlphaBlend : public CNtlSobProxySystemEffect
{
private:

    CNtlPLEntityAlphaWeightBlend *m_pWeightAlpha;
	RwBool			m_bExceptDecorationProxy;

public:

    CNtlSobProxySystemEffectAlphaBlend();
    CNtlSobProxySystemEffectAlphaBlend(CNtlSobProxy *pSobProxy);
    ~CNtlSobProxySystemEffectAlphaBlend();

    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void	Update(RwReal fElapsed);

    RwBool			IsFinish(void);
    RwBool			IsCalcWeightValue(void);

    RwReal			GetWeightValue(void);

	RwBool			IsExceptDecorationProxy(void);

    void			SetData(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE);
    void			SetData(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE );
};

//////////////////////////////////////////////////////////////////////////

class CNtlSobProxySystemEffectColor : public CNtlSobProxySystemEffect
{
protected:

    RwUInt8		m_byRed;
    RwUInt8		m_byGreen;
    RwUInt8		m_byBlue;

    RwReal		m_fWeightRed;
    RwReal		m_fWeightGreen;
    RwReal		m_fWeightBlue;

    RwUInt8		m_byUpdateCount;

    RwBool      m_bAddColorFlag;                    ///< Flag whether to add colors rather than blend them

protected:
    void			SetWeightValue(RwReal fWeightRed, RwReal fWeightGreen, RwReal fWeightBlue);

public:

    CNtlSobProxySystemEffectColor();
    CNtlSobProxySystemEffectColor(CNtlSobProxy *pSobProxy);
    ~CNtlSobProxySystemEffectColor();

    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void	Update(RwReal fElapsed);

    RwBool			IsFinish(void);
    RwBool			IsCalcWeightValue(void);

    void			GetWeightValue(RwReal& fWeightRed, RwReal& fWeightGreen, RwReal& fWeightBlue);
    void			SetData(RwInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwBool bAddColor = FALSE);
    RwBool          IsAddColorFlag() {return m_bAddColorFlag;}

};

//////////////////////////////////////////////////////////////////////////
class CNtlSobProxySystemEffectColorFlickering : public CNtlSobProxySystemEffectColor
{
private:

    RwBool		m_bOn;
    RwReal		m_fTime;
    RwReal		m_fFlickerTime;

public:

    CNtlSobProxySystemEffectColorFlickering();
    CNtlSobProxySystemEffectColorFlickering(CNtlSobProxy *pSobProxy);
    ~CNtlSobProxySystemEffectColorFlickering();


    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void	Update(RwReal fElapsed);

    RwBool			IsCalcWeightValue(void);

    void			SetData(RwInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fFlickerTime);
};

//////////////////////////////////////////////////////////////////////////
///Effect that changes size
class CNtlSobProxySystemEffectScale : public CNtlSobProxySystemEffect
{
public:
    CNtlSobProxySystemEffectScale();
    CNtlSobProxySystemEffectScale(CNtlSobProxy* pSobProxy);
    virtual ~CNtlSobProxySystemEffectScale();

    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void	Update(RwReal fElapsed);

    virtual RwBool	IsFinish(void);
    virtual RwBool	IsCalcWeightValue(void);

    RwReal          GetWeightValue();
    void            SetData(RwReal fEndScale, RwReal fTime);            ///< Set scale change and time.
protected:
    RwReal          m_fWeightValue;                                     ///< current value
    RwReal          m_fStartValue;                                      ///< starting value
    RwReal          m_fEndValue;                                        ///< end value
    RwReal          m_fTime;                                            ///< time
    RwReal          m_fElapsedTime;                                     ///< Total elapsed time
    RwBool          m_bFinish;                                          ///< Whether application has been completed
};

