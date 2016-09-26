
// eLTE_SDK_Video_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Video_Demo.h"
#include "eLTE_SDK_Video_DemoDlg.h"
#include "afxdialogex.h"
#include "MediaPlayerDlgMgr.h"

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


// CeLTE_SDK_Video_DemoDlg dialog




CeLTE_SDK_Video_DemoDlg::CeLTE_SDK_Video_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_SDK_Video_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLogin = FALSE;
}

void CeLTE_SDK_Video_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CeLTE_SDK_Video_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_SDK_Video_DemoDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeLTE_SDK_Video_DemoDlg message handlers

BOOL CeLTE_SDK_Video_DemoDlg::OnInitDialog()
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
	if (NULL == m_DConsoleDlg.m_hWnd)
	{
		m_DConsoleDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());
		m_DConsoleDlg.SetELTE_SDK_DemoDlg(this);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_SDK_Video_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_SDK_Video_DemoDlg::OnPaint()
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
HCURSOR CeLTE_SDK_Video_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CeLTE_SDK_Video_DemoDlg::OnBnClickedButtonLogin()
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

	GetDlgItem(IDC_EDIT_LOCALIP)->GetWindowText(m_strLocalIP);

	GetDlgItem(IDC_EDIT_SIPPORT)->GetWindowText(m_strSipPort);

	//login
	iRet = ELTE_SDK_Login(eLTE_Tool::UnicodeToANSI(m_strUserName).c_str(), eLTE_Tool::UnicodeToANSI(strPWD).c_str(), eLTE_Tool::UnicodeToANSI(m_strServerIP).c_str(), eLTE_Tool::UnicodeToANSI(m_strLocalIP).c_str(), atoi(eLTE_Tool::UnicodeToANSI(m_strSipPort).c_str()));
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_Login failed."));
		return;
	}
}

CString CeLTE_SDK_Video_DemoDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}

