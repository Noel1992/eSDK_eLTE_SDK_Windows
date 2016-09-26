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
// eLTE_VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "eLTE_Video.h"
#include "eLTE_VideoDlg.h"
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


// CeLTE_VideoDlg 对话框



int CeLTE_VideoDlg::m_iBypass = 1;
CeLTE_VideoDlg::CeLTE_VideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_VideoDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPasswd(_T(""))
	, m_strServerIP(_T(""))
	, m_strLocalIP(_T(""))
	, m_strMRSIP(_T(""))
	, m_strSipPort(_T(""))
	, m_strLogSavePath(_T(""))
	, m_bIsLogin(FALSE)
	, m_bMax(FALSE)
	, m_bClose(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CeLTE_VideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ELTE_PLAYERCTRL1, m_eLTE_Player);
	//  DDX_Control(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	DDX_Control(pDX, IDC_COMBO_MEDIA_PASS, m_cmbMediaPass);
	DDX_Text(pDX, IDC_EDIT_MRSIP, m_strMRSIP);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	//  DDX_Control(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
}

BEGIN_MESSAGE_MAP(CeLTE_VideoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_VideoDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeLTE_VideoDlg 消息处理程序

BOOL CeLTE_VideoDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cmbMediaPass.InsertString(0, _T("1"));
	m_cmbMediaPass.InsertString(1, _T("0"));
	m_cmbMediaPass.SetCurSel(1);
	
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
		m_strMRSIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}

	// 初始日志信息
	m_strLogSavePath = _T(".\\log");
	UpdateData(FALSE);

	// 创建DConsoleDlg
	m_DcDlg.SetCeLTE_VideoDlg(this);
	m_DcDlg.SetELtePlayer(&m_eLTE_Player);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CeLTE_VideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CeLTE_VideoDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CeLTE_VideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CeLTE_VideoDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	MoreSetting();
	UpdateData(TRUE);

	int iRet = 0;
	CString strResult;
	if (1 == m_cmbMediaPass.GetCurSel())
	{
		strResult = m_eLTE_Player.ELTE_OCX_SetBypassBuildMedia(0);
		m_iBypass = 0;

		CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_SetBypassBuildMedia"));
	}

	strResult = m_eLTE_Player.ELTE_OCX_Load(eLTE_LOAD_OPERATE_MGR);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_Load"));

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	//EnableWindow(FALSE);

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
BEGIN_EVENTSINK_MAP(CeLTE_VideoDlg, CDialogEx)
	ON_EVENT(CeLTE_VideoDlg, IDC_ELTE_PLAYERCTRL1, 50, CeLTE_VideoDlg::ELTE_OCX_EventEltePlayerctrl1, VTS_UI4 VTS_BSTR)
	ON_EVENT_RANGE(CeLTE_VideoDlg, OCX_ID_BEGIN, OCX_ID_END, 51, CeLTE_VideoDlg::ELTE_OCX_PlayerEventEltePlayerctrl1, VTS_UI4 VTS_UI4 VTS_BSTR)
END_EVENTSINK_MAP()


