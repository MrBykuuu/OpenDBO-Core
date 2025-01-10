////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CBalloon
// Desc: Balloon help.
//
// 2006.02.20 Peessi@hitel.net   
//
// To Be Desired:
//	1. If the number of characters is exceeded, omit it as...
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_BALLOON_H__
#define __GUI_BALLOON_H__

START_GUI

class CBalloon_Generic;

class CBalloon : public CComponent
{
//! Construction & Destruction:

public:
	// Balloon Constructor 

	CBalloon( CComponent* pParent, CSurfaceManager* pSurfaceManager, BOOL bLeft = TRUE, DWORD dwStyle = COMP_TEXT_VERTICAL_CENTER );

	CBalloon( CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, BOOL bLeft = TRUE, DWORD dwStyle = COMP_TEXT_VERTICAL_CENTER );	
	
	// Balloon Destructor

	~CBalloon(VOID);

//! Attributes:

public:
	
//! Operations:

public:
	// Starting point. Balloon Tail's Left or Right Bottom

	VOID SetOrigin( INT nScreenX, INT nScreenY, BOOL bLeft = FALSE );    // It does not move to the next text input.

	VOID SetOriginNow( INT nScreenX, INT nScreenY, BOOL bLeft = FALSE ); // It is transferred immediately.

	VOID SetMargin( INT nMarginX, INT nMarginY );

	// Balloon's texture vertex color settings.

	VOID SetBalloonColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	// Interface related to Static Box

	VOID CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes );
	VOID SetText( const CHAR* text );
	VOID SetText( const WCHAR* text );
	VOID SetText( INT number );
	VOID AddText( const CHAR* text ); 	
	VOID AddText( const WCHAR* text);
	VOID AddText( INT number );
	VOID Format( CHAR* format, ... );
	VOID AddFormat( CHAR* format, ... );
	VOID SetTextColor( COLORREF color );
	VOID SetEffectMode( INT nMode );
	VOID SetEffectColor( COLORREF color );
	VOID SEtEffectValue( INT nValue );
	VOID SetBkMode( int nBkMode );
	VOID SetBkColor( COLORREF color );
	VOID SetTextStyle( DWORD dwStyle );
	VOID SetLineSpace( INT nLineSpace );
	VOID SetMaxSize( INT nCX, INT nCY );
	VOID SetMinSize( INT nCX, INT nCY );

	VOID Clear();	
	
	// Outline-related Interface

	VOID AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
					  CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL, CSurface& surTail );

	VOID SetLeftCX( INT nCX ); 
	VOID SetRightCX( INT nCX ); 
	VOID SetTopCY( INT nCY ); 
	VOID SetBottomCY( INT nCY ); 
		
private:
//! Callbacks:

	VOID OnSetOption( const CComponentOptions& options );
	
	CSlot m_SlotSetOptions;
	
protected:
//! Variables:

	CStaticBox*			m_pStaticBox;
	CBalloon_Generic*	m_pImpl;

private:
//! Etc:

	CBalloon( const CBalloon& copy ) : CComponent( NULL, NULL ) { return; } // dissallow copy construction.

	friend class CBalloon_Generic;
};

END_GUI

#endif//__GUI_BALLOON_H__