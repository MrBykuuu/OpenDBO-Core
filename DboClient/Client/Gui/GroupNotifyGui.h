/******************************************************************************
*File           : GroupNotifyGui.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : November 5, 2008
*Abstract       : 
*****************************************************************************
*Desc           : A person with more than a certain level of authority within any community group
*                 Used to send notification messages to other group members
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

class CGroupNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGroupNotifyGui(const RwChar* pName);
	virtual ~CGroupNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

protected:
	CGroupNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Update_FadeIn();
	VOID			Update_Idle();
	VOID			Update_FadeOut();

	VOID			PositionAlign(RwInt32 iScreenWidth, RwInt32 iScreenHeight);

protected:
	gui::CStaticBox*	m_pMessageStatic;

	RwReal				m_fElapsedTime;
	VOID				(CGroupNotifyGui::*m_pStateUpdate)();
};