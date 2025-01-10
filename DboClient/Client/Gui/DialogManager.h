/******************************************************************************
*File			: DialogManager.h
*Author	    	: Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2006. 8. 11
*Abstract		: 
*****************************************************************************
*Desc			: Dialog Manager
*****************************************************************************/

#pragma once

#include <map>
#include <set>

// Core
#include "NtlDebug.h"
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"

// Gui
#include "gui_guimanager.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "DialogDefine.h"
#include "DialogPriority.h"

class CNtlPLGui;
class CNtlSob;
class CNtlSobItem;

#define dREGULARDIALOG_FIRST_X		37
#define dREGULARDIALOG_GAP			10
#define dCHARM_Y					450
#define dNEED_SIZE_MORE_REGULAR		379	///< Allowance required to open one more Regular Dialog
#define dDAFAULT_REGULAR_COUNT		2	///< Number of regular dialogs that can be opened by default



class CDialogManager : public RWS::CEventHandler
{
public:
	static CDialogManager*	GetInstance();

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

	struct sOpenTarget
	{
		CNtlSob*				pOpenedTarget;	///< NPC Pointer to the target that opened the window
		RwV3d					v3Pos;
	};

	struct sRequestedOpenDialog
	{
		RwInt32					eRequestedDialogType;
		SERIAL_HANDLE			hSerial;
		RwBool					bSound;
	};

	typedef std::list<eDialogType>						LIST_OPENED_REGULAR_DIALOG;
	typedef std::list<eDialogType>::iterator			LIST_OPENED_REGULAR_ITER;

	typedef std::map<int, DialogInfo>					DIALOG_MAP;
	typedef std::map<int, DialogInfo>::iterator			DIALOG_MAP_ITER;
	typedef std::map<int, DialogInfo>::value_type		DIALOG_MAP_VALUE;

	typedef std::map<eDialogType, RwUInt32>				REGULAR_MAP;
	typedef std::map<eDialogType, RwUInt32>::iterator	REGULAR_MAP_ITER;

	typedef std::list<sRequestedOpenDialog>				LIST_REQUESTED_OPENDIALOG_IN_NARRATION;
	typedef std::list<sRequestedOpenDialog>::iterator	LIST_REQUESTED_OPENDIALOG_IN_NARRATION_ITER;

	// Entire Focusing Dialog
	typedef REGULAR_MAP									FE_MAP;
	typedef REGULAR_MAP_ITER							FE_MAP_ITER;

	typedef std::list<int>								DIALOG_RAISE;

	static CDialogManager* m_pInstance;

	virtual ~CDialogManager();
	
	static VOID	CreateInstance(gui::CGUIManager* pGuiManager);								///< Manager initialization	
	static VOID	DestoryInstance();

	VOID		Update(RwReal fElapsed);

