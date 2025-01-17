#include "precomp_ntlcore.h"
#include "NtlAtomic.h"
#include "NtlDebug.h"
//#include "NtlToonMaterial.h"

/**
 * Plugin unique ID must not have the same ID as another plugin.
 * Originally, it had to be reported to Renderware and then created.
 */
const RwUInt32 PluginID = 0xFE;

/**
 * Version 0: RwUInt32 uiFlag
 * Version 1: RwUint32 uiFlag, RwUint16 UserDat 
 */
const RwUInt16 NTL_ATOMIC_SAVE_VER0 = 0;

const RwUInt16 NTL_ATOMIC_SAVE_VER = 1;

const RwUInt16 NTL_ATOMIC_SAVE_VER1 = 2;

/**
 *Additional Data
 */
struct NtlAtomicPluginData
{
	void		*pData;

	RwUInt16	uiVersion;			//Save Version
	RwUInt32	uiFlag;				//Atomic Flag

	// Currently, pUser is used in object shadows when it is made to be used universally in memory.
	// If there is no object shadow flag in NtlPLObj, you can use this Pointer publicly.
	// Additionally, since this variable is used for public use, responsibility for creation and deletion must be handled by each module.
	// There is a destruction issue in some cases when destructing a plugin. This value is not cloned.  -Woody
	void* pUserDat;				

	// This variable is exclusive to object shadows. As above, if there is no flag, this variable can be used publicly. This value is cloned. Be careful. -Woody
	RwUInt16 UserDat;

	// This variable is exclusive to object shadows. As above, if there is no flag, this variable can be used publicly. This value is cloned. Be careful. -Woody
	RwReal _UserDatReal;

	// Alpha value to be modulated in Atomic
	RwUInt8		alpha;

	RpAtomicCallBackRender atomicRenderCallBack;

	// This variable is dedicated to the environment map. As above, if there is no flag, this variable can be used publicly. This value is cloned. Be careful. -Woody
	RwUInt16 _EnvTexName;
};

/**
 * Extended Offset Memory address
 */
unsigned int iPlugin_Offset = 0;

/**
 * Extended Offset access function
 */
inline NtlAtomicPluginData* NtlGetPluginData( RpAtomic* pAtomic)
{
	NTL_FUNCTION("NtlGetPluginData()");

	NTL_PRE(iPlugin_Offset);

	const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(pAtomic);
	const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

	NTL_RETURN( reinterpret_cast<NtlAtomicPluginData*>(pluginAddr) );
}

/**
 * Extended Offset access function
 */
inline const NtlAtomicPluginData *NtlGetPluginData( const RpAtomic* pAtomic )
{
	NTL_FUNCTION("NtlGetPluginData()");

	NTL_PRE(iPlugin_Offset);

	const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(pAtomic);
	const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

	NTL_RETURN( reinterpret_cast<const NtlAtomicPluginData*>(pluginAddr) );
}

/**
 * Constructor
 */
void* Plugin_Constructor(void* pObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	NTL_FUNCTION("Plugin_Constructor()");

	NTL_PRE(pObject);

	NtlAtomicPluginData* pPluginData	= NtlGetPluginData((RpAtomic*)pObject);
	pPluginData->pData					= NULL;
	pPluginData->uiVersion				= NTL_ATOMIC_SAVE_VER1;
	pPluginData->uiFlag					= NTL_ATOMIC_FLAG_INVALID;
	pPluginData->pUserDat				= NULL;
	pPluginData->UserDat				= 0;
	pPluginData->_UserDatReal			= 1.f;
	pPluginData->alpha					= 255;
	pPluginData->_EnvTexName			= 0;

	pPluginData->atomicRenderCallBack	= NULL;

	NTL_RETURN(pObject);
}

/**
 * Destructor
 */
static void *Plugin_Destructor( void* pObject, RwInt32  offset, RwInt32  size __RWUNUSED__)
{
	NTL_FUNCTION("Plugin_Destructor()");

	NTL_PRE(pObject);

	/*
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pObject);

	if(pPluginData->pUserDat != NULL)
		RwTextureDestroy(pPluginData->pUserDat);
		*/

	NTL_RETURN(pObject);

}

/**
 * Copy
 */
