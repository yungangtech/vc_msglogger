/**********************************************************************/
/// @file		msglog.h
/// @brief		日志记录类
/// @author		王永龙
/// @date		2010-5-31 
/**********************************************************************/

#ifndef __MSG_LOG_H__
#define __MSG_LOG_H__

#include <string>
using namespace std;


#define MAX_STRING_LENGTH 8192*2		///< 单条记录最长字符数
#define MAX_LOG_STRING_COUNT 10000	///< 文件记录保留日志条数，当大小超过0x1fffff字节时，会自动截留

#define	LEVEL_ERROR     0			///< 日志记录类别：错误信息
#define	LEVEL_WARNING   1			///< 日志记录类别：警告信息
#define	LEVEL_INFO      2			///< 日志记录类别：一般信息
#define	LEVEL_FUNC      3			///< 日志记录类别：函数入口信息
#define	LEVEL_BIN	    4			///< 日志记录类别：二进制信息

#define DEF_TIME_LONG_STR "%04u-%02u-%02u %02u:%02u:%02u:%03u"	///< 日志记录时间格式
#define DEF_SHORT_FORMAT_STR "[0x%05x-0x%05x][%s]%s%s"			///< 日志记录消息格式：[process-thread][time]level:message

#define NEWLINE "\n"

/// \class CLog msglog.h "msglog.h"
/// \brief 日志记录类，支持记录文件和输出到DbgView.exe工具
/// \note 1)使用时，不需要直接使用该类，设计了几个宏，用宏来输出日志内容\n
/// \note 2)本类所记录的文件格式采用std::string，即非UNICODE字符，输入的参数不要采用_T()宏转换，\n
///			这么设计是为了在使用时更加方便快捷。
/// \note 3)记录函数进出口：如函数：void MyLogTest()，则在函数实现的第一行使用LOG_FUNC(MyLogTest);\n
///			即可记录该函数出入口，不用管函数有多少个出口。
/// \note 4)记录一般信息：使用LOG_INFO(("%s,%d","hello world", 512));注意是双括号
/// \note 5)记录警告信息：使用LOG_WARN(("%s,%d","hello world", 512));注意是双括号
/// \note 6)记录错误信息：使用LOG_ERR(("%s,%d","hello world", 512));注意是双括号
/// \note 7)记录二进制信息：使用LOG_BIN("这是二进制数据：",pBinData,BinDataLen);
/// \note 8)Debug版本自然具有显示日志的功能，是输出到DbgView.exe中，如果需要输出到文件，不管是\n
///			Debug版本还是Release版本，都需要在工程里面添加预定义宏：MSG_LOG_FILE
/// \note 9)如果是Release版本需要输出到DbgView.exe，则需要添加宏：MSG_LOG_DBGVIEW
class AFX_EXT_CLASS CLog
{
public:
	CLog();	///< 构造函数
	~CLog();	///< 析构函数

	/// \fn LogBinary(std::string strInfo, unsigned char* pData, unsigned long ulLen)
	/// \brief 用来记录二进制文件
	/// \param strInfo 一般说明
	/// \param pData 指向无符号字符数组的指针
	/// \param ulLen 数组长度，以字节为单位
	/// \return 无返回
	void LogBinary(std::string strInfo, unsigned char* pData, unsigned long ulLen);
	void LogBinaryToFile(std::string strInfo, unsigned char* pData, unsigned long ulLen);
	

	/// \fn LogString(unsigned long ulLogLevel, std::string strMsg)
	/// \brief 用来记录日志
	/// \param ulLogLevel 记录类别
	/// \param strMsg 需要记录的数据
	/// \return 无返回
	void LogString(unsigned long ulLogLevel, std::string strMsg, bool bLog2File = false);

	/// \fn LogFormatString( char * Format, ... )
	/// \brief 用来格式化字符串，类似sprintf函数
	/// \param Format 格式化字符串
	/// \param ... 填充格式话字符串的实际数据
	/// \return 返回std::string 字符串
	std::string LogFormatString( char * Format, ... );

	/// \fn GetPosition()
	/// \brief 用来获得信息记录的层次关系（返回空格）
	/// \return 返回std::string字符串（也就是当前记录的前面的空格数）
	std::string GetPosition();

protected:
	/// \fn LimitLogFile()
	/// \brief 用来对记录的文件限制其大小
	/// \return 无
	void LimitLogFile();

