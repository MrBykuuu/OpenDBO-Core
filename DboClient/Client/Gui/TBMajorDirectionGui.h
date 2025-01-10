////////////////////////////////////////////////////////////////////////////////
// File: TBMajorDirectionGui.h
// Desc: World's Best Martial Arts Finals Direction Gui
//
// 2008.07.04 Peessi@hitel.net   
// 2008.09.18 Peessi@hitel.net Almost fell over.
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MAJOR_DIRECTION_GUI_H__
#define __TB_MAJOR_DIRECTION_GUI_H__

#include "TBCommonDirectionGui.h"

class CTBMajorDirectionGui : public CTBMainCommonDirectionGui
{
public:
	//! Enum
	enum FLASHID { WAIT, TITLE, ROUNDSTART, WINLOSE };

	//! Constructor and Destructor
	CTBMajorDirectionGui( const RwChar* pName );
	~CTBMajorDirectionGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! CallBack
	//VOID	OnTitleMovieEnd( gui::CComponent* pComponent );
	//VOID	OnWinLoseMovieEnd( gui::CComponent* pComponent );
	
	//gui::CSlot			m_slotTitleMovieEnd;	
	gui::CSlot			m_slotWinLoseMovieEnd;
};

#endif //__TB_MAJOR_DIRECTION_GUI_H__