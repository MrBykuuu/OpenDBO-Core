/******************************************************************************
* File			: ChatSender.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 7. 2
* Abstract		: 
*****************************************************************************
* Desc			: Parsing of string entered through chat GUI and sending to chat server
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Dbo
#include "ChatDefine.h"

// Simulation
#include "NtlSLDef.h"

class CChatGui;
class CNtlSobActionSkill;

struct sCHAT_COMMAND_TBLDAT;


class CChatSender
{
public:
	struct sSOCIAL_ACTION
	{
		CNtlSobActionSkill*			pSobActionSkill;
		sCHAT_COMMAND_TBLDAT*		pCHAT_COMMAND_TBLDAT;
	};

	// In reality, I think it is rare to use social actions through commands.
	// The data structure was not defined using a multimap with social commands as keys.
	typedef std::map<SERIAL_HANDLE, sSOCIAL_ACTION*>		MAP_SOCIAL_ACTION;

public:	
	CChatSender();
	virtual ~CChatSender();

	RwBool		Create(CChatGui* pChatGui);
	VOID		Destroy();

	VOID		RegSocialAction(CNtlSobActionSkill* pSobActionSkill);
	VOID		UnregAllSocialAction();

	VOID		InputText(const WCHAR* pwcText);	

protected:
	VOID		Process_Send(const WCHAR* pwcText);
	eChatType	Process_Parsing(std::wstring& wstrOutputText, const WCHAR* pInputText, RwUInt8& bySendWorkType);

public:

	RwBool		HaveShortCut(std::wstring& wstrOutput, std::wstring& wstrOriginal,
								std::string strShortcutStringIndex, std::string strLocalShortcutStringIndex);

protected:

	SERIAL_HANDLE GetSocialActionHandle(std::wstring& wstrOriginal);

protected:
	CChatGui*			m_pChatGui;
	std::wstring		m_wstrLastUserName;				// Username that recently received whispers.
	MAP_SOCIAL_ACTION	m_mapSocialAction;
};