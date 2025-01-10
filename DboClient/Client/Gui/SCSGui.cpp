#include "precomp_dboclient.h"

//Core
#include "NtlDebug.h"
#include "ceventhandler.h"

//Gui
#include "gui_define.h"

//Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// Framework
#include "NtlApplication.h"


// Table
#include "HelpTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "SCSGui.h"

SCSGui::SCSGui()
{

}

SCSGui::~SCSGui()
{

}

RwBool SCSGui::Create( )
{
	return true;
}

void SCSGui::Destroy( )
{

}

void SCSGui::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSGui::SwitchDialog(bool bOpen)
{
	return 0;
}

