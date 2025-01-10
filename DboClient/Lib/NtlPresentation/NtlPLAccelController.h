/*****************************************************************************
*
* File			: NtlPLAccelController
* Author		: agebreak
* Copyright		: NTL Co., Ltd.
* Date			: 2007. 12. 29	
* Abstract		: Controller for calculating acceleration
*****************************************************************************
* Desc          : Controller for calculating acceleration used in UI slides, etc.
*
*****************************************************************************/

#pragma once

/// acceleration calculation controller
class CNtlPLAccelController
{
public:
    CNtlPLAccelController(void);
    ~CNtlPLAccelController(void);
	
    void    SetAccel(RwReal fStartVal, RwReal fEndVal, RwReal fStartVelocity, RwReal fAccel);       /// Set the value.
    RwBool  Update(RwReal fElapsed);                           ///< Update
	void	Stop();
    RwReal  GetCurrentValue() {return m_fCurrentVal;}          ///< Current value
    RwReal  GetCurrentVelocity() {return m_fCurrVel;}          ///< current speed
	RwReal	GetCurrentProgress();								///< Current progress based on 0.0 (start) to 1.0 (end)

protected:
    RwReal  m_fStartVal;                      ///< starting value
    RwReal  m_fEndVal;                        ///< end value
    RwReal  m_fCurrentVal;                    ///< Current value
    RwReal  m_fStartVelocity;                 ///< Startup speed
    RwReal  m_fAccel;                         ///< acceleration
    RwReal  m_fCurrVel;                       ///< current speed
	RwReal	m_fCurrProgress;				  ///< Current progress (0.0f ~ 1.0f)
    RwBool  m_bUpdate;                        ///< Update presence flag
};
