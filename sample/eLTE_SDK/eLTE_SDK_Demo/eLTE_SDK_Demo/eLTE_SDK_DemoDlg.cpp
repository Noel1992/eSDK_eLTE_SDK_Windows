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


// eLTE_SDK_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "eLTE_SDK_DemoDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "MediaPlayerDlgMgr.h"
#include "eLTE_Tool.h"

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


// CeLTE_SDK_DemoDlg dialog




CeLTE_SDK_DemoDlg::CeLTE_SDK_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_SDK_DemoDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPasswd(_T(""))
	, m_strServerIP(_T(""))
	, m_strLocalIP(_T(""))
	, m_strMRSIP(_T(""))
	, m_strSipPort(_T(""))
	, m_strLogPath(_T(""))
	, m_iLogLevel(0)
	, m_bMax(FALSE)
	, m_bIsLogin(FALSE)
	, m_bIsInit(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strMRSIP = _T("");
}

void CeLTE_SDK_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Text(pDX, IDC_EDIT_LOGPATH, m_strLogPath);
	DDX_CBIndex(pDX, IDC_COMBO_LOGLEVEL, m_iLogLevel);
	DDX_Text(pDX, IDC_EDIT_MRS_IP, m_strMRSIP);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_comboLocalIP);
}

BEGIN_MESSAGE_MAP(CeLTE_SDK_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_SDK_DemoDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CeLTE_SDK_DemoDlg message handlers

BOOL CeLTE_SDK_DemoDlg::OnInitDialog()
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

	// 坐标设置
	/*GetWindowRect(&m_RectMax);
	m_RectMin = m_RectMax;
	CRect rt;
	GetDlgItem(IDC_STATIC_LOGPARAM)->GetWindowRect(&rt);
	m_RectMin.right = rt.left;
	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);	*/

	// 初始化登录信息
	if (!ReadIniFile())
	{
		m_strName = _T("4114");
		//m_strPasswd = _T("4114");
		m_strServerIP = _T("172.22.9.105");
		m_strLocalIP = _T("172.24.0.190");
		m_strMRSIP = _T("10.170.102.234");
		m_strSipPort = _T("5060");
	}

	if (!GetLocalIP())
	{
		MessageBox(_T("Get Local IP failed！"));
	}

	// 初始日志信息
	m_strLogPath = _T(".\\log\\");
	m_iLogLevel = 0;
	CComboBox* pcmbLogLevel = (CComboBox*)GetDlgItem(IDC_COMBO_LOGLEVEL);
	pcmbLogLevel->InsertString(0, _T("Error"));
	pcmbLogLevel->InsertString(0, _T("Warn"));
	pcmbLogLevel->InsertString(0, _T("Info"));
	pcmbLogLevel->InsertString(0, _T("Debug"));
	UpdateData(FALSE);

	// 创建调度台主窗口
	if (NULL == m_DConsoleDlg.m_hWnd)
	{
		m_DConsoleDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());
		m_DConsoleDlg.SetELTE_SDK_DemoDlg(this);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_SDK_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_SDK_DemoDlg::OnPaint()
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
HCURSOR CeLTE_SDK_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CeLTE_SDK_DemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	//用户退出
	{
		if (m_bIsLogin)
		{
			ELTE_INT32 iRet = ELTE_SDK_Logout(eLTE_Tool::UnicodeToANSI(m_strName).c_str());
			if (0 != iRet)
			{
				MessageBox(_T("ELTE_SDK_Logout failed."));
			}
			//m_bIsLogin = FALSE;
		}
	}
	ELTE_SDK_SetEventCallBack(NULL, NULL);
	if (m_bIsInit)
	{
		ELTE_INT32 iRet = ELTE_SDK_Cleanup();
		if (0 != iRet)
		{
			MessageBox(_T("ELTE_SDK_Cleanup failed."));
		}
		m_bIsInit = FALSE;
	}

	if(0 < m_strKickOff.GetLength())
	{
		if(IDOK == m_DConsoleDlg.MessageBox(m_strKickOff))
		{
			if (m_DConsoleDlg.m_hWnd)
			{
				m_DConsoleDlg.SetELTE_SDK_DemoDlg(NULL);
				m_DConsoleDlg.PostMessage(WM_CLOSE);
				m_DConsoleDlg.DestroyWindow();
			}
		}
	}
	CDialogEx::OnClose();
}


