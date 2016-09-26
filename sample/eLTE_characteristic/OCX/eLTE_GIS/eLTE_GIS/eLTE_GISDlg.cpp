/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/
// eLTE_GISDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "eLTE_GIS.h"
#include "eLTE_GISDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CeLTE_GISDlg 对话框



int CeLTE_GISDlg::m_iBypass = 1;
CeLTE_GISDlg::CeLTE_GISDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_GISDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPasswd(_T(""))
	, m_strServerIP(_T(""))
	, m_strLocalIP(_T(""))
	, m_strSipPort(_T(""))
	, m_strLogSavePath(_T(""))
	, m_bIsLogin(FALSE)
	, m_bMax(FALSE)
	, m_bClose(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strName = _T("");
	m_strSipPort = _T("");
	m_strServerIP = _T("");
	m_strPasswd = _T("");
	m_strLocalIP = _T("");
}

void CeLTE_GISDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPasswd);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Control(pDX, IDC_ELTE_PLAYERCTRL1, m_eLTE_Player);
}

BEGIN_MESSAGE_MAP(CeLTE_GISDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_GISDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CeLTE_GISDlg 消息处理程序

BOOL CeLTE_GISDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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
	

	// 初始化登陆信息
	if (!ReadIniFile())
	{
		m_strName = _T("4101");
		m_strPasswd = _T("4101");
		m_strServerIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}

	// 初始日志信息
	m_strLogSavePath = _T(".\\log");
	UpdateData(FALSE);

	// 创建DConsoleDlg
	m_DcDlg.SetCeLTE_GISDlg(this);
	m_DcDlg.SetELtePlayer(&m_eLTE_Player);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_GISDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_GISDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当user 拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CeLTE_GISDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CeLTE_GISDlg::OnBnClickedButtonLogin()
{
	//登陆前其他非必须参数设置
	MoreSetting();
	UpdateData(TRUE);

	int iRet = 0;
	CString strResult;

	strResult = m_eLTE_Player.ELTE_OCX_Load(eLTE_LOAD_OPERATE_MGR);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_Load"));

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);

	strResult = m_eLTE_Player.ELTE_OCX_Login(m_strName, m_strPasswd, m_strServerIP, m_strLocalIP, m_strSipPort);
	iRet = GET_XML_RESULTE_CODE(strResult);
	if (0 != iRet)
	{
		CString str;
		str.Format(_T("ELTE_OCX_Login Failed. (%d)"), iRet);
		MessageBox(str);
		PostMessage(WM_CLOSE);
		return;
	}
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_Login"));
	m_DcDlg.SetParam(m_strName, m_strServerIP, m_strLocalIP);
	WriteIniFile();
}


BOOL CeLTE_GISDlg::GetLocalIP() 
{
	WORD wVersionReq = MAKEWORD(2, 2);
	WSADATA wsaData;

	if (eLTE_ERR_SUCCESS != WSAStartup(wVersionReq, &wsaData))
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


BOOL CeLTE_GISDlg::ReadIniFile()
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
	m_strName = tchValue;

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH);
	GetPrivateProfileString(_T("LoginInfo"), _T("Passwd"), _T(""), tchValue, LENGTH, szPath);
	m_strPasswd = tchValue;

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


void CeLTE_GISDlg::MoreSetting()
{
	UpdateData(TRUE);
	// 设置日志参数
	m_eLTE_Player.ELTE_OCX_SetLogLevel(0);
	m_eLTE_Player.ELTE_OCX_SetLogPath(m_strLogSavePath);

	// 获取版本
	CString strResult = m_eLTE_Player.ELTE_OCX_GetVersion(eLTE_VERSION_OCX);//1 ocx, 2 SDK
	CString strVersion = GET_XML_ELEM_VALUE_STR(strResult, _T("Version"));
	CString strText;
	strText.Format(_T("DConsole    [%s]"), strVersion);
	m_DcDlg.SetWindowText(strText);
}