void *Plugin_Copy(void* pDestObject, const void* pSrcObject, RwInt32 offset, RwInt32 size)
{
	NTL_FUNCTION("Plugin_Copy");
	NTL_PRE(pDestObject);
	NTL_PRE(pSrcObject);

	//void Data is not copied. There is a possibility of error due to incorrect pointer.	
	NtlAtomicPluginData* pSrcData  = NtlGetPluginData((RpAtomic*)pSrcObject);
	NtlAtomicPluginData* pDestData = NtlGetPluginData((RpAtomic*)pDestObject);

	pDestData->uiVersion    = pSrcData->uiVersion;
	pDestData->uiFlag		= pSrcData->uiFlag;
	pDestData->UserDat		= pSrcData->UserDat;
	pDestData->_UserDatReal = pSrcData->_UserDatReal;
	pDestData->_EnvTexName  = pSrcData->_EnvTexName;

	NTL_RETURN(pDestObject);
}

/**
 * Stream Read
 */
static RwStream *Plugin_StreamRead( RwStream *stream,
								    RwInt32 length __RWUNUSED__,
									void *pAtomic,
									RwInt32 offset __RWUNUSED__,
									RwInt32 size __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);

	// Default
	RwUInt16 uiNtlAtomicSaveVer = NTL_ATOMIC_SAVE_VER0;

	if (RwStreamRead(stream, (void *)(&(uiNtlAtomicSaveVer)), sizeof(uiNtlAtomicSaveVer)) != sizeof(uiNtlAtomicSaveVer))
		return NULL;
	
	pPluginData->uiVersion = uiNtlAtomicSaveVer;
	if(pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER0)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;
		
		if (RwStreamRead(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)) 
			!= sizeof(pPluginData->UserDat))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)) 
			!= sizeof(pPluginData->_UserDatReal))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER1)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)) 
			!= sizeof(pPluginData->UserDat))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)) 
			!= sizeof(pPluginData->_UserDatReal))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_EnvTexName)), sizeof(pPluginData->_EnvTexName)) 
			!= sizeof(pPluginData->_EnvTexName))
			return NULL;
	}
	else
		return NULL;

	return stream;
}

/**
 * Stream Write
 */
static RwStream *Plugin_StreamWrite( RwStream *stream,
									 RwInt32 length __RWUNUSED__,
									 const void *pAtomic,
									 RwInt32 offset __RWUNUSED__,
									 RwInt32 size __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);
	
	if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiVersion)), sizeof(pPluginData->uiVersion)))
		return NULL;

	if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER0)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;
		
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER1)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_EnvTexName)), sizeof(pPluginData->_EnvTexName)))
			return NULL;
	}
	return stream;
}

/**
 *Used only when saving the plugin.
 */
static RwInt32 Plugin_StreamGetSize( const void *pAtomic,
                                     RwInt32     offset,
                                     RwInt32     sizeInObj __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);
	
	// Always save the latest version. (NTL_ATOMIC_SAVE_VER)
	//pPluginData->uiVersion = NTL_ATOMIC_SAVE_VER;
	//return (sizeof(pPluginData->uiVersion) + sizeof(pPluginData->uiFlag) + sizeof(pPluginData->UserDat) + sizeof(pPluginData->_UserDatReal) );

	pPluginData->uiVersion = NTL_ATOMIC_SAVE_VER1;
	return (sizeof(pPluginData->uiVersion) + sizeof(pPluginData->uiFlag) + sizeof(pPluginData->UserDat) + sizeof(pPluginData->_UserDatReal) + sizeof(pPluginData->_EnvTexName));
}

/**
 * Plugin registration
 */      
RwBool RpNtlAtomicPluginAttach(void)
{
    NTL_FUNCTION("RpNtlAtomicPluginAttach");

	//When performing atomic expansion, the value passed is the actual expanded Offset address.
    iPlugin_Offset = RpAtomicRegisterPlugin (sizeof(NtlAtomicPluginData),
											 MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
											 Plugin_Constructor,  
											 Plugin_Destructor,
											 Plugin_Copy); //Plugin_Copy);
	RWS_POST(iPlugin_Offset);

	RwInt32 nOffset = -1;
    nOffset = RpAtomicRegisterPluginStream( MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
                                             Plugin_StreamRead,
                                             Plugin_StreamWrite,
                                             Plugin_StreamGetSize);

    if( nOffset != iPlugin_Offset)
    {
        NTL_RETURN(FALSE);
    }

    NTL_RETURN (0 != iPlugin_Offset);
}

/**
 *Plugin extension access function
 */   
void RpNtlAtomicSetData( RpAtomic* pAtomic, void *pData)
{
    NTL_FUNCTION("RpNtlAtomicSetData()");

    NTL_PRE( iPlugin_Offset );

	NTL_ASSERTE(pAtomic);
    NtlGetPluginData(pAtomic)->pData = pData;

    NTL_RETURNVOID();
}

static RpAtomic *SetAllAtomic(RpAtomic* pAtomic, void *pData)
{
	RpNtlAtomicSetData(pAtomic, pData);
    
	return pAtomic;
}

