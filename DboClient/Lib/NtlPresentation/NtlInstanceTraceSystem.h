#pragma once

#include <list>
#include "NtlInstanceImVertexSystem.h"
#include "NtlAnimEventData.h"

#define WEAPON_BONE_NAME_1      "TracePoint1"
#define WEAPON_BONE_NAME_2      "TracePoint2"

// Vertex structure used for trajectory
struct STraceEdge
{
    RwReal          fLifeTime;       ///< Edge's life time
    RwIm3DVertex    vVertices[2];    ///< Two vertices that make up an edge    
};

typedef std::list<STraceEdge*> ListTraceEdge;

/**
 * \ingroup NtlPresentation
 * \brief Trace effect instance class
 * \date 2006-09-14
 * \author agebreak
 */
class CNtlInstanceTraceSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceTraceSystem(void);
    virtual ~CNtlInstanceTraceSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(SEventTrace* pEventTrace, CNtlPLAttach* pAttach);        ///< Create an effect with a TraceEvent object.
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();

protected:
    virtual void Init();
    virtual void Reset();
    
    void    SetEdgePoint(CNtlPLAttach* pAttach);                  ///< Set the position of the vertex where the edge will be taken.
    virtual RwBool  UpdateVertices(RwReal fElapsedTime);          ///< Update the information of vertices.
    virtual void    CreateEdge();                                         ///< Add a new edge.
    void    CreateSplinePath();                                   ///< Create Spline Path Points and place them in the vertex buffer.

    virtual void UpdateUV();                                      ///< Update UV value.
    virtual void UpdateColor();                                   ///< Update the color value.

private:
    // This Create function is not used.

    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld) {return TRUE;}

protected:    
    SEventTrace*    m_pEventTrace;              ///< Event object containing trajectory information
    CNtlPLAttach*   m_pAttach;                  ///< Character or Item to attach the trajectory to

    RwV3d           m_vStartPoint;              ///< Starting point of the trajectory
    RwV3d           m_vEndPoint;                ///< End point of the trajectory

    ListTraceEdge   m_listTraceEdge;            ///< List of edges that make up the current trajectory
    ListTraceEdge   m_listControlPoint;         ///< List of ControlPoints
    STraceEdge*     m_poolTraceEdge;            ///< Trace Edge structure Pool (becomes Control Point in Spline)
    STraceEdge*     m_poolControlPoint;         ///< Structure Pool that constitutes the actual spline to be drawn
    RwInt32         m_nPoolIndex;               ///< Index to be used in Pool
    RwInt32         m_nControlPointIndex;       ///< SplineÀÇ Pool Index
    RwReal          m_fEdgeGapTemp;             ///< Variable for checking Edge Gap    
};

typedef std::list<CNtlInstanceTraceSystem*> ListTraceSystem;
