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


// eLTE_PlayerDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "eLTE_PlayerDemoDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "VideoParamDlg.h"
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


// CeLTE_PlayerDemoDlg dialog



int CeLTE_PlayerDemoDlg::m_iBypass = 1;
CeLTE_PlayerDemoDlg::CeLTE_PlayerDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_PlayerDemoDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPasswd(_T(""))
	, m_strServerIP(_T(""))
	, m_strLocalIP(_T(""))
	, m_strMRSIP(_T(""))
	, m_strSipPort(_T(""))
	, m_strLogSavePath(_T(""))
	, m_strBGLogoPath(_T(""))
	, m_bIsLogin(FALSE)
	, m_bMax(FALSE)
	, m_bClose(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strMRSIP = _T("");
}

void CeLTE_PlayerDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ELTE_PLAYERCTRL1, m_eLTE_Player);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Text(pDX, IDC_EDIT_LOGPATH, m_strLogSavePath);
	DDX_Text(pDX, IDC_EDIT_BGLOGO, m_strBGLogoPath);
	DDX_Control(pDX, IDC_COMBO_LOGLV, m_cmbLogLevel);
	DDX_Control(pDX, IDC_COMBO_MEDIA_PASS, m_cmbMediaPass);
	DDX_Text(pDX, IDC_EDIT_MRSIP, m_strMRSIP);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_comboLocalIP);
}

BEGIN_MESSAGE_MAP(CeLTE_PlayerDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_PlayerDemoDlg::OnBnClickedButtonLogin)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CeLTE_PlayerDemoDlg message handlers

BOOL CeLTE_PlayerDemoDlg::OnInitDialog()
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

	// Coordinate setting
	/*GetWindowRect(&m_RectMax);
	m_RectMin = m_RectMax;
	CRect rt;
	GetDlgItem(IDC_STATIC_MORESETTING)->GetWindowRect(&rt);
	m_RectMin.right = rt.left;
	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);	*/

	//// Set working directory
	//TCHAR tchPath[MAX_PATH] = {0};
	//GetModuleFileName(NULL, tchPath, MAX_PATH);
	//CString szPath(tchPath);
	//szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
	//SetCurrentDirectory(szPath);

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
		//m_strPasswd = _T("4101");
		m_strServerIP = _T("10.170.102.234");
		m_strMRSIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}

	// 初始日志信息
	m_strLogSavePath = _T(".\\log");
	m_cmbLogLevel.InsertString(0, _T("Error"));
	m_cmbLogLevel.InsertString(0, _T("Warn"));
	m_cmbLogLevel.InsertString(0, _T("Info"));
	m_cmbLogLevel.InsertString(0, _T("Debug"));
	m_cmbLogLevel.SetCurSel(0);
	UpdateData(FALSE);

	// 创建DConsoleDlg
	m_DcDlg.SetCeLTE_PlayerDemoDlg(this);
	m_DcDlg.SetELtePlayer(&m_eLTE_Player);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeLTE_PlayerDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_PlayerDemoDlg::OnPaint()
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
HCURSOR CeLTE_PlayerDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BEGIN_EVENTSINK_MAP(CeLTE_PlayerDemoDlg, CDialogEx)
	ON_EVENT(CeLTE_PlayerDemoDlg, IDC_ELTE_PLAYERCTRL1, 50, CeLTE_PlayerDemoDlg::ELTE_OCX_EventEltePlayerctrl1, VTS_UI4 VTS_BSTR)
	ON_EVENT_RANGE(CeLTE_PlayerDemoDlg, OCX_ID_BEGIN, OCX_ID_END, 51, CeLTE_PlayerDemoDlg::ELTE_OCX_PlayerEventEltePlayerctrl1, VTS_UI4 VTS_UI4 VTS_BSTR)
END_EVENTSINK_MAP()

