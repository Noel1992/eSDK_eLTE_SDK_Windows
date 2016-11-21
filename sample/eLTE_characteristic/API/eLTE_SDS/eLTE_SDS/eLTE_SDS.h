
// eLTE_SDS.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
#endif

#include "resource.h"		// Main symbol


// CeLTE_SDSApp:
// For the implementation of this class, see eLTE_SDS.cpp
//

class CeLTE_SDSApp : public CWinApp
{
public:
	CeLTE_SDSApp();

// Rewrite
public:
	virtual BOOL InitInstance();

// Realization

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_SDSApp theApp;