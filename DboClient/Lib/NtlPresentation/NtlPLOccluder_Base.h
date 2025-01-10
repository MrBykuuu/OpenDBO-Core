#pragma once

#include "NtlPLEntity.h"

#define dPL_OCCLUDER_OFFSET_Y		0.5f

enum EPLOccluderType
{
	EPLOCCLUDER_BASE			= 0,
	EPLOCCLUDER_QUAD,
};

enum EPLOccluderPlaneType
{
	EPLOCCLUDER_PLANE_ONESIDE	= 0,
	EPLOCCLUDER_PLANE_TWOSIDE,
	EPLOCCLUDER_PLANE_BILLBOARD, // It's not an actual billboard. Rotates in the camera position direction (Y).
};

enum EPLOccluderFuncFlag
{
	EPLOCCLUDER_FUNC_PVS		= 0x00000001, // In case of PVS, it operates regardless of the flags below.
	EPLOCCLUDER_FUNC_TERRAIN	= 0x00000002,
	EPLOCCLUDER_FUNC_OBJECT		= 0x00000004,
	EPLOCCLUDER_FUNC_CHARACTER	= 0x00000008,
	EPLOCCLUDER_FUNC_PORTAL		= 0x00000010,
};

class CNtlPLOccluder_Base : public CNtlPLEntity
{
public:
	CNtlPLOccluder_Base();
	virtual ~CNtlPLOccluder_Base();

	virtual RwBool					Create(const SPLEntityCreateParam *pParam = NULL)	= 0;
	virtual void					Destroy()											= 0;

	virtual RwBool					Update(RwReal fElapsed);

	// Update according to surface properties
	virtual RwBool					UpdateOneside(RwReal fElapsed)						= 0;
	virtual RwBool					UpdateTwoside(RwReal fElapsed)						= 0;
	virtual RwBool					UpdateBillboard(RwReal fElapsed)					= 0;
	
	virtual RwBool					Render();

	virtual RwBool					SetProperty(const CNtlPLProperty *pData);

	virtual RwUInt32				GetOccluderType()									= 0;

	virtual RwBool					OccluderTestSphere(RwSphere* pSphere)				= 0;
	virtual RwBool					OccluderTestBox(RwBBox* pBox)						= 0;

	// If it passes the VisibilityTest, it will be registered with the Manager in Update.
	virtual RwBool					VisibilityTest()									= 0;

	virtual RwBool					Pick(RwReal* pfDist, RwInt32* piIndex = NULL)		= 0;

	virtual	RwBool					PVSTest(RwV3d* pvStart, RwV3d* pvEnd)				= 0;

	virtual RwBool					IsRunable()											{ return m_bRunable; }
	virtual RwBool					IsOccluderFuncFlag(RwUInt32 eFlag)					{ return m_eOccluderFlag & eFlag; }
	virtual	void					SetOccluderFuncFlag(RwUInt32 eFlag);
	virtual	void					SetOccluderFuncFlag(RwUInt32 eFlag, RwBool bFlag);
	virtual	RwUInt32				GetOccluderFuncFlag()								{ return m_eOccluderFlag; }

	virtual void					SetPosition(const RwV3d* pPos)						{ m_vPos = *pPos; }
	virtual	RwV3d					GetPosition()										{ return m_vPos; }

	virtual void					SetOccluderPlaneType(RwUInt32 eType)				{ m_ePlaneType = eType; }
	virtual RwUInt32				GetOccluderPlaneType()								{ return m_ePlaneType; }

#ifdef dNTL_WORLD_TOOL_MODE
	virtual void					SetToolUI(RwBool bBool, RwInt32 iIndex = -1)		{ m_bToolUI = bBool; m_iIndexPicked = iIndex; }
	virtual void					SetOccluderPVS(RwBool bPVS);
#endif

protected:
	RwV3d					m_vPos;
	RwBool					m_bRunable;
	RwUInt32				m_ePlaneType;

	RwUInt32				m_eOccluderFlag;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	RwBool					m_bToolUI;
	RwInt32					m_iIndexPicked;
#endif
};