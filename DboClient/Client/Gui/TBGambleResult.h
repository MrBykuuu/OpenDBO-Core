/*****************************************************************************
 *
 *File			: TBGambleResultGui.h
 *Author	    : Haesung, Cho
*Copyright	    : (аж)NTL
 *Date			: 2008. 8. 20	
 *Abstract		: Tenkaichi budokai gamble npc result
 *****************************************************************************
 *Desc         : 
 *Confirm by floating the item that the gambling NPC spits out.
 *****************************************************************************/

#ifndef __TBGAMBLERESULT__H__
#define __TBGAMBLERESULT__H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SlotGui.h"
 

/**
* \ingroup client
* \brief When you gamble with a gambling NPC, the screen shows what items you got.
*/
class CTBGambleResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CTBGambleResultGui( const RwChar* pName );
	virtual ~CTBGambleResultGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg &msg);
	RwInt32		SwitchDialog(bool bOpen);

	VOID		SetGambleItem( RwInt32 hItemHandle );

	VOID		OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID		OnPaint();

	VOID		OnClickedBtnClose( gui::CComponent* pComponent );

protected:
	CRegularSlotGui			m_sgGambleItem;

	gui::CButton*		m_pBtnClose;
	gui::CSlot			m_slotClickedBtnClose;

	gui::CPanel*		m_pPnlSlot;
	gui::CStaticBox*	m_pStbMessage;

	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;
};
 
#endif//__TBGAMBLERESULT__H__