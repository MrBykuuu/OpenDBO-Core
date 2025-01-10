/*****************************************************************************
*
* File			: NtlPLDecal.h
* Author		: agebreak
* Copyright	    : (?)NTL
* Date			: 2006. 6. 15	
* Abstract		: Presentation layer decal entity class
*****************************************************************************
* Desc          : Decal class used in Effect and World. (also used for shadow expression)
*
*****************************************************************************/

#ifndef __NTL_PLDECAL_H__
#define __NTL_PLDECAL_H__

#include "NtlPLAttach.h"
#include "NtlEffectDefine.h"

/// Types of Decals
enum EDecalType
{
	DECAL_EFFECT,					///< Effect¿ë Decal
	DECAL_TERRAIN,					///< Decal for terrain
};

// Flag used only in Decal Effect
enum EDecalEffectFlag
{
	E_FLAG_DECAL_RENDER_OBJECT = 0x00100000,					///< Render decal on object
	E_FLAG_DECAL_RENDER_WORLD  = 0x00200000,					///< Render Decal to World
	E_FLAG_DECAL_WATER		   = 0x00400000,					///< If the decal appears only on water, turn on this flag (default value: False)
	E_FLAG_DECAL_DAYNIGHT 	   = 0x00800000						///< If it is a decal affected by Day&Night, turn on this flag (default value False)
};


/// Parameter structure passed to the callback function to draw a decal on an object
struct DecalCallbackParam
{
	RwV3d vPos;
	RwMatrix *matTrans;
	RwMatrix *matAtomicTrans;
	RwInt32 *pBufferCnt;
	RwIm3DVertex *pVertices;

	float fIntersectionRadius;
	float fScale;
	RwInt32 nMaxVertextCnt;				///< Maximum number of vertices of decal to be created
	RwReal fYOffset;
};

class CNtlPLWorldEntity;

struct SPLDecalCreateParam : public SPLEntityCreateParam
{
	RwInt32	nTexNameLen;
	const RwChar *pTexName;
	const RwChar *pTexPath;
	RwReal fIntersectionRadius;
	RwV3d  vSize;								///< Decal's Size (use only X, Z values)
	RwReal fScale;
	RwReal fVisibleSquaredDist;					///< The square of the visual distance the decal is visible
	RwReal fYOffset;							///< Y Offset value (Decal height from ground)
	RwTextureAddressMode rwWrapType;			///< Texture Adress Mode
	RwInt32	nFlagEffect;						///< Effect flags
	EDecalType eDecalType;						///< Decal Type

	SPLDecalCreateParam()
	: rwWrapType(rwTEXTUREADDRESSCLAMP), fScale(1.0f), fIntersectionRadius(3.0f), 
	  fVisibleSquaredDist(1600.0f), fYOffset(0.1f)
	{ 
		vSize.x = vSize.y = vSize.z = 1.0f;
		pPos = NULL;
		nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_RENDER_WORLD;		
		eDecalType = DECAL_EFFECT;
	}
};

class CNtlPLDecal : public CNtlPLAttach
{
public:

