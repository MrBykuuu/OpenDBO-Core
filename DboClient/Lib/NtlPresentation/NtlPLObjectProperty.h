/*****************************************************************************
 *
 * File			: NtlPLObjectProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: NTL Co., Ltd.
 * Date			: 2005. 8. 22	
 * Abstract		: Presentation layer object property class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLOBJECT_PROPERTY_H__
#define __NTL_PLOBJECT_PROPERTY_H__

#include "NtlPLProperty.h"
#include "NtlTypeAnimData.h"

#define XML_PARSING_BUFFER_SIZE		( 1024 )

/// type of object
enum EPLObjectType
{
    E_OBJECT_NORMAL = 0,                        ///< General object
    E_OBJECT_MILEPOST,                          ///< Objects for signposts (arrows)
};

/// Object flags (I'd like to replace the existing bool values with flags as well, but that would break backwards compatibility)
#define OBJ_FLAG_SORT_BY_POS        0x00000001
#define OBJ_FLAG_FORCE_PICKING      0x00000002
#define OBJ_FLAG_PE_NAVI_PRESS      0x00000004
#define OBJ_FLAG_CULLTEST_ATOMIC    0x00000008

/**
* \ingroup NtlPresentation
*This is a property class corresponding to CNtlPLObject.
*Property data is written as an xml file.
*
*/
class CNtlPLObjectProperty : public CNtlPLProperty
{
public:

	static unsigned int m_strVer;			/* property file version **/
	static std::string m_strObjectDataPath;	/* Object Data File Path(*.xml) **/
	static std::string m_strMeshPath;		/* object mesh file의 path name **/
	static std::string m_strAniPath;		/* object animation file의 path name **/
	static std::string m_strTexPath;		/* object texture file의 path name **/

	std::string m_strDffFile;				/* object mesh file name **/
    std::string m_strUVAnimFileName;        ///< UVAnim File Name (including relative path)
    std::string m_strAnimFileName;          ///< AnimFileName (including relative path)    
    std::string m_strCollisionMeshName;     ///< Path Engine용 Collision Mesh File Name

	std::vector<SEventLinkEffect*> m_vLinkEffect; ///< List of Link Effects 

protected:
    RwBBox          m_bbox;                         ///< Server Object의 BBox 
    RwBool          m_bCollision;                   ///< Presence of Server Collision
	RwBool          m_bShadow;                      ///< presence or absence of shadow display    
    RwReal          m_fUvAnimSpeed;                 ///< UVAnim Speed    
    RwReal          m_fCullingDistance;             ///< Culling Distance
    RwBool          m_bApplyPVS;                    ///< Whether PVS is affected or not
    RwInt32         m_nObjectFlag;

	CNtlTypeAnimTable m_TypeAnimTable;				///< Animation Table
    EPLObjectType   m_eObjectType;                  ///< Object Type

    SEventSound     m_SoundProp;                    ///< Basic playback sound properties

    

public:
	CNtlPLObjectProperty();
	virtual ~CNtlPLObjectProperty();

	// Function to retrieve property file name from ‘ObjectPropertyList.xml’
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);	
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

    virtual RwBool  SaveScript(const std::string &strFileName);          ///< Save it as an XML script file.
    virtual RwBool  LoadScript(const RwChar* strFileName);          ///< Load data from XML script.

	virtual void	SaveSerialize(CNtlSerializer& sOut);											///< Function to save data in Serialize
	virtual void	LoadSerialize(CNtlSerializer& sIn);												///< Function to retrieve data from Serialize
	
	void                    SetDffName(const RwChar* strDffName) {m_strDffFile = strDffName;}
	const char *			GetDffName( void ) const { return m_strDffFile.c_str(); }

    void    SetEnableShadow(RwBool bEnable) {m_bShadow = bEnable;}
    RwBool  GetEnableShadow() {return m_bShadow;}

    void    SetBBox(RwBBox bbox) {m_bbox = bbox;};                                  ///< Set up BBox
    RwBBox* GetBBox() {return &m_bbox;}                                             ///< Returns the set BBox.

    void    SetUVAnimSpeed(RwReal fSpeed) {m_fUvAnimSpeed = fSpeed;}                ///< Set UVAnim speed
    RwReal  GetUVAnimSpeed() {return m_fUvAnimSpeed;}                               ///< Returns UVAnim speed.

    void    SetCollision(RwBool bCollision) {m_bCollision = bCollision;}            ///< Set whether collision is possible or not.
    RwBool  GetCollision() {return m_bCollision;}                                   ///< Returns whether collision is possible or not.

    void    SetCullingDistance(RwReal fDistance) {m_fCullingDistance = fDistance;}  ///< Set Culling Distance.
    RwReal  GetCullingDistance() {return m_fCullingDistance;}                       ///< Returns Culling Distance.

	CNtlTypeAnimTable* GetAnimTable() {return &m_TypeAnimTable;}                        ///< Returns AnimTable.

    void    SetObjectType(EPLObjectType eObjectType) {m_eObjectType = eObjectType;} ///< Set the Object Type.
    EPLObjectType GetObjectType() {return m_eObjectType;}                           ///< Returns Object Type.

    void    SetApplyPVS(RwBool bApplyPVS) {m_bApplyPVS = bApplyPVS;}                ///< Set whether to be affected by PVS or not.
    RwBool  GetApplyPVS() {return m_bApplyPVS;}                                     ///< Returns whether PVS is affected or not.

    SEventSound* GetSoundProp() {return &m_SoundProp;}                              ///< Returns the sound settings of the object.

    // Flag-related functions
    RwBool  IsSortByPos() {return (GetFlag() & OBJ_FLAG_SORT_BY_POS);}
    RwBool  IsForcePicking() {return (GetFlag() & OBJ_FLAG_FORCE_PICKING);}
    RwBool  IsNaviPress()   {return (GetFlag() & OBJ_FLAG_PE_NAVI_PRESS);}           ///< Settings for Obstacle Mesh for pass engine
    RwBool  IsCullTestAllAtomic() {return (GetFlag() & OBJ_FLAG_CULLTEST_ATOMIC);}     ///< Returns whether or not to perform a Culling Test for Atomic in the animation.

};
#endif