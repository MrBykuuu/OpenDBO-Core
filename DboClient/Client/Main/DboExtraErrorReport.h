#pragma once

#include "NtlXMLDoc.h"

/**
 * \ingroup Client
 * \brief Class that records additional information in the error report
 * \date 2007-01-25
 * \author agebreak
 */
class CDboExtraErrorReport : public CNtlXMLDoc
{
public:
    CDboExtraErrorReport(void);
    virtual ~CDboExtraErrorReport(void);

    RwBool Open();                                          ///< Open the file.
    RwBool Save(const char* szFileName);                    ///< Record and save the file.

    void WriteInfo(RwChar* szNodeName, RwChar* szInfo);     ///< Records information in the file.
    void WriteInfo(RwChar* szNodeName, WCHAR* szInfo);      ///< Records information in the file.
    static void SetDXDiagLog();                             ///< Records Dxdiag log.

protected:
    RwBool SetIndent();                                             ///< Apply a style sheet to XML. (indent.xls)

protected:
    IXMLDOMElement*     m_pRootElem;                ///< Root Element
};