void CeLTE_PlayerDemoDlg::ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	// TODO: Add your message handler code here	
	switch (ulEventType)
	{
	case EVENT_NOTIFY_USER_STATUS:	// notifyUserStatus
		{
			CString strUserID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("UserID"));
			int iStatusType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusType"));
			int iStatusValue = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusValue"));

			m_DcDlg.UpdateUserStatus(strUserID, iStatusValue);

			// Return the current state of video display
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
				strEventMsg.Append(_T("under use"));
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
				//Video return answer（Or lens switching intermediate process）				
				UINT uiLocalVideoPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("LocalVideoPort"));
				UINT uiLocalAudioPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("LocalAudioPort"));
				UINT uiRemoteVideoPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("RemoteVideoPort"));
				UINT uiRemoteAudioPort = GET_XML_ELEM_VALUE_UINT(pEventDataXml, _T("RemoteAudioPort"));

				int iVideoFormat = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("VideoFormatType"));
				switch(iVideoFormat)
				{
				case 0:
					{
						m_DcDlg.SetVideoFormat(_T("CIF"));
					}
					break;
				case 1:
					{
						m_DcDlg.SetVideoFormat(_T("QCIF"));
					}
					break;
				case 2:
					{
						m_DcDlg.SetVideoFormat(_T("D1"));
					}
					break;
				case 3:
					{
						m_DcDlg.SetVideoFormat(_T("720P"));
					}
					break;
				case 4:
					{
						m_DcDlg.SetVideoFormat(_T("1080P"));
					}
					break;
				default:
					m_DcDlg.SetVideoFormat(_T("CIF"));
				}

				m_DcDlg.SetRealPlayParam(iResID, uiLocalVideoPort, uiLocalAudioPort, uiRemoteVideoPort, uiRemoteAudioPort);
				if(!m_iBypass)
				{
					m_DcDlg.DoMute(iResID);

				}
			}
			else if (3011 == iCallStatus)
			{
				//Terminal video upload or video distribution

				CString strResID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Uri"));
				CString strMuteType = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SoundMute"));

				m_DcDlg.SetVideoCaller(strResID,strMuteType, 0);
				m_DcDlg.PostMessage(WM_VIDEORECEIVED, 0, 0);
			}
			else if (3009 == iCallStatus)
			{
				//Video back to hang up（Or lens switching intermediate process）
				m_DcDlg.RealPlayHangup(iResID);
			}
			else if (3021 == iCallStatus || 3013 == iCallStatus || 3008 == iCallStatus)
			{
				//connect fail、busy、refused
				if(3008 == iCallStatus)
				{
					//if the other side refused the backhaul or dispatch video, We should get video resid from Uri
					iResID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Uri"));
				}
				m_DcDlg.RealPlayNotConnected(iResID, iCallStatus);
			}

			// Return the current state of video display
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
			// Callback event message display
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

			// Callback event message display
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
				// login failed
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
				// Resource not authorized
				else if (STATUS_FORBIDEN == iValue)
				{
					MessageBox(_T("Resource not authorized."));
					strEventMsg.Append(_T("Resource not authorized."));
				}
				// Account error
				else if (STATUS_NOT_FOUND == iValue)
				{
					MessageBox(_T("User or password error."));
					strEventMsg.Append(_T("User or password error."));
				}
				// resource not available
				else if (STATUS_TEMP_UNAVAILABLE == iValue)
				{
					MessageBox(_T("resource not available."));
					strEventMsg.Append(_T("resource not available."));
				}
				// resource conflict
				else if (STATUS_RES_CONFILCT == iValue)
				{
					MessageBox(_T("resource conflict."));
					strEventMsg.Append(_T("resource conflict."));
				}
				// 4017 STATUS_UNAUTHORIZED not authorized（Landing intermediate state）
				else if (STATUS_UNAUTHORIZED == iValue)
				{
					if (!m_bIsLogin)
					{
						strEventMsg.Append(_T("not authorized（Landing intermediate state）."));
					}
					else
					{
						strEventMsg.Append(_T("not authorized（Landing intermediate state）(Heartbeat check)."));
					}
				}
				// License limit
				else if (STATUS_LICENSE_LIMIT == iValue)
				{
					MessageBox(_T("License limit."));
					strEventMsg.Append(_T("License limit."));
				}
				// Safety red line
				//// password error
				//else if (STATUS_PASSWORD_WRONG == iValue)
				//{
				//	MessageBox(_T("password error."));
				//	strEventMsg.Append(_T("password error."));
				//}

				// login failed then exit MFC program
				if (STATUS_REGOK != iValue && STATUS_UNAUTHORIZED != iValue)
				{
					ShowWindow(SW_HIDE);
					PostMessage(WM_CLOSE);
				}
			}

			if(GRPCALLSTATUS == iType)
			{
				// Group actived
				if (STATUS_GROUP_ACTIVATED == iValue)
				{
					//MessageBox(_T("Group actived."));
					strEventMsg.Append(_T("Group actived"));
				}
				//  Group deactived
				else if (STATUS_GROUP_DEACTIVATED == iValue)
				{
					//MessageBox(_T("Group deactived."));
					strEventMsg.Append(_T("Group deactived."));
				}
			}

			if (RESASSIGNSTATUS == iType)//Group subscription
			{
				if (STATUS_ASSIGNED == iValue)//Subscription success
				{
					CString strMsg;
					strMsg.Format(_T("群组[%s]订阅成功."), strResId);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DEASSIGNED == iValue)//unsubscribe success
				{
					CString strMsg;
					strMsg.Format(_T("群组[%s]去订阅成功."), strResId);					
					//delete temporary group 
					//m_DcDlg.RemoveGroup(StrToInt(strResId));
					//Get selected item information
					GroupInfo* pInfo = NULL;
					//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
					CString szItemroot(strResId);
					szItemroot.Append(_T(" [临时组]"));
					HTREEITEM htiRes = m_DcDlg.FindStrGroupInfo(szItemroot, &pInfo);

					if ( (NULL != pInfo)&& (NULL != htiRes))
					{
						if ("-1" == pInfo->GroupCategory)
						{
							//delete temporary group
							m_DcDlg.RemoveGroup(StrToInt(strResId));
						}					
					}
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			if (USERDGNASTATUS == iType)//dynamic group
			{
				int iCause = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Cause"));
				int iGroup = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("AttachingGroup"));
				if (0 == iGroup)
				{
					CString strtemp = m_DcDlg.m_ModifyDGNAGroupId;
					iGroup = StrToInt(strtemp);
				}
				if (STATUS_DGNAFAIL == iValue)//create failed
				{
					CString strMsg;
					strMsg.Format(_T("动态群组[%d]create failed.失败原因=%d"), iGroup, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DGNAOK == iValue)//create success
				{
					CString strMsg;
					strMsg.Format(_T("动态群组[%d]成功."), iGroup);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.RemoveGroup(iGroup);
					m_DcDlg.AddGroup(iGroup);
				}
				else if (STATUS_DGNA_UEFAIL == iValue)//dynamic group UE reorganization failed
				{
					CString strMsg;
					//strMsg.Format(_T("动态重组[%d]UE[%s]重组失败.失败原因=%d"), iGroup, strResId, iCause);
					if (iCause == -20044)
					{
						strMsg.Append(_T("【"));
						strMsg.Append(strResId);
						strMsg.Append(_T("未注册】"));
						m_DcDlg.AddGroup(iGroup);
					}
					else if (iCause == -20042)
					{
						strMsg.Append(_T("【"));
						strMsg.Append(strResId);
						strMsg.Append(_T("超时】"));
						m_DcDlg.AddGroup(iGroup);
					}
					else
					{
						strMsg.Format(_T("动态群组[%d]UE[%s]重组失败.失败原因=%d,请稍后再试"), iGroup, strResId, iCause);
					}
					m_DcDlg.RemoveGroup(iGroup);
					Sleep(2000);
					m_DcDlg.AddGroup(iGroup);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			//派接组
			if (GRPPATCHSTATUS == iType)
			{
				int iCause = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("Cause"));
				int iGroupID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ResourceID"));
				int iMemberID = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("MemberID"));
				if(STATUS_GRPPATCH_CREATEOK == iValue)//create success
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]create success."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.AddPatchGroup(iGroupID);
				}
				else if(STATUS_GRPPATCH_CREATEFAIL == iValue)//create failed
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]create failed.失败原因=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_CANCELOK == iValue)//delete success
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]delete success."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.RemovePatchGroup(iGroupID);
				}
				else if(STATUS_GRPPATCH_CANCELFAIL == iValue)//delete failed
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]删除失败.失败原因=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_ADDOK == iValue)//add member success
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]添加成员成功."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.AddPatchGroupMember(iGroupID, iMemberID);
				}
				else if(STATUS_GRPPATCH_ADDFAIL == iValue)//add member failed
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]添加成员失败.失败原因=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				if(STATUS_GRPPATCH_DELOK == iValue)//delete member success
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]删除成员成功."), iGroupID);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
					m_DcDlg.DelPatchGroupMember(iGroupID, iMemberID);
				}
				else if(STATUS_GRPPATCH_DELFAIL == iValue)//delete member failed
				{
					CString strMsg;
					strMsg.Format(_T("派接组[%d]删除成员失败.失败原因=%d"), iGroupID, iCause);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			// Callback event message display
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
			//start initialize ProvMgr
			m_DcDlg.InitDlg();

			CString strEventMsg = GetTimeString();
			strEventMsg.Append(_T("notifyProvisionAllResync\r\n"));
			m_DcDlg.m_strEvent4.Append(strEventMsg);

			// initialize MRS
			if(!m_bClose)
			{
				CString strResult = m_eLTE_Player.ELTE_OCX_ProvisionManagerInitMRS(m_strMRSIP);
				int iRet = 0;
				CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_ProvisionManagerInitMRS"));
			}
		}
		break;
	case EVENT_NOTIFY_P2P_CALL_STATUS: // notifyP2pcallStatus
		{
			int iType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("P2pcallStatus"));
			CString strCaller = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Caller"));
			CString strCallee = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("Callee"));

			CString strEventMsg;
			strEventMsg.Format(_T("Type:%d Caller:%s Callee:%s"), iType, strCaller, strCallee);
			strEventMsg.Insert(0,GetTimeString());
			if (P2P_IND_STATUS_EMERGENCY == iType)
			{
				// emergency call
				strEventMsg.Append(_T(" 【emergency call】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PROCEEDING == iType)
			{
				strEventMsg.Append(_T(" 【处理中】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_REMOTE_NOTCONNECTED == iType)
			{
				strEventMsg.Append(_T(" 【远端未连接】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RINGING == iType)
			{
				strEventMsg.Append(_T(" 【响铃中】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_ANSWERED == iType)
			{
				strEventMsg.Append(_T(" 【对方已应答】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_ACKED == iType)
			{
				strEventMsg.Append(_T(" 【已接通】\r\n"));
				m_DcDlg.SetCaller(strCaller, 1);
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PREMPTED == iType)
			{
				strEventMsg.Append(_T(" 【被抢话】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_HANGUPED == iType)
			{
				strEventMsg.Append(_T(" 【对方已挂断】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PICKEDUP == iType)
			{
				strEventMsg.Append(_T(" 【本端已摘机】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RELEASED_BUSY == iType)
			{
				strEventMsg.Append(_T(" 【对方忙】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_HANGUPED_ACTIVE == iType)
			{
				strEventMsg.Append(_T(" 【结束通话】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RECEIVED == iType)
			{
				strEventMsg.Append(_T(" 【有来电】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);

				int iDiscreetListenee = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("DiscreetListenee"));
				int iFromString = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("FromString"));
				int iToString = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ToString"));
				if(iDiscreetListenee > 0 || iFromString == 0 || iToString == 0)
				{

				}
				else
				{
					m_DcDlg.SetCaller(strCaller, 0);
					m_DcDlg.PostMessage(WM_RECEIVED, 0, 0);
				}
			}
			else if (P2P_IND_STATUS_REMOTE_NOANSWER == iType)
			{
				strEventMsg.Append(_T(" 【对方无应答】\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else
			{
				strEventMsg.Append(_T("\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
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
				// emergency call
				strEventMsg.Append(_T(" 【emergency call】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_SNATCH == iType)
			{
				strEventMsg.Append(_T(" 【组呼建立成功】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_BEGIN == iType)
			{
				strEventMsg.Append(_T(" 【话权变更】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_IDLE == iType)
			{
				strEventMsg.Append(_T(" 【话权空闲】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_ACCEPT == iType)
			{
				strEventMsg.Append(_T(" 【释放话权接受】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_REJECT == iType)
			{
				strEventMsg.Append(_T(" 【抢权拒绝】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_HANGUP_OK == iType)
			{
				strEventMsg.Append(_T(" 【退出组呼成功】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CLOSE_OK == iType)
			{
				strEventMsg.Append(_T(" 【组呼结束】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
			{
				strEventMsg.Append(_T(" 【不被许可】\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CANCEL_OK == iType)
			{
				// emergency call
				strEventMsg.Append(_T(" 【组呼建立失败】\r\n"));
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
				strMsg.Format(_T("用户【%s】已经在别处登录\r\n%s"), m_strName, strModulePara);
				strEventMsg.Append(_T(" 【被踢下线】"));
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
				strTriggerCode = _T("周期上报");
			}
			else if(1 == iTriggerCode)
			{
				strTriggerCode = _T("特定事件, emergency call");
			}

			int iReportStatus = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("ReportStatus"));
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
			strEventMsg.Insert(0,GetTimeString());
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent9.Append(strEventMsg);
		}
		break;

	case EVENT_NOTIFY_SDS_REPORT: // notifySdsReport
		{
			CString strSdsType = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsType"));
			CString strSdsContent = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsContent"));
			CString strSdsFrom = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsFrom"));
			CString strSdsMmsFileNameList = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsMmsFileNameList"));
			
			CString strEventMsg;
			strEventMsg.Format(_T("SdsType:%s SdsContent:%s SdsFrom:%s SdsMmsFileNameList:%s"), strSdsType, strSdsContent, strSdsFrom, strSdsMmsFileNameList);
			strEventMsg.Insert(0,GetTimeString());
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent10.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_SDS_RETCODE: // notifySdsStatusReport
		{
			CString strSdsTo = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsReceiver"));
			CString strSdsMsgId = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsSubject"));
			CString strSdsRetCode = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("SdsRetCode"));

			CString strEventMsg;
			strEventMsg.Format(_T("strSdsTo:%s strSdsMsgId:%s strSdsRetCode:%s"), strSdsTo, strSdsMsgId, strSdsRetCode);
			strEventMsg.Insert(0,GetTimeString());
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent11.Append(strEventMsg);
		}
		break;
	}
}
CString CeLTE_PlayerDemoDlg::GetMRSIP()
{
	return m_strMRSIP;
}

void CeLTE_PlayerDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_eLTE_Player.ELTE_OCX_Logout(m_strName);
	static bool bShowKickOff = FALSE;

	if(0 < m_strKickOff.GetLength() && !bShowKickOff)
	{
		bShowKickOff = TRUE;
		if (IDOK == m_DcDlg.MessageBox(m_strKickOff))
		{
			if (m_DcDlg.m_hWnd)
			{
				m_DcDlg.SetCeLTE_PlayerDemoDlg(NULL);
				m_DcDlg.SendMessage(WM_CLOSE);
				m_DcDlg.DestroyWindow();
			}
		}
		
	}

	

	
	m_eLTE_Player.ELTE_OCX_UnLoad();
	m_bClose = true;
	CDialogEx::OnClose();
}


void CeLTE_PlayerDemoDlg::OnBnClickedButtonLogin()
{
	
	
	//Other non essential parameter settings before login
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


void CeLTE_PlayerDemoDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
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


BOOL CeLTE_PlayerDemoDlg::ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	// TODO: Add your message handler code here

	m_DcDlg.ELTE_PlayerEvent(CtlID, ulEventType, pEventDataXml);

	return TRUE;
}

BOOL CeLTE_PlayerDemoDlg::GetLocalIP() 
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
		m_comboLocalIP.AddString(LPCTSTR(CString(inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]))));
		if (host->h_addr_list[i]+host->h_length >= host->h_name)
		{
			break;
		}
	}
	return TRUE;
}

BOOL CeLTE_PlayerDemoDlg::ReadIniFile()
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

BOOL CeLTE_PlayerDemoDlg::WriteIniFile()
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

void CeLTE_PlayerDemoDlg::MoreSetting()
{
	//if (!m_bMax)
	//	return;

	UpdateData(TRUE);
	// set log parameter
	m_eLTE_Player.ELTE_OCX_SetLogLevel(m_cmbLogLevel.GetCurSel());
	m_eLTE_Player.ELTE_OCX_SetLogPath(m_strLogSavePath);

	// set logo path
	if (!m_strBGLogoPath.IsEmpty())
	{
		m_eLTE_Player.ELTE_OCX_UploadLogo(m_strBGLogoPath);
	}

	// get version
	CString strResult = m_eLTE_Player.ELTE_OCX_GetVersion(eLTE_VERSION_OCX);//1 ocx, 2 SDK
	CString strVersion = GET_XML_ELEM_VALUE_STR(strResult, _T("Version"));
	CString strText;
	strText.Format(_T("DConsole    [%s]"), strVersion);
	m_DcDlg.SetWindowText(strText);
}

CString CeLTE_PlayerDemoDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}
