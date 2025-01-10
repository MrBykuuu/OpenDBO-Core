/******************************************************************************
*File           : ScouterGui.h
*Author		    : Hong SungBock
*Copyright	    : NTL Co., Ltd.
*Date           : November 17, 2008
*Abstract		: 
*****************************************************************************
*Desc           : GM game shortcut menu interface
*****************************************************************************/

#pragma once

class CGMPopupGui;
class CPlayerInfoGui;

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"


class CGMQuickSlotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	enum eMenuGroup
	{
		MENU_GROUP_MAIN,
		MENU_GROUP_TENKAICHI,
	};

	typedef std::map<SERIAL_HANDLE, CPlayerInfoGui*>					MAP_POPUP;


	CGMQuickSlotGui(const RwChar* pName);
	virtual ~CGMQuickSlotGui();

	RwBool		Create();
	VOID		Destroy();

	CGMPopupGui*	GetPopupGui() { return m_pPopupGui; }
	RwInt32		SwitchDialog(bool bOpen);

protected:
	CGMQuickSlotGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetMenuGroup(eMenuGroup eGroup);
	VOID			ShowMainButtons(bool bShow);
	VOID			ShowTenkaichiButtons(bool bShow);

	VOID			AddPlayerInfo(SERIAL_HANDLE hPlayer);
	VOID			RemovePlayerInfo(SERIAL_HANDLE hPlayer);

	VOID			ShowPopup(RwBool bShow, RwUInt8 bySlot);

	VOID			OnClicked_CharShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi( gui::CComponent* pComponent );
	VOID			OnClicked_Portal( gui::CComponent* pComponent );
	VOID			OnClicked_FreeCamera( gui::CComponent* pComponent );

	VOID			OnClicked_Tenkaichi_CameraLoc( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_CameraMove( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_MenuShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Player( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Tournament( gui::CComponent* pComponent );
	VOID			OnClicked_Return( gui::CComponent* pComponent );

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	RwInt32			ActionMapQuickSlotDown( RwUInt32 uiKey );
	RwInt32			ActionMapQuickSlotUp( RwUInt32 uiKey );

protected:
	gui::CSlot			m_slotCharShowButton;
	gui::CSlot			m_slotCharHideButton;
	gui::CSlot			m_slotTenkaichiButton;
	gui::CSlot			m_slotPortalButton;
	gui::CSlot			m_slotFreeCameraButton;
	gui::CSlot			m_slotNormalCameraButton;

	gui::CSlot			m_slotTenkaichi_CameraLoc;
	gui::CSlot			m_slotTenkaichi_CameraMove;
	gui::CSlot			m_slotTenkaichi_MenuShow;
	gui::CSlot			m_slotTenkaichi_MenuHide;
	gui::CSlot			m_slotTenkaichi_Player;
	gui::CSlot			m_slotTenkaichi_Tournament;
	gui::CSlot			m_slotReturn;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pCharShowButton;				///< Show GM character button
	gui::CButton*		m_pCharHideButton;				///< GM character hide button
	gui::CButton*		m_pTenkaichiButton;				///< World's Best Martial Arts Menu Top Button
	gui::CButton*		m_pPortalButton;				///< Avatar movement button
	gui::CButton*		m_pFreeCameraButton;			///< Free camera button
	gui::CButton*		m_pNormalCameraButton;			///< Third-person camera button

	gui::CButton*		m_pTenkaichi_CameraLocButton;	///< World's Best Ballroom Camera Position Change Button
	gui::CButton*		m_pTenkaichi_CameraMoveButton;	///< World's Best Ballroom Camera Movement Button
	gui::CButton*		m_pTenkaichi_MenuShowButton;	///< The World's Best Martial Arts Club's designated menu Show button
	gui::CButton*		m_pTenkaichi_MenuHideButton;	///< World's Best Martial Arts Menu Hide Button
	gui::CButton*		m_pTenkaichi_PlayerButton;		///< World's Best Martial Arts Player Information Button
	gui::CButton*		m_pTenkaichi_TournamentButton;	///< View World's Best Martial Arts Tournament bracket button
	gui::CButton*		m_pReturnButton;				///< Return button

	CGMPopupGui*		m_pPopupGui;					///< GM Popup Menu

	eMenuGroup			m_eMenuGroup;
	RwBool				m_bAvatarShow;
	RwInt8				m_byPopupSlot;					///< Index of the slot where the pop-up menu appears

	MAP_POPUP			m_mapPlayerInfo;
};