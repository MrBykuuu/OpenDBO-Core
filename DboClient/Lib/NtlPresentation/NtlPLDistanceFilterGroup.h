/*****************************************************************************
*
* File			: NtlPLDistanceFilterGroup.h
* Author		: HyungSuk, Jang
* Copyright	    : (?)NTL
* Date			: 2006. 12. 12	
* Abstract		: Presentation distance filtering group 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLDISTANCEFILTER_GROUP_H__
#define __NTL_PLDISTANCEFILTER_GROUP_H__

class CNtlPLEntity;

/**
* \ingroup NtlPresentation
*An object that manages the same type of entity as a group.
*Use STL associative array map.
*
*/

class CNtlPLDistanceFilterGroup
{
public:

	enum { MAX_DIST_FILTER_GROUP = 200 };

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapSet;
	typedef std::list<MapSet*> ListGroup;


private:

	RwBool					m_bFirst;
	ListGroup::iterator		m_itCurr;
	ListGroup				m_listGroup;

	MapSet*		AllocateSet(void);
	void		DeAllocateSet(MapSet *pSet);
	MapSet*		FindInsert(void);

public:

	/**
	* constructor for Presentation distance filtering group
	*
	*/

	CNtlPLDistanceFilterGroup();

	/**
	* destructor for Presentation distance filtering group
	*
	*/

	~CNtlPLDistanceFilterGroup();

	/**
	*Function called after the distance filtering group object is created.
	*  \see Destroy
	*/
	RwBool Create(void);

	/**
	*Function called before the distance filtering group object is deleted.
	*  \see Create
	*/
	void Destroy(void);

	/**
	*Add entity to distance filtering group.
	*  \see RemoveEntity
	*/
	void AddEntity(CNtlPLEntity *pPLEntity);

	/**
	*Remove entity from distance filtering group.
	*  \see RemoveEntity
	*/
	void RemoveEntity(CNtlPLEntity *pPLEntity);

	/**
	*Get the next update filter.
	*/
	const MapSet* GetNextUpdateSet(void);

	/**
	*Get the distance filtering group.
	*/
	const ListGroup& GetEntities(void) const;

	/**
	*Number of distance filtering sets.
	*/
	RwInt32 GetFilterSetCount(void);

	/**
	*Total number of entities included in the distance filtering group.
	*/
	RwInt32 GetEntityCount(void);
};

#endif
