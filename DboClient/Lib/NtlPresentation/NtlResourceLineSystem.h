#pragma once

#include "NtlResourceImVertexSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief Resource class for creating line (thick line) effects.
 * \date 2006-09-21
 * \author agebreak
 */
class CNtlResourceLineSystem : public CNtlResourceImVertexSystem
{
public:
    CNtlResourceLineSystem(const RwChar* strName);
    virtual ~CNtlResourceLineSystem(void);

    virtual void Destroy();

    virtual RwBool Load(FILE* pFile);   ///<Load data from a file.

public:   
    SNtlLineEmitterStandard         m_EmitterStandard;          ///< Basic property values ??of Line System    
    
    SNtlPrtStdEmitterPrtSphere      m_EmitterSphere;            ///< Sphere Emitter
    SNtlPrtStdEmitterPrtRotate		m_EmitterPrtRotate;         ///< Rotate Emitter
    SNtlAdvMultiRotateEmitter		m_EmitterPrtMultiRotate;    ///< Multi Rotate

};
