#pragma once

#include "NtlWorldConceptController.h"

class CNtlPLCharacter;
class CNtlPLEntity;

/**
 * \ingroup NtlSimulation
 * \brief Control class when playing PVP 
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConceptPVP : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptPVP(void);
	~CNtlWorldConceptPVP(void);

	virtual void	Update(RwReal fElapsedTime);	
	virtual void	ChangeState(RwInt32 iState);

	//----PVP related methods
	void				SetFreePvPTarget(SERIAL_HANDLE hTarget);					///< Set the PVP target.
	SERIAL_HANDLE		GetFreePvPTarget(void) const;								///< Returns the Serail ID of the currently set PVP Target.

	void				SetFreePvPReferPos(RwV3d vPos);
	RwV3d				GetFreePvPReferPos(void) const;

	void				CreateFreePvPMark(void);							///< Create a PVP mark (currently a referee).
	void				DestroyFreePvPMark(void);							///< Remove the PVP mark.

	void				SetPvpName(SERIAL_HANDLE hTarget);

protected:
	/// PVP-related information structure
	struct FreeBattleInfo
	{
		CNtlPLCharacter		*pCharacter;
        CNtlPLEntity*       pBoundEffect;
		SERIAL_HANDLE		hFreePvPTarget;
		RwV3d				vFreePvPPos;
	};

protected:
	FreeBattleInfo		m_FreeBattleInfo;		///< PVP-related structures
};


// inline functions

inline void	CNtlWorldConceptPVP::SetFreePvPTarget(SERIAL_HANDLE hTarget)
{
	m_FreeBattleInfo.hFreePvPTarget = hTarget;
}

inline SERIAL_HANDLE CNtlWorldConceptPVP::GetFreePvPTarget(void) const
{
	return m_FreeBattleInfo.hFreePvPTarget;
}

inline void	CNtlWorldConceptPVP::SetFreePvPReferPos(RwV3d vPos)
{
	m_FreeBattleInfo.vFreePvPPos = vPos;
}

inline RwV3d CNtlWorldConceptPVP::GetFreePvPReferPos(void) const
{
	return m_FreeBattleInfo.vFreePvPPos;
}