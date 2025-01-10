/******************************************************************************
* File			: StringLocalConverter.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc: Change the word or letter according to the situation of each language or country.
* Example) Change Korean's 을/를, 이/가, etc...
* *
* Japanese is not applicable.
*****************************************************************************/

#pragma once

class CStringLocalConverter_LocalBase;

class CStringLocalConverter
{
public:
	#define dSTRING_CONVERTER_BUFFER_SIZE		1024

	static CStringLocalConverter* GetInstance();
	static RwBool	CreateInstance();
	static void		DestroyInstance();

	enum eLocalType
	{
		LOCAL_TYPE_KOREA,

		INVALID_LOCAL_TYPE
	};

	typedef std::map<RwUInt8, CStringLocalConverter_LocalBase*>		MAP_LOCAL_CONVERTER;

public:
	CStringLocalConverter() {}
	virtual ~CStringLocalConverter() {}

	///< Returns a language-specific context-modified string
	const WCHAR*	LocalStringCovert(const WCHAR* pwcText);

protected:
	static CStringLocalConverter* m_pInstance;

	WCHAR						m_awcBuffer[dSTRING_CONVERTER_BUFFER_SIZE];
	MAP_LOCAL_CONVERTER			m_mapLocalConverter;
};

static CStringLocalConverter* GetStringConverter()
{
	return CStringLocalConverter::GetInstance();
}