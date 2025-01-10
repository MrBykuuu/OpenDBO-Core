#include "precomp_ntlframework.h"
#include "NtlApplication.h"
#include "memoryhandler.h"
#include "NtlTimer.h"
//#include "NtlCoreUtil.h"

#include "NtlCoreApi.h"
#include "NtlDeviceRestoreHandler.h"
#include "NtlLoadingController.h"


CNtlApplication *CNtlApplication::m_pInstance = NULL;
RwUInt32 CNtlApplication::m_uiFrameRate = 1000;


bool minimized = false;

//Resource Arena's Size
#define rsRESOURCESDEFAULTARENASIZE (4 << 20)
//Maximum number of files that can be handled and opened simultaneously by the file system
#define MAX_NB_FILES_PER_FS (20)   

CNtlApplication::CNtlApplication() : m_hWnd(NULL), 
									 m_hAccel(NULL),
									 m_bMinimized(FALSE),
									 m_iWindowPosX(0),
									 m_iWindowPosY(0),
									 m_iScreenWidth(NTL_APP_SCREEN_WIDTH),
									 m_iScreenHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iWindowWidth(NTL_APP_SCREEN_WIDTH), 
									 m_iWindowHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iUserWidth(NTL_APP_SCREEN_WIDTH),
									 m_iUserHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iBitDepth(NTL_APP_BIT_DEPTH),
									 m_bFullScreen(FALSE),
									m_bShowFps(false),
									 m_bQuit(FALSE),
									 m_fFps(0.f),
									 m_fElapsedTime(0.f),
									 m_fTime(0.f),
									 m_bRenderEnable(TRUE)
{
	m_pInstance = this;
}

CNtlApplication::~CNtlApplication() 
{

}

/**
 *Function that processes messages received from Windows
 * \param hWnd Window Handle
 * \param message Window Message
 * \param wParam 
 * \param lParam 
 * \return message processing result
 */
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CNtlApplication::GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

/**
 *Variable functions that need to be initialized in the application
 */
void CNtlApplication::Init()
{
	
}

/**
 * 
 *
 * \return Window Class¸¦ 
 */
bool CNtlApplication::RegisterMainWindowClass(HANDLE processInstance)
{
	  WNDCLASS windowClass;
      
#ifdef UNDER_CE
      windowClass.style = CS_DBLCLKS;
#else
      windowClass.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
#endif
      windowClass.lpfnWndProc = (WNDPROC)MainWndProc;
      windowClass.cbClsExtra = 0;
      windowClass.cbWndExtra = 0;
      windowClass.hInstance = (HINSTANCE)processInstance;
      windowClass.hIcon = 0;
      windowClass.hCursor = LoadCursor(0, IDC_ARROW);
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = 0;
	  /*windowClass.*/
      windowClass.lpszClassName = kMainWindowClassName;
      
      return ( RegisterClass(&windowClass) != 0 );
}

