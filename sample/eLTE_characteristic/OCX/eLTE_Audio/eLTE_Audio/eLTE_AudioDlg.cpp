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
// eLTE_AudioDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "eLTE_Audio.h"
#include "eLTE_AudioDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CeLTE_AudioDlg �Ի���


int CeLTE_AudioDlg::m_iBypass = 1;
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
	DDX_Control(pDX, IDC_ELTE_PLAYERCTRL1, m_eLTE_Player);
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


// CeLTE_AudioDlg ��Ϣ�������

BOOL CeLTE_AudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_cmbMediaPass.InsertString(0, _T("1"));
	m_cmbMediaPass.InsertString(1, _T("0"));
	m_cmbMediaPass.SetCurSel(1);
	
	if (GetLocalIP())
	{
		printf("Get Local IP failed!\n");
	}
	

	// ��ʼ����½��Ϣ
	if (!ReadIniFile())
	{
		m_strName = _T("4101");
		m_strPasswd = _T("4101");
		m_strServerIP = _T("10.170.102.234");
		m_strMRSIP = _T("10.170.102.234");
		m_strLocalIP = _T("10.135.46.130");
		m_strSipPort = _T("5060");
	}

	// ��ʼ��־��Ϣ
	m_strLogSavePath = _T(".\\log");
	/*m_cmbLogLevel.InsertString(0, _T("Error"));
	m_cmbLogLevel.InsertString(0, _T("Warn"));
	m_cmbLogLevel.InsertString(0, _T("Info"));
	m_cmbLogLevel.InsertString(0, _T("Debug"));
	m_cmbLogLevel.SetCurSel(0);*/
	UpdateData(FALSE);

	// ����DConsoleDlg
	m_DcDlg.SetCeLTE_AudioDlg(this);
	m_DcDlg.SetELtePlayer(&m_eLTE_Player);
	m_DcDlg.Create(CDConsoleDlg::IDD, CWnd::GetDesktopWindow());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CeLTE_AudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CeLTE_AudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CeLTE_AudioDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
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
BEGIN_EVENTSINK_MAP(CeLTE_AudioDlg, CDialogEx)
	ON_EVENT(CeLTE_AudioDlg, IDC_ELTE_PLAYERCTRL1, 50, CeLTE_AudioDlg::ELTE_OCX_EventEltePlayerctrl1, VTS_UI4 VTS_BSTR)
END_EVENTSINK_MAP()


