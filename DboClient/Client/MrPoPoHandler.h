#pragma once

#include "NtlMrPoPoMsg.h"

/**
* \ingroup Client
* \brief Class that handles events sent from the client support tool (Mr.PoPo)
*
* \date 2008-08-01
* \author agebreak
*/
class CMrPoPoHandler
{
public:
    CMrPoPoHandler(void);
    ~CMrPoPoHandler(void);

    void HandleEvent(RwInt32 nMsg, DWORD param);

protected:
    DWORD           OnSetLevel(RwInt32 nLevel);
    void            OnCollectDragonBall();
    void            OnTestCrowd(RwInt32 nCrowdType);
    std::wstring    OnTeleport(RwInt32 nIndex);
    std::wstring    OnTeleport(RwInt32 nMsg, RwInt32 nVal);        
    void            OnUpgradeAllSkill();    
    void            OnSuperMode();
    void            OnNamekTest();
    void            OnTenkaichiMark(RwInt32 nType);
    void            OnChangeColor(RwInt32 nType);
    void            OnChangeState(RwUInt32 uiStateID);
    void            OnTransform(RwUInt32 uiTransformID);
    void            OnStun(RwUInt32 uiStunType);
    void            OnTargetMarking(RwBool bSelect);
    void            OnPushTest();

    // Events related to graphics settings
    void            OnTerrainDist(RwUInt32 uiDist);
    void            OnTerrainShadow(RwBool bEnable);
    void            OnWaterSpecular(RwBool bEnable);
	void            OnCharDist(RwUInt32 uiDist);
    void            OnCharEdge(RwBool bEnable);
	void            OnCharGrayColor(RwBool bEnable);
    void            OnLowEffect(RwBool bEnable);
	void            OnParticleRatio(RwUInt32 uiRatio);
	void            OnMeshEffect(RwBool bEnable);
	void            OnDecalEffect(RwBool bEnable);

protected:
    RwInt32         m_nTemp;

};

static CMrPoPoHandler* GetMrPoPoHandler()
{
    static CMrPoPoHandler mrPoPoHandler;

    return &mrPoPoHandler;
}
