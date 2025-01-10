#pragma once

#include <stdio.h>
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "NtlSound.h"

class CNtlSound;

struct sNtlVolume;
struct sNtlPitch;
struct sNtlSoundPlayParameta;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load from memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*CallbackLoadSoundFile)(const RwChar* pFileName, void** pData, int* iSize);
extern CallbackLoadSoundFile g_fnCallback_LoadSound_from_Memory;

void LinkLoadSound_from_Memory(CallbackLoadSoundFile fn);
void UnLinkLoadSound_from_Memory();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsExistSoundFile
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef bool (*CallbackIsExistSoundFile)(const RwChar* pFileName);
extern CallbackIsExistSoundFile g_fnCallback_IsExistSoundFile;

void Link_IsExistSoundFile(CallbackIsExistSoundFile fn);
void UnLink_IsExistSoundFile();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// log
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID Logic_NtlSoundLog(const RwChar* pcContition, const RwChar* pcSoundFileName = NULL);
VOID Logic_NtlSoundLog(const RwChar* pcContition, RwInt32 iChannelGroup, const RwChar* pcSoundFileName = NULL);
VOID Logic_NtlSoundLog(const RwChar* pcContition, FMOD_RESULT result, const RwChar* pcSoundFileName = NULL);

RwBool Logic_IsExistLog(const RwChar* pcText);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound operation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Convert to a volume supported by FMOD
RwReal Logic_GetFMODValidVolume(RwReal fVolume);

// Determines the volume used in actual FMOD
RwReal Logic_CalcPlayVolume(sNtlVolume* pNtlVolume);

// Convert to pitch supported by FMOD
RwReal Logic_GetFMODValidPitch(RwReal fPitch);

// Determines the pitch used in actual FMOD
RwReal Logic_CalcPlayPitch(sNtlPitch* pNtlPitch);

RwBool Logic_IsExistFile(const RwChar* pcFileName);

SOUND_HANDLE Logic_GetNewSoundHandle();

FMOD_RESULT API_Create_Stream(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName);
FMOD_RESULT API_Create_Sound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName);