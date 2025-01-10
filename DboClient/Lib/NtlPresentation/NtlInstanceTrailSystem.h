#pragma once

#include "NtlInstanceTraceSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief Effect class for expressing the tail of Particle and Mesh System. It is created by inheriting from TraceSystem.
 * \date 2006-12-29
 * \author agebreak
 */
class CNtlInstanceTrailSystem : public CNtlInstanceTraceSystem
{
public:
    CNtlInstanceTrailSystem(void);
    virtual ~CNtlInstanceTrailSystem(void);

    // new/delete override
    void* operator new(size_t size);
    void  operator delete(void* pObj);

    // Create/Delete override
    virtual RwBool  Create(CNtlInstanceComponentSystem* pComponentSystem, SNtlPrtStdEmitterTrail* pEmitterTrail);
    virtual void    Delete();

protected:
    virtual void Init();
    virtual void Reset();

    virtual void   CreateEdge();                                        ///< Add a new edge.
    void           SetEdgePoint();                                      ///< Set the edge location.

protected:
    SNtlPrtStdEmitterTrail*         m_pEmitterTrail;                    ///< Property object containing trail creation information
    CNtlInstanceComponentSystem*    m_pAttachComponent;                 ///< InstanceComponentSystem object to which the Trail will be attached
    
    RwV3d                           m_vPrevTrailPoint;                  ///< Trail Point calculated in the previous frame;
};
