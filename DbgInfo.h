// DbgInfo.h : DbgInfo DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDbgInfoApp
// �йش���ʵ�ֵ���Ϣ������� DbgInfo.cpp
//

class CDbgInfoApp : public CWinApp
{
public:
	CDbgInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
