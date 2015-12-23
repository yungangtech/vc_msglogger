/**********************************************************************/
/// @file		msglog.cpp
/// @brief		日志记录类
/// @author		王永龙
/// @date		2010-5-31 
/**********************************************************************/
#include "StdAfx.h"
#include "msglog.h"
#include "autounlock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static long g_called_deep_level = 0;
static long* p_g_called_deep_level = 0;


CLog & get_msg_log(void)
{
	static CLog msg_logger;
	return msg_logger;
}

CLog::CLog()
{
	m_strMutexName = _T("B432d73k-E8sg-47CC-32j7-ADl88EB90d4F");
	m_strMutexName += _T("msglog_mutex_holder_procid_");
	DWORD dwProcId = GetCurrentProcessId();

	m_strMutexName.Format(_T("%s_%d"),_T("B432d73k-E8sg-47CC-32j7-ADl88EB90d4F_msglog_mutex_holder_procid_"),dwProcId);

	m_hMutex = CreateMutex(NULL, FALSE, (LPCTSTR)m_strMutexName);
	m_bOpenFileSuccess = FALSE;
	CString strSMName(_T(""));
	strSMName.Format(_T("%s_log_level_%d"), m_strMutexName, dwProcId);

	p_g_called_deep_level = &g_called_deep_level;


#if defined(MSG_LOG_FILE)
	CString strLogPath;
	TCHAR sPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,sPath,MAX_PATH);

	strLogPath = sPath;
	ing nLen = strLogPath.ReverseFind(_T('\\'));
	strLogPath = strLogPath.Left(nLen);
	strLogPath += _T("\\gowinchatroom.log");


	CFileException err;
	TCHAR szErrMsg[512] = {0,};

	if ( !m_file.Open((LPCTSTR)strLogPath,CFile::modeReadWrite|CFile::shareDenyNone,&err) )
	{
		if ( !m_file.Open((LPCTSTR)strLogPath,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate|CFile::shareDenyNone,&err) )
		{
			m_bOpenFileSuccess = FALSE;
			err.GetErrorMessage(szErrMsg, 500);
			m_file.Close();
			throw std::wstring( szErrMsg );
		}
		else
		{
			m_bOpenFileSuccess = TRUE;
		}
	}
	else
	{
		m_bOpenFileSuccess = TRUE;
	}

	if (m_bOpenFileSuccess)
	{
		m_file.SeekToEnd();
	}
#endif
}

