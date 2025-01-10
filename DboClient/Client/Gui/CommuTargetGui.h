#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"

/**
 * \ingroup Client
 * \brief Community target Windows GUI
 * \date 2007-12-26
 * \author agebreak
 */
class CCommuTargetGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CCommuTargetGui(void);
	CCommuTargetGui(const RwChar* pName);
	virtual ~CCommuTargetGui(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();	
	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

protected:
    VOID        OnEventTargetSelect(SNtlEventCommuTargetSelect* data);
    VOID        OnEventTargetRelease();

protected:
    gui::CStaticBox*    m_pSttName;                     ///< Display target name
    gui::CStaticBox*    m_pSttType;                     ///< Display target type (friend, guild. ..)

    SNtlEventCommuTargetSelect     m_currTarget;

};	
