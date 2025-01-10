/*****************************************************************************
*File			: DboPackethandler_TU.cpp
*Author		    : Hong sungbock
*Copyright		: NTL Co., Ltd.
*Date			: 2008. 2. 25
*Abstract		: 
*****************************************************************************
*Desc           : Game Server & Community Server Guild Packet Handle
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// Shared
#include "NtlGuild.h"
#include "DojoTable.h"
#include "ObjectTable.h"

// Presentation
#include "NtlPLVisualManager.h"
#include "NtlPLDojoContainer.h"

// Simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlCameraController.h"
#include "NtlSobPlayer.h"
#include "NtlSobGroup.h"

// Dbo
#include "IconMoveManager.h"
#include "LobbyManager.h"


void PacketHandler_GSGuild_Create_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_GUILD_CREATE_RES );

	// Guild creation results
	sGU_GUILD_CREATE_RES* pResult = (sGU_GUILD_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CREATE_RES"), TRUE );
		return;
	}

	// I created a guild
	GetAlarmManager()->AlarmMessage("DST_GUILD_CREATE");
}

void PacketHandler_GSGuild_Invite_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_GUILD_INVITE_RES );

	// Guild invitation results
	sGU_GUILD_INVITE_RES* pResult = (sGU_GUILD_INVITE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_INVITE_RES"), TRUE );
		return;
	}

	// %s has been invited to the guild.
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_INVITE", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_CreateNfy(void *pPacket)
{
	// Notify other members that a guild has been created
	sTU_GUILD_CREATED_NFY *pResult = (sTU_GUILD_CREATED_NFY*)pPacket;

	// %s guild has been created
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CREATE_NFY", FALSE, NULL, pResult->wszGuildName);
}

void PacketHandler_TUGuild_Info(void *pPacket)
{
	// Receive guild information
	sTU_GUILD_INFO* pResult = (sTU_GUILD_INFO*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_NOTICE, wcslen(pResult->guildInfo.awchNotice),
													pResult->guildInfo.awchName,
													pResult->guildInfo.awchNotice);

	CNtlSLEventGenerator::SLGuildEvent(SLGE_GUILD_INFO, INVALID_SERIAL_ID, (void*)&pResult->guildInfo);
}

void PacketHandler_TUGuild_MemberInfo(void *pPacket)
{
	// Receive guild member information
	sTU_GUILD_MEMBER_INFO *pResult = (sTU_GUILD_MEMBER_INFO*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_ADD_MEMBER, INVALID_SERIAL_ID, (void*)&pResult->guildMemberInfo);

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar && pResult->guildMemberInfo.charId == pSobAvatar->GetCharID() )
	{
		SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
		CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
		if( !pLobby )
		{
			DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
			return;
		}

		RwUInt8 bySelectedCharIndex = pLobby->GetSelectedCharacterIndex();

		sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelectedCharIndex );
		if( !pLOBBY_CHARACTER )
		{
			DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
			return;
		}

		RwUInt32 uiMapNameIndex = pLOBBY_CHARACTER->tSummary.dwMapInfoIndex;
		CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_POS, pResult->guildMemberInfo.charId, (void*)&uiMapNameIndex);
	}
}

void PacketHandler_TUGuild_DisbandRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISBAND_RES);

	// Notify the guild leader that the guild is on standby for dissolution.
	sTU_GUILD_DISBAND_RES *pResult = (sTU_GUILD_DISBAND_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISBAND_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_BEING_DISBAND, INVALID_SERIAL_ID, (void*)&pResult->timeToDisband);
}

void PacketHandler_TUGuild_BeingDisbanded_Nfy(void *pPacket)
{
	// Notify guild members that the guild is waiting to be disbanded.
	sTU_GUILD_BEING_DISBANDED_NFY *pResult = (sTU_GUILD_BEING_DISBANDED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_BEING_DISBAND, INVALID_SERIAL_ID, (void*)&pResult->timeToDisband);
}

void PacketHandler_TUGuild_Disband_Cancel_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISBAND_CANCEL_RES);

	// Notify the guild leader that the guild has been canceled while waiting for dissolution.
	sTU_GUILD_DISBAND_CANCEL_RES *pResult = (sTU_GUILD_DISBAND_CANCEL_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISBAND_CANCEL_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CANCLE_DISBAND);
}

void PacketHandler_TUGuild_Disband_Canceled_Nfy(void *pPacket)
{
	// Notify the deputy guild leader and guild members that the guild has been canceled while waiting for dissolution.
	CNtlSLEventGenerator::SLGuildEvent(SLGE_CANCLE_DISBAND);
}

void PacketHandler_TUGuild_Disband_Nfy(void *pPacket)
{
	// Notify the deputy guild leader and guild members that the guild has been disbanded.
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISBAND);

	// The guild has been disbanded
	GetAlarmManager()->AlarmMessage("DST_GUILD_DISBAND");
}

void PacketHandler_TUGuild_Invited_Nfy(void *pPacket)
{
	// Receive an invitation to join a guild
	sTU_GUILD_INVITED_NFY *pResult = (sTU_GUILD_INVITED_NFY*)pPacket;

	// Would you like to join %s guild?
	GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_INVITE_REQ", FALSE, NULL, pResult->wszGuildName );
}

void PacketHandler_TUGuild_Response_Invitation_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_RESPONSE_INVITATION_RES);

	// Notifies you of your guild membership results
	sTU_GUILD_RESPONSE_INVITATION_RES *pResult = (sTU_GUILD_RESPONSE_INVITATION_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_RESPONSE_INVITATION_RES"), TRUE);
		return;
	}
}

void PacketHandler_TUGuild_Member_Joined_Nfy(void *pPacket)
{
	// Notifies existing guild members that a new guild member has joined.
	sTU_GUILD_MEMBER_JOINED_NFY *pResult = (sTU_GUILD_MEMBER_JOINED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_ADD_MEMBER, INVALID_SERIAL_ID, (void*)&pResult->memberInfo);

	// %s joined the guild
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_NEW_MEMBER", FALSE, NULL, pResult->memberInfo.wszMemberName);
}

void PacketHandler_TUGuild_Invitation_Declined_Nfy(void *pPacket)
{
	// I declined the guild invitation.
	sTU_GUILD_INVITATION_DECLINED_NFY *pResult = (sTU_GUILD_INVITATION_DECLINED_NFY*)pPacket;

	// %s refused to join the guild
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DECLINE", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_Invitation_Expired_Nfy(void *pPacket)
{
	// Didn't respond to guild invitation
	sTU_GUILD_INVITATION_EXPIRED_NFY *pResult = (sTU_GUILD_INVITATION_EXPIRED_NFY*)pPacket;

	// %s did not respond to your guild membership request.
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_EXPIRED", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_Leave_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_LEAVE_RES);

	// He left the guild
	sTU_GUILD_LEAVE_RES *pResult = (sTU_GUILD_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_LEAVE_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISBAND);
}

void PacketHandler_TUGuild_Member_Left_Nfy(void *pPacket)
{
	// Notify the deputy guild leader and guild members that a guild member has withdrawn.
	sTU_GUILD_MEMBER_LEFT_NFY *pResult = (sTU_GUILD_MEMBER_LEFT_NFY*)pPacket;	

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s has left the guild
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_LEAVE_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DEL_MEMBER, pResult->memberCharId);
}

void PacketHandler_TUGuild_Kick_Out_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_KICK_OUT_RES);

	// A guild member was expelled
	sTU_GUILD_KICK_OUT_RES *pResult = (sTU_GUILD_KICK_OUT_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_KICK_OUT_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->kickedOutMemberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->kickedOutMemberCharId);
		return;
	}
}

void PacketHandler_TUGuild_Member_Kicked_Out_Nfy(void *pPacket)
{
	// Notify guild members that a guild member has been expelled
	sTU_GUILD_MEMBER_KICKED_OUT_NFY* pResult = (sTU_GUILD_MEMBER_KICKED_OUT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s has been kicked out of the guild
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_KICKOUT_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DEL_MEMBER, pResult->memberCharId);
}


void PacketHandler_TUGuild_Appoint_Second_Master_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_APPOINT_SECOND_MASTER_RES);

	// Notify the guild leader that the deputy guild leader has been appointed.
	sTU_GUILD_APPOINT_SECOND_MASTER_RES* pResult = (sTU_GUILD_APPOINT_SECOND_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_APPOINT_SECOND_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->secondMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->secondMasterCharId);
		return;
	}

	// %s became deputy guild leader
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_SECOND_MASTER_APPOINTED", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_SECOND_MASTER, pResult->secondMasterCharId);
}

void PacketHandler_TUGuild_Second_Master_Appointed_Nfy(void *pPacket)
{
	// Notify the deputy guild leader and guild members that the deputy guild leader has been appointed.
	sTU_GUILD_SECOND_MASTER_APPOINTED_NFY* pResult = (sTU_GUILD_SECOND_MASTER_APPOINTED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s became deputy guild leader
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_SECOND_MASTER_APPOINTED", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_SECOND_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_DismissSecondMasterRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISMISS_SECOND_MASTER_RES);

	// Result of dismissal from position as vice guild leader
	sTU_GUILD_DISMISS_SECOND_MASTER_RES* pResult = (sTU_GUILD_DISMISS_SECOND_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISMISS_SECOND_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->secondMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->secondMasterCharId);
		return;
	}

	// %s became a regular guild member from assistant guild master.
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DISMISS_SECOND_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISMISS_SECOND_MASTER, pResult->secondMasterCharId);
}

void PacketHandler_TUGuild_SecondMasterDismissedNfy(void *pPacket)
{
	// Notify guild members that the deputy guild leader has been relieved of his position.
	sTU_GUILD_SECOND_MASTER_DISMISSED_NFY* pResult = (sTU_GUILD_SECOND_MASTER_DISMISSED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s became a regular guild member from assistant guild master.
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DISMISS_SECOND_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISMISS_SECOND_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_Change_Guild_Master_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_CHANGE_GUILD_MASTER_RES);

	// Notify the previous guild leader that the guild leader has changed.
	sTU_GUILD_CHANGE_GUILD_MASTER_RES* pResult = (sTU_GUILD_CHANGE_GUILD_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_CHANGE_GUILD_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->newMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->newMasterCharId);
		return;
	}

	// %s became the guild leader
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CHANGE_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_MASTER, pResult->newMasterCharId);
}

void PacketHandler_TUGuild_Guild_Master_Changed_Nfy(void *pPacket)
{
	// Notify guild members that the guild leader has changed.
	sTU_GUILD_GUILD_MASTER_CHANGED_NFY* pResult = (sTU_GUILD_GUILD_MASTER_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s became the guild leader
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CHANGE_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_MemberPositionChangedNfy(void* pPacket)
{
	// Change a guild member's region
	sTU_GUILD_MEMBER_POSITION_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_POSITION_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_POS, pResult->memberCharId, (void*)&pResult->newMapNameTblidx);
}

void PacketHandler_TUGuild_MemberClassChangedNfy(void* pPacket)
{
	// Change class of guild member
	sTU_GUILD_MEMBER_CLASS_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_CLASS_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s has become class %s
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CLASS_CHANGE_MEMBER", FALSE, NULL, pMember->wszMemberName, Logic_GetClassName(pResult->byNewClass));
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_CLASS, pResult->memberCharId, (void*)&pResult->byNewClass);
}

void PacketHandler_TUGuild_MemberLevelChangedNfy(void* pPacket)
{
	// Change the level of guild members
	sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s has reached level %d
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_LEVEL_CHANGE_MEMBER", FALSE, NULL, pMember->wszMemberName, pResult->byNewLevel);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_LEVEL, pResult->memberCharId, (void*)&pResult->byNewLevel);
}

void PacketHandler_TUGuild_ReputationChangedNfy(void* pPacket)
{
	// Change a guild member's reputation
	sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_REPUTATION, pResult->memberCharId, (void*)&pResult->dwNewReputation);
}

void PacketHandler_TUGuild_MemberOnlineNfy(void* pPacket)
{
	// A guild member has accessed the game.
	sTU_GUILD_MEMBER_ONLINE_NFY* pResult = (sTU_GUILD_MEMBER_ONLINE_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s is online
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_ONLINE_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_ONLINE, pResult->memberCharId, (void*)&pResult->newMapNameTblidx);
}

void PacketHandler_TUGuild_MemberOfflineNfy(void* pPacket)
{
	// A guild member left the game
	sTU_GUILD_MEMBER_OFFLINE_NFY *pResult = (sTU_GUILD_MEMBER_OFFLINE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_OFFLINE, pResult->memberCharId);
}

void PacketHandler_GSGuild_NameChanged_Res(void* pPacket)
{
	// Notify the guild leader that the guild's name has changed.
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CHANGE_NAME_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_CHANGE_NAME_RES* pResult = (sGU_GUILD_CHANGE_NAME_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CHANGE_NAME_RES"), TRUE );
		return;
	}

	// Guild name has been changed
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NAME");
}

void PacketHandler_GSGuild_NameChanged_Nfy(void* pPacket)
{
	sGU_GUILD_NAME_CHANGED_NFY* pResult = (sGU_GUILD_NAME_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::ChangeGuildName(pResult->hSubject, pResult->wszGuildName);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// Blocks until packets are cleared on the server side.
		/*
		// Guild name has been changed
		GetAlarmManager()->AlarmMessage(DST_GUILD_CHANGED_GUILD_NAME);
		*/
	}
}

