
// eLTE_GIS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CeLTE_GISApp:
// �йش����ʵ�֣������ eLTE_GIS.cpp
//

class CeLTE_GISApp : public CWinApp
{
public:
	CeLTE_GISApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_GISApp theApp;