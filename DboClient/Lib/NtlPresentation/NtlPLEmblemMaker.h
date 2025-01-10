/*****************************************************************************
*
* File			: NtlPLEmblemMaker.h
* Author		: Hong sungbock
* Copyright		: (?)NTL
* Date			: 2008. 3. 4
* Abstract		: Presentation layer emblem maker
*****************************************************************************
*Desc          :Create/return a guild emblem
*               Material texture: Local texture to combine to create an emblem
*****************************************************************************/

#ifndef __NTL_PLEMBLEM_MAKER_H__
#define __NTL_PLEMBLEM_MAKER_H__

#include "NtlPLDef.h"
#include "NtlPLEmblemStuffList.h"


#define dPLEMBLEM_INVALID_FACTOR			(0xff)

#define dMAX_EMBLEM_TEXTURE_SIZE			(256)
#define dEMBLEM_TEXTURE_DEFAULT_SIZE		(64)


class CNtlPLCameraRT;

struct sEmblemFactor
{
	RwUInt8		byTypeA;
	RwUInt8		byTypeB;
	RwUInt8		byTypeC;
	RwUInt8		byTypeAColor;
	RwUInt8		byTypeBColor;
	RwUInt8		byTypeCColor;

	sEmblemFactor()
	:byTypeA(dPLEMBLEM_INVALID_FACTOR)
	,byTypeB(dPLEMBLEM_INVALID_FACTOR)
	,byTypeC(dPLEMBLEM_INVALID_FACTOR)
	,byTypeAColor(0)
	,byTypeBColor(0)
	,byTypeCColor(0)
	{
	}
};

class CNtlPLEmblemMaker
{
public:
	struct sEmblemData
	{
		sEmblemFactor	factor;
		RwTexture*		pTexture;
	};

	typedef std::map<std::string, sEmblemData*>					MAP_TEXTURE;
	typedef std::map<std::string, sEmblemData*>::iterator		ITER_MAP_TEXTURE;

	typedef std::map<RwUInt8, CNtlPLCameraRT*>					MAP_RT_CAMERA;
	typedef std::map<RwUInt8, CNtlPLCameraRT*>::iterator		ITER_RT_CAMERA;

public:
	static CNtlPLEmblemMaker* GetInstance() { return m_pInstance; }
	


	CNtlPLEmblemMaker();
	virtual ~CNtlPLEmblemMaker();

	static VOID	CreateInstance();
	static VOID	DestoryInstance();

	VOID		DestoyEmblem(const char* pcTextureName);
	VOID		DestoyEmblem(RwTexture* pTexture);

	///< Returns an existing texture, if not, creates a new one and returns it
	///< Caution: If this function is called at rendering time and the returned RwTexture is not NULL, it will be annoying.
	///< Camera->CameraBeginUpdate called inside this function is the current rendering function.
	///< Since the pointer of the Current Camera is changed, this function must be called at a time other than rendering.
	RwTexture*	CreateEmblem(const sEmblemFactor* pEmblemFactor,
							 RwUInt8 byEmblemSize = dEMBLEM_TEXTURE_DEFAULT_SIZE,
							 const RwChar* pcBaseTextureName = NULL);


	///< Creates and returns the name of the emblem material texture that exists locally.
	VOID		MakeEmblemResourceName(eEmblemType eType, RwUInt8 byValue, char* pcOutBuffer, RwInt32 iBufferLength,
									   RwUInt8 byEmblemSize = dEMBLEM_TEXTURE_DEFAULT_SIZE);
	
	const MAP_STUFF* GetStuffList(eEmblemType eType);

protected:
	///< Returns the name of the emblem texture created as a parameter of the material texture. The emblem of the character model is not valid.
	///< Suffix: Suffix
	VOID		MakeEmblemTextureName(char* pcOutBuffer, RwInt32 iBufferLength, const sEmblemFactor* pEmblemFactor,
									  RwUInt8 byEmblemSize, const RwChar* pcBaseTextureName);

	RwTexture*	MakeEmblemImmidately(const sEmblemFactor* pEmblemFactor, const char* pcEmblemName,
									 RwUInt8 byEmblemSize, const RwChar* pcBaseTextureName = NULL);

	///< Draws the received texture in a separate buffer.
	VOID		RecoatEmblem(RwTexture* pTexture, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwTexture*	CopyToTexture(RwTexture* pTexture);

	///< Returns a camera that matches the texture size
	///< The sizes of the textures to be used for the emblem will not vary greatly, so
	///< Create a separate camera to capture the texture
	CNtlPLCameraRT* GetRTCamera(RwUInt8 bySize);

protected:
	static CNtlPLEmblemMaker* m_pInstance;

	MAP_RT_CAMERA			m_mapRTCamera;
	RwIm2DVertex			m_2dVertices[4];
	MAP_TEXTURE				m_mapTexture;

	CNtlPLEmblemStuffList*	m_pStuffList;
};

static CNtlPLEmblemMaker* GetEmblemMaker(VOID)
{
	return CNtlPLEmblemMaker::GetInstance();
}

#endif