void PacketHandler_GSGuild_FunctionAdd_Res(void* pPacket)
{
	// Notifies that the guild's functions have changed
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_FUNCTION_ADD_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_FUNCTION_ADD_RES* pResult = (sGU_GUILD_FUNCTION_ADD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_FUNCTION_ADD_RES"), TRUE );
		return;
	}	

	// Packet showing actual function change and message : PacketHandler_TUGuild_FunctionChange_Nfy
}

void PacketHandler_TUGuild_FunctionChange_Nfy(void* pPacket)
{
	// Guild or Dojo functions have changed
	sTU_GUILD_FUNCTION_CHANGE_NFY* pResult = (sTU_GUILD_FUNCTION_CHANGE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_FUNCTION, INVALID_DWORD, (VOID*)&pResult->qwGuildFunctionFlag, (void*)&pResult->byNewFunc);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return;

	// The function has changed in the guild
	if(  INVALID_BYTE == pResult->byNewFunc )
	{
		// Existing function removed
	}
	else if( IsGuildFunction((eDBO_GUILD_FUNCTION)pResult->byNewFunc) )
	{
		if (pAvatar->GetGuild()->IsGuildMaster(pAvatar->GetCharID()))
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_ADD_FUNCTION_NOTIFY_TO_MASTER", FALSE, NULL, Logic_GetGuildFunctionString(pResult->byNewFunc));
		else
			GetAlarmManager()->AlarmMessage("DST_GUILD_ADD_FUNCTION_NOTIFY_TO_MEMBER");
	}
	else if( IsDojoFunction((eDBO_GUILD_FUNCTION)pResult->byNewFunc) )
	{
		if (pAvatar->GetGuild()->IsGuildMaster(pAvatar->GetCharID()))
			GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_ADD_FUNCTION_NOTIFY_TO_MASTER", FALSE, NULL, Logic_GetGuildFunctionString(pResult->byNewFunc));
		else
			GetAlarmManager()->AlarmMessage("DST_DOJO_ADD_FUNCTION_NOTIFY_TO_MEMBER");
	}
	else
	{
		DBO_FAIL("Invalid guild function: " << pResult->byNewFunc);
	}
}

