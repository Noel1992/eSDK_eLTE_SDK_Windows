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
// eLTE_AudioDlg.cpp : Implementation file
//

#include "stdafx.h"
#include "eLTE_Audio.h"
#include "eLTE_AudioDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "DataType.h"
#include "eLTE_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog box for application "on" menu item

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Realization
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


// CeLTE_AudioDlg dlg


int CeLTE_AudioDlg::m_iBypass = 0;
CeLTE_AudioDlg::CeLTE_AudioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_AudioDlg::IDD, pParent)
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

void CeLTE_AudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_strName);
	//  DDX_Control(pDX, IDC_EDIT_MRSIP, m_MRSIP);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MRSIP, m_strMRSIP);
	DDX_Control(pDX, IDC_COMBO_MEDIA_PASS, m_cmbMediaPass);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
}

BEGIN_MESSAGE_MAP(CeLTE_AudioDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CeLTE_AudioDlg::OnBnClickedButtonLogin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeLTE_AudioDlg Message handler

BOOL CeLTE_AudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	//  Perform this operation
	SetIcon(m_hIcon, TRUE);			// Set big Icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization code
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
	m_DcDlg.SetCeLTE_AudioDlg(this);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // Returns TRUE if the focus is set to the control.
}

void CeLTE_AudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CeLTE_AudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CeLTE_AudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CDConsoleDlg& CeLTE_AudioDlg::GetDConsoleDlg()
{
	return m_DcDlg;
}

CString& CeLTE_AudioDlg::GetCurrentUserName()
{
	return m_strName;
}

BOOL& CeLTE_AudioDlg::GetIsLogin()
{
	return m_bIsLogin;
}

void CeLTE_AudioDlg::SetIsLogin(BOOL bLogin)
{
	m_bIsLogin = bLogin;
}

