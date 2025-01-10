/*****************************************************************************
*
* File			: NtlSLCENode.h
* Author		: agebreak
* Copyright		: NTL Co., Ltd.
* Date			: 2008. 7. 16	
*****************************************************************************
* Desc          : Node for directing the audience of the world's best Martial Arts
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"
#include "NtlCoreDescription.h"


#define     CENODE_VER_1			(RwReal)1
#define     CENODE_VER_1_1			(RwReal)1.1
#define     CENODE_VER_1_2			(RwReal)1.2
#define     CENODE_VER_1_2_1		(RwReal)1.21				///< Version of data for creating crowd effects
#define     CENODE_VER_LASTEST		(RwReal)1.3					///<Added referee narration

class CNtlSerializer;
class CNtlInstanceEffect;
class CNtlPLCharacter;


/// Node base class for audience production at the World's Best Martial Arts Festival
class CNtlSLCENode
{
public:
    enum ECENodeType            /// Node type
    {
        E_CE_EFFECT,        
        E_CE_SOUND,
        //And there's a balloon,
        E_CE_JUDGE,

		 NUM_E_CE,

		 INVALID_E_CE
    };

    static sVERSTION      m_Version;                     ///< Version information for backward compatibility

public:
    CNtlSLCENode(void);
    virtual ~CNtlSLCENode(void);

    virtual void    Create() = 0;
    virtual void    Destory() = 0;
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);   
	virtual void    SetPosition(const RwV3d& vPos) = 0;
	RwV3d			 GetPosition() { return m_vPos; }
	RwInt32			GetType() { return m_eType; }

    void            SetName(const std::string& strName) {m_strName = strName;}
    std::string     GetName() {return m_strName;}

	virtual void		HandleEvents(RWS::CMsg &pMsg) {}

protected:
    RwInt32                     m_eType;                    ///< Node type    
    std::string                 m_strName;                  ///<node name
    
    RwReal                      m_fTotalElapsedTime;        ///< Update time comparison variable
    RwV3d                       m_vPos;                     ///< Location where the node is created
};

/// Spectator Effect Node
class CNtlSLCENodeEffect : public CNtlSLCENode
{
public:
    CNtlSLCENodeEffect();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);    
    virtual void    SetPosition(const RwV3d& vPos);

    void            SetEffectName(const std::string& strName) {m_strEffectName = strName;}
    std::string     GetEffectName() {return m_strEffectName;}
    CNtlInstanceEffect* GetInstanceEffect() {return m_pEffect;}

protected:

protected:    
    std::string             m_strEffectName;                        ///< Name of the effect used

    CNtlInstanceEffect*     m_pEffect;                              ///< Created effect object
};

/// sound playback node
class CNtlSLCENodeSound : public CNtlSLCENode
{
public:
    CNtlSLCENodeSound();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);    
	virtual void    SetPosition(const RwV3d& vPos);
	void			  SetOffsetPos(const RwV3d& vPos) {m_vOffsetPos = vPos;}
	RwV3d			 GetOffsetPos() {return m_vOffsetPos;}

    void            SetSoundName(const std::string& strName) {m_strSoundFileName = strName;}
    std::string     GetSoundName() {return m_strSoundFileName;}
	void			  SetChannelGroup(RwUInt8 byChannelGroup) {m_byChannelGroup = byChannelGroup;}
	RwUInt8			 GetChannelGroup() {return m_byChannelGroup;}
    void            SetSoundLoop(RwBool bLoop) {m_bLoop = bLoop;}
    RwBool          GetSoundLoop() {return m_bLoop;}

protected:

protected:
    std::string         m_strSoundFileName;
	RwUInt8				  m_byChannelGroup;
    RwBool              m_bLoop;                                ///< Whether sound loop or not
	RwV3d           m_vOffsetPos;                                     ///< Offset position relative to the controller

    RwUInt32            m_hSoundHandle;
};
/*
/// Speech Bubble Node
class CNtlSLCENodeBalloon  : public CNtlSLCENode
{
public:
    

public:
    CNtlSLCENodeBalloon();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);   
    virtual void    SetPosition(const RwV3d& vPos);

    void            SetBallonRect(RwV3d& vStart, RwV3d& vEnd);                      ///< Sets the area of ??the speech bubble.
    RwV3d           GetBallonRectStart() {return m_vStartRect;}
    RwV3d           GetBallonRectEnd()   {return m_vEndRect;}

	void			  SetInstanceTime(RwReal fInstanceTime)	{m_fInstanceTime = fInstanceTime;}
	RwReal			  GetInstanceTime() {return m_fInstanceTime;}

	void			  SetLifeTime(RwReal fLifeTime)	{m_fLifeTime = fLifeTime;}
	RwReal			  GetLifeTime() {return m_fLifeTime;}

    void            SetTblIdx(RwUInt32 uiTblIdx) {m_uiTableIdx = uiTblIdx;}
    RwUInt32        GetTblIdx() {return m_uiTableIdx;}

protected:
    void            CreateBalloon();

protected:        
    RwV3d               m_vStartRect;                   ///< speech bubble starting area
    RwV3d               m_vEndRect;                     ///< end area of ??speech bubble
    RwReal              m_fInstanceTime;                ///< Frequency of speech bubble creation (in seconds)
    RwReal              m_fLifeTime;                    ///< creation time
    RwUInt32            m_uiTableIdx;

};
*/

