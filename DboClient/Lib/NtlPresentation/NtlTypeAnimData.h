/*****************************************************************************
 *
 * File			: NtlTypeAnimData.h
 * Author		: HongHoDong
 * Copyright	: (?)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlTypeAnimData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_TYPE_ANIM_DATA_H__
#define __NTL_TYPE_ANIM_DATA_H__

#include "NtlAnimEventData.h"
#include "NtlDebug.h"
#include "NtlCharacterData.h"
#include "NtlSerializer.h"

/// Flag-related settings
#define ANIM_FLAG_CULL_TEST_ALL_ATOMIC  0x00000001

/*!
 *\Animation
 *STypeAnimData: Data that does not change (read only)
 * 
 */
class AnimEvent_CompareFunc : public std::binary_function<SEventAnim *, SEventAnim *, bool> 
{
public:
	bool operator() (const SEventAnim* lhs, const SEventAnim* rhs) const
	{
		//if(lhs->fTime != ANIM_EVENT_NONE_TIME && rhs->fTime != ANIM_EVENT_NONE_TIME)
			return lhs->fTime < rhs->fTime;

		//return false;
	}
};

struct STypeAnimData
{
	std::string			strAnimName;							///< Animation File Name
	RwUInt32			uiAnimKey;								///< Animation Key
	RwReal              fPlayTime;                              ///< PlayTime of Animation
    RwUInt32            flagAnim;                               ///< Animation-related setting flags

	std::vector<SEventAnim *> vecAnimEvent;						///< Animation Events (must be aligned in fTime.)
	RwBool						m_bReLoad;						///< When you need to read the resource again
	
	STypeAnimData() : uiAnimKey(INVALID_GRAPHIC_ANIMATION_ID), m_bReLoad(FALSE), fPlayTime(0.0f), flagAnim(0)
	{
		vecAnimEvent.reserve(15);
	}

	~STypeAnimData()
	{
		if(vecAnimEvent.size())
		{
			for(RwUInt32 i = 0; i < vecAnimEvent.size(); i++)
			{
				if(vecAnimEvent[i] != NULL)
				{
                    // Because Multi Hissidan data is dynamically assigned to Hit Events,
                    // It must be deleted after conversion.
                    if(vecAnimEvent[i]->eEventID == EVENT_ANIM_HIT)
                    {
                        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)vecAnimEvent[i];
                        NTL_DELETE(pEventAnimHit);
                    }
                    else
                    {
					    NTL_DELETE (vecAnimEvent[i]);
                    }
				}
			}
		}

		vecAnimEvent.clear();
	}

	void AnimEventSort()
	{
		AnimEvent_CompareFunc cf;
		sort(vecAnimEvent.begin(), vecAnimEvent.end(), cf);
	}

    //---------Flag related
    void    SetCullTestAllAtomic(RwBool bFlag) {flagAnim |= ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
    RwBool  IsCullTestAllAtomic() {return flagAnim & ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
};


typedef	std::map<RwUInt32, STypeAnimData *>		      TYPE_ANIM_MAP;
typedef	std::map<RwUInt32, STypeAnimData *>::iterator TYPE_ANIM_MAP_ITER;

class CNtlTypeAnimTable
{
public:
	CNtlTypeAnimTable() {};
	~CNtlTypeAnimTable();

protected:
	TYPE_ANIM_MAP	m_mapTypeAnim;	
    std::string     m_strAnimPath;                                          ///< Animation Folder Path

public:
	void	Create();
	void	Destroy();
	
	STypeAnimData	*Get(RwUInt32 uiKey);									///< Get STypeAnimData
	STypeAnimData	*Add(RwUInt32 uiKey, const std::string &strAnimName);	///< Add STypeAnimData
    STypeAnimData	*Add(RwUInt32 uiKey, STypeAnimData *pTypeAnimData);     ///< Add STypeAnimData 
    void            Remove(RwUInt32 uiKey);                                 ///< Remove data from the map. (by Agebreak. 2006.05.01)

    TYPE_ANIM_MAP	*GetTypeAnimMap();

	const std::string	*GetFileName(RwUInt32 uiKey);								///< Get SixName

    void SetAnimPath(std::string& strAnimPath) { m_strAnimPath = strAnimPath;}
    std::string GetAnimPath() {return m_strAnimPath;}

	void	SaveSerialize( CNtlSerializer& sOut);						///< Save data in the Serialize object.
	void	LoadSerialize( CNtlSerializer& sIn);						///< Load data from the Serialize object.
};

#endif