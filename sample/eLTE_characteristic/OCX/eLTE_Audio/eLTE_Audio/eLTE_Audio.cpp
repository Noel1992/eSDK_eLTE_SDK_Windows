
// eLTE_Audio.cpp : 定义应用程序的类行为。
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


CeLTE_AudioApp theApp;


// CeLTE_AudioApp initial 

BOOL CeLTE_AudioApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set it to include all common control classes to 
	//be used in the application.
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

