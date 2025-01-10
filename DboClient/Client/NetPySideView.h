#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Dbo
#include "Windowby3.h"
#include "SideIconGui.h"
#include "DBOEvent.h"
    
/**
 * \ingroup Client
 * \brief Side View showing NetPy information
 *
 * \date 2009-04-20
 * \author agebreak
 */
class CNetPySideViewGui : public CSideViewBase, public RWS::CEventHandler
{
public:
    enum ENetPySideViewHeight
    {
        NETPY_SIDEVIEW_HEIGHT_NORMAL = 60,
        NETPY_SIDEVIEW_HEIGHT_PCBANG = 140,
        NETPY_SIDEVIEW_HEIGHT_NFY    = 60,
    };

public:
    CNetPySideViewGui(const RwChar* pName);
    virtual ~CNetPySideViewGui(void);

    RwBool		Create();
    VOID		Destroy();
    VOID		Update(RwReal fElapsed);

    virtual VOID	OnPressESC();
    virtual VOID	OnSideViewOpen(const void* pData);
    virtual VOID	OnSideViewClose();
    virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

protected:
    virtual VOID	HandleEvents( RWS::CMsg &msg );
    VOID			LocateComponent();
    VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
    VOID			OnResize( RwInt32 iOldW, RwInt32 iOldH );
    VOID			OnPaint();

    VOID            SetState(eNetPySideViewType type);
    VOID            OnShowInfo();           ///< Prints current NetPy information
    VOID            OnShowNotify();         ///< Prints NetPy acquisition information. 

protected:
    CWindowby3			m_BackPanel;		///< background
    eNetPySideViewType  m_type;             ///< Currently displayed type

    gui::CSlot			m_slotPaint;
    gui::CSlot			m_slotMove;
    gui::CSlot			m_slotResize;

    // Current NetPy information
    gui::CStaticBox*    m_pstbRemainTimeTitle;
    gui::CStaticBox*    m_pstbRemainTimeData;    
    gui::CStaticBox*    m_pstbGetTitle;
    gui::CStaticBox*    m_pstbGetData;    
    gui::CStaticBox*    m_pstbCurrentTitle;
    gui::CStaticBox*    m_pstbCurrentData;   

    // NetPy acquisition information
    gui::CStaticBox*    m_pstbGetPoint;

    SDboEventUpdateNetPy m_UpdateNetPyInfo;                 ///< NetPy update information received from the server
    DWORD                m_dwUpdateNetPyPoint;              ///< Points earned
    RwReal               m_fRemainTime;                     ///< Time remaining until next NetPy acquisition
    RwReal               m_fNotifyShowTime;                 ///< Notify display time    
    RwBool               m_bIsPCBang;                       ///< Determine whether it is a PC room.
};
