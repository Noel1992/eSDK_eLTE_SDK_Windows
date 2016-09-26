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

// eLTE_ResourceManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_ResourceManage.h"
#include "eLTE_ResourceManageDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "DataType.h"

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


// CeLTE_ResourceManageDlg dialog




int CeLTE_ResourceManageDlg::m_iBypass = 1;
CeLTE_ResourceManageDlg::CeLTE_ResourceManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_ResourceManageDlg::IDD, pParent)
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

void CeLTE_ResourceManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	//DDX_Control(pDX, IDC_COMBO_LOGLV, m_cmbLogLevel);
	DDX_Control(pDX, IDC_COMBO_MEDIA_PASS, m_cmbMediaPass);
	//DDX_Text(pDX, IDC_EDIT_LOGPATH, m_strLogSavePath);
	DDX_Text(pDX, IDC_EDIT_MRSIP, m_strMRSIP);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Control(pDX, IDC_ELTE_PLAYERCTRL1, m_eLTE_Player);
}

BEGIN_MESSAGE_MAP(CeLTE_ResourceManageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_ResourceManageDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeLTE_ResourceManageDlg message handlers

BOOL CeLTE_ResourceManageDlg::OnInitDialog()
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
	/*m_cmbLogLevel.InsertString(0, _T("Error"));
	m_cmbLogLevel.InsertString(0, _T("Warn"));
	m_cmbLogLevel.InsertString(0, _T("Info"));
	m_cmbLogLevel.InsertString(0, _T("Debug"));
	m_cmbLogLevel.SetCurSel(0);*/
	UpdateData(FALSE);

	// 创建DConsoleDlg
	m_DcDlg.SetCeLTE_ResourceManageDlg(this);
	m_DcDlg.SetELtePlayer(&m_eLTE_Player);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_ResourceManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_ResourceManageDlg::OnPaint()
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
HCURSOR CeLTE_ResourceManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




BEGIN_EVENTSINK_MAP(CeLTE_ResourceManageDlg, CDialogEx)
	ON_EVENT(CeLTE_ResourceManageDlg, IDC_ELTE_PLAYERCTRL1, 50, CeLTE_ResourceManageDlg::ELTE_OCX_EventEltePlayerctrl1, VTS_UI4 VTS_BSTR)	
	ON_EVENT_RANGE(CeLTE_ResourceManageDlg, OCX_ID_BEGIN, OCX_ID_END, 51, CeLTE_ResourceManageDlg::ELTE_OCX_PlayerEventEltePlayerctrl1, VTS_UI4 VTS_UI4 VTS_BSTR)
END_EVENTSINK_MAP()


void CeLTE_ResourceManageDlg::ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml)
{
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
				// Logout success 
				if (STATUS_REGOK == iValue)
				{
					if (!GetDlgItem(IDC_BUTTON_LOGIN)->IsWindowEnabled())
					{
						if (!m_bIsLogin)
						{
							MessageBox(_T("Login success."));
							m_DcDlg.ShowWindow(SW_SHOW);
							ShowWindow(SW_HIDE);
							m_bIsLogin = TRUE;
							strEventMsg.Append(_T("Login success."));
						}
						else
						{
							strEventMsg.Append(_T("Login success(heartbeat check)."));
						}
					}
				/*	else
					{
						MessageBox(_T("Logout success ."));
						strEventMsg.Append(_T("Logout success ."));
					}*/
				}
				// 登陆fail 
				else if (STATUS_REGFAIL == iValue)
				{
					if (!m_bIsLogin)
					{
						MessageBox(_T("Login fail ."));
						strEventMsg.Append(_T("Login fail ."));
					}
					else
					{
						//MessageBox(_T("Logout success ."));
						strEventMsg.Append(_T("Logout success ."));
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
				// 4017 STATUS_UNAUTHORIZED Without authentication（Landing intermediate state）
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
				// LicenseLimited
				else if (STATUS_LICENSE_LIMIT == iValue)
				{
					MessageBox(_T("License Limited."));
					strEventMsg.Append(_T("License Limited."));
				}
				// 安全红线
				//// 密码错误
				//else if (STATUS_PASSWORD_WRONG == iValue)
				//{
				//	MessageBox(_T("密码错误."));
				//	strEventMsg.Append(_T("密码错误."));
				//}

				// 登陆fail 则退出MFC程序
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
					strEventMsg.Append(_T("group under active status."));
				}
				//  组非活动状态
				else if (STATUS_GROUP_DEACTIVATED == iValue)
				{
					//MessageBox(_T("组非活动状态."));
					strEventMsg.Append(_T("group under inactive status."));
				}
			}

			if (RESASSIGNSTATUS == iType)//群组订阅
			{
				if (STATUS_ASSIGNED == iValue)//订阅success 
				{
					CString strMsg;
					strMsg.Format(_T("group [%s] subscribe success."), strResId);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DEASSIGNED == iValue)//去订阅success 
				{
					CString strMsg;
					strMsg.Format(_T("Group [%s] unsubscribe success."), strResId);					
					//delete temp Group
					//m_DcDlg.RemoveGroup(StrToInt(strResId));
					//获取选中项信息
					GroupInfo* pInfo = NULL;
					//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
					CString szItemroot(strResId);
					szItemroot.Append(_T(" [temp Group]"));
					HTREEITEM htiRes = m_DcDlg.FindStrGroupInfo(szItemroot, &pInfo);

					if ( (NULL != pInfo)&& (NULL != htiRes))
					{
						if ("-1" == pInfo->GroupCategory)
						{
							//delete temp Group
							m_DcDlg.RemoveGroup(StrToInt(strResId));
						}					
					}
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			if (USERDGNASTATUS == iType)//Dynamic reorganization
			{
				int iCause = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Cause"));
				int iGroup = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("AttachingGroup"));
				if (STATUS_DGNAFAIL == iValue)//create failed
				{
					CString strMsg;
					strMsg.Format(_T("Dynamic reorganization[%d]create failed.reason=%d"), iGroup, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DGNAOK == iValue)//create succeed
				{
					CString strMsg;
					strMsg.Format(_T("Dynamic reorganization[%d]create succeed."), iGroup);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.AddGroup(iGroup);
				}
				else if (STATUS_DGNA_UEFAIL == iValue)//Dynamic Group某些UE重组fail 
				{
					CString strMsg;
					//strMsg.Format(_T("Dynamic reorganization[%d]UE[%s]重组fail .reason=%d"), iGroup, strResId, iCause);
					if (iCause == -20044)
					{
						strMsg.Append(_T("【"));
						strMsg.Append(strResId);
						strMsg.Append(_T("unregistered】"));
						m_DcDlg.AddGroup(iGroup);
					}
					else if (iCause == -20042)
					{
						strMsg.Append(_T("【"));
						strMsg.Append(strResId);
						strMsg.Append(_T("overtime】"));
						m_DcDlg.AddGroup(iGroup);
					}
					else
					{
						strMsg.Format(_T("Dynamic reorganization[%d]UE[%s]reorganize failed.reason=%d"), iGroup, strResId, iCause);
					}
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			//Patch Group
			if (GRPPATCHSTATUS == iType)
			{
				int iCause = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Cause"));
				int iGroupID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ResourceID"));
				int iMemberID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("MemberID"));
				if(STATUS_GRPPATCH_CREATEOK == iValue)//create succeed
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]create succeed."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.AddPatchGroup(iGroupID);
				}
				else if(STATUS_GRPPATCH_CREATEFAIL == iValue)//create failed
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]create failed.reason=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_CANCELOK == iValue)//delete success
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]delete success."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.RemovePatchGroup(iGroupID);
				}
				else if(STATUS_GRPPATCH_CANCELFAIL == iValue)//delete fail
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]delete fail.reason=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_ADDOK == iValue)//add member success
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]add member success."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.AddPatchGroupMember(iGroupID, iMemberID);
				}
				else if(STATUS_GRPPATCH_ADDFAIL == iValue)//add member fail
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]add member fail.reason=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_DELOK == iValue)//delete member success 
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]delete member success ."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.DelPatchGroupMember(iGroupID, iMemberID);
				}
				else if(STATUS_GRPPATCH_DELFAIL == iValue)//delete member fail 
				{
					CString strMsg;
					strMsg.Format(_T("Patch Group[%d]delete member fail .reason=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			//show callback message
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
				strMsg.Format(_T("user 【%s】login somewhere else\r\n%s"), m_strName, strModulePara);
				strEventMsg.Append(_T(" 【Kicked off】"));
				m_strKickOff = strMsg;
				PostMessage(WM_CLOSE);
			}
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent7.Append(strEventMsg);
		}
		break;
	}
}


void CeLTE_ResourceManageDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
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
		m_DcDlg.SetCeLTE_ResourceManageDlg(NULL);
		m_DcDlg.SendMessage(WM_CLOSE);
		m_DcDlg.DestroyWindow();
	}


	m_eLTE_Player.ELTE_OCX_UnLoad();
	m_bClose = true;
	CDialogEx::OnClose();
}


void CeLTE_ResourceManageDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	//登陆前其他非必须参数设置
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

BOOL CeLTE_ResourceManageDlg::ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	// TODO: Add your message handler code here

	//m_DcDlg.ELTE_PlayerEvent(CtlID, ulEventType, pEventDataXml);

	return TRUE;
}

BOOL CeLTE_ResourceManageDlg::GetLocalIP() 
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


BOOL CeLTE_ResourceManageDlg::ReadIniFile()
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

BOOL CeLTE_ResourceManageDlg::WriteIniFile()
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

void CeLTE_ResourceManageDlg::MoreSetting()
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

CString CeLTE_ResourceManageDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}