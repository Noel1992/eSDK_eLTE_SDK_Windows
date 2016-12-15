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
	m_strMRSIP = _T("");
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
	

	// Initialize login information
	if (!ReadIniFile())
	{
		m_strName = _T("4101");
		m_strPasswd = _T("4101");
		m_strServerIP = _T("10.170.102.234");
		m_strMRSIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}

	// Initial log information
	m_strLogSavePath = _T(".\\log");
	/*m_cmbLogLevel.InsertString(0, _T("Error"));
	m_cmbLogLevel.InsertString(0, _T("Warn"));
	m_cmbLogLevel.InsertString(0, _T("Info"));
	m_cmbLogLevel.InsertString(0, _T("Debug"));
	m_cmbLogLevel.SetCurSel(0);*/
	UpdateData(FALSE);

	// create DConsoleDlg
	m_DcDlg.SetCeLTE_ResourceManageDlg(this);
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

END_EVENTSINK_MAP()


CDConsoleDlg& CeLTE_ResourceManageDlg::GetDConsoleDlg()
{
	return m_DcDlg;
}

CString& CeLTE_ResourceManageDlg::GetCurrentUserName()
{
	return m_strName;
}

BOOL& CeLTE_ResourceManageDlg::GetIsLogin()
{
	return m_bIsLogin;
}

void CeLTE_ResourceManageDlg::SetIsLogin(BOOL bLogin)
{
	m_bIsLogin = bLogin;
}

