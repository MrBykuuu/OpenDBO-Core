/*****************************************************************************
 *
 * File			: NtlDTNode.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 02	
 * Abstract		: direct node
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_DTNODE_H__
#define __NTL_DTNODE_H__

#include "NtlDTProp.h"

class CNtlSob;

enum EDirectLookAtType	
{
	DIRECT_LOOKAT_NOT_USE,

	//The camera's look at is determined by spline data, and is not transformed at all. 
	DIRECT_LOOKAT_SELF,			

	// The camera's look at is determined by spline data, 
	// When production begins, rotate the spline data to all axes in the direction input from the outside.
	DIRECT_LOOKAT_SELF_ALLAXIS,		

	// The camera's look at is determined by spline data, 
	// When production begins, the spline data is rotated only on the Y axis in the direction input from the outside.
	DIRECT_LOOKAT_SELF_YAXIS,				
	
	// The camera's look at always changes depending on the camera's current position and look at position.
	// When production starts, the spline data is rotated only on the scale and Y axis using the externally input target position.
	DIRECT_LOOKAT_DYNAMIC,

	// Check the spline position for ground height and apply it to the height.
	DIRECT_GROUND_MOVE,

	// The spline position is not checked for ground height.
	DIRECT_FLY_MOVE,

	DIRECT_LOOKAT_END = DIRECT_FLY_MOVE
};

class CNtlDTNodeBase
{
protected:

	RwBool m_bFinish;

public:
	
	CNtlDTNodeBase();
	virtual ~CNtlDTNodeBase() {}

	virtual void	SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj) {}
	virtual void	Update(RwReal fElapsed) {}

	RwBool	IsFinish(void) const { return m_bFinish; }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTSplineNode : public CNtlDTNodeBase
{
private:

	CNtlDTSpline	*m_pProp;
	CNtlSob			*m_pSobObj;

	RwReal			m_fCurrTime;		/** current time passing */
	RwReal			m_fPtTime;			/** Time of current spline point */
	RwInt32			m_iPtIdx;			/** Current curve data index */
	RwV3d			m_vPtPos;
	
	EDirectLookAtType	m_eLookAtType;
	
	RwMatrix		m_matTrans;			/** transform matrix */
	RwMatrix		m_matTransRot;		/** transform roate matrix */
	RwMatrix		m_matRot;			/** rotate temp matrix */
	RwV3d			m_vLookAtPos;		/** camera look at position */

private:

	void UniformTypeUpdate(RwReal fElapsed);
	void TimeSequencetypeUpdate(RwReal fElapsed);

	void GroundMoveUniformUpdate(RwReal fElapsed);
	void GroundMoveTimeSequencetypeUpdate(RwReal fElapsed);

	void SetLookAtSelf(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfScaleAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfScaleYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtDynamic(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);

public:

	CNtlDTSplineNode();
	~CNtlDTSplineNode();

	void SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos, EDirectLookAtType eLookAtType);

	void SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vTransDir);

	void SetDataGroundMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos);

	void SetDataFlyMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos);
	
	void Update(RwReal fElapsed);
};

#endif