void RpNtlClumpSetData(RpClump* pClump, void *pData)
{
	NTL_FUNCTION("RpNtlClumpSetData()");
	NTL_ASSERTE(pClump);
	RpClumpForAllAtomics( pClump, SetAllAtomic, (void *)pData);
	
	NTL_RETURNVOID();
}

void *RpNtlAtomicGetData( const RpAtomic* pAtomic )
{
    NTL_FUNCTION("NtlAtomicGetData");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
    NTL_RETURN(NtlGetPluginData(pAtomic)->pData);
}

void RpNtlAtomicSetFlag(RpAtomic* pAtomic, RwUInt32 uiFlag)
{
	NTL_FUNCTION("RpNtlAtomicSetRenderFlag()");

    NTL_PRE( iPlugin_Offset );

	NTL_ASSERTE(pAtomic);
	NtlGetPluginData(pAtomic)->uiFlag = uiFlag;
	
	NTL_RETURNVOID();
}

RwUInt32 RpNtlAtomicGetFlag(RpAtomic* pAtomic)
{
	NTL_FUNCTION("RpNtlAtomicGetRenderFlag()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	NTL_RETURN(NtlGetPluginData(pAtomic)->uiFlag);
}

RwTexture* RpNtlAtomicGetUserDat4RwTex(RpAtomic* _pAtomic)
{
	// careful for the old pUserDat had to be used for RwTextures
	return static_cast<RwTexture*>(NtlGetPluginData(_pAtomic)->pUserDat);
}

void RpNtlAtomicSetUserDat4RwTex(RpAtomic* _pAtomic, RwTexture* _pTex)
{
	// careful for the old pUserDat had to be used for RwTextures
	NtlGetPluginData(_pAtomic)->pUserDat = static_cast<void*>(_pTex);
}

void RpNtlAtomicSetUserDat(RpAtomic* _pAtomic, void* _pUserDat)
{
	NtlGetPluginData(_pAtomic)->pUserDat = _pUserDat;
}

void* RpNtlAtomicGetUserDat(RpAtomic* _pAtomic)
{
	return NtlGetPluginData(_pAtomic)->pUserDat;
}

void RpNtlAtomicSetEnvTexName(RpAtomic* _pAtomic, RwUInt16 _EnvTexName)
{
	NTL_ASSERTE(_pAtomic);

	NtlGetPluginData(_pAtomic)->_EnvTexName = _EnvTexName;
}

RwUInt16 RpNtlAtomicGetEnvTexName(RpAtomic* _pAtomic)
{
	NTL_ASSERTE(_pAtomic);

	return NtlGetPluginData((RpAtomic*)_pAtomic)->_EnvTexName;
}

void RpNtlAtomicSetUserDatU16(RpAtomic *pAtomic, RwUInt16 uiSize)
{
	NTL_ASSERTE(pAtomic);

	NtlGetPluginData(pAtomic)->UserDat = uiSize;
}

RwUInt16 RpNtlAtomicGetUserDatU16(RpAtomic *pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData((RpAtomic*)pAtomic)->UserDat;
}

void RpNtlAtomicSetUserDatReal(RpAtomic *pAtomic, RwReal uiSize)
{
	NTL_ASSERTE(pAtomic);

	NtlGetPluginData(pAtomic)->_UserDatReal = uiSize;
}

RwReal RpNtlAtomicGetUserDatReal(RpAtomic *pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData((RpAtomic*)pAtomic)->_UserDatReal;
}

void RpNtlAtomicSetAlpha(RpAtomic* pAtomic, RwUInt8 alpha)
{
	NTL_ASSERTE(pAtomic);
	
	NtlGetPluginData(pAtomic)->alpha = alpha;
}

RwUInt8 RpNtlAtomicGetAlpha(RpAtomic* pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData( (RpAtomic*)pAtomic)->alpha;
}

RpAtomicCallBackRender RpNtlAtomicGetRenderCallBack(RpAtomic *pAtomic)
{
	NTL_FUNCTION("RpNtlAtomicGetRenderCallBack()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	
	NTL_RETURN(NtlGetPluginData(pAtomic)->atomicRenderCallBack);
}

void RpNtlAtomicSetRenderCallBack(RpAtomic *pAtomic, RpAtomicCallBackRender atomicCallBackRender)
{
	NTL_FUNCTION("RpNtlAtomicSetRenderCallBack()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	
	NtlGetPluginData(pAtomic)->atomicRenderCallBack = atomicCallBackRender;


	NTL_RETURNVOID();
}