HWND CNtlApplication::CreateMainWindow(HANDLE processInstance, int posX, int posY, int sizeX, int sizeY, RwBool bFullScreen)
{          
    // If we are in exclusive mode i.e. full screen in windows ensure the window flags
    // are correctly set.
    if (bFullScreen)
    {
        m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE;
		m_bFullScreen = TRUE;
    }
    else
    {
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW;
		m_bFullScreen = FALSE;
    }

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = sizeX;
	rect.bottom = sizeY;

	m_iWindowWidth	= m_iScreenWidth = m_iUserWidth = sizeX;
	m_iWindowHeight = m_iScreenHeight = m_iUserHeight = sizeY;
	
	// Calculates the size of the window to which the current style is applied.
	AdjustWindowRect(&rect, m_wndStyle, FALSE);
	
	// Create a window of appropriate size.
    HWND hwnd = CreateWindow(  kMainWindowClassName, kMainWindowName,
        m_wndStyle,
        posX, posY,
        rect.right - rect.left, rect.bottom - rect.top,
        (HWND)0, (HMENU)0, (HINSTANCE)processInstance, 0);

	// Calculate the actual size of a Window
	GetWindowRect( hwnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Calculate the actual size of Screen
	GetClientRect( hwnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

    SetWindowPos(hwnd, 0, 0, 0, 0, 0, m_wndFlags);

	return hwnd;
}

static rwD3D9DeviceReleaseCallBack g_OldDeviceReleaseCallBack = NULL;
static rwD3D9DeviceRestoreCallBack g_OldDeviceRestoreCallBack = NULL;

/**
 *NTL's DeviceRelease CallBack function
 */
static void NtlDeviceReleaseCallBack(void)
{
    if (g_OldDeviceReleaseCallBack)    
        g_OldDeviceReleaseCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->ReleaseHandler();
	return;
}


/**
 *Ntl's DeviceRestore CallBack function
 */
static void NtlDeviceRestoreCallBack(void)
{
    if (g_OldDeviceRestoreCallBack)
        g_OldDeviceRestoreCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->RestoreHandler();
	return;
}

/**
*Function to register Ntl¡¯s DeviceRestore CallBack function
 */
static void NtlSetDxDeviceEventHandler(void)
{
	g_OldDeviceReleaseCallBack = _rwD3D9DeviceGetReleaseCallback();
	_rwD3D9DeviceSetReleaseCallback(NtlDeviceReleaseCallBack);

	g_OldDeviceRestoreCallBack = _rwD3D9DeviceGetRestoreCallback();
	_rwD3D9DeviceSetRestoreCallback(NtlDeviceRestoreCallBack);
}

/**
 *CallBack function that outputs debug information of renderware
 */

// 
static void NtlRenderwareDebugMessageHandler(RwDebugType type __RWUNUSED__, const RwChar *str)
{
    OutputDebugString(str);
    OutputDebugString(RWSTRING("\n"));
	
	return;
}


// Disable Shortcut Keys by HoDong(2007.4.16)
// DirectX SDK Technical Articles (Disabling Shotcut Keys in Games)
STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};    

void AllowAccessibilityShortcutKeys( bool bAllowKeys )
{
    if( bAllowKeys )
    {
        // Restore StickyKeys/etc to original state and enable Windows key      
        STICKYKEYS sk = g_StartupStickyKeys;
        TOGGLEKEYS tk = g_StartupToggleKeys;
        FILTERKEYS fk = g_StartupFilterKeys;
        
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }
    else
    {
        // Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
        // then leave the settings alone as its probably being usefully used
 
        STICKYKEYS skOff = g_StartupStickyKeys;
        if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
            skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
        }
 
        TOGGLEKEYS tkOff = g_StartupToggleKeys;
        if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
            tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
        }
 
        FILTERKEYS fkOff = g_StartupFilterKeys;
        if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
            fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
        }
    }
}


/**
 *A function that creates an application, creating a window and initializing the renderware engine.
 *Listen.
 * \param hInstance Window Instance
 * \param posX Window X starting position
 * \param posY Window Y starting position
 * \param sizeX Window horizontal size
 * \param sizeY Window vertical size
 * \param bFullScreen Check whether it is full screen or window screen.
 * \return Whether the application was successful or not
 */
