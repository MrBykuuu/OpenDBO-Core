/******************************************************************************
* File			: WorldMapScript.h
* Author		: Hong SungBock
* Copyright		: NTL Co., Ltd.
* Date			: 2009. 1. 5
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: Reads the map tree structure script used in the world map.
*****************************************************************************/

#pragma once

class CNtlTokenizer;


struct sMAP_NODE;
typedef std::map<RwUInt32, sMAP_NODE*>					MAP_MAPNODE;

struct sMAP_NODE
{
	RwUInt32			uiNodeID;

	// for current node
	std::string			strMainTexture;
	std::string			strChildResourceFile;
	std::string			strChildSurfaceFile; 

	// for parent node
	std::string			strFocusSurfaceName;

	RwInt32				iFocusPosX;
	RwInt32				iFocusPosY;

	MAP_MAPNODE			mapChildNode;

	sMAP_NODE() : iFocusPosX(0), iFocusPosY(0)
	{}
};


class CWorldMapScript
{
public:
	CWorldMapScript();
	virtual ~CWorldMapScript();

	sMAP_NODE*		LoadScript();

	VOID			UnloadScript(sMAP_NODE* pNode);

protected:
	sMAP_NODE*		LoadChildNode(CNtlTokenizer* pLexer);
};