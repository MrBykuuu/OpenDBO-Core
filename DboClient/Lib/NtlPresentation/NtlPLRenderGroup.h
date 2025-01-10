/*****************************************************************************
*
* File			: NtlPLSceneManager.h
* Author		: HyungSuk, Jang
* Copyright	: (?)NTL
* Date			: 2005. 8. 01	
* Abstract		: Presentation scene manager base class 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLRENDERGROUP_H__
#define __NTL_PLRENDERGROUP_H__

#include <rwcore.h>
#include <rpworld.h>
#include <map>
#include <list>

class CNtlPLEntity;

/**
* \ingroup NtlPresentation
*An object that manages the same type of entity as a group.
*Use STL associative array map.
*
*/

class CNtlPLRenderGroup
{
public:

	typedef std::map<RwUInt32, CNtlPLEntity*> MapEntity;

private:

	RwUInt32 m_uiLayer;
	MapEntity m_mapEntity;		/* entity map container **/

public:

	/**
	*  destructor for Presentation render group
	*
	*/

	~CNtlPLRenderGroup();

	/**
	*  A function called after the render group object is created.
	*  \see Destroy
	*/
	RwBool Create(void);

	/**
	*  A function called before the render group object is deleted.
	*  \see Create
	*/
	void Destroy(void);

	/**
	*  render group layer setting.
	*  \see GetLayer
	*/
	void SetLayer(RwUInt32 uiLayer);

	/**
	*  Get the render group layer.
	*  \see SetLayer
	*/
	RwUInt32 GetLayer(void) const;

	/**
	*  Add entity to group.
	*  \param pEntity is the entity pointer to add.
	*  \see RemoveEntity
	*/
	RwBool AddEntity(const CNtlPLEntity *pEntity);

	/**
	*  Remove entity from group.
	*  \param pEntity is the entity pointer to remove.
	*  \see AddEntity
	*/
	void RemoveEntity(CNtlPLEntity *pEntity);

	/**
	*Find the relevant entity in the group.
	*  \param pEntity is the entity pointer to find in the container.
	*  \return TRUE if successful, FALSE if failed
	*/
	RwBool FindEntity(const CNtlPLEntity *pEntity);

	/**
	*Find the number of entities in the group.
	*  \return Number of entities.
	*/
	RwInt32 GetEntityCount(void);

	/**
	*Returns a reference to the entity container corresponding to the group.
	*  \return entity container(m_mapEntity) Returns a reference.
	*/
	MapEntity* GetEntities(void);

	/**
	*Contains all entities corresponding to the group.
	*/
	void Gets(std::list<CNtlPLEntity*>& listEntities);
};

inline void CNtlPLRenderGroup::SetLayer(RwUInt32 uiLayer)
{
	m_uiLayer = uiLayer;
}

inline RwUInt32 CNtlPLRenderGroup::GetLayer(void) const
{
	return m_uiLayer;
}


inline CNtlPLRenderGroup::MapEntity* CNtlPLRenderGroup::GetEntities(void)
{
	return &m_mapEntity;
}


#endif