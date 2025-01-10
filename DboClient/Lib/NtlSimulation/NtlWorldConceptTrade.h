#pragma once

#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief Control class for private trades (currently controls nothing)
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConceptTrade : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptTrade(void);
	~CNtlWorldConceptTrade(void);

	virtual void		ChangeState( RwInt32 eState );

	VOID				SetOtherPCHandle(SERIAL_HANDLE hHandle);
	SERIAL_HANDLE		GetOtherPCHandler();

protected:
	SERIAL_HANDLE		m_hOtherPC;
};

inline VOID CNtlWorldConceptTrade::SetOtherPCHandle(SERIAL_HANDLE hHandle)
{
	m_hOtherPC = hHandle;
}

inline SERIAL_HANDLE CNtlWorldConceptTrade::GetOtherPCHandler()
{
	return m_hOtherPC;
}