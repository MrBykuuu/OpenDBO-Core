#include "precomp_dboclient.h"
#include "CharGuiGroup.h"

#include "NtlDebug.h"
#include "NtlPLGuiManager.h"

#include "CharCreateGui.h"
#include "CharSelectGui.h"
#include "ServerSelectGui.h"
#include "BackboardGui.h"


#define AddGuiMacro(pGui, classname, szName) \
    { \
        pGui = NTL_NEW classname(szName);\
        if(!pGui->Create())    \
        {                      \
        pGui->Destroy();   \
        NTL_DELETE(pGui);  \
        NTL_RETURN(FALSE); \
        }                      \
        CNtlPLGuiManager::GetInstance()->AddGui(pGui); \
    }


#define RemoveGuiMacro(pGui) \
    { \
        if(pGui)       \
        {              \
            CNtlPLGuiManager::GetInstance()->RemoveGui(pGui);     \
            pGui->Destroy();                                    \
            NTL_DELETE(pGui);                                   \
        } \
    }

CCharGuiGroup::CCharGuiGroup()
:m_pTurnOverBackboard(NULL)
,m_pCharCreate(NULL)
,m_pCharSelect(NULL)
,m_pServerSelect(NULL)
{

}

CCharGuiGroup::~CCharGuiGroup()
{
}


RwBool CCharGuiGroup::Create(void)
{
	NTL_FUNCTION("CCharGuiGroup::Create");

	gui::CResourceManager* pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	pResMgr->AddPage("gui\\Lobby.rsr");

	// Create a character 	
    AddGuiMacro(m_pCharCreate, CCharCreateGuiGui, "CharCreate");

	// character selection	
    AddGuiMacro(m_pCharSelect, CCharSelectGui, "CharSelect");

	// server select	
    AddGuiMacro(m_pServerSelect, CServerSelectGui, "ServerSelect");

	// Lobby screen transition effect	
    AddGuiMacro(m_pTurnOverBackboard, CBackboardGui, "LobbyTurnOver");

	NTL_RETURN(TRUE);
}

void CCharGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CCharGuiGroup::Destroy");

    RemoveGuiMacro(m_pCharCreate);
    RemoveGuiMacro(m_pCharSelect);
    RemoveGuiMacro(m_pServerSelect);
    RemoveGuiMacro(m_pTurnOverBackboard);

	gui::CResourceManager* pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	pResMgr->RemovePage("Lobby.rsr");

	NTL_RETURNVOID();
}

void CCharGuiGroup::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CCharGuiGroup::HandleEvents");

	NTL_RETURNVOID();
}