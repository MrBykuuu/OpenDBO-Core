//***********************************************************************************
//
//	File		:	NtlLine.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Line Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlLine.h"


CNtlLine::CNtlLine()
{
	// Do not initialize
}


CNtlLine::CNtlLine(const CNtlVector& rBegin, const CNtlVector& rEnd)
{
	m_rBegin = rBegin;
	m_rEnd = rEnd;
}
