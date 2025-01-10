#include "precomp_ntlcore.h"
#include "NtlBugTrap.h"
#include "NtlDebug.h"
#include "new.h"

void InvalidParameterHandler(const wchar_t* expression,
                             const wchar_t* function,
                             const wchar_t* file,
                             unsigned int line,
                             uintptr_t pReserved)
{
    throw NTL_NEW std::exception("InvalidParameterHandler");
}

void PurecallHandler(void)
{
    throw NTL_NEW std::exception("PurecallHandler");
}

int NewHandler( size_t )
{
    NTL_ASSERTFAIL("new Handler Fail!");
    throw NTL_NEW std::exception("NewHandler");
}

CNtlBugTrap::CNtlBugTrap(void)
{
    // Setup bug trapper
    // The bug trap can be changed by resetting it in each program that will use it.
    // BT_SetAppName(_T("BugTrap Report"));
    // BT_SetSupportEMail(_T("agebreak@ntl-inc.com"));
    // BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL | BTF_ATTACHREPORT | BTF_SCREENCAPTURE | BTF_INTERCEPTSUEF);
	// BT_SetFlags(BTF_NONE);
    // BT_SetSupportURL(_T("https://opendbo.org"));

    // BugTrapServer ===========================================
    // BT_SetSupportServer(_T("localhost"), 9999);

    // Invalid argument use for C standard functions
    _set_invalid_parameter_handler(InvalidParameterHandler);

    // Invalid pure virtual function call
    _set_purecall_handler(PurecallHandler);

    // New operator fails when allocating too much memory
    _set_new_handler( NewHandler );
}
