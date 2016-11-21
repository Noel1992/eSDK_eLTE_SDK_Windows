
// eLTE_SDS.cpp : implementation file¡£
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


// Only one CeLTE_SDSApp object

CeLTE_SDSApp theApp;


// CeLTE_SDSApp init

BOOL CeLTE_SDSApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	CShellManager *pShellManager = new CShellManager;

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

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