CeLTE_SDK_DemoDlg& CeLTE_SDK_DemoDlg::Instance()
{
	static CeLTE_SDK_DemoDlg s_dlg;
	return s_dlg;
}


ELTE_VOID __SDK_CALL CeLTE_SDK_DemoDlg::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	if (!Instance().m_bIsInit)
		return;

	char* pEventXml = (char*)pEventBuf;
	CString xmlStr(pEventXml);
	CString strRecvVideoPlayCallerId;

	if (ELTE_Event_NotifyProvisionAllResync == iEventType)
	{
		//开始初始化ProvMgr
		Instance().m_DConsoleDlg.PostMessage(WM_INIT);

		CString strEventMsg = Instance().GetTimeString();
		strEventMsg.Append(_T("notifyProvisionAllResync\r\n"));
		Instance().m_DConsoleDlg.m_AllResync.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyResourceStatus == iEventType)
	{
		CString strResId = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ResourceID"));
		int iType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusType"));
		int iValue = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusValue"));

		// 回调事件消息显示
		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Value:%d ResId:%s 【"), iType, iValue, strResId);
		strEventMsg.Insert(0, Instance().GetTimeString());

		// 登录登出
		if (_ttoi(strResId) == _ttoi(Instance().m_strName) && (RESREGSTATUS_PROXY == iType || RESREGSTATUS_DIRECT == iType))
		{
			// 登出成功
			if (STATUS_REGOK == iValue)
			{
				if (!Instance().m_bIsLogin)
				{
					Instance().MessageBox(_T("登录成功."));
					Instance().m_DConsoleDlg.ShowWindow(SW_SHOW);
					Instance().ShowWindow(SW_HIDE);
					Instance().m_bIsLogin = TRUE;
					strEventMsg.Append(_T("登录成功."));
				}
				else
				{
					strEventMsg.Append(_T("登录成功(心跳检测)."));
				}
			}
			// 资源注册失败
			else if (STATUS_REGFAIL == iValue)
			{
				if(!Instance().m_bIsLogin)
				{
					//ELTE_SDK_Cleanup();
					//Instance().m_bIsInit = FALSE;
					Instance().MessageBox(_T("资源注册失败."));
					strEventMsg.Append(_T("资源注册失败."));
				}
			}
			// 资源未授权
			else if (STATUS_FORBIDEN == iValue)
			{
				Instance().MessageBox(_T("资源未授权."));
				strEventMsg.Append(_T("资源未授权."));
			}
			// 账号错误(安全红线整合STATUS_PASSWORD_WRONG)
			else if (STATUS_NOT_FOUND == iValue)
			{
				Instance().MessageBox(_T("账号或者密码错误."));
				strEventMsg.Append(_T("账号或者密码错误."));
			}
			// 资源暂时不可用
			else if (STATUS_TEMP_UNAVAILABLE == iValue)
			{
				Instance().MessageBox(_T("资源暂时不可用."));
				strEventMsg.Append(_T("资源暂时不可用."));
			}
			// 资源冲突
			else if (STATUS_RES_CONFILCT == iValue)
			{
				Instance().MessageBox(_T("资源冲突."));
				strEventMsg.Append(_T("资源冲突."));
			}
			// 4017 STATUS_UNAUTHORIZED 未鉴权（登录中间状态）
			else if (STATUS_UNAUTHORIZED == iValue)
			{
				if (!Instance().m_bIsLogin)
				{
					strEventMsg.Append(_T("未鉴权（登录中间状态）."));
				}
				else
				{
					strEventMsg.Append(_T("未鉴权（登录中间状态）(心跳检测)."));
				}
			}
			// License限制
			else if (STATUS_LICENSE_LIMIT == iValue)
			{
				Instance().MessageBox(_T("License限制."));
				strEventMsg.Append(_T("License限制."));
			}

			// 登录失败则再次登录
			if (STATUS_REGOK != iValue && STATUS_UNAUTHORIZED != iValue)
			{
				//Instance().ShowWindow(SW_HIDE);
				//Instance().PostMessage(WM_CLOSE);
				if(Instance().m_bIsLogin)
				{
					Instance().ShowWindow(SW_HIDE);
					Instance().PostMessage(WM_CLOSE);
					Instance().m_bIsLogin = FALSE;
				}
				else
				{
					Instance().ShowWindow(SW_SHOW);
					Instance().GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
				}
				
			}
		}

		if(GRPCALLSTATUS == iType)
		{
			// 组活动状态
			if (STATUS_GROUP_ACTIVATED == iValue)
			{
				//MessageBox(_T("组活动状态."));
				strEventMsg.Append(_T("组呼活动状态."));
			}
			//  组非活动状态
			else if (STATUS_GROUP_DEACTIVATED == iValue)
			{
				//MessageBox(_T("组非活动状态."));
				strEventMsg.Append(_T("组呼非活动状态."));
			}
		}

		//群组订阅
		if (RESASSIGNSTATUS == iType)
		{
			if (STATUS_ASSIGNED == iValue)//订阅成功
			{
				CString strMsg;
				strMsg.Format(_T("群组[%s]订阅成功."), strResId);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DEASSIGNED == iValue)//去订阅成功
			{
				CString strMsg;
				strMsg.Format(_T("群组[%s]去订阅成功."), strResId);

				//获取选中项信息
				GroupInfo* pInfo = NULL;
				//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
				CString szItemroot(strResId);
				szItemroot.Append(_T(" [临时组]"));
				HTREEITEM htiRes = Instance().m_DConsoleDlg.FindStrGroupInfo(szItemroot, &pInfo);
				
				if ( (NULL != pInfo)&& (NULL != htiRes))
				{
					if ("-1" == pInfo->GroupCategory)
					{
						//删除临时组
						Instance().m_DConsoleDlg.RemoveGroup(StrToInt(strResId));
					}					
				}

				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		//派接组
		if (GRPPATCHSTATUS == iType)
		{
			int iCause = GET_XML_ELEM_VALUE_INT(xmlStr, _T("Cause"));
			int iGroupID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ResourceID"));
			int iMemberID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("MemberID"));
			if(STATUS_GRPPATCH_CREATEOK == iValue)//创建成功
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]创建成功."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				Instance().m_DConsoleDlg.AddPatchGroup(eLTE_Tool::Int2String(iGroupID).c_str());
			}
			else if(STATUS_GRPPATCH_CREATEFAIL == iValue)//创建失败
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]创建失败.失败原因=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_CANCELOK == iValue)//删除成功
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]删除成功."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				Instance().m_DConsoleDlg.RemovePatchGroup(iGroupID);
			}
			else if(STATUS_GRPPATCH_CANCELFAIL == iValue)//删除失败
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]删除失败.失败原因=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_ADDOK == iValue)//添加成员成功
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]添加成员成功."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				Instance().m_DConsoleDlg.AddPatchGroupMember(eLTE_Tool::Int2String(iGroupID).c_str(), eLTE_Tool::Int2String(iMemberID).c_str());
			}
			else if(STATUS_GRPPATCH_ADDFAIL == iValue)//添加成员失败
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]添加成员失败.失败原因=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_DELOK == iValue)//删除成员成功
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]删除成员成功."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				Instance().m_DConsoleDlg.DelPatchGroupMember(eLTE_Tool::Int2String(iGroupID).c_str(), eLTE_Tool::Int2String(iMemberID).c_str());
			}
			else if(STATUS_GRPPATCH_DELFAIL == iValue)//删除成员失败
			{
				CString strMsg;
				strMsg.Format(_T("派接组[%d]删除成员失败.失败原因=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		if (USERDGNASTATUS == iType)//动态重组
		{
			int iCause = GET_XML_ELEM_VALUE_INT(xmlStr, _T("Cause"));
			int iGroupID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("AttachingGroup"));
			if (0 == iGroupID)
			{
				CString strtemp = Instance().m_DConsoleDlg.m_ModifyDGNAGroupId;
				iGroupID = eLTE_Tool::String2Int(eLTE_Tool::UnicodeToANSI(strtemp));
			}
			if (STATUS_DGNAFAIL == iValue)//创建失败
			{
				CString strMsg;
				strMsg.Format(_T("动态群组[%d]创建失败.失败原因=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DGNAOK == iValue)//创建成功
			{
				//Instance().MessageBox(Instance().m_DConsoleDlg.m_ModifyDGNAGroupId);
				
				CString strMsg;
				strMsg.Format(_T("动态群组[%d]成功."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				Instance().m_DConsoleDlg.RemoveGroup(iGroupID);
				Instance().m_DConsoleDlg.AddGroup(eLTE_Tool::Int2String(iGroupID).c_str());
			}
			else if (STATUS_DGNA_UEFAIL == iValue)//动态组某些UE重组失败
			{
				CString strMsg;
				//strMsg.Format(_T("动态重组[%d]UE[%s]重组失败.失败原因=%d"), iGroupID, strResId, iCause);
				if (iCause == -20044)
				{
					strMsg.Append(_T("【"));
					strMsg.Append(strResId);
					strMsg.Append(_T("未注册】"));
				}
				else if (iCause == -20042)
				{
					strMsg.Append(_T("【"));
					strMsg.Append(strResId);
					strMsg.Append(_T("超时】"));
				}
				else
				{
					strMsg.Format(_T("动态群组[%d]UE[%s]重组失败.失败原因=%d,请稍后再试"), iGroupID, strResId, iCause);
				}
				//fail,but need refresh too. 
				Instance().m_DConsoleDlg.RemoveGroup(iGroupID);
				Sleep(2000);
				Instance().m_DConsoleDlg.AddGroup(eLTE_Tool::Int2String(iGroupID).c_str());
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		// 回调事件消息显示
		strEventMsg.Append(_T("】\r\n"));
		Instance().m_DConsoleDlg.m_ResStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyUserStatus == iEventType)
	{
		CString strUserID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("UserID"));
		int iStatusType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusType"));
		int iStatusValue = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusValue"));

		Instance().m_DConsoleDlg.UpdateUserStatus(strUserID, iStatusValue);

		// 显示视频回传当前状态
		CString strEventMsg;
		strEventMsg.Format(_T("UserID:%s Type:%d Value:%d 【"), strUserID, iStatusType, iStatusValue);
		if (iStatusValue == 4011)
		{
			strEventMsg.Append(_T("在线"));
		}
		else if (iStatusValue == 4012)
		{
			strEventMsg.Append(_T("不在线"));
		}
		else if (iStatusValue == 4020)
		{
			strEventMsg.Append(_T("正在发起呼叫"));
		}
		else if (iStatusValue == 4021)
		{
			strEventMsg.Append(_T("正在振铃"));
		}
		else if (iStatusValue == 4022)
		{
			strEventMsg.Append(_T("使用中"));
		}
		else if (iStatusValue == 4023)
		{
			strEventMsg.Append(_T("空闲"));
		}
		strEventMsg.Append(_T("】\r\n"));
		strEventMsg.Insert(0,Instance().GetTimeString());
		Instance().m_DConsoleDlg.m_UserStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyProvisionChange == iEventType)
	{
		CString strChangeType = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ChangeType"));
		CString strChangeMO = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ChangeMO"));

		CString strEventMsg;
		strEventMsg.Format(_T("ChangeType:%s ChangeMO:%s Content:%s\r\n"), strChangeType, strChangeMO, eLTE_Tool::UTF8ToUnicode(pEventXml));
		strEventMsg.Insert(0, Instance().GetTimeString());
		Instance().m_DConsoleDlg.m_ProvChange.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyModuleStatus == iEventType)
	{
		CString strResourceID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ResouceID"));
		int iModuleType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ModuleType"));
		int iModuleStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ModuleStatus"));
		int iCallBackMsgType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("CallBackMsgType"));
		CString strModulePara = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ModulePara"));

		CString strEventMsg;
		strEventMsg.Format(_T("ResouceID:%s ModuleType:%d ModuleStatus:%d CallBackMsgType:%d ModulePara:%s"), strResourceID, iModuleType, iModuleStatus, iCallBackMsgType, strModulePara);
		strEventMsg.Insert(0,Instance().GetTimeString());
		if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // 被踢下线
		{
			CString strMsg;
			strMsg.Format(_T("用户【%s】已经在别处登录\r\n%s"), Instance().m_strName, strModulePara);
			strEventMsg.Append(_T(" 【被踢下线】"));
		
			//Instance().MessageBox(strMsg);
			Instance().m_strKickOff = strMsg;
			Instance().PostMessage(WM_CLOSE);				
		}
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_ModuleStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyP2pvideocallStatus == iEventType)
	{
		int iCallStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("CallStatus"));
		CString strResID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));

		// 显示视频回传当前状态
		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Camera:%s 【"), iCallStatus, strResID);
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
			//视频回传接听（或者镜头切换中间过程）				
			UINT uiLocalVideoPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("LocalVideoPort"));
			UINT uiLocalAudioPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("LocalAudioPort"));
			UINT uiRemoteVideoPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("RemoteVideoPort"));
			UINT uiRemoteAudioPort = GET_XML_ELEM_VALUE_UINT(xmlStr, _T("RemoteAudioPort"));
			
			int iVideoFormat = GET_XML_ELEM_VALUE_INT(xmlStr, _T("VideoFormatType"));
			CString Callee = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));
			switch(iVideoFormat)
			{
			case 0:
				{
					Instance().m_DConsoleDlg.SetVideoParam(_T("CIF"), Callee);
				}
				break;
			case 1:
				{
					Instance().m_DConsoleDlg.SetVideoParam(_T("QCIF"), Callee);
				}
				break;
			case 2:
				{
					Instance().m_DConsoleDlg.SetVideoParam(_T("D1"), Callee);
				}
				break;
			case 3:
				{
					Instance().m_DConsoleDlg.SetVideoParam(_T("720P"), Callee);
				}
				break;
			case 4:
				{
					Instance().m_DConsoleDlg.SetVideoParam(_T("1080P"), Callee);
				}
				break;
			default:
				Instance().m_DConsoleDlg.SetVideoParam(_T("CIF"), Callee);
			}

			CString strMsg;
			strMsg.Format(_T("LocalVideoPort: %d, LocalAudioPort: %d, RemoteVideoPort: %d, RemoteAudioPort: %d"), uiLocalVideoPort, uiLocalAudioPort, uiRemoteVideoPort, uiRemoteAudioPort);
			strEventMsg.Append(strMsg);
			Instance().m_DConsoleDlg.SetPlayResID(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			Instance().m_DConsoleDlg.PostMessage(WM_START_REAL_PLAY);
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
		else if (3007 == iCallStatus)
		{
			strEventMsg.Append(_T("Released."));
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
		else if(3011 == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
			strRecvVideoPlayCallerId = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Uri"));
			Instance().m_DConsoleDlg.SetCommingVideo(strRecvVideoPlayCallerId);
			Instance().m_DConsoleDlg.PostMessage(WM_INCOMMING_VIDEO);
		}
		else if (3010 == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
		}
		else if (3040 == iCallStatus)
		{
			strEventMsg.Append(xmlStr);
		}
		if (3021 == iCallStatus || 3013 == iCallStatus || 3008 == iCallStatus || 3009 == iCallStatus)
		{
			//对端主动挂断
			CMediaPlayerDlg* pDlg = CMediaPlayerDlgMgr::Instance().GetMediaPlayerDlg(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			//目前为了区分调度台主动请求视频回传，还是接收视频回传
			if(NULL == pDlg)
			{
				if(3008 == iCallStatus)
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

		// 回调事件消息显示
		strEventMsg.Append(_T("】\r\n"));
		strEventMsg.Insert(0,Instance().GetTimeString());
		Instance().m_DConsoleDlg.m_VideoStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyP2pcallStatus == iEventType)
	{
		int iType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("P2pcallStatus"));
		CString strCaller = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Caller"));
		CString strCallee = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));

		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Caller:%s Callee:%s"), iType, strCaller, strCallee);
		strEventMsg.Insert(0,Instance().GetTimeString());
		if (P2P_IND_STATUS_EMERGENCY == iType)
		{
			// 紧急呼叫
			strEventMsg.Append(_T(" 【紧急呼叫】\r\n"));
		}
		else if (P2P_IND_STATUS_PROCEEDING == iType)
		{
			strEventMsg.Append(_T(" 【处理中】\r\n"));
		}
		else if (P2P_IND_STATUS_REMOTE_NOTCONNECTED == iType)
		{
			strEventMsg.Append(_T(" 【远端未连接】\r\n"));
		}
		else if (P2P_IND_STATUS_RINGING == iType)
		{
			strEventMsg.Append(_T(" 【响铃中】\r\n"));
		}
		else if (P2P_IND_STATUS_ANSWERED == iType)
		{
			strEventMsg.Append(_T(" 【对方已应答】\r\n"));
		}
		else if (P2P_IND_STATUS_ACKED == iType)
		{
			strEventMsg.Append(_T(" 【已接通】\r\n"));
			Instance().m_DConsoleDlg.SetCaller(strCaller, 1);
		}
		else if (P2P_IND_STATUS_PREMPTED == iType)
		{
			strEventMsg.Append(_T(" 【被抢话】\r\n"));
		}
		else if (P2P_IND_STATUS_HANGUPED == iType)
		{
			strEventMsg.Append(_T(" 【已挂断】\r\n"));
		}
		else if (P2P_IND_STATUS_PICKEDUP == iType)
		{
			strEventMsg.Append(_T(" 【本端已摘机】\r\n"));
		}
		else if (P2P_IND_STATUS_RELEASED_BUSY == iType)
		{
			strEventMsg.Append(_T(" 【对方忙】\r\n"));
		}
		else if (P2P_IND_STATUS_HANGUPED_ACTIVE == iType)
		{
			strEventMsg.Append(_T(" 【结束通话】\r\n"));
		}
		else if (P2P_IND_STATUS_RECEIVED == iType)
		{
			strEventMsg.Append(_T(" 【有来电】\r\n"));
			int iDiscreetListenee = GET_XML_ELEM_VALUE_INT(xmlStr, _T("DiscreetListenee"));
			int iFromString = GET_XML_ELEM_VALUE_INT(xmlStr, _T("FromString"));
			int iToString = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ToString"));
			if(iDiscreetListenee > 0 || iFromString == 0 || iToString == 0)
			{
				
			}
			else
			{
				Instance().m_DConsoleDlg.SetCaller(strCaller, 0);
				Instance().m_DConsoleDlg.PostMessage(WM_INCOMMING_CALL);
			}
		}
		else if (P2P_IND_STATUS_REMOTE_NOANSWER == iType)
		{
			strEventMsg.Append(_T(" 【对方无应答】\r\n"));
		}
		else
		{
			strEventMsg.Append(_T("\r\n"));
		}
		Instance().m_DConsoleDlg.m_CallStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyGroupStatus == iEventType)
	{
		int iType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("GroupCallStatus"));
		CString strGroupID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("GroupID"));
		CString strSpeaker = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Speaker"));

		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d GroupID:%s Speaker:%s"), iType, strGroupID, strSpeaker);
		strEventMsg.Insert(0,Instance().GetTimeString());
		if (PTT_IND_STATUS_EMERGENCY_BEGIN == iType)
		{
			// 紧急呼叫
			strEventMsg.Append(_T(" 【紧急呼叫】\r\n"));
		}
		else if (PTT_IND_STATUS_SNATCH == iType)
		{
			strEventMsg.Append(_T(" 【组呼建立成功】\r\n"));
		}
		else if (PTT_IND_STATUS_TX_BEGIN == iType)
		{
			strEventMsg.Append(_T(" 【话权变更】\r\n"));
		}
		else if (PTT_IND_STATUS_TX_IDLE == iType)
		{
			strEventMsg.Append(_T(" 【话权空闲】\r\n"));
		}
		else if (PTT_IND_STATUS_ACCEPT == iType)
		{
			strEventMsg.Append(_T(" 【释放话权接受】\r\n"));
		}
		else if (PTT_IND_STATUS_REJECT == iType)
		{
			strEventMsg.Append(_T(" 【抢权拒绝】\r\n"));
		}
		else if (PTT_IND_STATUS_HANGUP_OK == iType)
		{
			strEventMsg.Append(_T(" 【退出组呼成功】\r\n"));
		}
		else if (PTT_IND_STATUS_CLOSE_OK == iType)
		{
			strEventMsg.Append(_T(" 【组呼结束】\r\n"));
		}
		else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
		{
			strEventMsg.Append(_T(" 【不被许可】\r\n"));
		}
		else if (PTT_IND_STATUS_CANCEL_OK == iType)
		{
			// 紧急呼叫
			strEventMsg.Append(_T(" 【组呼建立失败】\r\n"));
		}
		else
		{
			strEventMsg.Append(_T("\r\n"));
		}
		Instance().m_DConsoleDlg.m_GroupStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyGISStatus == iEventType)
	{
		CString strResourceID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ResourceID"));
		CString strStatusList = GET_XML_ELEM_VALUE_STR(xmlStr, _T("AckStatusList"));

		CString strEventMsg;
		strEventMsg.Format(_T("ResouceID:%s AckStatusList:%s"), strResourceID, strStatusList);
		strEventMsg.Insert(0,Instance().GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_GISStatus.Append(strEventMsg);
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
			strTriggerCode = _T("周期上报");
		}
		else if(1 == iTriggerCode)
		{
			strTriggerCode = _T("特定事件, 紧急呼叫");
		}

		int iReportStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ReportStatus"));
		CString strReportStatus;
		if(0 == iReportStatus)
		{
			strReportStatus = _T("GPS正常开启");
		}
		else if(1 == iReportStatus)
		{
			strReportStatus = _T("GPS搜星失败");
		}
		else if(2 == iReportStatus)
		{
			strReportStatus = _T("其他异常");
		}

		CString strEventMsg;
		strEventMsg.Format(_T("ResouceID:%s Time:%s Altitude:%s Longtitude:%s Latitude:%s TriggerCode:%s ReportStatus:%s"), strResourceID, strTime, strAltitude, strLongtitude, strLatitude, strTriggerCode, strReportStatus);
		strEventMsg.Insert(0,Instance().GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_GISReport.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifySDSReport == iEventType)
	{
		CString strSdsType = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsType"));
		CString strSdsContent = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pEventXml), _T("SdsContent"));
		CString strSdsFrom = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsFrom"));
		CString strSdsMmsFileNameList = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pEventXml), _T("SdsMmsFileNameList"));

		CString strEventMsg = eLTE_Tool::UTF8ToUnicode(pEventXml);
		//strEventMsg.Format(_T("SdsType:%s SdsContent:%s SdsFrom:%s SdsMmsFileNameList:%s"), strSdsType, strSdsContent, strSdsFrom, strSdsMmsFileNameList);
		strEventMsg.Insert(0,Instance().GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_SDSReport.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifySDSSendStatus == iEventType)
	{
		CString strSdsTo = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsReceiver"));
		CString strSdsMsgId = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsSubject"));
		CString strSdsRetCode = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsRetCode"));

		CString strEventMsg;
		strEventMsg.Format(_T("strSdsTo:%s strSdsMsgId:%s strSdsRetCode:%s"), strSdsTo, strSdsMsgId, strSdsRetCode);
		strEventMsg.Insert(0,Instance().GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_SDSStatus.Append(strEventMsg);
	}
}


