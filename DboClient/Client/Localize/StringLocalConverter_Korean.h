/******************************************************************************
* File			: StringLocalConverter_Korean.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc : Korean has five articles that change depending on the ending of the preceding letter.
* λ
* Example ) 을/를, 이/가, 은/은, 와/과,
* If there is no endings or if the endings are 'ㄹ', it is changed to 로/로
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