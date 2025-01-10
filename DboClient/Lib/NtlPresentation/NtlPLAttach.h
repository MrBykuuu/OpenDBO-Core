/*****************************************************************************
 *
 * File			: NtlPLAttach.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer attach entity class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLATTACH_H__
#define __NTL_PLATTACH_H__

#include <list>
#include <string>
#include "NtlPLEntity.h"
#include "NtlMath.h"

class CNtlPLAttach;

enum EPLAttachType
{
	PL_ATTACH_BONE,
	PL_ATTACH_BONE_OFFSET_POS_ROOT_ROTATE_Y,//Offset position and root Y-axis rotation value are applied to the character bone.

	PL_ATTACH_WORLD_POS,
	PL_ATTACH_WORLD_POSANDROTATE,			//First of all, it was added arbitrarily (By HoDong)
    PL_ATTACH_WORLD_POS_HEIGHT,             ///< Attaches to location but height is World Height

    PL_ATTACH_NONE,                         ///< When not attached to Bone or World, but only entered in pure Attach List (Trace Effect, Grade Effect)
};

#define MAX_ATTR_BONE_NAME	128

typedef struct _SPLAttachAttr
{
	CNtlPLAttach *pPLEntity;
	EPLAttachType eType;
	char chBoneName[MAX_ATTR_BONE_NAME];
	RwV3d vOffsetPos;
	RwV3d vScale;
	RwV3d vRot;

    _SPLAttachAttr()
    {
        ZeroMemory(chBoneName, sizeof(chBoneName));
        vScale.x = vScale.y = vScale.z = 1.0f;
		vRot.x = vRot.y = vRot.z = 0.0f;
        vOffsetPos = ZeroAxis;
    }
}SPLAttachAttr;
typedef std::list<SPLAttachAttr*> ListAttachAttr;


class CNtlPLAttach : public CNtlPLEntity
{
private:

	static RwFreeList *m_pAttachAttrFreeList;	/* attach attribute data memory pool **/

public:

	static RwBool AllocFreeList(void);
	static void DeAllocFreeList(void);

protected:
	CNtlPLAttach *m_pAttachParent;
	ListAttachAttr m_listAttachAttr;

private:

	RwBool Find(const CNtlPLAttach *pPLAttachEntity);
	void AttachDestroy(void);

public:

	CNtlPLAttach();
	virtual ~CNtlPLAttach();

	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool RenderToTexture(void);

	RwBool AttachBone(CNtlPLAttach *pPLAttachEntity, const RwChar *pBoneName, SPLAttachAttr* _pSPLAttachAttr = NULL);
    //RwBool AttachBoneOffset(CNtlPLAttach* pPLAttachEntity, const RwChar* pBoneName, RwV3d vOffset);
	RwBool AttachBoneOffsetPosRootRotateY(CNtlPLAttach *pPLAttachEntity, const RwChar* pBoneName, RwV3d vOffset);

	RwBool AttachWorldPos(CNtlPLAttach *pPLAttachEntity, RwV3d vOffset);
	RwBool AttachWorldPosAndRotate(CNtlPLAttach *pPLAttachEntity, RwV3d vOffset);
    RwBool AttachWorldPosHeight(CNtlPLAttach* pPLAttachEntity); ///< Attach to the terrain height.

    RwBool Attach(CNtlPLAttach* pPLAttachEntity);               ///< It is not attached to Bone or World, but only enters the Attach List.
    
	void Detach(CNtlPLAttach *pPLAttachEntity);

	virtual void SetAttachParent(CNtlPLAttach *pPLAttachEntity);
	virtual CNtlPLAttach* GetAttachParent(void) const;

	virtual void CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue); 
	virtual void DeleteWeightElapsedController(void);

	virtual void SetVisible(RwBool bVisible);
	virtual void SetCullFlags(RwUInt32 uiFlag, RwBool bCulled);
	virtual void SetCullFlags(RwUInt32 uiFlag);

	virtual RwMatrix* GetBoneMatrix(const RwChar *pBoneName) { return NULL; }
	virtual RwFrame* GetFrame(void) { return NULL; }

	virtual ListAttachAttr* GetAttachAttrList() { return &m_listAttachAttr; }
};

#endif