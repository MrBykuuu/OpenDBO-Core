#include "precomp_dboclient.h"
#include "AltarGui.h"

#include "NtlDebug.h"

#include "DboGlobal.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSlLogic.h"
#include "TableContainer.h"
#include "DragonBallTable.h"
#include "ItemTable.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobAvatarAttr.h"
#include "NtlWorldConcept.h"
#include "NtlSobAvatar.h"
#include "ChatGui.h"
#include "TextAllTable.h"
#include "NtlItem.h"
#include "NtlResultCode.h"

#include "GUISoundDefine.h"

#include "NtlPLGuiManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "NtlSobManager.h"
#include "InputActionMap.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"

#include "DBCRewardGUI.h"
#include "DBCNarrationGui.h"
#include "NtlWorldConceptDBC.h"

#define DB_NORMAL_ICON_SIZE 34
#define DB_RAID_ICON_SIZE	42

CAltarGui* CAltarGui::m_pInstance = NULL;

CAltarGui::CAltarGui(void)
{
	Init();
}

CAltarGui::CAltarGui( const RwChar* pName ) 
: CNtlPLGui(pName)
{
	Init();
}

CAltarGui::~CAltarGui(void)
{
	Destroy();
}

VOID CAltarGui::Init() 
{
	m_pBtnExit		= NULL;
	m_pBtnOK		= NULL;
	m_pInKeyword	= NULL;	
	m_pPnlInputBack = NULL;
	m_nDBCount		= 0;
	m_bSpawnDragon	= FALSE;
	m_pWorldConceptDBC = NULL;
	m_bDelete		= FALSE;
	m_pStaticKeyword = NULL;

	for(int i = 0; i < DRAGON_BALL_TYPE_COUNT; ++i)
    {
        for(int j = 0; j < NTL_ITEM_MAX_DRAGONBALL; ++j)
        {
            m_DBSlot[i][j].m_eDBType = E_DRAGONBALL_NONE;
            m_DBSlot[i][j].m_surface.UnsetTexture();
            m_DBSlot[i][j].m_pSobItem = NULL;
        }
    }
}

RwBool CAltarGui::Create() 
{
	NTL_FUNCTION(__FUNCTION__);
	
	if(!CNtlPLGui::Create("", "gui\\Altar.srf", "gui\\Altar.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pPnlInputBack = (gui::CPanel*)GetComponent("pnlInputBack");
	for(int i = 0; i < 7 ; ++i)
	{
		char buf[64];
		sprintf_s(buf, "pnlLight%d", i);
		m_apPnlLight[i] = (gui::CPanel*)GetComponent(buf);
	}
	m_pBtnExit		= (gui::CButton*)GetComponent("btnExit");
	m_pBtnOK		= (gui::CButton*)GetComponent("btnOK");
	m_pInKeyword	= (gui::CInputBox*)GetComponent("inKeyword");
	m_pStaticKeyword = (gui::CStaticBox*)GetComponent("sttInputText");	

	m_slotMove	 	    = m_pThis->SigMove().Connect( this, &CAltarGui::OnMove );	
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect(this, &CAltarGui::OnMouseDown);
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CAltarGui::OnMouseUp);
	m_slotPaint			= m_pBtnExit->SigPaint().Connect(this, &CAltarGui::OnPaint);	
	m_slotClickExitBtn	= m_pBtnExit->SigClicked().Connect(this, &CAltarGui::OnClickExitBtn);
	m_slotClickOKBtn	= m_pBtnOK->SigClicked().Connect(this, &CAltarGui::OnClickOKBtn);
	m_slotGotFocus		= m_pInKeyword->SigGotFocus().Connect(this, &CAltarGui::OnInputBoxGotFocus);
	m_slotLostFocus		= m_pInKeyword->SigLostFocus().Connect(this, &CAltarGui::OnInputBoxLostFocus);
    m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CAltarGui::OnCaptureMouseDown);

	m_pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);

	// [order]
	WCHAR buf[32] = {0,};
	swprintf_s(buf, L"[%s]", GetDisplayStringManager()->GetString("DST_DBC_KEYWORD"));
	m_pStaticKeyword->SetText(buf);

	LinkMsg(g_EventDBCCheck_Res);

	Show(false);	
	GetNtlGuiManager()->AddUpdateFunc(this);

	return TRUE;
}

