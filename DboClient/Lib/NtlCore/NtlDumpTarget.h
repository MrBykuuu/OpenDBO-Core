/*****************************************************************************
 *
 * File			: NtlDumpTarget.h
 * Author       : HyungSuk, Jang
 * Copyright    : NTL Co., Ltd.
 * Date         : December 26, 2005	
 * Abstract     : entity dump target class
 *****************************************************************************
 * Desc         : This class is a target class for dumping the class contents of all objects.
 *                The contents of the dump may be a console window, file, or graphic user interface.	
 *****************************************************************************/


#ifndef __NTL_DUMPTARGET_H__
#define __NTL_DUMPTARGET_H__

#include <string>
#include <rwcore.h>

class CNtlDumpTarget
{
private:

    RwChar *m_pBuffer;		/** stream buffer */
	RwInt32 m_iCurrSize;	/** current buffer size */
	RwInt32 m_iDataSize;	/** current À¯È¿ÇÑ data size */

	RwBool m_bActive;		/** Is dump target active? */

private:

	RwBool CheckBuffer(RwInt32 iLen);

public:

	CNtlDumpTarget();
	virtual ~CNtlDumpTarget();
	
	CNtlDumpTarget& operator << (char chData);
	CNtlDumpTarget& operator << (unsigned char byData);
	CNtlDumpTarget& operator << (short shData);
	CNtlDumpTarget& operator << (unsigned short wData);
	CNtlDumpTarget& operator << (int iData);
	CNtlDumpTarget& operator << (unsigned int uiData);
	CNtlDumpTarget& operator << (long lData);
	CNtlDumpTarget& operator << (__int64 i64Data);
	CNtlDumpTarget& operator << (unsigned __int64 ui64Data);
	CNtlDumpTarget& operator << (float fData);
	CNtlDumpTarget& operator << (double dData);
	CNtlDumpTarget& operator << (char *pData);
	CNtlDumpTarget& operator << (const char *pData);
	CNtlDumpTarget& operator << (std::string	&str);
	CNtlDumpTarget& Format(const char* pData /*= NULL*/, ...);
	
	virtual void Dump(void);
	virtual void Active(RwBool bActive);
	virtual void Color(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) {}

	RwBool IsActive(void);
	const RwChar* GetStream(void) const;
};

inline RwBool CNtlDumpTarget::IsActive(void)
{
	return m_bActive;
}

inline const RwChar* CNtlDumpTarget::GetStream(void) const
{
	return m_pBuffer;
}

#endif