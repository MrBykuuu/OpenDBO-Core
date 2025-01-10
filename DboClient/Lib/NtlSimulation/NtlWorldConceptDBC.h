#pragma once

#include "NtlWorldConceptController.h"
#include "NtlSoundDefines.h"
#include "NtlDragonBall.h"

class CNtlPLCharacter;
class CNtlSob;
class CNtlSobItem;

typedef std::list<CNtlSobItem*>	ListSobItem;

/**
 * \ingroup NtlSimulation
 * \brief Controller to control dragon god summoning and dragon ball reward status
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConceptDBC : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptDBC(void);
	~CNtlWorldConceptDBC(void);

	virtual void		Update(RwReal fElapsedTime);				///< Update processing function for contents suitable for each concept
	virtual RwBool		IsEnableAction(EAvatarAction eAction);		///< Determine whether the Avatar's Action can currently be processed according to each concept.
	virtual void		ChangeState(RwInt32 iState);				///< State Changes the state.	

    // DragonBall List	
	void				AddDragonBall(CNtlSobItem* pItem);			///< Add a Dragon Ball item to the list.
	void				RemoveDragonBall(CNtlSobItem* pItem);		///< Remove the Dragon Ball item from the list.
	ListSobItem*		GetDBList() {return &m_listDragonBall;}

    // Property
    void				SetDBKind(eDRAGON_BALL_TYPE eDBKind) {m_eDragonBallKind = eDBKind;}	///< Set the type of Dragon Ball currently in progress.
    eDRAGON_BALL_TYPE	GetDBKind() {return m_eDragonBallKind;}		                        ///< Returns the type of Dragon Ball currently in progress.			

    void				SetAltarSerialID(RwUInt32 nSerialID) {m_uiAltarID = nSerialID;}
    RwUInt32			GetAltarSerialID() {return m_uiAltarID;}

    void				ResetRemainTime();							///< Set the remaining time limit.
    RwUInt32			GetReaminTime();	                        ///< Gets the remaining time limit.

    void                SetMyDragon(RwBool bMyDragon) {m_bMyDragon = bMyDragon;}    ///< Set whether it is a dragon god you have summoned.
    RwBool              IsMyDragon() {return m_bMyDragon;}                          ///< Returns whether it is a dragon god summoned for you.

protected:	

protected:
	RwUInt32				m_uiAltarID;							///< Serial ID of the altar
	eDRAGON_BALL_TYPE		m_eDragonBallKind;						///< Types of Dragon Ball production currently in progress
	RwUInt32				m_uiRemainTime;							///< Remaining time limit
	ListSobItem				m_listDragonBall;						///< Dragon Ball items
    RwBool                  m_bMyDragon;                            ///< Flag to check whether it is the dragon god you have summoned
    RwBool                  m_bUpdateRemainTime;                    ///< Time-limited update presence flag
    SOUND_HANDLE            m_hBGMHandle;                           ///< The handle of the sound that comes out of the Dragon God
};