VOID CAltarGui::Destroy() 
{
	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventDBCCheck_Res);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CAltarGui::Update(RwReal fElapsed)
{
	if(m_bDelete)
	{
		CAltarGui::DeleteInstance();
		return;
	}

	BYTE byIncAlpha = (BYTE)(fElapsed * 255.0f);

	// Fade effect on the panel that appears when the dragon ball is equipped
    RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();
	for(int i = 0; i < 7; ++i)
	{
		if(m_apPnlLight[i] && m_apPnlLight[i]->IsVisible())
		{
            if(nDBKind != DRAGON_BALL_TYPE_NONE && m_DBSlot[nDBKind][i].m_eDBType != E_DRAGONBALL_NONE)			
			{
				if(m_apPnlLight[i]->GetAlpha() < 255)
				{
					RwInt32 nAlpha = m_apPnlLight[i]->GetAlpha() + byIncAlpha;
					if(nAlpha >= 255)
						nAlpha = 255;

					m_apPnlLight[i]->SetAlpha((BYTE)nAlpha);
				}				
			}
			else
			{
				RwInt32 nAlpha = m_apPnlLight[i]->GetAlpha() - byIncAlpha;
				if(nAlpha <= 0)
				{
					m_apPnlLight[i]->Show(FALSE);
				}
				else
				{
					m_apPnlLight[i]->SetAlpha((BYTE)nAlpha);
				}
			}
		}
	}

	
}

RwInt32 CAltarGui::SwitchDialog( bool bOpen ) 
{
	if(bOpen)
	{	
		GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT)->ChangeState(WORLD_DBC_ALTAR_UI);		
		Show(true);

		Logic_PlayGUISound ( GSD_SYSTEM_DRAGOBALL_UI_OPEN );

		m_pPnlInputBack->Show(FALSE);
		m_pStaticKeyword->Show(FALSE);
		m_pInKeyword->Show(FALSE);
		m_pBtnOK->Show(FALSE);		

		for(int i = 0; i < 7; ++i)
		{
			if(m_apPnlLight[i])
				m_apPnlLight[i]->Show(FALSE);
		}
	}
	else
	{
		Show(false);

		Logic_PlayGUISound ( GSD_SYSTEM_DRAGOBALL_UI_CLOSE );

		if(!m_bSpawnDragon)	// It was canceled without summoning the Dragon God.
		{
			GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_DRAGONBALL_COLLECT);
			
			//Unlocks the currently set Dragon Ball items.
			for(int i = 0; i < 7; ++i)
			{
				RemoveDragonBallSlot(i);
			}
		}

		m_bDelete = TRUE;
	}

	return 1;
}

VOID CAltarGui::HandleEvents( RWS::CMsg &pMsg ) 
{
	if(pMsg.Id == g_EventDBCCheck_Res)
	{
		SNtlEventDBC_Res* pData = (SNtlEventDBC_Res*)pMsg.pData;
		if(pData)
		{
			if(pData->wResultCode == GAME_SUCCESS)
			{
				CNtlWorldConceptDBC* pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
				pWorldConceptDBC->ResetRemainTime();

                // Creating instances of additional UIs
                CDBCNarrationGui::CreateInstance();
                CDBCRewardGUI::CreateInstance();                

                // Summon the Dragon God
				SpawnDragon();					
			}
			else
			{
				CNtlSLEventGenerator::SysMsg(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), Logic_GetResultCodeString(pData->wResultCode, "").c_str());
			}						
		}
	}	
}

void CAltarGui::CreateInstance()
{
	// Create a singleton and register it in the dialog manager.
	if(!m_pInstance)
	{
		m_pInstance = NTL_NEW CAltarGui("AltarGui");
		if(!m_pInstance->Create())
		{
			m_pInstance->Destroy();
			NTL_DELETE(m_pInstance);			
		}

		// Add to Gui Manager.
		GetNtlGuiManager()->AddGui(m_pInstance);
		GetDialogManager()->RegistDialog(DIALOG_DBC_ALTAR, m_pInstance, &CAltarGui::SwitchDialog);

        GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	}
}
void CAltarGui::DeleteInstance() 
{
	if(m_pInstance)
	{
		GetDialogManager()->UnRegistDialog(DIALOG_DBC_ALTAR);		
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pInstance);		
		m_pInstance->Destroy();
		NTL_DELETE(m_pInstance);		
	}	
}

