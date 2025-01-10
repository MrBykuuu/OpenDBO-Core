////////////////////////////////////////////////////////////////////////////////
// File: MiniMapGui.h
// Desc: MiniMap
//
// 2006.6.21 Peessi@hitel.net   
//
// To Be Desired:
//	1. Only the current character and mob are displayed. A mask function must be added to display map images.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MINIMAP_GUI_H__
#define __MINIMAP_GUI_H__

#include <map>

// Shared
#include "NtlDojo.h"

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo Trigger
#include "DboTSCoreDefine.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlParty.h"

// Dbo
#include "SurfaceGui.h"
#include "SurfaceCamera.h"
#include "DboTableInfo.h"

struct sNPC_TBLDAT;
struct sMOB_TBLDAT;
struct sTBLDAT;

class CMapNameTextTable;

class CMapItem
{
public:
	enum eMapPosition
	{
		///< Top left        Top              Top right
		MP_LT,				MP_CT,				MP_RT,

		///< Left           Middle              Right
		MP_LC,				MP_CC,				MP_RC,

		///< Bottom left    Bottom           Bottom right
		MP_LB,				MP_CB,				MP_RB,

		NUM_MAPPOSITON
	};

	struct RealRect
	{
		RwReal		left, right, top, bottom;
	};

	CMapItem();
	CMapItem(const CMapItem& map) {}
	virtual ~CMapItem();

	static VOID		CreateCamera();
	static VOID		DestroyCamera();

	VOID			Create();	
	VOID			Destroy();

	RwBool			ReadyMakeTexture(RwReal fElapsed);
	VOID			EndMakeTexture();
	VOID			Render();

	VOID			SetPosition(RwInt32 iXPos, RwInt32 iYPos);

	VOID			SetScale(RwReal fScale);
	RwReal			GetScale();

	RwReal			GetRate();								///< Minimap to texture ratio according to scale
	RealRect		GetAreaRect();

protected:
	VOID			UpdateAreaRect(RwV3d vFieldPos);	///< Calculate 4 field areas centered on the avatar
	VOID			UpdatePotalMapUpdateResource();		///< Loading required resources	

	VOID			LoadingSurface(CSurfaceGui& surface, RwInt32 iFieldIndex, RwInt32 iMapPosition);///< Loads the minimap to be drawn in the actual game

	RwInt32			CalcIndex(RwInt32 iCenterIndex, RwInt32 iPosition); ///< Returns the valid index of the specified position
	RwBool			IsWorldIndex(RwInt32 iIndex);		///< Check if it is an index in the world
	

protected:
	static CSurfaceCamera*		m_pRealMapCamera;		///< Camera for creating actual grill texture
	static CSurfaceCamera*		m_pRealMapCamera2;		///< Camera for creating actual grill texture
	
	CCameraRenderTargetSurface	m_ResourceSurface[NUM_MAPPOSITON];	///< Surface to store resources
	
	CFilteredSurfaceGui			m_SurfaceMap2;			///< Map to be drawn on the actual screen
	CCameraRenderTargetSurface	m_SurfaceMap;			///< Map to be drawn on the actual screen

	RealRect					m_rtAreaRect;			///< Field area loaded centered on the avatar

	RwInt32						m_iPostField;			///< Previously located field index		
	RwInt32						m_iCurField;			///< Current field index
	WORLDID						m_PostWorldID;			///< Previous World ID

	RwReal						m_fScale;				///< Minimap scale
};



class CMinimapGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
#define dMINIMAPSCALE_COUNT					(3)			///< Number of minimap scales

	enum eMapStyle
	{
		MS_NORMAL,
		MS_MINIMAM,
		MS_HIDE,

		MS_NONE
	};

	enum eTriggerObjectMarkType
	{
		TOMT_POPOSTONE,									///< Four PoPostone

		NUM_TOMT
	};

	enum eMonsterMarkType
	{
		MMT_STRONG,
		MMT_NORMAL,
		MMT_WEAK,		
		MMT_HAVE_DRAGONBALL,
		MMT_HAVE_QUEST,

		NUM_MMT
	};

	enum eQuestMarkType
	{
		QMT_NEW,
		QMT_PROGRESS,
		QMT_REWARD,

		NUM_QMT
	};

	enum eNextQuestMarkType
	{
		NQMT_INSIGHT,									///< Within minimap viewing distance (NPC, OBJECT)
		NQMT_INSIGHT_POSITION,							///< Within the minimap viewing distance (POSITION)
		NQMT_CHECK,	// to do
		NQMT_OUTSIGHT,									///<Out of minimap viewing range

		NUM_NQMT
	};

	enum eMiniMapPosition
	{
		MMP_INSIGHT,									///< Within minimap viewing distance
		MMP_OUTSIGHT,									///<Out of minimap viewing range

		NUM_MMP
	};

	enum eCampPeopleType
	{
		CAMP_PEOPLE_MY_PARTY,
		CAMP_PEOPLE_MY_TEAM,
		CAMP_PEOPLE_EMENY_TEAM,

		NUM_CAMP_PEOPLE
	};

	struct sTRIGGER_OBJECT
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		eTriggerObjectMarkType	eType;
		RwBool				bShow;
		RwBool				bInfoWindow;
		std::wstring		wstrText;
	};

	struct sHIDDEN_OBJECT
	{
		SERIAL_HANDLE		hHandle;
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwBool				bShow;
		RwBool				bInfoWindow;
		std::wstring		wstrText;
	};

	struct sMONSTER
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwBool				bShow;
		RwBool				bInfoWindow;
		eMonsterMarkType	eMobMarkType;
		sMOB_TBLDAT*		pMOB_TBLDAT;
	};

	struct sMINIMAP_NPC
	{
		sMINIMAP_NPC() : ucJob(0xFF), bGetOnBus(false)
		{}

		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwUInt8				ucJob;
		RwBool				bGetOnBus;
		RwBool				bShow;
		RwBool				bInfoWindow;
		sNPC_TBLDAT*		pNPC_TBLDAT;
		SERIAL_HANDLE		hHandle;
	};
	
	struct sMINIMAP_QUEST
	{
		sMINIMAP_QUEST() : ucQuest(0)
		{}

		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwUInt8				ucQuest;
		RwBool				bShow;
		RwBool				bInfoWindow;
		sNPC_TBLDAT*		pNPC_TBLDAT;
	};

	struct sNEXT_QUEST
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwV2d				v2OutCenter;		///< Valid when eNextQuestMarkType is NQMT_OUTSIGHT
		eNextQuestMarkType	eType;
		RwBool				bShow;
		RwBool				bInfoWindow;
		RwReal				fAngle;
		WORLDID				WorldID;
		eQMI_TARGET_TYPE	eTargetType;
		const WCHAR*		pwcText;
		sTBLDAT*			pTable;
	};

	struct sCHECK_POINT
	{
		// Currently there is only one at a time, but it is managed as a std::vector for future use.
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwV2d				v2OutCenter;		///< Valid when eMiniMapPosition is MMP_OUTSIGHT
		eMiniMapPosition	eType;
		RwBool				bShow;
		RwReal				fAngle;
		std::wstring		wstrText;
	};	

	struct sPARTYMEMBER
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwV2d				v2OutCenter;		///< Valid when eMiniMapPosition is MMP_OUTSIGHT
		eMiniMapPosition	eType;
		WCHAR*				pwcName;
		RwBool				bShow;
		RwBool				bInfoWindow;
		RwReal				fAngle;
	};

	struct sLANKMARK
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		eLandMark_SpecialType eLandMarkType;
		WCHAR*				pwcName;
		RwBool				bShow;
	};

	struct sCOMMON_TARGET
	{
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwBool				bShow;
		RwBool				bInfoWindow;
		std::wstring		wstrName;
	};

	struct sSCRAMBLE_SEAL
	{
		eDBO_TEAM			eTeamSeal;
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwBool				bShow;
		RwBool				bInfoWindow;
		std::wstring		wstrName;
	};

	struct sSCRAMBLE_SEAL_NOTIFY
	{
		RwBool				bEnable;
		RwBool				bOffence;
		RwV2d				v2Pos;
		gui::CFlash*		pFlash;
		RwReal				fRemainTime;
		RwReal				fRestTime;
	};

	struct sCAMP_PEOPLE
	{
		eCampPeopleType		ePeopleType;
		RwV2d				v2Pos;
		RwV2d				v2Icon;
		RwBool				bShow;
		const WCHAR*		pwcName;
	};

	struct sSCRAMBLE_VISIBLE
	{
		RwBool				bScramble;
		RwBool				bShowScrambleMyTeam;
		RwBool				bShowScrambleEnemyTeam;
	};

	typedef std::list<sCAMP_PEOPLE>						LIST_CAMP_PEOPLE;

//! Constructor & Destructor	
	CMinimapGui(const RwChar *pName);
	~CMinimapGui();

//! Operatrion
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );
	VOID	UpdateBeforeCamera(RwReal fElapsed);	///< update before camera

	VOID	UpdateRegionTitle();

	VOID	NextMapStyle();
	RwUInt8	GetMapStyle();
	VOID	ChangeMapStyle(RwUInt8 byStyle);

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
	RwInt32 SwitchDialog( bool bOpen );

private:
	CMinimapGui();

//! Implementation
	VOID	TransWorldToMapAxis( RwV2d& v2Avatar, RwV2d& v2d, RwReal fRate, RwV2d& V2PosResult );