RwBool CNtlApplication::Create( HINSTANCE hInstance, 
							    RwInt32 posX, 
								RwInt32 posY, 
								RwInt32 sizeX, 
								RwInt32 sizeY, 
								RwBool bFullScreen)
{
	//Create Window
	RegisterMainWindowClass(hInstance);
	m_hWnd = CreateMainWindow(hInstance, posX, posY, sizeX, sizeY, bFullScreen);

	// Disable Shortcut Keys by HoDong(2007.4.16)
	// Save the current sticky/toggle/filter key settings so they can be restored them later 
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
	AllowAccessibilityShortcutKeys( false );
	
	//Memory Function and ArenaSize settings
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
	{
		DBO_FAIL("Engine initial fail !!!");
		return FALSE;
	}
	
	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	//Plugin registration
	AttachPlugin();

	//Open the Renderware engine.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		DBO_FAIL("Engine open fail !!!");
		RwEngineTerm();
		return FALSE;
	}

	//Check whether the current graphics support the mode.
	if( !SelectVideoMode() )
	{
		DBO_FAIL("Select video mode fail !!!");
		return FALSE;
	}
	
	//multisampling test by HoDong
	//RwUInt32 uiMaxMultiSampling = RwD3D9EngineGetMaxMultiSamplingLevels();
	//RwBool bSuccess = RwD3D9ChangeMultiSamplingLevels(1);
	
	//Start Renderware Engine.
	RwD3D9EngineSetMultiThreadSafe( TRUE );
	if( !RwEngineStart() )
	{
		DBO_FAIL("Select start fail !!!");
		return FALSE;
	}	
	
	//Initialize File System Manager and install.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//Image Loader(Bmp, Png) Enrollment
	RegisterImageLoader();
	

	ShowWindow(m_hWnd, 1);
	UpdateWindow(m_hWnd);

	// The size of the window created after ShowWindow may change. Update the changed size.
	RECT rect;
	GetClientRect( m_hWnd, &rect );
	Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	
	//Creating SubSystems (Camera, Charset ...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();
	g_SetElapsedTime(0.f);

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();

	return TRUE;
}


/**
 *A function that creates an application, creating a window and initializing the renderware engine.
 *Listen (for MFC)
 * \param HWND Window Handle
 * \param bFullScreen Check whether it is full screen or window screen.
 * \return Whether the application was successful or not
 */
RwBool CNtlApplication::Create(HWND hHandle, RwBool bFullScreen)
{
	m_hWnd			= hHandle;
	m_bFullScreen	= bFullScreen;

	RECT rtRect;
	GetClientRect(m_hWnd, &rtRect);
	m_iScreenWidth  = rtRect.right - rtRect.left;
	m_iScreenHeight = rtRect.bottom - rtRect.top;

	//Memory Function and ArenaSize settings
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
		return FALSE;

	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	AttachPlugin();

	//Open the Renderware engine.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		RwEngineTerm();
		return FALSE;
	}

	//Check whether the current graphics support the mode.
	if( !SelectVideoMode() )
	{
		return FALSE;
	}
	
	//Start Renderware Engine.
	if( !RwEngineStart() )
	{
		return FALSE;
	}	
	
	//Initialize File System Manager and install.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//PlugIn registration and Image Loader (Bmp, Png) registration
	
	RegisterImageLoader();
	
	//Creating SubSystems (Camera, Charset...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();
	
	return TRUE;
}

/**
 *Terminates the function of the application.
 */
void CNtlApplication::Destroy()
{
	// Disable Shortcut Keys by HoDong(2007.4.16)
	AllowAccessibilityShortcutKeys( true );
	
	RtFSManagerClose();
	
	//Stop RW engine
	RwEngineStop();
	
	//Close RW engine
	RwEngineClose();
	
	//Terminate RW engine
	RwEngineTerm();
}



/** 
 *m_nWindowWidth, m_nWindowHeight, and m_nBitDepth are the current card
 *Check whether it is supported.
 * Whether \return mode is supported
 */
