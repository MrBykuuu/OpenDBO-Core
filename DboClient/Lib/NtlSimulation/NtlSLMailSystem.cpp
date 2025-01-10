#include "precomp_ntlsimulation.h"

// Core
#include "NtlDebug.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

#include "NtlSLMailSystem.h"

CNtlMailSystem::CNtlMailSystem()
:m_hObject(INVALID_SERIAL_ID)
,m_uiMailNum(0)
,m_bAway(FALSE)
,m_uiNotify(MAILSYSTEM_NOTIFY_VALUE_NONE)
,m_bMailStart(NULL)
,m_uiMailCount(0)
,m_uiUnreadMailCountNormal(0)
,m_uiUnreadMailCountManager(0)
,m_uiOldUnreadMailCountNormal(0)
,m_uiOldUnreadMailCountManager(0)
{
}

CNtlMailSystem::~CNtlMailSystem()
{
	ClearMail();
}

RwBool CNtlMailSystem::Create()
{
	NTL_FUNCTION("CNtlMailSystem::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	CheckMailSystemNotify(pAvatarInfo->byMailCount, pAvatarInfo->byUnreadMailCountNormal, pAvatarInfo->byUnreadMailCountManager);

	NTL_RETURN(TRUE);
}

void CNtlMailSystem::Destroy()
{
	NTL_FUNCTION("CNtlMailSystem::Destroy");

	ClearMail();

	NTL_RETURNVOID();
}

void CNtlMailSystem::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlMailSystem::HandleEvents");

	if (pMsg.Id == g_EventMailStart)
	{		
		SNtlEventMailStart* pEvent = reinterpret_cast<SNtlEventMailStart*>(pMsg.pData);

		m_bAway		= pEvent->bIsAway;
		m_bMailStart= TRUE;

		m_hObject	= pEvent->hSerial;
	}
	else if (pMsg.Id == g_EventMailLoadInfo)
	{
		SNtlEventMailLoadInfo* pEvent = reinterpret_cast<SNtlEventMailLoadInfo*>(pMsg.pData);

		AddMail(&pEvent->sMailProfile);
	}
	else if (pMsg.Id == g_EventMailLoadData)
	{
		SNtlEventMailLoadInfo* pEvent = reinterpret_cast<SNtlEventMailLoadInfo*>(pMsg.pData);

		AddMail(&pEvent->sMailProfile);
	}
	else if (pMsg.Id == g_EventMailRead)
	{
		SNtlEventMailRead* pEvent = reinterpret_cast<SNtlEventMailRead*>(pMsg.pData);

		sMAIL_PROFILE*	pMailProfile = GetMail(pEvent->mailID);
		if (pMailProfile)
		{
			if (!pMailProfile->bIsRead)
			{
				pMailProfile->bIsRead	= true;
				/*pMailProfile->byExpired	= pEvent->uiExpiredDay;*/
				pMailProfile->endTime = pEvent->endTime;

				if (pMailProfile->bySenderType == eMAIL_SENDER_TYPE_SYSTEM || pMailProfile->bySenderType == eMAIL_SENDER_TYPE_GM || pMailProfile->bySenderType == eMAIL_SENDER_TYPE_QUEST)
				{
					--m_uiUnreadMailCountManager;
					--m_uiOldUnreadMailCountManager;
				}
				else
				{
					--m_uiUnreadMailCountNormal;
					--m_uiOldUnreadMailCountNormal;
				}

				CheckMailSystemNotify(m_uiMailCount, m_uiUnreadMailCountNormal, m_uiUnreadMailCountManager);
			}
		}
		else
		{
			NTL_ASSERT(pMailProfile, "maildata is not a possibility of searching");
		}

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventMailDel)
	{
		SNtlEventMailDel* pEvent = reinterpret_cast<SNtlEventMailDel*>(pMsg.pData);

		DelMail(pEvent->mailID);
		CheckMailSystemNotify(m_uiMailNum, m_uiUnreadMailCountNormal, m_uiUnreadMailCountManager);

		m_hObject	= pEvent->hSerialID;
	}
	else if( pMsg.Id == g_EventMailMultiDelRes )
	{
		SNtlEventMailMultiDelRes* pResult = reinterpret_cast<SNtlEventMailMultiDelRes*>(pMsg.pData);

		for( RwInt32 i=0; i<pResult->byCount; ++i )
		{
			DelMail( pResult->aMailID[i] );
		}

		CheckMailSystemNotify( m_uiMailNum, m_uiUnreadMailCountNormal, m_uiUnreadMailCountManager );
		m_hObject = pResult->hObject;
	}
	else if (pMsg.Id == g_EventMailReturn)
	{
		SNtlEventMailReturn* pEvent = reinterpret_cast<SNtlEventMailReturn*>(pMsg.pData);

		DelMail(pEvent->mailID);
		CheckMailSystemNotify(m_uiMailNum, m_uiUnreadMailCountNormal, m_uiUnreadMailCountManager);

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventMailItemReceive)
	{
		SNtlEventMailItemReceive* pEvent = reinterpret_cast<SNtlEventMailItemReceive*>(pMsg.pData);

		sMAIL_PROFILE*	pMailProfile = GetMail(pEvent->mailID);
		if (pMailProfile)
		{
			pMailProfile->bIsAccept = true;
		}
		else
		{
			NTL_ASSERT(pMailProfile, "maildata is not a possibility of searching");
		}

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventMailReload)
	{
		SNtlEventMailReload* pEvent = reinterpret_cast<SNtlEventMailReload*>(pMsg.pData);

		ReloadMail(pEvent->uiMailCount, pEvent->pmailID);
		CheckMailSystemNotify(pEvent->uiMailCount, pEvent->uiUnreadMailCountNormal, pEvent->uiUnreadMailCountManager);

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventMailReloadNfy)
	{
		SNtlEventMailReloadNfy* pEvent = reinterpret_cast<SNtlEventMailReloadNfy*>(pMsg.pData);

		ReloadMail(pEvent->uiMailCount, pEvent->pmailID);
		CheckMailSystemNotify(pEvent->uiMailCount, pEvent->uiUnreadMailCountNormal, pEvent->uiUnreadMailCountManager);
	}
	else if (pMsg.Id == g_EventMailLoad)
	{
		SNtlEventMailLoad* pEvent = reinterpret_cast<SNtlEventMailLoad*>(pMsg.pData);

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventMailLock)
	{
		SNtlEventMailLock* pEvent = reinterpret_cast<SNtlEventMailLock*>(pMsg.pData);

		sMAIL_PROFILE*	pMailProfile = GetMail(pEvent->mailID);
		if (pMailProfile)
		{
			pMailProfile->bIsLock	= B2b(pEvent->bIsLock);
			/*pMailProfile->byExpired	= pEvent->uiExpiredDay;*/
			pMailProfile->endTime	= pEvent->endTime;
		}
		else
		{
			NTL_ASSERT(pMailProfile, "maildata is not a possibility of searching");
		}

		m_hObject	= pEvent->hSerialID;
	}
	else if (pMsg.Id == g_EventCharAway)
	{
		SNtlEventCharAway* pEvent = reinterpret_cast<SNtlEventCharAway*>(pMsg.pData);
		
		m_bAway = pEvent->bIsAway;
	}
	else if (pMsg.Id == g_EventMailCloseNfy)
	{
		m_hObject	= INVALID_SERIAL_ID;
	}

	NTL_RETURNVOID();
}