ELTE_VOID __SDK_CALL CeLTE_ResourceManageDlg::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	char* pEventXml = (char*)pEventBuf;
	CString xmlStr(pEventXml);
	CString strRecvVideoPlayCallerId;
	CeLTE_ResourceManageDlg* pDlg = (CeLTE_ResourceManageDlg*)pUserData;

	//Res Download Success
	if(ELTE_Event_NotifyProvisionAllResync == iEventType)
	{
		pDlg->GetDConsoleDlg().InitDlg();
		CString strEventMsg = pDlg->GetTimeString();
		strEventMsg.Append(_T("notifyProvisionAllResync\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent4.Append(strEventMsg);
		if(!pDlg->m_bClose)
		{
			int iRet = ELTE_SDK_ProvisionManagerInitMRS(eLTE_Tool::UnicodeToANSI(pDlg->m_strMRSIP).c_str());
			//CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_ProvisionManagerInitMRS"));
		}
	}

	//Res Status
	else if(ELTE_Event_NotifyResourceStatus == iEventType)
	{
		CString strResId = CXml::Instance().XmlParseElemValue(xmlStr, _T("ResourceID"));
		int iType = StrToInt(CXml::Instance().XmlParseElemValue(xmlStr, _T("StatusType")));
		int iValue = StrToInt(CXml::Instance().XmlParseElemValue(xmlStr, _T("StatusValue")));

		// Callback event message display
		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Value:%d ResId:%s ["), iType, iValue, strResId);
		strEventMsg.Insert(0,pDlg->GetTimeString());

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
		if(GRPCALLSTATUS == iType)
		{
			// group active status
			if (STATUS_GROUP_ACTIVATED == iValue)
			{
				strEventMsg.Append(_T("Group call active state."));
			}
			//  group unactive status
			else if (STATUS_GROUP_DEACTIVATED == iValue)
			{
				strEventMsg.Append(_T("Group call inactive state."));
			}
		}

		if (RESASSIGNSTATUS == iType)//group sub
		{
			if (STATUS_ASSIGNED == iValue)//sub success
			{
				CString strMsg;
				strMsg.Format(_T("Group [%s] sub success."), strResId);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DEASSIGNED == iValue)//unsub success
			{
				CString strMsg;
				strMsg.Format(_T("Group [%s] unsub success."), strResId);					
				//delete temporary group
				//m_DcDlg.RemoveGroup(StrToInt(strResId));
				//Get selected item information
				GroupInfo* pInfo = NULL;
				//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
				CString szItemroot(strResId);
				szItemroot.Append(_T(" [temp group]"));
				HTREEITEM htiRes = pDlg->GetDConsoleDlg().FindStrGroupInfo(szItemroot, &pInfo);

				if ( (NULL != pInfo)&& (NULL != htiRes))
				{
					if ("-1" == pInfo->GroupCategory)
					{
						//delete temporary group
						pDlg->GetDConsoleDlg().RemoveGroup(StrToInt(strResId));
					}					
				}
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		if (USERDGNASTATUS == iType)//dynamic group
		{
			int iCause = GET_XML_ELEM_VALUE_INT(xmlStr, _T("Cause"));
			int iGroup = GET_XML_ELEM_VALUE_INT(xmlStr, _T("AttachingGroup"));
			if (STATUS_DGNAFAIL == iValue)//create failed
			{
				CString strMsg;
				strMsg.Format(_T("Dynamic group [%d] create fail.error=%d"), iGroup, iCause);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DGNAOK == iValue)//create success
			{
				CString strMsg;
				strMsg.Format(_T("Dynamic group [%d] create success."), iGroup);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				pDlg->GetDConsoleDlg().AddGroup(iGroup);
			}
			else if (STATUS_DGNA_UEFAIL == iValue)//dynamic group UE reorganization failed
			{
				CString strMsg;
				if (iCause == -20044)
				{
					strMsg.Append(_T("["));
					strMsg.Append(strResId);
					strMsg.Append(_T("unregistered]"));
					pDlg->GetDConsoleDlg().AddGroup(iGroup);
				}
				else if (iCause == -20042)
				{
					strMsg.Append(_T("["));
					strMsg.Append(strResId);
					strMsg.Append(_T("time out]"));
					pDlg->GetDConsoleDlg().AddGroup(iGroup);
				}
				else
				{
					strMsg.Format(_T("Dynamic group[%d]UE[%s]create fail.error=%d"), iGroup, strResId, iCause);
				}
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		//patch group
		if (GRPPATCHSTATUS == iType)
		{
			int iCause = GET_XML_ELEM_VALUE_INT(xmlStr, _T("Cause"));
			int iGroupID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("ResourceID"));
			int iMemberID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("MemberID"));
			if(STATUS_GRPPATCH_CREATEOK == iValue)//create success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] create success."), iGroupID);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				pDlg->GetDConsoleDlg().AddPatchGroup(iGroupID);
			}
			else if(STATUS_GRPPATCH_CREATEFAIL == iValue)//create failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] create fail.error=%d"), iGroupID, iCause);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_CANCELOK == iValue)//delete success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d] del success."), iGroupID);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				pDlg->GetDConsoleDlg().RemovePatchGroup(iGroupID);
			}
			else if(STATUS_GRPPATCH_CANCELFAIL == iValue)//delete failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] del fail.error=%d"), iGroupID, iCause);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_ADDOK == iValue)//add member success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] add member success."), iGroupID);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				pDlg->GetDConsoleDlg().AddPatchGroupMember(iGroupID, iMemberID);
			}
			else if(STATUS_GRPPATCH_ADDFAIL == iValue)//add member failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] add member fail.error=%d"), iGroupID, iCause);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_DELOK == iValue)//delete member success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] del member success."), iGroupID);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
				pDlg->GetDConsoleDlg().DelPatchGroupMember(iGroupID, iMemberID);
			}
			else if(STATUS_GRPPATCH_DELFAIL == iValue)//delete member failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group [%d] del member fail.error=%d"), iGroupID, iCause);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		//show callback message
		strEventMsg.Append(_T("]\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent2.Append(strEventMsg);

	}
	else if (ELTE_Event_NotifyUserStatus == iEventType)
	{
		CString strUserID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("UserID"));
		int iStatusType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusType"));
		int iStatusValue = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusValue"));
		pDlg->GetDConsoleDlg().UpdateUserStatus(strUserID, iStatusValue);

		// Callback event message display
		CString strEventMsg;
		strEventMsg.Format(_T("UserID:%s Type:%d Value:%d ["), strUserID, iStatusType, iStatusValue);
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
			strEventMsg.Append(_T("calling"));
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
		strEventMsg.Append(_T("]\r\n"));
		strEventMsg.Insert(0, pDlg->GetTimeString());
		pDlg->GetDConsoleDlg().m_strEvent0.Append(strEventMsg);
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
		strEventMsg.Insert(0,pDlg->GetTimeString());
		if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // kicked off
		{
			CString strMsg;
			strMsg.Format(_T("The user [%s] has logged in elsewhere\r\n%s"), pDlg->m_strName, strModulePara);
			strEventMsg.Append(_T(" [Kicked off the assembly line]"));
			pDlg->m_strKickOff = strMsg;
			pDlg->PostMessage(WM_CLOSE);
		}
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent7.Append(strEventMsg);
	}
}


void CeLTE_ResourceManageDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ELTE_SDK_Logout(eLTE_Tool::UnicodeToANSI(m_strName).c_str());
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


	ELTE_SDK_Cleanup();
	m_bClose = true;
	CDialogEx::OnClose();
}


void CeLTE_ResourceManageDlg::OnBnClickedButtonLogin()
{
	// TODO: 
	// Log parameter setting
	MoreSetting();
	UpdateData(TRUE);

	int iRet = 0;

	iRet = ELTE_SDK_Init(FALSE);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_Init"));

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	//EnableWindow(FALSE);

	// set call back function
	iRet = ELTE_SDK_SetEventCallBack(ELTE_EventCallBack, this);
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_SetEventCallBack failed."));
		return;
	}

	iRet = ELTE_SDK_Login(eLTE_Tool::UnicodeToANSI(m_strName).c_str(), eLTE_Tool::UnicodeToANSI(m_strPasswd).c_str(), eLTE_Tool::UnicodeToANSI(m_strServerIP).c_str(), eLTE_Tool::UnicodeToANSI(m_strLocalIP).c_str(), atoi(eLTE_Tool::UnicodeToANSI(m_strSipPort).c_str()));
	if (0 != iRet)
	{
		CString str;
		str.Format(_T("ELTE_SDK_Login Failed. (%d)"), iRet);
		MessageBox(str);
		PostMessage(WM_CLOSE);
		return;
	}
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_Login"));
	m_DcDlg.SetParam(m_strName, m_strServerIP, m_strLocalIP);
	WriteIniFile();
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
	// Set log parameters
	ELTE_SDK_SetLogLevel(0);
	ELTE_SDK_SetLogPath(eLTE_Tool::UnicodeToANSI(m_strLogSavePath).c_str());

	// Get version
	/*ELTE_CHAR* pVersion = NULL;
	int iRet = ELTE_SDK_GetVersion(&pVersion);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetVersion"));

	CString strText;
	strText.Format(_T("DConsole   [%s]"), eLTE_Tool::ANSIToUnicode(pVersion));
	m_DcDlg.SetWindowText(strText);

	ELTE_SDK_ReleaseBuffer(pVersion);
	pVersion = NULL;*/
}

CString CeLTE_ResourceManageDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}