#pragma once

class CNtlSob;
class CNtlPLEntity;

/**
 * \ingroup NtlSimulation
 * \brief Class that manages shared target marks
 *
 * \date 2009-08-13
 * \author agebreak
 */
class CNtlShareTargetMark
{
public:
    // Shared target type
    enum EShareTargetType
    {
        SHARE_TARGET_NORMAL,        
        SHARE_TARGET_TARGET,        
        SHARE_TARGET_ATTACK,
        SHARE_TARGET_NONE,
    };

    CNtlShareTargetMark(CNtlSob* pSobObject);
    virtual ~CNtlShareTargetMark();

    void    CreateShareTargetMark(RwUInt8 bySlot, RwInt32 type);               ///< Create a shared target mark.
    void    DeleteShareTargetMark();                                            ///< Unmark the shared target.
    RwBool  IsShareTargeting();                                                 ///< Returns whether the shared target is set.
    RwUInt8 GetSlot() {return m_bySlot;}                                        ///< Returns the currently configured slot.    

protected:

protected:
    CNtlSob*            m_pSobObj;                  ///< Main entity to which the shared target will be attached
    CNtlPLEntity*       m_pShareTargetMark;         ///< Shared target effect
    RwUInt8             m_bySlot;                   ///< Shared target number (slot)
    EShareTargetType    m_type;                     ///< Shared target type  
    
};