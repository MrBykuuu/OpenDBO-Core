#ifndef __GUI_BUTTON_GENERIC_H__
#define __GUI_BUTTON_GENERIC_H__

#include "gui_button.h"
#include "gui_texture.h"


START_GUI

class CButton_Generic
{
public:

	CButton_Generic(CButton *pSelf, bool bToggleMode);
	~CButton_Generic();

	CButton *m_pButton;

	VOID OnSetOptions(const CComponentOptions & options);
	VOID OnKeyDown(CComponent*, CInputDevice*, const CKey&);
	VOID OnKeyUp(CComponent*, CInputDevice*, const CKey&);
	VOID OnMouseLeave( CComponent* pComponent );
	VOID OnMouseDown(const CKey&);
	VOID OnMouseUp(const CKey&);
	VOID OnMouseMove( INT nKey, INT nX, INT nY );
	VOID OnPaint(VOID);
	VOID OnSetColor( BYTE byRed, BYTE byGreen, BYTE byBlue );
	VOID OnSetAlpha( BYTE ucAlpha );
		
	std::list<CSurface> m_stlSurfaceUp;
	std::list<CSurface> m_stlSurfaceDown;
	std::list<CSurface> m_stlSurfaceFocus;
	std::list<CSurface> m_stlSurfaceDisabled;
	std::list<CSurface>* m_pRenderSurface;
	CSurface m_surMask;

	bool m_bToggled;
	bool m_bToggleMode;
	BOOL m_bMouseEnter;
	BOOL m_bClickEnable;							// Enable only the button, not the enable of the component itself. All messages are allowed, but only clicks are not possible. 

	BOOL m_bEnableMouseCapture;						// Whether to perform MouseCapture when MouseDown.

		
	COLORREF m_colorTextUpColor;					// Each text color.

	COLORREF m_colorTextDownColor;
	COLORREF m_colorTextFocusColor;
	COLORREF m_colorTextDisableColor;
	BYTE	 m_ucTextUpAlpha;						// Each text alpha.

	BYTE	 m_ucTextDownAlpha;
	BYTE	 m_ucTextFocusAlpha;
	BYTE	 m_ucTextDisableAlpha;

	INT		m_nTextDownCoordDiffX;					// Text changes when button down.

	INT		m_nTextDownCoordDiffY;

	INT		m_nTextCoordX;							// Text location in button

	INT		m_nTextCoordY;							

	std::string	m_strFocusSound;					
	std::string m_strClickSound;				
	std::string m_strDisableSound;	

	CSignal_v2<CComponent*,bool> m_SigToggled;
	CSignal_v1<CComponent*> m_SigClicked;
	CSignal_v1<CComponent*> m_SigPressed;
	CSignal_v1<CComponent*> m_SigReleased;
	CSignal_v2<CComponent*,bool> m_SigFocused;
	
	CSlot m_SlotSetOptions;
	CSlot m_SlotKeyDown;
	CSlot m_SlotKeyUp;
	CSlot m_SlotMouseLeave;
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;
	CSlot m_SlotMouseMove;
	CSlot m_SlotPaint;
	CSlot m_slotSetAlpha;
	CSlot m_slotSetColor;
	
public:

	RwImage *m_pMaskImage;

	// Is the mask an invalid area in the picture?

	bool PtInMask(int nX, int nY);
	BOOL PtInValidRegion( INT nX, INT nY );
	VOID CreateMaskImage(const char *pFileName);
	VOID SetRenderSurface(VOID);
	VOID SetMouseEnter( BOOL bMouseEnter );
};  

END_GUI

#endif
