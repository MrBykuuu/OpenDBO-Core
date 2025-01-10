//***********************************************************************************
//	File		:	NtlResourceImVertexSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	? 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourcecomponentsystem.h"

class CNtlResourceImVertexSystem : public CNtlResourceComponentSystem
{
public:

	std::string						m_strTextureName;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	SNtlPrtStdEmitterPrtSize		m_EmitterPrtSize;

	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	SNtlAdvMultiSizeEmitter			m_EmitterPrtMultiSize;

    SNtlPrtStdEmitterPrtFinish      m_EmitterPrtFinish;             ///< Properties used to direct Finish

public:
	CNtlResourceImVertexSystem(const RwChar* strName);
	virtual ~CNtlResourceImVertexSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
