
// eLTE_SDS.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include "stdafx.h" in order to generate the PCH file before the file contains the file"
#endif

#include "resource.h"		// Main symbol


// CeLTE_SDSApp:
// The realization of such£¬see eLTE_SDS.cpp
//

class CeLTE_SDSApp : public CWinApp
{
public:
	CeLTE_SDSApp();

// rewrite
public:
	virtual BOOL InitInstance();

// Realization

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_SDSApp theApp;