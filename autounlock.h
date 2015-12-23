/**********************************************************************/
/// @file		autounlock.h
/// @brief		���̲����Զ�������
/// @author		������
/// @date		2010-5-27
/// @note		 
/**********************************************************************/
#ifndef __AUTO_UNLOCK_H__
#define __AUTO_UNLOCK_H__

/// \class CAutoUnlock autounlock.h autounlock.h
/// \brief �Զ�������
/// \note	����Ҫʹ�������Ƶĺ������У��ڹؼ�����ǰ������һ��������󣬲����뻥������������ʵ��\n
///			�����������������ڲ�����ɣ������˳�ʱ���Զ�������\n
///						\n
///			ʹ�÷�����	\n
///					CAutoUnlock mylock(m_hMutex/*���������*/);\n
///					mylock.Lock();
class CAutoUnlock
{
public :
	/// \fn CAutoUnlock(HANDLE & hMutex)
	/// \brief ���캯��
	/// @param hMutex ����Ӧ�õķ�ʽ��������󴫵ݽ���
	/// @note ��ʹ��ʱʵ�����ö����ڶ�������ʱ�Զ��������״̬
	CAutoUnlock(HANDLE& hMutex);

	/// \fn Lock(DWORD dwWaitTime = INFINITE)
	/// \brief ������
	/// @param dwWaitTime ���룬�ȴ�ʱ�䣬Ĭ�ϲ���Ϊ INFINITE
	/// @return û�з���ֵ
	void Lock(DWORD dwWaitTime = INFINITE);

	/// \fn ~CAutoUnlock()
	/// \brief ��������
	/// @note ʵ������ʱ���Զ�����
	virtual ~CAutoUnlock();

	/// \var HANDLE hLockMutex
	/// \brief �����ԣ��ᱣ�洫��Ļ������ʵ������һ��ָ�򻥳�����ָ�룬�������õķ���
	HANDLE	m_hLockMutex;	/// < ���ػ��������
};


#endif //__AUTO_UNLOCK_H__