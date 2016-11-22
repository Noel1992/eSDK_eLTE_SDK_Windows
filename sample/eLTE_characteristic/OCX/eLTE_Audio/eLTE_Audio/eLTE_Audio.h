
// eLTE_Audio.h : PROJECT_NAME Main header file for the application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include "stdafx.h" in order to generate the PCH file before the file contains the file"
#endif

#include "resource.h"		// main symbol


// CeLTE_AudioApp:
// The realization of this class£¬see eLTE_Audio.cpp
//

class CeLTE_AudioApp : public CWinApp
{
public:
	CeLTE_AudioApp();

// rewrite
public:
	virtual BOOL InitInstance();

// Realization

	DECLARE_MESSAGE_MAP()
};

extern CeLTE_AudioApp theApp;