/*****************************************************************************
 * File			: NtlPLGuiGroup.h
 * Author		: Hong Sungbock
 * Copyright	: NTL Co., Ltd.
 * Date			: 2007. 12. 13
 * Abstract		: Create a PL group GUI group
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

class CNtlPLSceneChangerGui;
class CNtlPLProfilerGui;


class CNtlPLGuiGroup
{
public:
	CNtlPLGuiGroup();
	~CNtlPLGuiGroup();

	RwBool	Create(void);
	void	Destroy(void);

protected:
	CNtlPLSceneChangerGui*		m_pSceneChanger;
	CNtlPLProfilerGui*			m_pProfilerGui;
};
