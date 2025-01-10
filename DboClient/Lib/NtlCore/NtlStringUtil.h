//***********************************************************************************
//	File		:	NtlStringUtil.h
//	Desc		:	
//	Begin		:	2007. 4. 4
//	Author		:	avooo
//	Update		:	
//***********************************************************************************

#pragma once


// Returns the number of char type bytes in the input string.
// In the wchar_t type, all characters are 2 bytes, but the char type for each language varies from 1 to 3 bytes.
// Created to determine how many bytes a string is in order to match the length (bytes) of the string stored on the server.

// If the argument is received in char type, some characters may not be recognized properly.
// Example) ыс: It is actually 2 bytes, but it is not recognized, so it is converted to ? and recognized as 1 byte.
RwUInt32 GetByte(const wchar_t* pwcText);


//Find out how many digits the entered number is
RwInt32 GetNumberLength(RwInt32 iNumber);
RwInt32 GetNumberLength(RwUInt32 uiNumber);


// Creates and returns a string according to the input data type.
// Example) MakeStr("KOSPI rose by %f%s and closed at %f points", 23.3, "points", 2003.23);
char* MakeStr(char* pcText, ...);

void		StringTokenize(std::vector<std::string>& strOutToken, const std::string& strText, const std::string& strToken);
std::string StringReplace(const std::string& strText, const std::string& strSrc, const std::string& strReplace);