/**********************************************************************/
/// @file		msglog.h
/// @brief		��־��¼��
/// @author		������
/// @date		2010-5-31 
/**********************************************************************/

#ifndef __MSG_LOG_H__
#define __MSG_LOG_H__

#include <string>
using namespace std;


#define MAX_STRING_LENGTH 8192*2		///< ������¼��ַ���
#define MAX_LOG_STRING_COUNT 10000	///< �ļ���¼������־����������С����0x1fffff�ֽ�ʱ�����Զ�����

#define	LEVEL_ERROR     0			///< ��־��¼��𣺴�����Ϣ
#define	LEVEL_WARNING   1			///< ��־��¼��𣺾�����Ϣ
#define	LEVEL_INFO      2			///< ��־��¼���һ����Ϣ
#define	LEVEL_FUNC      3			///< ��־��¼��𣺺��������Ϣ
#define	LEVEL_BIN	    4			///< ��־��¼��𣺶�������Ϣ

#define DEF_TIME_LONG_STR "%04u-%02u-%02u %02u:%02u:%02u:%03u"	///< ��־��¼ʱ���ʽ
#define DEF_SHORT_FORMAT_STR "[0x%05x-0x%05x][%s]%s%s"			///< ��־��¼��Ϣ��ʽ��[process-thread][time]level:message

#define NEWLINE "\n"

/// \class CLog msglog.h "msglog.h"
/// \brief ��־��¼�֧࣬�ּ�¼�ļ��������DbgView.exe����
/// \note 1)ʹ��ʱ������Ҫֱ��ʹ�ø��࣬����˼����꣬�ú��������־����\n
/// \note 2)��������¼���ļ���ʽ����std::string������UNICODE�ַ�������Ĳ�����Ҫ����_T()��ת����\n
///			��ô�����Ϊ����ʹ��ʱ���ӷ����ݡ�
/// \note 3)��¼���������ڣ��纯����void MyLogTest()�����ں���ʵ�ֵĵ�һ��ʹ��LOG_FUNC(MyLogTest);\n
///			���ɼ�¼�ú�������ڣ����ùܺ����ж��ٸ����ڡ�
/// \note 4)��¼һ����Ϣ��ʹ��LOG_INFO(("%s,%d","hello world", 512));ע����˫����
/// \note 5)��¼������Ϣ��ʹ��LOG_WARN(("%s,%d","hello world", 512));ע����˫����
/// \note 6)��¼������Ϣ��ʹ��LOG_ERR(("%s,%d","hello world", 512));ע����˫����
/// \note 7)��¼��������Ϣ��ʹ��LOG_BIN("���Ƕ��������ݣ�",pBinData,BinDataLen);
/// \note 8)Debug�汾��Ȼ������ʾ��־�Ĺ��ܣ��������DbgView.exe�У������Ҫ������ļ���������\n
///			Debug�汾����Release�汾������Ҫ�ڹ����������Ԥ����꣺MSG_LOG_FILE
/// \note 9)�����Release�汾��Ҫ�����DbgView.exe������Ҫ��Ӻ꣺MSG_LOG_DBGVIEW
class AFX_EXT_CLASS CLog
{
public:
	CLog();	///< ���캯��
	~CLog();	///< ��������

	/// \fn LogBinary(std::string strInfo, unsigned char* pData, unsigned long ulLen)
	/// \brief ������¼�������ļ�
	/// \param strInfo һ��˵��
	/// \param pData ָ���޷����ַ������ָ��
	/// \param ulLen ���鳤�ȣ����ֽ�Ϊ��λ
	/// \return �޷���
	void LogBinary(std::string strInfo, unsigned char* pData, unsigned long ulLen);
	void LogBinaryToFile(std::string strInfo, unsigned char* pData, unsigned long ulLen);
	

	/// \fn LogString(unsigned long ulLogLevel, std::string strMsg)
	/// \brief ������¼��־
	/// \param ulLogLevel ��¼���
	/// \param strMsg ��Ҫ��¼������
	/// \return �޷���
	void LogString(unsigned long ulLogLevel, std::string strMsg, bool bLog2File = false);

	/// \fn LogFormatString( char * Format, ... )
	/// \brief ������ʽ���ַ���������sprintf����
	/// \param Format ��ʽ���ַ���
	/// \param ... ����ʽ���ַ�����ʵ������
	/// \return ����std::string �ַ���
	std::string LogFormatString( char * Format, ... );

	/// \fn GetPosition()
	/// \brief ���������Ϣ��¼�Ĳ�ι�ϵ�����ؿո�
	/// \return ����std::string�ַ�����Ҳ���ǵ�ǰ��¼��ǰ��Ŀո�����
	std::string GetPosition();

protected:
	/// \fn LimitLogFile()
	/// \brief �����Լ�¼���ļ��������С
	/// \return ��
	void LimitLogFile();

	/// \fn OutputLog(std::string strLog)
	/// \brief ����Ԥ�ȵĶ��������־��Ϣ
	/// \return ��
	/// \note ������ʵ�����̰߳�ȫ
	void OutputLog(std::string strLog, bool bLog2File=false);

