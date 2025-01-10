#pragma once

#include "BugTrap.h"

#ifdef _UNICODE
#pragma comment(lib, "BugTrapU.lib")  // Link to Unicode DLL
#else
#pragma comment(lib, "BugTrap.lib")      // Link to ANSI DLL
#endif

/**
 * \ingroup NtlCore
 * \brief Class that uses BugTrap (http://www.codeproject.com/tools/BugTrap.asp)
 *If you include only this header, a bug trap will be executed when the program crashes.
 *To change the output message, refer to the comments in the cpp file.
 * \date 2006-12-08
 * \author agebreak
 */
class CNtlBugTrap
{
public:
    CNtlBugTrap(void);    
};

static CNtlBugTrap g_dumpTrap; 