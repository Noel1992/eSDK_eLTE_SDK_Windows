
// eLTE_Audio.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
#endif

#include "resource.h"		// Main symbol


// CeLTE_AudioApp:
// For the implementation of this class, see eLTE_Audio.cpp
//

class CeLTE_AudioApp : public CWinApp
{
public:
	CeLTE_AudioApp();

// Rewrite
public:
	virtual BOOL InitInstance();

// Realization

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_AudioApp theApp;