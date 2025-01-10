/*****************************************************************************
 *
 * File			: NtlPLEntityFactory.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer entity factory class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_FACTORY_H__
#define __NTL_PLENTITY_FACTORY_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"
#include "NtlPLEntity.h"

/**
 * \ingroup NtlPresentation
 *Factory class responsible for creating and destroying entities.
 *The creation and destruction of all entities in the presentation uses the factory method.
 *
 */

class CNtlPLEntityFactory
{
public:

	/**
	*Function that creates an entity.
	*  \param pEntityName Entity name to be created.
	*\retrun Returns an entity pointer if creation is successful, or NULL if creation fails.
	*  \see DeleteEntity
	*/
	static CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const SPLEntityCreateParam* _pParam);

	/**
	*A function that destroys an entity.
	*  \param pEntity The entity pointer to be destroyed.
	*  \see CreateEntity
	*/
	static void DeleteEntity(CNtlPLEntity *pEntity);
};


#endif
