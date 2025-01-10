/******************************************************************************
* File			: ChatLog.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 7. 2
* Abstract		: 
*****************************************************************************
* Desc			: Records chat logs
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Dbo
#include "ChatDefine.h"

// Simulation
#include "NtlSLDef.h"

class CChatGui;

struct sChatLog
{
	std::wstring		wstrString;
	RwUInt8				byChatType;
};

typedef std::list<sChatLog*>					LIST_CHATLOG;
typedef std::list<sChatLog*>::iterator			ITER_CHATLOG;


class CChatReceiver : public RWS::CEventHandler
{
public:
	CChatReceiver();
	virtual ~CChatReceiver();

	RwBool			Create(CChatGui* pChatGui);
	VOID			Destroy();

	VOID			RegisterNotify(CChatDisplayGui* pDisplayDialog);
	VOID			UnregisterNotify(CChatDisplayGui* pDisplayDialog);

	ITER_CHATLOG	Begin();
	ITER_CHATLOG	End();

protected:
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			Chatlog_Save(RwUInt8 byType, const WCHAR* pText);
	VOID			Chatlog_Notify(RwUInt8 byType, const WCHAR* pwcText);
	VOID			Chatlog_Notify(RwUInt8 byType, const WCHAR* pwcText, unsigned int uiColor);

	const WCHAR*	GetMsgTypeString(BYTE byMsgType, BYTE byChannelID);
	const WCHAR*	GetChannelString(BYTE byChannelID);

private:

	void			BattleNotification(RWS::CMsg& msg);

protected:
	CChatGui*			m_pChatGui;

	LIST_CHATLOG		m_listChatLog;			///< Chat log
	LIST_NOTIFY			m_listNotify;			///< Notification when chat contents are updated
};