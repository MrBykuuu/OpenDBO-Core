#pragma once


/**
 * \ingroup NtlPresentation
 * \brief Classes that handle UVAnim
 * \date 2006-07-11
 * \author agebreak
 */
class CNtlPLUVAnim
{
public:
    CNtlPLUVAnim(void);
    virtual ~CNtlPLUVAnim(void);

    RwBool Create(const char* szUVFileName);
    RwBool SetClump(RpClump* pClump);
    RwBool Create(const char* szUVFileName, RpClump* pClump);       ///< Create UVAnim.
    void   Destory();                                               ///< Releases the allocated memory.
    void   Update(RwReal fElapsedTime);                             ///< Update UVAim.
    void   SetUVAnimSpeed(RwReal fSpeed) {m_fUVAnimTime = fSpeed;}  ///< Specifies the speed of UVAnim

protected:
    RtDict* UVAnimDictLoad(const RwChar* strFullName);              ///<Load the *.uva file.

protected:
    RtDict*							m_pUvAnimDict;                 ///< UVAnim dict object
    RwSList*						m_pAnimationMaterialList;      ///< Material list to apply UVAnim to
    RwReal                          m_fUVAnimTime;                 ///< UVAnim Speed
    
    
};
