
// eLTE_GIS.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include "stdafx.h" in order to generate the PCH file before the file contains the file"
#endif

#include "resource.h"		// Main symbol


// CeLTE_GISApp:
// The realization of such£¬see eLTE_GIS.cpp
//

class CeLTE_GISApp : public CWinApp
{
public:
	CeLTE_GISApp();

// rewrite
public:
	virtual BOOL InitInstance();

// implement

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_GISApp theApp;