BOOL CeLTE_GISDlg::WriteIniFile()
{
	TCHAR pszPath[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), pszPath, MAX_PATH);
	CString szPath(pszPath);
	szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
	szPath.Append(_T("Server.ini"));

	WritePrivateProfileString(_T("LoginInfo"), _T("Name"), m_strName, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("Passwd"), m_strPasswd, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("ServerIP"), m_strServerIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("LocalIP"), m_strLocalIP, szPath);
	//WritePrivateProfileString(_T("LoginInfo"), _T("MRSIP"), m_strMRSIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("SipPort"), m_strSipPort, szPath);

	return TRUE;
}

BEGIN_EVENTSINK_MAP(CeLTE_GISDlg, CDialogEx)
	ON_EVENT(CeLTE_GISDlg, IDC_ELTE_PLAYERCTRL1, 50, CeLTE_GISDlg::ELTE_OCX_EventEltePlayerctrl1, VTS_UI4 VTS_BSTR)
	END_EVENTSINK_MAP()


void CeLTE_GISDlg::ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	// TODO: 在此处添加消息处理程序代码
	switch (ulEventType)
	{
	case EVENT_NOTIFY_USER_STATUS:	// notifyUserStatus
		{
			CString strUserID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("UserID"));
			int iStatusType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusType"));
			int iStatusValue = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusValue"));

			m_DcDlg.UpdateUserStatus(strUserID, iStatusValue);

			// 显示视频回传当前状态
			CString strEventMsg;
			strEventMsg.Format(_T("UserID:%s Type:%d Value:%d 【"), strUserID, iStatusType, iStatusValue);
			if (iStatusValue == 4011)
			{
				strEventMsg.Append(_T("online"));
			}
			else if (iStatusValue == 4012)
			{
				strEventMsg.Append(_T("offline"));
			}
			else if (iStatusValue == 4020)
			{
				strEventMsg.Append(_T("start calling processing"));
			}
			else if (iStatusValue == 4021)
			{
				strEventMsg.Append(_T("ringing"));
			}
			else if (iStatusValue == 4022)
			{
				strEventMsg.Append(_T("using"));
			}
			else if (iStatusValue == 4023)
			{
				strEventMsg.Append(_T("free"));
			}
			strEventMsg.Append(_T("】\r\n"));
			strEventMsg.Insert(0,GetTimeString());
			m_DcDlg.m_strEvent0.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_RESOURCE_STATUS:	// notifyResourceStatus
		{
			CString strResId = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ResourceID"));
			int iType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusType"));
			int iValue = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusValue"));

			// 回调事件消息显示
			CString strEventMsg;
			strEventMsg.Format(_T("Type:%d Value:%d ResId:%s 【"), iType, iValue, strResId);
			strEventMsg.Insert(0,GetTimeString());

			if (_ttoi(strResId) == _ttoi(m_strName) && (RESREGSTATUS_PROXY == iType || RESREGSTATUS_DIRECT == iType))
			{
				// login success
				if (STATUS_REGOK == iValue)
				{
					if (!GetDlgItem(IDC_BUTTON_LOGIN)->IsWindowEnabled())
					{
						if (!m_bIsLogin)
						{
							MessageBox(_T("login success."));
							m_DcDlg.ShowWindow(SW_SHOW);
							ShowWindow(SW_HIDE);
							m_bIsLogin = TRUE;
							strEventMsg.Append(_T("login success."));
						}
						else
						{
							strEventMsg.Append(_T("login success(Heartbeat check)."));
						}
					}
				/*	else
					{
						MessageBox(_T("login success."));
						strEventMsg.Append(_T("login success."));
					}*/
				}
				// 登陆失败
				else if (STATUS_REGFAIL == iValue)
				{
					if (!m_bIsLogin)
					{
						MessageBox(_T("login failed."));
						strEventMsg.Append(_T("login failed."));
					}
					else
					{
						//MessageBox(_T("login success."));
						strEventMsg.Append(_T("login success."));
						//logout
						m_bIsLogin = FALSE;
					}
					//return;
				}
				// 资源未授权
				else if (STATUS_FORBIDEN == iValue)
				{
					MessageBox(_T("resource Not authorized."));
					strEventMsg.Append(_T("resource Not authorized."));
				}
				// 账号错误
				else if (STATUS_NOT_FOUND == iValue)
				{
					MessageBox(_T("account or password error."));
					strEventMsg.Append(_T("account or password error."));
				}
				// resource not available now
				else if (STATUS_TEMP_UNAVAILABLE == iValue)
				{
					MessageBox(_T("resource not available now."));
					strEventMsg.Append(_T("resource not available now."));
				}
				// resource conflict
				else if (STATUS_RES_CONFILCT == iValue)
				{
					MessageBox(_T("resource conflict."));
					strEventMsg.Append(_T("resource conflict."));
				}
				// 4017 STATUS_UNAUTHORIZED 未鉴权（登陆中间状态）
				else if (STATUS_UNAUTHORIZED == iValue)
				{
					if (!m_bIsLogin)
					{
						strEventMsg.Append(_T("Without authentication（Landing intermediate state）."));
					}
					else
					{
						strEventMsg.Append(_T("Without authentication（Landing intermediate state）(Heartbeat Check)."));
					}
				}
				// License限制
				else if (STATUS_LICENSE_LIMIT == iValue)
				{
					MessageBox(_T("License限制."));
					strEventMsg.Append(_T("License限制."));
				}
				// 安全红线
				//// 密码错误
				//else if (STATUS_PASSWORD_WRONG == iValue)
				//{
				//	MessageBox(_T("密码错误."));
				//	strEventMsg.Append(_T("密码错误."));
				//}

				// 登陆失败则退出MFC程序
				if (STATUS_REGOK != iValue && STATUS_UNAUTHORIZED != iValue)
				{
					ShowWindow(SW_HIDE);
					PostMessage(WM_CLOSE);
				}
			}

			// 回调事件消息显示
			strEventMsg.Append(_T("】\r\n"));
			m_DcDlg.m_strEvent2.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_PROVISION_CHANGE: //notifyProvisionChange
		{
			CString strChangeType = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ChangeType"));
			CString strChangeMO = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ChangeMO"));

			CString strEventMsg;
			strEventMsg.Format(_T("ChangeType:%s ChangeMO:%s Content:%s\r\n"), strChangeType, strChangeMO, pEventDataXml);
			strEventMsg.Insert(0, GetTimeString());
			m_DcDlg.m_strEvent3.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_PROVISION_ALLRESYNC: // notifyProvisionAllResync
		{
			//开始初始化ProvMgr
			m_DcDlg.InitDlg();

			CString strEventMsg = GetTimeString();
			strEventMsg.Append(_T("notifyProvisionAllResync\r\n"));
			m_DcDlg.m_strEvent4.Append(strEventMsg);

			// 初始化MRS
			if(!m_bClose)
			{
				CString strResult = m_eLTE_Player.ELTE_OCX_ProvisionManagerInitMRS(m_strServerIP);
				int iRet = 0;
				CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_ProvisionManagerInitMRS"));
			}
		}
		break;
	case EVENT_NOTIFY_GROUP_STATUS: // notifyGroupStatus
		{
			int iType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("GroupCallStatus"));
			CString strGroupID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("GroupID"));
			CString strSpeaker = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Speaker"));

			CString strEventMsg;
			strEventMsg.Format(_T("Type:%d GroupID:%s Speaker:%s"), iType, strGroupID, strSpeaker);
			strEventMsg.Insert(0,GetTimeString());
			if (PTT_IND_STATUS_EMERGENCY_BEGIN == iType)
			{
				// Emergency call
				strEventMsg.Append(_T(" 【Emergency call】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_SNATCH == iType)
			{
				strEventMsg.Append(_T(" 【Success of group call】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_BEGIN == iType)
			{
				strEventMsg.Append(_T(" 【Change of the power of speech】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_IDLE == iType)
			{
				strEventMsg.Append(_T(" 【Right to speak】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_ACCEPT == iType)
			{
				strEventMsg.Append(_T(" 【Release the right to receive】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_REJECT == iType)
			{
				strEventMsg.Append(_T(" 【Rob the right to refuse】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_HANGUP_OK == iType)
			{
				strEventMsg.Append(_T(" 【Quit group call success】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CLOSE_OK == iType)
			{
				strEventMsg.Append(_T(" 【End of group call】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
			{
				strEventMsg.Append(_T(" 【Not allowed】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CANCEL_OK == iType)
			{
				// Emergency call
				strEventMsg.Append(_T(" 【Failed group call】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else
			{
				strEventMsg.Append(_T("\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
		}
		break;
	case EVENT_NOTIFY_MODULE_STATUS: // notifyModuleStatus
		{
			CString strResourceID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ResouceID"));
			int iModuleType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ModuleType"));
			int iModuleStatus = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ModuleStatus"));
			int iCallBackMsgType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("CallBackMsgType"));
			CString strModulePara = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ModulePara"));

			CString strEventMsg;
			strEventMsg.Format(_T("ResouceID:%s ModuleType:%d ModuleStatus:%d CallBackMsgType:%d ModulePara:%s"), strResourceID, iModuleType, iModuleStatus, iCallBackMsgType, strModulePara);
			strEventMsg.Insert(0,GetTimeString());
			if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // 被踢下线
			{
				CString strMsg;
				strMsg.Format(_T("user 【%s】 Login somewhere else\r\n%s"), m_strName, strModulePara);
				strEventMsg.Append(_T(" 【kicked off】"));
				m_strKickOff = strMsg;
				PostMessage(WM_CLOSE);
			}
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent7.Append(strEventMsg);
		}
		break;

	case EVENT_NOTIFY_GIS_STATUS: // notifyGISStatus
		{
			CString strResourceID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ResourceID"));
			CString strModulePara = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("AckStatusList"));

			CString strEventMsg;
			strEventMsg.Format(_T("ResouceID:%s AckStatusList:%s"), strResourceID, strModulePara);
			strEventMsg.Insert(0,GetTimeString());
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent8.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_GIS_REPORT: // notifyGISReport
		{
			CString strResourceID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ResourceID"));
			CString strTime = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Time"));
			CString strAltitude = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Altitude"));
			CString strLatitude = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Latitude"));
			CString strLongtitude = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Longtitude"));
			int iTriggerCode = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("TriggerCode"));
			CString strTriggerCode;
			if(0 == iTriggerCode)
			{
				strTriggerCode = _T("Periodic reporting");
			}
			else if(1 == iTriggerCode)
			{
				strTriggerCode = _T("Specific event, emergency call");
			}

			int iReportStatus = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ReportStatus"));
			CString strReportStatus;
			if(0 == iReportStatus)
			{
				strReportStatus = _T("GPS Normal opening");
			}
			else if(1 == iReportStatus)
			{
				strReportStatus = _T("GPSsearch satellite fail");
			}
			else if(2 == iReportStatus)
			{
				strReportStatus = _T("Other anomalies");
			}

			CString strEventMsg;
			strEventMsg.Format(_T("ResouceID:%s Time:%s Altitude:%s Longtitude:%s Latitude:%s TriggerCode:%s ReportStatus:%s"), strResourceID, strTime, strAltitude, strLongtitude, strLatitude, strTriggerCode, strReportStatus);
			strEventMsg.Insert(0,GetTimeString());
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent9.Append(strEventMsg);
		}
		break;
	}
}

CString CeLTE_GISDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}