void CeLTE_VideoDlg::ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml)
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
				strEventMsg.Append(_T("start calling"));
			}
			else if (iStatusValue == 4021)
			{
				strEventMsg.Append(_T("Ringing"));
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
	case EVENT_NOTIFY_P2P_VIDEO_CALL_STATUS:	// notifyP2pvideocallStatus
		{
			int iCallStatus = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("CallStatus"));
			INT iResID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Callee"));
			
			if (3003 == iCallStatus /*|| 3006 == iCallStatus*/)
			{
				//视频回传接听（或者镜头切换中间过程）				
				UINT uiLocalVideoPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("LocalVideoPort"));
				UINT uiLocalAudioPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("LocalAudioPort"));
				UINT uiRemoteVideoPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("RemoteVideoPort"));
				UINT uiRemoteAudioPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("RemoteAudioPort"));

				m_DcDlg.SetRealPlayParam(iResID, uiLocalVideoPort, uiLocalAudioPort, uiRemoteVideoPort, uiRemoteAudioPort);
				if(!m_iBypass)
				{
					m_DcDlg.DoMute(iResID);

				}
			}
			else if (3011 == iCallStatus)
			{
				//终端视频上传或者视频分发

				CString strResID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Uri"));
				CString strMuteType = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SoundMute"));

				m_DcDlg.SetVideoCaller(strResID,strMuteType, 0);
				m_DcDlg.PostMessage(WM_VIDEORECEIVED, 0, 0);
			}
			else if (3009 == iCallStatus)
			{
				//视频回传挂断（或者镜头切换中间过程）
				m_DcDlg.RealPlayHangup(iResID);
			}
			else if (3021 == iCallStatus || 3013 == iCallStatus || 3008 == iCallStatus)
			{
				//连接失败、对方忙、对方挂断
				if(3008 == iCallStatus)
				{
					//对于回传和视频分发给本调度台的视频，如果对方关闭的话，应该从Uri获取相应的视频resid
					iResID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Uri"));
				}
				m_DcDlg.RealPlayNotConnected(iResID, iCallStatus);
			}

			// 显示视频回传当前状态
			CString strEventMsg;
			strEventMsg.Format(_T("Type:%d Camera:%d 【"), iCallStatus, iResID);
			if (3001 == iCallStatus)
			{
				strEventMsg.Append(_T("Initiated."));
			}
			else if (3002 == iCallStatus)
			{
				strEventMsg.Append(_T("Received."));
			}
			else if (3003 == iCallStatus)
			{
				strEventMsg.Append(_T("Answered."));
			}
			else if (3004 == iCallStatus)
			{
				strEventMsg.Append(_T("Proceeding."));
			}
			else if (3005 == iCallStatus)
			{
				strEventMsg.Append(_T("Ringing."));
			}
			else if (3006 == iCallStatus)
			{
				strEventMsg.Append(_T("Acked."));
			}
			else if (3009 == iCallStatus)
			{
				strEventMsg.Append(_T("HangupActive."));
			}
			else if (3008 == iCallStatus)
			{
				strEventMsg.Append(_T("Hangup."));
			}
			else if (3013 == iCallStatus)
			{
				strEventMsg.Append(_T("ReleasedBusy."));
			}
			else if (3021 == iCallStatus)
			{
				strEventMsg.Append(_T("NotConnected."));
			}
			else if (3022 == iCallStatus)
			{
				strEventMsg.Append(_T("NotSupported."));
			}
			// 回调事件消息显示
			strEventMsg.Append(_T("】\r\n"));
			strEventMsg.Insert(0,GetTimeString());
			m_DcDlg.m_strEvent1.Append(strEventMsg);
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
				// resource unauthority
				else if (STATUS_FORBIDEN == iValue)
				{
					MessageBox(_T("resource unauthority."));
					strEventMsg.Append(_T("resource unauthority."));
				}
				// 账号错误
				else if (STATUS_NOT_FOUND == iValue)
				{
					MessageBox(_T("account or password error."));
					strEventMsg.Append(_T("account or password error."));
				}
				// resource unaccessible
				else if (STATUS_TEMP_UNAVAILABLE == iValue)
				{
					MessageBox(_T("resource unaccessible."));
					strEventMsg.Append(_T("resource unaccessible."));
				}
				// resource conflict
				else if (STATUS_RES_CONFILCT == iValue)
				{
					MessageBox(_T("resource conflict."));
					strEventMsg.Append(_T("resource conflict."));
				}
				// 4017 STATUS_UNAUTHORIZED 未鉴权（login processing）
				else if (STATUS_UNAUTHORIZED == iValue)
				{
					if (!m_bIsLogin)
					{
						strEventMsg.Append(_T("Unauthority（login processing）."));
					}
					else
					{
						strEventMsg.Append(_T("Unauthority（login processing）(Heartbeat check)."));
					}
				}
				// LicenseLimited
				else if (STATUS_LICENSE_LIMIT == iValue)
				{
					MessageBox(_T("License Limited."));
					strEventMsg.Append(_T("License Limited."));
				}

				// 登陆失败则退出MFC程序
				if (STATUS_REGOK != iValue && STATUS_UNAUTHORIZED != iValue)
				{
					ShowWindow(SW_HIDE);
					PostMessage(WM_CLOSE);
				}
			}

			if(GRPCALLSTATUS == iType)
			{
				// 组活动状态
				if (STATUS_GROUP_ACTIVATED == iValue)
				{
					//MessageBox(_T("组活动状态."));
					strEventMsg.Append(_T("Group Call Active."));
				}
				//  组非活动状态
				else if (STATUS_GROUP_DEACTIVATED == iValue)
				{
					//MessageBox(_T("组非活动状态."));
					strEventMsg.Append(_T("Group Call Deactive."));
				}
			}

			if (RESASSIGNSTATUS == iType)//群组订阅
			{
				if (STATUS_ASSIGNED == iValue)//订阅成功
				{
					CString strMsg;
					strMsg.Format(_T("Group [%s]Subscribe success."), strResId);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DEASSIGNED == iValue)//去订阅成功
				{
					CString strMsg;
					strMsg.Format(_T("Group[%s]Unsubscribe success."), strResId);					
					//删除tempGroup
					//m_DcDlg.RemoveGroup(StrToInt(strResId));
					//获取选中项信息
					GroupInfo* pInfo = NULL;
					//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
					CString szItemroot(strResId);
					szItemroot.Append(_T(" [tempGroup]"));
					HTREEITEM htiRes = m_DcDlg.FindStrGroupInfo(szItemroot, &pInfo);

					if ( (NULL != pInfo)&& (NULL != htiRes))
					{
						if ("-1" == pInfo->GroupCategory)
						{
							//删除tempGroup
							m_DcDlg.RemoveGroup(StrToInt(strResId));
						}					
					}
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
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
				CString strResult = m_eLTE_Player.ELTE_OCX_ProvisionManagerInitMRS(m_strMRSIP);
				int iRet = 0;
				CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_ProvisionManagerInitMRS"));
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
				strMsg.Format(_T("user【%s】login somewhere else\r\n%s"), m_strName, strModulePara);
				strEventMsg.Append(_T(" 【kickoff】"));
				m_strKickOff = strMsg;
				PostMessage(WM_CLOSE);
			}
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent7.Append(strEventMsg);
		}
		break;
	}
}


