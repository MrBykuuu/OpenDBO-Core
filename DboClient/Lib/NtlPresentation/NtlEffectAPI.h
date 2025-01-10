

#ifndef _NTLEFFECT_API_
#define _NTLEFFECT_API_

#include <rphanim.h>
#include <rpskin.h>
#include <rpuvanim.h>
#include <rtdict.h>


RpAtomic*	API_GetSkinHierarchy(RpAtomic *atomic, void *data);
void		API_SetAnimation(RpClump* pClump, RtAnimAnimation* pAnimation);

RpAtomic*	API_MeshColorUpdate(RpAtomic *pAtomic, void *data __RWUNUSED__);

/// Find the material with UVAnim applied in Clump and add it to the list.
RwSList*	API_AddAnimatedMaterialsList(RwSList* pList, RpClump* pClump);

/// Update UVAnim's time (add time)
void		API_MaterialsInterpolatorsAddAnimTime(RwSList *animatedMaterials, RwReal deltaTime);
/// Update the time of UVAnim (time setting)
void        API_MaterialsInterpolatorsSetCurrentTime(RwSList* animatedMaterials, RwReal fTime);
/// Update UVAnim.
void		API_MaterialsAnimApply(RwSList *animatedMaterials);

#endif