void PacketHandler_GSGuild_GiveZenny_Res(void* pPacket)
{
	// Zenny was invested in the guild.
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_GIVE_ZENNY_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_GIVE_ZENNY_RES* pResult = (sGU_GUILD_GIVE_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_GIVE_ZENNY_RES"), TRUE );
		return;
	}

	// Not implemented yet
	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_INVEST_ZENNY);

	// Zenny donated
	GetAlarmManager()->AlarmMessage("DST_GUILD_CONTRIBUTION_ZENNY_NOTIFY");
}

void PacketHandler_TUGuild_GiveZenny_Nfy(void* pPacket)
{
	// Announces that a guild member has invested Zenny
	sTU_GUILD_GIVE_ZENNY_NFY* pResult = (sTU_GUILD_GIVE_ZENNY_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->charId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->charId);
		return;
	}

	// %s donated %d Zenny
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CONTRIBUTION_ZENNY_OTHERS", FALSE, NULL, pMember->wszMemberName, Logic_FormatZeni(pResult->dwZenny));
}

void PacketHandler_GSGuild_WarehouseStart_Res(void* pPacket)
{
	// Open the guild warehouse
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_START_RES);

	sGU_GUILD_BANK_START_RES* pResult = (sGU_GUILD_BANK_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		Logic_CancelNpcFacing();

		// Originally, it was correct to receive and process the event below, but at this point, the guild warehouse GUI class object
		// Since it has not been created, processing when an event is received is done here.
		// Raising the event is due to unity with other logic that generates DIALOGEVENT_OPEN_FAIL_NPC_DIALOG
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_GUILD_WAREHOUSEBAR);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_START_RES"), TRUE );
		return;
	}

	CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
	pWorldConceptController->ChangeState(WORLD_NPC_GUILD_WAREHOUSE);

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_START, pResult->handle);
}