	/// \fn GetCurTime()
	/// \brief ������ȡ��ǰʱ�䣬�����ظ�ʽ���ַ���
	/// \return ��ǰʱ��ĸ�ʽ���ַ���
    std::string GetCurTime();

	CString	m_strMutexName;		///< ��������������
	HANDLE m_hMutex;			///< ��������
	CStdioFile m_file;			///< ��־��¼���ļ�ʱ���ļ��Ķ���
	BOOL m_bOpenFileSuccess;	///< ��ʾ����־�ļ��Ƿ�ɹ�
};

/// \class CFuncIOLog  msglog.h "msglog.h"
/// \brief �����Զ���¼��������ڵ���
class AFX_EXT_CLASS CFuncIOLog
{
public:
	/// \fn CFuncIOLog(std::string strFunctionName)
	/// \brief ���캯��
	/// \param strFunctionName Ϊ������
    CFuncIOLog(std::string strFunctionName, bool bLog2File=false);

    ~CFuncIOLog(); ///< ��������
    
private:
    std::string	m_strFuncName;	///< �ڲ�����ĺ����������˳�ʱ�õ�
    CLog*	m_pLog;			///< ָ����־��¼���ָ�����
	bool m_bLog2File;
};

/// \fn get_msg_log(void)
/// \brief ������ȡ��־��¼��ľ�̬����ȫ�֣�
/// \return ������־��¼�ྲ̬���������
CLog & get_msg_log(void);

#if defined(MSG_LOG_FILE) || defined(_DEBUG) || defined(MSG_LOG_DBGVIEW)


#if defined(MSG_LOG_FILE)
#undef NEWLINE
#define NEWLINE "\r\n"
#define LOG2FILE true
#else
#define LOG2FILE false
#endif

/// \def LOG_FUNC(func)
/// \brief �����������־��¼��
/// \note ����Ҫ��¼�ĺ������������LOG_FUNC(MyFunctionName); ����Ҫʹ��_T()�꣬����Ҫ��˫����
#define LOG_FUNC(func)							\
CFuncIOLog funcinoutlogger(#func,LOG2FILE);

/// \def LOG_INFO(msg)
/// \brief һ����Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_INFO(("%s %s","hello world!","it's me!"));
#define LOG_INFO(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_INFO,str,LOG2FILE);		\
}

#ifdef _DEBUG

/// \def LOG_ERR(msg)
/// \brief ������Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_ERR(("%s %s","hello world!","it's me!"));
#define LOG_ERR(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	str += " file:";							\
	str += __FILE__;							\
	str += " line:";							\
	char buf[30] = {0,};						\
	str += _itoa(__LINE__,buf,10);				\
	msg_logger.LogString(LEVEL_ERROR,str,LOG2FILE);		\
}
#else

/// \def LOG_ERR(msg)
/// \brief ������Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_ERR(("%s %s","hello world!","it's me!"));
#define LOG_ERR(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_ERROR,str,LOG2FILE);		\
}
#endif

/// \def LOG_WARN(msg)
/// \brief ������Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_WARN(("%s %s","hello world!","it's me!"));
#define LOG_WARN(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_WARNING,str,LOG2FILE);	\
}	

/// \def LOG_BIN(msg,buf,len)
/// \brief ��������־��¼��
/// \param msg �Զ��������ݵ�һ��˵������Ҫ��_T()�꣬���ڷ�UNICODE�ַ���
/// \param buf ���������ݵ�ָ��
/// \param len ���������ݵĳ���
#define LOG_BIN(msg,buf,len)					\
{												\
	CLog& msg_logger = get_msg_log();			\
	msg_logger.LogBinary(msg,buf,len);			\
}

#define LOG_BIN_TO_FILE(msg,buf,len)			\
{												\
	CLog& msg_logger = get_msg_log();			\
	msg_logger.LogBinaryToFile(msg,buf,len);	\
}

#else/*���ú궨��û��ʵ������*/

/// \def LOG_FUNC(func)
/// \brief �����������־��¼��
/// \note ����Ҫ��¼�ĺ������������LOG_FUNC(MyFunctionName); ����Ҫʹ��_T()�꣬����Ҫ��˫����
#define LOG_FUNC(func)

/// \def LOG_INFO(msg)
/// \brief һ����Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_INFO(("%s %s","hello world!","it's me!"));
#define LOG_INFO(msg)

/// \def LOG_ERR(msg)
/// \brief ������Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_ERR(("%s %s","hello world!","it's me!"));
#define LOG_ERR(msg)

/// \def LOG_WARN(msg)
/// \brief ������Ϣ��־��¼��
/// \note ʹ��ʱ��������ʱ˫���ţ�����LOG_WARN(("%s %s","hello world!","it's me!"));
#define LOG_WARN(msg)

/// \def LOG_BIN(msg,buf,len)
/// \brief ��������־��¼��
/// \param msg �Զ��������ݵ�һ��˵������Ҫ��_T()�꣬���ڷ�UNICODE�ַ���
/// \param buf ���������ݵ�ָ��
/// \param len ���������ݵĳ���
#define LOG_BIN(msg,buf,len)

#endif


#endif //__MSG_LOG_H__