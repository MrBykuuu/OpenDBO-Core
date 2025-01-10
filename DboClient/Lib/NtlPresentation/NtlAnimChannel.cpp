#include "precomp_ntlpresentation.h"
#include "NtlAnimChannel.h"
#include "NtlDebug.h"

/**
 *Create AnimChannel
 * \param pBaseHierarchy Pre-generated BaseHierarchy
 * \return Success or not
 */
RwBool CNtlAnimChannel::Create(RpHAnimHierarchy *pBaseHierarchy)
{
	NTL_FUNCTION("CNtlAnimChannel::Create()");
	NTL_ASSERTE(pBaseHierarchy != NULL);
	NTL_ASSERTE(pBaseHierarchy->currentAnim->pCurrentAnim != NULL);
	if(pBaseHierarchy == NULL || pBaseHierarchy->currentAnim->pCurrentAnim == NULL)
		NTL_RETURN(FALSE);

	m_pHierarchy = RpHAnimHierarchyCreateFromHierarchy( pBaseHierarchy, 
			                                            RpHAnimHierarchyGetFlags(pBaseHierarchy),
			                                            pBaseHierarchy->currentAnim->maxInterpKeyFrameSize);
	
	NTL_ASSERTE(m_pHierarchy);
	if(m_pHierarchy == NULL)
		NTL_RETURN(FALSE);

	//If there is no Default Anim, an error occurs when calculating AddTogether.
	RpHAnimHierarchySetCurrentAnim( m_pHierarchy, pBaseHierarchy->currentAnim->pCurrentAnim);
	
	NTL_RETURN(TRUE);
}

/**
 * Animation Update
 * \return Success or not
 */
RwBool CNtlAnimChannel::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlAnimChannel::Update");
	RpHAnimHierarchyAddAnimTime(m_pHierarchy, fElapsed);
	NTL_RETURN(TRUE);
}

/**
 * Destroy
 */
void CNtlAnimChannel::Destroy()
{
	NTL_FUNCTION("CNtlAnimChannel::Update");
	if(m_pHierarchy)
	{
		RpHAnimHierarchyDestroy(m_pHierarchy);
		m_pHierarchy = NULL;
	}

	NTL_RETURNVOID();
}

/**
 * Obtain the AnimChannel hierarchy.
 * \return RpHANimHierarchy
 */
RpHAnimHierarchy *CNtlAnimChannel::GetHierarchy()				
{
	NTL_ASSERTE(m_pHierarchy != NULL);
	return m_pHierarchy; 
}

/**
 * Apply Animation Data.
 * \param 
 * \return RpHANimHierarchy
 */
void CNtlAnimChannel::SetAnimData(SInstanceAnimData *pInstanceAnimData)	
{
	NTL_ASSERTE(pInstanceAnimData);
	NTL_ASSERTE(pInstanceAnimData->pResource);

	m_pAnimData = pInstanceAnimData;
	RpHAnimHierarchySetCurrentAnim(m_pHierarchy, pInstanceAnimData->pResource->GetAnimation());
}

SInstanceAnimData *CNtlAnimChannel::GetAnimData()				
{
	NTL_ASSERTE(m_pAnimData);
	return m_pAnimData;
}

