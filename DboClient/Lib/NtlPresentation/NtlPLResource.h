/*****************************************************************************
 *
 * File			: NtlPLResouce.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 7. 21	
 * Abstract		: Presentation layer resource base class
 *****************************************************************************
 * Desc         : 
 * Adjust		: HuunSuk, Jang(2005. 7 29)
 *				    - Added dictionary type variable/function.
 *
 *****************************************************************************/

#ifndef __NTL_PLRESOURCE_H__
#define __NTL_PLRESOURCE_H__

#include <list>
#include <rwcore.h>
#include <rpworld.h>
#include <rtanim.h> 

/**
 * \ingroup NtlPresentation
 * RenderWare resource wrapping class.
 *In RenderWare, clump and atomic data can be cloned. 
 *For consistent management of resources, a reference count was used when data with the same name was used. 
 *In the case of a clone resource, the method of linking CNtlPLResource to the original CNtlPLResource as a clone list was selected.
 */

class CNtlPLResource
{
private:

	std::string m_strName;					/**< String describing the resource name */
	unsigned int m_uiType;					/**< id describing the resource type */
	const void *m_pData;					/**< Pointer to resource */
    unsigned int m_uiRefCount;				/**< Reference count used to lock the resource */
	bool m_bClone;

public:
      
	  CNtlPLResource(void) { m_uiType = 0; m_uiRefCount = 0; m_pData = NULL; m_bClone = false; }
	  ~CNtlPLResource(void) {} 
	  
      /**
      *  Set the name of the resource, managed by this CResource object.
      *
      *  \param pName A string containing the name of the resource.
      */
	  void SetName(const char *pName) { m_strName = pName; }

      /**
      *  Set the type of the resource, managed by this CResource object.
      *
      *   \param pType A unsigned int containing the type of the resource.
      */
      void SetType(unsigned int uiType)  {  m_uiType = uiType; }
     
	  /**
      *  Set resource pointer to the resource, managed by this CResource object.
      *
      *   \param bClone renderware resource clone flag
      */
	  void SetClone(bool bClone) { m_bClone = bClone; }

      /**
      *  Set resource pointer to the resource, managed by this CResource object.
      *
      *   \param pResource A pointer to the resource
      */
      void SetData(const void *pData) { m_pData = pData; }
          
      /**
      *  refcount of the resource, increment.
      */
      void AddRef(void) { m_uiRefCount++; }

	  /**
      *  refcount of the resource, decrement.
      */
	  void ReleaseRef(void) { if(m_uiRefCount > 0) m_uiRefCount--; }
      
      /**
      *  Get the name of the resource, managed by this CResource object.
      *
      *   \return A string containing the type of the resource.
      */
      const char* GetName(void) const {return m_strName.c_str(); }

      /**
      *  Get the type of the resource, managed by this CResource object.
      *
      *   \return A unsigned int containing the type of the resource.
      */
      unsigned int GetType(void) const {return m_uiType;}

       /**
      *  Get the type of the resource, managed by this CResource object.
      *
      *   \return clone flag the renderware resource.
      */

	  bool GetClone(void) const { return m_bClone; }

      /**
      *  Get resource pointer to the resource, managed by this CResource object.
      *
      *   \return A pointer to the resource
      */
      const void* GetData(void) const {return m_pData;}
      
      /**
      *  Get the lock refcount of the resource, managed by this CResource object.
      *
      *   \return The lock refcount of this resource
      */
      unsigned int GetRefCount(void) const {return m_uiRefCount;}

	  /**
      *  Casting to RpWorld pointer.
      */
	  RpWorld* GetWorld(void) { return reinterpret_cast<RpWorld*>(const_cast<void*>(m_pData)); }

  	  /**
      *  Casting to RpClump pointer.
      */
	  RpClump* GetClump(void) { return reinterpret_cast<RpClump*>(const_cast<void*>(m_pData)); }

	  /**
      *  Cast to RpAtomic pointer.
      */
	  RpAtomic* GetAtomic(void) { return reinterpret_cast<RpAtomic*>(const_cast<void*>(m_pData)); }

	  /**
      *  Cast to RtAnimAnimation pointer.
      */
	  RtAnimAnimation* GetAnimation(void) { return reinterpret_cast<RtAnimAnimation*>(const_cast<void*>(m_pData)); }
};


#endif
