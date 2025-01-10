/*****************************************************************************
*
* File			: NtlPLPropertyContaier.h
* Author		: HyungSuk, Jang
* Copyright	: (?)NTL
* Date			: 2005. 8. 09	
* Abstract		: Presentation object property container class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_PLPROPERTY_CONTAINER_H__
#define __NTL_PLPROPERTY_CONTAINER_H__

#include "NtlWorldCommon.h"
#include "NtlPLDef.h"
#include "NtlFileSerializer.h"

#pragma warning(disable : 4996)

class CNtlPLProperty;

/**
* \ingroup NtlPresentation
*This is a conatiner class that manages all properties used in presentation.\
*This object is a singleton object. Use the GetInstance() function.
*
*/
class CNtlPLPropertyContainer
{
public:

	typedef std::map<std::string, CNtlPLProperty*> MapProp;

private:

	static CNtlPLPropertyContainer* m_pInstance;

protected:

	MapProp m_mapSky;			/* sky property **/
	MapProp m_mapNtlWorld;		/* woody1019 **/
	MapProp m_mapSound;			/* woody1019 **/
	MapProp m_mapShoreLine;		/* woody1019 **/
	MapProp m_mapWater;			
	MapProp m_mapObject;		/* object property **/
	MapProp m_mapCharacter;		/* character property **/
	MapProp m_mapEffect;		/* effect property **/	
	MapProp m_mapSpline;		/* by agebreak **/	
	MapProp m_mapGui;
	MapProp m_mapItem;          /* by hodong **/
	MapProp m_mapPlant;
	MapProp m_mapWeather;

public:

	/**
	*  Add property to container.
	*  \param eType entity type.
	*  \param pProp Property pointer to add.
	*/
	// by agebreak
	virtual RwBool AddProp(ENtlPLEntityType eType, CNtlPLProperty *pProp);

protected:

	// Woody1019
	RwBool	LoadNtlWorld(const RwChar *pFileName);
	RwBool	LoadWater(const RwChar *pFileName);
	RwBool	LoadShoreLine(const RwChar *pFileName);
	RwBool	LoadSound(const RwChar *pFileName);

	/**
	*Function that loads effect properties.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*  \see LoadObject
	*  \see LoadCharacter
	*/
	RwBool	LoadEffect(const RwChar *pFileName);

	/**
	*Function that loads effect properties.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*  \see LoadObject
	*  \see LoadCharacter
	*  \see LoadEffect
	*/
	RwBool	LoadGui(const RwChar *pFileName);

	/**
	*Function to load plant properties.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*/
	RwBool	LoadPlant(const RwChar *pFileName);

	/**
	*Function that loads weather properties.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*/
	RwBool	LoadWeather(const RwChar *pFileName, RwUInt32 uiLevel);

	/**
	*Function to load field bgm/env/share property.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*/
	RwBool	LoadSoundField(const RwChar *pFileName, const RwChar* pName);

	// Serialize save/load related methods
	RwBool	SaveCharacterSerialize(CNtlFileSerializer& sl);
	RwBool	LoadCharacterSerialize(CNtlFileSerializer& sl);

	RwBool	SaveObjectSerialzie(CNtlFileSerializer& sl);
	RwBool	LoadObjectSerialize(CNtlFileSerializer& sl);

	RwBool	SaveItemSerialize(CNtlFileSerializer& sl);
	RwBool	LoadItemSerialize(CNtlFileSerializer& sl);

public:

	/**
	*  Default constructor for Presentation property container
	*
	*/
	CNtlPLPropertyContainer();

	/**
	*  destructor for Presentation property container
	*
	*/
	virtual ~CNtlPLPropertyContainer();

	/**
	*  A function called after a property container object is created.
	*  \see Destroy
	*/
	virtual RwBool Create(void);

	/**
	*  A function called after a property container object is created.
	*  \see Destroy
	*/
	virtual void Destroy(void);

	/**
	*  property container instance function.
	*  Use this function to access member functions/variables.
	*  \see Destroy
	*/
	static CNtlPLPropertyContainer* GetInstance(void);

	/**
	*  Function that loads all property files used for presentation.
	*  \param pFileName is filename.
	*/
	virtual RwBool Load();

	/**
	*  A function that loads object properties.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*  \see LoadCharacter
	*  \see LoadEffect
	*/
	RwBool LoadObject(const RwChar *pFileName);

	/**
	*  Function that loads character property.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*  \see LoadObject
	*  \see LoadEffect
	*/
	RwBool LoadCharacter(const RwChar *pFileName);

	/**
	*  Function that loads item property.
	*  \param pFileName property file name.
	*  \return TRUE if successful, FALSE if failed.
	*  \see LoadObject
	*  \see LoadEffect
	*/
	RwBool LoadItem(const RwChar *pFileName);


	virtual RwBool LoadSerialize(RwBool bEncrypt = FALSE, RwChar* szCryptPassword = NULL);				///< Loads data converted to binary.
	virtual RwBool SaveSerialize(RwBool bEncrypt = FALSE, RwChar* szCryptPassword = NULL);				///< Save the data converted to binary.

	void		SaveObjectScript(std::string &strFileName);
	void		SaveCharacterScript(std::string &strFileName);
	void		SaveItemScript(std::string &strFileName);

	/**
	*  Function that loads all property files used for presentation.
	*  \param eType is entity type
	*  \param uiId property id to retrieve
	*  \return If successful, it is a property object pointer. If failed, it is NULL.
	*/
	CNtlPLProperty* GetProperty(ENtlPLEntityType eType, const RwChar *pKey);

	/**
	*Obtains the reference variable of the property container corresponding to the entity type.
	*  \param eType is entity type
	*  \param mapProps Reference variable to get property container.
	*/
	MapProp& Gets(ENtlPLEntityType eType);

#ifdef dNTL_WORLD_TOOL_MODE
	RwBool ReloadPlant();
	RwBool ReloadWeather();
#endif
};

#endif
