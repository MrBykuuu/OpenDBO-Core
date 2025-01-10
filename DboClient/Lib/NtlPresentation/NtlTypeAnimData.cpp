#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlTypeAnimData.h"

CNtlTypeAnimTable::~CNtlTypeAnimTable() 
{
    Destroy();
}

void CNtlTypeAnimTable::Create()
{
	//m_strFilePath = strFilePath;
	//Add(COMMON_NORMAL_IDLE_LOOP, "A_HFI_F_Idle1.anm");
}

/**
 * .anm Resource Destroy
 */
void CNtlTypeAnimTable::Destroy()
{
	TYPE_ANIM_MAP_ITER stAnimMapIT  = m_mapTypeAnim.begin();
	TYPE_ANIM_MAP_ITER endAnimMapIT = m_mapTypeAnim.end();
	for(;stAnimMapIT != endAnimMapIT;)
	{
		STypeAnimData *pTypeAnimData = stAnimMapIT->second;
		if(pTypeAnimData)
		{
			NTL_DELETE(pTypeAnimData);
		}
		
		++stAnimMapIT;
	}
	
	m_mapTypeAnim.clear();
}


/**
 *Create a vaule with the key value. Used only during initial creation
 * \param strAnimName Animation file name
 * \return STypeAnimData
 */
STypeAnimData *CNtlTypeAnimTable::Add(RwUInt32 uiKey, const std::string &strAnimName)
{
    // Check where duplicate data is coming in (by agebreak. 2006.4.20)
    STypeAnimData *pTypeAnimData = NULL;

    if(m_mapTypeAnim.find(uiKey) != m_mapTypeAnim.end())
    {
        pTypeAnimData = m_mapTypeAnim[uiKey];    
    }
    else
    {
	    pTypeAnimData = NTL_NEW STypeAnimData;	    
    }

    pTypeAnimData->strAnimName = strAnimName;
    pTypeAnimData->uiAnimKey = uiKey;
    m_mapTypeAnim[uiKey] = pTypeAnimData;
	
	return m_mapTypeAnim[uiKey];
}


STypeAnimData *CNtlTypeAnimTable::Add(RwUInt32 uiKey, STypeAnimData *pTypeAnimData)
{
    // New data cannot be added using existing code.=_=; (By agebreak. 2006.4.25)	
	pTypeAnimData->uiAnimKey = uiKey;
	m_mapTypeAnim[uiKey] = pTypeAnimData;
	return m_mapTypeAnim[uiKey];
}

/**
 *Returns the Map used internally.
 * \return TYPE_ANIM_MAP *
 */
TYPE_ANIM_MAP *CNtlTypeAnimTable::GetTypeAnimMap()
{
	return &m_mapTypeAnim;
}

/**
 *Obtain STypeAnimData as the key value.
 * \param uiKey Unique Key value
 * \return STypeAnimData *
 */
STypeAnimData *CNtlTypeAnimTable::Get(RwUInt32 uiKey)
{
	if(m_mapTypeAnim.find( uiKey ) == m_mapTypeAnim.end())
		return NULL;
	
	STypeAnimData *pTypeAnimData = m_mapTypeAnim[uiKey];
	if(pTypeAnimData == NULL)
		return NULL;
		
	return pTypeAnimData;
}

/**
 *Obtain AnimFile as the key value.
 * \param uiKey Unique Key value
 * \return Animation File Name
 */
const std::string *CNtlTypeAnimTable::GetFileName(RwUInt32 uiKey)
{
	if(m_mapTypeAnim.find( uiKey ) == m_mapTypeAnim.end())
		return NULL;
	
	STypeAnimData *pTypeAnimData = m_mapTypeAnim[uiKey];
	if(pTypeAnimData == NULL)
		return NULL;
	
	return (&pTypeAnimData->strAnimName);
}

void CNtlTypeAnimTable::Remove(RwUInt32 uiKey)
{
    if(m_mapTypeAnim.find(uiKey) == m_mapTypeAnim.end())
        return;

    STypeAnimData* pTypeAnimData = m_mapTypeAnim[uiKey];
    if(pTypeAnimData)
    {
        NTL_DELETE(pTypeAnimData);
        pTypeAnimData = NULL;
    }

    m_mapTypeAnim.erase(uiKey);
}

