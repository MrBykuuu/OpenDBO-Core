// SSOWebLib.h: interface for the CSSOWebLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
#define AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
typedef void* HWEBCTRL;

typedef void (SWEBCALLBACK_ONBN2)(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
typedef void (SWEBCALLBACK_ONNC2)(LPCTSTR strURL);
typedef void (SWEBCALLBACK_ONQUIT)();

// Function called to use the SSO web browser as a shared DLL (must be called before InitSSOWebBrowser())
void PresetSSOWebBrowserToSharedDll();

// Function called to change the cookie name according to the service site (must be called before InitSSOWebBrowser())
// If it is a Daum game, pass ¡°daum¡± (if it is a Netmarble game, there is no need to call it)
void PresetSSOWebBrowserSetSiteName(LPCTSTR strSiteName);

void PresetSSOWebBrowserBaseFolder(LPCTSTR strFolder);

// SSO web browser initialization function
BOOL InitSSOWebBrowser(HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie);

// Initialization function for testing
BOOL InitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie);

// SSO web browser destruction function (must be called before the main window is destroyed)
BOOL UnInitSSOWebBrowser();

// Function to get cookie enabled page URL
LPCTSTR GetActCookieURL();

// Function to get cookie refresh page URL
LPCTSTR GetRefreshCookieURL();

// SSO Function to obtain a cookie from the web browser (since it is converted internally, you can just put_cookie as it is obtained)
BOOL GetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp);

//////////////////////////////////////////////////////////////////////
// SSO web browser class

class CSSOWebBrowser  
{
public:

	HWEBCTRL		m_hWeb;

	CSSOWebBrowser();
	virtual ~CSSOWebBrowser();

	// Initializing and creating a web browser
	BOOL Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID);
	void Destroy();

	void SetOption(BOOL bHideBorder);
	BOOL PutCharSet(LPCTSTR strCharset);
	BOOL InsertHTML(LPCTSTR strHtml);
	BOOL ExecJavascript(LPCTSTR strScript);

	BOOL Navigate(LPCTSTR strURL);
	BOOL GetNetmarbleCookie(LPSTR pstrAuthCookie, LPSTR pstrDataCookie, LPSTR pstrCpCookie);

	void* GetWnd();
	int  GetUserNavigatedCount();
	void SetUserNavigatedCount(int count);
	void ResetUserNavigatedCount();

	static HMODULE	m_hDllModule;
	static BOOL		m_bInitialized;

	//virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//virtual void OnNavigateComplete2( LPCTSTR strURL );
	//virtual void OnQuit();

	//callback function
	//void (*m_pCallback_OnBeforeNavigate2)(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, void* pbaPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//void (*m_pCallback_OnNavigateComplete2)( LPCTSTR strURL );
	//void (*m_pCallback_OnQuit)();

	void SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback);
	void SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback);
	void SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback);
};

#endif // !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