RwInt32 CAltarGui::GetChildSlotIdx( RwInt32 nX, RwInt32 nY ) 
{
	// Since the slot for Raid is larger, for convenience, check the slot size for Legendary.
	for(RwInt32 i = 0; i < 7; ++i)
	{
        if(m_DBSlot[DRAGON_BALL_TYPE_LEGENDARY][i].m_rt.PtInRect(nX, nY))
            return i;
	}

	return -1;
}

VOID CAltarGui::OnClickExitBtn( gui::CComponent* pComponent ) 
{
	// If it is locked, release it.
	if(API_GetSLPacketLockManager()->Lock(GU_DRAGONBALL_CHECK_RES))
		API_GetSLPacketLockManager()->Unlock(GU_DRAGONBALL_CHECK_RES);

	GetDialogManager()->CloseDialog(DIALOG_DBC_ALTAR);		

	// Unlock all locked Dragon Balls.
	for(int i = 0; i < 7; ++i)
	{
		RemoveDragonBallSlot(i);
	}
}

VOID CAltarGui::OnClickOKBtn( gui::CComponent* pComponent ) 
{
	// If you click once and are waiting for a packet response, it will not be clicked.
	if(API_GetSLPacketLockManager()->IsLock(GU_DRAGONBALL_CHECK_RES))
		return;

	if(m_nDBCount < 7)
	{
		//An error message appears indicating that the number of dragon balls is insufficient.				
		GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_ENOUGH" );
		return;
	}

	if(m_pInKeyword->GetLength() <= 0)
	{
		//An error message is displayed asking you to enter an order.				
		GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_KEYWORD" );
		return;
	}

	// Send a verification packet to the server.	
	sITEM_POSITION_DATA dbItemData[7];
    RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();

	for(int i = 0; i < 7; ++i)
	{
        // There is a difference of 1 in the bag index between the server and the client.
        dbItemData[i].hItem		= m_DBSlot[nDBKind][i].m_pSobItem->GetSerialID();
        dbItemData[i].byPlace	= (BYTE)m_DBSlot[nDBKind][i].m_pSobItem->GetParentItemSlotIdx() + 1;
        dbItemData[i].byPos		= (BYTE)m_DBSlot[nDBKind][i].m_pSobItem->GetItemSlotIdx();
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendDBCCheckReq(m_pWorldConceptDBC->GetAltarSerialID(), dbItemData);
}

VOID CAltarGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	// Hardcode the slot location	
    for(int i = 0; i < DRAGON_BALL_TYPE_COUNT; ++i)
    {
        if(i != DRAGON_BALL_TYPE_LEGENDARY)
        {
            // For size 34
            m_DBSlot[i][0].m_rt.SetRectWH(160, 88, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][1].m_rt.SetRectWH(280, 85, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][2].m_rt.SetRectWH(351, 182, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][3].m_rt.SetRectWH(330, 299, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][4].m_rt.SetRectWH(223, 348, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][5].m_rt.SetRectWH(119, 302, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
            m_DBSlot[i][6].m_rt.SetRectWH(89, 188, DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);
        }
        else    
        {
            // For size 42
            m_DBSlot[i][0].m_rt.SetRectWH(156, 84, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][1].m_rt.SetRectWH(275, 81, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][2].m_rt.SetRectWH(347, 176, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][3].m_rt.SetRectWH(325, 294, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][4].m_rt.SetRectWH(217, 343, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][5].m_rt.SetRectWH(115, 286, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
            m_DBSlot[i][6].m_rt.SetRectWH(85, 184, DB_RAID_ICON_SIZE, DB_RAID_ICON_SIZE);
        }

        for(int j = 0; j < NTL_ITEM_MAX_DRAGONBALL; ++j)
        {
            m_DBSlot[i][j].m_surface.SetRectWH(m_pThis->GetPosition().left + m_DBSlot[i][j].m_rt.left, 
                                               m_pThis->GetPosition().top + m_DBSlot[i][j].m_rt.top, 
                                               DB_NORMAL_ICON_SIZE, DB_NORMAL_ICON_SIZE);            
        }
    }
}

VOID CAltarGui::OnMouseDown( const CKey& key ) 
{
	
}

VOID CAltarGui::OnMouseUp( const CKey& key ) 
{
	RwInt32 nClickIdx = GetChildSlotIdx((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if(nClickIdx == -1)
		return;

	// Attached with the left button, detached with the right attachment.
	if(key.m_nID == UD_LEFT_BUTTON)
	{
        if(GetIconMoveManager()->IsActive() && GetDialogManager()->IsMode(DIALOGMODE_UNKNOWN))            
		{
			// If already installed, return immediately		
            RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();
            if(nDBKind != DRAGON_BALL_TYPE_NONE && m_DBSlot[nDBKind][nClickIdx].m_eDBType != E_DRAGONBALL_NONE)
                return;

			RwUInt32 uiSerial = GetIconMoveManager()->GetSrcSerial();            
            if(uiSerial == INVALID_SERIAL_ID)
            {
                GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_DRAGONBALL" );
                return;
            }

            sITEM_TBLDAT* pTblDat = Logic_GetItemDataFromSob(uiSerial);
            if(!pTblDat)
            {
                GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_DRAGONBALL" );
                return;
            }

			TBLIDX tblIdx = pTblDat->tblidx;

			CDragonBallTable* pDBTable = API_GetTableContainer()->GetDragonBallTable();
			if(pDBTable)
			{
				BYTE byType = 0;
				BYTE byNumber = 0;

				if(!pDBTable->GetType(tblIdx, byType, byNumber))
                {
					GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_DRAGONBALL" );
					return;
                }

                // When it is not the same type as the one currently in place
                if(m_pWorldConceptDBC->GetDBKind() != byType &&
                   m_pWorldConceptDBC->GetDBKind() != DRAGON_BALL_TYPE_NONE)
                {
                    // Error message (only dragon balls of the same type can be placed)
                    GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_SAME" );
                    return;
                }

                // When a dragon ball with the same number is already placed
                if(IsExistSameType((EDragonBallType)byNumber))
                {
                    GetAlarmManager()->AlarmMessage( "DST_DBC_NOT_SAME_NUMBER");
                    return;
                }       

                // Set the current foundation type
                m_pWorldConceptDBC->SetDBKind((eDRAGON_BALL_TYPE)byType);
                SetDragonBallSlot(nClickIdx, (EDragonBallType)byNumber);

				Logic_PlayGUISound ( GSD_SYSTEM_DRAGOBALL_REGISTRATION );
				Logic_PlayGUISound ( GSD_SYSTEM_DRAGOBALL_UI_LIGHT );					
			}
		}
	}
	else if(key.m_nID == UD_RIGHT_BUTTON)
	{
		if(GetIconMoveManager()->IsActive())
		{
			GetIconMoveManager()->IconMoveEnd();
		}
		else
		{
			// Disarms the equipped Dragon Ball.
			RemoveDragonBallSlot(nClickIdx);
		}		
	}
}

VOID CAltarGui::OnPaint() 
{
    RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();
    if(nDBKind == DRAGON_BALL_TYPE_NONE)
        return;

    for(int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
    {
        if(m_DBSlot[nDBKind][i].m_eDBType != E_DRAGONBALL_NONE)
        {
            m_DBSlot[nDBKind][i].m_surface.Render();
        }
    }
}

VOID CAltarGui::SetDragonBallSlot( RwInt32 nSlotIdx, EDragonBallType eDragonBallType ) 
{
	// Change the appearance of the altar UI
	m_apPnlLight[nSlotIdx]->Show(TRUE);
	m_apPnlLight[nSlotIdx]->SetAlpha(0);

	// Apply Item Lock.
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
	CDboEventGenerator::DialogEvent( DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx() );
	m_pWorldConceptDBC->AddDragonBall(pItem);

    char buf[64] = {0,};
    RwInt32 nDBKInd = (RwInt32)m_pWorldConceptDBC->GetDBKind();
    
    switch(nDBKInd)
    {
    case DRAGON_BALL_TYPE_BASIC:
        sprintf_s(buf, "icon\\BIG_DBL_B_%02d.png", eDragonBallType);        
        break;
    case DRAGON_BALL_TYPE_NORMAL:
        sprintf_s(buf, "icon\\big_db_%02d.png", eDragonBallType);        
        break;
    case DRAGON_BALL_TYPE_LEGENDARY:
        sprintf_s(buf, "icon\\BIG_DBL_R_%02d.png", eDragonBallType);        
        break;
    case DRAGON_BALL_TYPE_EVENT:
        sprintf_s(buf, "icon\\BIG_DBL_E_%02d.png", eDragonBallType);        
        break;
    default:
        NTL_ASSERTFAIL(__FUNCTION__ << "Not DragonBall Type");
        return;
    }

    m_DBSlot[nDBKInd][nSlotIdx].m_surface.SetTexture(buf);
    m_DBSlot[nDBKInd][nSlotIdx].m_eDBType = eDragonBallType;
    m_DBSlot[nDBKInd][nSlotIdx].m_pSobItem = pItem;

	++m_nDBCount;
	NTL_ASSERT(m_nDBCount <= 7, "DragonBall Count is Over 7");

	GetIconMoveManager()->IconMoveEnd();

	

	// When 7 Dragon Balls are equipped, the default wish is displayed.
	if(m_nDBCount == 7)
	{
		m_pPnlInputBack->Show(TRUE);
		m_pStaticKeyword->Show(TRUE);
		m_pInKeyword->Show(TRUE);
		m_pBtnOK->Show(TRUE);

		sDRAGONBALL_TBLDAT* pData =  (sDRAGONBALL_TBLDAT*)API_GetTableContainer()->GetDragonBallTable()->GetDBTbldat((BYTE)m_pWorldConceptDBC->GetDBKind());
		if(pData)
		{
			std::wstring strSummonChat = API_GetTableContainer()->GetTextAllTable()->GetDragonTbl()->GetText(pData->defaultSummonChat);
			m_pInKeyword->SetText(strSummonChat.c_str());
		}
	}
	else
	{
		m_pPnlInputBack->Show(FALSE);
		m_pInKeyword->Show(FALSE);
		m_pBtnOK->Show(FALSE);

		m_pInKeyword->Clear();
	}
}

VOID CAltarGui::RemoveDragonBallSlot( RwInt32 nSlotIdx ) 
{
    RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();	
    if(nDBKind >= DRAGON_BALL_TYPE_COUNT)
        return;

	m_DBSlot[nDBKind][nSlotIdx].m_surface.UnsetTexture();
	m_DBSlot[nDBKind][nSlotIdx].m_eDBType = E_DRAGONBALL_NONE;
	
	if(m_DBSlot[nDBKind][nSlotIdx].m_pSobItem)
	{
		m_pWorldConceptDBC->RemoveDragonBall(m_DBSlot[nDBKind][nSlotIdx].m_pSobItem);
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, m_DBSlot[nDBKind][nSlotIdx].m_pSobItem->GetParentItemSlotIdx(), m_DBSlot[nDBKind][nSlotIdx].m_pSobItem->GetItemSlotIdx() );
		m_DBSlot[nDBKind][nSlotIdx].m_pSobItem = NULL;
		--m_nDBCount;		
	}

	if(m_nDBCount < 0)
	{
		m_nDBCount = 0;
	}

	// When all dragon balls are removed, they return to NONE state.
	if(m_nDBCount == 0)
	{
		m_pWorldConceptDBC->SetDBKind(DRAGON_BALL_TYPE_NONE);
	}

	// Cancel the entered order.
	m_pPnlInputBack->Show(FALSE);
	m_pStaticKeyword->Show(FALSE);
	m_pInKeyword->Show(FALSE);
	m_pBtnOK->Show(FALSE);
	m_pInKeyword->Clear();
}

VOID CAltarGui::OnInputBoxGotFocus() 
{
	
}

VOID CAltarGui::OnInputBoxLostFocus() 
{
}

VOID CAltarGui::SpawnDragon() 
{
	m_bSpawnDragon = TRUE;

	//Send message via regular chat	
	std::wstring strFilterText = GetChattingFilter()->Filtering( m_pInKeyword->GetText() );
    GetDboGlobal()->GetChatPacketGenerator()->SendChatMsgShout( strFilterText.c_str() );
    
	// -> Processed in World Concept
    CNtlWorldConceptDBC* pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
    if(pWorldConceptDBC)
    {
        pWorldConceptDBC->ChangeState(WORLD_DBC_NIGHT_ON);        
        pWorldConceptDBC->SetMyDragon(TRUE);                    // The fact that the SpawnDragon() function is called means that I have summoned a dragon god.
    }

	// Close all UI on the screen.		
	GetDialogManager()->CloseNotDefaultDialog();
}

VOID CAltarGui::OnCaptureMouseDown( const CKey& key )
{
    CAPTURE_MOUSEDOWN_RAISE(DIALOG_DBC_ALTAR, key.m_fX, key.m_fY);
}

RwBool CAltarGui::IsExistSameType( EDragonBallType eType ) 
{
    RwInt32 nDBKind = (RwInt32)m_pWorldConceptDBC->GetDBKind();
    if(nDBKind == DRAGON_BALL_TYPE_NONE)
        return FALSE;
    
    for(int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
    {
        if(m_DBSlot[nDBKind][i].m_eDBType == eType)
            return TRUE;
    }

    return FALSE;
}