//! CallBack
	gui::CSlot	m_slotMapShowButtonClicked;
	gui::CSlot	m_slotMapHideButtonClicked;
	gui::CSlot	m_slotWorldMapClicked;
	gui::CSlot	m_slotExpansionClicked;
	gui::CSlot	m_slotReductionClicked;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotMouseLeave;
	gui::CSlot	m_slotRegionTitleMouseMove;
	gui::CSlot	m_slotRegionTitleMouseLeave;
	gui::CSlot	m_slotCaptureWheelMove;

	VOID	ZooninMiniMap();
	VOID	ZoomoutMiniMap();
	VOID	AdjustIconSize();

	VOID	SetCommonTargetInfo(RwInt32 iCommonTargetIndex, RwV2d& v2Pos, RwV2d& v2Icon);
	VOID	ClearSeal();

	VOID	CheckInfoWindow();
	RwReal  GetAngle(RwV2d v2Dir, RwV2d* pv3OutSqrtDir = NULL);

	VOID	OnClicked_MapShowButton( gui::CComponent* pComponent );
	VOID	OnClicked_MapHideButton( gui::CComponent* pComponent );
	VOID	OnClicked_WorldMap( gui::CComponent* pComponent );
	VOID	OnClicked_Expansion( gui::CComponent* pComponent );
	VOID	OnClicked_Reduction(gui::CComponent* pComponent);

	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);	
	VOID	OnMouseLeave(gui::CComponent* pComponent);
	VOID	OnRegionTitleMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID	OnRegionTitleMouseLeave(gui::CComponent* pComponent);
	VOID	OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);

//! Variables
	gui::CButton*	m_pMapShowBtn;
	gui::CButton*	m_pMapHideBtn;
	gui::CButton*	m_pWorldMapBtn;
	gui::CButton*	m_pExpansionBtn;
	gui::CButton*	m_pReductionBtn;

	TBLIDX			m_idxAreaInfoIndex;
	gui::CStaticBox*	m_pRegionTitle;
	
	CSurfaceGui		m_surFrame;
	CSurfaceGui		m_surBack;	
	RwBool			m_bShowMap;

	CMapItem		m_MapItem;

	CSurfaceGui		m_surMarkAvatar;
	CSurfaceGui		m_surMarkMob[NUM_MMT];
	CSurfaceGui		m_surMarkNPC;
	CSurfaceGui		m_surMarkQuest[NUM_QMT];
	CSurfaceGui		m_surMarkNextQuest[NUM_NQMT];
	CSurfaceGui		m_surMarkCheckPoint[NUM_MMP];
	CSurfaceGui		m_surHiddenObject;
	CSurfaceGui		m_surPopoStone;
	CSurfaceGui		m_surPopoStoneActive;
	CSurfaceGui		m_surMarkPartyMember[NUM_MMP];
	CSurfaceGui		m_surMarkCommonTarget[NTL_MAX_SHARETARGET_COUNT];
	CSurfaceGui		m_surMarkDojoSeal[MAX_TOBJECT_STATE_TYPE_C];
	CSurfaceGui		m_surCamp[NUM_CAMP_PEOPLE];					///< (Fia division) People who divide into teams and fight

	RwReal			m_fElapsedTime;
	
	RwUInt8			m_byMapStyle;
	RwInt8			m_byCurScale;								///< Current map scale	
	RwReal			m_fMiniMapScaleLevels[dMINIMAPSCALE_COUNT];	///< Map scale level

	sSCRAMBLE_VISIBLE	m_tSCRAMBLE_VISIBLE;

	SERIAL_HANDLE	m_hBus_with_Avatar;							///< Steering wheel of the bus the avatar is riding on

	CMapNameTextTable*			m_pMapNameTextTable;			///< Map name code of where the avatar is currently located
	
	std::vector<sTRIGGER_OBJECT> m_vecTriggerObject;
	std::vector<sHIDDEN_OBJECT> m_vecHiddenObject;				///< It is a type of Trigger object, but managed separately.
	std::vector<sMONSTER>		m_vecMob;
	std::vector<sMINIMAP_NPC>	m_vecNPC;
	std::vector<sMINIMAP_QUEST>	m_vecQuest;
	std::vector<sNEXT_QUEST>	m_vectNextQuest;
	std::vector<sCHECK_POINT>	m_vecCheckPoint;
	std::vector<sPARTYMEMBER>	m_vecPartyMember;
	sCOMMON_TARGET				m_aCommonTarget[NTL_MAX_SHARETARGET_COUNT];	///< Match the array index and shared target number
	std::map<RwUInt32, sSCRAMBLE_SEAL*>			m_mapScrambleSeal;
	std::map<RwUInt32, sSCRAMBLE_SEAL_NOTIFY*>	m_mapScrambleSealNotify;
	LIST_CAMP_PEOPLE			m_listCampPeople;
};

#endif