void PacketHandler_GSGuild_WarehouseMove_Res(void* pPacket)
{
	// Move items from the guild warehouse
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_MOVE_RES);

	sGU_GUILD_BANK_MOVE_RES* pResult = (sGU_GUILD_BANK_MOVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_MOVE_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSLEventGenerator::GuildWarehouseItemMove(pResult->hSrcItem, pResult->hDstItem,
		pResult->bySrcPlace, pResult->bySrcPos, pResult->byDestPlace, pResult->byDestPos);
}

void PacketHandler_GSGuild_WarehouseMoveStack_Res(void* pPacket)
{
	// Move stack items from guild warehouse
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_MOVE_STACK_RES);

	sGU_GUILD_BANK_MOVE_STACK_RES* pResult = (sGU_GUILD_BANK_MOVE_STACK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_MOVE_STACK_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSLEventGenerator::GuildWarehouseItemStackMove(pResult->hSrcItem, pResult->hDestItem,
		pResult->bySrcPlace, pResult->bySrcPos, pResult->byDestPlace, pResult->byDestPos,
		pResult->byStackCount1, pResult->byStackCount2);
}

void PacketHandler_GSGuild_WarehouseZenny_Res(void* pPacket)
{
	// Change Zenny in the guild warehouse
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_ZENNY_RES);

	sGU_GUILD_BANK_ZENNY_RES* pResult = (sGU_GUILD_BANK_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_ZENNY_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	if( pResult->bIsSave )
	{
		// %d Zenny saved
		GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_WAREHOUSE_SAVE_ZENNY", FALSE, NULL, Logic_FormatZeni(pResult->dwZenny));

		CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_ADD_ZENNY, pResult->dwZenny);
	}
	else
	{
		// %d Zenny was withdrawn
		GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_WAREHOUSE_ZENNY_DRAW_OUT", FALSE, NULL, Logic_FormatZeni(pResult->dwZenny));

		CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_SUB_ZENNY, pResult->dwZenny);
	}
}

void PacketHandler_GSGuild_WarehouseZennyInfo(void* pPacket)
{
	// I received Zenny information from the guild warehouse.
	sGU_GUILD_BANK_ZENNY_INFO* pResult = (sGU_GUILD_BANK_ZENNY_INFO*)pPacket;

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_SET_ZENNY, pResult->dwZenny);
}

void PacketHandler_GSGuild_WarehouseItemInfo(void* pPacket)
{
	// Received item information from the guild warehouse
	sGU_GUILD_BANK_ITEM_INFO* pResult = (sGU_GUILD_BANK_ITEM_INFO*)pPacket;

	CNtlSLEventGenerator::GuildWarehouseItemInfo(pResult->byPlace, pResult->byItemCount, (VOID*)&pResult->aItemProfile);
}

void PacketHandler_GSGuild_WarehouseEnd_Res(void* pPacket)
{
	// Guild warehouse closed
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_END_RES);

	sGU_GUILD_BANK_END_RES* pResult = (sGU_GUILD_BANK_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_END_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_END);
}

