#pragma once

/**
* \ingroup Client
* \brief
*		CSSOWebBrowsingManager, a management class that configures and controls CSSOWebBrowser provided by Netmarble as a member.
*
*		You can run multiple web browsers.
*	
*		Web browsers cannot be moved.
*
* 
* \date 2009-08-22
* \author woosung_test
*/

#include "SSOWebLib.h"
/*
#ifdef _DEBUG
#pragma comment(lib, "SSOWebLib_MDd.lib")
#else
#pragma comment(lib, "SSOWebLib_MD.lib")
#endif

#include <vector>

using namespace std;

class CSSOWebBrowserInfo
{
public:

	CSSOWebBrowser*		m_pSSOWebBrowser;
	int					m_iID;
	RECT				m_rectPosition;

	CSSOWebBrowserInfo();
	void				SetCallBackFuncs( SWEBCALLBACK_ONBN2* pCallbackOnBeforeNavigate, 
								SWEBCALLBACK_ONNC2* pCallbackOnNavigateComplete, SWEBCALLBACK_ONQUIT* pCallbackOnQuit );

	SWEBCALLBACK_ONBN2* m_pCallbackOnBeforeNavigate;
	SWEBCALLBACK_ONNC2* m_pCallbackOnNavigateComplete;
	SWEBCALLBACK_ONQUIT* m_pCallbackOnQuit;
};

class CSSOWebBrowsingManager
{
public:
	CSSOWebBrowsingManager( void) ;
	~CSSOWebBrowsingManager( void );

	BOOL	Create( HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie );
	void	Destroy();

	void	AddMember( CSSOWebBrowserInfo& member, LPCTSTR strURL = NULL);
	void	DelMember( int iID );
	CSSOWebBrowserInfo* FindMember( int iID );

	BOOL	Navigate( int iID, LPCTSTR strURL);
private:
	HWND						m_hWnd;
	vector<CSSOWebBrowserInfo>	m_vecMembers;
};
*/