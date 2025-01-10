/*****************************************************************************
 *
 * File			: NtlPLCharacterProperty.h
 * Author		: HongHoDong
 * Copyright	: (?)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: Presentation layer object property class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CHARACTER_PROPERTY_H__
#define __NTL_CHARACTER_PROPERTY_H__

//#include <string>
#include "NtlPLProperty.h"
#include "NtlCharacterData.h"
#include "NtlTypeAnimData.h"
#include "NtlPLEntityRenderHelpers.h"

// character flag
#define CHAR_FLAG_APPLY_NORMAL       0x00000001            ///< Affected by terrain curvature.

struct sUNKNOWN_CHAR_PROP
{
	int		nUnknown;
	RwV3d	m_vUnknownPos;
	RwV3d	m_vUnknownPos2;
};

/**
 * \ingroup NtlPresentation
 *This is a property class corresponding to CNtlPLCharacter.
 *Property data is written as an xml file.
 *
 */
class CNtlPLCharacterProperty : public CNtlPLProperty
{
public:
	static unsigned int m_strVer;							///< Property File Version
	static std::string m_strCharacterDataPath;				///< Character Data File Path(*.script)
	
	std::string			m_strTexturePath;					///< Character Texture File Path(*.texture)        
    std::string         m_strSoundPath;                     ///< Sound File Path
    std::string			m_TypeMeshFileName;					///< Base Mesh(Bone)	TypeData
    std::string         m_strMeshPath;                      ///< Base Mesh Path

	std::vector<SEventLinkEffect*> m_vLinkEffect;                 ///< Link Effect? ??? 

	std::vector<sUNKNOWN_CHAR_PROP*> m_vLinkUnknownProp;

protected:	
	CNtlTypeAnimTable	m_TypeAnimTable;					///< Animation Resource TypeData
	STypeBoneData		m_TypeBoneData;						///< Bone Scale Data    TypeData
    RwBBox              m_bboxAnim;                         ///< BBox of Animation    
    RwV3d               m_vFaceCameraPos;                   ///< Face Camera Position
    RwV3d               m_vFaceCameraLookAt;                ///< Face Camera LookAt Vector	
	RwReal				m_fInitWalkFrontAnimSpeed;		
	RwReal				m_fInitRunFrontAnimSpeed;    
    RwReal              m_fAlphaDistance;                   ///< Set the distance from the camera that the alpha will eat

public:
	CNtlPLCharacterProperty();
	virtual ~CNtlPLCharacterProperty();
	
	// Function to get a list of properties from 'CharacterPropertyList.xml'
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

	RwBool	LoadScript(const std::string &strFileName);										///< Load XML Script
    RwBool  SaveScript(const std::string &strFileName);                                     ///< XML Script Save

	virtual void	SaveSerialize(CNtlSerializer& sOut);											///< Function to save data in Serialize
	virtual void	LoadSerialize(CNtlSerializer& sIn);												///< Function to retrieve data from Serialize
	
	STypeBoneData		*GetBoneScaleData()	{ return &m_TypeBoneData; }						///< Get BoneScale Data
	CNtlTypeAnimTable	*GetAnimTable()		{ return &m_TypeAnimTable; }					///< Get Anim Table
	
	std::string	&GetBaseMeshFileName()	{ return m_TypeMeshFileName; }						///< Get Base Mesh Name
	void SetBaseMeshFileName(const std::string &strName) { m_TypeMeshFileName = strName; }	///< Set Base Mesh Name

    void SetBaseMeshFilePath(const std::string &strPath) { m_strMeshPath = strPath;}
    std::string& GetBaseMeshFilePath() {return m_strMeshPath;}
	
	const std::string &GetTexturePath() { return m_strTexturePath; }						///< Get Texture path
	void SetTexturePath(const std::string &strPath) { m_strTexturePath = strPath; }			///< Set Texture Path

    const std::string& GetSoundPath() {return m_strSoundPath;}                              ///< Get Sound Path
    void SetSoundPath(const std::string& strPath) {m_strSoundPath = strPath;}               ///< Set Sound Path

    void SetAnimBBox(RwBBox bboxAnim) {m_bboxAnim = bboxAnim;};                             ///< Set Animation BBox
    RwBBox* GetAnimBBox() {return &m_bboxAnim;};                                            ///< Get Animation BBox

    // Face Camera 
    void  SetFaceCameraPos(const RwV3d& vFaceCameraPos) {m_vFaceCameraPos = vFaceCameraPos;} ///< Set Face Camera Position
    RwV3d GetFaceCameraPos() {return m_vFaceCameraPos;}                                     ///< Get Face Camera Position

    void SetFaceCameraLookAt(const RwV3d& vLookAt) {m_vFaceCameraLookAt = vLookAt;}          ///< Set Face Camera LookAt Height
    RwV3d GetFaceCameraLookAt() {return m_vFaceCameraLookAt;}                                ///< Get Face Camera LookAt Height

	// Walk Speed
	void	SetInitWalkFrontAnimSpeed(RwReal fAnimSpeed) {m_fInitWalkFrontAnimSpeed = fAnimSpeed;}
	RwReal	GetInitWalkFrontAnimSpeed() {return m_fInitWalkFrontAnimSpeed;}

	void	SetInitRunFrontAnimSpeed(RwReal fAnimSpeed) {m_fInitRunFrontAnimSpeed = fAnimSpeed;}
	RwReal	GetInitRunFrontAnimSpeed() {return m_fInitRunFrontAnimSpeed;}
    
    void    SetAlphaDistance(RwReal fAlphaDist) {m_fAlphaDistance = fAlphaDist;}
    RwReal  GetAlphaDistance() {return m_fAlphaDistance;}

    // Flag-related functions
    RwBool  IsApplyNormal() {return (GetFlag() & CHAR_FLAG_APPLY_NORMAL);}                  ///< Returns whether it is affected by terrain bends.
};

#endif