	CNtlPLDecal();
	virtual ~CNtlPLDecal();

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);
	
	virtual void SetNameLen(RwInt32 len) { m_iNameLen = len; }
	virtual void SetPosition(const RwV3d *pPos);	
	virtual void SetAlpha(RwUInt8 byAlpha);
	virtual void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	virtual void SetScale(RwReal fScale);
    virtual void SetScaleSize(RwReal fScale);
	virtual void SetSize(RwReal fXSize, RwReal fZSize);
	virtual void SetRotate(RwReal fDegree);															///< Rotate the decal. It rotates only based on the Y axis. It does not apply if Rotate Action is applied.
	virtual void SetMatrix(RwMatrix& matWorld);														///< Apply World Matrix. 
	virtual void SetYOffset(RwReal fYOffset) {m_fYOffset = fYOffset;}								///< Set the YOffset value of the decal (how much to draw above the ground)

	virtual RwInt32	GetNameLen() { return m_iNameLen; }

	virtual RwV3d	GetPosition() {return m_vPos;}
	virtual RwRGBA	GetColor() {return m_color;}
	virtual RwReal	GetScale() {return m_fScale;}
	virtual RwReal	GetBaseScale() { return m_fStartScale; }
	virtual RwV3d	GetSize() {return m_vSize;}
	virtual RwReal	GetRotate() {return m_fDegree;}
	virtual RwMatrix& GetMatrix() {return m_matWorld;}
	virtual RwReal	GetYOffset() {return m_fYOffset;}
	virtual RwReal  GetVisibleDist() { return m_fVisibleSquaredDist; }
	virtual SNtlPrtStdEmitterPrtUVAnim& GetUVChunk() { return m_EmitterPrtUVAnim; }
    virtual RwUInt32 GetMemoryUseSize() {return m_uiMemoryUseSize;}                                 ///< Returns memory usage.

	void			GetRotateActionValue(RwReal& fAngle, RwReal& fTimeGap);
	void			GetScaleActionValue(RwReal& _StartScale, RwReal& _EndScale, RwReal& _TimeGap);
	
	void SetLifeTime(RwReal fLifeTime) {m_fLifeTime = fLifeTime;}									///< Set the Decal¡¯s LifeTime. (Set 0 for permanent Decal)
	
	void SetAlphaBlend(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);			///< Set alpha blend.
	RwInt32	GetVerexCount() {return m_nRenderVertexCnt;}											///< Returns the number of vertices currently being drawn.

	// Effect Flag
	RwInt32	GetEffectFlag() {return m_nFlagEffect;}
	void	SetEffectFlag(RwInt32 nFlag) {m_nFlagEffect = nFlag;}

	// Action
	void	Set2DRoateAction(RwReal fAngle, RwReal fTimeGap = 1.0f);						///< Apply rotation action. (Set the rotation angle over 1 second (the angle is Degree.))
	void	SetColorAction(const RwRGBA& colStart, const RwRGBA& colEnd, RwReal fTimeGap);	///< Apply Color Action.
	void	SetMultiColorAction(const RpPrtAdvPrtMultiColorEmitter& emitterPrtMulticol);		///< Apply Multi Color Action.
	void	SetSizeAction(RwReal fStartScale, RwReal fEndScale, RwReal fTimeGap);			///< Apply Size Action.
	void	SetMultiSizeAction(const SNtlAdvMultiSizeEmitter& emitterPrtMultiSize);			///< Apply Multi Size Action.
	void	SetMultiTextureAction(const RpPrtAdvEmtPrtMultiTextures& emitterMultiTexture);	///< Apply Multi Texture Action.
	void	SetUVAnimAction(const SNtlPrtStdEmitterPrtUVAnim& emitterUVAnim);				///< Apply UV Animation.
    void    SetUpdateVertex(RwBool bUpdate) {m_bVertexUpdate = bUpdate;}                    ///< Change the flag when forcefully updating vertices.

protected:
	RwBool	UpdateVertices();						///< Update Decal Vertex information.
	void	SetVertexColor(const RwRGBA& color);	///< Set the color of vertices.	
	void	UpdateUV();								///< Update UV values ??of vertices
	
	void	UpdateRoate(RwReal fElapsedTime);		///< Apply the Rotate Action value.
	void	UpdateColor(RwReal fElapsedTime);		///< Apply the Color Action value.
	void	UpdateScale(RwReal fElapsedTime);		///< Apply the Scale Action value.
	void	UpdateMultiTexture(RwReal fElapsedTime);///< Apply Multi Texture Action value.
	void	UpdateMultiColor(RwReal fElapsedTime);	///< Apply Multi Color Action value.
	void	UpdateMultiSize(RwReal fElapsedTime);	///< Apply Multi Size Action value. (size only applies the X value)
	void	UpdateUVAnim(RwReal fElapsedTime);		///< Apply UV Anim Action value.
	void	UpdateFade(RwReal fElapsedTime);		///< Update Fade In/Out.

	void	GetObjectDecalVertex();					///< Calculate the Decal drawn on the Object.		
	RwBool	GetWaterDecalVertex();					///< Calculate the decal drawn on the water.
	RwBool	IsVisble();								///< Calculates camera culling and Visible Distance and returns whether or not it is visible.
    void    ChangeBigVB();                          ///< If the vertex buffer is insufficient, change to a larger buffer.

	void	SetTextureMatrix(RwIm3DVertex* pVertices, RwInt32 nVertCnt, RwMatrix* pMatrix);	///< Apply Texture Matrix to UV of vertices.