void PacketHandler_TUGuild_Reputation_Change_Nfy(void* pPacket)
{
	// The guild's reputation has changed
	sTU_GUILD_REPUTATION_CHANGE_NFY* pResult = (sTU_GUILD_REPUTATION_CHANGE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_REPUTATION, INVALID_DWORD, (VOID*)&pResult->dwReputation, (VOID*)&pResult->dwMaxGuildPointEver);
}

void PacketHandler_TUGuild_ChangeNotice_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_CHANGE_NOTICE_RES);

	// Notify the person who changed the notice that the guild notice has changed.
	sTU_GUILD_CHANGE_NOTICE_RES* pResult = (sTU_GUILD_CHANGE_NOTICE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_CHANGE_NOTICE_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_GUILD_NOTICE);

	// A guild notice has been sent
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NOTICE_SUCCESS");
}

void PacketHandler_TUGuild_changeNotice_Nfy(void* pPacket)
{
	// Notify guild members that the guild notice has changed.
	sTU_GUILD_CHANGE_NOTICE_NFY* pResult = (sTU_GUILD_CHANGE_NOTICE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_NOTICE, pResult->wNoticeLengthInUnicode, 
										pResult->awchCharName,
										pResult->awchNotice);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// There is a new guild announcement
		GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NOTICE");
	}	
}

void PacketHandler_GSGuild_Create_Emblem_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CREATE_MARK_RES);

	// A guild emblem was created.
	sGU_GUILD_CREATE_MARK_RES* pResult = (sGU_GUILD_CREATE_MARK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CREATE_MARK_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_EMBLEM_MAKER);

	// Guild emblem has been created
	GetAlarmManager()->AlarmMessage("DST_GUILD_CREATE_GUILD_EMBLEM");	
}

void PacketHandler_GSGuild_Change_Emblem_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CHANGE_MARK_RES);

	// The guild emblem was changed.
	sGU_GUILD_CHANGE_MARK_RES* pResult = (sGU_GUILD_CHANGE_MARK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CHANGE_MARK_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_EMBLEM_MAKER);

	// Guild emblem has been changed
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGE_GUILD_EMBLEM");	
}

void PacketHandler_GSGuild_Change_Emblem_Nfy(void* pPacket)
{
	sGU_GUILD_MARK_CHANGED_NFY* pResult = (sGU_GUILD_MARK_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::ChangeGuildEmblem(pResult->hSubject,
											pResult->sMark.byMarkMain, pResult->sMark.byMarkMainColor,
											pResult->sMark.byMarkInLine, pResult->sMark.byMarkInColor,
											pResult->sMark.byMarkOutLine, pResult->sMark.byMarkOutColor);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// Blocks until packets are cleared on the server side.
		/*
		if( pGuild->IsHaveEmblem() )
			// Guild emblem has been changed
			GetAlarmManager()->AlarmMessage(DST_GUILD_CHANGE_GUILD_EMBLEM);	
		else
			// Guild emblem has been created
			GetAlarmManager()->AlarmMessage(DST_GUILD_CREATE_GUILD_EMBLEM);	
			*/

		CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CHANGE_GUILD_EMBLEM);
	}
}


//////////////////////////////////////////////////////////////////////////
//								Seal //
//////////////////////////////////////////////////////////////////////////

void PacketHandler_GSDojo_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_CREATE_RES);

	Logic_CancelNpcFacing();

	// Notify the leader of the school that the dojo has been established.
	sGU_DOJO_CREATE_RES* pResult = (sGU_DOJO_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_CREATE_RES"), TRUE );
		return;
	}
}

void PacketHandler_TUDojo_CreateNfy(void* pPacket)
{
	// Announce to all members of the school that the dojo has been established.
	//sTU_DOJO_CREATED_NFY* pResult = (sTU_DOJO_CREATED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();
	GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SUCCESS_ESTABLISHMENT", FALSE, NULL, pGuild->GetGuildName() );


	// If there is a previously set dojo color, apply it.
	RwUInt8 byColorIndex = pGuild->GetDojoDogiColorIndex();
	if( INVALID_BYTE == byColorIndex )
		return;

	Logic_SetDogiItemColor(pAvatar, byColorIndex);

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( pSobGroup )
	{					
		COMMUNITY_ITER	it_Begin	= pGuild->Begin();
		COMMUNITY_ITER	it_End		= pGuild->End();

		for( ; it_Begin != it_End ; ++it_Begin )
		{
			sGuildMember* pGuildMember = reinterpret_cast<sGuildMember*>( *it_Begin );
			CNtlSobGroup::MapObject::iterator it;

			for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

				if( pSobPlayer->GetCharID() == pGuildMember->charID )
				{
					// Dobok color
					Logic_SetDogiItemColor(pSobPlayer, byColorIndex);
					break;
				}
			}
		}
	}
}

