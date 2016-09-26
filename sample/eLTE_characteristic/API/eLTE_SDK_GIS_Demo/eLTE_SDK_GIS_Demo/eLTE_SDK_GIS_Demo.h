
// eLTE_SDK_GIS_Demo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CeLTE_SDK_GIS_DemoApp:
// See eLTE_SDK_GIS_Demo.cpp for the implementation of this class
//

class CeLTE_SDK_GIS_DemoApp : public CWinApp
{
public:
	CeLTE_SDK_GIS_DemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_SDK_GIS_DemoApp theApp;