class CNtlSobNpc;

/**
 * \ingroup NtlSimulation
 * \brief Judgment rendering node
 *
 * \date 2008-07-28
 * \author agebreak
 */

typedef std::vector<RwUInt32>		VEC_NARRATION_ID;
typedef std::vector<RwUInt32>		VEC_ANIM_ID;

class CNtlSLCENodeReferee : public CNtlSLCENode
{
public:
	enum eNarrationPlayType
	{
		NARRATION_PLAY_RANDOM,
		NARRATION_PLAY_SEQUENCE,
	};

    enum ERefreeMoveType
    {
        E_REF_MOVE_STOP,
        E_REF_MOVE_EDGE,
        E_REF_MOVE_TARGET,        
    };

	enum EAnimPlayType
	{
		ANIM_PLAY_TYPE_RANDOM,
		ANIM_PLAY_TYPE_SEQUENCE,
		ANIM_PLAY_TYPE_LOOP,
	};

	struct sNarrationInfo
	{
		eNarrationPlayType				ePlayType;
		RwReal							fElapsed;
		RwReal							fRemainTime;
		RwUInt8							byCurNarrationIndex;		

		VEC_NARRATION_ID				vecNarrationID;
	};

	struct sMoveInfo
	{
		ERefreeMoveType					eMoveType;
		EAnimPlayType					eAnimPlayType;		
		RwBool							bAnimStart;
		RwV3d							v3RefreePos;	
		RwV3d							vTargetPos;                            ///< Target location to move to
		RwV3d							vOrisinalPos;
		RwReal							fDelayTime;
		RwReal							fMoveSpeed;
		RwReal							fAngleY;

		VEC_ANIM_ID						vecAnimID; 
		VEC_ANIM_ID::iterator			itCurAnimID;
	};

public:
    CNtlSLCENodeReferee();
    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);
	virtual void	SetPosition(const RwV3d& vPos) {}
    
	void			SetRefreePosition(const RwV3d& vPos);
	RwV3d			GetRefreePosition() { return m_MoveInfo.v3RefreePos; }

	void			SetNarrationPlayType(RwUInt8 byNarrationPlayType);
	RwUInt8			GetNarrationPlayType() { return (RwUInt8)m_NarrationInfo.ePlayType; }

	void			AppendNarrationIndex(RwUInt32 uiNarrationIndex);
	RwInt32			GetCurNarrationIndex();
	void			RemoveNarrationIndex(RwUInt32 uiNarrationIndex);
	void			RemoveAllNarrationIndex();
	VEC_NARRATION_ID& GetNarrationList() { return m_NarrationInfo.vecNarrationID; }

	void			SetAngleY(const RwReal fAngleY);
	RwReal			GetAngleY(){ return m_MoveInfo.fAngleY; }

	void			SetAnimPlayType(RwUInt8 byAnimType);
	RwUInt8			GetAnimPlayType() { return (RwUInt8)m_MoveInfo.eAnimPlayType; }

	void			AppendAnimId(RwUInt32 uiAnimID);
	RwInt32			GetCurAnimId();
	void			RemoveAnimId(RwUInt32 uiAnimID);
	void			RemoveAllAnimId();
	VEC_ANIM_ID&	GetAnimList() { return m_MoveInfo.vecAnimID; }
	RwBool			IsExistAnimId(RwUInt32 uiAnimID);

	void			SetMoveType(ERefreeMoveType eType);
	ERefreeMoveType GetMoveType() {return m_MoveInfo.eMoveType;}

	void			SetMoveSpeed(RwReal fSpeed) { m_MoveInfo.fMoveSpeed = fSpeed; }
	RwReal			GetMoveSpeed() { return m_MoveInfo.fMoveSpeed; }

	void			SetTargetPos(const RwV3d& vPos) {m_MoveInfo.vTargetPos = vPos;}
	RwV3d			GetTargetPos() {return m_MoveInfo.vTargetPos;}
    
	RwBool			IsPlay() { return m_MoveInfo.bAnimStart; }

	virtual void	HandleEvents(RWS::CMsg &pMsg);

protected:    
    void			UpdateMove(RwReal fElapsedTime, ERefreeMoveType eType);         
	void			UpdateNarration(RwReal fElapsedTime);

	void			SetNextNarration();
	RwBool			SetNextAnim();

protected:	
	sNarrationInfo		m_NarrationInfo;
	sMoveInfo			m_MoveInfo;

protected:    
	CNtlSobNpc*         m_pRefreeActor;                                      ///< Judgment Actor object pointer
};