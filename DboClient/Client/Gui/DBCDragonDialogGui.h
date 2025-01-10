#pragma once

#include "NtlDebug.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"

class CTextTable;

// Structure that forms a speech bubble
struct SDragonDialog
{
	gui::CPanel*		m_pPanel;
	gui::CStaticBox*	m_pStatic;
	RwReal				m_fLifeTime;

	SDragonDialog()
	{
		m_pPanel = NULL;
		m_pStatic = NULL;
		m_fLifeTime = 0.0f;
	}

	void Show(RwBool bShow)
	{
		m_pPanel->Show(B2b(bShow));
		m_pStatic->Show(B2b(bShow));
	}

	void SetAlpha(RwReal fAlpha)
	{
		BYTE byAlpha = (BYTE)(fAlpha * 255.0f);
		m_pPanel->SetAlpha(byAlpha);
		m_pStatic->SetAlpha((UCHAR)byAlpha);
	}

	void SetPosition(RwInt32 x, RwInt32 y)
	{
		m_pPanel->SetPosition(x, y);
		m_pStatic->SetPosition(x, y);
	}
};

/**
* \ingroup Client
* \brief GUI class that displays the dragon god's dialogue (speech balloon)
* \date 2007-07-6
* \author agebreak
*/
class CDBCDragonDialogGUI : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum EDialogType			///< Type of dialog
	{
		E_DIALOG_NORMAL,
		E_DIALOG_WARN,

		DIALOG_COUNT,
	};

public:
	CDBCDragonDialogGUI(void);
	CDBCDragonDialogGUI(const RwChar* pName);
	virtual ~CDBCDragonDialogGUI(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);	

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	VOID		SetText(RwInt32 nTblIdx, EDialogType eDialogType = E_DIALOG_NORMAL);					///< Displays conversations in the Yongshin conversation table index.
	VOID		SetText(std::wstring& strWString, EDialogType eDialogType = E_DIALOG_NORMAL);		///< Displays text.	
	VOID		ShowNormalDialog(RwBool bShow);				///< Set whether to display the general dialogue window.

	static		CDBCDragonDialogGUI* GetInstance();			///< Returns a singleton instance.
	static		void				 DeleteInstance();		///< Delete the singleton instance.

protected:	
	void		AdjustDialog();								///< Determine the location of the UI.    

protected:
	static		CDBCDragonDialogGUI* m_pInstance;			///< singleton instance

	SDragonDialog	m_SDragonDialog[DIALOG_COUNT];			///< Speech bubble structure to display the dragon god¡¯s conversation
	CTextTable*		m_pDragonTextTable;						///< Dragon God's Dialogue Table		
	RwBool			m_bDelete;
    
};	
