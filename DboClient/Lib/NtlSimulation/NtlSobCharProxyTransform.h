#pragma once

#include "NtlSLEvent.h"

class RWS::CEventHandler;
class CNtlSobActor;
class CNtlPLCharacter;
class CNtlPLItem;
class CNtlInstanceEffect;
class CNtlSobProxySystemEffect;
struct SEquipItem;



/**
* \ingroup NtlSimulation
* \brief Class that handles transformation-related visuals
*
* \date 2008-10-07
* \author agebreak
*/
class CNtlSobCharProxyTransform : public RWS::CEventHandler
{
public:
    CNtlSobCharProxyTransform(CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter);
    virtual ~CNtlSobCharProxyTransform(void);

    void	        Update(RwReal fElapsed);
    void	        HandleEvents(RWS::CMsg &pMsg);
    void            Destory();

    void            SetActor(CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter);          ///< Set the character. (if the pointer changes)        
    void            PostChangeAdult();                                                      ///< Function called after changing to adult/child
    RwBool          IsTransform()   {return m_bIsTransform;}        ///< Returns whether or not it has been transformed.    

    // Event function when entering/deleting the world
    void            AddWorld();
    void            RemoveWorld();

protected:
    // Event
    void            EventSobTransform(SNtlEventTransform* pData);     ///< Transformation event processing
    void            EventSobSkillAction(RWS::CMsg& pMsg);             ///< Processes skill use events.
    void            EventSobChangeAdult(RWS::CMsg& pMsg);             ///< Processes transformation into an adult.

    // Transform
    void            OnTransform_SuperSaiyan(RwBool bTransform);     ///< Processing of Super Saiyan transformation
    void            OnTransform_KaiOuKen(RwBool bTransform, BYTE byLevel);        ///< Gyewangkwon transformation processing
    void            OnTransform_BigNamek(RwBool bTransform);        ///< Processing of the giant Namek transformation
    void            OnTransform_PureMajin(RwBool bTransform);       ///< Pure Mine transformation processing
    void            OnTransform_Mob(RwUInt32 tblIdx);               ///< Mob transformation processing
    void            OnTransform_Candy(RwBool bTransform);           ///< Candy skill transformation processing
    void            DetachEffect();                                 ///< Cancellation of attachment effect when transformed

	void            OnTransform_CinematicScale(float fScale, float fTime);

    // Super Saiyan
    void            AttachEffectSuperSaiyan(CNtlPLCharacter* pPLCharacter);                      ///< Super Saiyan effect attached
    CNtlPLItem*     GetTransformItem();                             ///< Returns the Super Saiyan transformation item.

    // Kaioken
    void            AttachEffectKaiouKen(CNtlPLCharacter* pPLCharacter, BYTE byLevel);    ///< Creation of Kaioken effect    

    // Giant Namek
    void            DetachWeapon();                         ///< Unlocks the originally equipped weapon.
    void            AttachWeapon();                         ///< Equip the weapon that was originally equipped.
    void            ChangeWeapon(RWS::CMsg& pMsg);          ///< When changing weapons in transformation state

    // pure majin
    CNtlPLCharacter* CreatePureMajinModel();                ///< Creates and returns a pure mine model.

    // candy transformation
    CNtlPLCharacter* CreateCandyModel();                    ///< Creates and returns a candy model.

protected:
    CNtlSobActor*       m_pActor;                           ///< Actor object
    CNtlPLCharacter*    m_pPLCharacter;                     ///< Actor's PLEntity
    SNtlEventTransform  m_transFormData;                    ///< Transformation information structure
    CNtlPLCharacter*    m_pPLCharacterUI;                   ///< Characters expressed in the UI (only avatars used)
	CNtlPLCharacter*    m_pPLCharacterUI_Dogi;              ///< Characters expressed in Dogi UI (only avatars used)
    RwBool              m_bIsTransform;                     ///< Flag whether it has been transformed or not    
    RwUInt32            m_uiTransformId;                    ///< ID of what it was transformed into
    RwBool              m_bReadyTransform;                  ///< Used transformation skill
    RwBool              m_bScheduleLoadingCheck;            ///< Schedule loading check flag
    BYTE                m_byTransformLevel;                 ///< Transformation level    
    std::vector<CNtlInstanceEffect*> m_vecEffect;           ///< Effects upon transformation    

    // Super Saiyan
    CNtlPLItem*         m_pItemTransform;                   ///< Hair during Super Saiyan transformation
	CNtlPLItem*         m_pItemTransformUI;                 ///< Items for UI
    CNtlPLItem*         m_pItemTransformUI_Dogi;            ///< Items for Dogi UI
    RwTexture*          m_pFaceTransform;                   ///< Super Saiyan transformation face texture
    CNtlSobProxySystemEffect* m_pSystemEffectColor[2];      ///< Effect Color Controller during Super Saiyan transformation

    // Kaioken
    CNtlSobProxySystemEffect* m_pKaiOuKenColor[2];          ///< Color controller when transforming into Kaiwangkwon

    // Giant Namek
    SEquipItem*         m_pItemOrgWeapon;                   ///< Weapon originally worn when transformed into Namek 
    SEquipItem*         m_pItemOrgSubWeapon;                ///< The sub-weapon originally worn when transformed into Namek
    RwReal              m_fOrgAttackRange;                  ///< Original attack range before transformation

    // pure majin    
    CNtlPLCharacter*    m_pMajinOrgModel;                   ///< Original Mine model
    CNtlPLCharacter*    m_pMajinOrgModelUI;                 ///< Original Mine model for UI
	CNtlPLCharacter*    m_pMajinOrgModelUI_Dogi;            ///< Original mine model for Dogi UI
    CNtlPLCharacter*    m_pMajinPureModel;                  ///< Transformed pure mine model    
    CNtlPLCharacter*    m_pMajinPureModelUI;                ///< Pure mine model for UI    
	CNtlPLCharacter*    m_pMajinPureModelUI_Dogi;           ///< Pure mine model for Dogi UI    

    // candy transformation
    CNtlPLCharacter*    m_pCandyOrgModel;                   ///< Original model before being transformed into Candy
    CNtlPLCharacter*    m_pCandyOrgModelUI;                 ///< Original model for UI
	CNtlPLCharacter*    m_pCandyOrgModelUI_Dogi;            ///< Original model for Dogi UI
    CNtlPLCharacter*    m_pCandyModel;                      ///< Candy transformation model
	CNtlPLCharacter*    m_pCandyModelUI;                    ///< Candy model for UI
	CNtlPLCharacter*    m_pCandyModelUI_Dogi;               ///< Candy model for Dogi UI
};
