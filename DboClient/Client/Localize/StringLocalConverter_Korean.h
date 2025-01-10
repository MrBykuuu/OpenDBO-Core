/******************************************************************************
* File			: StringLocalConverter_Korean.h
* Author		: Hong SungBock
* Copyright		: (��)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc : Korean has five articles that change depending on the ending of the preceding letter.
* ��
* Example ) ��/��, ��/��, ��/��, ��/��,
* If there is no endings or if the endings are '��', it is changed to ��/��
*****************************************************************************/

#pragma once

#include "StringLocalConverter_LocalBase.h"

class CStringLocalConverter_Korean : public CStringLocalConverter_LocalBase
{
public:
	virtual void	LocalStringCovert(WCHAR* pwcOutput, const WCHAR* pwcText);

protected:
	///< Returns the ending order of Unicode Korean syllables
	RwInt32			GetKoreanThirdSound(const WCHAR* pwcText);
};