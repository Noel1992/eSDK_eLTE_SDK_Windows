
// eLTE_Audio.cpp : Defining an application's class behavior.
//

#include "stdafx.h"
#include "eLTE_Audio.h"
#include "eLTE_AudioDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeLTE_AudioApp

BEGIN_MESSAGE_MAP(CeLTE_AudioApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CeLTE_AudioApp construct

CeLTE_AudioApp::CeLTE_AudioApp()
{
	// Support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// Only one CeLTE_AudioApp object

CeLTE_AudioApp theApp;


// CeLTE_AudioApp init

BOOL CeLTE_AudioApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;


	SetRegistryKey(_T("Application wizard generated local application"));

	CeLTE_AudioDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

