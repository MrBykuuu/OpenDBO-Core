#pragma once

#include "NtlInstanceImVertexSystem.h"
#include "NtlResourceLineSystem.h"

/// A structure containing information that constitutes the plane of the LineSystem
struct SLineSystemVertex
{
    RwV3d vStrartPoint;     ///< Location of starting point                 
    RwV3d vUpdatePoint;     ///< Location of the updated endpoint
    RwV3d vDir;             ///< Direction moving forward
    RwIm3DVertex imVertices[6]; ///< Pointers to the 6 vertices that make up the line
    RwReal fLifeTime;       ///< Life Time of Line
    RwV2d vLineSize;        ///< Size of the line (required for Size Action)
};

typedef std::list<SLineSystemVertex*> ListLineSystemVertex;

/**
 * \ingroup NtlPresentation
 * \brief Line Effect Instance Class
 * \date 2006-09-21
 * \author agebreak
 */
class CNtlInstanceLineSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceLineSystem(void);
    virtual ~CNtlInstanceLineSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();    

    RwInt32 GetPolyCount() {return (int)m_listLineSystemVertex.size();} ///< Returns the current number of polygons.

protected:
    virtual void Init();
    virtual void Reset();    
    virtual void UpdateColor(RwReal fDeltaTime);
    virtual void UpdateSize(RwReal fDeltaTime);

    void UpdateVertices(RwReal fElapsedTime);
    void BuildEmitterStandard();
    void CreateLine();                                              ///< Create a new line.
    void CreatePlane(SLineSystemVertex* pLineSystemVertex, const RwV3d& vStartAxis, const RwV3d& vEndAxis); ///< Create a line by creating a plane using two points of information.
    void UpdateZBillBoard(SLineSystemVertex* pLineSystemVertex);    ///< Create a surface with a z-axis billboard.    
    void UpdateNoneZBillBoard(SLineSystemVertex* pLineSystemVertex);///< Create a side to which the Z-axis billboard is not applied.
    void UpdateShake(SLineSystemVertex* pLineSystemVertex);         ///< Apply Shake.    
    void UpdateRotate(RwReal fDeltaTime);                           ///< Apply Rotate.
    
protected:
    CNtlResourceLineSystem*     m_pResourceLineSystem;          ///< Line resource class containing creation information
    ListLineSystemVertex        m_listLineSystemVertex;         ///< List of faces that make up the LineSystem
    SLineSystemVertex*          m_poolLineSystemVertex;         ///< Pointer to LineSystem Vertex Memory Pool
    RwInt32                     m_nPoolIndex;                   ///< Index of Memory Pool
    RwReal                      m_fCreateGapTime;               ///< Variable for comparing generation cycles
    RwInt32                     m_nRoateSeed;                   ///< Seed value to be used for Rotate Bias               
};
