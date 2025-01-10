/******************************************************************************
*File			: OptionGraphicTab.h
*Author		: Hae sung, Cho
*Copyright		: NTL Co., Ltd.
*Date			: 2009. 3. 24
*Abstract		: 
*****************************************************************************
*Desc			: Tab of graphics
*****************************************************************************/

#ifndef _OPTION_GRAPHIC_TAB_H_
#define _OPTION_GRAPHIC_TAB_H_

#pragma once

////Video Mode List
typedef struct _SVideoModeInfo
{
	RwInt32 iWidth;
	RwInt32 iHeight;
	RwInt32 iColorDepth;
}SVideoModeInfo;

typedef std::list<SVideoModeInfo> ListVideoMode;

/**
* \ingroup client
*/
class COptionGraphic : public COptionBase
{
public:
	enum eQuality
	{
		eQUALITY_LOW = 0,
		eQUALITY_MIDDLE,
		eQUALITY_HIGH,
		eQUALITY_USER,

		eQUALITY_NUMS,
		eQUALITY_INVALID = 0XFF
	};

public:
	COptionGraphic();
	virtual ~COptionGraphic();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();

	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	void				OnScrollBarChanged(RwInt32 nParam);
	void				OnAbsoluteMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	void				OnListToggled(BOOL bToggled, gui::CComponent* pComponent);

	// Resolution & Gamma
public:
	void				SetComponentResolution(RwBool bDefault = FALSE);

	RwInt32				GetVideoMode(RwBool bDefault = FALSE);
	void				SetVideoMode(RwInt32 iIndex);
	std::wstring		GetVideoModeText();
	void				SetVideoModeText(const std::wstring& strText);

	RwInt32				GetGammaValue();
	void				SetGammaValue(RwInt32 iValue);
	void				OnGammaChanged(RwInt32 iParam);

	void				SetFpsValue(RwInt32 iValue);
	void				OnFpsSliderMoved(RwInt32 iValue);
	void				SetFpsToolTip(RwInt32 iValue);

	void				SelectVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iColorDepth); 
	void				SelectVideoModeWin(RwInt32 iWidth, RwInt32 iHeight);
	void				GetSelectVideoMode(SVideoModeInfo& sVideoModeInfo);
	ListVideoMode&		GetVideoModeList(RwBool bFullMode);

	// Preset
public:
	void				SetPreset( RwInt32 nLevel );
	
	void				OnOtherDataChanged( RwInt32 nParam );
	void				OnOtherDataToggled( gui::CComponent* pComponent, bool bToggle );

	// Handler
public:
	int					RestoreHandler();

protected:
	int					m_nCurrentQuality;	
	DEVICE_RESTORE_HANDLE m_hRestore;

