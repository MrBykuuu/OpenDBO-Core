/*****************************************************************************
*
*File			: PetSkillWindowGui.h
*Author		    : agebreak
*Copyright	    : NTL Co., Ltd.
*Date			: 2007. 8. 16	
*Abstract		: DBO Pet Skill Window Gui
*****************************************************************************
*Desc         :Class that displays Summon Pet's Skill Window GUI
*             : The transformation cancellation UI is also displayed here.
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

class CNtlSobIcon;

#define SUMMON_PET_MAX_SKILL 3

class CPetSkillWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    /// Enumeration type to distinguish the type of summoned pet
    enum EPetType               
    {
        E_PET_TYPE_SUMMON   = 0x00000001,
        E_PET_TYPE_ITEM     = 0x00000002,
        E_TRANSFORM_CANCEL  = 0x00000004,
    };

public:
	CPetSkillWindowGui();
	CPetSkillWindowGui(const RwChar* pName);
	~CPetSkillWindowGui();

	virtual void	Init();
	virtual RwBool	Create();
	virtual void	Destroy();
	virtual void	Update(RwReal fElapsed);	
	virtual RwInt32	SwitchDialog(bool bOpen);	
    virtual void    HandleEvents(RWS::CMsg &pMsg);

    static  CPetSkillWindowGui* GetInstance();
    static  void                DeleteInstance();

    void    AdjustDialog();                                     ///< Determine the location of the UI.
    void    SetPetID(RwUInt32 uiSerialID, EPetType ePetType);   ///< Set the Serial ID of the summoned Pet.
    void    SetTransformCancel(RwBool bEnable);                 ///< Displays the transformation cancellation icon.
    void    UpdatePetInfo();                                    ///< Update and display pet information.

	RwUInt32 GetInfoFlag();

protected:
    VOID	OnPaint(VOID);
    VOID	OnClickSummonCloseBtn(gui::CComponent* pComponent);
    VOID    OnClickItemCloseBtn(gui::CComponent* pComponent);
    VOID    OnClickSummonAttack(const CKey& key);               ///< When you click the Summon Pet's attack button
    VOID    OnMouseDownSummonAttack(const CKey& key);           ///< When pressing the Summon Pet's attack button with the right button -the image of the button changes.
    VOID    OnClickTransformCancel(gui::CComponent* pComponent);
    VOID    OnMouseEnter(gui::CComponent* pComponent);
	VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
    VOID    OnMouseLeave(gui::CComponent* pComponent);
    VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
    VOID	OnMouseDialogLeave(gui::CComponent* pComponent);

    void    SetSkillIcon();                                     ///< Set the texture of the skill icon.
    void    UpdateSkillIcon(RwReal fElapsedTime);               ///< Updates the status (cool time) effect of the skill icon.

protected:
    static CPetSkillWindowGui*  m_pInstance;

    gui::CPanel*    m_pPnlSummonPetBack;
    gui::CPanel*    m_pPnlItemPetBack;
    gui::CPanel*    m_pPnlTransformCancelBack;
    gui::CButton*   m_pBtnSummonClose;
    gui::CButton*   m_pBtnItemClose;
    gui::CButton*   m_pBtnSummonAttack;
    gui::CButton*   m_pBtnTransformCancel;

    gui::CSlot      m_slotPaint;
    gui::CSlot      m_slotClickCloseBtn1;
    gui::CSlot      m_slotClickCloseBtn2;
    gui::CSlot      m_slotClickSummonAttack;
    gui::CSlot      m_slotMouseDownSummonAttack;
    gui::CSlot      m_slotClickTransformCancel;
    gui::CSlot      m_slotMouseEnter;
    gui::CSlot      m_slotMouseLeave;
	gui::CSlot		m_slotMove;
    gui::CSlot      m_slotMouseMove;
    gui::CSlot      m_slotMouseDialogLeave;

    CSurfaceGui     m_surFocus;                               ///< Display surface when the Attack button has focus
    RwBool          m_bVisibeFocus;

    CSurfaceGui     m_surSummonSkill[SUMMON_PET_MAX_SKILL];   ///< Summon Pet's skill icon surface
    gui::CRadarEffect m_CoolTimeEffect[SUMMON_PET_MAX_SKILL]; ///< Cooltime surface for Summon Pet skill
    CNtlSobIcon*    m_pSobSkillIcon[SUMMON_PET_MAX_SKILL];    ///< Summon Pet's skill icon object
 
    RwUInt32        m_uiSummonPetID;                          ///< Serial ID of Summon Pet
    RwUInt32        m_uiItemPetID;                            ///< Serial ID of Item Pet
    RwUInt32        m_uiInfoFlag;                             ///< Flag for what information to display

    gui::CSurface   m_surAttackBtnUp;                         ///< Original size surface of Attack button
    gui::CSurface   m_surAttackBtnDown;                       ///< Downsized surface for Attack button
    gui::CSurface   m_surTransformNamek;                      ///< Namek transformation cancel icon
    gui::CSurface   m_surTransformSaiya;                      ///< Saiyan transformation cancel icon
    gui::CSurface   m_surTransformMajin;                      ///< Mine transformation cancel icon

};