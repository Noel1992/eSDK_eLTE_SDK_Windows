
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


CeLTE_SDK_Video_DemoDlg& CeLTE_SDK_Video_DemoDlg::Instance()
{
	static CeLTE_SDK_Video_DemoDlg s_dlg;
	return s_dlg;
}

CeLTE_SDK_Video_DemoDlg::CeLTE_SDK_Video_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeLTE_SDK_Video_DemoDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strServerIP(_T(""))
	, m_strSipPort(_T(""))
	, m_strLocalIP(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLogin = FALSE;
}

void CeLTE_SDK_Video_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SIPPORT, m_strSipPort);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cmbLocalIP);
	DDX_CBString(pDX, IDC_COMBO_LOCALIP, m_strLocalIP);
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

		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Value:%d ResId:%s ["), iType, iValue, strResId);
		strEventMsg.Insert(0, Instance().GetTimeString());

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
				pDlg->MessageBox(_T("User or password error."));
				strEventMsg.Append(_T("User or password error."));
			}
			else if (STATUS_LICENSE_LIMIT == iValue)
			{
				pDlg->MessageBox(_T("License limit."));
				strEventMsg.Append(_T("License limit."));
			}
			else if (STATUS_RES_CONFILCT == iValue)
			{
				pDlg->MessageBox(_T("Resource confilct."));
				strEventMsg.Append(_T("Resource confilct."));
			}
			else if (STATUS_TEMP_UNAVAILABLE == iValue)
			{
				pDlg->MessageBox(_T("Resource unavailable."));
				strEventMsg.Append(_T("Resource unavailable."));
			}
			else if (STATUS_FORBIDEN == iValue)
			{
				pDlg->MessageBox(_T("Resource forbiden."));
				strEventMsg.Append(_T("Resource forbiden."));
			}
			else if (STATUS_REGFAIL == iValue)
			{
				if(!pDlg->GetIsLogin())
				{
					pDlg->MessageBox(_T("Resource register fail."));
					strEventMsg.Append(_T("Resource register fail."));
				}
				else
				{
					pDlg->SetIsLogin(FALSE);
					pDlg->PostMessage(WM_CLOSE);
				}
			}
			else if (STATUS_UNAUTHORIZED == iValue)
			{
				strEventMsg.Append(_T("not authorized."));
			}
		}
		if(GRPCALLSTATUS == iType)
		{
			// Group active state
			if (STATUS_GROUP_ACTIVATED == iValue)
			{
				strEventMsg.Append(_T("Group call active state."));
			}
			//  Group call not active state
			else if (STATUS_GROUP_DEACTIVATED == iValue)
			{
				strEventMsg.Append(_T("Group call not active state."));
			}
		}

		//Group subscription
		if (RESASSIGNSTATUS == iType)
		{
			if (STATUS_ASSIGNED == iValue)//Subscription success
			{
				CString strMsg;
				strMsg.Format(_T("Group[%s]subscription success."), strResId);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DEASSIGNED == iValue)//unsubscribe success
			{
				CString strMsg;
				strMsg.Format(_T("Group[%s]unsubscription success."), strResId);

				Instance().MessageBox(strMsg);
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
				strMsg.Format(_T("Patch group[%d]create success."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if(STATUS_GRPPATCH_CREATEFAIL == iValue)//create failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]create failed.Cause of failure=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_CANCELOK == iValue)//delete success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]delete success."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if(STATUS_GRPPATCH_CANCELFAIL == iValue)//delete failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]del failed.Cause of failure=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_ADDOK == iValue)//add member success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]add member success."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if(STATUS_GRPPATCH_ADDFAIL == iValue)//add member failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]add member failed.Cause of failure=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			if(STATUS_GRPPATCH_DELOK == iValue)//delete member success
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]del member success."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if(STATUS_GRPPATCH_DELFAIL == iValue)//delete member failed
			{
				CString strMsg;
				strMsg.Format(_T("Patch group[%d]delete member failed.Cause of failure=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		if (USERDGNASTATUS == iType)//dynamic group
		{
			int iCause = GET_XML_ELEM_VALUE_INT(xmlStr, _T("Cause"));
			int iGroupID = GET_XML_ELEM_VALUE_INT(xmlStr, _T("AttachingGroup"));
// 			if (0 == iGroupID)
// 			{
// 				CString strtemp = Instance().m_DConsoleDlg.m_ModifyDGNAGroupId;
// 				iGroupID = eLTE_Tool::String2Int(eLTE_Tool::UnicodeToANSI(strtemp));
// 			}
			if (STATUS_DGNAFAIL == iValue)//create failed
			{
				CString strMsg;
				strMsg.Format(_T("Dynamic group[%d]create failed.Cause of failure=%d"), iGroupID, iCause);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DGNAOK == iValue)//create success
			{
				CString strMsg;
				strMsg.Format(_T("Dynamic group[%d]success."), iGroupID);
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
			else if (STATUS_DGNA_UEFAIL == iValue)//dynamic group UE reorganization failed
			{
				CString strMsg;
				if (iCause == -20044)
				{
					strMsg.Append(_T("["));
					strMsg.Append(strResId);
					strMsg.Append(_T("unregistered]"));
				}
				else if (iCause == -20042)
				{
					strMsg.Append(_T("["));
					strMsg.Append(strResId);
					strMsg.Append(_T("time out]"));
				}
				else
				{
					strMsg.Format(_T("Dynamic group[%d]UE[%s]reorganization failure.Cause of failure=%d,Please try again later"), iGroupID, strResId, iCause);
				}
				//fail,but need refresh too. 
				Instance().MessageBox(strMsg);
				strEventMsg.Append(strMsg);
			}
		}

		// Callback event message display
		strEventMsg.Append(_T("]\r\n"));
		Instance().m_DConsoleDlg.m_ResStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyUserStatus == iEventType)
	{
		CString strUserID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("UserID"));
		int iStatusType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusType"));
		int iStatusValue = GET_XML_ELEM_VALUE_INT(xmlStr, _T("StatusValue"));
		pDlg->GetDConsoleDlg().UpdateUserStatus(strUserID, iStatusValue);
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
		if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // Kicked off
		{
			strEventMsg.Append(_T(" [Kicked off]"));
			//Instance().PostMessage(WM_CLOSE);				
		}
		strEventMsg.Append(_T("\r\n"));
		Instance().m_DConsoleDlg.m_ModuleStatus.Append(strEventMsg);
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
			// Emergency call
			strEventMsg.Append(_T(" [Emergency call]\r\n"));
		}
		else if (PTT_IND_STATUS_SNATCH == iType)
		{
			strEventMsg.Append(_T(" [Success of group call]\r\n"));
		}
		else if (PTT_IND_STATUS_TX_BEGIN == iType)
		{
			strEventMsg.Append(_T(" [Change of the power of speech]\r\n"));
		}
		else if (PTT_IND_STATUS_TX_IDLE == iType)
		{
			strEventMsg.Append(_T(" [Idle]\r\n"));
		}
		else if (PTT_IND_STATUS_ACCEPT == iType)
		{
			strEventMsg.Append(_T(" [Release]\r\n"));
		}
		else if (PTT_IND_STATUS_REJECT == iType)
		{
			strEventMsg.Append(_T(" [Reject]\r\n"));
		}
		else if (PTT_IND_STATUS_HANGUP_OK == iType)
		{
			strEventMsg.Append(_T(" [Hangup ok]\r\n"));
		}
		else if (PTT_IND_STATUS_CLOSE_OK == iType)
		{
			strEventMsg.Append(_T(" [End of PTT]\r\n"));
		}
		else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
		{
			strEventMsg.Append(_T(" [License limit]\r\n"));
		}
		else if (PTT_IND_STATUS_CANCEL_OK == iType)
		{
			strEventMsg.Append(_T(" [Group call failed]\r\n"));
		}
		else
		{
			strEventMsg.Append(_T("\r\n"));
		}
		Instance().m_DConsoleDlg.m_GroupStatus.Append(strEventMsg);
	}
	else if (ELTE_Event_NotifyP2pvideocallStatus == iEventType)
	{
		int iCallStatus = GET_XML_ELEM_VALUE_INT(xmlStr, _T("CallStatus"));
		CString strResID = GET_XML_ELEM_VALUE_STR(xmlStr, _T("Callee"));
		INT  iVideoFormatType = GET_XML_ELEM_VALUE_INT(xmlStr, _T("VideoFormatType"));

		// current status
		CString strEventMsg;
		strEventMsg.Format(_T("Type:%d Camera:%s ["), iCallStatus, strResID);
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
		strEventMsg.Append(_T("]\r\n"));
		strEventMsg.Insert(0,pDlg->GetTimeString());
		Instance().m_DConsoleDlg.m_VideoStatus.Append(strEventMsg);
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


BOOL CeLTE_SDK_Video_DemoDlg::ReadIniFile()
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


BOOL CeLTE_SDK_Video_DemoDlg::WriteIniFile()
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

BOOL CeLTE_SDK_Video_DemoDlg::GetLocalIP() 
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