void CNtlTypeAnimTable::SaveSerialize(  CNtlSerializer& sOut ) 
{
	sOut<<m_strAnimPath;
	sOut<<m_mapTypeAnim.size();				// Size
	TYPE_ANIM_MAP_ITER it = m_mapTypeAnim.begin();
	for(; it != m_mapTypeAnim.end(); ++it)
	{
		STypeAnimData* pTypeAnimData = it->second;
		if(!pTypeAnimData)
			continue;

		sOut<<pTypeAnimData->strAnimName;			// Anim Name
		sOut<<pTypeAnimData->uiAnimKey;				// Anim Key
		sOut<<pTypeAnimData->fPlayTime;				// Play Time
        sOut<<pTypeAnimData->flagAnim;              // Anim Flag

		sOut<<pTypeAnimData->vecAnimEvent.size();	// Event Size		
		for each(SEventAnim* pEventAnim in pTypeAnimData->vecAnimEvent)
		{
			if(pEventAnim)
			{
				sOut<<pEventAnim->eEventID;			// Enter the event type as a header.
				switch(pEventAnim->eEventID)
				{
				case EVENT_ANIM_HIT:	
                    {
                        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)pEventAnim;
                        pEventAnimHit->SaveSerialize(sOut);
                    }                    
					break;
				case EVENT_ANIM_VISUAL_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventVisualEffect));
					break;
				case EVENT_ANIM_VISUAL_SOUND:					
					sOut.In(pEventAnim, sizeof(SEventSound));
					break;
				case EVENT_ANIM_FOOT_STEP:					
					sOut.In(pEventAnim, sizeof(SEventFootStep));
					break;
				case EVENT_ANIM_LINK_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventLinkEffect));
					break;
				case EVENT_ANIM_WEIGHT_TIME:					
					sOut.In(pEventAnim, sizeof(SEventWeightTime));
					break;
				case EVENT_ANIM_TRACE_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventTrace));
					break;
				case EVENT_ANIM_SUB_WEAPON:					
					sOut.In(pEventAnim, sizeof(SEventSubWeapon));
					break;
				case EVENT_ANIM_POST_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventPostEffect));					
					break;		
				case EVENT_ANIM_SUMMON_PET:
					sOut.In(pEventAnim, sizeof(SEventSummonPet));
					break;
                case EVENT_ANIM_TMQ:
                    sOut.In(pEventAnim, sizeof(SEventAnimCinematic));
                    break;
                case EVENT_ANIM_ALPHA:
                    sOut.In(pEventAnim, sizeof(SEventAlpha));
                    break;
                case EVENT_ANIM_EXPLOSION:
                    sOut.In(pEventAnim, sizeof(SEventExplosion));
                    break;
                case EVENT_ANIM_DIRECT:
                    sOut.In(pEventAnim, sizeof(SEventDirect));
                    break;
                case EVENT_ANIM_COLOR_CHANGE:
                    sOut.In(pEventAnim, sizeof(SEventColorChange));
                    break;
                case EVENT_ANIM_STRETCH:
                    sOut.In(pEventAnim, sizeof(SEventStretch));
                    break;
                case EVENT_ANIM_TRIGGER:
                    sOut.In(pEventAnim, sizeof(SEventTrigger));
                    break;
                case EVENT_ANIM_SKILL_CANCEL:
                    sOut.In(pEventAnim, sizeof(SEventSkillCancel));
                    break;
				default:
					assert(!"Data is Wrong");
					break;
				}				
			}
		}
	}
}

