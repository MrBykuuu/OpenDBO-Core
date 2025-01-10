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
#include "SCSActive.h"

SCSActive::SCSActive()
{

}

SCSActive::~SCSActive()
{

}

RwBool SCSActive::Create( )
{
	return true;
}

void SCSActive::Destroy( )
{

}

void SCSActive::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSActive::SwitchDialog(bool bOpen)
{
	return 0;
}