RwUInt32 CNtlMailSystem::AddMail(const sMAIL_PROFILE* pMailProfile)
{
	MAIL_PROFILE_MAP::iterator itMap = m_mapMailProfile.find(pMailProfile->mailID);
	NTL_ASSERT(itMap == m_mapMailProfile.end(), "map key was duplicated");
	sMAIL_PROFILE* pMailProfileNew = NULL;
	if (itMap == m_mapMailProfile.end())
	{
		pMailProfileNew = NTL_NEW sMAIL_PROFILE();
		memcpy(pMailProfileNew, pMailProfile, sizeof(sMAIL_PROFILE));

		m_mapMailProfile.insert(MAIL_PROFILE_MAP::value_type(pMailProfileNew->mailID, pMailProfileNew));
		m_vecMailID.push_back(pMailProfileNew->mailID);
	}	
	else
	{
		pMailProfileNew = itMap->second;
		memcpy(pMailProfileNew, pMailProfile, sizeof(sMAIL_PROFILE));
	}

	//SortMail();
	return m_uiMailNum = m_mapMailProfile.size();
}

RwUInt32 CNtlMailSystem::DelMail(const MAILID& mailID)
{
	MAIL_PROFILE_MAP::iterator itMap = m_mapMailProfile.find(mailID);
	if (itMap != m_mapMailProfile.end())
	{
		sMAIL_PROFILE* pMailProfile = itMap->second;

		NTL_DELETE(pMailProfile);
		m_mapMailProfile.erase(itMap);
	}

	MAIL_ID_VEC::iterator itVec = std::find(m_vecMailID.begin(), m_vecMailID.end(), mailID);
	if (itVec != m_vecMailID.end())
	{
		m_vecMailID.erase(itVec);
	}

	return m_uiMailNum = m_mapMailProfile.size();
}

void CNtlMailSystem::ClearMail()
{
	for (MAIL_PROFILE_MAP::iterator itMap = m_mapMailProfile.begin(); itMap != m_mapMailProfile.end(); ++itMap)
	{
		sMAIL_PROFILE* pMailProfile = itMap->second;
		NTL_DELETE(pMailProfile);
	}
	m_mapMailProfile.clear();
	m_vecMailID.clear();
	m_uiMailNum = 0;
}

