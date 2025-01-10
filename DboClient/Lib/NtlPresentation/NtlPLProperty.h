/*****************************************************************************
 *
 * File			: NtlPLProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation layer property base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLPROPERTY_H__
#define __NTL_PLPROPERTY_H__

#define SIZE_PROP_BUF   64

#include "NtlSerializer.h"

class CNtlXMLDoc;

/**
 * \ingroup NtlPresentation
 *This is a base class that represents the attribute of the entity.
 *For all entities, property data is passed through the SetProperty interface function after an instance is created.
 *
 */
class CNtlPLProperty
{
protected:

	RwUInt32 m_uiId;
	std::string m_strName;				/**< String describing the presentation entity property name */
	std::string m_strClassID;			/**< String describing the presentation entity property class id */
    RwUInt32    m_uiFlag;               ///< Flag for each entity 

public:

	/**
    *  Default constructor for Presentation entity property.
    *
    */
	CNtlPLProperty() {m_uiId = 0xffffffff; m_uiFlag = 0;}

	/**
    *  Virtual destructor for Presentation entity property.
    *
    */
	virtual ~CNtlPLProperty() {}

	/**
	*  prosentation entity property id setting.
	*  \param pName property id.
	*  \see GetId
	*/
	void SetId(const RwUInt32 uiId);

	/**
	*  prosentation entity property name setting.
	*  \return Returns property id.
	*  \see SetId
	*/
	RwUInt32 GetId(void) const;

	/**
	*  prosentation entity property name setting.
	*  \param pName property name.
	*  \see GetName
	*/
	void SetName(const RwChar *pName);

	/**
	*  Function to obtain the prosentation entity property name.
	*  \return Returns property name string.
	*  \see SetName
	*/
	const RwChar* GetName(void);

	/**
	*  prosentation entity property class id setting.
	*  \return property name을 리턴한다.
	*  \see GetClassID
	*/
	void SetClassID(const RwChar *pClassID);

	/**
	*  Function to obtain the prosentation entity property class id.
	*  \return property class id string.
	*  \see SetClassID
	*/
	const RwChar* GetClassID(void);

	///**
	//*  property data load function.
	//*  \return TRUE if success, FALSE if failure
	//*  \param pNode XML doc node
	//*  \see Save
	//*/

	virtual RwBool Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) = 0;

	///**
	//*  property data save function.
	//*  \return TRUE if success, FALSE if failure
	//*  \param pNode XML doc node
	//*  \see Load
	//*/
	virtual RwBool Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) = 0;

	virtual void	SaveSerialize(CNtlSerializer& sOut) {}											///< Function to save data in Serialize
	virtual void	LoadSerialize(CNtlSerializer& sIn) {}											///< Function to retrieve data from Serialize

    virtual RwUInt32 GetFlag() {return m_uiFlag;}
    virtual void     SetFlag(RwUInt32 uiFlag) {m_uiFlag = uiFlag;}
};

inline void CNtlPLProperty::SetId(const RwUInt32 uiId)
{
	m_uiId = uiId;
}

inline RwUInt32 CNtlPLProperty::GetId(void) const
{
	return m_uiId;
}

inline void CNtlPLProperty::SetName(const RwChar *pName)
{
	m_strName = pName;
}

inline const RwChar* CNtlPLProperty::GetName(void)
{
	return m_strName.c_str(); 
}

inline void CNtlPLProperty::SetClassID(const RwChar *pClassID)
{
	m_strClassID = pClassID;
}

inline const RwChar* CNtlPLProperty::GetClassID(void) 
{
	return m_strClassID.c_str(); 
}


#endif