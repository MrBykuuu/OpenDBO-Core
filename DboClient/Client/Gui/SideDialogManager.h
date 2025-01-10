/******************************************************************************
*File			: SideDialogManager.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2007. 11. 6
*Abstract		: 
*****************************************************************************
*Desc			: Manages the dialog that appears in the center right of the client.
*                If the heights overlap and are displayed on the screen without open/close effects,
*                Close all existing overlapping SideDialogs
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "DialogPriority.h"
#include "SideDialogDefine.h"

class CSideDialogManager : public CNtlPLGui, public RWS::CEventHandler
{
public:
	static CSideDialogManager*	GetInstance();

	enum eOpenType
	{
		OPENTYPE_OPEN,							///< Current status displayed on screen
		OPENTYPE_MINIMAM,						///< Currently displayed on screen, but minimized
		OPENTYPE_CONCEAL,						///< State covered by another window
	};

	struct DialogInfo
	{
		CNtlPLGui*				pDialog;		///< Pointer to dialog
		CNtlCallbackParam1bool* pCallSwitch;	///< Callback to the function that opens and closes the dialog

		DialogInfo()
		{
			pDialog = NULL;
			pCallSwitch = NULL;
		}
	};

	struct sPresentationDialog
	{
		eSideDialogType			eSDialog;
		DialogInfo*				pDialogInfo;	
		eOpenType				openType;
	};

	typedef std::map<int, DialogInfo>							MAP_SDIALOG;
	typedef std::map<int, DialogInfo>::iterator					MAP_SDIALOG_ITER;

	typedef std::map<RwInt32, sPresentationDialog>				MAP_PRESENTATION;
	typedef std::map<RwInt32, sPresentationDialog>::iterator	MAP_PRESENTATION_ITER;

	typedef std::map<RwInt32, RwUInt32>							MAP_ATTRIBUTE;
	typedef std::map<RwInt32, RwUInt32>::iterator				MAP_ATTRIBUTE_ITER;


	CSideDialogManager(const RwChar* pName);
	virtual ~CSideDialogManager();

	static VOID CreateInstance();
	static VOID	DestoryInstance();

	template <class Callbackclass>
	VOID		RegistDialog(int iSideDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen));
	VOID		UnRegistDialog(int iSideDialog);

	VOID		Update(RwReal fElapsed);
	RwInt32		SwitchDialog(bool bOpen);
	RwBool		SwitchDialog(int iDialog);			///< Opens or closes the side dialog

	VOID		OpenDefaultDialog();
	RwBool		OpenDialog(RwInt32 iSideDialog, RwBool bPlaySound = TRUE);

	RwBool		CloseDialog(RwInt32 iSideDialog, RwBool bPlaySound = TRUE);
	VOID		CloseAll();

	DialogInfo*	GetDialogInfo(int iSideDialog);
	CNtlPLGui*	GetDialog(RwInt32 iSideDialog);

	RwBool		IsOpenDialog(int iSideDialog);
	RwBool		IsConcealDialg(int iSideDialog);
	RwBool		IsDefaultDialog(int iSideDialog);
	RwBool		IsEventDialog(int iSideDialog);
	RwBool		IsNormalDialog(int iSideDialog);
	//// Test Func		/// woosungs_test 20090723
	CNtlPLGui* const GetpDialogTEST(const char* szFrmFileName);
	RwBool		ShowDialogTEST(RwInt32 iDialog, bool bOpen = true);
	VOID		ShowAllDialogTEST(bool bOpen = true);
protected:
	CSideDialogManager() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool		IsAttribute(RwInt32 iSideDialog, RwUInt32 uiFlag);

	VOID		PlayOpenSound(RwInt32 iSideDialog);
	VOID		PlayCloseSound(RwInt32 iSideDialog);

	VOID		RegisterAttribute();

	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	static CSideDialogManager* m_pInstance;

	gui::CSlot				m_slotMove;

	MAP_SDIALOG				m_mapDialog;					///< Collection of pointers to registered dialogs
	MAP_PRESENTATION		m_mapPresentDialog;				///< Information on the currently displayed dialog

	MAP_ATTRIBUTE			m_mapAttribute;
};

static CSideDialogManager* GetSideDialogManager()
{
	return CSideDialogManager::GetInstance();
}

template <class Callbackclass>
VOID CSideDialogManager::RegistDialog(int iSideDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen))
{
	DialogInfo info;

	info.pDialog = pDialog;
	info.pCallSwitch = NTL_NEW CNtlCallbackWidget1bool<Callbackclass>(reinterpret_cast<Callbackclass*>(pDialog), callback);;

	m_mapDialog[iSideDialog] = info;

	info.pDialog->GetDialog()->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);
}