void PacketHandler_TUDojo_DestoryNfy(void* pPacket)
{
	// Notify all school members that the school has been disbanded and ownership of the school has been lost.
	sTU_DOJO_DESTROYED_NFY* pResult = (sTU_DOJO_DESTROYED_NFY*)pPacket;

	sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(pResult->dojoTblidx) );
	if(!pDOJO_TBLDAT)
	{
		DBO_FAIL("Not exist dojo table of index : " << pResult->dojoTblidx);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_LOST_DOJO");

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_LOST_DOJO, pResult->dojoTblidx);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();

	// If there is a previously set school uniform color, apply it.
	RwUInt8 byColorIndex = pGuild->GetGuildDogiColorIndex();
	if( INVALID_BYTE == byColorIndex )
		return;

	Logic_SetDogiItemColor(pAvatar, byColorIndex);

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( pSobGroup )
	{					
		COMMUNITY_ITER	it_Begin	= pGuild->Begin();
		COMMUNITY_ITER	it_End		= pGuild->End();

		for( ; it_Begin != it_End ; ++it_Begin )
		{
			sGuildMember* pGuildMember = reinterpret_cast<sGuildMember*>( *it_Begin );
			CNtlSobGroup::MapObject::iterator it;

			for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

				if( pSobPlayer->GetCharID() == pGuildMember->charID )
				{
					// Dobok color
					Logic_SetDogiItemColor(pSobPlayer, byColorIndex);
					break;
				}
			}
		}
	}
}

void PacketHandler_GSDojo_FunctionAddRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_FUNCTION_ADD_RES);

	// Notify the head of the school that the stamp function has been added.
	sGU_DOJO_FUNCTION_ADD_RES* pResult = (sGU_DOJO_FUNCTION_ADD_RES*)pPacket;

	switch( pResult->byFunction )
	{
//	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_1:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_2:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_3:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_4:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_5:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_6:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_7:
		{
			break;
		}
	default:
		{
		Logic_CancelNpcFacing();
			break;
		}
	}

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_FUNCTION_ADD_RES"), TRUE );
		return;
	}

	// Packet showing actual function change and message: PacketHandler TUGuild_FunctionChange_Nfy
}

void PacketHandler_GSDojo_LevelChangedNfy(void* pPacket)
{
	// Notifies the user that the dojo level has changed
	sGU_DOJO_LEVEL_CHANGED_NFY* pResult = (sGU_DOJO_LEVEL_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_LEVEL, pResult->dojoTblidx, (VOID*)&pResult->byLevel);

	{
		CNtlPLVisualManager* pVisualMng = (CNtlPLVisualManager*)GetSceneManager();

		if ( GAMERULE_DOJO == Logic_GetActiveWorldRuleType() )
		{
			pVisualMng->GetDojoContainer()->SetDojoWorld_Level( pResult->byLevel );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
		else
		{
			pVisualMng->GetDojoContainer()->SetDojo_Level( pResult->dojoTblidx, pResult->byLevel );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
	}
}

void PacketHandler_GSDojo_MarkChangedNfy(void* pPacket)
{
	// Notifies the user that the emblem information attached to the stamp has changed.
	sGU_DOJO_MARK_CHANGED_NFY* pResult = (sGU_DOJO_MARK_CHANGED_NFY*)pPacket;

	{
		CNtlPLVisualManager* pVisualMng = (CNtlPLVisualManager*)GetSceneManager();

		sEmblemFactor sEmFactor;

		sEmFactor.byTypeA		= pResult->sMark.byMarkMain;
		sEmFactor.byTypeB		= pResult->sMark.byMarkInLine;
		sEmFactor.byTypeC		= pResult->sMark.byMarkOutLine;

		sEmFactor.byTypeAColor	= pResult->sMark.byMarkMainColor;
		sEmFactor.byTypeBColor	= pResult->sMark.byMarkInColor;
		sEmFactor.byTypeCColor	= pResult->sMark.byMarkOutColor;

		if ( GAMERULE_DOJO == Logic_GetActiveWorldRuleType() )
		{
			pVisualMng->GetDojoContainer()->SetDojoWorld_Emblem( &sEmFactor );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
		else
		{
			pVisualMng->GetDojoContainer()->SetDojo_Emblem( pResult->dojoTblidx, &sEmFactor );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
	}
}

void PacketHandler_GSDojo_ScrambleRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_SCRAMBLE_RES);

	// Reply to the guild master who applied for a contest
	sGU_DOJO_SCRAMBLE_RES* pResult = (sGU_DOJO_SCRAMBLE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_SCRAMBLE_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_ASK_SCRAMBLE");
}

void PacketHandler_TUDojo_ScrambleNfy(void* pPacket)
{
	// Ask the guild master who received the contest request whether or not to accept the contest.
	sTU_DOJO_SCRAMBLE_NFY* pResult = (sTU_DOJO_SCRAMBLE_NFY*)pPacket;

	GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE", FALSE, NULL, pResult->wszName, DBO_DOJO_SCRAMBLE_REJECT_FARE);
}

void PacketHandler_TUDojo_ScrambleReceiveNfy(void* pPacket)
{
	// The guild master who applied for the contest is notified of the response results of the guild master who received the contest application.
	sTU_DOJO_SCRAMBLE_RECEIVE_NFY* pResult = (sTU_DOJO_SCRAMBLE_RECEIVE_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlDojo* pDojo = pAvatar->GetDojo();
	sDOJO_INFO* pDOJO_INFO = pDojo->GetDojoInfo( pResult->dojoTblidx );

	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info of dojo table index : " << pResult->dojoTblidx);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_ACCEPTED_SCRAMBLE");

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_ACCEPT, pResult->dojoTblidx, (VOID*)&pResult->guildId);
}

void PacketHandler_TUDojo_ScrambleReceiveInfoNfy(void* pPacket)
{
	// All members of the factions participating in the contest received basic information about the contest.
	sTU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY* pResult = (sTU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY*)pPacket;

	CNtlSLEventGenerator::ScrambleDefaultInfo(pResult->dojoTblidx, pResult->defGuildId, pResult->attGuildId,
											  pResult->wszDefGuildName, pResult->wszAttGuildName,
											  (VOID*)&pResult->sDefMark, (VOID*)&pResult->sAttMark);
}

void PacketHandler_GSDojo_ScrambleResponseRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_SCRAMBLE_RESPONSE_RES);

	// Result of response from the guild master who received the competition request
	sGU_DOJO_SCRAMBLE_RESPONSE_RES* pResult = (sGU_DOJO_SCRAMBLE_RESPONSE_RES*)pPacket;

	if( pResult->bIsRetry )
	{
		GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE", FALSE, NULL, pResult->wszGuildName, DBO_DOJO_SCRAMBLE_REJECT_FARE);
	}

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_SCRAMBLE_RESPONSE_RES"), TRUE );
		return;
	}
}

