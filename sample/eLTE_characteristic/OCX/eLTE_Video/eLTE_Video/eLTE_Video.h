
// eLTE_Video.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CeLTE_VideoApp:
// �йش����ʵ�֣������ eLTE_Video.cpp
//

class CeLTE_VideoApp : public CWinApp
{
public:
	CeLTE_VideoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_VideoApp theApp;