#include "precomp_dboclient.h"
#include "ScrambleBalloonHelper.h"


// Core
#include "NtlDebug.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConcept.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobManager.h"
#include "NtlSobAvatarAttr.h"

// Client
#include "DboEventGenerator.h"
#include "ChatBalloonManager.h"


#define dSAFETY_CREATE_BATTLE_BALLOON_TICK_COUNT		(2)



ScrambleBalloonHelper::ScrambleBalloonHelper()
{

}

ScrambleBalloonHelper::~ScrambleBalloonHelper()
{
}

RwBool ScrambleBalloonHelper::Create()
{
	NTL_FUNCTION( "ScrambleBalloonHelper::Create" );

	LinkMsg(g_EventSobCreate);
	LinkMsg(g_EventSobDelete);
	LinkMsg(g_EventEndterWorld);
	LinkMsg(g_EventChangeWorldConceptState);

	NTL_RETURN(TRUE);
}

VOID ScrambleBalloonHelper::Destroy()
{
	NTL_FUNCTION("ScrambleBalloonHelper::Destroy");

	ClearBalloon();

	UnLinkMsg(g_EventSobCreate);
	UnLinkMsg(g_EventSobDelete);
	UnLinkMsg(g_EventEndterWorld);
	UnLinkMsg(g_EventChangeWorldConceptState);

	NTL_RETURNVOID();
}

VOID ScrambleBalloonHelper::ClearBalloon()
{
	MAP_PLAYER_BALLOON::iterator it_playerBalloon = m_mapPlayerBalloon.begin();
	for( ; it_playerBalloon != m_mapPlayerBalloon.end() ; ++it_playerBalloon )
	{
		sBALLOON_INFO& tBALLOON_INFO = it_playerBalloon->second;
		if( BATTLE_BALLOON_STATE_CREATED == tBALLOON_INFO.eBattleBalloonState )
		{
			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(it_playerBalloon->first);
			if( pSob )
				GetBalloonManager()->FadeOutBalloon(pSob, CBalloonGui::TYPE_BATTLEGAUGE);
		}
	}
	m_mapPlayerBalloon.clear();

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar )
		GetBalloonManager()->FadeOutBalloon(pAvatar, CBalloonGui::TYPE_BATTLEGAUGE);
}

VOID ScrambleBalloonHelper::Update(RwReal fElapsed)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild				= pAvatar->GetGuild();
	const WCHAR*	pwcAvatarGuildName	= pGuild->GetGuildName();


	MAP_PLAYER_BALLOON::iterator it_playerBalloon = m_mapPlayerBalloon.begin();
	while( it_playerBalloon != m_mapPlayerBalloon.end() )
	{
		sBALLOON_INFO& tBALLOON_INFO = it_playerBalloon->second;

		if( BATTLE_BALLOON_STATE_NOT_CREATE == tBALLOON_INFO.eBattleBalloonState )
		{
			if( tBALLOON_INFO.byTickCount < dSAFETY_CREATE_BATTLE_BALLOON_TICK_COUNT )
			{
				++tBALLOON_INFO.byTickCount;
				++it_playerBalloon;
				continue;
			}

			SERIAL_HANDLE hHandle = it_playerBalloon->first;
			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hHandle);
			if( !pSob )
			{
				DBO_FAIL("Not exist sob player of handle : " << hHandle);
				tBALLOON_INFO.eBattleBalloonState = BATTLE_BALLOON_STATE_FAIL_CREATE;
			}
			else
			{
				if( SLCLASS_PLAYER != pSob->GetClassID() )
				{
					it_playerBalloon = m_mapPlayerBalloon.erase(it_playerBalloon);
					continue;
				}

				// If the appearance is not visible, the speech bubble creation fails.

				if( !pSob->IsVisible() )
				{
					++it_playerBalloon;
					continue;
				}

				RwBool				bMyTeam			= FALSE;
				CNtlSobPlayer*		pSobPlayer		= reinterpret_cast<CNtlSobPlayer*>( pSob );
				CNtlSobPlayerAttr*	pSobPlayerAttr	= reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );


				if(_wcsicmp(pSobPlayer->GetGuildName(), pwcAvatarGuildName) == 0 )
					bMyTeam = TRUE;

				GetBalloonManager()->AddBattleGauge(pSobPlayer,
													pSobPlayerAttr->GetName(),
													pSobPlayerAttr->GetMaxLp(),
													pSobPlayerAttr->GetMaxLp(),
													bMyTeam);

				tBALLOON_INFO.eBattleBalloonState = BATTLE_BALLOON_STATE_CREATED;
			}
		}

		++it_playerBalloon;
	}
}

VOID ScrambleBalloonHelper::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("ScrambleBalloonHelper::HandleEvents");

	if( msg.Id == g_EventSobCreate )
	{
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		// avooo's comment: Why not create a battle speech bubble right away?
		//					 After the g_EventSobCreate event occurs and the Sob object is created, there is no separate notification.
		//					 Once a few ticks have passed since the event occurred and it is guaranteed that Sob data exists
		//					 Create a battle speech bubble
		SNtlEventSob* pEvent = reinterpret_cast<SNtlEventSob*>( msg.pData );

		if( INVALID_SERIAL_ID == pEvent->hSerialId )
			NTL_RETURNVOID();


		MAP_PLAYER_BALLOON::iterator it_playerBalloon = m_mapPlayerBalloon.find(pEvent->hSerialId);
		if( it_playerBalloon != m_mapPlayerBalloon.end() )
			NTL_RETURNVOID();

		sBALLOON_INFO tBALLOON_INFO;
		tBALLOON_INFO.eBattleBalloonState	= BATTLE_BALLOON_STATE_NOT_CREATE;
		tBALLOON_INFO.byTickCount			= 0;

		m_mapPlayerBalloon[pEvent->hSerialId] = tBALLOON_INFO;
	}
	else if( msg.Id == g_EventSobDelete )
	{
		SNtlEventSobDelete* pEvent = reinterpret_cast<SNtlEventSobDelete*>( msg.pData );

		MAP_PLAYER_BALLOON::iterator it_playerBalloon = m_mapPlayerBalloon.find(pEvent->hSerialId);
		if( it_playerBalloon == m_mapPlayerBalloon.end() )
			NTL_RETURNVOID();


		sBALLOON_INFO& tBALLOON_INFO = it_playerBalloon->second;
		if( BATTLE_BALLOON_STATE_CREATED == tBALLOON_INFO.eBattleBalloonState )
		{
			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pEvent->hSerialId);
			if( pSob )
				GetBalloonManager()->FadeOutBalloon(pSob, CBalloonGui::TYPE_BATTLEGAUGE);
		}

		m_mapPlayerBalloon.erase(it_playerBalloon);
	}
	else if( msg.Id == g_EventEndterWorld )
	{
		// The avatar basically shows a speech bubble for the stamp competition.
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			NTL_RETURNVOID();
		}

		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

		GetBalloonManager()->AddBattleGauge(pAvatar, pAvatarAttr->GetName(),
											pAvatarAttr->GetMaxLp(), pAvatarAttr->GetMaxLp(),
											TRUE);
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_DOJO_SCRAMBLE )
			NTL_RETURNVOID();

		if( WORLD_STATE_EXIT == pEvent->uiState )
		{
			ClearBalloon();
		}
	}	

	NTL_RETURNVOID();
}