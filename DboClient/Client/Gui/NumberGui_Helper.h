/******************************************************************************
*File			: NumberGui_Helper.h
*Author		    : Hong SungBock
*Copyright		: (аж)NTL
*Date			: 2008. 11. 21
*Abstract		: 
*****************************************************************************
*Desc			: Various useful functions are gathered in CNumberGui.
*****************************************************************************/

#pragma once

class CNumberGui;

// Core
#include "NtlCallbackWidget.h"


//////////////////////////////////////////////////////////////////////////
//	The size of the input CNumberGui changes sequentially.
//  1. Call the AddNumberGui() function as many times as CNumberGui to apply.
//  2. Call Play(true)
//  3. Call the Update() function directly
//////////////////////////////////////////////////////////////////////////

class CNumberGui_ZoomHelper
{
public:
	struct sZoomData
	{
		RwBool				bFinish;
		RwReal				fStartRate;
		RwReal				fEndRate;
		RwReal				fRemainTime;
		RwReal				fElapsed;
		CNumberGui*			pNumberGui;
	};

	typedef std::list<sZoomData*>				LIST_ZOOM;


	CNumberGui_ZoomHelper();
	virtual ~CNumberGui_ZoomHelper();

	VOID		Update(RwReal fElapsed);
	VOID		Play(RwBool bPlay);
	VOID		Restart();

	// The size changes during fRemainTime from fStartRate to fEndRate.
	// If adding CNumberGui, which will provide the scale effect, fails, returns false.
	RwBool		AddNumberGui(CNumberGui* pNumberGui, RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime);
	VOID		RemoveAll();

	RwBool		IsFinish();
	RwBool		IsPlay();


	template <class Callbackclass>	
	VOID LinkFinishCallback(Callbackclass *cbclass, RwInt32 (Callbackclass::*callback)())
	{	
		if(m_pCallback)
		{
			NTL_DELETE(m_pCallback);
		}

		m_pCallback = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
	}

protected:
	sZoomData*				GetCurData();

protected:
	RwBool					m_bFinish;
	RwBool					m_bPlay;
	CNtlCallbackParam0*		m_pCallback;
	LIST_ZOOM				m_listZoom;
	LIST_ZOOM::iterator		m_itCur;
};