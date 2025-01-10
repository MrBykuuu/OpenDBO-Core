#pragma once

#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief Control class when using a personal store (currently does not control anything)
 * \date 2007-07-11
 * \author Cz
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShop : seller
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNtlWorldConceptPrivateShop : public CNtlWorldConceptController
{
	RwUInt32 m_uiPrivateShopState; // E privateshop state

public:
	CNtlWorldConceptPrivateShop(void);
	~CNtlWorldConceptPrivateShop(void);

	virtual RwBool		IsEnableAction(EAvatarAction eAction);
	
	virtual RwUInt32	GetPrivateShopState();
	virtual void		SetPrivateShopState(RwUInt32 uiPrivateShopState);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldConceptPrivateShopVisitor : buyer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNtlWorldConceptPrivateShopVisitor : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptPrivateShopVisitor(void);
	~CNtlWorldConceptPrivateShopVisitor(void);

	virtual void	ChangeState(RwInt32 iState);				
	virtual RwBool	IsEnableAction(EAvatarAction eAction);
};
