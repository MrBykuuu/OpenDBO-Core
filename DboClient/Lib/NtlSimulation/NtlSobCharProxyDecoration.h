/*****************************************************************************
*
* File			: NtlSobCharDecorationProxy.h
* Author		: agebreak
* Copyright	: (?)NTL
* Date			: 2005. 11. 30	
* Abstract		: Simulation object attribute base class
*****************************************************************************
* Desc         : Class related to decoration of characters, such as effects and shadows.
*
*****************************************************************************/

#pragma once

class CNtlPLCharacter;
class CNtlPLPlayerName;
class CNtlPLDecal;
class CNtlPLItem;
class CNtlPLCameraRenderTexture;
class CNtlSobItem;
class CDBODirectorIndicate;
class CNtlSobCharDecorationProxy;
class CNtlInstanceEffect;
class CNtlSobCharProxyTransform;
class CNtlShareTargetMark;

struct sEmblemFactor;

/// Classes related to decoration of characters, such as effects and shadows.
class CNtlSobCharDecorationProxy
{
public:

	static RwBool				m_bShadowCreate;			/** Shadow creation on/off flag Controls the creation of a specific character shadow*/
    static RwBool				m_bShadowRender;			/** shadow rendering on/off flag */

private:

    CNtlSob						*m_pSobObj;
    CNtlPLCharacter				*m_pPLCharacter;			/** character resource */

    /*Rw bool m b name visible;*/
	RwReal						m_fNameHeightOffset;		/** name height offset */
	RwV3d						m_vNamePos;					/** name position */
    RwReal						m_fDefShadowScale;			/** shadow scale */
    RwReal						m_fTransShadowScale;		/** shadow scale */
    RwBool						m_bResLoaded;
    RwBool						m_bResLoadComplete;
    RwInt32                     m_nPrevSectorIdx;           // Previous world sector index (used for shadow discrimination)    

    CNtlPLDecal					*m_pShadowDecal;			/** shadow decal */
    CNtlPLPlayerName			*m_pPlayerName;				/** presentation player name entity */
    CNtlPLEntity				*m_pTargetMark;				/** character target mark */
    CNtlPLEntity                *m_pTargetMark02;           // decal target mark
    CNtlPLEntity				*m_pAttackMark;				/** character attack mark */
    CNtlPLEntity                *m_pAttackMark02;           // decal attack mark
    CNtlPLEntity				*m_pQuestMark;				/**	character quest mark */        
    CNtlPLEntity                *m_pTenkaichiMark;          ///< World¡¯s Best Martial Arts Club Title Marking Effect
    CNtlPLEntity                *m_pTargetMarkingMark;      /// Target marking mark (Mega Dodongpa)
    CDBODirectorIndicate        *m_pDBODirectorIndicate;    /** TMQ/Quest Director Indicate */    
	sEmblemFactor				*m_pEmblemFactor;			/** Emblem component */
    CNtlShareTargetMark         *m_pShareTargetMark;        ///< Shared target mark management object

    std::vector<CNtlInstanceEffect*> m_vecRPBonusEffect;    ///< Vector to store the created effect for RPBonus

    CNtlSobCharProxyTransform   *m_pProxyTransform;         ///< Proxy object related to transformation    

	CNtlPLEntity*				m_pTitleEffect;
	CNtlPLEntity*				m_pGuardEffect;
	CNtlPLEntity*				m_pRpChargeEffect;

protected:

    /** 
    *Create shadow decal
    */
    void CreatePLShadowDecal(void);
    void DeletePLShadowDecal(void);
    void ChangePLShadowDecal();             // Change the shadow color.

	/** 
	*Player Name
	*/
    void CreatePLPlayerName(void);
    void DeletePLPlayerName(void);

	/**
	*player title effect
	*/
	void CreatePLPlayerTitle(const char *pEffectKey, const char *pBoneKey);
	void DeletePLPlayerTitle();

    /** 
    *character attack mark
    */ 
    void CreatePLAttackMark(void);
    void DeletePLAttackMark(void);

    
    // character target mark
    void CreatePLTargetMark(void);
    void DeletePLTargetMark(void);
    RwBool IsNotCreateDecalMark();                                          ///< Check whether the target mark is an object that does not display a floor decal.

