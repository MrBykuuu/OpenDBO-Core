//***********************************************************************************
//	File		:	NtlResourceLightningSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	? 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourceImVertexsystem.h"

/**
 * \ingroup NtlPresentation
 * \brief Resource class that generates Lighting
 * \date 2006-08-11
 * \author agebreak
 */
class CNtlResourceLightningSystem : public CNtlResourceImVertexSystem
{
public:
	SNtlLightningEmitterStandard		m_EmitterStandard;

public:
	CNtlResourceLightningSystem(const RwChar* strName);
	virtual ~CNtlResourceLightningSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
