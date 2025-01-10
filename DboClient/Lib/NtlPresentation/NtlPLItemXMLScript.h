#pragma once

#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLItemProperty.h"
#include "NtlPLXMLScriptHelper.h"

#define ITEM_XML_SCRIPT_VER       L"2.0"

/**
 * \ingroup NtlPresentation
 * \brief Class that saves/loads item script (XML)
 * \date 2006-06-26
 * \author agebreak
 */
class CNtlPLItemXMLScript : public CNtlXMLDoc
{
public:
    CNtlPLItemXMLScript(void);
    virtual ~CNtlPLItemXMLScript(void);

    RwBool SaveItemScript(const char* strFileName, CNtlPLItemProperty* pProperty);      ///< Save the script file.
    RwBool LoadItemScript(const char* strFileName, CNtlPLItemProperty* pProperty);      ///<Load the script file.

protected:
    RwBool SaveHeader(CNtlPLItemProperty* pProperty);             ///< Save the header part.
    RwBool SaveBody(CNtlPLItemProperty* pProperty);               ///< Save the body part.        

    RwBool LoadHeader(CNtlPLItemProperty* pProperty);             ///<Load the header part.
    RwBool LoadBody(CNtlPLItemProperty* pProperty);               ///< Save the body part


    RwBool SaveLinkEffect(CNtlPLItemProperty* pProperty);    ///< Save the Link Effect Data section.
    RwBool LoadLinkEffect(CNtlPLItemProperty* pProperty);    ///< Load the Link Effect Data section.

    void   SaveTraceEvent(const SEventTrace* pEventTrace);  ///< Save the Trace Event.    
    void   LoadTraceEvent(SEventTrace* pEventTrace);           ///< Load Trace Event.

    RwBool SaveAnimTable(CNtlPLItemProperty* pProperty);          ///< Saves the AnimTable of the Item.
    RwBool LoadAnimTable(CNtlPLItemProperty* pProperty);          ///< Loads the AnimTable of the Item.  

    RwBool SaveUpgradeEffect(CNtlPLItemProperty* pProperty);      ///< Save the Upgrade Effect Property of the item.
    RwBool LoadUpgradeEffect(CNtlPLItemProperty* pProperty);      ///< Load the UpgradeEffect Property of the Item.

protected:
    IXMLDOMElement*     m_pElemRoot;                ///< Root Element
    RwReal              m_fScriptVer;               ///< Version of the script currently being loaded
    CNtlPLXMLScriptHelper m_XMLScriptHelper;        ///< DBO XML Script Helper
};
