/*****************************************************************************
 *
 * File			: NtlPLEntityBlend.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2007. 11. 05	
 * Abstract		: Presentation object entity blend class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_BLEND_H__
#define __NTL_PLENTITY_BLEND_H__

class CNtlPLEntity;

// Class that manages weight information in Alpha Blend
class CNtlPLEntityAlphaWeightBlend
{
private:

	enum 
	{
		PLEAW_BLEND,                    // Blending
		PLEAW_END                       // Blended finished
	};

	RwUInt8	m_byBlendType;
	RwReal	m_fWeightValue;
	RwReal	m_fCurrTime;

	RwReal	m_fStartWeightValue;
	RwReal	m_fEndWeightValue;
	RwReal	m_fBlendTime;
	RwReal	m_fLifeTime;
	RwBool	m_bLoop;

	RwBool	m_bCalcWeightValue;
	RwBool	m_bFinish;

public:

	CNtlPLEntityAlphaWeightBlend();
	~CNtlPLEntityAlphaWeightBlend();

	virtual void Update(RwReal fElapsed);

	RwBool	IsCalcWeightValue(void);
	RwBool	IsFinish(void);

	void	SetWeight(RwReal fWeightValue, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
	void	SetWeight(RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);

	RwReal	GetWeight(void) const { return m_fWeightValue; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLAtomicAlphaWeightBlend : public CNtlPLEntityAlphaWeightBlend
{
private:

	std::string m_strName;

public:

	CNtlPLAtomicAlphaWeightBlend();
	~CNtlPLAtomicAlphaWeightBlend();

	void	Update(RwReal fElapsed);

	void	SetWeight(const RwChar *pAtomicName, RwReal fWeightValue, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
	void	SetWeight(const RwChar *pAtomicName, RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);

	std::string& GetAtomicName(void); 
};

inline std::string& CNtlPLAtomicAlphaWeightBlend::GetAtomicName(void)
{
	return m_strName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Controller that manages alpha blend weight objects
class CNtlPLEntityBlendController
{

public:
    CNtlPLEntityBlendController(CNtlPLEntity *pPLEntity);
    ~CNtlPLEntityBlendController();

    void Update(RwReal fElapsed);

    CNtlPLEntityAlphaWeightBlend*  AddAlpha(RwReal fWeightAlpha, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
    CNtlPLEntityAlphaWeightBlend*  AddAlpha(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);    

	CNtlPLAtomicAlphaWeightBlend*  AddAtomicAlpha(const RwChar *pAtomicName, RwReal fWeightAlpha, RwReal fLifeTime = 0.0f, RwBool bLoop = TRUE);
    CNtlPLAtomicAlphaWeightBlend*  AddAtomicAlpha(const RwChar *pAtomicName, RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop);    
    
    void AddAlphaBlend(CNtlPLEntityAlphaWeightBlend* pAlphaBlend);              ///< Adds an arbitrary alpha blending object to the list.
    void RemoveAlpha(CNtlPLEntityAlphaWeightBlend* pAlphaBlend);
	void RemoveAtomicAlpha(CNtlPLAtomicAlphaWeightBlend *pAtomicAlphaBlend);

    inline RwBool IsFinish(void);

protected:

    void	UpdateEntityAlpha(RwReal fElasped);
	void	UpdateAtomicAlpha(RwReal fElapsed);

	CNtlPLAtomicAlphaWeightBlend* FindAtomicAlphaBlend(const RwChar *pAtomicName);	

protected:

	typedef std::list<CNtlPLEntityAlphaWeightBlend*> ListAlphaWeightBlend;
	typedef std::list<CNtlPLAtomicAlphaWeightBlend*> ListAtomicAlphaWeightBlend;

	CNtlPLEntity				*m_pPLEntity;

	ListAlphaWeightBlend		m_listAlphaBlend;
	ListAtomicAlphaWeightBlend	m_listAtomicAlphaBlend;
};

inline RwBool CNtlPLEntityBlendController::IsFinish(void)
{
	if(m_listAlphaBlend.empty() && m_listAtomicAlphaBlend.empty())
		return TRUE;

	return FALSE;
}

#endif