void CNtlTypeAnimTable::LoadSerialize(  CNtlSerializer& sIn ) 
{
	sIn>>m_strAnimPath;
	UINT nAnimDataCount = 0;
	sIn>>nAnimDataCount;

	for(UINT i = 0; i < nAnimDataCount; ++i)
	{
		STypeAnimData* pTypeAnimData = NTL_NEW STypeAnimData();
		sIn>>pTypeAnimData->strAnimName;
		sIn>>pTypeAnimData->uiAnimKey;
		sIn>>pTypeAnimData->fPlayTime;
        sIn>>pTypeAnimData->flagAnim;
        Add(pTypeAnimData->uiAnimKey, pTypeAnimData);

		UINT nAnimEventSize = 0;
		sIn>>nAnimEventSize;
		for(UINT j = 0; j < nAnimEventSize; ++j)
		{
			int nEventID = 0;
			sIn>>nEventID;

			SEventAnim* pEventAnim = NULL;
			switch(nEventID)
			{
			case EVENT_ANIM_HIT:
                {
                    pEventAnim = NTL_NEW SEventAnimHit();				
                    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)pEventAnim;
                    pEventAnimHit->LoadSerialize(sIn);
                }				
				break;
			case EVENT_ANIM_VISUAL_EFFECT:
				pEventAnim = NTL_NEW SEventVisualEffect();
				sIn.Out(pEventAnim, sizeof(SEventVisualEffect));
				break;
			case EVENT_ANIM_VISUAL_SOUND:
				pEventAnim = NTL_NEW SEventSound();
				sIn.Out(pEventAnim, sizeof(SEventSound));
				break;
			case EVENT_ANIM_FOOT_STEP:
				pEventAnim = NTL_NEW SEventFootStep();
				sIn.Out(pEventAnim, sizeof(SEventFootStep));
				break;
			case EVENT_ANIM_LINK_EFFECT:
				pEventAnim = NTL_NEW SEventLinkEffect();
				sIn.Out(pEventAnim, sizeof(SEventLinkEffect));
				break;
			case EVENT_ANIM_WEIGHT_TIME:
				pEventAnim = NTL_NEW SEventWeightTime();
				sIn.Out(pEventAnim, sizeof(SEventWeightTime));
				break;
			case EVENT_ANIM_TRACE_EFFECT:
				pEventAnim = NTL_NEW SEventTrace();
				sIn.Out(pEventAnim, sizeof(SEventTrace));
				break;
			case EVENT_ANIM_SUB_WEAPON:
				pEventAnim = NTL_NEW SEventSubWeapon();
				sIn.Out(pEventAnim, sizeof(SEventSubWeapon));
				break;
			case EVENT_ANIM_POST_EFFECT:
				pEventAnim = NTL_NEW SEventPostEffect();
				sIn.Out(pEventAnim, sizeof(SEventPostEffect));
				break;		
			case EVENT_ANIM_SUMMON_PET:
				pEventAnim = NTL_NEW SEventSummonPet();
				sIn.Out(pEventAnim, sizeof(SEventSummonPet));
				break;
            case EVENT_ANIM_TMQ:
				pEventAnim = NTL_NEW SEventAnimCinematic;
				sIn.Out(pEventAnim, sizeof(SEventAnimCinematic));
                break;
            case EVENT_ANIM_ALPHA:
                pEventAnim = NTL_NEW SEventAlpha();
                sIn.Out(pEventAnim, sizeof(SEventAlpha));
                break;
            case EVENT_ANIM_EXPLOSION:
                pEventAnim = NTL_NEW SEventExplosion();
                sIn.Out(pEventAnim, sizeof(SEventExplosion));
                break;
            case EVENT_ANIM_DIRECT:
                pEventAnim = NTL_NEW SEventDirect();
                sIn.Out(pEventAnim, sizeof(SEventDirect));
                break;
            case EVENT_ANIM_COLOR_CHANGE:
                pEventAnim = NTL_NEW SEventColorChange();
                sIn.Out(pEventAnim, sizeof(SEventColorChange));
                break;
            case EVENT_ANIM_STRETCH:
                pEventAnim = NTL_NEW SEventStretch();
                sIn.Out(pEventAnim, sizeof(SEventStretch));
                break;
            case EVENT_ANIM_TRIGGER:
                pEventAnim = NTL_NEW SEventTrigger();
                sIn.Out(pEventAnim, sizeof(SEventTrigger));
                break;
            case EVENT_ANIM_SKILL_CANCEL:
                pEventAnim = NTL_NEW SEventSkillCancel();
                sIn.Out(pEventAnim, sizeof(SEventSkillCancel));
                break;
            default:
                assert(!"Data is Wrong");
                break;
			}

			DBO_ASSERT(pEventAnim, "CNtlTypeAnimTable::LoadSerialize::pEventAnim, NULL");
			if(pEventAnim)
			{
				pTypeAnimData->vecAnimEvent.push_back(pEventAnim);
			}			
		}		
	}
}
