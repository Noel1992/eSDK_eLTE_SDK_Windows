
// eLTE_Audio.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CeLTE_AudioApp:
// �йش����ʵ�֣������ eLTE_Audio.cpp
//

class CeLTE_AudioApp : public CWinApp
{
public:
	CeLTE_AudioApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_AudioApp theApp;