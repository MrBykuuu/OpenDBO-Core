/******************************************************************************
*File           : GroupChatGui.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : October 17, 2008
*Abstract		:  
*****************************************************************************
*Desc           : Create a chat room and talk to people in the room
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

class CGroupChatGui : public CNtlPLGui
{
public:
	CGroupChatGui(const RwChar* pName);
	virtual ~CGroupChatGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
	CGroupChatGui() {}
};