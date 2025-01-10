#include "precomp_ntlpresentation.h"
#include "NtlEffectAPI.h"
#include "NtlPLUVAnim.h"
#include "NtlTimer.h"
#include "NtlPLResourcePack.h"
#include "NtlPLApi.h"

CNtlPLUVAnim::CNtlPLUVAnim(void)
: m_fUVAnimTime(1.0f)
{
    m_pUvAnimDict = NULL;
    m_pAnimationMaterialList = NULL;
}

CNtlPLUVAnim::~CNtlPLUVAnim(void)
{
    Destory();
}

void CNtlPLUVAnim::Destory() 
{
    if (m_pAnimationMaterialList != NULL)
    {
        rwSListDestroy(m_pAnimationMaterialList);
        m_pAnimationMaterialList = NULL;
    }

    if (m_pUvAnimDict != NULL)
    {
        RtDictDestroy(m_pUvAnimDict);
        m_pUvAnimDict = NULL;
    }

    m_fUVAnimTime = 1.0f;
}


/**
 *Create UVAnim from uva file and apply to Clump.
 * \param szUVFileName uvAnim file path to apply (*.uva)
 * \param pClump Clump object to apply UVAnim to.
 *Whether return is successful or not
 */
RwBool CNtlPLUVAnim::Create( const char* szUVFileName, RpClump* pClump ) 
{
    if(!szUVFileName || !pClump)
        return FALSE;

    if(m_pUvAnimDict)
    {
        Destory();
    }

    m_pUvAnimDict = UVAnimDictLoad(szUVFileName);
    if(!m_pUvAnimDict)
        return FALSE;

    RtDictSchemaSetCurrentDict(RpUVAnimGetDictSchema(), m_pUvAnimDict);    
    
    m_pAnimationMaterialList = rwSListCreate(sizeof(RpMaterial*), rwID_NAOBJECT);
    API_AddAnimatedMaterialsList(m_pAnimationMaterialList, pClump);

    return TRUE;
}

RwBool CNtlPLUVAnim::Create( const char* szUVFileName ) 
{
    if(m_pUvAnimDict)
    {
        Destory();
    }

    m_pUvAnimDict = UVAnimDictLoad(szUVFileName);
    if(!m_pUvAnimDict)
        return FALSE;


    // NOTE: When there are two or more objects on the screen with different UVAnim applied. Could there be a problem?
    // Check the results later and correct any problems.
    // A change may be needed in the form of adding Entry(RpUVAnim*) to the DIct object.
    RtDictSchemaSetCurrentDict(RpUVAnimGetDictSchema(), m_pUvAnimDict);    

    return TRUE;
}

RwBool CNtlPLUVAnim::SetClump( RpClump* pClump ) 
{
    m_pAnimationMaterialList = rwSListCreate(sizeof(RpMaterial*), rwID_NAOBJECT);
    API_AddAnimatedMaterialsList(m_pAnimationMaterialList, pClump);

    return TRUE;
}

void CNtlPLUVAnim::Update( RwReal fElapsedTime ) 
{
    if(!m_pAnimationMaterialList)
        return;

    //API_MaterialsInterpolatorsAddAnimTime(m_pAnimationMaterialList, fElapsedTime * m_fUVAnimTime);    

 // SetCurrentTime, not AddTime, because the Material is common to multiple clumps.
    API_MaterialsInterpolatorsSetCurrentTime(m_pAnimationMaterialList, CNtlTimer::GetAppTime() * m_fUVAnimTime);

    API_MaterialsAnimApply(m_pAnimationMaterialList);
}

/**
 *Load the *.uva file.
 * \param strFullName uva file path
 *return RtDict object that loaded the uva file
 */
RtDict* CNtlPLUVAnim::UVAnimDictLoad( const RwChar* strFullName ) 
{
	return API_PL_LoadUVAnimDict(strFullName);
}


