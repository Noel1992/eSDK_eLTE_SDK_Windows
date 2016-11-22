
// eLTE_SDS.cpp : Defining an application's class behavior.
//

#include "stdafx.h"
#include "eLTE_SDS.h"
#include "eLTE_SDSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeLTE_SDSApp

BEGIN_MESSAGE_MAP(CeLTE_SDSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CeLTE_SDSApp construct

CeLTE_SDSApp::CeLTE_SDSApp()
{
	// Support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// The only one CeLTE_SDSApp object

CeLTE_SDSApp theApp;


// CeLTE_SDSApp initialize

BOOL CeLTE_SDSApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create a shell manager to prevent a dialog box containing
	// either the shell tree view control or the shell list view control.
	CShellManager *pShellManager = new CShellManager;

	// standard initialize
	SetRegistryKey(_T("Application wizard generated local application"));

	CeLTE_SDSDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// As the dialog box is closed, it will return FALSE to
	// exit the application instead of starting the application's message pump.
	return FALSE;
}

