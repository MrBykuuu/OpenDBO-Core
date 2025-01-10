#pragma once

#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLXMLScriptHelper.h"

#define OBJECT_XML_SCRIPT_VER       L"1.0"

/**
 * \ingroup NtlPresentation
 * \brief Class for saving/loading object script (XML)
 * \date 2006-06-07
 * \author agebreak
 */
class CNtlPLObjectXMLScript : public CNtlXMLDoc
{
public:
    CNtlPLObjectXMLScript(void);
    virtual ~CNtlPLObjectXMLScript(void);

    RwBool SaveObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty);      ///< Save the script file.
    RwBool LoadObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty);      ///<Load the script file.

protected:    
    RwBool SaveHeader(CNtlPLObjectProperty* pProperty);             ///< Save the header part.
    RwBool SaveBody(CNtlPLObjectProperty* pProperty);               ///< Save the body part.
    RwBool SaveLinkEffect(CNtlPLObjectProperty* pProperty);		    ///< Save the Link Effect Data section.
	RwBool SaveAnimTable(CNtlPLObjectProperty* pProperty);       ///< Save the AnimTable part.      

    RwBool LoadHeader(CNtlPLObjectProperty* pProperty);             ///<Load the header part.
    RwBool LoadBody(CNtlPLObjectProperty* pProperty);               ///<Load the header part.
    RwBool LoadLinkEffect(CNtlPLObjectProperty* pProperty);         ///< Load the Link Effect Data section.
	RwBool LoadAnimTable(CNtlPLObjectProperty* pProperty);                     ///< Loads the AnimTable part.

protected:
    IXMLDOMElement*     m_pElemRoot;                ///< Root Element
    CNtlPLXMLScriptHelper m_XMLScriptHelper;        ///< DBO XML Script Helper
};
