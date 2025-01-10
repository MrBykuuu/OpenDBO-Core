#pragma once

#include "NtlInstanceImVertexSystem.h"

class CNtlPLItem;

///< Number of Bone pairs with Grade Effect
enum EGradeBoneIndex
{
    GRADE_EFFECT_BONE1,     ///< Attach to the first pair of bones. (General weapons such as knives)
    GRADE_EFFECT_BONE2,     ///< Attach to the second pair of bones. (Glove, claw.)
};

/**
 * \ingroup NtlPresentation
 * \brief Class that implements the Upgrade Effect of Item (NOTE: Used alone without Resource class.)
 *Effects not created in Effect Tool
 * \date 2006-12-13
 * \author agebreak
 */
class CNtlInstanceUpgradeEffectSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceUpgradeEffectSystem(void);
    virtual ~CNtlInstanceUpgradeEffectSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(CNtlPLItem* pItem, EGradeBoneIndex eBoneIndex = GRADE_EFFECT_BONE1);
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();

    void    SetTexture(RwChar* szTextureName);             ///< Set the texture.    
protected:
    virtual void Init();
    virtual void Reset();

    RwBool  UpdateVertices(RwReal fElapsedTime);

private:
    // This Create function is not used.
    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld) {return TRUE;}

protected:
    CNtlPLItem*     m_pItem;                        ///< Pointer to the Item object with Effect attached    
    RwReal          m_fEffetWidthRatio;
    RwBool          m_bWidthSizeUp;    
    EGradeBoneIndex      m_eGradeBone;                   ///< Setting value for which bone to attach to
};
