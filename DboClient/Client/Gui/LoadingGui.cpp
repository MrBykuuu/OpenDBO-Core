#include "precomp_dboclient.h"
#include "LoadingGui.h"

// Core
#include "NtlDebug.h"

// Shared
#include "Table.h"
#include "PCTable.h"
#include "TextAllTable.h"
#include "WorldTable.h"
#include "TimeQuestTable.h"
#include "TableContainer.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"

// Client
#include "DboGlobal.h"
#include "StageManager.h"
#include "GameStage.h"
#include "DialogPriority.h"
#include "LobbyManager.h"


#define dGAME_TIP_PAGE_RATE				(0.7f)


CLoadingGui::CLoadingGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_eLoadingPageType(INVALID_LOADING_PAGE)
,m_idxStart(INVALID_TBLIDX)
,m_idxEnd(INVALID_TBLIDX)
{

}

CLoadingGui::~CLoadingGui()
{

}

RwBool CLoadingGui::Create()
{
	NTL_FUNCTION( "CLoadingGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\Loading.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_FLASH_PROLOG);


	CRectangle	rect;

	m_pFlash = NTL_NEW gui::CFlash( m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	m_pFlash->SetResizeType( gui::CFlash::EXACTFIT );
	m_pFlash->AutoRewind( TRUE );
	m_pFlash->PlayMovie( TRUE );
	m_slotMovieEnd = m_pFlash->SigMovieEnd().Connect( this, &CLoadingGui::OnMovieEnd );

	rect.SetRectWH(0, 0, 200, 15);
	m_pTipTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pTipTitle->CreateFontStd( DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pTipTitle->SetTextColor( RGB(255, 190, 77) );
	m_pTipTitle->SetText( GetDisplayStringManager()->GetString("DST_TODAT_TIP") );
	m_pTipTitle->Enable(false);

	rect.SetRectWH(0, 0, 450, 60);
	m_pTipMessage = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pTipMessage->CreateFontStd( DEFAULT_FONT, 100, DEFAULT_FONT_ATTR);
	m_pTipMessage->Clear();
	m_pTipMessage->Enable(false);

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CLoadingGui::OnMouseDown );


	LinkMsg(g_EventResize);

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CLoadingGui::Destroy()
{
	NTL_FUNCTION("CLoadingGui::Destroy");

	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CLoadingGui::Update(RwReal fElapsed)
{
	m_pFlash->Update(fElapsed);
}

VOID CLoadingGui::Load_LoadingPage(eLOADING_PAGE_TYPE eType)
{
	sWORLD_INFO* pWORLD_INFO = Logic_GetActiveWorldInfo();

	m_eLoadingPageType = eType;

	switch(m_eLoadingPageType)
	{
	case LOADING_PAGE_FIRST_STEP:
		{
			m_pTipTitle		->Show(true);
			m_pTipMessage	->Show(true);

			if( GAMERULE_NORMAL == pWORLD_INFO->sRuleInfo.byRuleType )
			{
				RwReal fRate = NtlRandomNumber(0.f, 1.0f);

				if( dGAME_TIP_PAGE_RATE > fRate )
				{
					if( LoadPage_GameTip() )
					{
						SetGameTipText();
						LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
						return;
					}
				}

				CNtlStage* pStage = CStageManager::GetInstance()->GetActiveStage();
				if( pStage )
				{
					std::string strStageName = pStage->GetName();

					if( strStageName == GAME_STAGE_NAME )
					{
						CGameStage* pGameStage = reinterpret_cast<CGameStage*>( pStage );

						if( pGameStage->IsActiveLoadingScene() )
						{
							if( LoadPage_Region() )
							{
								SetGameTipText();
								LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
								return;
							}
						}
					}
				}
			}
			else
			{
				if( LoadPage_Region() )
				{
					SetGameTipText();
					LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
					return;
				}
			}

			if( LoadPage_Job() )
			{
				SetGameTipText();
				LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			}

			break;
		}
	case LOADING_PAGE_SECOND_STEP:
		{
			m_pTipTitle		->Show(false);
			m_pTipMessage	->Show(false);

			if( GAMERULE_NORMAL == pWORLD_INFO->sRuleInfo.byRuleType )
			{
				if( LoadFlash("DBO_Tutorial_01.swf", m_pFlash) )
				{
					LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
					return;
				}
			}
			else if( GAMERULE_TUTORIAL == pWORLD_INFO->sRuleInfo.byRuleType )
			{
				if( LoadFlash("DBO_Tutorial_01.swf", m_pFlash) )
				{
					LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
					return;
				}
			}
			else if( GAMERULE_TIMEQUEST == pWORLD_INFO->sRuleInfo.byRuleType )
			{
				sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = (sTIMEQUEST_TBLDAT*)API_GetTableContainer()->GetTimeQuestTable()->FindData( pWORLD_INFO->sRuleInfo.sTimeQuestRuleInfo.timeQuestTblidx );

				if( pTIMEQUEST_TBLDAT )
				{
					char acBuffer[1024];
					wcstombs(acBuffer, pTIMEQUEST_TBLDAT->wszPrologueDirection, 1024);

					if( strlen(acBuffer) > 0 )
					{
						if( LoadFlash(acBuffer, m_pFlash) )
						{
							LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
							return;
						}
					}
				}
			}

			if( LoadPage_Job() )
				LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			
			break;
		}
	case LOADING_PAGE_THIRD_STEP:
		{
			m_pTipTitle		->Show(false);
			m_pTipMessage	->Show(false);

			if( LoadFlash("DBO_Tutorial_02.swf", m_pFlash) )
				LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			break;
		}
	default:
		{
			DBO_FAIL("Wrong loading page type");
			break;
		}
	}
}

RwBool CLoadingGui::LoadPage_Job()
{
	SAvatarInfo*	pAvatarInfo	= GetNtlSLGlobal()->GetAvatarInfo();
	CPCTable*		pPCTable	= API_GetTableContainer()->GetPcTable();
	sPC_TBLDAT*		pPC_TBLDAT	= reinterpret_cast<sPC_TBLDAT*>( pPCTable->FindData(pAvatarInfo->sCharPf.tblidx) );

	if( !pPC_TBLDAT )
	{
		DBO_FAIL("Not exist pc table of index : " << pAvatarInfo->sCharPf.tblidx);
		return FALSE;
	}


	std::string strFlashName = "Loading_Default";

	switch( pPC_TBLDAT->byClass )
	{
		case PC_CLASS_HUMAN_FIGHTER:
		case PC_CLASS_STREET_FIGHTER:
		case PC_CLASS_SWORD_MASTER:
		{
			strFlashName	+= "_HFI.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		case PC_CLASS_HUMAN_MYSTIC:
		case PC_CLASS_CRANE_ROSHI:
		case PC_CLASS_TURTLE_ROSHI:
		{
			strFlashName	+= "_HMY.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		case PC_CLASS_HUMAN_ENGINEER:
		case PC_CLASS_GUN_MANIA:
		case PC_CLASS_MECH_MANIA:
		{
			strFlashName += "_HMY.swf"; // to do
			m_idxStart = 101;
			m_idxEnd = 199;
		}
		break;
		case PC_CLASS_NAMEK_FIGHTER:
		case PC_CLASS_DARK_WARRIOR:
		case PC_CLASS_SHADOW_KNIGHT:
		{
			strFlashName	+= "_NFI.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		case PC_CLASS_NAMEK_MYSTIC:
		case PC_CLASS_DENDEN_HEALER:
		case PC_CLASS_POCO_SUMMONER:
		{
			strFlashName	+= "_NMY.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		case PC_CLASS_MIGHTY_MAJIN:
		case PC_CLASS_GRAND_MA:
		case PC_CLASS_ULTI_MA:
		{
			strFlashName	+= "_MMI.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		case PC_CLASS_WONDER_MAJIN:
		case PC_CLASS_PLAS_MA:
		case PC_CLASS_KAR_MA:
		{
			strFlashName	+= "_MWO.swf";
			m_idxStart		= 101;
			m_idxEnd		= 199;
		}
		break;
		default:
		{
			DBO_FAIL("Wrong class : " << pPC_TBLDAT->byClass);
			return FALSE;
		}
	}

	return LoadFlash(strFlashName.c_str(), m_pFlash);
}

RwBool CLoadingGui::LoadPage_Region()
{
	char acBuffer[1024];

	sWORLD_TBLDAT* pENTER_WORLD_TBLDAT = (sWORLD_TBLDAT*)API_GetTableContainer()->GetWorldTable()->FindData( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx ); 
	if( pENTER_WORLD_TBLDAT )
	{
		sWORLD_INFO* pWORLD_INFO = Logic_GetActiveWorldInfo();
		if( pWORLD_INFO )
		{
			m_idxStart	= pWORLD_INFO->worldID*100 + 1;
			m_idxEnd	= pWORLD_INFO->worldID*100 + 99;
		}

		wcstombs( acBuffer, pENTER_WORLD_TBLDAT->wszEnterResourceFlash, 1024 );
		return LoadFlash(acBuffer, m_pFlash);
	}

	return FALSE;
}

RwBool CLoadingGui::LoadPage_GameTip()
{
	// Introduce contents by level of avatar. Hard-coded because it is content that can not be added often.


	RwUInt8 byLevel = 0;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar )
	{
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
		byLevel = pAvatarAttr->GetLevel();
	}
	else
	{
		SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
		CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
		if( !pLobby )
		{
			DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
			return FALSE;
		}

		sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
		if( !pLOBBY_CHARACTER )
		{
			DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
			return FALSE;
		}

		byLevel = pLOBBY_CHARACTER->tSummary.byLevel;
	}


	if( byLevel < 10 )
	{
		RwInt32 iIndex = NtlRandomNumber(0, 2);

		switch(iIndex)
		{
			case 0:		m_idxStart = 101;		m_idxEnd = 199;			return LoadFlash("Loading_TLQ.swf", m_pFlash);
			case 1:		m_idxStart = 201;		m_idxEnd = 299;			return LoadFlash("Loading_TMQ1.swf", m_pFlash);
			case 2:		m_idxStart = 301;		m_idxEnd = 399;			return LoadFlash("Loading_DBC.swf", m_pFlash);
			default:	DBO_FAIL("Invalid index : " << iIndex);			return FALSE;
		}
	}
	else
	{
		RwInt32 iIndex = NtlRandomNumber(0, 15);

		switch(iIndex)
		{
			case 0:		m_idxStart = 401;		m_idxEnd = 499;			return LoadFlash("Loading_Vehicle.swf", m_pFlash);
			case 1:		m_idxStart = 501;		m_idxEnd = 599;			return LoadFlash("Loading_Dojo.swf", m_pFlash);
			case 2:		m_idxStart = 601;		m_idxEnd = 699;			return LoadFlash("Loading_HTB1.swf", m_pFlash);
			case 3:		m_idxStart = 701;		m_idxEnd = 799;			return LoadFlash("Loading_Class_Change1.swf", m_pFlash);
			case 4:		m_idxStart = 801;		m_idxEnd = 899;			return LoadFlash("Loading_Transform1.swf", m_pFlash);
			case 5:		m_idxStart = 901;		m_idxEnd = 999;			return LoadFlash("Loading_RP_Ability.swf", m_pFlash);
			case 6:		m_idxStart = 1001;		m_idxEnd = 1099;		return LoadFlash("Loading_Hoipoi_Mix.swf", m_pFlash);
			case 7:		m_idxStart = 1101;		m_idxEnd = 1199;		return LoadFlash("Loading_RB1.swf", m_pFlash);
			case 8:		m_idxStart = 1201;		m_idxEnd = 1299;		return LoadFlash("Loading_UD1.swf", m_pFlash);
			case 9:		m_idxStart = 1301;		m_idxEnd = 1399;		return LoadFlash("Loading_HTB2.swf", m_pFlash);
			case 10:	m_idxStart = 1601;		m_idxEnd = 1699;		return LoadFlash("Loading_Budoji_Milage1.swf", m_pFlash);
			case 11:	m_idxStart = 1701;		m_idxEnd = 1799;		return LoadFlash("Loading_Tenkaichi1.swf", m_pFlash);
			default: break; // return false;
		}
	}

	return FALSE;
}

VOID CLoadingGui::SetGameTipText()
{
	if( INVALID_TBLIDX == m_idxStart )
		return;


	CTextTable*		pDBOTipTextTable	= API_GetTableContainer()->GetTextAllTable()->GetDBOTipTbl();

	// avooo's comment: First of all, write two help articles for each type.
	m_idxEnd	= m_idxStart + 1;

	for( RwUInt8 i = 0 ; i < 2 ; ++i )
	{
		// Randomly selects a table from the start and end of the index to be searched.
		RwInt32			iSize			= abs((RwInt32)(m_idxEnd - m_idxStart));

		srand( GetTickCount() );

		RwInt32			iRendom			= NtlRandomNumber(0, iSize);
		TBLIDX			idxTable		= m_idxStart + (RwUInt32)iRendom;
		sTEXT_TBLDAT*	pTEXT_TBLDAT	= NULL;


		while( true )
		{
			pTEXT_TBLDAT = reinterpret_cast<sTEXT_TBLDAT*>( pDBOTipTextTable->FindData(idxTable) );
			if( pTEXT_TBLDAT )
			{
				m_pTipMessage->SetText(pTEXT_TBLDAT->wstrText.c_str());
				return;
			}

			++idxTable;

			if( INVALID_TBLIDX == idxTable || m_idxEnd < idxTable )
				idxTable = m_idxStart;

			// Even though I went around the search area, there are no tables.
			if( idxTable == (m_idxStart + (RwUInt32)iRendom) )
				break;
		}

		// If Text is not found in the first While, display basic help.
		m_idxStart	= 1;
		m_idxEnd	= 10;
	}
}

RwBool CLoadingGui::LoadFlash(const char* pcFileName, gui::CFlash* pFlash)
{
	if( pFlash->Load(pcFileName) == false )
	{
		if( pcFileName )
		{
			char acBuffer[256] = "";
			sprintf_s(acBuffer, 256, "CLoadingGui::LoadFlash, Not exist %s flash file", pcFileName);
			NtlLogFilePrint(acBuffer);
		}

		return FALSE;
	}

	pFlash->PlayMovie(TRUE);

	return TRUE;
}

VOID CLoadingGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle	rect;
	RwInt32		iYPos	= iHeight - m_pTipMessage->GetHeight() - 10;

	rect.SetRectWH(0, 0, iWidth, iHeight);

	m_pThis		->SetPosition( rect );
	m_pFlash	->SetPosition( rect );

	m_pTipMessage->SetPosition(20, iYPos);
	iYPos -= m_pTipTitle->GetHeight();

	m_pTipTitle->SetPosition(20, iYPos);	
}

VOID CLoadingGui::OnMovieEnd(gui::CComponent* pComponent)
{
	if( LOADING_PAGE_SECOND_STEP == m_eLoadingPageType )
	{
		CDboEventGenerator::LoadingEvent(LOADING_EVENT_SECOND_MOVIE_END);
	}
	else if( LOADING_PAGE_THIRD_STEP == m_eLoadingPageType )
	{
		CDboEventGenerator::LoadingEvent(LOADING_EVENT_THIRD_MOVIE_END);
	}
}

VOID CLoadingGui::OnMouseDown( const CKey& key )
{

}

VOID CLoadingGui::Render()
{
	if( IsShow() )
		m_pThis->Paint();
}

VOID CLoadingGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CLoadingGui::HandleEvents");

	if(msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
		LocateComponent(pEvent->iWidht, pEvent->iHeight);	
	}

	NTL_RETURNVOID();
}