void PacketHandler_GSDojo_NPCInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_NPC_INFO_RES);

	// Received information from the dojo information window.
	sGU_DOJO_NPC_INFO_RES* pResult = (sGU_DOJO_NPC_INFO_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_NPC_INFO_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_NPC_GUI_INFO, pResult->sData.dojoTblidx, (VOID*)&pResult->sData);
}

void PacketHandler_GSDojo_SealCurrentStateNfy(void* pPacket)
{
	// Receive initial seal information when entering a seal competition.
	sGU_DOJO_SEAL_CURRENT_STATE_NFY* pResult = (sGU_DOJO_SEAL_CURRENT_STATE_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_STATE, pResult->sealObjectTblidx,
									(VOID*)&pResult->byState, (VOID*)&pResult->vLoc);
}
}

void PacketHandler_GSDojo_SealAttackBeginNfy(void* pPacket)
{
	// During the seal competition, casting begins to steal the seal.
	sGU_DOJO_SEAL_ATTACK_BEGIN_NFY* pResult = (sGU_DOJO_SEAL_ATTACK_BEGIN_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_BEGIN, pResult->sealObjectTblidx,
										  (VOID*)&pResult->bIsBeingStolen, (VOID*)&pResult->vLoc);
}
}

void PacketHandler_GSDojo_SealAttackEndNfy(void* pPacket)
{
	// During the seal battle, casting to steal the seal ends.
	sGU_DOJO_SEAL_ATTACK_END_NFY* pResult = (sGU_DOJO_SEAL_ATTACK_END_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_END, pResult->sealObjectTblidx);
	}
}

void PacketHandler_GSDojo_ScrambleShiftStateNfy(void* pPacket)
{
	// Notify the attack/defense school members that the status of the seal has changed during the seal competition.
	sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY* pResult = (sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY*)pPacket;

	if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
		return;


	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER, pResult->dojoTblidx, (VOID*)&pResult->objectTblidx, (VOID*)&pResult->guildId);


	CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable( Logic_GetActiveWorldId() );
	if( pObjectTable == NULL )
	{
		DBO_FAIL("Not exist obejct table of world ID : " << Logic_GetActiveWorldId());
		return;
	}

	sOBJECT_TBLDAT* pOBJECT_TBLDAT		= reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTable->FindData(pResult->objectTblidx) );
	if( pOBJECT_TBLDAT )
	{
		CTextTable* pObjectTextTable	= API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
		std::wstring wstrText			= pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
		const WCHAR* pwcText			= GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_CHANGE_SEAL_OWNER");
		if( pwcText )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_CHANGE_SEAL_OWNER", FALSE, NULL, pResult->wszName, wstrText.c_str());
		}		
	}	
}

void PacketHandler_TUDojo_BriefNfy(void* pPacket)
{
	// Received stamp information (from all users on the server)
	sTU_DOJO_BRIEF_NFY* pResult = (sTU_DOJO_BRIEF_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_DOJO_INFO, 0xffffffff, (VOID*)&pResult->sDojoData);
}

void PacketHandler_TUDojo_ScrambleRejectNfy(void* pPacket)
{
	// Notify the guild master who applied for the competition that it has been rejected.
	//sTU_DOJO_SCRAMBLE_REJECT_NFY* pResult = (sTU_DOJO_SCRAMBLE_REJECT_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage("DST_DOJO_REJECT_SCRAMBLE");
}

void PacketHandler_TUDojo_ScrambleInformNfy(void* pPacket)
{
	// To the guild master who applied for a contest and the guild master who received a contest request
	// Send information and status of the seal
	sTU_DOJO_SCRAMBLE_INFORM_NFY* pResult = (sTU_DOJO_SCRAMBLE_INFORM_NFY*)pPacket;
	CNtlSLEventGenerator::DojoState(pResult->dojoTblidx, pResult->byState, pResult->tmNextStepTime);
}

