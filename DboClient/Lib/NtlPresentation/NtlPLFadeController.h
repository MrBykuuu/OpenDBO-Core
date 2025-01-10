/*****************************************************************************
*
* File			: NtlPLFadeController.h
* Author		: agebreak
* Copyright		: NTL Co., Ltd.
* Date			: 2007. 11. 21	
* Abstract		: Controller for values ​​for Fade In/Out
*****************************************************************************
* Desc          : Entity has a controller called AlphaWeight, but other than Entity,
*                Create a new one because it cannot be applied.
*
*****************************************************************************/

#pragma once

/// Controller for calculating fade values
class CNtlPLFadeController
{
public:
    CNtlPLFadeController(void);
    virtual ~CNtlPLFadeController(void);

    void Set(RwReal fOrgValue, RwReal fDestValue, RwReal fFadeTime);
    void Update(RwReal fElapsedTime);
    
    RwReal GetValue() {return m_fValue;}    ///< Returns the current Fade value.
    RwBool IsEnd() {return m_bEnd;}         ///< Returns whether Fade has ended.

protected:
    RwReal m_fOrgValue;                     ///< Original value
    RwReal m_fDestValue;                    ///< end value
    RwReal m_fValue;                        ///< Current value
    RwReal m_fFadeTime;                     ///< Fade Time
    RwReal m_fTime;                         ///< Current Time
    RwBool m_bEnd;                          ///< Whether Fade is finished
};
