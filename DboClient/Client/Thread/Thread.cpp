#include "precomp_dboclient.h"
#include "Thread.h"
#include <process.h>


void CWorkTreadMgr::AddThread( HANDLE hHandle )
{
	m_defTreadList[hHandle] = hHandle;
}

void CWorkTreadMgr::RemoveTread( HANDLE hHandle )
{
	mapdef_ThreadList::iterator it = m_defTreadList.find( hHandle );
	if ( it != m_defTreadList.end() )
	{
		m_defTreadList.erase( it );
	}
}

int CWorkTreadMgr::GetThreadCnt( void )
{
	return (int) m_defTreadList.size();
}


CWorkTreadMgr g_clWorkThreadMgr;


CThread::CThread( void )
{
	m_hThread = 0;
	m_uiThreaID = 0;

	m_bExit = false;
}

CThread::~CThread( void )
{
	Delete();
}

bool CThread::Create( void )
{
	if ( m_hThread ) return false;

	// Create a Thread
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreaFuncCB, this, CREATE_SUSPENDED, &m_uiThreaID );
	if ( 0 == m_hThread ) return false;

	g_clWorkThreadMgr.AddThread( m_hThread );

	return true;
}

void CThread::Delete( void )
{
	if ( m_hThread )
	{
		// If this object is destroyed before the thread terminates,
		// Therefore, the Thread cannot perform a graceful termination.
		// Wait and then quit
		SetExit( true );
		WaitForSingleObject( m_hThread, INFINITE );

		// Close the Thread handle
		CloseHandle( m_hThread );

		g_clWorkThreadMgr.RemoveTread( m_hThread );

		m_hThread = 0;
		m_uiThreaID = 0;

		Sleep( 10 );
	}
}

void CThread::SuspendThread( void )
{
	if ( m_hThread ) ::SuspendThread( m_hThread );
}

void CThread::ResumeThread( void )
{
	if ( m_hThread ) ::ResumeThread( m_hThread );
}

int CThread::ThreadCallBackFunc( void* pParam )
{
	if ( IsExit() ) return RESULT_THREAD_FORCE_EXIT;
	return RESULT_THREAD_SUCCESS;
}

bool CThread::IsExit( void )
{
	bool bExit;

	m_clCSExit.Lock();
	bExit = m_bExit;
	m_clCSExit.Unlock();

	return bExit;
}

void CThread::SetExit( bool bExit )
{
	m_clCSExit.Lock();
	m_bExit = bExit;
	m_clCSExit.Unlock();
}

unsigned int CThread::ThreaFuncCB( void* pParam )
{
	if ( 0 == pParam ) return RESULT_THREAD_ERROR;
	return ((CThread*)pParam)->ThreadCallBackFunc( pParam );
}