protected:

	RwInt32	m_iNameLen;

    RwInt32 m_nMaxVertexCnt;
	RwV3d m_vPos;
	RwV3d m_vSize;
	RwReal m_fIntersectionRadius;
	RwReal m_fScale;
	RwReal m_fVisibleSquaredDist;
	RwTextureAddressMode m_rwWrapType;				///< Wrap Type of Texture
	RwMatrix m_matWorld;							///< World Matrix
	RwReal m_fYOffset;								///< Y Offset value
	RwBool m_bVertexUpdate;							///< Flag whether to update vertex
	RwV3d  m_vOriginPos;							///< Center position of the currently created World Vertex
	RwReal m_fRadiusWorldVertex;					///< Radius of the currently created World Vertex
	RwReal m_fDegree;								///< Rotation°ª (Degree)
	EDecalType m_eDecalType;						///< DecalType

	RwTexture* m_pTexture;
	RwTexture* m_pCurrentTexture;
	RwTexture** m_ppMultiTextures;
	RwRGBA m_color;	

	// Render State
	RwBool				m_bBlend;			///< Presence of alpha blending
	RwBlendFunction		m_blendSrc;			///< Sauce Blend
	RwBlendFunction		m_blendDest;		///< Target Blend

	RwIm3DVertex*		m_pIm3DBuffer;		///< Vertex Buffer 
	RwInt32				m_nRenderVertexCnt;	///< Number of vertices to be rendered	

	// Emitter Action
	RwInt32				m_nFlagEffect;		///< Effect property Flag
	RwReal				m_fTotalElapsedTime;///< Time since creation
	RwReal				m_fElapsedTime;		///< One frame of time
	RwReal				m_fLifeTime;		///< DecalÀÇ Life Time

	RwReal				m_f2DRoateAngle;	///< Rotation angle during 2D Roate Action
	RwBool				m_bFirstRotate;		///< Variable used to check the change in the center of the texture when rotating
	RwReal				m_fOrgRotateAngle;	///<Originally set Rotate Angle value
	RwReal				m_fRotateTimeGap;	///< Set Rotate Time Gap

	RwMatrix			m_matTexture;		///< Texture matrix used to change UV coordinates

	RwRGBA				m_colStart;			///< Start Color used in Color Action
	RwRGBA				m_colEnd;			///< End Color used in Color Action
	RwReal				m_fColorActionTime;	///< Time used for Color Action

	RwReal				m_fStartScale;		///< Start Scale used for Size Aciton
	RwReal				m_fEndScale;		///< End Scale used in Scale Action;
	RwReal				m_fScaleActionTimeGap; ///< Time used for Scale Action

	RwUInt32			m_nCurrentTexture;		  ///< Index number of the texture currently being rendered in Multi Texture Action
	RwReal				m_fMultiTextureDeltaTime; ///< Time used for Multi Texture Acion

	RwRGBA				m_colPrev;				  ///< Previous Color used in Multi Color Action
	RwUInt32			m_nNextColorIndex;		  ///< Index used in Multi Color Action
	RwReal				m_fMultiColorDeltaTime;	  ///< Time used for Multi Color Action

	RwReal				m_fPrevSize;			  ///< Previous Size used in Multi Size Action
	RwUInt32			m_nNextSizeIndex;		  ///< Index used in Multi Size Action
	RwReal				m_fMultiSizeDeltaTime;	  ///< Time used for Multi Size Action

	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;		///< Structure containing Multi Color Action information
	RpPrtAdvEmtPrtMultiTextures		m_EmitterMultiTextures;		///< Multi Texture Action information structure
	SNtlAdvMultiSizeEmitter			m_EmitterPrtMultiSize;		///< Multi Size Action information structure
	SNtlPrtStdEmitterPrtUVAnim		m_EmitterPrtUVAnim;			///< UV Anim Action information structure

	EFadeStatus			m_eFadeStatus;			  ///< Fade state variable (used only in case of terrain decal)
	RwInt32 			m_uiFadeAlpha;			  ///< Alpha used when fading 
	RwReal				m_fFadeGap;

    RwUInt32            m_uiMemoryUseSize;        ///< Memory usage

	static const RwReal		m_cfFadeTime;
};

#endif