void CeLTE_SDK_DemoDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*if (m_bMax)
	{
	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);
	}
	else
	{
	SetWindowPos(NULL,0,0,m_RectMax.Width(),m_RectMax.Height(),SWP_NOMOVE);
	}
	m_bMax = !m_bMax;*/

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CeLTE_SDK_DemoDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
	// 日志参数设置
	MoreSetting();

	// 初始化SDK
	if (!m_bIsInit)
	{
		ELTE_INT32 iRet = ELTE_SDK_Init(FALSE);
		CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_Init"));
		m_bIsInit = TRUE;
	}

	// 设置回调函数
	ELTE_INT32 iRet = ELTE_SDK_SetEventCallBack(ELTE_EventCallBack, NULL);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_SetEventCallBack"));

	// 登录
	UpdateData(TRUE);
	iRet = ELTE_SDK_Login(eLTE_Tool::UnicodeToANSI(m_strName).c_str(), eLTE_Tool::UnicodeToANSI(m_strPasswd).c_str(), eLTE_Tool::UnicodeToANSI(m_strServerIP).c_str(), eLTE_Tool::UnicodeToANSI(m_strLocalIP).c_str(), atoi(eLTE_Tool::UnicodeToANSI(m_strSipPort).c_str()));
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_Login"));
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);

	WriteIniFile();
}

