/*****************************************************************************
 *
 * File			: NtlLocalize.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2007. 3. 19	
 * Abstract		: Definition according to Localizing.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

//----------------------------------------
// Execution order of functions in DBO Launcher
//----------------------------------------

// 1. Local_Init();
// 2. Local_DetectData();
// 3. Local_DBOEncryptConfig();
// 4. When running DBO Client, enter DBO encrypt config data in the command.
// 5. Local_Terminate();


//----------------------------------------
// Execution order of functions in DBO client
//----------------------------------------

// 1. Local_Init();
// 2. Local_DBODecryptConfig();  Enter the value in the command.
// 3. Local_Terminate();


#ifndef __NTL_LOCALIZE__
#define __NTL_LOCALIZE__

#include "NtlLocalizeDef.h"

void			Local_Init(void);
void			Local_Terminate(void);

unsigned char	Local_GetType(void);	
SLocalConfig*	Local_GetConfig(void);

//Localizing data detection. (decrypt key transferred to launcher)
bool			Local_DetectData(const char *pKey);


// DBO data make
const char*		Local_DBOEncryptConfig(void);
bool			Local_DBODecryptConfig(const char *pConfig);


#endif
