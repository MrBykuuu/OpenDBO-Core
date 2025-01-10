#pragma once

//|-------------------------------------------------------------------------------------------------------------------
//|	Topic	| CNtlPLOptionManager
//|---------|---------------------------------------------------------------------------------------------------------
//|	Content | Class for Visual Option Test
//|---------|---------------------------------------------------------------------------------------------------------
//|	Author	| Cz
//|-------------------------------------------------------------------------------------------------------------------

class CNtlPLOptionManager
{
private:
	static CNtlPLOptionManager*		g_pOptionManager;

public:
	static CNtlPLOptionManager*		GetInstance() { return g_pOptionManager; }
	static void						CreateInstance();
	static void						DestroyInstance();

public:
	CNtlPLOptionManager();
	virtual ~CNtlPLOptionManager();

	RwBool		Create();
	void		Destroy();

	RwBool		Update(const RwCamera* pCameram, RwReal fElapsed);

public:
	// Object viewing distance
	void		SetObjectFar(RwReal fFar);
	RwReal		GetObjectFar();	
	
	// Terrain visibility
	void		SetTerrainFar(RwReal fFar);
	RwReal		GetTerrainFar();

	// Presence of Terrain Shadow Render
	void		SetTerrainShadow(RwBool bRender);
	RwBool		GetTerrainShadow();

	void		SetWaterSpecular(RwBool bRender);
	RwBool		GetWaterSpecular();

	// weather details
	void		SetWeatherLevel(RwUInt32 uiLevel);
	RwUInt32	GetWeatherLevel();

	// full visibility
	void		SetPlantClipDist(RwReal fDist);
	RwReal		GetPlantClipDist();

	// presence or absence of haze
	void		SetHeatHaze(RwBool bUse);
	RwBool		GetHeatHaze();

	// Texture level //the lower the higher. 0 == original.
	void		SetTextureQuality(RwUInt32 uiQualityLevel);
	RwUInt32	GetTextureQuality();

#ifdef dNTL_WORLD_COMBINE
	// Terrain Texture Combine // I will not use it.
	void		SetTerrainTextrueCombine(RwBool bUse);
	RwBool		GetTerrainTextrueCombine();
#endif

private:
	RwReal			m_fFarObject;
	RwReal			m_fFarTerrain;

	RwBool			m_bWaterSpecular;

	RwUInt32		m_uiWeatherLevel;

#ifdef dNTL_WORLD_COMBINE
	RwBool			m_bTerrainCombine;
#endif
};

inline CNtlPLOptionManager* GetNtlPLOptionManager(void)
{
	return CNtlPLOptionManager::GetInstance(); 
}