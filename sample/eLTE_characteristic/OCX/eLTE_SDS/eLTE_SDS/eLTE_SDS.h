
// eLTE_SDS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CeLTE_SDSApp:
// �йش����ʵ�֣������ eLTE_SDS.cpp
//

class CeLTE_SDSApp : public CWinApp
{
public:
	CeLTE_SDSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_SDSApp theApp;