#pragma once

#include "DboGlobal.h"

/// Class that encrypts/decrypts DBO-related config and script files.
class CDBOEncrypter
{
public:
    CDBOEncrypter();
    ~CDBOEncrypter();    

    // Config options.xml 
    RwBool  EncryptConfig(const RwChar* szFilePath);                 ///< Encrypt the Config file.
    RwBool  DecryptConfig(OUT SConfigData* pConfigData, const RwChar* szFilePath);                 ///< Decrypt the Config file.

    // Lua compilation 
    RwBool  CompileLuaFiles();                                      ///< Compile all Lua files in the Script folder.

protected:    
    

    // Config options.xml 
    RwBool LoadConfigOptions(OUT SConfigData* pConfigData, const RwChar* szFilePath);             ///< Load the Config file.
    RwBool SaveConfigOptions(SConfigData* pConfigData, const RwChar* szDestPath, const RwChar* szCryptPassword);

protected:

};

static CDBOEncrypter* GetDBOEncrypter()
{
    static CDBOEncrypter dboEnctyper;

    return &dboEnctyper;
}