RwBool CNtlApplication::SelectVideoMode()
{
	#define MAX_SUBSYSTEMS      (16)
	RwSubSystemInfo GsubSysInfo[MAX_SUBSYSTEMS];

	RwVideoMode     vm;
	RwInt32			nGnumSubSystems = 0;
	RwInt32			nGcurSel = 0;
	RwInt32			nGcurSelVM = 0;
	RwInt32			nDefDeviceNum = 0;

	//Returns the number of graphics devices (Graphic Devices) that can currently render in the system. 
	nGnumSubSystems = RwEngineGetNumSubSystems();
	
	/* Just to be sure ... */
    nGnumSubSystems = (nGnumSubSystems > MAX_SUBSYSTEMS) ? MAX_SUBSYSTEMS : nGnumSubSystems;
	
	//Get all the names of the Graphics Device.
    for (RwInt32 subSysNum = 0; subSysNum < nGnumSubSystems; ++ subSysNum)
    {
        RwEngineGetSubSystemInfo(&GsubSysInfo[subSysNum], subSysNum);
    }
	
	//Get the current Graphics Device.
	nGcurSel = RwEngineGetCurrentSubSystem();
	
	//Engine Setting to the current Graphics Device.
    if (!RwEngineSetSubSystem(nGcurSel))
    {
        return FALSE;
    }
	
	// Find the optimal resolution. Check whether the mode is supported.
	nGcurSelVM = GetVideoMode( m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth );
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode(m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth);
		if ( nGcurSelVM < 0 )
		{
			DBO_FAIL("Not failed best video mode !!!");
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwEngineSetVideoMode(nGcurSelVM))
    {
        return FALSE;
    }

	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());
	if( !(vm.flags & rwVIDEOMODEEXCLUSIVE) )
	{
		RECT rect;
		GetWindowRect(m_hWnd, &rect);

		m_iWindowPosX = rect.left;
		m_iWindowPosY = rect.top;
	}
	
	// Updates the size of the currently selected resolution
	m_iUserWidth = vm.width;
	m_iUserHeight = vm.height;
	m_iBitDepth = vm.depth;

	return TRUE;
}

void CNtlApplication::ChangeWindowStyle(RwBool bFullScreen)
{
	m_bFullScreen = bFullScreen;

	if(m_bFullScreen)
	{
		m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_TOPMOST;
	}
	else
	{
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_NOTOPMOST;		
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_iUserWidth;
	rect.bottom = m_iUserHeight;
	
	AdjustWindowRect(&rect, m_wndStyle, FALSE);

	SetWindowLong(m_hWnd, GWL_STYLE, m_wndStyle);
	SetWindowPos(m_hWnd, m_hWndInsertAfter, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_wndFlags);

	// After resizing, if the size does not match the current size using GetClientRect, resize accordingly.
	RECT rectAfter;
	GetClientRect( m_hWnd, &rectAfter );
	if( rect.right - rect.left != rectAfter.right - rectAfter.left ||
		rect.bottom - rect.top != rectAfter.bottom - rectAfter.top )
	{
		Resize( rectAfter.right - rectAfter.left, rectAfter.bottom - rectAfter.top, TRUE, FALSE );
	}

	// If not full screen, change to middle position
	if( !m_bFullScreen )
	{
		RECT rectWindow;
		GetWindowRect( m_hWnd, &rectWindow );

		RwInt32 nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
		RwInt32 nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
		RwInt32 nWindowWidth = rectWindow.right - rectWindow.left;
		RwInt32 nWindowHeight = rectWindow.bottom - rectWindow.top;

		m_iWindowPosX = (RwInt32)(nScreenWidth / 2 ) - (RwInt32)( nWindowWidth / 2 );
		m_iWindowPosY = (RwInt32)(nScreenHeight / 2 ) - (RwInt32)( nWindowHeight / 2 );

		SetWindowPos( m_hWnd, 0, m_iWindowPosX, m_iWindowPosY, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	}	
}

RwBool CNtlApplication::ToggleFullMode(void)
{
	RwBool bFullScreen = !m_bFullScreen;

	ChangeWindowStyle(bFullScreen);

	//Check whether the mode is supported.
	RwInt32 nGcurSelVM = GetVideoMode(bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth);	
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode( bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth );
		if( nGcurSelVM < 0 )
		{
			DBO_FAIL("failed to change video mode : " << m_iUserWidth << "*" << m_iUserHeight << "," << m_iBitDepth );
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwD3D9ChangeVideoMode (nGcurSelVM))
    {
        return FALSE;
    }

	RwVideoMode vm;
	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());

	// If it is full screen mode and vm.flags is not EXCLUSIVE, it is considered a window screen -> full screen mode conversion failure.
	// Re-align the windows. In the opposite case, the same logic applies.
	// Even if the screen change is successful, change the window style again to match the current resolution.
	if( (vm.flags & rwVIDEOMODEEXCLUSIVE) == 0)
	{
		ChangeWindowStyle( FALSE );
	}
	else
		ChangeWindowStyle( TRUE );
	

	API_CoreToggleFullScreenNotify( B2b(bFullScreen) );

	return TRUE;
}


void CNtlApplication::ChangeVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth)
{
	// ChangeVideo Mode will not work unless it is in FullScreen Mode.
	if(m_bFullScreen)
	{
		// Check if Mode is supported and apply if supported.
		RwInt32 nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
		if( nGcurSelVM < 0)
		{
			DBO_WARNING_MESSAGE("Monitor does not support resolution : " << iWidth << "*" << iHeight << "," << iBitDepth );
			
			iWidth = NTL_APP_SCREEN_WIDTH;
			iHeight = NTL_APP_SCREEN_HEIGHT;
			iBitDepth = NTL_APP_BIT_DEPTH;

			nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
			if (nGcurSelVM < 0)
			{
				DBO_FAIL("Go to DBOG/user and open SystemEnv.txt and change WINDOW_MODE to true");
				return;
			}
		}	

		// Update user-selected resolution
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		if (!RwD3D9ChangeVideoMode(nGcurSelVM))
			return;

		// In the case of ToggleFullMode(), a Resize message of the Window is generated, but in the case of ChangeVideoMode, the Resize is forcibly generated.
		RECT rect;
		GetClientRect( m_hWnd, &rect );
		Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	}
	// Window Mode
	else
	{
		// Update user-selected resolution
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		// Gets the size of the current desktop and forces the size to be modified if it is larger than the desktop.
		// The selected size is kept as is.
		int nSystemWidth = GetSystemMetrics(SM_CXSCREEN);
		int nSystemHeight = GetSystemMetrics(SM_CYSCREEN);

		if( nSystemWidth < iWidth )
			iWidth = nSystemWidth;
		
		if( nSystemHeight < iHeight )
			iHeight = nSystemHeight;

		int nPosX = ( nSystemWidth - iWidth ) ? ((nSystemWidth - iWidth )/2) : 0;
		int nPosY = ( nSystemHeight - iHeight ) ? ((nSystemHeight - iHeight )/2) : 0;

		SetWindowPos( m_hWnd, NULL, nPosX, nPosY, iWidth, iHeight, SWP_NOZORDER );
	}
}

