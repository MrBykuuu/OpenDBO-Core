#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManager.h"
#include "NtlDebug.h"
#include "NtlPLLinkEffect.h"

CNtlPLLinkEffect::CNtlPLLinkEffect(void)
{
}

CNtlPLLinkEffect::~CNtlPLLinkEffect(void)
{
    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            m_vecLinkEffect[i]->SetVisible(FALSE);
            GetSceneManager()->DeleteEntity(m_vecLinkEffect[i]);        
            m_vecLinkEffect[i] = NULL;
        }        
    }

    m_vecLinkEffect.clear();
}

/**
 *Create and attach LinkEffect.
 * \param pAttach Parent object to attach to.
 * \param pEventLinkEffect Property object to create the effect
 * \param vLinkEffect Instance The vector to create the Effect.
 *return Created Effect object; returns NULL in case of failure.
 */
CNtlInstanceEffect* CNtlPLLinkEffect::AttachLinkEffect(CNtlPLAttach* pAttach, SEventLinkEffect* pEventLinkEffect)
{
    if(!pAttach || !pEventLinkEffect)
        return NULL;

    // Add an effect to the screen
    CNtlInstanceEffect* pEffect = NULL;
    if(pEffect == NULL)
    {
	//	DBO_WARNING_MESSAGE("pEventLinkEffect->chEffectName: " << pEventLinkEffect->chEffectName);
        pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventLinkEffect->chEffectName);
		if (!pEffect)
		{
			return NULL;
		}

        // Auto Delete effect cannot be used.
        if(pEffect->IsAutoDelete())
        {
            pEffect->SetVisible(FALSE);
            GetSceneManager()->DeleteEntity(pEffect);
            return NULL;
        }
    }

    // Attach.
    RwBool bSuccess = FALSE;
    if(pEventLinkEffect->bAttachBone)
    {
        SPLAttachAttr attr;
        attr.vOffsetPos = pEventLinkEffect->vOffsetPos;        
         bSuccess = pAttach->AttachBone(pEffect, pEventLinkEffect->chBoneName, &attr);
    }
    else
    {
		bSuccess = pAttach->AttachWorldPosAndRotate(pEffect, pEventLinkEffect->vOffsetPos);
    }

    if(!bSuccess)       // When attaching does not work properly (for example, there is no pattern...)
    {
        GetSceneManager()->DeleteEntity(pEffect);
        return NULL;
    }

    // Follows the parent's Visible property.
    pEffect->SetVisible(pAttach->IsVisible());

    m_vecLinkEffect.push_back(pEffect);    

    return pEffect;
}

/**
*Remove LinkEffect.
* \param pAttach Parent object to remove LinkEffect from.
* \param pEventLinkEffect LinkEffect to remove.
* \param vLinkEffect Instance The vector from which to remove the Effect.
*Whether return is successful or not
*/
RwBool CNtlPLLinkEffect::DetachLinkEffect(CNtlPLAttach* pAttach, CNtlInstanceEffect* pLinkEffect)
{
    if(!pAttach || !pLinkEffect)
        return FALSE;

    pAttach->Detach(pLinkEffect);
    pLinkEffect->SetVisible(FALSE);
    GetSceneManager()->DeleteEntity(pLinkEffect);    

    // Remove from the vector of instances.    
    VecLinkEffect::iterator it = m_vecLinkEffect.begin();
    for(; it != m_vecLinkEffect.end(); ++it)
    {
        CNtlInstanceEffect* pLinkEfffectInstance = *it;
        if(pLinkEffect == pLinkEfffectInstance)
        {
            m_vecLinkEffect.erase(it);
            break;
        }
    }
    
    return TRUE;
}


/**
 *Returns the instance effect corresponding to the name in the Effect instance list.
 * \param szName Name of the effect to find.
 *return The instance effect object found. If not found, NULL is returned.
 */
CNtlInstanceEffect* CNtlPLLinkEffect::GetLinkEffectFromName(RwChar* szName)
{
    if(!szName)
        return NULL;

    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            if(strcmp(m_vecLinkEffect[i]->GetName(), szName) == 0)
                return m_vecLinkEffect[i];
        }        
    }

    return NULL;
}

void CNtlPLLinkEffect::SetVisible(BOOL bVisible)
{
    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            m_vecLinkEffect[i]->SetVisible(bVisible);
        }
    }
}