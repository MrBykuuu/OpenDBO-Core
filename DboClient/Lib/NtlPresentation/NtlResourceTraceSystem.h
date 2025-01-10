#pragma once

#include "NtlResourceImVertexSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief Resource class for creating trace effects
 * \date 2006-09-14
 * \author agebreak
 */
class CNtlResourceTraceSystem : public CNtlResourceImVertexSystem
{
public:
    CNtlResourceTraceSystem(const RwChar* strName);
    virtual ~CNtlResourceTraceSystem(void);

    virtual void Destroy();

    virtual RwBool Load(FILE* pFile);           ///< Loads data from a file.

public:
    // properties properties
    RwInt32 m_nMaxFrameCount;                   ///< Maximum number of frames to draw trajectories. To be exact, the number of sides. Vertices are created with twice this value. (Up and down pair). Up and down lines are drawn one by one in each frame.

protected:

protected:

};
