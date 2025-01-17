/******************************************************************************
*File			: ScrambleNotifyGui.h
*Author		    : Hong SungBock
*Copyright		: (��)NTL
*Date			: 2009. 5. 6
*Abstract		: 
*****************************************************************************
*Desc			: 
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presetation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SlotGui.h"

class CResultNarrationGui;


class CScrambleNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sSTART_WIDGET
	{
		gui::CDialog*				pParentDialog;
		gui::CFlash*				pFlash;
		gui::CStaticBox*			pOffenceGuildName;
		gui::CStaticBox*			pDefenceGuildName;
		gui::CStaticBox*			pRuleTitle;
		gui::CHtmlBox*				pRule;

		gui::CSlot					slotFSCallback;	
		gui::CSlot					slotMovieEnd;
	};

	struct sREWARD_WIDGET
	{
		gui::CDialog*				pParentDialog;
		gui::CFlash*				pFlash;
		gui::CStaticBox*			pGuildNameStatic;				///< 'U-file name'
		gui::CStaticBox*			pDojoNameStatic;				///< ��Ownership stamp��
		gui::CStaticBox*			pRewardReputationPointStatic;	///< ��Yupa Point��
		gui::CStaticBox*			pRewardItemStatic;				///< ��Acquired items��
		gui::CStaticBox*			pGuildName;
		gui::CStaticBox*			pDojoName;
		gui::CStaticBox*			pRewardReputationPoint;
		gui::CStaticBox*			pRewardItem;

		CResultNarrationGui*		pResultNarration;

		RwBool						bShowRewardItem;
		CRegularSlotGui					srfRewardItem;

		RwUInt32					uiGuildReputation;
		RwUInt32					uiItemTableIndex;
		RwUInt8						byItemCount;

		gui::CSlot					slotFSCallback;	
		gui::CSlot					slotMovieEnd;
	};

	struct sWARNING_INFO
	{
		RwBool						bCanWarningSeal;
		RwBool						bCanWarningPoint;
		RwReal						fWarningElapsed;
	};	

public:
	CScrambleNotifyGui(const RwChar* pName);
	virtual ~CScrambleNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CScrambleNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CheckInfoWindow();	

	VOID			PlayReward();
	VOID			DestroyResultNarration();

	RwBool			LoadFlash(gui::CFlash* pFlash, const char* acFileName);
	VOID			LocateComponent();
	VOID			FinishFlash();

	VOID			OnFSCallback_StartWidget(const char* pcString, const char* pcString2);
	VOID			OnFSCallback_RewardWidget(const char* pcString, const char* pcString2);
	
	VOID			OnMovieEnd_Notify(gui::CComponent* pComponent);
	VOID			OnMovieEnd_Reward(gui::CComponent* pComponent);

	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotMovieEnd_Notify;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CFlash*		pFlash_Notify;
	gui::CFlash*		pFlash_Notify2;

	sSTART_WIDGET		m_tSTART_WIDGET;
	sREWARD_WIDGET		m_tREWARD_WIDGET;

	std::string			m_strFileName;

	sWARNING_INFO		m_tWARNING_INFO;
};