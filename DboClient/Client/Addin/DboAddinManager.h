#pragma once
#include "ceventhandler.h"

/// Class that manages addin libraries
/// Currently managing Devil for screenshots.
class CDBOAddinManager : public RWS::CEventHandler
{
public:
    CDBOAddinManager();
    ~CDBOAddinManager();

    static void Init();
    static void ShutDown();

protected:
    virtual void			HandleEvents(RWS::CMsg &pMsg); 

    void    OnEventScreenShot(RWS::CMsg& pMsg);                ///< After taking a screenshot, process file changes

protected:
    static CDBOAddinManager* m_pInstance;

};