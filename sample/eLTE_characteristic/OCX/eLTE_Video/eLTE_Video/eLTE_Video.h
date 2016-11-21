
// eLTE_Video.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include "stdafx.h" in order to generate the PCH file before the file contains the file"
#endif

#include "resource.h"		// Main symbol


// CeLTE_VideoApp:
// The realization of such£¬see eLTE_Video.cpp
//

class CeLTE_VideoApp : public CWinApp
{
public:
	CeLTE_VideoApp();

// rewrite
public:
	virtual BOOL InitInstance();

// Realization

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_VideoApp theApp;