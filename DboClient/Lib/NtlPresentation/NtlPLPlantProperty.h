#ifndef __NTL_PLPLANT_PROPERTY_H__
#define __NTL_PLPLANT_PROPERTY_H__

#include "NtlPLProperty.h"

/**
 * \ingroup NtlPresentation
 *This is a property class corresponding to CNtlPLPlant.
 *Property data is written as an xml file.
 *
 */

class CNtlPLPlantProperty : public CNtlPLProperty
{
public:
	struct sPLANT_PROP
	{
		std::string					strName;
		RwInt32						iNumMin;
		RwInt32						iNumMax;
		RwReal						fScaleMin;
		RwReal						fScaleMax;
		RwBool						bIsRotationTerrain;
		std::vector<std::string>	vecstrResourceName;
	};

public:
	CNtlPLPlantProperty();
	virtual ~CNtlPLPlantProperty();

	virtual RwBool	Load( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
	virtual RwBool	Save( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

	void			SetResourceFilePath( const RwChar * pName ) { m_strResourceFilePath = pName; }
	const RwChar *	GetResourceFilePath( void ) const { return m_strResourceFilePath.c_str(); }

	void			SetTextureFilePath( const RwChar * pName ) { m_strTextureFilePath = pName; }
	const RwChar *	GetTextureFilePath( void ) const { return m_strTextureFilePath.c_str(); }

	void			SetClipDistance(RwReal fDist) { m_fClipDistance = fDist; }
	const RwReal&	GetClipDistance() const { return m_fClipDistance; }

public:
	typedef std::map<RwInt32, sPLANT_PROP>	PROPERTY_MAP;
	typedef PROPERTY_MAP::iterator			PROPERTY_MAP_IT;
	PROPERTY_MAP							m_mapProperty;

protected:
	std::string						m_strResourceFilePath;
	std::string						m_strTextureFilePath;
	RwReal							m_fClipDistance;
};

#endif