ELTE_VOID __SDK_CALL CeLTE_AudioDlg::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	
	char* pEventXml = (char*)pEventBuf;
	CString xmlStr(pEventXml);
	CString strRecvVideoPlayCallerId;
	CeLTE_AudioDlg* pDlg = (CeLTE_AudioDlg*)pUserData;

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
				strEventMsg.Append(_T("Group active status."));
			}
			//  group unactive status
			else if (STATUS_GROUP_DEACTIVATED == iValue)
			{
				strEventMsg.Append(_T("Group not active status."));
			}
		}
		if (RESASSIGNSTATUS == iType)//group sub
		{
			if (STATUS_ASSIGNED == iValue)//sub success
			{
				CString strMsg;
				strMsg.Format(_T("group [%s]."), strResId);
				pDlg->MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DEASSIGNED == iValue)//unsub success
			{
				CString strMsg;
				strMsg.Format(_T("Group[%s]unsubscribe success."), strResId);					
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

		// Return the current state of video display
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
		strEventMsg.Append(_T("]\r\n"));
		strEventMsg.Insert(0, pDlg->GetTimeString());
		pDlg->GetDConsoleDlg().m_strEvent0.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyP2pcallStatus == iEventType)
	{
		int iType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("P2pcallStatus"));
		CString strCaller = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Caller"));
		CString strCallee = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));

		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Caller:%s Callee:%s"), iType, strCaller, strCallee);
		strEventMsg.Insert(0,pDlg->GetTimeString());
		if (P2P_IND_STATUS_EMERGENCY == iType)
		{
			// Emergency call
			strEventMsg.Append(_T(" [emergency call]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_PROCEEDING == iType)
		{
			strEventMsg.Append(_T(" [processing]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_REMOTE_NOTCONNECTED == iType)
		{
			strEventMsg.Append(_T(" [far-end not connected]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_RINGING == iType)
		{
			strEventMsg.Append(_T(" [ringing]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_ANSWERED == iType)
		{
			strEventMsg.Append(_T(" [be answered]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_ACKED == iType)
		{
			strEventMsg.Append(_T(" [Connected]\r\n"));
			pDlg->GetDConsoleDlg().SetCaller(strCaller, 1);
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_PREMPTED == iType)
		{
			strEventMsg.Append(_T(" [be breakin]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_HANGUPED == iType)
		{
			strEventMsg.Append(_T(" [be hangup]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_PICKEDUP == iType)
		{
			strEventMsg.Append(_T(" [The end has been picking up]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_RELEASED_BUSY == iType)
		{
			strEventMsg.Append(_T(" [busy]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_HANGUPED_ACTIVE == iType)
		{
			strEventMsg.Append(_T(" [end calling]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else if (P2P_IND_STATUS_RECEIVED == iType)
		{
			strEventMsg.Append(_T(" [coming call]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
			pDlg->GetDConsoleDlg().SetCaller(strCaller, 0);
			pDlg->GetDConsoleDlg().PostMessage(WM_RECEIVED, 0, 0);
		}
		else if (P2P_IND_STATUS_REMOTE_NOANSWER == iType)
		{
			strEventMsg.Append(_T(" [no answer]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
		else
		{
			strEventMsg.Append(_T("\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent5.Append(strEventMsg);
		}
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
		if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // kick off
		{
			CString strMsg;
			strMsg.Format(_T("User[%s]already logged in elsewhere\r\n%s"), pDlg->m_strName, strModulePara);
			strEventMsg.Append(_T(" [Kicked off]"));
			pDlg->m_strKickOff = strMsg;
			pDlg->PostMessage(WM_CLOSE);
		}
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent7.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyGroupStatus == iEventType)
	{
		int iType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("GroupCallStatus"));
		CString strGroupID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("GroupID"));
		CString strSpeaker = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Speaker"));

		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d GroupID:%s Speaker:%s"), iType, strGroupID, strSpeaker);
		strEventMsg.Insert(0,pDlg->GetTimeString());
		if (PTT_IND_STATUS_EMERGENCY_BEGIN == iType)
		{
			// Emergency call
			strEventMsg.Append(_T(" [emergency call]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_SNATCH == iType)
		{
			strEventMsg.Append(_T(" [group call built success]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_TX_BEGIN == iType)
		{
			strEventMsg.Append(_T(" [speech right changed]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_TX_IDLE == iType)
		{
			strEventMsg.Append(_T(" [Right to speak]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_ACCEPT == iType)
		{
			strEventMsg.Append(_T(" [Release the right to receive]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_REJECT == iType)
		{
			strEventMsg.Append(_T(" [breakin reject]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_HANGUP_OK == iType)
		{
			strEventMsg.Append(_T(" [exit group call success]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_CLOSE_OK == iType)
		{
			strEventMsg.Append(_T(" [group call end]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
		{
			strEventMsg.Append(_T(" [Not allowed]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else if (PTT_IND_STATUS_CANCEL_OK == iType)
		{
			// Emergency call
			strEventMsg.Append(_T(" [build group call failed]\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
		else
		{
			strEventMsg.Append(_T("\r\n"));
			pDlg->GetDConsoleDlg().m_strEvent6.Append(strEventMsg);
		}
	}
	else if (ELTE_Event_NotifySDSReport == iEventType)
	{
		CString strSdsType = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsType"));
		CString strSdsContent = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pEventXml), _T("SdsContent"));
		CString strSdsFrom = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsFrom"));
		CString strSdsMmsFileNameList = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pEventXml), _T("SdsMmsFileNameList"));

		CString strEventMsg;
		strEventMsg.Format(_T("SdsType:%s SdsContent:%s SdsFrom:%s SdsMmsFileNameList:%s"), strSdsType, strSdsContent, strSdsFrom, strSdsMmsFileNameList);
		strEventMsg.Insert(0,pDlg->GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent10.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifySDSSendStatus == iEventType)
	{
		CString strSdsTo = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsReceiver"));
		CString strSdsMsgId = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsSubject"));
		CString strSdsRetCode = GET_XML_ELEM_VALUE_STR(xmlStr, _T("SdsRetCode"));

		CString strEventMsg;
		strEventMsg.Format(_T("strSdsTo:%s strSdsMsgId:%s strSdsRetCode:%s"), strSdsTo, strSdsMsgId, strSdsRetCode);
		strEventMsg.Insert(0,pDlg->GetTimeString());
		strEventMsg.Append(_T("\r\n"));
		pDlg->GetDConsoleDlg().m_strEvent11.Append(strEventMsg);
	}

}

void CeLTE_AudioDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
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
BEGIN_EVENTSINK_MAP(CeLTE_AudioDlg, CDialogEx)
END_EVENTSINK_MAP()

void CeLTE_AudioDlg::OnClose()
{

	CDialogEx::OnClose();
}

BOOL CeLTE_AudioDlg::GetLocalIP() 
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


BOOL CeLTE_AudioDlg::ReadIniFile()
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

BOOL CeLTE_AudioDlg::WriteIniFile()
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


void CeLTE_AudioDlg::MoreSetting()
{
		//if (!m_bMax)
	//	return;

	UpdateData(TRUE);
	// Set log parameters
	ELTE_SDK_SetLogLevel(0);
	ELTE_SDK_SetLogPath(eLTE_Tool::UnicodeToANSI(m_strLogSavePath).c_str());

	// Get version
	ELTE_CHAR* pVersion = NULL;
	int iRet = ELTE_SDK_GetVersion(&pVersion);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetVersion"));

	CString strText;
	strText.Format(_T("DConsole   [%s]"), eLTE_Tool::ANSIToUnicode(pVersion));
	m_DcDlg.SetWindowText(strText);

	ELTE_SDK_ReleaseBuffer(pVersion);
	pVersion = NULL;
}

CString CeLTE_AudioDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}