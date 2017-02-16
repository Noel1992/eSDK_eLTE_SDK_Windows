
// eLTE_SDK_GIS_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_GIS_Demo.h"
#include "eLTE_SDK_GIS_DemoDlg.h"
#include "afxdialogex.h"
#include "eLTE_Tool.h"
#include "DataType.h"
#include "Xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CeLTE_SDK_GIS_DemoDlg dialog




CeLTE_SDK_GIS_DemoDlg::CeLTE_SDK_GIS_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_SDK_GIS_DemoDlg::IDD, pParent)
	, m_strLocalIP(_T(""))
	, m_strUserName(_T(""))
	, m_strServerIP(_T(""))
	, m_strSipPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLogin = FALSE;
}

void CeLTE_SDK_GIS_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
}

BEGIN_MESSAGE_MAP(CeLTE_SDK_GIS_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_SDK_GIS_DemoDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeLTE_SDK_GIS_DemoDlg message handlers

BOOL CeLTE_SDK_GIS_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (GetLocalIP())
	{
		printf("Get Local IP failed!\n");
	}

	if (!ReadIniFile())
	{
		m_strUserName = _T("4101");
		m_strServerIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}
	UpdateData(FALSE);
	if (NULL == m_DConsoleDlg.m_hWnd)
	{
		m_DConsoleDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());
		m_DConsoleDlg.SetELTE_SDK_DemoDlg(this);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_SDK_GIS_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CeLTE_SDK_GIS_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CeLTE_SDK_GIS_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CeLTE_SDK_GIS_DemoDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here

	// init SDK
	ELTE_INT32 iRet = ELTE_SDK_Init(FALSE);
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_Init failed."));
		return;
	}

	// set call back function
	iRet = ELTE_SDK_SetEventCallBack(ELTE_EventCallBack, this);
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_SetEventCallBack failed."));
		return;
	}

	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(m_strUserName);

	CString strPWD;
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPWD);

	GetDlgItem(IDC_EDIT_SERVERIP)->GetWindowText(m_strServerIP);

	GetDlgItem(IDC_COMBO_LOCALIP)->GetWindowText(m_strLocalIP);

	GetDlgItem(IDC_EDIT_SIPPORT)->GetWindowText(m_strSipPort);

	//login
	iRet = ELTE_SDK_Login(eLTE_Tool::UnicodeToANSI(m_strUserName).c_str(), eLTE_Tool::UnicodeToANSI(strPWD).c_str(), eLTE_Tool::UnicodeToANSI(m_strServerIP).c_str(), eLTE_Tool::UnicodeToANSI(m_strLocalIP).c_str(), atoi(eLTE_Tool::UnicodeToANSI(m_strSipPort).c_str()));
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_Login failed."));
		return;
	}
	WriteIniFile();
}

CString CeLTE_SDK_GIS_DemoDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}