/**
 *
 * \param bFullScreen
 * \param nWidth Window Width
 * \param nHeight Window Height
 * \return Is the current Video Mode supported?
 */
RwInt32 CNtlApplication::GetVideoMode(RwBool bFullScreen, RwInt32 nWidth, RwInt32 nHeight, RwInt32 bitDepth)
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();
	
	RwVideoMode vidModemInfo;
	for (RwInt32 vidMode = 0; vidMode < numVidModes; vidMode++)
	{
		RwEngineGetVideoModeInfo(&vidModemInfo, vidMode);		
		if (bFullScreen)
		{
			if (vidModemInfo.flags & rwVIDEOMODEEXCLUSIVE && 
				vidModemInfo.width  == nWidth && 
				vidModemInfo.height == nHeight && 
				vidModemInfo.depth  == bitDepth)
			{
				return vidMode;
			}
		}		
		else if (vidModemInfo.flags == 0)
		{
			return vidMode;
		}
	}

	return -1;
}


/**
 *Install the file system.
 * \return Whether installation was successful or not
 */
RwBool CNtlApplication::RsInstallFileSystem()
{
    RwChar      curDirBuffer[NTL_MAX_DIR_PATH];
    RwUInt32    retValue;
    RtFileSystem *wfs, *unc;

    RwUInt32 drivesMask;
    RwInt32 drive;
    RwChar  fsName[2];
    
    /* get the current directory */
    retValue = GetCurrentDirectory(NTL_MAX_DIR_PATH, curDirBuffer);
    if (0 == retValue || retValue > NTL_MAX_DIR_PATH)
    {
        return FALSE;
    }

    /* Register a unc file system */
    
    /* tag a \ to the end of the current directory */
    /* only fails if the buffer size is exhausted */
    rwstrcat( curDirBuffer, "\\" );

    /** Setup the file system manager */
    if ((unc = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != NULL)
    {
        /* Now actually register the file system */
        if (RtFSManagerRegister(unc) == FALSE)
        {
            return (FALSE);
        }
    }
    else
    {
        return (FALSE);
    }
    
    /* Now register local file systems */
    
    CharUpper(curDirBuffer);

    /* 
     *loop through all logical drives and register a file system for each
     *valid one.
     *Start at 2: this removes a:
     */
    drivesMask = GetLogicalDrives();

    for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
    {
        if (drivesMask & 0x01)
        {
            RwInt32 ret;
            RwChar  deviceName[4];

            deviceName[0] = drive + 'A' - 1;
            deviceName[1] = ':';
            deviceName[2] = '\\';   /* Needed for Win98 */
            deviceName[3] = '\0';

            ret = GetDriveType(deviceName);

            if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
            {
                /* Fix device name */
                deviceName[2] = '\0';

                fsName[0] = deviceName[0];
                fsName[1] = '\0';

                /** Setup the file system manager */
                if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != NULL)
                {
                    /* Now actually register the file system */
                    if (RtFSManagerRegister(wfs) == FALSE)
                    {
                        return (FALSE);
                    }
                    else
                    {
                        /* Set the unc file system as default if we have a unc path */
                        if (curDirBuffer[1] != ':')
                        {
                            RtFSManagerSetDefaultFileSystem(unc);
                        }
                        else if (deviceName[0] == curDirBuffer[0])
                        {
                            RtFSManagerSetDefaultFileSystem(wfs);
						//	RtFileSystemDefaultToAsync(wfs, TRUE);
                        }
                    }
                }
                else
                {
                    return (FALSE);
                }
            }
        }
    }
    
    return (TRUE);
}

