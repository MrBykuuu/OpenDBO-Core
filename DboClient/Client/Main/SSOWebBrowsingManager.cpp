#include "precomp_dboclient.h"		//// 
#include "SSOWebBrowsingManager.h"
/*
CSSOWebBrowserInfo::CSSOWebBrowserInfo()									
{	
	m_pSSOWebBrowser = NULL;
	m_pCallbackOnBeforeNavigate = NULL;
	m_pCallbackOnNavigateComplete = NULL;
	m_pCallbackOnQuit = NULL;	
}

void CSSOWebBrowserInfo::SetCallBackFuncs( SWEBCALLBACK_ONBN2* pCallbackOnBeforeNavigate, 
										  SWEBCALLBACK_ONNC2* pCallbackOnNavigateComplete, SWEBCALLBACK_ONQUIT* pCallbackOnQuit )
{
	m_pCallbackOnBeforeNavigate = pCallbackOnBeforeNavigate;
	m_pCallbackOnNavigateComplete = pCallbackOnNavigateComplete;
	m_pCallbackOnQuit = pCallbackOnQuit;
}

CSSOWebBrowsingManager::CSSOWebBrowsingManager(void)
{
	m_vecMembers.clear();
}

CSSOWebBrowsingManager::~CSSOWebBrowsingManager(void)
{
}

BOOL CSSOWebBrowsingManager::Create( HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie )
{
	//Initialize the SSO web browser module
	if(!InitSSOWebBrowser(hWndMain, strAuthCookie, strDataCookie, strCpCookie)) 
	{
		//Terminate the program in case of failure
		MessageBoxA(NULL, "error", "The SSO WebBrowser module could not be initialized.", MB_OK);
		//end program
		return FALSE;
	}

	m_hWnd = hWndMain;

	return TRUE;
}

void CSSOWebBrowsingManager::Destroy()
{
	//Before destroying the SSO web browser module, you must destroy the web browser first.
	for( int i = 0; i < m_vecMembers.size(); i++)
	{
		m_vecMembers[i].m_pSSOWebBrowser->Destroy();
		delete( m_vecMembers[i].m_pSSOWebBrowser );
	}

	//Destroy the SSO web browser module
	UnInitSSOWebBrowser();

}

void CSSOWebBrowsingManager::AddMember( CSSOWebBrowserInfo& member, LPCTSTR strURL )
{
	if( NULL == FindMember( member.m_iID ) )
	{
		CSSOWebBrowser* pSSOWebBrowser = member.m_pSSOWebBrowser;

		if( NULL == member.m_pSSOWebBrowser)
			member.m_pSSOWebBrowser = new CSSOWebBrowser;

		if( member.m_pSSOWebBrowser )
		{
			m_vecMembers.push_back( member );
			member.m_pSSOWebBrowser->Create( WS_VISIBLE|WS_CHILD, member.m_rectPosition, m_hWnd, member.m_iID );
			member.m_pSSOWebBrowser->SetOption( TRUE );

			if( member.m_pCallbackOnBeforeNavigate )
				member.m_pSSOWebBrowser->SetCallback_OnBeforeNavigate2( member.m_pCallbackOnBeforeNavigate );
			if( member.m_pCallbackOnNavigateComplete )
				member.m_pSSOWebBrowser->SetCallback_OnNavigateComplete2( member.m_pCallbackOnNavigateComplete );
			if( member.m_pCallbackOnQuit )
				member.m_pSSOWebBrowser->SetCallback_OnQuit( member.m_pCallbackOnQuit );

			if(strURL)
				member.m_pSSOWebBrowser->Navigate( strURL );

		}

		member.m_pSSOWebBrowser = pSSOWebBrowser;
	}
}

void CSSOWebBrowsingManager::DelMember( int iID )
{
	for( int i = 0; i < m_vecMembers.size(); i++)
	{
		if( iID == m_vecMembers[i].m_iID )
		{
			m_vecMembers[i].m_pSSOWebBrowser->Destroy();
			delete( m_vecMembers[i].m_pSSOWebBrowser );
			m_vecMembers[i] = m_vecMembers[m_vecMembers.size()-1];
			m_vecMembers.pop_back();

			return;
		}
	}
}

CSSOWebBrowserInfo* CSSOWebBrowsingManager::FindMember( int iID )
{
	for( int i = 0; i < m_vecMembers.size(); i++)
	{
		if( iID == m_vecMembers[i].m_iID )
			return &m_vecMembers[i];
	}

	return NULL;
}

BOOL CSSOWebBrowsingManager::Navigate( int iID, LPCTSTR strURL)
{
	CSSOWebBrowserInfo* pMember = FindMember( iID );

	if( pMember)
	{
		pMember->m_pSSOWebBrowser->Navigate( strURL );

		return TRUE;
	}

	return FALSE;
}
*/