ELTE_VOID __SDK_CALL CeLTE_SDK_GIS_DemoDlg::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	char* pEventXml = (char*)pEventBuf;
	CString xmlStr(pEventXml);
	CString strRecvVideoPlayCallerId;
	CeLTE_SDK_GIS_DemoDlg* pDlg = (CeLTE_SDK_GIS_DemoDlg*)pUserData;

	//Res Download Success
	if(ELTE_Event_NotifyProvisionAllResync == iEventType)
	{
		pDlg->GetDConsoleDlg().PostMessage(WM_INIT);
	}

	//Res Status
	else if(ELTE_Event_NotifyResourceStatus == iEventType)
	{
		CString strResId = CXml::Instance().XmlParseElemValue(xmlStr, _T("ResourceID"));
		int iType = StrToInt(CXml::Instance().XmlParseElemValue(xmlStr, _T("StatusType")));
		int iValue = StrToInt(CXml::Instance().XmlParseElemValue(xmlStr, _T("StatusValue")));

		if (_ttoi(strResId) == _ttoi(pDlg->GetCurrentUserName()) && (RESREGSTATUS_PROXY == iType || RESREGSTATUS_DIRECT == iType))
		{
			if (STATUS_REGOK == iValue)
			{
				if(!pDlg->GetIsLogin())
				{
					pDlg->MessageBox(_T("Login Success"));
					pDlg->SetIsLogin(TRUE);
					pDlg->GetDConsoleDlg().ShowWindow(SW_SHOW);
					pDlg->ShowWindow(SW_HIDE);
				}
				//pDlg->PostMessageW(WM_CLOSE);
			}
			else if (STATUS_NOT_FOUND == iValue)
			{
				pDlg->MessageBox(_T("Account or Password is wrong."));
			}
			else if (STATUS_LICENSE_LIMIT == iValue)
			{
				pDlg->MessageBox(_T("License limit."));
			}
			else if (STATUS_RES_CONFILCT == iValue)
			{
				pDlg->MessageBox(_T("Res confilct."));
			}
			else if (STATUS_TEMP_UNAVAILABLE == iValue)
			{
				pDlg->MessageBox(_T("Res unavailable."));
			}
			else if (STATUS_FORBIDEN == iValue)
			{
				pDlg->MessageBox(_T("Res forbiden."));
			}
			else if (STATUS_REGFAIL == iValue)
			{
				if(!pDlg->GetIsLogin())
				{
					pDlg->MessageBox(_T("Res reg fail."));
				}
				else
				{
					pDlg->SetIsLogin(FALSE);
					pDlg->PostMessage(WM_CLOSE);
				}
			}
		}
	}
	else if (ELTE_Event_NotifyUserStatus == iEventType)
	{
		CString strUserID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("UserID"));
		int iStatusType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusType"));
		int iStatusValue = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusValue"));
		pDlg->GetDConsoleDlg().UpdateUserStatus(strUserID, iStatusValue);
	}
	else if (ELTE_Event_NotifyGISStatus == iEventType)
	{
		CString strResourceID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ResourceID"));
		CString strStatusList = GET_XML_ELEM_VALUE_STR(xmlStr, _T("AckStatusList"));

		CString strEventMsg;
		strEventMsg.Format(_T("ResouceID:%s AckStatusList:%s"), strResourceID, strStatusList);
		strEventMsg.Insert(0,pDlg->GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().SetGISStatus(strEventMsg);
	}
	else if (ELTE_Event_NotifyGISReport == iEventType)
	{
		CString strResourceID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ResourceID"));
		CString strTime = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Time"));
		CString strAltitude = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Altitude"));
		CString strLatitude = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Latitude"));
		CString strLongtitude = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Longtitude"));
		int iTriggerCode = GET_XML_ELEM_VALUE_INT(xmlStr, _T("TriggerCode"));
		CString strTriggerCode;
		if(0 == iTriggerCode)
		{
			strTriggerCode = _T("Reporting period");
		}
		else if(1 == iTriggerCode)
		{
			strTriggerCode = _T("Specific events, emergency call");
		}

		int iReportStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ReportStatus"));
		CString strReportStatus;
		if(0 == iReportStatus)
		{
			strReportStatus = _T("GPS normally open");
		}
		else if(1 == iReportStatus)
		{
			strReportStatus = _T("GPS satellite search failed");
		}
		else if(2 == iReportStatus)
		{
			strReportStatus = _T("Other errors");
		}

		CString strEventMsg;
		strEventMsg.Format(_T("ResouceID:%s Time:%s Altitude:%s Longtitude:%s Latitude:%s TriggerCode:%s ReportStatus:%s"), strResourceID, strTime, strAltitude, strLongtitude, strLatitude, strTriggerCode, strReportStatus);
		strEventMsg.Insert(0, pDlg->GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().SetGISReport(strEventMsg);
	}
}

void CeLTE_SDK_GIS_DemoDlg::SetIsLogin(BOOL bLogin)
{
	m_bLogin = bLogin;
}

CString& CeLTE_SDK_GIS_DemoDlg::GetCurrentUserName()
{
	return m_strUserName;
}

BOOL& CeLTE_SDK_GIS_DemoDlg::GetIsLogin()
{
	return m_bLogin;
}

CDConsoleDlg& CeLTE_SDK_GIS_DemoDlg::GetDConsoleDlg()
{
	return m_DConsoleDlg;
}

void CeLTE_SDK_GIS_DemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ELTE_SDK_SetEventCallBack(NULL, NULL);

	ELTE_INT32 iRet = ELTE_SDK_Cleanup();
	if (0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_Cleanup failed."));
	}

	m_DConsoleDlg.DestroyWindow();

	CDialogEx::OnClose();
}


BOOL CeLTE_SDK_GIS_DemoDlg::ReadIniFile()
{
	TCHAR pszPath[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), pszPath, MAX_PATH);
	CString szPath(pszPath);
	szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
	szPath.Append(_T("Server.ini"));

	if (!(::PathFileExists(szPath)))
	{
		return FALSE;
	}

	const int LENGTH = 16;
	TCHAR tchValue[LENGTH] = {0};
	GetPrivateProfileString(_T("LoginInfo"), _T("Name"), _T(""), tchValue, LENGTH, szPath);
	m_strUserName = tchValue;

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH);
	GetPrivateProfileString(_T("LoginInfo"), _T("ServerIP"), _T(""), tchValue, LENGTH, szPath);
	m_strServerIP = tchValue;

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH);
	GetPrivateProfileString(_T("LoginInfo"), _T("LocalIP"), _T(""), tchValue, LENGTH, szPath);
	m_strLocalIP = tchValue;

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH);
	GetPrivateProfileString(_T("LoginInfo"), _T("SipPort"), _T(""), tchValue, LENGTH, szPath);
	m_strSipPort = tchValue;

	return TRUE;
}


BOOL CeLTE_SDK_GIS_DemoDlg::WriteIniFile()
{
	TCHAR pszPath[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), pszPath, MAX_PATH);
	CString szPath(pszPath);
	szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
	szPath.Append(_T("Server.ini"));

	WritePrivateProfileString(_T("LoginInfo"), _T("Name"), m_strUserName, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("ServerIP"), m_strServerIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("LocalIP"), m_strLocalIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("SipPort"), m_strSipPort, szPath);

	return TRUE;
}

BOOL CeLTE_SDK_GIS_DemoDlg::GetLocalIP() 
{
	WORD wVersionReq = MAKEWORD(2, 2);
	WSADATA wsaData;

	if (0 != WSAStartup(wVersionReq, &wsaData))
	{
		return FALSE;
	}
	//local variation
	char localIp[255] = {0};
	gethostname(localIp, sizeof(localIp));
	hostent* phost = gethostbyname(localIp);
	if (NULL == phost)
	{
		return FALSE;
	}

	HOSTENT* host = gethostbyname(localIp);	
	for (int i=0; i<256; i++)
	{
		m_cmbLocalIP.AddString(LPCTSTR(CString(inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]))));
		if (host->h_addr_list[i]+host->h_length >= host->h_name)
		{
			break;
		}
	}
	return TRUE;
}
