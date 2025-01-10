#ifndef __NTL_ITEM_PROPERTY_H__
#define __NTL_ITEM_PROPERTY_H__

#include "NtlPLProperty.h"
#include "NtlAnimEventData.h"
#include "NtlPLItemData.h"
#include "NtlTypeAnimData.h"

// Item Flags
#define ITEM_FLAG_APPLY_EMBLEM        0x00000001

class CNtlPLItemProperty : public CNtlPLProperty
{
public:
	static unsigned int m_strVer;				///< Property File Version
	static std::string m_strItemDataPath;		///< Item Data File Path

    std::vector<SEventLinkEffect*> m_vLinkEffect;     ///< List of Link Effects     
    SEventTrace        m_eventTrace;            //< Trajectory Effect Properties

protected:
	std::string			m_TypeMeshPath;			///< Item Mesh File Path
	std::string			m_TypeMeshFileName;		///< Item Mesh File Name(*.dff)

	EItemResType		m_TypeResType;			///< ResType
	EItemEquipSlotType	m_TypeEquipSlotType;	///< Equip Slot Type

    CNtlTypeAnimTable   m_TypeAnimTable;        ///< Animation Table
    RwV3d               m_v3dAttachOffset;      ///< Item Attach Offset

    RwBool              m_bTraceEnableDefault;  ///< Whether or not the trajectory effect is displayed Default value
    
    SUpgradeEffectProperty m_UpgradeEffectProp; ///< Upgrade Effect Related Properties  

public:
	CNtlPLItemProperty();
	virtual ~CNtlPLItemProperty();

	// Function to get a list of properties from 'ItemPropertyList.xml'
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

    RwBool	LoadScript(const std::string &strFileName);							    	///< Load XML Script
    RwBool  SaveScript(const std::string &strFileName);                                 ///< XML Script Save    

	virtual void	SaveSerialize(CNtlSerializer& sOut);											///< Function to save data in Serialize
	virtual void	LoadSerialize(CNtlSerializer& sIn);												///< Function to retrieve data from Serialize

    CNtlTypeAnimTable* GetAnimTable() {return &m_TypeAnimTable;}                        ///< Returns AnimTable.
	
	std::string	&GetMeshFileName()	{ return m_TypeMeshFileName; }						///< Get Mesh Name
	void SetMeshFileName(const std::string &strName) { m_TypeMeshFileName = strName; }	///< Set Mesh Name

	const std::string &GetMeshPath() { return m_TypeMeshPath; } 						///< Get Mesh Path
	void SetMeshPath(const std::string &strPath)	{ m_TypeMeshPath = strPath; }		///< Set Mesh Path

	void SetResType(EItemResType eResType) { m_TypeResType = eResType; }
	EItemResType GetResType() { return m_TypeResType; }

	void SetEquipSlotType(EItemEquipSlotType eEquipSlotType) { m_TypeEquipSlotType = eEquipSlotType; }
	EItemEquipSlotType GetEquipSlotType() { return m_TypeEquipSlotType; }

    void SetAttachOffset(const RwV3d& v3dOffset) {m_v3dAttachOffset = v3dOffset;}       ///< Set the Attach Offset value of the Item.
    RwV3d GetAttachOffset() {return m_v3dAttachOffset;}                                 ///< Returns the Attach Offset value of the Item.

    void    SetTraceEnableDefault(const RwBool bEnable) {m_bTraceEnableDefault = bEnable;}  ///< Set the default value for the presence or absence of trajectory display.
    RwBool  GetTraceEnableDefault() {return m_bTraceEnableDefault;}                         ///< Returns the default value for whether the trajectory is displayed.

    SUpgradeEffectProperty* GetUpgradeEffectProperty() {return &m_UpgradeEffectProp;}    ///< Returns the property of the Upgrade Effect.

    // Flag comparison function
    RwBool  IsApplyEmblem() {return (GetFlag() & ITEM_FLAG_APPLY_EMBLEM);}                ///< Returns whether the Emblem is an applied item.
};

#endif