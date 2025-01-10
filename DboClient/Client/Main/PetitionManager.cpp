#include "precomp_dboclient.h"
#include "PetitionManager.h"

// Core
#include "NtlDebug.h"

// Share
#include "NtlSharedType.h"
#include "NtlPetition.h"

// Framework
#include "NtlStage.h"

// Client
#include "StageManager.h"
#include "DboGlobal.h"


CPetitionManager* CPetitionManager::m_pInstance = NULL;

CPetitionManager::CPetitionManager()
{
	m_Petition.petitionID		= INVALID_PETITIONID;
	m_Petition.iCategory		= PETITION_CATEGORY_1_INVALID;
	m_Petition.iCategory2		= PETITION_CATEGORY_2_INVALID;

	m_GMChat.GMID				= INVALID_ACCOUNTID;
	m_GMChat.bNeedSatsifaction	= FALSE;
	m_GMChat.bGMChatting		= FALSE;
}

CPetitionManager::~CPetitionManager()
{

}

RwBool CPetitionManager::CreateInstance()
{
	NTL_ASSERT(!m_pInstance, "CPetitionManager::CreateInstance, Already exist petition manager");

	m_pInstance = NTL_NEW CPetitionManager;
	if( !m_pInstance->Create() )
	{
		CPetitionManager::DestroyInstance();
		return FALSE;
	}

	return TRUE;
}

void CPetitionManager::DestroyInstance()
{
	if( !m_pInstance )
		return;

	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

RwBool CPetitionManager::Create()
{
	return TRUE;
}

void CPetitionManager::Destroy()
{

}

void CPetitionManager::FinishPetition()
{
	m_Petition.petitionID	= INVALID_PETITIONID;
	m_Petition.iCategory	= PETITION_CATEGORY_1_INVALID;
	m_Petition.iCategory2	= PETITION_CATEGORY_2_INVALID;
	m_Petition.wstrPetitionContent.clear();
}

void CPetitionManager::StartGMChatting(PETITIONID petitionID, ACCOUNTID GMID, const WCHAR* pwcName, RwBool bNeedSatsifaction)
{
	CNtlStage* pStage = CStageManager::GetInstance()->GetActiveStage();
	if( !pStage )
	{
		// The user has not yet entered the world
		GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatStartRes(PETITION_NOT_YET_ENTER_WORLD, GMID);
		return;
	}

	std::string strStageName = pStage->GetName();
	if( strStageName != GAME_STAGE_NAME )
	{
		// The user has not yet entered the world
		GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatStartRes(PETITION_NOT_YET_ENTER_WORLD, GMID);
		return;
	}

	if( IsGMChatting() )
	{
		// User is already chatting with GM
		GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatStartRes(PETITION_AREADY_GM_CHATTING, GMID);
		return;
	}

		
	if( FALSE == GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatStartRes(CHAT_SUCCESS, GMID) )
		return;

	// Even if the user does not submit a complaint, the GM can create a complaint and actively chat with the user.
	m_Petition.petitionID			= petitionID;

	m_GMChat.bGMChatting			= TRUE;
	m_GMChat.bNeedSatsifaction		= bNeedSatsifaction;
	m_GMChat.GMID					= GMID;

	if( pwcName )
		m_GMChat.wstrGMName		= pwcName;
	else
		m_GMChat.wstrGMName		= L"GM";
}

void CPetitionManager::EndGMChatting()
{
	FinishPetition();

	m_GMChat.bGMChatting		= FALSE;
	m_GMChat.bNeedSatsifaction	= FALSE;
	m_GMChat.GMID				= INVALID_ACCOUNTID;
	m_GMChat.wstrGMName.clear();
}

void CPetitionManager::SetPetitionID(PETITIONID petitionID)
{
	NTL_ASSERT(m_Petition.petitionID == INVALID_PETITIONID, "CPetitionManager::SetPetitionID, Already exist petition ID");

	m_Petition.petitionID = petitionID;
}

void CPetitionManager::SetPetitionContent(RwInt32 iCategory, RwInt32 iCategory2, const WCHAR* pwcText)
{
	m_Petition.iCategory			= iCategory;
	m_Petition.iCategory2			= iCategory2;
	m_Petition.wstrPetitionContent	= pwcText;
}

PETITIONID CPetitionManager::GetPetitionID()
{
	return m_Petition.petitionID;
}

RwInt32 CPetitionManager::GetCategory()
{
	return m_Petition.iCategory;
}

RwInt32 CPetitionManager::GetCategory2()
{
	return m_Petition.iCategory2;
}

const WCHAR* CPetitionManager::GetPetitionContent()
{
	return m_Petition.wstrPetitionContent.c_str();
}

ACCOUNTID CPetitionManager::GetChattingGMID()
{
	return m_GMChat.GMID;
}

const WCHAR* CPetitionManager::GetChattingGMName()
{
	return m_GMChat.wstrGMName.c_str();
}

RwBool CPetitionManager::HavePetition()
{
	if( m_Petition.petitionID == INVALID_PETITIONID )
		return FALSE;

	return TRUE;
}

RwBool CPetitionManager::IsGMChatting()
{
	return m_GMChat.bGMChatting;
}

RwBool CPetitionManager::IsNeedSatisfation()
{
	return m_GMChat.bNeedSatsifaction;
}