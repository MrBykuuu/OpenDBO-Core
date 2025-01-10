#include "precomp_ntlpresentation.h"
#include "NtlPLAccelController.h"

CNtlPLAccelController::CNtlPLAccelController(void)
{
    m_fStartVal     = 0.0f;
    m_fEndVal       = 0.0f;
    m_fCurrentVal   = 0.0f;
    m_fStartVelocity = 0.0f;
    m_fAccel        = 0.0f;
    m_fCurrVel      = 0.0f;
	m_fCurrProgress = 0.0f;
    m_bUpdate       = FALSE;
}

CNtlPLAccelController::~CNtlPLAccelController(void)
{

}

// fStartVal: Start value
// fEndVal: target value
// fStartVelocity: Velocity (increment value/sec)
// fAccel: Acceleration (increase speed/sec)
void CNtlPLAccelController::SetAccel( RwReal fStartVal, RwReal fEndVal, RwReal fStartVelocity, RwReal fAccel ) 
{
    m_fStartVal = m_fCurrentVal =  fStartVal;
    m_fEndVal = fEndVal;
    m_fStartVelocity = m_fCurrVel = fStartVelocity;
    m_fAccel = fAccel;                 ///< Acceleration is squared in advance to reduce calculations at each update.
    m_bUpdate = TRUE;
}

RwBool CNtlPLAccelController::Update( RwReal fElapsed ) 
{
    if(!m_bUpdate)
        return FALSE;

    // increase speed
    m_fCurrVel += m_fAccel * fElapsed;

    // increase value
    if(m_fEndVal >= m_fStartVal)
    {
        m_fCurrentVal += m_fCurrVel * fElapsed;
    }
    else
    {
        m_fCurrentVal -= m_fCurrVel * fElapsed;
    }

    if((m_fEndVal >= m_fStartVal && m_fCurrentVal >= m_fEndVal) ||
       (m_fEndVal < m_fStartVal && m_fCurrentVal <= m_fEndVal))
    {
        m_fCurrentVal = m_fEndVal;        
        m_bUpdate = FALSE;        
    }

	// Current Progress
	m_fCurrProgress = RwFabs(( m_fStartVal-m_fCurrentVal)/(m_fStartVal-m_fEndVal));

    return m_bUpdate;
}

void CNtlPLAccelController::Stop()
{
	m_bUpdate = false;
}

RwReal CNtlPLAccelController::GetCurrentProgress()
{
	return m_fCurrProgress;
}