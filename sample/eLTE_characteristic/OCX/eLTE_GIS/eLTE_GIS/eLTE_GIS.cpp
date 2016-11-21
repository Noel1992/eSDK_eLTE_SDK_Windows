
// eLTE_GIS.cpp : Defining an application's class behavior.
//

#include "stdafx.h"
#include "eLTE_GIS.h"
#include "eLTE_GISDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeLTE_GISApp

BEGIN_MESSAGE_MAP(CeLTE_GISApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CeLTE_GISApp construct

CeLTE_GISApp::CeLTE_GISApp()
{
	// Support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


CeLTE_GISApp theApp;


// CeLTE_GISApp initialize

BOOL CeLTE_GISApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("Application wizard generated local application"));

	CeLTE_GISDlg dlg;
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

	// As the dialog box is closed, it will return FALSE to exit the application instead of
	//starting the application's message pump.
	return FALSE;
}

