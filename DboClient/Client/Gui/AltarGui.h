#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlDragonBall.h"

class CNtlSobItem;
class CNtlWorldConceptDBC;

/// dragon ball type
enum EDragonBallType
{
	E_DRAGONBALL_NONE,
	E_DRAGONBALL_1,
	E_DRAGONBALL_2,
	E_DRAGONBALL_3,
	E_DRAGONBALL_4,
	E_DRAGONBALL_5,
	E_DRAGONBALL_6,
	E_DRAGONBALL_7,
};

/// Dragon Ball Slot Structure
struct DragonBallSlot
{
	CRectangle		 m_rt;		///< Realm of Dragon Ball Slots
	CSurfaceGui		 m_surface;	///< Surface of Dragon Ball slot (required for texture setting)
	EDragonBallType	 m_eDBType;	///< Dragon ball type in slot
	CNtlSobItem*	 m_pSobItem; ///< Pointer to the item in the slot

	DragonBallSlot()
	{
		m_eDBType = E_DRAGONBALL_NONE;
		m_pSobItem = NULL;
	}
};


/**
 * \ingroup Client
 * \brief GUI that appears when you click on the Dragon Ball Dragon God Summoning Altar
 *Set 7 dragon balls and use them to summon the dragon god.
 * \date 2007-06-20
 * \author agebreak
 */
class CAltarGui	: public CNtlPLGui, public RWS::CEventHandler
{
public:
	CAltarGui(void);
	CAltarGui(const RwChar* pName);
	virtual ~CAltarGui(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	// Singleton create / remove function
// Almost no GUI is generated, so you only need to create it.
	static void		  CreateInstance();
	static void		  DeleteInstance();

protected:
	// CallBack
	VOID	OnClickExitBtn(gui::CComponent* pComponent);
	VOID	OnClickOKBtn(gui::CComponent* pComponent);
	VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID	OnMouseDown(const CKey& key);
	VOID	OnMouseUp(const CKey& key);
	VOID	OnPaint();
	VOID	OnInputBoxGotFocus();
	VOID	OnInputBoxLostFocus();
    VOID	OnCaptureMouseDown(const CKey& key);


	RwInt32	GetChildSlotIdx(RwInt32 nX, RwInt32 nY);	///< Calculate which slot it corresponds to when you click the mouse.

	VOID	SetDragonBallSlot(RwInt32 nSlotIdx, EDragonBallType eDragonBallType);	///< Set the dragon ball in the slot.
	VOID	RemoveDragonBallSlot(RwInt32 nSlotIdx);									///< Release the set dragon ball from the slot.
    RwBool  IsExistSameType(EDragonBallType eType);                                 ///< Check if a dragon ball of the same type (number) is already placed.

	VOID	SpawnDragon();								///< Execute the dragon god summoning production.

	

protected:
	static CAltarGui*	m_pInstance;			///< Singleton instance object	

	gui::CButton*		m_pBtnExit;				///< Exit button
	gui::CButton*		m_pBtnOK;				///< OK button
	gui::CInputBox*		m_pInKeyword;			///< Order input box
	gui::CPanel*		m_pPnlInputBack;		///< Input box background
	gui::CPanel*		m_apPnlLight[7];		///< Background that will change when Dragon Ball is inserted
	gui::CStaticBox*	m_pStaticKeyword;		///< Text before the order input box

	gui::CSlot			m_slotClickExitBtn;	
	gui::CSlot			m_slotClickOKBtn;	
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseDown;	
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
    gui::CSlot			m_slotCaptureMouseDown;

    DragonBallSlot      m_DBSlot[DRAGON_BALL_TYPE_COUNT][NTL_ITEM_MAX_DRAGONBALL];          ///< Slot to display Dragon Ball icon
	RwInt32				m_nDBCount;				///< Currently equipped Dragon Ball Count
	RwBool				m_bSpawnDragon;			///< Whether the Dragon God has been summoned
	RwBool				m_bDelete;

	CNtlWorldConceptDBC*	m_pWorldConceptDBC;	///< Pointer to the Dragon Ball Collection Controller

};
