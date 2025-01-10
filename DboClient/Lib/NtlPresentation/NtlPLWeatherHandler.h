#pragma once

#include "CEventHandler.h"
#include "NtlPLWeather.h"
#include "NtlSoundManager.h"

#define dNTL_PL_WEATHER_PROP_NUM	2
#define dNTL_PL_WEATHER_FADE_TIME	7.0f

class CNtlPLWeatherHandler : public RWS::CEventHandler
{
public:
	typedef std::vector<CNtlPLWeather*>	VEC_WEATHER;
	typedef VEC_WEATHER::iterator		VEC_WEATHER_IT;

	//typedef std::vector<SOUND_HANDLE>	VEC_SOUND;
	//typedef VEC_WEATHER::iterator		VEC_SOUND_IT;

public:
	CNtlPLWeatherHandler();
	virtual ~CNtlPLWeatherHandler();

public:
	virtual void	HandleEvents(RWS::CMsg &pMsg);
	virtual void	Update(RwReal fElapsedTime);
	virtual void	SetVisible(RwBool bVisible/*, RwReal fFade = 0.0f*/);
	virtual RwBool	GetVisible() { return m_bVisible; }

	RwInt32			GetNumList();
	RwInt32			GetNumRender();

// 	void			SetWeatherLevel(RwUInt32 uiWeatherLevel);
// 	RwUInt32		GetWeatherLevel();


	void			InitWeather();
	void			RefreshWeather();

	void			LoadWeatherReal(sNTL_FIELD_PROP* pNtlFieldProp, RwReal fFadeTime);	// Add Weather from Field.
	void			LoadWeatherDumm(RwReal fFadeTime);									// Add Dumm from WeatherReal.
	void			UnLoadWeatherReal();												// Unload the Real List.
	void			UnLoadWeatherDumm();												// Unload the Dumm List.



protected:
	VEC_WEATHER m_vecWeatherDumm;
	VEC_WEATHER m_vecWeatherReal;

	RwBool		m_bVisible;

	//RwUInt32	m_uiWeatherLevel;
};