	template <class Callbackclass>
	VOID		RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen));	///< Register the dialog.	
	VOID		UnRegistDialog(int iDialog);		///< Disables the relevant dialog.	

	VOID		SwitchBag(RwBool bOpen);			///< Show bag On/Off
	VOID		SwitchBagByIndex(RwInt32 nIndex);
	RwBool		SwitchDialog(int iDialog);			///< Opens or closes the dialog.	

	VOID		OpenGMDialog();						///< Open all GM dialogs
	VOID		OpenDefaultDialog();				///<Opens the basic dialog
	RwBool		OpenDialog(int iDialog, SERIAL_HANDLE hSerial = INVALID_SERIAL_ID, RwBool bPlaySound = TRUE); ///< Open a dialog, uiNPCSerial: Use NPC dialog

	RwBool		CloseDialog(int iDialog, RwBool bPlaySound = TRUE);				///< Closes the dialog.
	VOID		CloseGMDialog();					///< Close all GM dialogs
	VOID		CloseNotDefaultDialog();			///< Closes all dialogs except the default dialog.
	VOID		CloseAll();							///< Close all dialogs.


	VOID		RaiseDialogbyUser(RwInt32 iDialog);	///< The moment the dialog appears on the screen by the user, it is notified that it is being drawn at the top.
	VOID		HideDialogbyUser(RwInt32 iDialog);	///< The moment the dialog disappears from the screen by the user, it is notified that it will not be drawn on the screen.
		
	RwInt32		GetOpendNPCDIalog();				///< Check the open NPC dialog.	
	CNtlSob*	GetOpenedTarget();

	DialogInfo*	GetDialogInfo(int iDialog);			///< Returns the information of the relevant dialog.
	CNtlPLGui*	GetDialog(int iDialog);			///< Returns the pointer to the corresponding dialog.
	RwInt32		GetTopDialog();						///< Returns the index of the dialog being drawn at the top.

	int			HandleDialogAction(unsigned int iAction); ///< Process the action. by Kell
	VOID		HandleMouse(const CKey& key);		///< Handles the mouse

	VOID		OnMode(eDialogMode eMode);			///< Mode settings
	VOID		OffMode();							///< Release the mode
	eDialogMode	GetMode();
	RwInt32		HowManyOpenMaxRegular();			///< Returns the maximum number of regular dialogs that can be opened in the current client width.
	RwInt32		GetPosY_of_HalfRegular();			///< Returns the Y coordinate of a regular dialog about half the length
	VOID		Disable_OnObserver(RwBool bVisible);	///< Determines whether to show some dialogs in Observer mode

	VOID		LocateBag(RwInt32 iWidth, RwInt32 iHeight, RwBool bForce = FALSE);///< Arrange the bag in the appropriate position
	VOID		LocationDialogs(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_MovableDefaultPosition();	///< Specifies the default position when the dialog can be moved

	RwBool		IsExistDialog(RwInt32 iDialog);		///< Whether the dialog exists
	RwBool		CanMoveItem_in_Capsule();			///< Determine whether the icon in the capsule can be moved
	RwBool		CanPickupItem_in_Capsule();			///< Determine whether the icon in the capsule can be picked up
	RwBool		IsOpenDialog(int iDialog);			///< Checks whether the dialog is open.
	RwBool		IsBagOpen();						///< t : One or more bags are open.
	RwBool		IsOpenNPCDialog();					///< Check whether the NPC-related dialog is open.
	RwBool		IsNPCDialog(RwInt32 iDialog);		///< Check whether it is an NPC-related dialog.
	RwBool		IsTriggerSystemDialogOpen();		///< Check whether the Quest or Trigger related window is open. by peessi
	RwBool		IsMode(eDialogMode eMode);			///< Find out about dialog-related modes
	RwBool		IsRegularDialog(eDialogType eType);
	RwBool		IsEFDialog(eDialogType eType);
	RwBool		IsOpenEFDialog();
	RwBool		IsHaveAttribute(eDialogType eType, RwUInt32 uiAttr);
	RwBool		CanOpenRegularDialog();
	RwBool		IsDialogMovable();
	//// Test Func				///woosungs_test 20090723
	CNtlPLGui* const GetpDialogTEST(const char* szFrmFileName);
	RwBool		ShowDialogTEST(RwInt32 iDialog, bool bOpen = true);
	VOID		ShowAllDialogTEST(bool bOpen = true);

	void			SetClickedItem(CNtlSobItem* pItem) { m_pClickedItem = pItem; }
	CNtlSobItem*	GetClickedItem() { return m_pClickedItem; }

protected:
	CDialogManager();
	CDialogManager(const CDialogManager& dialog) {};

	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID		RegisteRegularDialog();
	VOID		RegisteEntireFocusingDialog();
	
	VOID		ProcessESC();						///< Handled when the ESC key is pressed	
	VOID		ProcessOpenRegular(eDialogType eDialog);
	VOID		ProcessLayer(eDialogType eDialog);		///< Organize the dialog layers
	VOID		ProcessCloseRegular(eDialogType eDialog);
	VOID		ProcessMovableDialogs(RwBool bMovable);

	VOID		Locate_AutoPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserDefaultPosition(RwInt32 iWidth, RwInt32 iHeight);	

	VOID		PlayOpenSound(RwInt32 iDialog);		///< Sound effect when the user opens the dialog
	VOID		PlayCloseSound(RwInt32 iDialog);	///< Sound effect when the user closes the dialog

protected:
	gui::CSlot		m_slotMouse;

	DIALOG_MAP		m_mapDialog;					///< Collection of pointers to registered dialogs
	sOpenTarget		m_OpenedTarget;
	
	eDialogMode		m_eMode;						///< mode
	RwInt32			m_iRegularStrartY;
	RwInt32			m_iOldWidth;
	RwInt32			m_iOldHeight;

	RwBool			m_bExpectingCloseNPCDialog;
	RwBool			m_bESCSkip;
	RwBool			m_bDialogMovable;
	RwBool			m_bCanVisible_OnOberserver;

	LIST_OPENED_REGULAR_DIALOG m_listOpenedRegularDilaog;
	LIST_REQUESTED_OPENDIALOG_IN_NARRATION m_listRequestedOpenDialogInNarration;	
	
	DIALOG_RAISE	m_listRaiseDialogbyUser;		///< Collection of GUIs that appear on the screen through user operation
													///< Sort in ascending order by the last dialog drawn.

	REGULAR_MAP		m_mapRegular;					///< Collection of dialogs that open at a specified location on the screen
	FE_MAP			m_mapEntireFocuing;				///< Dialog to focus the user's perspective by using the entire screen area

	// If we clicked an item (like dogi ball)
	CNtlSobItem*	m_pClickedItem;
};

static CDialogManager* GetDialogManager(VOID)
{
	return CDialogManager::GetInstance();
}

template <class Callbackclass>
VOID CDialogManager::RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen))
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if (pDialogInfo)
	{
		return;
	}

	DialogInfo info;

	pDialog->SetMovable(m_bDialogMovable);

	info.pDialog = pDialog;
	info.pCallSwitch = NTL_NEW CNtlCallbackWidget1bool<Callbackclass>(reinterpret_cast<Callbackclass*>(pDialog), callback);

	m_mapDialog[iDialog] = info;

	// EFDialog prioritization
	if( IsEFDialog((eDialogType)iDialog) )
		info.pDialog->GetDialog()->SetPriority(dDIALOGPRIORITY_EFDIALOG);

	// Regular Dialog, Bag, and Side Dialog use dDIALOGPRIORITY_EFDIALOG
}