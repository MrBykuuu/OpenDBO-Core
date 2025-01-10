#pragma once

// Be careful not to overlap with the dialog index.
enum eSideDialogType
{
	SDIALOG_FIRST = 500,
	

	// Basically, a dialog that should always be shown if the situation is right.
	SDIALOG_DEFAULT_FIRST = SDIALOG_FIRST,

	SDIALOG_RANKBATTLE_INFO = SDIALOG_DEFAULT_FIRST,	///< Rank Battle Information Window
	SDIALOG_BUDOKAI_MINORMATCH_INFO,			///< World's Best Martial Arts Preliminary Information Window
	SDIALOG_BUDOKAI_MAJORMATCH_INFO,			///< The World's Best Martial Arts Tournament final information window
	SDIALOG_BUDOKAI_FINALMATCH_INFO,			///< The World's Best Martial Arts Tournament final information window
	SDIALOG_QUEST_INDICATOR,					///< Basic quest indicator
	SDIALOG_TMQ_STATUS,							///< TMQ status progress window
	SDIALOG_SCRAMBLE_STATUS,					///< Dojo battle progress window
	SDIALOG_SCS_AUTOCHECK,						///< AutoCheck progress window
	SDIALOG_CCBD_STATUS,

	SDIALOG_DEFAULT_LAST = SDIALOG_CCBD_STATUS,
	

	// Event Dialog
	SDIALOG_EVENT_FIRST,

    SDIALOG_SCOUTER_PC_VIEWER = SDIALOG_EVENT_FIRST, ///< View Scouter PC equipment
	
	SDIALOG_EVENT_LAST = SDIALOG_SCOUTER_PC_VIEWER,    


	// Dialog drawn on the screen by user operation
	SDIALOG_NORMAL_FIRST = SDIALOG_EVENT_LAST,	///< Until the dialog is defined here
	//SDIALOG_NORMAL_FIRST;

	SDIALOG_NORMAL_LAST = SDIALOG_NORMAL_FIRST,


	SDIALOG_LAST = SDIALOG_NORMAL_LAST,

	SDIALOG_UNKNOWN								///< Invalid dialog index
};

// side dialog attribute
#define dSDA_NONE							0x00000000
#define dSDA_INDEPENDENTLY					0x00000001	///< Does not affect other side dialogs and positions
#define dSDA_FREE_Y_POSITION				0x00000002	///< Adjust the Y coordinate to the height of the original dialog rather than using the side dialog.

// side dialog default font color
#define SIDE_DIALOG_FONT_GREEN_COLOR_RED	131
#define SIDE_DIALOG_FONT_GREEN_COLOR_GREEN	255
#define SIDE_DIALOG_FONT_GREEN_COLOR_BLUE	143
#define SIDE_DIALOG_FONT_GREEN_COLOR		RGB(SIDE_DIALOG_FONT_GREEN_COLOR_RED,SIDE_DIALOG_FONT_GREEN_COLOR_GREEN,SIDE_DIALOG_FONT_GREEN_COLOR_BLUE)

#define SIDE_DIALOG_FONT_YELLOW_COLOR_RED	253
#define SIDE_DIALOG_FONT_YELLOW_COLOR_GREEN	243
#define SIDE_DIALOG_FONT_YELLOW_COLOR_BLUE	107
#define SIDE_DIALOG_FONT_YELLOW_COLOR		RGB(SIDE_DIALOG_FONT_YELLOW_COLOR_RED,SIDE_DIALOG_FONT_YELLOW_COLOR_GREEN,SIDE_DIALOG_FONT_YELLOW_COLOR_BLUE)