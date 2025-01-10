#include "precomp_ntlframework.h"
#include "NtlRender.h"
#include "NtlDebug.h"

/**
*
*  constructor for CNtlRender
*
*/
CNtlRender::CNtlRender() : m_bEnabled(false) 
{
}


bool CNtlRender::Create(void)
{
	NTL_FUNCTION("CNtlRender::Create");

	NTL_RETURN(true);
}

void CNtlRender::Destroy(void)
{
	NTL_FUNCTION("CNtlRender::Destroy");

	NTL_RETURNVOID();
}

/**
*
*Rendering object update function.
*  \param fElapsed is the delta time of the current tick from the previous tick.
*
*/
void CNtlRender::Update(float fElapsed)
{
	NTL_FUNCTION("CNtlRender::Update");

	// Get input.
	// Update the logic. (including rendering objects)

	g_MsgTick.Id = g_EventRenderTick;
	m_fFreeElapsed = fElapsed;

	g_MsgTick.pData = reinterpret_cast<void*>(&m_fFreeElapsed);
	SendMsg(g_MsgTick);
	
	NTL_RETURNVOID();
}

