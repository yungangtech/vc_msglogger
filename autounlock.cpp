/**********************************************************************/
/// @file		autounlock.cpp
/// @brief		过程操作自动解锁类
/// @author		王永龙
/// @date		2010-5-31
/// @note		 
/**********************************************************************/

#include "Stdafx.h"
#include <windows.h>
#include "autounlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAutoUnlock::CAutoUnlock(HANDLE& hMutex)
{
	m_hLockMutex = hMutex;
};

CAutoUnlock::~CAutoUnlock()
{
	if (m_hLockMutex != NULL)
	{
		BOOL bRet = ReleaseMutex(m_hLockMutex);
		m_hLockMutex = NULL;
	}
};

void CAutoUnlock::Lock(DWORD dwWaitTime /* = INFINITE */)
{
	DWORD m_dwWaitSecond = dwWaitTime;
	DWORD dwRet = WaitForSingleObject(m_hLockMutex,m_dwWaitSecond);
	if (dwRet != WAIT_OBJECT_0)
	{
		return;
	}
}

