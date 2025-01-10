#pragma once

#include "NtlPLAttach.h"
#include "NtlAnimEventData.h"

typedef std::vector<CNtlInstanceEffect*> VecLinkEffect;
/**
 * \ingroup NtlPresentation
 * \brief Link Effect class (effect that remains attached to Model) 
 * \date 2006-07-14
 * \author agebreak
 */
class CNtlPLLinkEffect
{
public:
    CNtlPLLinkEffect(void);
    virtual ~CNtlPLLinkEffect(void);
    
    CNtlInstanceEffect* AttachLinkEffect(CNtlPLAttach* pAttach, SEventLinkEffect* pEventLinkEffect);    
    RwBool DetachLinkEffect(CNtlPLAttach* pAttach, CNtlInstanceEffect* pLinkEffect);
    CNtlInstanceEffect* GetLinkEffectFromName(RwChar* szName);          ///< Searches for and returns LinkEffect Instance by name. (Used only in model tools)
    void SetVisible(BOOL bVisible);         ///< Sets the Visible of all LinkEffects. 

public:
    VecLinkEffect m_vecLinkEffect;          ///< Vector of Link Effect Instances

};