BOOL CeLTE_SDK_DemoDlg::ReadIniFile()
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
	//GetPrivateProfileString(_T("LoginInfo"), _T("Passwd"), _T(""), tchValue, LENGTH, szPath);
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

BOOL CeLTE_SDK_DemoDlg::WriteIniFile()
{
	TCHAR pszPath[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), pszPath, MAX_PATH);
	CString szPath(pszPath);
	szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
	szPath.Append(_T("Server.ini"));

	WritePrivateProfileString(_T("LoginInfo"), _T("Name"), m_strName, szPath);
	//WritePrivateProfileString(_T("LoginInfo"), _T("Passwd"), m_strPasswd, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("ServerIP"), m_strServerIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("LocalIP"), m_strLocalIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("MRSIP"), m_strMRSIP, szPath);
	WritePrivateProfileString(_T("LoginInfo"), _T("SipPort"), m_strSipPort, szPath);

	return TRUE;
}

void CeLTE_SDK_DemoDlg::MoreSetting()
{
	/*if (!m_bMax)
		return;*/

	UpdateData(TRUE);
	// 设置日志参数
	ELTE_INT32 iRet = ELTE_SDK_SetLogLevel(m_iLogLevel);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_SetLogLevel"));

	iRet = ELTE_SDK_SetLogPath(eLTE_Tool::UnicodeToANSI(m_strLogPath).c_str());
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_SetLogPath"));

	// 获取版本
	ELTE_CHAR* pVersion = NULL;
	iRet = ELTE_SDK_GetVersion(&pVersion);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetVersion"));

	CString strText;
	strText.Format(_T("DConsole   [%s]"), eLTE_Tool::ANSIToUnicode(pVersion));
	m_DConsoleDlg.SetWindowText(strText);

	ELTE_SDK_ReleaseBuffer(pVersion);
	pVersion = NULL;
}

CString CeLTE_SDK_DemoDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}


BOOL CeLTE_SDK_DemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return FALSE;
	}
	else
	{
		return CDialogEx::PreTranslateMessage(pMsg);
	}
}

BOOL CeLTE_SDK_DemoDlg::GetLocalIP()
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
		m_comboLocalIP.AddString(LPCTSTR(CString(inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]))));
		if (host->h_addr_list[i]+host->h_length >= host->h_name)
		{
			break;
		}
	}
	return TRUE;
}