void CNtlMailSystem::ReloadMail(RwUInt8 uiCount, MAILID* pmailID)
{
	// Determine whether MailStart has been performed at least once && whether it is possible to read
	if (!m_bMailStart)
		return;

	if ( ( GetAbility() & MAILSYSTEM_ABILITY_READ ) == FALSE )
		return;

	RwUInt8 uiIndex = 0;
	while (uiIndex < m_vecMailID.size())
	{
		MAILID mailID = m_vecMailID.at(uiIndex);
		RwBool bCheck = false;
		for (int i = 0; i < uiCount; ++i)
		{
			if (pmailID[i] == mailID)
			{
				bCheck = true;
				break;
			}
		}
		if (bCheck)
		{
			++uiIndex;
		}
		else
		{
			DelMail(mailID);
		}
	}

	// by daneos. Dont need to do this.

	/*RwUInt8	uiCountNew = 0;
	MAILID	aMailID[NTL_MAX_MAIL_SLOT_COUNT];

	for (int i = 0; i < uiCount; ++i)
	{
		MAIL_ID_VEC::iterator it = find(m_vecMailID.begin(), m_vecMailID.end(), pmailID[i]);
		if (it == m_vecMailID.end())
		{
			aMailID[uiCountNew++] = pmailID[i];
		}
	}
	if (uiCountNew > 0)
	{
		API_GetSLPacketGenerator()->SendMailLoadReq(m_hObject, uiCountNew, aMailID);
	}*/
}

RwBool CNtlMailSystem::GetMail(sMAIL_PROFILE& outMailProfile, const MAILID& mailID)
{
	sMAIL_PROFILE* pMailProfile = GetMail(mailID);
	if (pMailProfile)
	{
		memcpy(&outMailProfile, pMailProfile, sizeof(sMAIL_PROFILE));
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlMailSystem::GetMail(sMAIL_PROFILE& outMailProfile, const RwUInt32& uiIndex, const RwBool& bReverse)
{
	if (uiIndex < m_uiMailNum)
	{
		MAILID mailID;;
		if (bReverse)
		{
			mailID = m_vecMailID.at(m_uiMailNum - uiIndex - 1);
		}
		else
		{
			mailID = m_vecMailID.at(uiIndex);
		}

		return GetMail(outMailProfile, mailID);
	}
	return FALSE;
}

sMAIL_PROFILE* CNtlMailSystem::GetMail(const MAILID& mailID)
{
	sMAIL_PROFILE*				pMailProfile	= NULL;
	MAIL_PROFILE_MAP::iterator itMap		= m_mapMailProfile.find(mailID);
	if (itMap != m_mapMailProfile.end())
	{
		pMailProfile = itMap->second;
	}
	return pMailProfile;
}

RwUInt32 CNtlMailSystem::GetAbility()
{
	if (m_hObject != INVALID_SERIAL_ID)
	{
		return (RwUInt32)MAILSYSTEM_ABILITY_ALL;
	}

	return MAILSYSTEM_ABILITY_NONE;
}

void CNtlMailSystem::SortMail()
{
	std::sort(m_vecMailID.begin(), m_vecMailID.end(), SortMailCompare);
}

bool CNtlMailSystem::SortMailCompare(const MAILID& mailIDLeft, const MAILID& mailIDRight)
{
	return mailIDLeft < mailIDRight;
}

void CNtlMailSystem::CheckMailSystemNotify(RwUInt32 uiMailCount, RwUInt32 uiUnreadMailCountNormal, RwUInt32 uiUnreadMailCountManager)
{
	RwUInt32 uiNotify = MAILSYSTEM_NOTIFY_VALUE_NONE;

	m_uiMailCount				= uiMailCount;				// Total number of emails on the server
	m_uiUnreadMailCountNormal	= uiUnreadMailCountNormal;	// Unread regular mail
	m_uiUnreadMailCountManager	= uiUnreadMailCountManager; // Unread system mail (mail sent automatically by the system)

	//send mail
	if (m_uiMailCount == NTL_MAX_MAIL_SLOT_COUNT)
	{
		uiNotify |= MAILSYSTEM_NOTIFY_VALUE_MAIL_FULL;
	}
	if (m_uiUnreadMailCountNormal > 0)
	{
		uiNotify |= MAILSYSTEM_NOTIFY_VALUE_MAIL_NORMAL;
	}
	
	if (m_uiUnreadMailCountManager > 0)
	{
		uiNotify |= MAILSYSTEM_NOTIFY_VALUE_MAIL_MANAGER;
	}

	m_uiNotify = uiNotify;
	CNtlSLEventGenerator::MailSystemNfy(MAILSYSTEM_NOTIFY_TYPE_NONE, uiNotify);

	//Check if new mail has been updated
	RwBool bNewMail = FALSE;
	if( m_uiUnreadMailCountNormal > m_uiOldUnreadMailCountNormal )
	{
		m_uiOldUnreadMailCountNormal = m_uiUnreadMailCountNormal;
		bNewMail = TRUE;
	}
	
	if( m_uiUnreadMailCountManager > m_uiOldUnreadMailCountManager )
	{
		m_uiOldUnreadMailCountManager = m_uiUnreadMailCountManager;
		bNewMail = TRUE;
	}

	if( bNewMail )
		CNtlSLEventGenerator::MailSystemNfy(MAILSYSTEM_NOTIFY_TYPE_NEWMAIL, uiNotify);
}