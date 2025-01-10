#pragma once

enum eSideIconType
{
	SIDEICON_HINT,						///< Hint icon
	SIDEICON_DBC,						///< Dragon Ball collection icon
	SIDEICON_RB,						///< Rank Battle Icon
	SIDEICON_DOREPAIR,					///< Repair request icon
	SIDEICON_TUTORIAL,					///< Tutorial icon
	SIDEICON_CAUTION,					///< warning icon
	SIDEICON_BUDOKAI,					///< World¡¯s First Martial Arts Association Icon
	SIDEICON_PETITION,					///< Petition icon
	SIDEICON_GM,						///< GM icon
	SIDEICON_DOJO,						///< Dojo side icon
    SIDEICON_NETPY,                     ///< NetPy side icon
	SIDEICON_HLS,

	SIDEICON_NUMS,
	INVALID_SIDEICON
};

enum eSideViewType
{
	SIDEVIEW_HINT_HELP,					///< Help hint view
	SIDEVIEW_HINT_GUIDE,				///< Guide hint view
	SIDEVIEW_HINT_STRING,				///< String notification view
	SIDEVIEW_DBC,						///< Dragon Ball collection notification view
	SIDEVIEW_RB,						///< Rank Battle View
	SIDEVIEW_DOREPAIR,					///< Repair request view
	SIDEVIEW_TUTORIAL,					///< Tutorial view
	SIDEVIEW_CAUTION,					///< Alert view
	SIDEVIEW_BUDOKAI,					///< View of the World¡¯s First Martial Arts Club
	SIDEVIEW_PETITION,					///< Petition view
	SIDEVIEW_GM,						///< GM View
	SIDEVIEW_DOJO,						///< Dojo view
    SIDEVIEW_NETPY,                     ///< NetPy view
	SIDEVIEW_HLS,

	SIDEVIEW_NUMS,
	INVALID_SIDEVIEW
};

// Side icon status of Dragon Ball Collection
enum eDBCSideIconType
{
	E_ICON_NO_SEASON,					///< When it is not the Dragon Ball collection period
	E_ICON_SEASON,						///< During the Dragon Ball collection period
	E_ICON_COLLECT,						///< When dragon ball collection is completed
};

enum eNetPySideViewType
{
    E_NETPY_INFO,                       ///< Show current NetPy status
    E_NETPY_NOTIFY,                     ///< Show NetPy acquisition notification
};


// Side icon priority ////////////////////////////////////////////////////////////////////////
// The lower the priority, the output is on the right.
#define dSIDEICON_PRIORITY_DEFAULT		50
#define dSIDEICON_PRIORITY_CAUTION		25
#define dSIDEICON_PRIORITY_TUTORIAL		0