/**
 *Function called when the size of the window changes
 * \param nWidth Window Horizontal Size
 * \param nHeight WindowHeight Size
 * \param zBuffer Whether to use ZBuffer or not
 * \return Whether or not Resize was successful
 */
RwBool CNtlApplication::Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	RECT rect;

	// Calculate the actual size of a Window
	GetWindowRect( m_hWnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Calculate the actual size of Screen
	GetClientRect( m_hWnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

	return TRUE;
}

/**
*When the location or size of the application changes
*Processed before WM_SIZE or WM_MOVE messages
*/
void CNtlApplication::SetApplicationMinMax(LPMINMAXINFO lpmmi)
{
}

/**
 *Functions for things that need to be created additionally
 * \param nWidth Horizontal size of the client of the currently created window
 * \param nHeight Vertical size of the client of the currently created window
 * \param zBuffer Whether or not to use ZBuffer
 * \return 
 */
RwBool CNtlApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	return TRUE;
}

LRESULT CNtlApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(PreTranslateMessage(window, message, wParam, lParam))
		return 0L;

	switch (message)
    {
    case WM_ACTIVATEAPP:
        minimized = !wParam;
        return( 0L ); 
    
	case WM_CLOSE:
        DestroyWindow(window);
        return( 0L);

    case WM_DESTROY:
        PostQuitMessage(0);
        return ( 0L );

    case WM_PAINT:
        PAINTSTRUCT Paint;
        BeginPaint(window, &Paint);
        //RWS::MainLoop::Render::Poll();
        EndPaint(window, &Paint);
        return ( 0L );
	
	case WM_SIZE:		
		RwRect r;
		RwBool bRestore_from_Minimize;

		r.x = 0;
		r.y = 0;
		r.w = LOWORD(lParam);
		r.h = HIWORD(lParam);

		bRestore_from_Minimize = FALSE;

		if( wParam == SIZE_MINIMIZED )
		{
			m_bMinimized = TRUE;
		}
		else if( wParam == SIZE_RESTORED )
		{
			if( m_bMinimized )
			{
				m_bMinimized = FALSE;
				bRestore_from_Minimize = TRUE;
			}
		}

		Resize(r.w, r.h, TRUE, bRestore_from_Minimize);
        return ( 0L );
	case WM_SIZING:
		return FixedWindowSizing( wParam, lParam );
	case WM_GETMINMAXINFO:
		SetApplicationMinMax( (LPMINMAXINFO)lParam );
		return( 0L );
	
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		return ( 0L );	
	default:
        return ( DefWindowProc(window, message, wParam, lParam) );
    }
	return (-1L);
}

