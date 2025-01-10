/*****************************************************************************
 *
 * File			: NtlPLPlayerName.h
 * Author		: HyungSuk Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 11. 24	
 * Abstract		: Presentation layer player name entity class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLPLAYERNAME_H__
#define __NTL_PLPLAYERNAME_H__

#include "NtlPLAttach.h"
#include "NtlPLGlobal.h"
#include "NtlPLEmblemMaker.h"


#include "gui_font.h"
#include "gui_texture.h"

// The most clearly visible polygon size in 1 size of texture
// Multiplying the Textrue Size gives the most clearly visible polygon size.
// ex) 512 * PLAYERNAME_BOX_BASIS_CX = 2.0f
#define PLAYERNAME_BOX_BASIS_CX			0.00390625f
#define PLAYERNAME_BOX_BASIS_CY			0.00625f

#define PLAYERNAME_EMBLEM_BASIS_CX		0.0025f
#define PLAYERNAME_EMBLEM_BASIS_CY		0.003f

#define PLAYERNAME_BOX_OFFSET			0.02f;
#define PLAYERNAME_NICK_OFFSET			0.08f;

#define PLAYERNAME_VERTEX_COUNT			4

// When outputting the player name, curl according to distance.
#define PLAYERNAME_DIST							100.0f // changed from 40 to 100 by daneos. To make names visible
#define PLAYERNAME_CAMERA__BOUNDING_RADIUS		0.1f

#define PLAYERNAME_LINE_INTERVAL_Y				5

// FONT SIZE
//#define dNTL_PLAYERNAME_FONTSIZE_NAME	230 
//#define dNTL_PLAYERNAME_FONTSIZE_GUILD	180
// modified by kell ( 2008. 7. 1 Planning Team Request )
#define dNTL_PLAYERNAME_FONTSIZE_NAME	200 
#define dNTL_PLAYERNAME_FONTSIZE_GUILD	200
#define dNTL_PLAYERNAME_FONTSIZE_NICK	190
#define dNTL_PLAYERNAME_FONTSIZE_TITLE	200 

// Flag
#define dNTL_PL_FLAG_NAME		0x00000001
#define dNTL_PL_FLAG_GUILD		0x00000002
#define dNTL_PL_FLAG_EMBLEM		0x00000004
#define dNTL_PL_FLAG_NICK		0x00000008
#define dNTL_PL_FLAG_TITLE		0x00000010

/**
* \brief Data structure of PlayerNameEntity
*/
struct SPLPlayerNameCreateParam : public SPLEntityCreateParam
{
	const WCHAR		*pName;
	COLORREF		Color;
	RwReal			fCullDist;
	const WCHAR		*pGuildName;
	COLORREF		ColorGuild;
	const WCHAR		*pTitleName;
	COLORREF		ColorTitle;
	sEmblemFactor	*pEmblemFactor;

	SPLPlayerNameCreateParam()
		:pName(NULL)
		,Color(RGB(255,255,255))
		,fCullDist(PLAYERNAME_DIST)
		,pGuildName(NULL)
		,ColorGuild(RGB(255,255,255))
		, pTitleName(NULL)
		, ColorTitle(RGB(255, 255, 255))
		,pEmblemFactor(NULL)
	{
	}
};

/**
* \ingroup presentation
*/
class CNtlPLPlayerName : public CNtlPLAttach
{
public:
	CNtlPLPlayerName();
	virtual ~CNtlPLPlayerName();

	RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	void	Destroy(void);
	RwBool	Update(RwReal fElapsed);
	RwBool	Render(void);
	RwBool	SetProperty(const CNtlPLProperty *pData) { return TRUE;}

	void    SetAlpha(RwUInt8 byValue);
	__inline RwUInt8 GetAlpha() {return m_byAlpha;}

	void*	operator new(size_t size);
	void	operator delete(void *pObj);

	RwBool	IsExistEmblemTexture();