    /** 
    *character quest mark
    */ 
    void CreatePLQuestMark(const RwChar *pKey);
    void DeletePLQuestMark(void);

    /// Share target mark
    void CreateShareTargetMark(RwUInt8 bySlot, RwUInt32 type);
    void DeleteShareTargetMark();

    // World's Best Martial Arts Club Title Event
    void CreateTenkaichiMark(BYTE byType);
    void DeleteTenkaichiMark();

    // Target Marking Event (Mega Dodongpa)
    void CreateTargetMarkingMark();
    void DeleteTargetMarkingMark();
    
public:

    CNtlSobCharDecorationProxy();
    ~CNtlSobCharDecorationProxy();

    void	SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter);
    void	Update(RwReal fElapsed);
    void	HandleEvents(RWS::CMsg &pMsg);

    //-------------------------------------------------------------------------------
    // event handler
    //-------------------------------------------------------------------------------
public:

    void	SobFaintingEventHandler(RWS::CMsg &pMsg);
    void    SobStateTanslation(RWS::CMsg& pMsg);
    void	SobTargetSelectEventHandler(RWS::CMsg &pMsg);
    void	SobAttackSelectEventHandler(RWS::CMsg &pMsg);
    void	SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg);
    void	SobAttackMarkReleaseEventHandler(RWS::CMsg &pMsg);
    void	SobQuestMark(RWS::CMsg &pMsg);
    void    SobDirectionNfy(RWS::CMsg &pMsg);    
    void    SobShareTargetSelectHandler(RWS::CMsg& pMsg);                   ///< Share target mark setting event processing
    void    SobShareTargetReleaseHandler(RWS::CMsg& pMsg);                  ///< Processing of shared target mark release event        
    void    SobTenkaichiMarkingHandler(RWS::CMsg& pMsg);                    ///< World¡¯s Best Martial Arts Club title event processing    

	void    SobTitleEffectHandler(RWS::CMsg& pMsg);

    void AddWorld(void);
    void RemoveWorld(void);	

    void SetAlpha(RwUInt8 byAlpha);
    void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
    void SetVisible(RwBool bEnable);
    void SetScale(RwReal fScale);
    void SetShadowOnOff(RwBool bOnOff);
    void EnablePlayerNameVisible(RwBool bVisible);

    void CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue);
    void DeleteWeightElapsedController(void);

    void DetachConvertClassEquipItem(void);
    void AttachConvertClassEquipItem(CNtlPLCharacter *pPLCharacter);

	void SetNameColor(const WCHAR* pwcName, COLORREF nameColor,
					  const WCHAR* pwcGuildName, COLORREF guildColor, 
					  const WCHAR* pwcTitleName, COLORREF titlecolor);

	void SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor);
    CNtlPLPlayerName* GetPLPlayerName();

	void UpdatePlayerNamePosition( RwV3d& vPos, float fNewHeight = 0.0f );

	const sEmblemFactor* GetEmblemFactor();
	void SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
						 RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
						 RwUInt8 byTypeC, RwUInt8 byTypeCColor);

    void ResourceLoadComplete(RwBool bVisible);

    // Director Indicate
    void CreateDirectorIndicate();
    void DeleteDirectorIndicate();

    // Rp Bonus Skill
    RwBool AttachRPBonusEffect();             ///< Creates an RP Bonus Effect.
    RwBool DetachRPBonusEffect();             ///< Removes the RP Bonus Effect.

	 // Guard Skill
	void CreateGuardEffect(RwChar *pKey);
	void DeleteGuardEffect();

	// rp charging Skill
	void CreateRpChargeEffect(RwChar *pKey);
	void DeleteRpChargeEffect();

    // Transform
    void						SobChangeAdult(CNtlPLCharacter* pPLCharacter);                    ///< When the UI Model is changed
    CNtlSobCharProxyTransform*  GetProxyTransform() {return m_pProxyTransform;}    
};
