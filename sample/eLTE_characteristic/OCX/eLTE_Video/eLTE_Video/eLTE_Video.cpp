
// eLTE_Video.cpp : Defining an application's class behavior.
//

#include "stdafx.h"
#include "eLTE_Video.h"
#include "eLTE_VideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeLTE_VideoApp

BEGIN_MESSAGE_MAP(CeLTE_VideoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CeLTE_VideoApp construct

CeLTE_VideoApp::CeLTE_VideoApp()
{
	// Support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// the only one CeLTE_VideoApp object

CeLTE_VideoApp theApp;


// CeLTE_VideoApp initialize

BOOL CeLTE_VideoApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set it to include all common control classes 
	// to be used in the application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	CShellManager *pShellManager = new CShellManager;


	SetRegistryKey(_T("Application wizard generated local application"));

	CeLTE_VideoDlg dlg;
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

