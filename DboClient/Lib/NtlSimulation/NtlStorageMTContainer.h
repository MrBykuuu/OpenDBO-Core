/*****************************************************************************
* File			: NtlStorageMTContainer.h
* Author		: HaeSung, Cho
* Copyright		: NTL Co., Ltd.
* Date			: 2009. 2. 5	
* Abstract		:  Mapping table that stores NtlStorage key and related information
*****************************************************************************
* Desc			: 
*
* If you want to modify MappingTable, modify the Define value in NtlStorageDefine.h and
* Fills in the mapping table data in the current class. You can insert at the end of a table array, but never modify/delete it.
* Don't do it. If there is a KEY that needs to be discarded, leave the defined KEY as is and do not use it.
*
* The Mapping Table is composed of a static array, and intermediate insertion/deletion must never be performed.
* ( Mapping table is composed of static array, and you should not insert/delete it from the midterm )
*****************************************************************************/

#ifndef __NTL_STORAGE_MT_CONTAINER_H__
#define __NTL_STORAGE_MT_CONTAINER_H__

#include "NtlStorageDefine.h"

#include <map>
#include <string>

#pragma once

class CNtlStorageMTContainer
{
public:
	CNtlStorageMTContainer(void);
	virtual ~CNtlStorageMTContainer(void);

	const SNtlStorageMappingTableData* GetMappingTable( unsigned int uiKey );

	bool					SortTable();
	bool					SetUnitCheckFunc( unsigned int uiKey, NtlStorageUnitCheckFunc funcApply);
	NtlStorageUnitCheckFunc		GetUnitCheckFunc( unsigned int uiKey );

	unsigned int				GetCount();
	eNTL_STORAGE_VARIABLE_TYPE	GetVariableType( unsigned int uiKey );
	eNTL_STORAGE_TYPE			GetStorageType( unsigned int uiKey );
	const char*					GetString( unsigned int uiKey );
	const char*					GetDefaultString( unsigned int uiKey );
	bool						GetDefaultBool( unsigned int uiKey );
	int							GetDefaultInt( unsigned int	uiKey );
	float						GetDefaultFloat( unsigned int uiKey );
	bool						GetKeyFromString( std::string& strKeyName, unsigned int& uiKey );

	static CNtlStorageMTContainer* GetInstance()
	{
		static CNtlStorageMTContainer instance;
		return &instance;
	}

protected:
	static SNtlStorageMappingTableData m_StorageMappingTable[];

	// Pointers are stored in the multimap for each type.
	typedef std::multimap	<unsigned char, SNtlStorageMappingTableData*> SORTMMAP;
	SORTMMAP		m_mmapCategory;

	// They are stored in the MAP in order of KEY ID.
	typedef std::map	<unsigned int, SNtlStorageMappingTableData* > TABLEMAP;
	TABLEMAP		m_mapTable;

	typedef std::map	<std::string, SNtlStorageMappingTableData* > STRINGMAP;
	STRINGMAP		m_mapString;


	bool			m_bSorted;			///< Sort the table and check if you have it
	unsigned int	m_uiCount;			///< Number of table elements
};

static CNtlStorageMTContainer* GetNtlStorageMTContainer()
{
	return CNtlStorageMTContainer::GetInstance();
}

#endif