#pragma once

#include "NtlXMLDoc.h"

// speech bubble structure
struct STeleCastBalloon
{
    RwV2d v2dOffset;
    RwV2d v2dSize;
    std::string strFileName;

    STeleCastBalloon()
    {
        ZeroMemory(&v2dOffset, sizeof(RwV2d));
        ZeroMemory(&v2dSize, sizeof(RwV2d));
    }
};

// Set which type of broadcast window to call
enum TELECAST_TYPE
{
    TELECAST_TYPE_TMQ,                  
    TELECAST_TYPE_TUTORIAL,
    TELECAST_TYPE_SERVER_NOTIFY,
};

/// Telecast-related UIConfig structure
struct SUIConfigTeleCast
{
    TELECAST_TYPE           m_eTelecastType; ///< Previously set broadcast window type
    RwInt32     nYPos;                      ///< Y position
    RwV2d       v2dSize;                    ///< size
    RwReal      fFadeTime;                  ///< Time taken to produce fade
    std::string strFadeFlashFile;           ///< Fade production flash file name
    std::string strOpeningFlashFile;        ///< Open production flash file name
    std::string strClosingFlashFile;        ///< Close Directed flash file name
    std::string strBackNormalFlashFile;     ///< Normal state background flash file name
    std::string strBackWarnFlashFile;       ///< Warning/critical status background flash file name
    RwReal      fShakeFreq;                 ///< Vibrations per second when performing Shake
    RwInt32     nShakeAmp;                  ///< Amplitude when producing Shake (pixel unit)    
    std::map<RwInt32, STeleCastBalloon>  mapBalloonRes; ///< Speech balloon resource file name map
    RwReal      fSlideStartVel;             ///< Slide start speed
    RwReal      fSlideAccel;                ///< Slide acceleration
    
    RwUInt32    uiNPCID;                    ///< NPC table ID (only used in server notification window)
    RwReal      fShowTime;                  ///< Time to be displayed (only used in server notification window)
    
    SUIConfigTeleCast() 
    : fShakeFreq(0), nShakeAmp(0), nYPos(578), fFadeTime(5.0f), m_eTelecastType(TELECAST_TYPE_TMQ)
    {        
        v2dSize.x = 235.0f;
        v2dSize.y = 176.0f;
        fSlideStartVel = 50.0f;
        fSlideAccel = 20.0f;        
    }
};

#define dBROADCAST_SHAKE_TYPE_NUMS	3

/**
* \brief BroadCast UIConfig
*/
struct SUIConfigBroadCast
{
	RwReal		fPosXRate;									///< X Pos Rate
	RwReal		fPosYRate;									///< Y Pos Rate
	RwReal		fSlideStartVel;								///< Sliding: Startup speed
	RwReal		fSlideAccelVel;								///< Sliding: Acceleration
	RwReal		fBlendStartVel;								///< UI Blending: Startup speed
	RwReal		fBlendAccelVel;								///< UI Blending: Acceleration
	RwReal		fBlendTextStartVel;							///< Blend text Text: Startup speed
	RwReal		fBlendTextAccelVel;							///< Blend text: Acceleration
	RwReal		fIntervalTextTime;							///< Line: Update time
	RwReal		fIntervalPresNum;							///< Line: Number of characters to appear at once
	RwReal		fFadeSlidePos;								///< Fade-Slide: Start OffsetX
	RwReal		fFadeSlideStartVel;							///< Fade-Slide: Start speed
	RwReal		fFadeSlideAccelVel;							///< Fade-Slide: Acceleration
	RwReal		fShakeXAmp[dBROADCAST_SHAKE_TYPE_NUMS];		///< ShakeX: Shake width
	RwReal		fShakeXFreq[dBROADCAST_SHAKE_TYPE_NUMS];	///< ShakeX: Shake speed
	RwReal		fShakeYAmp[dBROADCAST_SHAKE_TYPE_NUMS];		///< ShakeY: Shake width
	RwReal		fShakeYFreq[dBROADCAST_SHAKE_TYPE_NUMS];	///< ShakeY: shaking speed
};

// Notify-related structures
struct SUIConfigNotifyPosition
{
	SUIConfigNotifyPosition(VOID)
	{
		fActionNotify_YPosRate = 0.215f;
		fPublicNotify_YPosRate = 0.123f;
		fTerritoryNotify_YPosRate = 0.345f;
		fCautionNotiry_YPosRate = 0.293f;
		fGroupNotiry_YPosRate = .7f;
		fNameNotiry_YPosRate = .6f;
		fBroadcastNotiry_YPosRate = 0.345f;
	}

	RwReal		fActionNotify_YPosRate;						///< YPos /768; 
	RwReal		fPublicNotify_YPosRate;				
	RwReal		fTerritoryNotify_YPosRate;
	RwReal		fCautionNotiry_YPosRate;
	RwReal		fGroupNotiry_YPosRate;
	RwReal		fNameNotiry_YPosRate;
	RwReal		fBroadcastNotiry_YPosRate;
};

/// /Singleton class that reads and stores UI settings from script/UIConfig.XML
class CDBOUIConfig : CNtlXMLDoc
{
public:
    CDBOUIConfig(void);
    ~CDBOUIConfig(void);

    RwBool Load();                              ///< Read Config information from XML file.
    RwBool Reflash();                           ///< Reloads the information from the XML file.

    RwBool LoadTeleCast(TELECAST_TYPE eType);   ///< Loads information for TeleCast.    
	RwBool LoadBroadCast();						///< Loading information for BroadCast
	RwBool LoadNotify();					

    SUIConfigTeleCast*  GetTeleCastConfig()		{	return &m_TeleCast;		} ///< Returns Config information for TeleCast.
	SUIConfigBroadCast*	GetBroadCastConfig()	{	return &m_BroadCast;	}
	SUIConfigNotifyPosition* GetNotifyConfig()	{	return &m_Notify; }

protected:    
    RwBool LoadTest();                          ///< Load data for testing.

protected:    
    char   szBuf[64];                           ///< used buffer

    SUIConfigTeleCast       m_TeleCast;         ///< Settings object for Telecast
	SUIConfigBroadCast		m_BroadCast;		///< BroadCast
	SUIConfigNotifyPosition	m_Notify;			///< Notify
    
    RwReal                  m_fCameraTargetRatio; ///< Target height ratio of Follow Camera (ratio to character's height) -for testing purposes
  
};