CLog::~CLog()
{
	if (m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	if (m_bOpenFileSuccess)
	{
		m_file.Close();
	}
}

std::string CLog::GetPosition()
{
	std::string strTmp("");
	g_called_deep_level = (long)*p_g_called_deep_level;

	for (long i = 1; i <= g_called_deep_level; i++)
	{
		strTmp += "    ";
	}

	return strTmp;
}

std::string CLog::GetCurTime()
{
	char szTime[512];
	SYSTEMTIME	SystemTime;
	GetLocalTime( &SystemTime );

	memset(szTime,0,512);

	_snprintf( szTime, 
		512 - 1,
		DEF_TIME_LONG_STR, 
		SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute,
		SystemTime.wSecond, SystemTime.wMilliseconds );

	std::string strTime = szTime;
	return strTime;
}

void CLog::LogString(unsigned long ulLogLevel, std::string strMsg, bool bLog2File/* = false*/)
{
	std::string msgHeader("");
	std::string retString("");

	switch( ulLogLevel )
	{
    case LEVEL_ERROR	: msgHeader = "ERR :  ";	break;
    case LEVEL_WARNING	: msgHeader = "WORN:  ";	break;
    case LEVEL_INFO		: msgHeader = "INFO:  ";	break;
	case LEVEL_FUNC		: msgHeader = "FUNC:  ";	break;
	case LEVEL_BIN		: msgHeader = "BIN :  ";	break;
    default: msgHeader = "    :";	break;
	};

	retString = LogFormatString(
		DEF_SHORT_FORMAT_STR,
		GetCurrentProcessId(),
		GetCurrentThreadId(),
		GetCurTime().c_str(),
		msgHeader.c_str(),
		strMsg.c_str() );

	OutputLog(retString,bLog2File);
	return;
}

void CLog::OutputLog(std::string strLog, bool bLog2File)
{
	CAutoUnlock unlock(m_hMutex);
	unlock.Lock();

	if (bLog2File)
	{	
		if (m_bOpenFileSuccess)
		{
			m_file.SeekToEnd();
		}
		if(!m_bOpenFileSuccess)
		{
			return;
		}
		CString str = strLog.c_str();
		str += _T(NEWLINE);

		m_file.SeekToEnd();
		m_file.WriteString((LPCTSTR)str);

		LimitLogFile();
	}
	else
	{
		std::string strLogMsg = strLog + NEWLINE;
		OutputDebugStringA(strLogMsg.c_str());
	}
}


void CLog::LimitLogFile()
{
	if(!m_bOpenFileSuccess)
	{
		return;
	}
	CStringList lstFile;
	lstFile.RemoveAll();

	CString strtmp;
	if (m_file.GetLength() < 0x1fffff)
	{
		return;
	}
	m_file.SeekToBegin();
	while (1)
	{
		if (!m_file.ReadString(strtmp))
		{
			break;
		}
		else
		{
			lstFile.AddTail(strtmp);
		}
	}

	m_file.SetLength(0);

	if (lstFile.GetCount()>MAX_LOG_STRING_COUNT)
	{
		while (lstFile.GetCount()>MAX_LOG_STRING_COUNT)
		{
			lstFile.RemoveHead();
		}

		m_file.SeekToBegin();

		while (lstFile.GetCount()>0)
		{
			m_file.WriteString((LPCTSTR)lstFile.GetHead());
			m_file.WriteString(_T("\n"));
			lstFile.RemoveHead();
		}
	}
	lstFile.RemoveAll();
}

std::string CLog::LogFormatString( char * Format, ... )
{
	std::string str = "";
	va_list args;

	char m_tmpBuffer[MAX_STRING_LENGTH];

	va_start( args, Format );
	memset(m_tmpBuffer,0,MAX_STRING_LENGTH);
	_vsnprintf( m_tmpBuffer, MAX_STRING_LENGTH-1, Format, args );
	va_end( args );

	str += m_tmpBuffer;

	return str;
}

void CLog::LogBinary(std::string strInfo, unsigned char* pData, unsigned long ulLen)
{
	std::string strTmp = GetPosition();

	int iHeadLen = (int)strlen("[0x00000-0x00000][2012-12-22 22:22:22:222]ERR :  ");
	
	char szChar[4] = {0,};
	char szTmp[512] = {0,};
	char szBlank[256] = {0,};
	char szBin[MAX_STRING_LENGTH] = {0,};

	memset(szChar,0,4);
	memset(szTmp,0,512);
	memset(szBlank,0,256);
	memset(szBin,0,MAX_STRING_LENGTH);

	for (int i=0;i<iHeadLen;i++)
	{
		szBlank[i] = ' ';
	}
	sprintf(szTmp,"%s (%d bytes):",strInfo.c_str(),ulLen);
	strcpy(szBin,GetPosition().c_str());
	strcat(szBin,szTmp);
	strcat(szBin,NEWLINE);
	sprintf(szTmp,"%s%s",szBlank,GetPosition().c_str());
	strcat(szBin,szTmp);
	for (unsigned long ulcount = 0; ulcount < ulLen; ulcount++)
	{
		memset(szChar,0,4);
		sprintf(szChar,"%02X ",pData[ulcount]);
		strcat(szBin,szChar);
		if ((ulcount != 0) && ((ulcount+1)%0x10 == 0))
		{
			strcat(szBin,NEWLINE);
			strcat(szBin,szTmp);
		}

		if (strlen(szBin) > (MAX_STRING_LENGTH - 256))
		{
			break;
		}
	}

//	strcat(szBin,NEWLINE);
	std::string strMsg = szBin;

	LogString(LEVEL_BIN,strMsg);	
}

void CLog::LogBinaryToFile(std::string strInfo, unsigned char* pData, unsigned long ulLen)
{
	std::string strTmp = GetPosition();

	int iHeadLen = (int)strlen("[0x00000-0x00000][2012-12-22 22:22:22:222]ERR :  ");
	char szChar[4] = {0,};
	char szTmp[512] = {0,};
	char szBlank[256] = {0,};
	char szBin[MAX_STRING_LENGTH] = {0,};

	memset(szChar,0,4);
	memset(szTmp,0,512);
	memset(szBlank,0,256);
	memset(szBin,0,MAX_STRING_LENGTH);

	for (int i=0;i<iHeadLen;i++)
	{
		szBlank[i] = ' ';
	}
	sprintf(szTmp,"%s (%d bytes):",strInfo.c_str(),ulLen);
//	strcpy(szBin,GetPosition().c_str());
	strcpy(szBin,szTmp);
	strcat(szBin,NEWLINE);
	for (unsigned long ulcount = 0; ulcount < ulLen; ulcount++)
	{
		memset(szChar,0,4);
		sprintf(szChar,"%02X ",pData[ulcount]);
		strcat(szBin,szChar);
		if ((ulcount != 0) && ((ulcount+1)%0x20 == 0))
		{
			strcat(szBin,NEWLINE);
		}
		
		if (strlen(szBin) > (MAX_STRING_LENGTH - 256))
		{
			break;
		}
	}

	strcat(szBin,NEWLINE);
	std::string strMsg = szBin;
	CFileException err;
	TCHAR szErrMsg[512] = {0,};
	TCHAR szPath[MAX_PATH] = {0,};

	CAutoUnlock unlock(m_hMutex);
	unlock.Lock();

	if (!m_bOpenFileSuccess)
	{
		_tcscpy(szPath,_T("C:\\Users\\Administrator\\AppData\\LocalLow\\blxkey.txt"));
		
		if ( !m_file.Open(szPath,CFile::modeReadWrite|CFile::shareDenyNone  /*|CFile::shareDenyWrite|CFile::shareDenyRead*/,&err) )
		{
			if ( !m_file.Open(szPath,CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyNone  /*|CFile::modeNoTruncate|CFile::shareDenyWrite*/,&err) )
			{
				m_bOpenFileSuccess = FALSE;
			}
			else
			{
				m_bOpenFileSuccess = TRUE;
			}
		}
		else
		{
			m_bOpenFileSuccess = TRUE;
		}
	}

	if (m_bOpenFileSuccess)
	{
		std::string msgHeader("");
		std::string retString("");

		switch( LEVEL_BIN )
		{
		case LEVEL_ERROR	: msgHeader = "ERR :  ";	break;
		case LEVEL_WARNING	: msgHeader = "WORN:  ";	break;
		case LEVEL_INFO		: msgHeader = "INFO:  ";	break;
		case LEVEL_FUNC		: msgHeader = "FUNC:  ";	break;
		case LEVEL_BIN		: msgHeader = "BIN :  ";	break;
		default: msgHeader = "    :";	break;		
		};

		retString = LogFormatString( 
			DEF_SHORT_FORMAT_STR, 
			GetCurrentProcessId(), 
			GetCurrentThreadId(), 
			GetCurTime().c_str(), 
			msgHeader.c_str(), 
			strMsg.c_str() );

		if (m_bOpenFileSuccess)
		{
			m_file.SeekToEnd();
		}
		if(!m_bOpenFileSuccess)
		{
			return;
		}
		CString str = retString.c_str();
		str += _T(NEWLINE);

		m_file.SeekToEnd();
		m_file.WriteString((LPCTSTR)str);

		LimitLogFile();
	}
}

CFuncIOLog::CFuncIOLog(std::string strFunctionName, bool bLog2File)
{
	m_pLog = &get_msg_log();
	m_strFuncName = strFunctionName;
	m_strFuncName += "()";

	std::string strTmp = m_pLog->GetPosition();

	InterlockedIncrement(p_g_called_deep_level);
	strTmp += "->";
	strTmp += m_strFuncName;

	m_bLog2File = bLog2File;

	if( m_pLog != NULL )
	{	
		m_pLog->LogString(LEVEL_FUNC, strTmp, m_bLog2File );
	}
}

CFuncIOLog::~CFuncIOLog()
{
	InterlockedDecrement(p_g_called_deep_level);

	std::string strTmp = m_pLog->GetPosition();
	strTmp += "<-";
	strTmp += m_strFuncName;
	if( m_pLog != NULL )
	{
		m_pLog->LogString(LEVEL_FUNC, strTmp, m_bLog2File );
	}
}
