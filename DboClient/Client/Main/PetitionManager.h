/******************************************************************************
* File			: PetitionManager.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 10. 16
* Abstract		: 
*****************************************************************************
* Desc			: Responsible for user complaints that occur during the game
*****************************************************************************/

#pragma once

// share
#include "NtlSharedType.h"
#include "NtlPetition.h"

class CPetitionManager
{
public:
	struct sPetitionInfo
	{
		PETITIONID			petitionID;
		RwInt32				iCategory;
		RwInt32				iCategory2;
		std::wstring		wstrPetitionContent;
	};

	struct sGMChat
	{
		ACCOUNTID			GMID;
		std::wstring		wstrGMName;
		RwBool				bGMChatting;
		RwBool				bNeedSatsifaction;
	};

	static RwBool	CreateInstance();
	static void		DestroyInstance();

	static CPetitionManager* GetInstance() { return m_pInstance; }

	CPetitionManager();
	virtual ~CPetitionManager();

	RwBool			Create();
	void			Destroy();

	void			FinishPetition();

	void			StartGMChatting(PETITIONID petitionID, ACCOUNTID GMID, const WCHAR* pwcName, RwBool bNeedSatsifaction);
	void			EndGMChatting();

	void			SetPetitionID(PETITIONID petitionID);
	void			SetPetitionContent(RwInt32 iCategory, RwInt32 iCategory2, const WCHAR* pwcText);	

	PETITIONID		GetPetitionID();
	RwInt32			GetCategory();
	RwInt32			GetCategory2();
	const WCHAR*	GetPetitionContent();

	ACCOUNTID		GetChattingGMID();
	const WCHAR*	GetChattingGMName();	

	RwBool			HavePetition();
	RwBool			IsGMChatting();
	RwBool			IsNeedSatisfation();

protected:
	static CPetitionManager* m_pInstance;

	sPetitionInfo		m_Petition;
	sGMChat				m_GMChat;
};

static CPetitionManager* GetPetitionManager(VOID)
{
	return CPetitionManager::GetInstance();
};