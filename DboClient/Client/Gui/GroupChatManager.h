/******************************************************************************
*File           : GroupChatManager.h
*Author         : Hong SungBock
*Copyright      : NTL Co., Ltd.
*Date           : October 17, 2008
*Abstract       : 
*****************************************************************************
*Desc           : Group chat manager
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

class CGroupChatGui;

class CGroupChatManager : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef std::map<RwUInt32, CGroupChatGui*>			MAP_CHATGUI;

	CGroupChatManager(const RwChar* pName);
	virtual ~CGroupChatManager();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	VOID		AddText(RwUInt8 byChatType, const WCHAR* pwcText);

protected:
	CGroupChatManager() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
};