	void	SetVisible(RwBool bVisible);
	void	EnablePlayerNameVisible(RwBool bVisible);
	RwBool	IsEnablePlayerNameVisible();
	void	SetCameraCullin(RwBool bCullin);

protected:
	void	AllVertexInit(void);
	void	VertexAssign(void);

	void	NameVertexAssign(void);
	void	EmblemVertexAssign(void);
	void	NickNameVertexAssign(void);

	RwReal	GetAdjustPolygonWidth(RwInt32 nSize);
	RwReal	GetAdjustPolygonHeight(RwInt32 nSize);

	RwBool	CreateNameTexture(const WCHAR* pName);
	RwBool	CreateTitleNameTexture(const WCHAR* pTitleName);
	RwBool	CreateGuildNameTexture(const WCHAR* pGuildName);
	RwBool	CreateGuildEmblemTexture(sEmblemFactor* pEmblemFactor);
	RwBool	CreateNickNameTexture(const WCHAR* pNickName);

	RwBool	RenderName(void);
	RwBool	RenderEmblem(void);
	RwBool	RenderNick(void);

public:
	void	SetPosition(const RwV3d *pPos);
	RwV3d	GetPosition(void);

	void	SetNameColor(const WCHAR* pwcName, COLORREF color);
	void	SetNameColor(const WCHAR* pwcName, COLORREF color, const WCHAR* pwcGuildName, COLORREF guildcolor
		, sEmblemFactor* pEmblemFactor, const WCHAR* pwcTitleName, COLORREF titlecolor);
	void	SetNickNameColor(const WCHAR* pwcTitle, COLORREF color );

protected:
	RwBool	m_bCameraCullIn;				///< Is it culled by the camera?
	RwReal	m_fCullDist;					///< Curling distance

	RwBool m_bNameVisible;					///< Whether or not the PlayerName can be Visible
	RwBool m_bTitleNameVisible;				///< Whether or not the TitleName can be Visible
	
	gui::CGuiFont *m_pFont;					///< Name Font
	gui::CGuiFont *m_pGuildFont;			///< Guild Name Font
	gui::CGuiFont *m_pNickFont;				///< Nick Font
	gui::CGuiFont *m_pTitleFont;			///< Title Name Font

	gui::CTexture *m_pTexture;				///< Name & Guild Texture & title
	RwTexture *m_pEmblemTexture;			///< Emblem Texture
	gui::CTexture *m_pNickTexture;			///< Nick Texture

	CPos	m_posNameSize;					///< The pixel size of the name.
	CPos	m_posGuildNameSize;				///< The pixel size of the guild name
	CPos	m_posNickNameSize;				///< Nick's pixel size
	CPos	m_posTitleNameSize;				///< Title pixel size

	RwV3d			m_v3WorldPosition;		///< World position of PlayerName Entity
	
	RwV3d			m_v3Pos[PLAYERNAME_VERTEX_COUNT];				///< Local coordinates of name and guild name
	RwIm3DVertex	m_3dVertices[PLAYERNAME_VERTEX_COUNT];			///< Vertex buffer to be used when drawing the actual object

	RwV3d			m_v3EmblemPos[PLAYERNAME_VERTEX_COUNT];			///< Local coordinates of the guild emblem
	RwIm3DVertex	m_3dEmblemVertices[PLAYERNAME_VERTEX_COUNT];	///< Vertex buffer to be used when drawing the emblem object

	RwV3d			m_v3NickPos[PLAYERNAME_VERTEX_COUNT];
	RwIm3DVertex	m_3dNickVertices[PLAYERNAME_VERTEX_COUNT];

	RwMatrix		m_matWorld;				///< World matrix obtained from camera inverse matrix and scale value
    RwUInt8			m_byAlpha;				///< Alpha of PlayerName

	RwUInt32		m_byFlag;				///< Status tag that is output or calculates local coordinates
};

#endif