protected:
	gui::CDialog*		m_pThis;			///< Current Dialog
	gui::CDialog*		m_pScrollDialog;	///< Scrolling Dialog

	gui::CSlot			m_slotAbsoluteMouseWheel;

	gui::CStaticBox*	m_pStbResolutionTitle;	///< Resolution settings
	gui::CStaticBox*	m_pStbResolution;		///< Resolution
	gui::CComboBox*		m_pCbbResolution;		///< Resolution setting combo box
	gui::CSlot			m_slotListToggled;	

	gui::CStaticBox*	m_pStbGamma;			///< Brightness
	gui::CScrollBar*	m_pScbGamma;			///< Brightness settings scrollbar
	gui::CSlot			m_slotGammaSliderMoved;
	gui::CSlot			m_slotGammaValueChanged;

	gui::CStaticBox*	m_pStbFps;
	gui::CScrollBar*	m_pScbFps;
	gui::CSlot			m_slotFpsSliderMoved;
	gui::CSlot			m_slotFpsValueChanged;

	gui::CButton*		m_pBtnWindowMode;		///< Window mode settings toggle bar

	gui::CStaticBox*	m_pStbEffectTitle;		///< Effect title
	gui::CStaticBox*	m_pStbGraphicQuality;	///< Overall graphics quality
	gui::CStaticBox*	m_pStbGraphicQualityLow;	///< low
	gui::CStaticBox*	m_pStbGraphicQualityMiddle;	///< Normal
	gui::CStaticBox*	m_pStbGraphicQualityHigh;	///< high
	gui::CStaticBox*	m_pStbGraphicQualityUser;	///< User settings
	gui::CScrollBar*	m_pScbGraphicQuality;		///< Graphic quality settings scrollbar
	gui::CSlot			m_slotQualitySliderMoved;
	gui::CSlot			m_slotQualityValueChanged;

	gui::CStaticBox*	m_pStbTerrainDist;		///< Terrain viewing distance
	gui::CScrollBar*	m_pScbTerrainDist;		///< Terrain viewing distance setting scroll bar
	gui::CSlot			m_slotTerrainSliderMoved;
	gui::CSlot			m_slotTerrainValueChanged;
	
	gui::CStaticBox*	m_pStbObjectDist;		///< Object viewing distance
	gui::CScrollBar*	m_pScbObjectDist;		///< Object viewing distance setting scroll bar
	gui::CSlot			m_slotObjectSliderMoved;
	gui::CSlot			m_slotObjectValueChanged;

	gui::CButton*		m_pBtnTerrainShadow;	///< Terrain Shadows
	gui::CSlot			m_slotTerrainShadowToggled;

	gui::CButton*		m_pBtnWaterEffect;		///< water effect
	gui::CSlot			m_slotWaterEffectToggled;

	/*gui::CStaticBox*  m_pStbCharacterDist;	///< Character viewing distance
	gui::CScrollBar*    m_pScbCharacterDist;	///< Character viewing distance setting scroll bar
	gui::CSlot          m_slotCharacterDistSliderMoved;
	gui::CSlot          m_slotCharacterDistValueChanged;*/

	gui::CButton*		m_pBtnCharacterShadow;	///< Character shadow
	gui::CSlot			m_slotCharacterShadowToggled;

	gui::CStaticBox*	m_pStbMagicEffect;		///< Magic detailed effects
	gui::CScrollBar*	m_pScbMagicEffect;		///< Magic detailed effect settings scroll bar
	gui::CSlot			m_slotMagicEffectSliderMoved;
	gui::CSlot			m_slotMagicEffectValueChanged;

	gui::CStaticBox*	m_pStbWeatherEffect;	///< Weather effects
	gui::CScrollBar*	m_pScbWeatherEffect;	///< Weather effect settings scroll bar
	gui::CSlot			m_slotWeatherEffectSliderMoved;
	gui::CSlot			m_slotWeatherEffectValueChanged;

	gui::CStaticBox*	m_pStbTextureQuality;	///< Texture Resolution
	gui::CScrollBar*	m_pScbTextureQuality;	///< Texture resolution setting scroll bar
	gui::CSlot			m_slotTextureQualitySliderMoved;
	gui::CSlot			m_slotTextureQualityValueChanged;

	gui::CStaticBox*	m_pStbCharacterEffectTitle;	///< Character effect title
	gui::CButton*		m_pBtnCartoonEdge;			///< Cartoon outline
	/*gui::CButton*     m_pBtnUpgradeEffect;		///< Upgrade effect*/
	gui::CSlot			m_slotCartoonEdgeToggled;
	/*gui::CSlot        m_slotUpgradeEffectToggled;*/

	gui::CStaticBox*	m_pStbEtcTitle;				
	gui::CButton*		m_pBtnHdrEffect;			///< HDR effect
	gui::CSlot			m_slotHdrEffectToggled;
	
	gui::CScrollBar*	m_pThisScrollBar;			///< scroll bar
	gui::CSlot			m_slotScrollBarChanged;
	gui::CSlot			m_slotScrollBarSliderMoved;

	RwInt32				m_iGamma;
	ListVideoMode		m_listFullVideo;
	ListVideoMode		m_listWinVideo;
	SVideoModeInfo		m_sSelectMode;
};


#endif