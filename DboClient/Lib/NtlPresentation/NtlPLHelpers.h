/*****************************************************************************
*
* File			: NtlPLHelpers.h
* Author		: All Author
* Copyright	    : NTL Co., Ltd.
* Date			: 2005. 8. 11	
* Abstract		: Presentation layer helper function.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLHELPER_H__
#define __NTL_PLHELPER_H__
/*
#include <windows.h>
#include <rwcore.h>
#include <rpworld.h>
*/

#include <tchar.h>
#include "NtlPLDef.h"
#include "NtlPLEmblemMaker.h"
#include "NtlMath.h"


class CNtlPLEntity;
class CNtlPLPlayerName;


ENtlPLEntityType Helper_PLEntityTypeToName(const RwChar* pName);

const RwChar* Helper_PLEntityNameToType(const ENtlPLEntityType eType);

/// Extracts only the file name from relative and absolute paths.
RwChar* Helper_AbstractFileName(RwChar *pPathName);

/// Parse the string. (by agebreak 2006.06.26)
void Helper_StringSplit(const RwChar* szInput, RwChar* szOutput, size_t sizeOutPut, const RwChar* szSplit, RwInt32 nCount);

//////////////////////////////////////////////////////////////////////////
// The contents of the basic structures are L"x;y;z;" Convert it to a string of forms.
//////////////////////////////////////////////////////////////////////////

/// Change the contents of the RwV3d structure to a string in the form L"x;y;z". (by agebreak 2006.08.16)
#define  Helper_V3D2StringU(buf, vPos)   swprintf_s(buf, L"%.2f;%.2f;%.2f", vPos.x, vPos.y, vPos.z);
RwV3d Helper_String2V3D(const RwChar* buf); // Converts a string in the form "x;y;z;" to RwV3d form and returns it.

// Change the contents of the RwV2d structure to a string in the form L"x;y". (by agebreak 2006.08.16)
#define  Helper_V2D2StringU(buf, vPos)   swprintf_s(buf, L"%.2f;%.2f", vPos.x, vPos.y);

// Converts a string in "x;y" format to RwV2d format and returns it.
RwV2d Helper_String2V2D(const RwChar* buf);
RwV2d Helper_String2V2D(const WCHAR* buf);

// "r;g;b;a;" Converts the string in the form RwRGBA and returns it.
RwRGBA Helper_String2RGBA(const RwChar* buf);
#define Helper_RGBA2String(buf, rgba) swprintf_s(buf, L"%d;%d;%d;%d", rgba.red, rgba.green, rgba.blue, rgba.alpha);

// Converts RGB information to Gray Color.
RwUInt8 Helper_RGB2Gray(RwUInt8& _R, RwUInt8& _G, RwUInt8& _B);

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a presentation entity.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CNtlPLPlayerName* Helper_CreatePlayerName(const RwV3d *pPos, COLORREF Color, const WCHAR *pName);
CNtlPLPlayerName* Helper_CreatePlayerName(const RwV3d *pPos, COLORREF Color, const WCHAR *pName,
										  COLORREF ColorGuild = RGB(255, 255, 255), const WCHAR *pGuildName = NULL,
										  sEmblemFactor *pEmblemFactor = NULL,
										  COLORREF ColorTitle = RGB(255, 255, 255), const WCHAR *pTitleName = NULL);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attach function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Helper_AttachWorldPos(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, RwV3d vOffset = ZeroAxis, RwBool bApplyRotate = FALSE);
void Helper_AttachWorldPosHeight(CNtlPLEntity* pSource, CNtlPLEntity* pTarget);           ///< When attaching to a world location, the World Height value of that location is applied.
void Helper_AttachBone(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, const RwChar *pBoneName, RwV3d vOffset = ZeroAxis);
void Helper_DetachPLEntity(CNtlPLEntity *pSource, CNtlPLEntity *pTarget);

//////////////////////////////////////////////////////////////////////////
// Bit flag setting related
//////////////////////////////////////////////////////////////////////////

__inline void Helper_SetBigFlag(BITFLAG* bitFlag, int index, BOOL bBool)
{
    if(index < 0 || index > 31)
        return;

    if(bBool)
    {
        *bitFlag |= 1 << index;
    }
    else
    {
        *bitFlag &= ~(1 << index);
    }    
}

__inline BOOL Helper_GetBitFlag(BITFLAG bitFlag, int index)
{
    if(index < 0 || index > 31)
        return FALSE;

    return bitFlag &= 1 << index;
}


//////////////////////////////////////////////////////////////////////////
// Callback function
//////////////////////////////////////////////////////////////////////////

typedef RwUInt8 (*Callback_GetDogiColorIndex)(RwUInt32 uiSerialID);
extern Callback_GetDogiColorIndex g_fnGetDogiColorIndex;

void LinkCallback_GetDogiColorIndex(Callback_GetDogiColorIndex fn);
RwUInt8 Logic_GetDogiColorIndex(RwUInt32 uiSerialID);


#endif