void PacketHandler_TUDojo_AddBudokaiSeedAddRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_DOJO_BUDOKAI_SEED_ADD_RES);

	// Notify the school leader of the results of adding Budokai seed participants.
	sTU_DOJO_BUDOKAI_SEED_ADD_RES* pResult = (sTU_DOJO_BUDOKAI_SEED_ADD_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_DOJO_BUDOKAI_SEED_ADD_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_TENKAI_SEED_PLAYER, pResult->dojoTblidx, (VOID*)pResult->wszCharName);
}

void PacketHandler_TUDojo_ScramblePointNfy(void* pPacket)
{
	// Notify the members of the school of the dojo competition score.
	sTU_DOJO_SCRAMBLE_POINT_NFY* pResult = (sTU_DOJO_SCRAMBLE_POINT_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_SCORE, pResult->sData.dojoTblidx, (VOID*)&pResult->sData);
}

void PacketHandler_TUDojo_NoticeChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_DOJO_NOTICE_CHANGE_RES);

	// Notify the head of the school of the results of changes to the dojo notice.
	sTU_DOJO_NOTICE_CHANGE_RES* pResult = (sTU_DOJO_NOTICE_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_DOJO_NOTICE_CHANGE_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_GUILD_NOTICE);
}

void PacketHandler_TUDojo_NoticeChangeNfy(void* pPacket)
{
	// Notify the head of the school that the dojo notice has changed.
	// If the plan changes, other people may also notice that the dojo notice has changed. No separate work required at this time
	sTU_DOJO_NOTICE_CHANGE_NFY* pResult = (sTU_DOJO_NOTICE_CHANGE_NFY*)pPacket;

	if( NTL_MAX_LENGTH_OF_GUILD_NOTICE < pResult->wNoticeLengthInUnicode )
		pResult->wNoticeLengthInUnicode = NTL_MAX_LENGTH_OF_GUILD_NOTICE;

	WCHAR acNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
	ZeroMemory(acNotice, sizeof(acNotice));
	memcpy(acNotice, pResult->awchNotice, sizeof(WCHAR)*pResult->wNoticeLengthInUnicode);
	
	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_DOJO_NOTICE, pResult->dojoTblidx,
									(VOID*)pResult->awchCharName, (VOID*)acNotice);
}

void PacketHandler_TUDojo_ScrambleResultNfy(void* pPacket)
{
	// Inform members of the school of the results of the dojo competition.
	sTU_DOJO_SCRAMBLE_RESULT_NFY* pResult = (sTU_DOJO_SCRAMBLE_RESULT_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_RESULT, pResult->winGuildId);
}

void PacketHandler_TUDojo_ScrambleRewardNfy(void* pPacket)
{
	// Announcement of rewards to members of the school who won the dojo battle
	sTU_DOJO_SCRAMBLE_REWARD_NFY* pResult = (sTU_DOJO_SCRAMBLE_REWARD_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_REWARD, pResult->dwAddGuildReputation,
									(VOID*)&pResult->itemTblidx, (VOID*)&pResult->byCount);	
}

//////////////////////////////////////////////////////////////////////////
//								Dobok //
//////////////////////////////////////////////////////////////////////////

void PacketHandler_GSDogi_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOGI_CREATE_RES);

	// Inform the head of the school that the school uniform has been made.
	sGU_DOGI_CREATE_RES* pResult = (sGU_DOGI_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOGI_CREATE_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOGI_CHANGED");
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSDogi_ChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOGI_CHANGE_RES);

	// Notify the head of the school that the school uniform has been changed.
	sGU_DOGI_CHANGE_RES* pResult = (sGU_DOGI_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOGI_CHANGE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSDogi_UpdateNfy(void* pPacket)
{
	// Notifies nearby users that there is a change in the uniform of a certain school.
	sGU_DOGI_UPDATE_NFY* pResult = (sGU_DOGI_UPDATE_NFY*)pPacket;

	if( Logic_GetAvatarHandle() == pResult->handle )
	{
		CNtlSobAvatar*	pAvatar			= GetNtlSLGlobal()->GetSobAvatar();
		CNtlGuild*		pGuild			= pAvatar->GetGuild();
		sDBO_DOGI_DATA&	rDBO_DOGI_DATA	= pResult->sData;

		if( pGuild->GetGuildID() == rDBO_DOGI_DATA.guildId )
		{
			CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI_NFY, INVALID_SERIAL_ID, (void*)&rDBO_DOGI_DATA);
		}
	}

	CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pResult->handle) );
	if( !pSobPlayer )
	{
		DBO_FAIL("Not exist sob player of handle : " << pResult->handle);
		return;
	}

	pSobPlayer->SetGuildDogiColorIndex(pResult->sData.byGuildColor);
	pSobPlayer->SetDojoDogiColorIndex(pResult->sData.byDojoColor);
	Logic_SetDogiItemColor(pSobPlayer, pResult->sData.byGuildColor);		
}

void PacketHandler_GSGuildDogi_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_DOGI_CREATE_RES);

	// Inform the head of the school that the school uniform has been made.
	sGU_GUILD_DOGI_CREATE_RES* pResult = (sGU_GUILD_DOGI_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_DOGI_CREATE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSGuildDogi_ChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_DOGI_CHANGE_RES);

	// Notify the head of the school that the school uniform has been changed.
	sGU_GUILD_DOGI_CHANGE_RES* pResult = (sGU_GUILD_DOGI_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_DOGI_CHANGE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}
