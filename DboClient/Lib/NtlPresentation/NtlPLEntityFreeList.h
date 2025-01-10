/*****************************************************************************
 *
 * File			: NtlPLEntityFreeList.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer entity free list
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLENTITY_FREELIST_H__
#define __NTL_PLENTITY_FREELIST_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"

/**
 * \ingroup NtlPresentation
 *Entity memory pool class to prevent memory fragmentation.
 *Use the free list of renderware.
 *
 */

class CNtlPLEntityFreeList
{
private:

	static RwFreeList *m_pObjFreeList;			/* object memory pool **/
	static RwFreeList *m_pCharFreeList;			/* character memory pool **/
	// by agebreak
	static RwFreeList *m_pEffectFreeList;		/* effect memory pool **/
	// Woody1019
	static RwFreeList *m_pWorldFreeList;
	static RwFreeList *m_pSoundFreeList;
	static RwFreeList *m_pSoundBGMFreeList;
	// Jhssugi
	static RwFreeList *m_pDummyWorldFreeList;	/** dummy world free list */
	// by hodong
	static RwFreeList *m_pItemFreeList;
    static RwFreeList *m_pPlayerNameFreeList;
	static RwFreeList *m_pDamageBoxFreeList;
	static RwFreeList *m_pDecalFreeList;
	static RwFreeList *m_pSunFreeList;			// Sun free list by agebreak
	static RwFreeList *m_pPlanetFreeList;		// Addition of Planet freelist due to Sun free list issue
public:

	/**
	*A function that initializes an entity free list object.
	*  \see Destroy
	*/
	static RwBool AllocFreeList(void);

	/**
	*A function that terminates an entity free list object.
	*  \see Destroy
	*/
	static void DeAllocFreeList(void);

	/**
	*Allocate free list memory.
	*  \param eType entity type
	*  \return The requested free list memory if successful, NULL if failed.
	*  \see Free
	*/
	static void* Alloc(const ENtlPLEntityType eType);

	/**
	*Release free list memory.
	*  \param eType entity type
	*  \param pObj free list memory to release.
	*  \see Alloc
	*/
	static void Free(const ENtlPLEntityType eType, void *pObj);
};

#endif