RwBool CNtlApplication::Update()
{
	CNtlTimer::UpdateTimer();

	m_fTime			= CNtlTimer::GetAppTime();
	m_fElapsedTime	= CNtlTimer::GetElapsedTime();
	m_fFps			= CNtlTimer::GetFps();

	g_SetElapsedTime(m_fElapsedTime);

	if(Update(m_fTime, m_fElapsedTime))
		return FALSE;

	return TRUE;
}

RwBool CNtlApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	return TRUE;
}

int CNtlApplication::MessagePump()
{
	int result = 0;
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&frequency);

	while (!m_bQuit)
	{
		MSG message;

		if (PeekMessage(&message, 0, 0U, 0U, PM_REMOVE | PM_NOYIELD))
		{
			if (message.message == WM_QUIT)
			{
				SetQuit();
				result = (int)message.wParam;
			}
			else
			{
				if (m_hAccel == NULL || TranslateAccelerator(m_hWnd, m_hAccel, &message) == 0)
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}
		else
		{
			QueryPerformanceCounter(&start);
			RwUInt32 uiRateTime = (1000000 / m_uiFrameRate); // in microseconds

			Update();

			do {
				QueryPerformanceCounter(&end);
			} while ((end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart < uiRateTime);
		}
	}

	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}

	return (result);
}


RwBool CNtlApplication::AttachPlugin()
{
	return TRUE;
}


CallTextureLoad		g_fnCallTextureLoad		= NULL;
//CallTextureLoadPath g_fnCallTextureLoadPath	= NULL;

void LinkTextureLoad(CallTextureLoad fn/*, CallTextureLoadPath fnPath*/)
{
	g_fnCallTextureLoad		= fn;
	//g_fnCallTextureLoadPath	= fnPath;
}

void UnLinkTextureLoad(void)
{
	g_fnCallTextureLoad = NULL;
}


RwTextureCallBackRead OldCB;

RwTexture * DDSTextureRead(const RwChar * szName, const RwChar * szMaskName)
{
	CNtlLoadingController::GetInstance()->Lock();

	RwTexture*	pTexture			= NULL; 
	//RwBool		bTextureLoadPath	= FALSE;
	RwChar*		pcPath				= NULL;
	RwInt32		iPathSize			= 0;

// 	if(g_fnCallTextureLoadPath && (bTextureLoadPath = g_fnCallTextureLoadPath(RwImageGetPath(), &pcPath, &iPathSize)))
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	if(g_fnCallTextureLoad)
	{
		char *pPath = RwImageGetPath();
		pTexture = (*g_fnCallTextureLoad)(pPath, szName, szMaskName);
		if(pTexture)
		{
			CNtlLoadingController::GetInstance()->Unlock();
			
// 			if(bTextureLoadPath)
// 			{
// 				_rwImageSwapPath(&pcPath, &iPathSize);
// 			}

			return pTexture;
		}
	}
	
	RwChar *extender = rwstrrchr(szName, '.');
	if( extender != NULL )
	{
		if( _stricmp(extender, ".png") == 0 ||
		    _stricmp(extender, ".bmp") == 0 )
		{
			pTexture = OldCB(szName, szMaskName);
		}
	}
	
	//There are times when png comes in without extension.
	//(Eg skydummy) by HoDong (7/8/2006)
	if(!pTexture)
	{
		pTexture = RwD3D9DDSTextureRead(szName, szMaskName);
		if(pTexture == NULL)
			pTexture = OldCB(szName, szMaskName);
	}
	
	CNtlLoadingController::GetInstance()->Unlock();

// 	if(bTextureLoadPath)
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	return pTexture;
}

