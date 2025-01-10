/*****************************************************************************
* File			: NtlStorageGroupQuest.h
* Author		: HaeSung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 2. 5	
* Abstract		: Quest Group, which stores player quest information
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_QUEST_H__
#define __NTL_STORAGE_GROUP_QUEST_H__

typedef VOID (*UserQuestSaveFunc)();

#pragma once

#include "NtlStorageDefine.h"
#include "NtlStorageGroup.h"

class CNtlStorageGroupQuest : public CNtlStorageGroup
{
public:
	CNtlStorageGroupQuest(void);
	~CNtlStorageGroupQuest(void);

	virtual void Clear(void);

	virtual bool Load(CNtlStorageSerializer* pS);
	virtual bool Save(CNtlStorageSerializer* pS);

	void					IndicateQuest(RwUInt32 uiID, RwBool bIndicate);
	void					NewQuest(RwUInt32 uiID, RwBool bNew);
	SMemoryQuestData*		GetQuestData(RwUInt32 uiID);

	void					LoadQuestInfomation(CNtlSerializer* pS);

protected:
	typedef std::map<RwUInt32, SMemoryQuestData>	MapMemoryQuest;
	MapMemoryQuest			m_mapQuest;
};

#endif//__NTL_STORAGE_GROUP_QUEST_H__