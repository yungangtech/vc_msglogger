/**********************************************************************/
/// @file		autounlock.h
/// @brief		过程操作自动解锁类
/// @author		王永龙
/// @date		2010-5-27
/// @note		 
/**********************************************************************/
#ifndef __AUTO_UNLOCK_H__
#define __AUTO_UNLOCK_H__

/// \class CAutoUnlock autounlock.h autounlock.h
/// \brief 自动解锁类
/// \note	在需要使用锁机制的函数体中，在关键代码前，生成一个该类对象，并传入互斥体句柄，可以实现\n
///			操作过程上锁，并在操作完成，函数退出时，自动解锁。\n
///						\n
///			使用方法：	\n
///					CAutoUnlock mylock(m_hMutex/*互斥对象句柄*/);\n
///					mylock.Lock();
class CAutoUnlock
{
public :
	/// \fn CAutoUnlock(HANDLE & hMutex)
	/// \brief 构造函数
	/// @param hMutex 采用应用的方式将互斥对象传递进来
	/// @note 在使用时实例化该对象，在对象析构时自动解除锁定状态
	CAutoUnlock(HANDLE& hMutex);

	/// \fn Lock(DWORD dwWaitTime = INFINITE)
	/// \brief 锁函数
	/// @param dwWaitTime 毫秒，等待时间，默认参数为 INFINITE
	/// @return 没有返回值
	void Lock(DWORD dwWaitTime = INFINITE);

	/// \fn ~CAutoUnlock()
	/// \brief 析构函数
	/// @note 实例析构时，自动解锁
	virtual ~CAutoUnlock();

	/// \var HANDLE hLockMutex
	/// \brief 类属性，会保存传入的互斥对象，实际上是一个指向互斥对象的指针，采用引用的方法
	HANDLE	m_hLockMutex;	/// < 本地互斥对象句柄
};


#endif //__AUTO_UNLOCK_H__