void HookDDSTextureRead(void)
{    
    OldCB = RwTextureGetReadCallBack(); 
    RwTextureSetReadCallBack(DDSTextureRead);
}

RwBool CNtlApplication::RegisterImageLoader()
{
	if (!RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, RtBMPImageWrite))
    {
        return FALSE;
    }

    if (!RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, RtPNGImageWrite))
    {
        return FALSE;
    }

	HookDDSTextureRead();

	return TRUE;
}

// Gamma
void CNtlApplication::SetGammaRamp(RwReal fGamma)
{
	// Inherit and implement.
}

/**
* \brief Returns the most appropriate video mode.
*/
RwInt32 CNtlApplication::GetBestVideoMode( RwBool bFullScreen, RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth )
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();

	RwInt32 nBestVideoMode = -1;

	RwInt32 nMinWidthMargin = 99999;
	RwInt32 nMinHeightMargin = 99999;
	RwInt32 nMinBitDepthMargin = 99999;
	RwInt32 nMinSummary = 99999;
	RwInt32 nCompare = 0;

	RwVideoMode sModeInfo;

	for (RwInt32 nModeIndex = 0; nModeIndex < numVidModes; nModeIndex++)
	{
		RwEngineGetVideoModeInfo(&sModeInfo, nModeIndex);

		if( sModeInfo.width <= 800 && sModeInfo.height <= 600 )
			continue;

		// In the case of full screen, the Video Mode with the lowest sum of width, height, and depth errors is returned.
		// In case of windowed mode, if Flags is checked as windowed mode, it returns unconditionally.
		if (bFullScreen)
		{
			if (sModeInfo.flags & rwVIDEOMODEEXCLUSIVE )
			{
				// area error
				nCompare = iWidth - sModeInfo.width;
				if( nCompare >= 0 )
					nMinWidthMargin = nCompare;

				// height error
				nCompare = iHeight - sModeInfo.height;
				if( nCompare >= 0 ) 
					nMinHeightMargin = nCompare;

				// depth error
				nCompare = iBitDepth - sModeInfo.depth;
				if( nCompare >= 0 )
					nMinBitDepthMargin = nCompare;

				// sum of errors
				nCompare = nMinWidthMargin + nMinHeightMargin + nMinBitDepthMargin;
				if( nCompare >= 0 &&
					nCompare < nMinSummary )
				{
					nMinSummary = nCompare;
					nBestVideoMode = nModeIndex;
				}

				// matches exactly
				if( nMinSummary == 0 )
					return nModeIndex;
			}
		}		
		else if (sModeInfo.flags == 0)
		{
			nBestVideoMode = nModeIndex;
		}
	}

	return nBestVideoMode;
}

/**
* \brief Function to resize a window at a fixed rate
*/
RwBool CNtlApplication::FixedWindowSizing( WPARAM wParam, LPARAM lParam )
{
	RECT* pRect = reinterpret_cast<RECT*>(lParam);
	int nCurrentWindowWidth = pRect->right - pRect->left;
	int nCurrentWindowHeight = pRect->bottom - pRect->top;

	float fWidthAdjust = (float)m_iWindowWidth / (float)m_iWindowHeight;
	float fHeightAdjust = (float)m_iWindowHeight / (float)m_iWindowWidth;

	switch( wParam )
	{
		// height increases.
	case WMSZ_BOTTOMLEFT:
	case WMSZ_BOTTOMRIGHT:
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->bottom = pRect->top + nFixHeight;
		}
		break;
	case WMSZ_TOP:
		{
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->left = pRect->right - nFixWidth;
		}
		break;
	case WMSZ_BOTTOM:
		{		
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->right = pRect->left + nFixWidth;
		}
		break;
	case WMSZ_TOPLEFT:
	case WMSZ_TOPRIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->top = pRect->bottom - nFixHeight;
		}
		break;
	default:
		break;
	}

	return TRUE;
}