ELTE_VOID __SDK_CALL CeLTE_SDK_Video_DemoDlg::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	char* pEventXml = (char*)pEventBuf;
	CString xmlStr(pEventXml);
	CString strRecvVideoPlayCallerId;
	CeLTE_SDK_Video_DemoDlg* pDlg = (CeLTE_SDK_Video_DemoDlg*)pUserData;

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
	else if (ELTE_Event_NotifyP2pvideocallStatus == iEventType)
	{
		int iCallStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("CallStatus"));
		CString strResID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));
		INT  iVideoFormatType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("VideoFormatType"));

		// current status
		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Camera:%s ¡¾"), iCallStatus, strResID);
		if (P2Pvideocall_IND_STATUS_INITIATED == iCallStatus)
		{
			strEventMsg.Append(_T("Initiated."));
		}
		else if (P2Pvideocall_IND_STATUS_RECEIVED == iCallStatus)
		{
			strEventMsg.Append(_T("Received."));
		}
		else if (P2Pvideocall_IND_STATUS_ANSWERED == iCallStatus)
		{
			strEventMsg.Append(_T("Answered."));
			//receive video				
			UINT uiLocalVideoPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("LocalVideoPort"));
			UINT uiLocalAudioPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("LocalAudioPort"));
			UINT uiRemoteVideoPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("RemoteVideoPort"));
			UINT uiRemoteAudioPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("RemoteAudioPort"));
			std::map<CString, int>::iterator&  iter = pDlg->GetVideoInfo().find(strResID);
			if(pDlg->GetVideoInfo().end() != iter)
			{
				iter->second = iVideoFormatType;
			}
			else
			{
				 pDlg->GetVideoInfo().insert(std::map<CString, int>::value_type (strResID, iVideoFormatType));
			}
			CString strMsg;
			strMsg.Format(_T("LocalVideoPort: %d, LocalAudioPort: %d, RemoteVideoPort: %d, RemoteAudioPort: %d"), uiLocalVideoPort, uiLocalAudioPort, uiRemoteVideoPort, uiRemoteAudioPort);
			strEventMsg.Append(strMsg);
			pDlg->GetDConsoleDlg().SetPlayResID(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			pDlg->GetDConsoleDlg().PostMessage(WM_START_REAL_PLAY);
		}
		else if (P2Pvideocall_IND_STATUS_PROCEEDING == iCallStatus)
		{
			strEventMsg.Append(_T("Proceeding."));
		}
		else if (P2Pvideocall_IND_STATUS_RINGING == iCallStatus)
		{
			strEventMsg.Append(_T("Ringing."));
		}
		else if (P2Pvideocall_IND_STATUS_ACKED == iCallStatus)
		{
			std::map<CString, int>::iterator&  iter = pDlg->GetVideoInfo().find(strResID);
			if(pDlg->GetVideoInfo().end() != iter)
			{
				iter->second = iVideoFormatType;
			}
			else
			{
				pDlg->GetVideoInfo().insert(std::map<CString, int>::value_type (strResID, iVideoFormatType));
			}
			strEventMsg.Append(_T("Acked."));
		}
		else if (P2Pvideocall_IND_STATUS_RELEASED == iCallStatus)
		{
			strEventMsg.Append(_T("Released."));
		}
		else if (P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE == iCallStatus)
		{
			strEventMsg.Append(_T("HangupActive."));
		}
		else if (P2Pvideocall_IND_STATUS_HANGUPED == iCallStatus)
		{
			strEventMsg.Append(_T("Hangup."));
		}
		else if (P2Pvideocall_IND_STATUS_RELEASED_BUSY == iCallStatus)
		{
			strEventMsg.Append(_T("ReleasedBusy."));
		}
		else if (P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED == iCallStatus)
		{
			strEventMsg.Append(_T("NotConnected."));
		}
		else if (P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED == iCallStatus)
		{
			strEventMsg.Append(_T("NotSupported."));
		}
		else if(P2Pvideocall_IND_STATUS_MONITOR == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
			strRecvVideoPlayCallerId = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Uri"));
			pDlg->GetDConsoleDlg().SetCommingVideo(strRecvVideoPlayCallerId);
			pDlg->GetDConsoleDlg().PostMessage(WM_INCOMMING_VIDEO);
		}
		else if (P2Pvideocall_IND_STATUS_CANCELLED == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
		}
		else if (P2Pvideocall_IND_STATUS_PICKEDUP == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
		}
		if (P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED == iCallStatus || P2Pvideocall_IND_STATUS_RELEASED_BUSY == iCallStatus || P2Pvideocall_IND_STATUS_HANGUPED == iCallStatus || P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE == iCallStatus)
		{
			//hangup
			CMediaPlayerDlg* pDlg = CMediaPlayerDlgMgr::Instance().GetMediaPlayerDlg(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			//In order to distinguish the current initiative to request video dispatcher return, or receive a video return
			if(NULL == pDlg)
			{
				if(P2Pvideocall_IND_STATUS_HANGUPED == iCallStatus)
				{
					strResID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Uri"));
					pDlg = CMediaPlayerDlgMgr::Instance().GetMediaPlayerDlg(eLTE_Tool::UnicodeToANSI(strResID).c_str());
				}
			}
			if(pDlg)
			{
				pDlg->PostMessage(WM_CLOSE);
			}
		}

		// show call back msg
		strEventMsg.Append(_T("¡¿\r\n"));
		strEventMsg.Insert(0,pDlg->GetTimeString());
		//Instance().m_DConsoleDlg.m_VideoStatus.Append(strEventMsg);
	}
}

void CeLTE_SDK_Video_DemoDlg::SetIsLogin(BOOL bLogin)
{
	m_bLogin = bLogin;
}

CString& CeLTE_SDK_Video_DemoDlg::GetCurrentUserName()
{
	return m_strUserName;
}

BOOL& CeLTE_SDK_Video_DemoDlg::GetIsLogin()
{
	return m_bLogin;
}

CDConsoleDlg& CeLTE_SDK_Video_DemoDlg::GetDConsoleDlg()
{
	return m_DConsoleDlg;
}


void CeLTE_SDK_Video_DemoDlg::OnClose()
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
