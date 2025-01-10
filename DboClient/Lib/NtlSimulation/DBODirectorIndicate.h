/*****************************************************************************
*
* File			: DBODirectorIndicate
* Author		: agebreak
* Copyright	: (аж)NTL
* Date			: 2007. 11. 20	
* Abstract		: 
*****************************************************************************
* Desc         : Direction system implementation class used in TS/TMQ
*
*****************************************************************************/
#pragma once

#include "ceventhandler.h"
#include "NtlSLGlobal.h"
#include "NtlPLFadeController.h"

class CNtlPLCharacter;
class CNtlInstanceEffect;

///Direction system implementation class used in TS/TMQ
class CDBODirectorIndicate
{
public:
    CDBODirectorIndicate(CNtlPLCharacter* pPLCharacter);
    ~CDBODirectorIndicate(void);

    RwBool Create();
    VOID   Destory();
    RwBool Update(RwReal fElapsedTime);
    VOID   HandleEvents(RWS::CMsg &pMsg);
    VOID   SetCharacter(CNtlPLCharacter* pPLCharacter) {m_pCharacter = pPLCharacter;}

protected:
    RwBool      CreateDirector();        ///< Creates a direction indicator object.
    RwBool      CreateTargetEffect();    ///< Creates a targeting effect.     
    RwBool      CalcDistance();          ///< Calculate the distance.
    VOID        UpdateDirection();       ///< Updates the direction of the object.
    VOID        SetVisibleEffect(RwBool bVisible);  ///< Sets the presence or absence of expression of effects.
    VOID        SetTargetEffectPos(RwV3d vPos);    ///< Target Positions the effect at the target location.

protected:
    CNtlPLCharacter*                    m_pCharacter;             ///< Player Character
    CNtlInstanceEffect*                 m_pDirectorEffect;        ///< Effect to display direction indication
    CNtlInstanceEffect*                 m_pTargetEffect;          ///< Effect representing the target           
    RwV3d                               m_vTargetPos;             ///< location of target
    RwBool                              m_bUpdate;                ///< Update presence flag
};
