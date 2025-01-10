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
#include "SCSActiveMSG.h"

SCSActiveMSG::SCSActiveMSG()
{

}

SCSActiveMSG::~SCSActiveMSG()
{

}

RwBool SCSActiveMSG::Create( )
{
	return true;
}

void SCSActiveMSG::Destroy( )
{

}

void SCSActiveMSG::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSActiveMSG::SwitchDialog(bool bOpen)
{
	return 0;
}

