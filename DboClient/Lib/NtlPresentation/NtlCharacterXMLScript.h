#pragma once
#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLXMLScriptHelper.h"

// 1.1: Changed to not save/load bone scale data when bone scale is not applied

#define CHARACTER_XML_SCRIPT_VER        L"2"

/**
 * \ingroup NtlPresentation
 * \brief Class for saving/loading character script (XML)
 * \date 2006-05-16
 * \author agebreak
 */
class CNtlCharacterXMLScript : public CNtlXMLDoc
{
public:
    CNtlCharacterXMLScript(void);
    virtual ~CNtlCharacterXMLScript(void);

    RwBool SaveCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty);         ///< Save character properties as XML script.
    RwBool LoadCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty);         ///< Load the XML script and set it in the character properties.

protected:
    RwBool  SaveHeader(CNtlPLCharacterProperty* pProperty);                        ///< Save the Header part.
    RwBool  SaveBoneData(CNtlPLCharacterProperty* pProperty);                      ///< Save the Bone Data part
    RwBool  SaveLinkEffect(CNtlPLCharacterProperty* pProperty);                    ///< Save the Link Effect Data section.
    RwBool  SaveAnimTable(CNtlPLCharacterProperty* pProperty);                     ///< Save the AnimTable part.      

    RwBool  LoadHeader(CNtlPLCharacterProperty* pProperty);                        ///< Load the Header part.
    RwBool  LoadBoneData(CNtlPLCharacterProperty* pProperty);                      ///< Load the Bone Data part.
    RwBool  LoadLinkEffect(CNtlPLCharacterProperty* pProperty);                    ///< Load the Link Effect Data section.
    RwBool  LoadAnimTable(CNtlPLCharacterProperty* pProperty);                     ///< Loads the AnimTable part.


    //////////////////////////////////////////////////////////////////////////
    void    SaveHitEvent(IXMLDOMElement* pElemEvent, const SEventAnimHit* pEventHit);  ///< Save the Hit Event.    
    void    LoadHitEvent(IXMLDOMNode* pNodeEvent, SEventAnimHit* pEventHit);           ///< Load Hit Event.

    void    SaveTraceEvent(IXMLDOMElement* pElemEvent, const SEventTrace* pEventTrace);  ///< Save the Trace Event.    
    void    LoadTraceEvent(IXMLDOMNode* pNodeEvent, SEventTrace* pEventTrace);           ///< Load Trace Event.

    void    SaveSubWeaponEvent(IXMLDOMElement* pElemEvent, const SEventSubWeapon* pEventSubWeapon); ///< Save the SubWeapon Event.
    void    LoadSubWeaponEvent(IXMLDOMNode* pNodeEvent, SEventSubWeapon* pEventSubWeapon);          ///< Load SubWeapon Event.

	void	SavePostEffectEvent(IXMLDOMElement* pElemEvent, const SEventPostEffect* pEventPostEffect);	///< Save the Post Effect Event.
	void	LoadPostEffectEvent(IXMLDOMNode* pNodeEvent, SEventPostEffect* pPostEffect);				///< Load Post Effect Event.

    void    SaveFootStepEvent(IXMLDOMElement* pElemEvent, const SEventFootStep* pEventFootStep);        ///< Save the footstep event.
    void    LoadFootStepEvent(IXMLDOMNode* pNodeEvent, SEventFootStep* pFootStep);                      ///<Load the footstep event.

    void    SaveDirectEvent(IXMLDOMElement* pElemEvent, const SEventDirect* pEventDirect);
    void    LoadDirectEvent(IXMLDOMNode* pNodeEvent, SEventDirect* pEventDirect);

    void    SaveColorChangeEvent(IXMLDOMElement* pElemEvent, const SEventColorChange* pEvent);         
    void    LoadColorChangeEvent(IXMLDOMNode* pNodeEvent, SEventColorChange* pEvent);

    void    SaveStretchEvent(IXMLDOMElement* pElemEvent, const SEventStretch* pEvent);
    void    LoadStretchEvent(IXMLDOMNode* pNodeEvent, SEventStretch* pEvent);
    
    void    SaveTriggerEvent(IXMLDOMElement* pElemEvent, const SEventTrigger* pEvent);
    void    LoadTriggerEvent(IXMLDOMNode* pNodeEvent, SEventTrigger* pEvent);

    void    SaveSkillCancelEvent(IXMLDOMElement* pElemEvent, const SEventSkillCancel* pEvent);
    void    LoadSkillCancelEvent(IXMLDOMNode* pNodeEvent, SEventSkillCancel* pEvent);   

protected:
    IXMLDOMElement*     m_pElemRoot;             ///< Root Element
    CNtlPLXMLScriptHelper m_XMLScriptHelper;     ///< DBO XML Script Helper 
};
