
// eLTE_ResourceManage.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CeLTE_ResourceManageApp:
// See eLTE_ResourceManage.cpp for the implementation of this class
//

class CeLTE_ResourceManageApp : public CWinApp
{
public:
	CeLTE_ResourceManageApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_ResourceManageApp theApp;