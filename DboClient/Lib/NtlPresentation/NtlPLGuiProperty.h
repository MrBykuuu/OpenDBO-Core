/*****************************************************************************
 *
 * File			: NtlPLGuiProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 8. 22	
 * Abstract		: Presentation layer gui property class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLGUI_PROPERTY_H__
#define __NTL_PLGUI_PROPERTY_H__

#include <string>
#include "NtlPLProperty.h"

/**
 * \ingroup NtlPresentation
 *This is a property class corresponding to CNtlPLGui.
 *Property data is written as an xml file.
 *
 */

class CNtlPLGuiProperty : public CNtlPLProperty
{
public:

	static unsigned int m_strVer;			/* gui property file version **/
	static std::string m_strScriptPath;		/* gui script path name **/ 
	static std::string m_strTexPath;		/* path name of gui texture file **/ 
	
	std::string m_strResFile;				/* gui resource script file name **/ 
	std::string m_strSurFile;				/* gui surface script file name **/ 
	std::string m_strCompFile;				/* gui component script file name **/ 

public:

	/**
    *  Default constructor for Presentation object property
    *
    */
	CNtlPLGuiProperty();

	/**
    *  destructor for Presentation object property
    *
    */
	virtual ~CNtlPLGuiProperty();

	/**
    *Function to load object property data from xml file
	*  \param pDoc is an xml wrapping class pointer.
	*  \param pNod is a valid xml node.
    *
    */
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);

	/**
    *Function to save object property data to xml file
	*  \param pDoc is an xml wrapping class pointer.
	*  \param pNod is a valid xml node.
    *
    */
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
};


#endif