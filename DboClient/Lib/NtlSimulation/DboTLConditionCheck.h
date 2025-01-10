/*****************************************************************************
*
* File			: DboTLConditionCheck.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 4. 28
* Abstract		: 
*****************************************************************************
* Desc          : Manage the user's practice status during tutorials
*
*****************************************************************************/

#ifndef __NTL_TL_CONDITION__CHECK_H__
#define __NTL_TL_CONDITION__CHECK_H__

// core
#include "ceventhandler.h"

// simulation
#include "DboTLDefine.h"

#define dMAX_SIZE_MOUSE_INPUT_TYPE							(ETL_MOUSE_INPUT_TYPE_ALL/8 + 1)
#define dMAX_SIZE_KEYBOARD_INPUT_TYPE						(ETL_KEYBOARD_INPUT_TYPE_ALL/8 + 1)
#define dMAX_SIZE_ATTACK_INPUT_TYPE							(ETL_ATTACK_TYPE_ALL/8 + 1)

class CDboConditionCheckBase;

class CDboTLConditionCheck
{
public:
	typedef std::list<CDboConditionCheckBase*>					LIST_CONDITIONCHECK;
	typedef std::list<CDboConditionCheckBase*>::iterator		LIST_CONDITIONCHECK_ITER;


	CDboTLConditionCheck();
	virtual ~CDboTLConditionCheck();

	RwBool			Create();
	VOID			Update(RwReal fElapsed);
	VOID			Destroy();
	VOID			HandleEvents(RWS::CMsg &pMsg);

	RwBool			CanMouseInput(ETL_MOUSE_INPUT_TYPE byType);
	RwBool			CanKeybaordInput(ETL_KEYBOARD_INPUT_TYPE byType);
	RwBool			CanAttack(ETL_ATTACK_TYPE byType);

protected:
	VOID			SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
	VOID			UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt8 bySize);
	RwBool			IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

protected:
	RwChar			m_acMouseInput[dMAX_SIZE_MOUSE_INPUT_TYPE];
	RwChar			m_acKeyboardInput[dMAX_SIZE_KEYBOARD_INPUT_TYPE];
	RwChar			m_acAttack[dMAX_SIZE_ATTACK_INPUT_TYPE];

	LIST_CONDITIONCHECK		m_listConditoinCheck;
};

#endif