void CeLTE_AudioDlg::ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml)
{
	switch (ulEventType)
	{
	case EVENT_NOTIFY_USER_STATUS:	// notifyUserStatus
		{
			CString strUserID = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("UserID"));
			int iStatusType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusType"));
			int iStatusValue = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusValue"));

			m_DcDlg.UpdateUserStatus(strUserID, iStatusValue);

			// ��ʾ��Ƶ�ش���ǰ״̬
			CString strEventMsg;
			strEventMsg.Format(_T("UserID:%s Type:%d Value:%d ��"), strUserID, iStatusType, iStatusValue);
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
			strEventMsg.Append(_T("��\r\n"));
			strEventMsg.Insert(0,GetTimeString());
			m_DcDlg.m_strEvent0.Append(strEventMsg);
		}
		break;
	case EVENT_NOTIFY_RESOURCE_STATUS:	// notifyResourceStatus
		{
			CString strResId = GET_XML_ELEM_VALUE_STR(pEventDataXml, _T("ResourceID"));
			int iType = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusType"));
			int iValue = GET_XML_ELEM_VALUE_INT(pEventDataXml, _T("StatusValue"));

			// �ص��¼���Ϣ��ʾ
			CString strEventMsg;
			strEventMsg.Format(_T("Type:%d Value:%d ResId:%s ��"), iType, iValue, strResId);
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
				// ��½ʧ��
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
				// ��Դδ��Ȩ
				else if (STATUS_FORBIDEN == iValue)
				{
					MessageBox(_T("resource Not authorized."));
					strEventMsg.Append(_T("resource Not authorized."));
				}
				// �˺Ŵ���
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
				// 4017 STATUS_UNAUTHORIZED δ��Ȩ����½�м�״̬��
				else if (STATUS_UNAUTHORIZED == iValue)
				{
					if (!m_bIsLogin)
					{
						strEventMsg.Append(_T("Without authentication��Landing intermediate state��."));
					}
					else
					{
						strEventMsg.Append(_T("Without authentication��Landing intermediate state��(Heartbeat Check)."));
					}
				}
				// License����
				else if (STATUS_LICENSE_LIMIT == iValue)
				{
					MessageBox(_T("License limit."));
					strEventMsg.Append(_T("License limit."));
				}
				// ��½ʧ�����˳�MFC����
				if (STATUS_REGOK != iValue && STATUS_UNAUTHORIZED != iValue)
				{
					ShowWindow(SW_HIDE);
					PostMessage(WM_CLOSE);
				}
			}

			if(GRPCALLSTATUS == iType)
			{
				// ��״̬
				if (STATUS_GROUP_ACTIVATED == iValue)
				{
					//MessageBox(_T("��״̬."));
					strEventMsg.Append(_T("group under active status."));
				}
				//  ��ǻ״̬
				else if (STATUS_GROUP_DEACTIVATED == iValue)
				{
					//MessageBox(_T("��ǻ״̬."));
					strEventMsg.Append(_T("group under inactive status."));
				}
			}

			if (RESASSIGNSTATUS == iType)//Ⱥ�鶩��
			{
				if (STATUS_ASSIGNED == iValue)//���ĳɹ�
				{
					CString strMsg;
					strMsg.Format(_T("group [%s] subscribe success."), strResId);
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
				else if (STATUS_DEASSIGNED == iValue)//ȥ���ĳɹ�
				{
					CString strMsg;
					strMsg.Format(_T("Group[%s]unsubscribe success."), strResId);					
					//ɾ����ʱ��
					//m_DcDlg.RemoveGroup(StrToInt(strResId));
					//��ȡѡ������Ϣ
					GroupInfo* pInfo = NULL;
					//Instance().m_DConsoleDlg.GetSelGroupInfo(&pInfo);
					CString szItemroot(strResId);
					szItemroot.Append(_T(" [temp group]"));
					HTREEITEM htiRes = m_DcDlg.FindStrGroupInfo(szItemroot, &pInfo);

					if ( (NULL != pInfo)&& (NULL != htiRes))
					{
						if ("-1" == pInfo->GroupCategory)
						{
							//ɾ����ʱ��
							m_DcDlg.RemoveGroup(StrToInt(strResId));
						}					
					}
					MessageBox(strMsg);
					strEventMsg.Append(strMsg);
				}
			}

			// �ص��¼���Ϣ��ʾ
			strEventMsg.Append(_T("��\r\n"));
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
			//��ʼ��ʼ��ProvMgr
			m_DcDlg.InitDlg();

			CString strEventMsg = GetTimeString();
			strEventMsg.Append(_T("notifyProvisionAllResync\r\n"));
			m_DcDlg.m_strEvent4.Append(strEventMsg);

			// ��ʼ��MRS
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
				// ��������
				strEventMsg.Append(_T(" ��emergency call��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PROCEEDING == iType)
			{
				strEventMsg.Append(_T(" ��processing��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_REMOTE_NOTCONNECTED == iType)
			{
				strEventMsg.Append(_T(" ��far-end not connected��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RINGING == iType)
			{
				strEventMsg.Append(_T(" ��ring��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_ANSWERED == iType)
			{
				strEventMsg.Append(_T(" ��be answered��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_ACKED == iType)
			{
				strEventMsg.Append(_T(" ��Connected��\r\n"));
				m_DcDlg.SetCaller(strCaller, 1);
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PREMPTED == iType)
			{
				strEventMsg.Append(_T(" ��be breakin��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_HANGUPED == iType)
			{
				strEventMsg.Append(_T(" ��be hangup��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_PICKEDUP == iType)
			{
				strEventMsg.Append(_T(" ��The end has been picking up��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RELEASED_BUSY == iType)
			{
				strEventMsg.Append(_T(" ��busy��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_HANGUPED_ACTIVE == iType)
			{
				strEventMsg.Append(_T(" ��end calling��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
			}
			else if (P2P_IND_STATUS_RECEIVED == iType)
			{
				strEventMsg.Append(_T(" ��coming call��\r\n"));
				m_DcDlg.m_strEvent5.Append(strEventMsg);
				m_DcDlg.SetCaller(strCaller, 0);
				m_DcDlg.PostMessage(WM_RECEIVED, 0, 0);
			}
			else if (P2P_IND_STATUS_REMOTE_NOANSWER == iType)
			{
				strEventMsg.Append(_T(" ��no answer��\r\n"));
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
				// ��������
				strEventMsg.Append(_T(" ��emergency call��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_SNATCH == iType)
			{
				strEventMsg.Append(_T(" ��group call built success��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_BEGIN == iType)
			{
				strEventMsg.Append(_T(" ��speech right changed��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_TX_IDLE == iType)
			{
				strEventMsg.Append(_T(" ��Right to speak��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_ACCEPT == iType)
			{
				strEventMsg.Append(_T(" ��Release the right to receive��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_REJECT == iType)
			{
				strEventMsg.Append(_T(" ��breakin reject��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_HANGUP_OK == iType)
			{
				strEventMsg.Append(_T(" ��exit group call success��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CLOSE_OK == iType)
			{
				strEventMsg.Append(_T(" ��group call end��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_LICENSE_LIMIT == iType)
			{
				strEventMsg.Append(_T(" ��Not allowed��\r\n"));
				m_DcDlg.m_strEvent6.Append(strEventMsg);
			}
			else if (PTT_IND_STATUS_CANCEL_OK == iType)
			{
				// ��������
				strEventMsg.Append(_T(" ��build group call failed��\r\n"));
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
			if (iModuleType == SIP_MODULE && iModuleStatus == KICK_OFF) // ��������
			{
				CString strMsg;
				strMsg.Format(_T("user:��%s��has login somewhere else\r\n%s"), m_strName, strModulePara);
				strEventMsg.Append(_T(" ��kicked off��"));
				m_strKickOff = strMsg;
				PostMessage(WM_CLOSE);
			}
			strEventMsg.Append(_T("\r\n"));
			m_DcDlg.m_strEvent7.Append(strEventMsg);
		}
		break;
	}
}


void CeLTE_AudioDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// ������־����
	m_eLTE_Player.ELTE_OCX_SetLogLevel(0);
	m_eLTE_Player.ELTE_OCX_SetLogPath(m_strLogSavePath);

	// ����logo·��
/*	if (!m_strBGLogoPath.IsEmpty())
	{
		m_eLTE_Player.ELTE_OCX_UploadLogo(m_strBGLogoPath);
	}
*/
	// ��ȡ�汾
	CString strResult = m_eLTE_Player.ELTE_OCX_GetVersion(eLTE_VERSION_OCX);//1 ocx, 2 SDK
	CString strVersion = GET_XML_ELEM_VALUE_STR(strResult, _T("Version"));
	CString strText;
	strText.Format(_T("DConsole    [%s]"), strVersion);
	m_DcDlg.SetWindowText(strText);
}

CString CeLTE_AudioDlg::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();;
	CString szTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	return szTime;
}