//***********************************************************************************
//	File		:	NtlResourceMeshSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	? 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourcecomponentsystem.h"

#include <string>

class CNtlResourceMeshSystem : public CNtlResourceComponentSystem
{
public:
	std::string						m_strMeshFileName;
	std::string						m_strAniFileName;
	std::string						m_strUvFileName;

	SNtlMeshEmitterStandard			m_EmitterStandard;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	SNtlPrtStdEmitterPrtSize		m_EmitterPrtSize;
	SNtlPrtStdEmitterPrtRotate		m_EmitterPrtRotate;

    SNtlPrtStdEmitterPrtFinish      m_EmitterPrtFinish;             ///< Properties used to produce Finish

	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	SNtlAdvMultiSizeEmitter			m_EmitterPrtMultiSize;
	SNtlAdvMultiRotateEmitter		m_EmitterPrtMultiRotate;

    SNtlPrtStdEmitterTrail          m_EmitterTrail;               ///< Trail Action related properties

public:
	CNtlResourceMeshSystem(const RwChar* strName);
	virtual ~CNtlResourceMeshSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
