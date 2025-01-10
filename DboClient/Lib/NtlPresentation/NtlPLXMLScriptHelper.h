#pragma once

#include "NtlXMLDoc.h"
#include "NtlSharedCommon.h"
#include "NtlAnimEventData.h"
#include "NtlTypeAnimData.h"

/**
* \ingroup NtlPresentation
* \brief Events among XML Scripts used in Model Tool are common.
*Helper class to collect common code to store these common events 
* \date 2007-10-25
* \author agebreak
*/
class CNtlPLXMLScriptHelper : public CNtlXMLDoc
{
public:
    CNtlPLXMLScriptHelper(void);
    ~CNtlPLXMLScriptHelper(void);

    // Common event save/load helper functions
    RwBool  SaveTMQEvent(IXMLDOMElement* pElemEvent, const SEventAnimCinematic* pEventTMQ);               ///< Save the TMQ Event.
    RwBool  LoadTMQEvent(IXMLDOMNode* pNodeEvent, SEventAnimCinematic* pEventTMQ);

    RwBool  SaveAlphaEvent(IXMLDOMElement* pElemEvent, const SEventAlpha* pEventAlpha);         ///< Save the Alpha Event.
    RwBool  LoadAlphaEvent(IXMLDOMNode* pNodeEvent, SEventAlpha* pEventAlpha);                  ///< Load Alpha Event.

    RwBool  SaveExplosionEvent(IXMLDOMElement* pElemEvent, const SEventExplosion* pEventExplosion); ///< Save the explosion event.
    RwBool  LoadExplosionEvent(IXMLDOMNode* pNodeEvent, SEventExplosion* pEventExplosion);          ///< Load the explosion event.

    RwBool  SaveSoundEvent(IXMLDOMElement* pElemEvent, const SEventSound* pEventSound);         ///< Save the Sound Event.
    RwBool  LoadSoundEvent(IXMLDOMNode* pNodeEvent, SEventSound* pEventSound);                  ///< Load Sound Event.

    RwBool  SaveEffectEvent(IXMLDOMElement* pElemEvent, const SEventVisualEffect* pEventEffect);    ///< Saves the visual effect event.
    RwBool  LoadEffectEvent(IXMLDOMNode* pNodeEvent, SEventVisualEffect* pEventEffect);             ///< Loads the visual effect event.

    RwBool  SaveAnimData(IXMLDOMElement* pElemAnimData, const STypeAnimData* pAnimData);        ///< Save animation data.    
    RwBool  LoadAnimData(IXMLDOMNode* pNodeAnimData, STypeAnimData* pAnimData);                 ///< Load animation data.

    // Internal helper functions
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwInt32 nInt);                ///< Store the Int type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwUInt32 nInt);               ///< Store the Int type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwReal fFloat);               ///< Save the float type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const char* szChar);          ///< Save the char type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwV3d& v3d);            ///< Save the RwV3d type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwV2d& v2d);            ///< Save the RwV2d type in Attribute.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwRGBA& rgba);

protected:    
    static WCHAR   wBuf[128];
    static char    szBuf[128];    
};
