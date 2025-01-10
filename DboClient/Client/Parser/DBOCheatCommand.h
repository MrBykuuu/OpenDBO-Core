/*****************************************************************************
 *
 *File			: DBOCheatCommand
 *Author	    : agebreak
 *Copyright	    : NTL Co., Ltd.
 *Date			: 2007. 9. 13	
 *Abstract		: DBO Cheat Command
 *****************************************************************************
 *Desc         : Parses and processes client-side cheat commands used in DBO.
 *
 *****************************************************************************/

#pragma once

#include "NtlTokenizer.h"
#include "NtlSoundManager.h"


class CDBOCheatCommand
{
public:
    typedef RwBool (CDBOCheatCommand::*CheatHandler)(CNtlTokenizer* pLexer);
    typedef std::map<std::string, CheatHandler> MapCheatHandler;

public:
    CDBOCheatCommand();
    ~CDBOCheatCommand();

    RwBool  OnCmdParsing(const std::string& strCmd);            ///< Parse the cheat key

protected:
    void		Register();                 ///< Register cheat strings and functions.

	RwBool		OnElapsedTime(CNtlTokenizer* pLexer);
    RwBool		OnDump(CNtlTokenizer* pLexer);
    RwBool		OnDumpToConsole(CNtlTokenizer* pLexer);
    RwBool		OnDumpToGui(CNtlTokenizer* pLexer);
    RwBool		OnDumpToFile(CNtlTokenizer* pLexer);
	RwBool      OnDump_Action(CNtlTokenizer* pLexer);
	RwBool      OnDump_Sob(CNtlTokenizer* pLexer);
	RwBool      OnDump_FSM(CNtlTokenizer* pLexer);
	RwBool      OnDump_Contents(CNtlTokenizer* pLexer);

    RwBool		OnCompileLua(CNtlTokenizer* pLexer);
	RwBool		OnRunLua(CNtlTokenizer* pLexer);
    RwBool		OnCAngle(CNtlTokenizer* pLexer);
    RwBool		OnSAngle(CNtlTokenizer* pLexer);
	RwBool		OnSpeech(CNtlTokenizer* pLexer);
	RwBool		OnResetGui(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixDelayTime(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixTime(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixValue(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixCameraLifeTime(CNtlTokenizer* pLexer);
	RwBool		OnPacketLockMsg(CNtlTokenizer* pLexer);
	RwBool		OnServerMsg(CNtlTokenizer* pLexer);
	RwBool		OnShadow(CNtlTokenizer* pLexer);
	RwBool		OnCharLoadTime(CNtlTokenizer* pLexer);			///< Cheat key for character loading test
    RwBool		OnTest(CNtlTokenizer* pLexer);                    ///< Cheat key for testing
    RwBool		OnTest2(CNtlTokenizer* pLexer);                    ///< Cheat key for testing
    RwBool		OnTest3(CNtlTokenizer* pLexer);                    ///< Cheat key for testing
	RwBool		OnTestInfoScrollTime(CNtlTokenizer* pLexer);		///< Infobox scroll speed factor
	RwBool		OnTutorialScript(CNtlTokenizer* pLexer);	
	RwBool		OnFlashNotify(CNtlTokenizer* pLexer);
	RwBool		OnAvatarShowOnOff(CNtlTokenizer* pLexer);
	RwBool		OnServerChangeOut(CNtlTokenizer* pLexer);
	RwBool		OnLowSpecCharacter(CNtlTokenizer* pLexer);
	RwBool		OnSkipEdge(CNtlTokenizer* pLexer);
	RwBool		OnSkipTerrain(CNtlTokenizer* pLexer);
	RwBool		OnObjectDistance(CNtlTokenizer* pLexer);
	RwBool		OnTerrainDistance(CNtlTokenizer* pLexer);
    RwBool      OnLowSpecEffect(CNtlTokenizer* pLexer);         ///< Apply effect low specs
	RwBool      OnLowSpecEffectRatio(CNtlTokenizer* pLexer);    ///< Apply effect low specs
    RwBool      OnRenderMesh(CNtlTokenizer* pLexer);            ///< presence of mesh effect rendering
    RwBool      OnRenderDecal(CNtlTokenizer* pLexer);           ///< Presence of decal effect rendering	

	RwBool      OnRenderWaterSpecular(CNtlTokenizer* pLexer);           ///< Presence of water specular rendering
	RwBool		OnRenderTerrainShadow(CNtlTokenizer* pLexer);           ///< Availability of terrain shadow rendering

	RwBool		OnWeatherLevel(CNtlTokenizer* pLexer);           ///< Weather level settings.

	RwBool		OnBroadCastPosX(CNtlTokenizer* pLexer);
	RwBool		OnMiniNarration(CNtlTokenizer* pLexer);

	RwBool		OnAnimSyncActive(CNtlTokenizer* pLexer);

	RwBool		OnTSReload(CNtlTokenizer* pLexer);

	RwBool		OnDeveloperPreview(CNtlTokenizer* pLexer);

	RwBool		OnDojo(CNtlTokenizer* pLexer);

    RwBool      OnRunAnimSpeedChange(CNtlTokenizer* pLexer);

	RwBool      OnCreatePC(CNtlTokenizer* pLexer);

	RwBool		LoadPalette(CNtlTokenizer* pLexer);

	RwBool		ReloadTable(CNtlTokenizer* pLexer);

//	RwBool      OnTestCull(CNtlTokenizer* pLexer);

	RwBool      OnCrash(CNtlTokenizer* pLexer);

	RwBool      OnPerf(CNtlTokenizer* pLexer);

	RwBool      OnDialog(CNtlTokenizer* pLexer);				///< dialog related /dialog all open(close)
	RwBool      OnPacketTrace(CNtlTokenizer* pLexer);			/// woosungs_test 20090804

	// Sound
	RwBool		OnPlayManySound(CNtlTokenizer* pLexer);
	RwBool		OnStopManySound(CNtlTokenizer* pLexer);

	RwBool		OnMinMaxRate(CNtlTokenizer* pLexer);
	RwBool		OnPlaySound(CNtlTokenizer* pLexer);
	RwBool		OnStopSound(CNtlTokenizer* pLexer);

	RwBool		OnShareBGMRate(CNtlTokenizer* pLexer);

	RwBool		OnDestMove(CNtlTokenizer* pLexer);

protected:
    MapCheatHandler m_mapCheatHandler;  ///< Cheat string and processing function map    

	std::list<SOUND_HANDLE>		m_listPlayList;
	SOUND_HANDLE	m_hSound;

	RwInt32			m_iCurrOpenedDialogIndex;
};

