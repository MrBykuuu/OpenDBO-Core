#ifndef __GUI_BALLOON_GENERIC_H__
#define __GUI_BALLOON_GENERIC_H__

START_GUI

class CDynamicOutline;

class CBalloon_Generic
{
public:
//! Constructor & Destructor:

	CBalloon_Generic( CBalloon* pSelf, BOOL bLeft );
	~CBalloon_Generic(VOID);

//! Operations:

	VOID SetOrigin( INT nScreenX, INT nScreenY, BOOL bLeft );
	VOID SetOriginNow( INT nScreenX, INT nScreenY, BOOL bLeft );
	
	VOID CalcRect(VOID);

	VOID SetBalloonColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	
//! Callbacks:

	VOID OnSetOption( const CComponentOptions& options );
	VOID OnMove( INT nOldX, INT nOldY );
	VOID OnPaint(VOID);
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	CSlot m_SlotSetOption;
	CSlot m_SlotMove;
	CSlot m_SlotPaint;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;

public:
//! Variables:

	CDynamicOutline*	m_pOutline;

	CPos				m_ptTailLT;		// Tail's Screen Left Top coordinates.

	FLOAT				m_fTailFactor;	// Count where the tail is placed. ( 0 ~ 1 )


	INT					m_nMarginX;		// Margin where text is placed.

	INT					m_nMarginY;
	
	CBalloon*			m_pBalloon;		// Reference Pointer.

};

END_GUI

#endif//__GUI_BALLOON_GENERIC_H__

