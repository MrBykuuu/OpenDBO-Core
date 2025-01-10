/*****************************************************************************
*
*File			: PetStatusBarGui.h
*Author		    : agebreak
*Copyright	    : NTL Co., Ltd.
*Date			: 2007. 8. 16	
*Abstract		: DBO Pet HP/MP gui.
*****************************************************************************
*Desc          : A class that displays the Summon Pet's StatusBar GUI
*
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NumberGui.h"

//Sound
#include "GUISoundDefine.h"

class CBuffDispGui;

class CPetStatusBarGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPetStatusBarGui();
	CPetStatusBarGui(const RwChar* pName);
	~CPetStatusBarGui();

	virtual void	Init();
	virtual RwBool	Create();
	virtual void	Destroy();
	virtual void	Update(RwReal fElapsed);
	virtual void	UpdateBeforeCamera(RwReal fElapsed);
	virtual RwInt32	SwitchDialog(bool bOpen);
    virtual void    HandleEvents(RWS::CMsg &pMsg);

	static CPetStatusBarGui* GetInstance();
	static void				 DeleteInstance();

	void			SetPetID(RwUInt32 uiSerialId);			///< Set the ID of the summoned Pet.
	RwUInt32		GetPetID();

protected:
	VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID	OnPaint(VOID);
	VOID	OnPaintLast(VOID);
   /* VOID    OnPaintPower(VOID);
    VOID    OnMovePower( RwInt32 nOldX, RwInt32 nOldY );*/
    VOID	OnClickInfoBtn(gui::CComponent* pComponent);
    VOID	OnCaptureMouseDown(const CKey& key);
	VOID	OnCaptureMouseUp(const CKey& key);
    
	void	UpdatePetInfo();						///< Update the pet's information in the UI.
	//void	UpdatePetPowerValue();					///< Updates the Pet's combat power in the UI.
    void    ToggleInfo();                           ///< Toggle the detailed information window	

protected:
	static CPetStatusBarGui* m_pInstance;			///< singleton object
    
	gui::CProgressBar*	m_ppgbHp;
	gui::CProgressBar*	m_ppgbEp;		
	gui::CStaticBox*	m_sttHp;
	gui::CStaticBox*	m_sttMp;	
	gui::CButton*		m_pbtnPetInfo;	
	gui::CStaticBox*    m_psttNickName;

	// disable by daneos
    /*gui::CPanel*		m_ppnlPowerText;
    CNumberGui			m_numPower;*/

    gui::CPanel*        m_pPnlInfoBack;
    gui::CStaticBox*    m_pSttInfoLPLabel;
    gui::CStaticBox*    m_pSttInfoEPLabel;    
    gui::CStaticBox*    m_pSttInfoHRLabel;
    gui::CStaticBox*    m_pSttInfoDRLabel;
    gui::CStaticBox*    m_pSttInfoPOLabel;
    gui::CStaticBox*    m_pSttInfoPDLabel;    
    gui::CStaticBox*    m_pSttInfoEDLabel;
    gui::CStaticBox*    m_pSttInfoLP;
    gui::CStaticBox*    m_pSttInfoEP;    
    gui::CStaticBox*    m_pSttInfoHR;           // Attack success rate
    gui::CStaticBox*    m_pSttInfoDR;           // Evasion rate
    gui::CStaticBox*    m_pSttInfoPO;           // attack power
    gui::CStaticBox*    m_pSttInfoPD;           // Strike Defense    
    gui::CStaticBox*    m_pSttInfoED;           // Qigong Defense

	//CFilteredSurfaceGui m_surCharacter;
	gui::CTexture		m_texCharacter;
	CSurfaceGui			m_surCharacter;			///< Pet Illust -by Kell

	gui::CSlot			m_slotMove;
	gui::CSlot	        m_slotPaint;	
    gui::CSlot          m_slotPaintLast;
    gui::CSlot          m_slotInfoBtnClick;
    gui::CSlot          m_slotCaptureMouseDown;
	gui::CSlot			m_slotCaptureMouseUp;
    /*gui::CSlot          m_slotPowerPaint;
    gui::CSlot          m_slotPowerMove;*/

	// Emergency Effect
	gui::CFlickerEffect	m_feEmergency;

	// RP Gauge
	RwReal	m_fDestValue;
	RwReal	m_fCurrentValue;
	RwInt32	m_nMaxValue;

	// Buff
	CBuffDispGui*	m_pBuff;

	RwBool			m_bDelete;
	RwUInt32		m_uiPetID;
    bool            m_bShowInfo;                ///< Whether to display the information window
};