void CeLTE_VideoDlg::MoreSetting()
{
	//if (!m_bMax)
	//	return;

	UpdateData(TRUE);
	// 设置日志参数
	m_eLTE_Player.ELTE_OCX_SetLogLevel(0);
	m_eLTE_Player.ELTE_OCX_SetLogPath(m_strLogSavePath);

	// 设置logo路径
/*	if (!m_strBGLogoPath.IsEmpty())
	{
		m_eLTE_Player.ELTE_OCX_UploadLogo(m_strBGLogoPath);
	}
*/
	// 获取版本
	CString strResult = m_eLTE_Player.ELTE_OCX_GetVersion(eLTE_VERSION_OCX);//1 ocx, 2 SDK
	CString strVersion = GET_XML_ELEM_VALUE_STR(strResult, _T("Version"));
	CString strText;
	strText.Format(_T("DConsole    [%s]"), strVersion);
	m_DcDlg.SetWindowText(strText);
}

BOOL CeLTE_VideoDlg::WriteIniFile()
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
	WritePrivateProfileString(_T("LoginInfo"), _T("MRSIP"), m_strMRSIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("SipPort"), m_strSipPort, szPath);

	return TRUE;
}

BOOL CeLTE_VideoDlg::GetLocalIP() 
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



BOOL CeLTE_VideoDlg::ReadIniFile()
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
	GetPrivateProfileString(_T("LoginInfo"), _T("MRSIP"), _T(""), tchValue, LENGTH, szPath);
	m_strMRSIP = tchValue;

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH);
	GetPrivateProfileString(_T("LoginInfo"), _T("SipPort"), _T(""), tchValue, LENGTH, szPath);
	m_strSipPort = tchValue;

	return TRUE;
}


void CeLTE_VideoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_eLTE_Player.ELTE_OCX_Logout(m_strName);
	static bool bShowKickOff = FALSE;

	if(0 < m_strKickOff.GetLength() && !bShowKickOff)
	{
		bShowKickOff = TRUE;
		if (IDOK == m_DcDlg.MessageBox(m_strKickOff))
		{
		}

	}

	if (m_DcDlg.m_hWnd)
	{
		m_DcDlg.SetCeLTE_VideoDlg(NULL);
		m_DcDlg.SendMessage(WM_CLOSE);
		m_DcDlg.DestroyWindow();
	}


	m_eLTE_Player.ELTE_OCX_UnLoad();
	m_bClose = true;
	CDialogEx::OnClose();
}


BOOL CeLTE_VideoDlg::ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	// TODO: 在此处添加消息处理程序代码
	m_DcDlg.ELTE_PlayerEvent(CtlID, ulEventType, pEventDataXml);

	return TRUE;
}


CString CeLTE_VideoDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}

