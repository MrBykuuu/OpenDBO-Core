#pragma once

enum eSoundResultType
{	
	SOUNDRESULT_OK,								// Successfully created sound/or stored it in List.

	SOUNDRESULT_RECOVERY_FROM_FADE_OUT,			///< The sound that fades out fades in and continues to be played.

	SOUNDRESULT_NOT_CREATED_SOUND,				///< FMOD::Sound creation failed

	SOUNDRESULT_FAIL_PLAY,						///< The playSound function failed in FMOD.

	SOUNDRESULT_INVALID_CHANNELGROUP,			///< Type not found in eChannelGroupType

	SOUNDRESULT_INVALID_HANDLE_PTR,				///< Invalid handle value.

	SOUNDRESULT_OUT_OF_RANGE,					///< Exited playable area.

	SOUNDRESULT_FULL_DUPLICATION,				///< The number of sounds with the same name that can be played simultaneously has been exceeded. (Same sound can be played)

	SOUNDRESULT_ALREADY_PLAY_SAME_SOUND,		///< A sound with the same name is already playing. (Same sound cannot be played)	

	SOUNDRESULT_EMPTY_FILENAME,					///< The name of the file you want to play is missing.

	SOUNDRESULT_MORE_WAIT_EFFECT_SOUND,			///< The effect file with the same name has not been played for enough time yet

	SOUNDRESULT_MAX_EFFECT_CHANNELS,			///< All specified effect channels are being used.

	SOUNDRESULT_NOT_EXIST_FILE,					///< Sound file does not exist

	SOUNDRESULT_PROHIBITION_STATE,				///< It is not a mute function, so it is not playing at all.

	SOUNDRESULT_DISABLE_FLASH_MUSIC,			///< Flash music group has decided not to use it for now.

	SOUNDRESULT_FAILT_STORE_SOUND,				///<Failed to save sound content

	SOUNDRESULT_NUM
};