	/// \fn OutputLog(std::string strLog)
	/// \brief 根据预先的定义输出日志信息
	/// \return 无
	/// \note 本函数实现了线程安全
	void OutputLog(std::string strLog, bool bLog2File=false);

	/// \fn GetCurTime()
	/// \brief 用来获取当前时间，并返回格式化字符串
	/// \return 当前时间的格式化字符串
    std::string GetCurTime();

	CString	m_strMutexName;		///< 互斥体对象的命名
	HANDLE m_hMutex;			///< 互斥体句柄
	CStdioFile m_file;			///< 日志记录到文件时，文件的对象
	BOOL m_bOpenFileSuccess;	///< 表示打开日志文件是否成功
};

/// \class CFuncIOLog  msglog.h "msglog.h"
/// \brief 用来自动记录函数出入口的类
class AFX_EXT_CLASS CFuncIOLog
{
public:
	/// \fn CFuncIOLog(std::string strFunctionName)
	/// \brief 构造函数
	/// \param strFunctionName 为函数名
    CFuncIOLog(std::string strFunctionName, bool bLog2File=false);

    ~CFuncIOLog(); ///< 析构函数
    
private:
    std::string	m_strFuncName;	///< 内部保存的函数名，在退出时用到
    CLog*	m_pLog;			///< 指向日志记录类的指针对象
	bool m_bLog2File;
};

/// \fn get_msg_log(void)
/// \brief 用来获取日志记录类的静态对象（全局）
/// \return 返回日志记录类静态对象的引用
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
/// \brief 函数出入口日志记录宏
/// \note 在需要记录的函数体里面加上LOG_FUNC(MyFunctionName); 不需要使用_T()宏，不需要加双引号
#define LOG_FUNC(func)							\
CFuncIOLog funcinoutlogger(#func,LOG2FILE);

/// \def LOG_INFO(msg)
/// \brief 一般消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_INFO(("%s %s","hello world!","it's me!"));
#define LOG_INFO(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_INFO,str,LOG2FILE);		\
}

#ifdef _DEBUG

/// \def LOG_ERR(msg)
/// \brief 错误消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_ERR(("%s %s","hello world!","it's me!"));
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
/// \brief 错误消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_ERR(("%s %s","hello world!","it's me!"));
#define LOG_ERR(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_ERROR,str,LOG2FILE);		\
}
#endif

/// \def LOG_WARN(msg)
/// \brief 警告消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_WARN(("%s %s","hello world!","it's me!"));
#define LOG_WARN(msg)							\
{												\
	CLog& msg_logger = get_msg_log();			\
	std::string str = msg_logger.GetPosition();	\
	str += msg_logger.LogFormatString msg;		\
	msg_logger.LogString(LEVEL_WARNING,str,LOG2FILE);	\
}	

/// \def LOG_BIN(msg,buf,len)
/// \brief 二进制日志记录宏
/// \param msg 对二进制数据的一个说明，不要用_T()宏，属于非UNICODE字符串
/// \param buf 二进制数据的指针
/// \param len 二进制数据的长度
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

#else/*设置宏定义没有实际意义*/

/// \def LOG_FUNC(func)
/// \brief 函数出入口日志记录宏
/// \note 在需要记录的函数体里面加上LOG_FUNC(MyFunctionName); 不需要使用_T()宏，不需要加双引号
#define LOG_FUNC(func)

/// \def LOG_INFO(msg)
/// \brief 一般消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_INFO(("%s %s","hello world!","it's me!"));
#define LOG_INFO(msg)

/// \def LOG_ERR(msg)
/// \brief 错误消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_ERR(("%s %s","hello world!","it's me!"));
#define LOG_ERR(msg)

/// \def LOG_WARN(msg)
/// \brief 警告消息日志记录宏
/// \note 使用时，参数外时双括号，形如LOG_WARN(("%s %s","hello world!","it's me!"));
#define LOG_WARN(msg)

/// \def LOG_BIN(msg,buf,len)
/// \brief 二进制日志记录宏
/// \param msg 对二进制数据的一个说明，不要用_T()宏，属于非UNICODE字符串
/// \param buf 二进制数据的指针
/// \param len 二进制数据的长度
#define LOG_BIN(msg,buf,len)

#endif


#endif //__MSG_LOG_H__