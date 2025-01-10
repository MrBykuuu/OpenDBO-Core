/****************************************************************************
*File           : DboScriptLinkDef.h
*Author         : HaeSung, Cho
*Copyright      : NTL Co., Ltd.
*Date           : May 26, 2008	
*Abstract		:  IDs defined in DBO's script link table
*****************************************************************************
*Desc: 
*
*****************************************************************************/

#ifndef __DBO_SCRIPT_LINK_DEF_H___
#define __DBO_SCRIPT_LINK_DEF_H___

// Type of linked content
enum eScriptLinkType
{
	eLINKTYPE_TENKAICHIBUDOKAI = 0,
	eLINKTYPE_TIMEMACHINEQUEST,
	eLINKTYPE_RANKBATTLE,

	eLINKTYPE_NUMS,
	eLINKTYPE_INVALID = 0xFF
};

// Action number from World's First Martial Arts Club
enum eTenKaichiBudokaiAction
{
	eBUDOKAI_NEWSMAIN = 0,					///< Opens the main newsletter, NEWS.
	eBUDOKAI_REQUEST,						///< View the application UI.
	eBUDOKAI_TOURNAMENT,					///< View tournament bracket
	eBUDOKAI_HTML,							///< Open public HTML
	eBUDOKAI_CLOSE,							///< Close interface

	eBUDOKAI_NUMS,
	eBUDOKAI_INVALID = 0xFF
};

#endif
