/*****************************************************************************
 *
 * File			: NTLCameraHelper.h
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 25	
 * Abstract		: NTl CameraHelper
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_CAMERA_HELPER__
#define __NTL_CAMERA_HELPER__

/**
 * \ingroup Client
 * These are camera-related helper functions.
 * 
 */

class CNtlCameraHelper
{

public:
	static RwBool CameraRotation(const RwCamera *pCamera, RwReal turn, RwReal tilt);
	static RwBool CameraTranslateAt(const RwCamera *pCamera, RwReal delta);
	static RwBool CameraTranslate(const RwCamera *pCamera, const RwV3d *delta);
};

#endif