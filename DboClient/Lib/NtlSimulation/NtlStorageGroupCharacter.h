/*****************************************************************************
* File			: NtlStorageGroupCharacter.h
* Author		: HaeSung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 8. 16	
* Abstract		: Options that can be grouped by character
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_CHARACTER_H__
#define __NTL_STORAGE_GROUP_CHARACTER_H__

#pragma once

#include "NtlStorageGroup.h"

class CNtlStorageGroupCharacter : public CNtlStorageGroup
{
public:
	CNtlStorageGroupCharacter(void);
	virtual ~CNtlStorageGroupCharacter(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_CHARACTER_H__