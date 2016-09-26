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

// DConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "VideoParamDlg.h"
#include "InfoDlg.h"
#include "DGNAParam.h"
#include "eLTE_PlayerDemoDlg.h"
#include "SDSMessage.h"
#include "DispatchParamDlg.h"
#include "UnDispatchParamDlg.h"
#include "VWall.h"
#include "P2PTransferDlg.h"
#include "CreatePatchGroupDlg.h"
#include "PTZCtrlDlg.h"
#include "SetGisCfgDlg.h"
#include "TempUserJoinGrpCall.h"
#include "ModifyDGNADlg.h"

// CDConsoleDlg dialog
typedef enum 
{
	DCUSERS_RESID = 0,
	DCUSERS_STATUS,
	DCUSERS_NAME,
	DCUSERS_CATEGORY,
	DCUSERS_PRIORITY
} DCUSERS_COLUMN_E;

typedef enum
{
	Img_camera_offline = 0,
	Img_camera_online,
	Img_dispatcher_offline,
	Img_dispatcher_online,
	Img_gwuser_offline,
	Img_gwuser_online,
	Img_pttuser_offline,
	Img_pttuser_online
} IMAGE_LIST_E;

#define ID_MENU_BEGIN			 3000
#define ID_MENU_DCUSERINFO		 (ID_MENU_BEGIN+1)
#define ID_MENU_DCGROUPINFO		 (ID_MENU_BEGIN+2)
#define ID_MENU_SUB_DCGROUP		 (ID_MENU_BEGIN+3)
#define ID_MENU_UNSUB_DCGROUP	 (ID_MENU_BEGIN+4)
#define ID_MENU_RM_DGNA			 (ID_MENU_BEGIN+5)
#define ID_MENU_DCGROUPUSERINFO	 (ID_MENU_BEGIN+6)
#define ID_MENU_DCP2PDIAL        (ID_MENU_BEGIN+7)
#define ID_MENU_DCP2PHANGUP      (ID_MENU_BEGIN+8)
#define ID_MENU_DCP2PBREAKIN     (ID_MENU_BEGIN+9)
#define ID_MENU_DCP2PBREAKOFF    (ID_MENU_BEGIN+10)
#define ID_MENU_DCP2PVOLMUTE     (ID_MENU_BEGIN+11)
#define ID_MENU_DCP2PUNVOLMUTE   (ID_MENU_BEGIN+12)
#define ID_MENU_DCGISSUBSCRIBE   (ID_MENU_BEGIN+13)
#define ID_MENU_DCGISUNSUBSCRIBE (ID_MENU_BEGIN+14)

#define ID_MENU_JOIN_DCGROUP     (ID_MENU_BEGIN+15)
#define ID_MENU_SUB_JOIN_DCGROUP (ID_MENU_BEGIN+16)
#define ID_MENU_DCPTTDIAL        (ID_MENU_BEGIN+17)
#define ID_MENU_DCPTTRELEASE     (ID_MENU_BEGIN+18) 
#define ID_MENU_DCPTTHANGUP      (ID_MENU_BEGIN+19) 
#define ID_MENU_DCPTTEMERGENCY   (ID_MENU_BEGIN+20) 
#define ID_MENU_GROUPBREAKOFF    (ID_MENU_BEGIN+21)
#define ID_MENU_DCPTTVOLMUTE     (ID_MENU_BEGIN+22)
#define ID_MENU_DCPTTVOLUNMUTE   (ID_MENU_BEGIN+23)

#define ID_MENU_DCP2PTRANSFER      (ID_MENU_BEGIN+24)
#define ID_MENU_RMPATCHROUP        (ID_MENU_BEGIN+25)
#define	ID_MENU_ADDPATCHROUPMEMBER (ID_MENU_BEGIN+26)
#define	ID_MENU_DELPATCHROUPMEMBER (ID_MENU_BEGIN+27)
#define	ID_MENU_GETPATCHGROUPINFO  (ID_MENU_BEGIN+28)
#define ID_MENU_PTZCONTROL         (ID_MENU_BEGIN+29)

#define ID_MENU_P2PHALFDPXDIAL     (ID_MENU_BEGIN+30)
#define ID_MENU_P2PHALFDPXRELEASE  (ID_MENU_BEGIN+31)
#define ID_MENU_GETGISCFG          (ID_MENU_BEGIN+32)
#define ID_MENU_SETGISCFG          (ID_MENU_BEGIN+33)
#define ID_MENU_JOINCALL           (ID_MENU_BEGIN+34)
#define ID_MENU_MODIFYDGROUP       (ID_MENU_BEGIN+35)

#define ID_MENU_END				   (ID_MENU_BEGIN+60)


#define CHECK_RESOURCE()						\
	CString strResID;							\
	GetDlgItemText(IDC_EDIT_RESID, strResID);	\
	if (strResID.IsEmpty())						\
	{											\
		MessageBox(_T("ResourceID is empty."));	\
		return;									\
	}

#define CHECK_DISRESOURCE()						\
	CString strResID;							\
	GetDlgItemText(IDC_EDIT_RESOURCEID, strResID);	\
	if (strResID.IsEmpty())						\
{											\
	MessageBox(_T("ResourceID is empty."));	\
	return;									\
	}

#define CHECK_RESOURCE_EX()						\
	CString strResID;							\
	GetDlgItemText(IDC_EDIT_RESID, strResID);	\
	if (strResID.IsEmpty())						\
	{											\
		MessageBox(_T("ResourceID is empty."));	\
		return;									\
	}											\
	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));	\
	if (m_ResourceMap.end() == itor)															\
	{																							\
		MessageBox(_T("Not found the resource."));												\
		return;																					\
	}																							\
	ST_REALPLAY_PARAM& param = itor->second;													\
	if (NULL == param.pOCX)																		\
	{																							\
		MessageBox(_T("Ocx is null."));															\
		return;																					\
	}

IMPLEMENT_DYNAMIC(CDConsoleDlg, CDialog)

CDConsoleDlg::CDConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDConsoleDlg::IDD, pParent)
	, m_pCeLTE_PlayerDemoDlg(NULL)
	, m_peLTE_Player(NULL)
	, m_strUserID(_T(""))
	, m_strServerIP(_T(""))
	, m_strMRSIP(_T(""))
	, m_strLocalIP(_T(""))
	, m_strEvent0(_T(""))
	, m_strEvent1(_T(""))
	, m_strEvent2(_T(""))
	, m_strEvent3(_T(""))
	, m_strEvent4(_T(""))
	, m_strEvent5(_T(""))
	, m_strEvent6(_T(""))
	, m_strEvent10(_T(""))
	, m_strEvent11(_T(""))
{

}

CDConsoleDlg::~CDConsoleDlg()
{
}

void CDConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DCUSERS, m_DcUsers);
	DDX_Control(pDX, IDC_TREE_DCGROUPS, m_DcGroups);
	DDX_Control(pDX, IDC_TREE_PATCHGROUPS, m_PatchGroups);
}


BEGIN_MESSAGE_MAP(CDConsoleDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DCUSERS, &CDConsoleDlg::OnBnClickedButtonDcusers)
	ON_BN_CLICKED(IDC_BUTTON_DCGROUPS, &CDConsoleDlg::OnBnClickedButtonDcgroups)
	ON_BN_CLICKED(IDC_BUTTON_PROVINIT, &CDConsoleDlg::OnBnClickedButtonProvinit)
	ON_BN_CLICKED(IDC_BUTTON_PROVEXIT, &CDConsoleDlg::OnBnClickedButtonProvexit)
	ON_BN_CLICKED(IDC_BUTTON_DCINFO, &CDConsoleDlg::OnBnClickedButtonDcinfo)
	ON_BN_CLICKED(IDC_BUTTON_RECFLIST, &CDConsoleDlg::OnBnClickedButtonRecflist)
	ON_BN_CLICKED(IDC_BUTTON_REALPLAY, &CDConsoleDlg::OnBnClickedButtonRealplay)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, &CDConsoleDlg::OnBnClickedButtonStopplay)
	ON_BN_CLICKED(IDC_BUTTON_REVERSEPLAY, &CDConsoleDlg::OnBnClickedButtonReverseplay)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, &CDConsoleDlg::OnBnClickedButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CDConsoleDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CDConsoleDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_MUTE, &CDConsoleDlg::OnBnClickedButtonMute)
	ON_BN_CLICKED(IDC_BUTTON_UNMUTE, &CDConsoleDlg::OnBnClickedButtonUnmute)
	ON_CBN_SELCHANGE(IDC_COMBO_TOOLBAR, &CDConsoleDlg::OnCbnSelchangeComboToolbar)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG, &CDConsoleDlg::OnCbnSelchangeComboLang)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DCGROUPS, &CDConsoleDlg::OnNMRClickTreeDcgroups)
	ON_BN_CLICKED(IDC_BUTTON_TITLETEXT, &CDConsoleDlg::OnBnClickedButtonTitletext)
	ON_BN_CLICKED(IDC_BUTTON_WINPOS, &CDConsoleDlg::OnBnClickedButtonWinpos)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMDblclkListDcusers)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMRClickListDcusers)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickTreeMenuItem)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_EVENT0, &CDConsoleDlg::OnBnClickedButtonEvent0)
	ON_BN_CLICKED(IDC_BUTTON_EVENT1, &CDConsoleDlg::OnBnClickedButtonEvent1)
	ON_BN_CLICKED(IDC_BUTTON_EVENT2, &CDConsoleDlg::OnBnClickedButtonEvent2)
	ON_BN_CLICKED(IDC_BUTTON_EVENT3, &CDConsoleDlg::OnBnClickedButtonEvent3)
	ON_BN_CLICKED(IDC_BUTTON_EVENT4, &CDConsoleDlg::OnBnClickedButtonEvent4)
	ON_BN_CLICKED(IDC_BUTTON_EVENT5, &CDConsoleDlg::OnBnClickedButtonEvent5)
	ON_BN_CLICKED(IDC_BUTTON_EVENT6, &CDConsoleDlg::OnBnClickedButtonEvent6)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_DGNA, &CDConsoleDlg::OnBnClickedButtonCreateDgna)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_TEMPG, &CDConsoleDlg::OnBnClickedButtonCreateTempg)
	ON_BN_CLICKED(IDC_BUTTON_EVENT7, &CDConsoleDlg::OnBnClickedButtonEvent7)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CDConsoleDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_NORMALSCREEN, &CDConsoleDlg::OnBnClickedButtonNormalscreen)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDConsoleDlg::OnItemclickListDcusers)
	ON_MESSAGE(WM_RECEIVED, OnCallMsgReceived)
	ON_BN_CLICKED(IDC_BUTTON_EVENT8, &CDConsoleDlg::OnBnClickedButtonEvent8)
	ON_BN_CLICKED(IDC_BUTTON_EVENT9, &CDConsoleDlg::OnBnClickedButtonEvent9)
	ON_BN_CLICKED(IDC_BUTTON_SDS, &CDConsoleDlg::OnBnClickedButtonSds)
	ON_BN_CLICKED(IDC_BUTTON2, &CDConsoleDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_EVENT11, &CDConsoleDlg::OnBnClickedButtonEvent11)
	ON_BN_CLICKED(IDC_BUTTON_DISPATCHVIDEO, &CDConsoleDlg::OnBnClickedButtonDispatchvideo)
	ON_BN_CLICKED(IDC_BUTTON_UNDISPATCH, &CDConsoleDlg::OnBnClickedButtonUndispatch)
	ON_MESSAGE(WM_VIDEORECEIVED, &CDConsoleDlg::OnVideoMsgReceived)
	ON_MESSAGE(WM_SHOW_COMMING_VIDEO, &CDConsoleDlg::ShowForRecv)
	ON_BN_CLICKED(IDC_BUTTON_VWALL, &CDConsoleDlg::OnBnClickedButtonVwall)
	ON_BN_CLICKED(IDC_BTN_CALLPHONE, &CDConsoleDlg::OnBnClickedBtnDialphone)
	ON_BN_CLICKED(IDC_BTN_HANGUP, &CDConsoleDlg::OnBnClickedBtnHangup)
	ON_BN_CLICKED(IDC_BTN_DISCREETLISTEN, &CDConsoleDlg::OnBnClickedBtnDiscreetlisten)
	ON_BN_CLICKED(IDC_BTN_STOPDISCREENLISTEN, &CDConsoleDlg::OnBnClickedBtnStopdiscreenlisten)
	ON_BN_CLICKED(IDC_BTN_ENVLISTEN, &CDConsoleDlg::OnBnClickedBtnEnvlisten)
	ON_BN_CLICKED(IDC_BTN_P2PTRANSFER, &CDConsoleDlg::OnBnClickedBtnP2ptransfer)
	ON_BN_CLICKED(IDC_BUTTON_CREATEPG, &CDConsoleDlg::OnBnClickedButtonCreatepg)
	ON_BN_CLICKED(IDC_BUTTON_PATCHGROUPS, &CDConsoleDlg::OnBnClickedButtonPatchgroups)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PATCHGROUPS, &CDConsoleDlg::OnRclickTreePatchgroups)
	ON_BN_CLICKED(IDC_BUTTON_GETGISSUB, &CDConsoleDlg::OnBnClickedButtonGetgissub)
END_MESSAGE_MAP()


// CDConsoleDlg message handlers


BOOL CDConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//设置列表框为单选模式
	m_DcUsers.ModifyStyle(0, LVS_SINGLESEL);
	m_DcUsers.InsertColumn(DCUSERS_RESID, _T("ResourceID"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_STATUS, _T("UserStatus"), LVCFMT_LEFT, 100);
	m_DcUsers.InsertColumn(DCUSERS_NAME, _T("UserName"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_CATEGORY, _T("UserCategory"), LVCFMT_LEFT, 85);
	m_DcUsers.InsertColumn(DCUSERS_PRIORITY, _T("UserPriority"), LVCFMT_LEFT, 85);

	//图片列表
	m_ImageList.Create(30, 30, ILC_COLORDDB, 8, 1);
	AddImage(m_ImageList, IDB_BITMAP_CAMERA_OFFLINE);
	AddImage(m_ImageList, IDB_BITMAP_CAMERA_ONLINE);
	AddImage(m_ImageList, IDB_BITMAP_DISPATCHER_OFFLINE);
	AddImage(m_ImageList, IDB_BITMAP_DISPATCHER_ONLINE);
	AddImage(m_ImageList, IDB_BITMAP_GWUSER_OFFLINE);
	AddImage(m_ImageList, IDB_BITMAP_GWUSER_ONLINE);
	AddImage(m_ImageList, IDB_BITMAP_PTTUSER_OFFLINE);
	AddImage(m_ImageList, IDB_BITMAP_PTTUSER_ONLINE);
	m_DcUsers.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_DcGroups.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_ImageList.Detach();

	//设置List风格:全选、子列支持图片
	m_DcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP);

	//设置坐标
	/*GetWindowRect(&m_RectMax);
	m_RectMin = m_RectMax;
	CRect rtSplit;
	GetDlgItem(IDC_STATIC_DCSPLIT)->GetWindowRect(&rtSplit);
	m_RectMin.right = rtSplit.left;
	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);*/

	//设置坐标
	CRect rect;
	GetDlgItem(IDC_STATIC_DC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRect rt(rect);
	rt.left = 10;
	rt.top = 50;
	rt.right -= 10;
	m_DcUsers.MoveWindow(rt);
	m_DcGroups.MoveWindow(rt);
	m_PatchGroups.MoveWindow(rt);
	m_DcUsers.ShowWindow(SW_SHOW);
	m_DcGroups.ShowWindow(SW_HIDE);
	m_PatchGroups.ShowWindow(SW_HIDE);

	// 初始化数据
	CComboBox* pToorBar = (CComboBox*)GetDlgItem(IDC_COMBO_TOOLBAR);
	pToorBar->InsertString(0, _T("隐藏"));
	pToorBar->InsertString(0, _T("显示"));
	pToorBar->SetCurSel(0);
	CComboBox* pLang = (CComboBox*)GetDlgItem(IDC_COMBO_LANG);
	pLang->InsertString(0, _T("西班牙语"));
	pLang->InsertString(0, _T("English"));
	pLang->InsertString(0, _T("中文"));
	pLang->SetCurSel(0);
	SetDlgItemText(IDC_EDIT_TITLE, _T("实时视频"));
	SetDlgItemInt(IDC_EDIT_LEFT, 0);
	SetDlgItemInt(IDC_EDIT_TOP, 0);
	SetDlgItemInt(IDC_EDIT_WIDTH, 450);
	SetDlgItemInt(IDC_EDIT_HEIGHT, 400);
	GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDConsoleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedButtonProvexit();
	ClearDcGroupsTree();

	for (size_t i=0; i<m_AllOcxList.size(); i++)
	{
		CElte_playerctrl1* pOcx = m_AllOcxList[i];
		if (m_peLTE_Player == pOcx)
		{
			continue;
		}
		pOcx->ELTE_OCX_UnLoad();
	}

	m_AllOcxList.clear();
	m_ActiveOcxList.clear();
	m_FreeOcxList.clear();

	if (m_pCeLTE_PlayerDemoDlg && m_pCeLTE_PlayerDemoDlg->GetSafeHwnd())
	{
		m_pCeLTE_PlayerDemoDlg->PostMessage(WM_CLOSE);
	}

	CDialog::OnClose();
}


void CDConsoleDlg::OnBnClickedButtonDcusers()
{
	// TODO: Add your control notification handler code here

	m_DcUsers.ShowWindow(SW_SHOW);
	m_DcGroups.ShowWindow(SW_HIDE);
	m_PatchGroups.ShowWindow(SW_HIDE);
}


void CDConsoleDlg::OnBnClickedButtonDcgroups()
{
	// TODO: Add your control notification handler code here

	m_DcUsers.ShowWindow(SW_HIDE);
	m_DcGroups.ShowWindow(SW_SHOW);
	m_PatchGroups.ShowWindow(SW_HIDE);
}


void CDConsoleDlg::OnBnClickedButtonProvinit()
{
	// TODO: Add your control notification handler code here
	//CString strResult = m_peLTE_Player->ELTE_OCX_ProvisionManagerInit(m_strServerIP, m_strUserID);
	//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ProvisionManagerInit"));

	//// 初始化MRS
	//
	//{
	//	strResult = m_peLTE_Player->ELTE_OCX_ProvisionManagerInitMRS(((CeLTE_PlayerDemoDlg*)m_pCeLTE_PlayerDemoDlg)->GetMRSIP());
	//	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ProvisionManagerInitMRS"));
	//}
	//GetDlgItem(IDC_BUTTON_PROVINIT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(TRUE);
}


void CDConsoleDlg::OnBnClickedButtonProvexit()
{
	// TODO: Add your control notification handler code here
	//if (GetDlgItem(IDC_BUTTON_PROVEXIT)->IsWindowEnabled())
	//{
	//	CString strResult = m_peLTE_Player->ELTE_OCX_ProvisionManagerExit();
	//	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ProvisionManagerExit"));

	//	// 清理
	//	ClearProv();

	//	GetDlgItem(IDC_BUTTON_PROVINIT)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(FALSE);
	//}
}


void CDConsoleDlg::OnBnClickedButtonDcinfo()
{
	// TODO: Add your control notification handler code here
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcInfo(m_strUserID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcInfo"));

	DcInfo dcInfo;
	CXml::Instance().XmlParseDcInfo(strResult, dcInfo);

	CInfoDlg dlg(this);
	dlg.SetInfo(1, _T("DcID"), dcInfo.DcID);
	dlg.SetInfo(2, _T("Priviledge"), dcInfo.Priviledge);
	dlg.SetInfo(3, _T("Role"), dcInfo.Role);
	dlg.SetInfo(4, _T("Alias"), dcInfo.Alias);
	dlg.DoModal();
}


void CDConsoleDlg::OnBnClickedButtonRecflist()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_RECFileDlg.m_hWnd)
	{
		m_RECFileDlg.m_peLTE_Player = m_peLTE_Player;
		m_RECFileDlg.Create(CRECFileDlg::IDD, this);
	}
	m_RECFileDlg.ShowWindow(SW_SHOW);
}


void CDConsoleDlg::OnBnClickedButtonCreateDgna()
{
	// TODO: Add your control notification handler code here
	DGNAParamInfo info;
	CDGNAParam dlg(this);
	dlg.SetDGNAParamInfo(m_strUserID, &info, true);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// 构造XML
	CString strDGNAParam;
	strDGNAParam.Append(_T("<Content>"));
	strDGNAParam.Append(_T("<GroupID>"));
	strDGNAParam.Append(info.GroupID);
	strDGNAParam.Append(_T("</GroupID>"));
	strDGNAParam.Append(_T("<DcID>"));
	strDGNAParam.Append(info.DcID);
	strDGNAParam.Append(_T("</DcID>"));
	strDGNAParam.Append(_T("<Alias>"));
	strDGNAParam.Append(info.Alias);
	strDGNAParam.Append(_T("</Alias>"));
	strDGNAParam.Append(_T("<Priority>"));
	strDGNAParam.Append(info.Priority);
	strDGNAParam.Append(_T("</Priority>"));
	strDGNAParam.Append(_T("<MaxPeriod>"));
	strDGNAParam.Append(info.MaxPeriod);
	strDGNAParam.Append(_T("</MaxPeriod>"));
	strDGNAParam.Append(_T("<GroupList>"));
	for (int i=0; i<8; i++)
	{
		if (info.GroupList[i].IsEmpty())
			break;
		strDGNAParam.Append(_T("<GroupID>"));
		strDGNAParam.Append(info.GroupList[i]);
		strDGNAParam.Append(_T("</GroupID>"));
	}
	strDGNAParam.Append(_T("</GroupList>"));
	strDGNAParam.Append(_T("<UserList>"));
	for (int i=0; i<200; i++)
	{
		if (info.UserList[i].IsEmpty())
			break;
		strDGNAParam.Append(_T("<UserID>"));
		strDGNAParam.Append(info.UserList[i]);
		strDGNAParam.Append(_T("</UserID>"));
	}
	strDGNAParam.Append(_T("</UserList>"));
	strDGNAParam.Append(_T("</Content>"));

	CString strResult = m_peLTE_Player->ELTE_OCX_CreateDynamicGroup(strDGNAParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_CreateDynamicGroup"));
}


void CDConsoleDlg::OnBnClickedButtonCreateTempg()
{
	// TODO: Add your control notification handler code here
	DGNAParamInfo info;
	CDGNAParam dlg(this);
	dlg.SetDGNAParamInfo(m_strUserID, &info, false);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	CString strResult = m_peLTE_Player->ELTE_OCX_GetNextTempGroupID();
	CString strGroupID = GET_XML_ELEM_VALUE_STR(strResult, _T("NextTempGroupID"));

	// 构造XML
	CString strDGNAParam;
	strDGNAParam.Append(_T("<Content>"));
	strDGNAParam.Append(_T("<GroupID>"));
	strDGNAParam.Append(strGroupID);
	strDGNAParam.Append(_T("</GroupID>"));
	strDGNAParam.Append(_T("<DcID>"));
	strDGNAParam.Append(info.DcID);
	strDGNAParam.Append(_T("</DcID>"));
	strDGNAParam.Append(_T("<Alias>"));
	strDGNAParam.Append(info.Alias);
	strDGNAParam.Append(_T("</Alias>"));
	strDGNAParam.Append(_T("<Priority>"));
	strDGNAParam.Append(info.Priority);
	strDGNAParam.Append(_T("</Priority>"));
	strDGNAParam.Append(_T("<MaxPeriod>"));
	strDGNAParam.Append(info.MaxPeriod);
	strDGNAParam.Append(_T("</MaxPeriod>"));
	strDGNAParam.Append(_T("<GroupList>"));
	for (int i=0; i<8; i++)
	{
		if (info.GroupList[i].IsEmpty())
			break;
		strDGNAParam.Append(_T("<GroupID>"));
		strDGNAParam.Append(info.GroupList[i]);
		strDGNAParam.Append(_T("</GroupID>"));
	}
	strDGNAParam.Append(_T("</GroupList>"));
	strDGNAParam.Append(_T("<UserList>"));
	for (int i=0; i<200; i++)
	{
		if (info.UserList[i].IsEmpty())
			break;
		strDGNAParam.Append(_T("<UserID>"));
		strDGNAParam.Append(info.UserList[i]);
		strDGNAParam.Append(_T("</UserID>"));
	}
	strDGNAParam.Append(_T("</UserList>"));
	strDGNAParam.Append(_T("</Content>"));

	strResult = m_peLTE_Player->ELTE_OCX_CreateTempGroup(strDGNAParam);
	int iRet = GET_XML_RESULTE_CODE(strResult);
	if (0 == iRet)
	{
		AddTempGroup(strGroupID);
	}
	else
	{
		CString str;
		str.Format(_T("ELTE_OCX_CreateTempGroup Failed. (%d)"), iRet);
		MessageBox(str);
	}
}


void CDConsoleDlg::OnBnClickedButtonRealplay()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();

	VideoParamInfo info;
	CVideoParamDlg dlg(this);
	dlg.SetVideoParamInfo(&info);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// 构造参数
	CString strVideoParam;
	strVideoParam.Append(_T("<Content>"));
	strVideoParam.Append(_T("<VideoParam>"));
	strVideoParam.Append(_T("<VideoFormat>"));
	strVideoParam.Append(info.strFormat);
	strVideoParam.Append(_T("</VideoFormat>"));
	strVideoParam.Append(_T("<CameraType>"));
	strVideoParam.Append(info.strCamera);
	strVideoParam.Append(_T("</CameraType>"));
	strVideoParam.Append(_T("<UserConfirmType>"));
	strVideoParam.Append(info.strConfirm);
	strVideoParam.Append(_T("</UserConfirmType>"));
	strVideoParam.Append(_T("<MuteType>"));
	strVideoParam.Append(info.strMute);
	strVideoParam.Append(_T("</MuteType>"));
	strVideoParam.Append(_T("</VideoParam>"));
	strVideoParam.Append(_T("</Content>"));

	CString strResult = m_peLTE_Player->ELTE_OCX_StartRealPlay(strResID, strVideoParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StartRealPlay"));

	CElte_playerctrl1* pOcx = NULL;
	if (!m_FreeOcxList.empty())
	{
		std::vector<CElte_playerctrl1*>::iterator itor = m_FreeOcxList.begin();
		pOcx = *itor;
		m_ActiveOcxList.push_back(pOcx);
		m_FreeOcxList.erase(itor);
	}
	else
	{
		pOcx = new CElte_playerctrl1;
		CRect rect;
		m_peLTE_Player->GetClientRect(&rect);
		pOcx->Create(_T(""),WS_CHILD,rect,m_pCeLTE_PlayerDemoDlg,OCX_ID_BEGIN+m_AllOcxList.size());
		m_AllOcxList.push_back(pOcx);
		m_ActiveOcxList.push_back(pOcx);

		strResult = pOcx->ELTE_OCX_Load(eLTE_LOAD_MEDIA_PLAYER);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_Load"));
	}

	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
	if (m_ResourceMap.end() == itor)
	{
		ST_REALPLAY_PARAM param;
		param.uiState |= STATE_ISPLAY;
		param.pOCX = pOcx;
		param.videoParam = info;
		m_ResourceMap.insert(std::make_pair(StrToInt(strResID), param));
	}
	else
	{
		ST_REALPLAY_PARAM& param = itor->second;
		param.uiState = STATE_ISPLAY;
		param.pOCX = pOcx;
		param.videoParam = info;
	}

	// 按钮状态
	GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VWALL)->EnableWindow(TRUE);
}


void CDConsoleDlg::OnBnClickedButtonStopplay()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();

	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
	if (m_ResourceMap.end() == itor)
	{
		MessageBox(_T("Not found the resource."));
		return;
	}

	ST_REALPLAY_PARAM& param = itor->second;
	if (std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), param.pOCX) == m_ActiveOcxList.end())
	{
		MessageBox(_T("The resource is not active."));
		return;
	}

	if (!(param.uiState & STATE_ISPLAY))
	{
		MessageBox(_T("The resource is not realplay."));
		return;
	}

	CString strResult = m_peLTE_Player->ELTE_OCX_StopRealPlay(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopRealPlay"));

	// 释放资源
	CElte_playerctrl1* pOcx = param.pOCX;
	param.Reset();
	m_ResourceMap.erase(itor);

	std::vector<CElte_playerctrl1*>::iterator it = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
	if (it != m_ActiveOcxList.end())
	{
		m_ActiveOcxList.erase(it);
		m_FreeOcxList.push_back(pOcx);
	}

	// 按钮状态
	GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VWALL)->EnableWindow(FALSE);
}


void CDConsoleDlg::OnBnClickedButtonReverseplay()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();

	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
	if (m_ResourceMap.end() == itor)
	{
		MessageBox(_T("Not found the resource."));
		return;
	}

	ST_REALPLAY_PARAM& param = itor->second;
	if (std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), param.pOCX) == m_ActiveOcxList.end())
	{
		MessageBox(_T("The resource is not active."));
		return;
	}

	if (!(param.uiState & STATE_ISPLAY))
	{
		MessageBox(_T("The resource is not realplay."));
		return;
	}

	param.videoParam.strCamera = param.videoParam.strCamera == "0" ? "1" : "0";

	// 构造参数
	CString strVideoParam;
	strVideoParam.Append(_T("<Content>"));
	strVideoParam.Append(_T("<VideoParam>"));
	strVideoParam.Append(_T("<VideoFormat>"));
	strVideoParam.Append(param.videoParam.strFormat);
	strVideoParam.Append(_T("</VideoFormat>"));
	strVideoParam.Append(_T("<CameraType>"));
	strVideoParam.Append(param.videoParam.strCamera);
	strVideoParam.Append(_T("</CameraType>"));
	strVideoParam.Append(_T("<UserConfirmType>"));
	strVideoParam.Append(param.videoParam.strConfirm);
	strVideoParam.Append(_T("</UserConfirmType>"));
	strVideoParam.Append(_T("<MuteType>"));
	strVideoParam.Append(param.videoParam.strMute);
	strVideoParam.Append(_T("</MuteType>"));
	strVideoParam.Append(_T("</VideoParam>"));
	strVideoParam.Append(_T("</Content>"));

	CString strResult = m_peLTE_Player->ELTE_OCX_ReverseRealPlay(strResID, strVideoParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ReverseRealPlay"));

	if(CeLTE_PlayerDemoDlg::m_iBypass)
	{
		param.uiState &= ~STATE_ISSHOW;
		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
}


void CDConsoleDlg::OnBnClickedButtonSnapshot()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	CString strResult = param.pOCX->ELTE_OCX_Snapshot(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_Snapshot"));
}


void CDConsoleDlg::OnBnClickedButtonShow()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	//参数构造
	CString strLocalParam;
	strLocalParam.Append(_T("<Content>"));
	strLocalParam.Append(_T("<LocalMediaAddr>"));
	strLocalParam.Append(_T("<LocalIP>"));
	strLocalParam.Append(param.strLocalIP);
	strLocalParam.Append(_T("</LocalIP>"));
	strLocalParam.Append(_T("<VideoPort>"));
	strLocalParam.Append(UIntToStr(param.uiLocalVideoPort));
	strLocalParam.Append(_T("</VideoPort>"));
	strLocalParam.Append(_T("<AudioPort>"));
	strLocalParam.Append(UIntToStr(param.uiLocalAudioPort));
	strLocalParam.Append(_T("</AudioPort>"));
	strLocalParam.Append(_T("</LocalMediaAddr>"));
	strLocalParam.Append(_T("</Content>"));

	CString strRemoteParam;
	strRemoteParam.Append(_T("<Content>"));
	strRemoteParam.Append(_T("<RemoteMediaAddr>"));
	strRemoteParam.Append(_T("<RemoteIP>"));
	strRemoteParam.Append(param.strRemoteIP);
	strRemoteParam.Append(_T("</RemoteIP>"));
	strRemoteParam.Append(_T("<VideoPort>"));
	strRemoteParam.Append(UIntToStr(param.uiRemoteVideoPort));
	strRemoteParam.Append(_T("</VideoPort>"));
	strRemoteParam.Append(_T("<AudioPort>"));
	strRemoteParam.Append(UIntToStr(param.uiRemoteAudioPort));
	strRemoteParam.Append(_T("</AudioPort>"));
	strRemoteParam.Append(_T("</RemoteMediaAddr>"));
	strRemoteParam.Append(_T("</Content>"));
	
	CString strResult = param.pOCX->ELTE_OCX_ShowRealPlay(strResID, strLocalParam, strRemoteParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ShowRealPlay"));

	param.uiState |= STATE_ISSHOW;

	GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(TRUE);
}




void CDConsoleDlg::OnBnClickedButtonHide()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	CString strResult = param.pOCX->ELTE_OCX_HideRealPlay();
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_HideRealPlay"));

	param.uiState &= ~STATE_ISSHOW;

	GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
}


void CDConsoleDlg::OnBnClickedButtonMute()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	CString strResult; 
	if(CeLTE_PlayerDemoDlg::m_iBypass)
	{
		strResult = param.pOCX->ELTE_OCX_StopSound();
		CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopSound"));
	}
	else
	{
		CString strMuteParam;
		strMuteParam.Append(_T("<Content>"));
		strMuteParam.Append(_T("<MuteParam>"));

		strMuteParam.Append(_T("<CallType>"));
		strMuteParam.Append(_T("2"));
		strMuteParam.Append(_T("</CallType>"));
		strMuteParam.Append(_T("</MuteParam>"));
		strMuteParam.Append(_T("</Content>"));
		strResult = param.pOCX->ELTE_OCX_VolMute(strResID, strMuteParam);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_VolMute"));
	}
	
	param.uiState &= ~STATE_ISUNMUTE;

	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
}

void CDConsoleDlg::DoMute(const int& resID)
{
	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(resID);
	if (m_ResourceMap.end() == itor)															
	{																							
		MessageBox(_T("Not found the resource."));												
		return;																				
	}																							
	ST_REALPLAY_PARAM& param = itor->second;													
	if (NULL == param.pOCX)																		
	{																							
		MessageBox(_T("Ocx is null."));															
		return;																					
	}
	CString strResId;
	strResId.Format(L"%d", resID);
	SetDlgItemText(IDC_EDIT_RESID, strResId);
	CString strResult; 
	CString strMuteParam;
	strMuteParam.Append(_T("<Content>"));
	strMuteParam.Append(_T("<MuteParam>"));
	strMuteParam.Append(_T("<CallType>"));
	strMuteParam.Append(_T("2"));
	strMuteParam.Append(_T("</CallType>"));
	strMuteParam.Append(_T("</MuteParam>"));
	strMuteParam.Append(_T("</Content>"));
	strResult = param.pOCX->ELTE_OCX_VolMute(strResId, strMuteParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_VolMute"));
	param.uiState &= ~STATE_ISUNMUTE;

	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
}

void CDConsoleDlg::OnBnClickedButtonUnmute()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();
	CString strResult;
	if(CeLTE_PlayerDemoDlg::m_iBypass)
	{
		strResult = param.pOCX->ELTE_OCX_PlaySound();
		CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PlaySound"));
	}
	else
	{
		CString strUnmuteParam;
		strUnmuteParam.Append(_T("<Content>"));
		strUnmuteParam.Append(_T("<MuteParam>"));
		strUnmuteParam.Append(_T("<TaskType>"));
		strUnmuteParam.Append(_T("false"));
		strUnmuteParam.Append(_T("</TaskType>"));
		strUnmuteParam.Append(_T("<CallType>"));
		strUnmuteParam.Append(_T("2"));
		strUnmuteParam.Append(_T("</CallType>"));
		strUnmuteParam.Append(_T("</MuteParam>"));
		strUnmuteParam.Append(_T("</Content>"));
		strResult = param.pOCX->ELTE_OCX_VolUnMute(strResID, strUnmuteParam);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_VolUnMute"));
	}

	param.uiState |= STATE_ISUNMUTE;

	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(TRUE);
}


void CDConsoleDlg::OnBnClickedButtonTitletext()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	CString strText;
	GetDlgItemText(IDC_EDIT_TITLE, strText);
	CString strResult = param.pOCX->ELTE_OCX_SetTitleText(strText);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetTitleText"));
}


void CDConsoleDlg::OnBnClickedButtonWinpos()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	int iLeft = GetDlgItemInt(IDC_EDIT_LEFT);
	int iTop = GetDlgItemInt(IDC_EDIT_TOP);
	int iWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
	int iHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);

	CString strResult = param.pOCX->ELTE_OCX_SetVideoWindowPos(iLeft, iTop, iWidth, iHeight);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetVideoWindowPos"));
}


void CDConsoleDlg::OnBnClickedButtonFullscreen()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	int iLeft = GetDlgItemInt(IDC_EDIT_LEFT);
	int iTop = GetDlgItemInt(IDC_EDIT_TOP);
	int iWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
	int iHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);

	CString strResult = param.pOCX->ELTE_OCX_FullScreenDisplay();
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_FullScreenDisplay"));
}


void CDConsoleDlg::OnBnClickedButtonNormalscreen()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	int iLeft = GetDlgItemInt(IDC_EDIT_LEFT);
	int iTop = GetDlgItemInt(IDC_EDIT_TOP);
	int iWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
	int iHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);

	CString strResult = param.pOCX->ELTE_OCX_NormalScreenDisplay();
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_NormalScreenDisplay"));
}


void CDConsoleDlg::OnCbnSelchangeComboToolbar()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_TOOLBAR))->GetCurSel();
	ULONG ulTooBar = 0 == iSel ? 1 : 0;
	CString strResult = param.pOCX->ELTE_OCX_ShowToolbar(ulTooBar);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ShowToolbar"));
}


void CDConsoleDlg::OnCbnSelchangeComboLang()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE_EX();

	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_LANG))->GetCurSel();
	ULONG m_ulLang = iSel + 1;
	CString strResult = param.pOCX->ELTE_OCX_SetLanguage(m_ulLang);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetLanguage"));
}


void CDConsoleDlg::OnNMRClickTreeDcgroups(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint pt, pmenu;
	::GetCursorPos(&pt);
	::GetCursorPos(&pmenu);
	m_DcGroups.ScreenToClient(&pt);

	UINT uFlag = 0;
	HTREEITEM hSelectedItem = m_DcGroups.HitTest(pt,&uFlag);
	if (NULL == hSelectedItem || !(TVHT_ONITEM & uFlag))
	{
		return;
	}

	m_DcGroups.SelectItem(hSelectedItem);

	if (m_DcGroups.GetParentItem(hSelectedItem) == NULL)
	{
		GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(hSelectedItem);
		//用户节点
		CMenu menu;
		menu.CreatePopupMenu();	
		menu.AppendMenu(MF_STRING, ID_MENU_DCGROUPINFO, _T("Info"));
		//menu.AppendMenu(MF_STRING, ID_MENU_SUB_DCGROUP, _T("Sub"));
		if (pInfo->GroupCategory != "-1")
		{
			menu.AppendMenu(MF_STRING, ID_MENU_UNSUB_DCGROUP, _T("UnSub"));
			menu.AppendMenu(MF_STRING, ID_MENU_SUB_JOIN_DCGROUP, _T("SubJoin"));
			menu.AppendMenu(MF_STRING, ID_MENU_JOINCALL, _T("JoinCall"));
		}		
		
		
		if (!CeLTE_PlayerDemoDlg::m_iBypass)
		{
			CMenu pttMenu;
			pttMenu.CreatePopupMenu();
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTDIAL, _T("Dial"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTRELEASE, _T("Release"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTHANGUP, _T("Hangup"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTEMERGENCY, _T("Emergency"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_GROUPBREAKOFF, _T("Breakoff"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTVOLMUTE, _T("Mute"));
			pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTVOLUNMUTE, _T("UnMute"));
			//menu.EnableMenuItem(ID_MENU_DCPTTRELEASE, MF_GRAYED);
			//menu.EnableMenuItem(ID_MENU_DCPTTHANGUP, MF_GRAYED);
			menu.AppendMenu(MF_POPUP,(UINT)pttMenu.m_hMenu, _T("PTT"));//
		}
	
		/*menu.AppendMenu(MF_STRING, ID_MENU_JOIN_DCGROUP, _T("JoinGroup"));*/
		
		//临时组
		if (pInfo->GroupCategory == "-1")
		{	
			menu.EnableMenuItem(ID_MENU_DCPTTEMERGENCY, MF_GRAYED);
			menu.EnableMenuItem(ID_MENU_DCGROUPINFO, MF_GRAYED);
		}

		//动态组
		if (pInfo->GroupCategory == "9")
		{
			menu.AppendMenu(MF_STRING, ID_MENU_RM_DGNA, _T("Remove"));
			menu.AppendMenu(MF_STRING, ID_MENU_MODIFYDGROUP, _T("Modify"));
		}

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
	else
	{
		//用户节点
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MENU_DCGROUPUSERINFO, _T("Info"));

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
}

//双击事件
void CDConsoleDlg::OnNMDblclkListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (-1 == nItem)
	{
		return;
	}

	POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
	int item = m_DcUsers.GetNextSelectedItem(pos);
	CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

	// 判断是否为摄像头
	CString strResult = m_peLTE_Player->ELTE_OCX_GetUserInfo(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetUserInfo"));
	CString strCategory = GET_XML_ELEM_VALUE_STR(strResult, _T("UserCategory"));
	int iUserCategory = StrToInt(strCategory);
	if (1 != iUserCategory && 9 != iUserCategory)
	{
		SetDlgItemText(IDC_EDIT_RESID, _T(""));
		GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VWALL)->EnableWindow(FALSE);
		return;
	}

	SetDlgItemText(IDC_EDIT_RESID, strResID);
	SetDlgItemText(IDC_EDIT_RESOURCEID, strResID);
	SetDlgItemText(IDC_EDIT_TITLE, _T("实时视频 ")+strResID);

	// 设置资源对应状态
	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
	if (m_ResourceMap.end() == itor)
	{
		// 按钮状态
		GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VWALL)->EnableWindow(FALSE);
	}
	else
	{
		ST_REALPLAY_PARAM& param = itor->second;
		// 按钮状态
		if (param.uiState & STATE_ISPLAY)
		{
			GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(FALSE);
			if (!param.uiVideoType)
			{
				GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(TRUE);
			}			
			GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(TRUE);
			if (param.uiState & STATE_ISSHOW)
			{
				GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(TRUE);
				if (param.uiState & STATE_ISUNMUTE)
				{
					GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
				}
				else 
				{
					GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
				}
			}
			else
			{
				GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_VWALL)->EnableWindow(FALSE);
		}
	}
}


void CDConsoleDlg::OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem = pNMListView->iItem;
	if (-1 == nItem)
	{
		return;
	}

	//弹出菜单
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_MENU_DCUSERINFO, _T("Info"));
	CString sUserType = m_DcUsers.GetItemText(nItem, DCUSERS_CATEGORY);
	if(!CeLTE_PlayerDemoDlg::m_iBypass)
	{
		CMenu p2pMenu;
		p2pMenu.CreatePopupMenu();//
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PTRANSFER, _T("Transfer"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PDIAL, _T("Dial"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PHANGUP, _T("Hangup"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PBREAKIN, _T("Breakin"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PBREAKOFF, _T("Breakoff"));

		

		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PVOLMUTE, _T("Mute"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_DCP2PUNVOLMUTE, _T("UnMute"));

		p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PHALFDPXDIAL, _T("HalfDpxDial"));
		p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PHALFDPXRELEASE, _T("HalfDpxRelease"));

		CString sStatus = m_DcUsers.GetItemText(nItem, DCUSERS_STATUS);
		if(_T("1") != sUserType && _T("2") != sUserType)
		{
			menu.AppendMenu(MF_POPUP, (UINT)p2pMenu.m_hMenu, _T("P2P"));
		}

		if (_T("1") == sUserType)
		{
			menu.AppendMenu(MF_STRING, ID_MENU_PTZCONTROL, _T("PTZControl"));
		}

		if (sStatus == _T("offline"))
		{
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PTRANSFER, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PDIAL, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PHANGUP, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PBREAKIN, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PBREAKOFF, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PVOLMUTE, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_DCP2PUNVOLMUTE, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_P2PHALFDPXDIAL, MF_GRAYED);
			p2pMenu.EnableMenuItem(ID_MENU_P2PHALFDPXRELEASE, MF_GRAYED);
		}
	}

	if (sUserType == _T("9"))
	{
		CMenu gisMenu;
		gisMenu.CreatePopupMenu();//
		gisMenu.AppendMenu(MF_STRING, ID_MENU_DCGISSUBSCRIBE, _T("Sub"));
		gisMenu.AppendMenu(MF_STRING, ID_MENU_DCGISUNSUBSCRIBE, _T("UnSub"));
		//gisMenu.AppendMenu(MF_STRING, ID_MENU_GETGISCFG, _T("GetCfg"));
		//gisMenu.AppendMenu(MF_STRING, ID_MENU_SETGISCFG, _T("SetCfg"));
		menu.AppendMenu(MF_POPUP, (UINT)gisMenu.m_hMenu, _T("GIS"));
		
	}

	CPoint pmenu;
	::GetCursorPos(&pmenu);
	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
}


void CDConsoleDlg::OnClickTreeMenuItem(UINT uID)
{
	switch (uID)
	{
	case ID_MENU_DCUSERINFO:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_GetUserInfo(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetUserInfo"));

			CString strUserID = GET_XML_ELEM_VALUE_STR(strResult, _T("UserID"));
			CString strUserCategory = GET_XML_ELEM_VALUE_STR(strResult, _T("UserCategory"));
			CString strUserPriority = GET_XML_ELEM_VALUE_STR(strResult, _T("UserPriority"));
			CString strUserName = GET_XML_ELEM_VALUE_STR(strResult, _T("UserName"));

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("UserID"), strUserID);
			dlg.SetInfo(2, _T("UserCategory"), strUserCategory);
			dlg.SetInfo(3, _T("UserPriority"), strUserPriority);
			dlg.SetInfo(4, _T("UserName"), strUserName);
			dlg.DoModal();
		}
		break;
	case ID_MENU_DCP2PDIAL:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_P2PDial(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PDial"));
		}
		break;
	case ID_MENU_P2PHALFDPXDIAL:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_P2PHalfDpxDial(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PHalfDpxDial"));
		}
		break;
	case ID_MENU_P2PHALFDPXRELEASE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_P2PHalfDpxRelease(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PHalfDpxRelease"));
		}
		break;
	case ID_MENU_DCP2PTRANSFER:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			P2PTransferDlg dlg(NULL);
			dlg.SetParamInfo(m_strUserID, strResID);
			dlg.SetELtePlayer(m_peLTE_Player);
			dlg.DoModal();
		}
		break;
	case ID_MENU_DCP2PBREAKIN:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_P2PBreakin(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PBreakin"));
		}
		break;
	case ID_MENU_DCP2PBREAKOFF:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			CString strStatus = m_DcUsers.GetItemText(item, DCUSERS_STATUS);
			//由于demo,关于调度台用户没有状态变更,所以该条件判断会使强拆调度台点呼无效。
			//if(_T("使用中") == strStatus)
			//{
				CString strResult = m_peLTE_Player->ELTE_OCX_P2PBreakoff(strResID);
				CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PBreakoff"));
			//}
		}
		break;
	case ID_MENU_GETGISCFG:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult;
			//CString strResult = m_peLTE_Player->ELTE_OCX_GetUserSpecificGISCfg(strResID);
			//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetUserSpecificGISCfg"));

			CString strPeriod = GET_XML_ELEM_VALUE_STR(strResult, _T("ReportPeriod"));
			CString strDistance = GET_XML_ELEM_VALUE_STR(strResult, _T("ReportDistance"));

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("Period"), strPeriod);
			dlg.SetInfo(2, _T("Distance"), strDistance);
			dlg.DoModal();
		}
		break;
	case  ID_MENU_SETGISCFG:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CSetGisCfgDlg dlg(NULL);
			dlg.SetParamInfo(strResID);
			dlg.SetELtePlayer(m_peLTE_Player);
			dlg.DoModal();
		}
		break;
	case ID_MENU_DCGISSUBSCRIBE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			CString strGisParam;
			strGisParam.Append(_T("<Content>"));
			strGisParam.Append(_T("<GISParam>"));
			strGisParam.Append(_T("<SubType>"));
			strGisParam.Append(_T("7"));
			strGisParam.Append(_T("</SubType>"));
			strGisParam.Append(_T("<ResourceList>"));
			strGisParam.Append(strResID);
			strGisParam.Append(_T("</ResourceList>"));
			strGisParam.Append(_T("<Subscriber>"));
			strGisParam.Append(_T("</Subscriber>"));
			strGisParam.Append(_T("</GISParam>"));
			strGisParam.Append(_T("</Content>"));

			CString strResult = m_peLTE_Player->ELTE_OCX_GISSubscribe(_T("0"), strGisParam);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GISSubscribe"));
		}
		break;
	case ID_MENU_DCGISUNSUBSCRIBE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			CString strGisParam;
			strGisParam.Append(_T("<Content>"));
			strGisParam.Append(_T("<GISParam>"));
			strGisParam.Append(_T("<SubType>"));
			strGisParam.Append(_T("8"));
			strGisParam.Append(_T("</SubType>"));
			strGisParam.Append(_T("<ResourceList>"));
			strGisParam.Append(strResID);
			strGisParam.Append(_T("</ResourceList>"));
			strGisParam.Append(_T("<Subscriber>"));
			strGisParam.Append(_T("</Subscriber>"));
			strGisParam.Append(_T("</GISParam>"));
			strGisParam.Append(_T("</Content>"));

			CString strResult = m_peLTE_Player->ELTE_OCX_GISSubscribe(_T("0"), strGisParam);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GISSubscribe"));
		}
		break;
	case ID_MENU_DCP2PVOLMUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			CString strStatus = m_DcUsers.GetItemText(item, DCUSERS_STATUS);
			if(_T("使用中") == strStatus)
			{
				CString strMuteParam;
				strMuteParam.Append(_T("<Content>"));
				strMuteParam.Append(_T("<MuteParam>"));
				strMuteParam.Append(_T("<CallType>"));
				strMuteParam.Append(_T("0"));
				strMuteParam.Append(_T("</CallType>"));
				strMuteParam.Append(_T("</MuteParam>"));
				strMuteParam.Append(_T("</Content>"));
				CString strResult = m_peLTE_Player->ELTE_OCX_VolMute(strResID, strMuteParam);
				CHECK_RESULTE_CODE(strResult, _T("P2P ELTE_OCX_VolMute"));
			}
		}
		break;
	case ID_MENU_DCP2PUNVOLMUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			CString strStatus = m_DcUsers.GetItemText(item, DCUSERS_STATUS);
			if(_T("使用中") == strStatus)
			{
				CString strMuteParam;
				strMuteParam.Append(_T("<Content>"));
				strMuteParam.Append(_T("<MuteParam>"));
				strMuteParam.Append(_T("<CallType>"));
				strMuteParam.Append(_T("0"));
				strMuteParam.Append(_T("</CallType>"));
				strMuteParam.Append(_T("</MuteParam>"));
				strMuteParam.Append(_T("</Content>"));
				CString strResult = m_peLTE_Player->ELTE_OCX_VolUnMute(strResID, strMuteParam);
				CHECK_RESULTE_CODE(strResult, _T("P2P ELTE_OCX_VolUnMute"));
			}
		}
		break;
	case ID_MENU_PTZCONTROL:
		{
			unsigned uiCode, uiValue;
			CPTZCtrlDlg dlg;
			dlg.SetParam(&uiCode, &uiValue);
			int iExit = dlg.DoModal();
			if (IDOK != iExit)
			{
				return;
			}

			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_PTZControl(strResID, uiCode, uiValue);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PTZControl"));
		}
		break;
	case ID_MENU_DCP2PHANGUP:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strResult = m_peLTE_Player->ELTE_OCX_P2PHangup(strResID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PHangup"));
		}
		break;
	case ID_MENU_DCGROUPINFO:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			CString strResult = m_peLTE_Player->ELTE_OCX_GetGroupInfo(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetGroupInfo"));

			CString strGroupID = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupID"));
			CString strGroupCategory = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupCategory"));
			CString strGroupPriority = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupPriority"));
			CString strGroupName = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupName"));
			CString strGroupCreator = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupCreator"));


			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("GroupID"), strGroupID);
			if(_T("9") == strGroupCategory)
			{
				dlg.SetInfo(2, _T("GroupCreator"), strGroupCreator);
			}
			else
			{
				dlg.SetInfo(2, _T("GroupCategory"), strGroupCategory);
			}
			dlg.SetInfo(3, _T("GroupPriority"), strGroupPriority);
			dlg.SetInfo(4, _T("GroupName"), strGroupName);
			dlg.DoModal();
		}
		break;
	case ID_MENU_SUB_DCGROUP:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			//CString strResult = m_peLTE_Player->ELTE_OCX_SubscribeGroup(pInfo->GroupID, 1);
			//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SubscribeGroup"));
		}
		break;
	case ID_MENU_UNSUB_DCGROUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player->ELTE_OCX_UnSubscribeGroup(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_UnSubscribeGroup"));
		}
		break;
	case ID_MENU_DCPTTDIAL:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player->ELTE_OCX_PTTDial(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PTTDial"));
		}
		break;
	case ID_MENU_GETPATCHGROUPINFO:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);

			CString strResult = m_peLTE_Player->ELTE_OCX_GetPatchGroupInfo(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetPatchGroupInfo"));

			CString strGroupID = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupNumber"));
			CString strSetupDcId = GET_XML_ELEM_VALUE_STR(strResult, _T("SetupDcId"));
			CString strGroupPriority = GET_XML_ELEM_VALUE_STR(strResult, _T("PGPriority"));
			CString strGroupName = GET_XML_ELEM_VALUE_STR(strResult, _T("PGName"));

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("GroupID"), strGroupID);
			dlg.SetInfo(2, _T("GroupCreator"), strSetupDcId);
			dlg.SetInfo(3, _T("GroupPriority"), strGroupPriority);
			dlg.SetInfo(4, _T("GroupName"), strGroupName);
			dlg.DoModal();
		}
		break;
	case ID_MENU_DCPTTHANGUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);			

			CString strResult = m_peLTE_Player->ELTE_OCX_PTTHangup(pInfo->GroupID);
			//remove Item from group list
			if ("-1" == pInfo->GroupCategory)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PTTHangup"));
		}
		break;
	case ID_MENU_DCPTTEMERGENCY:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player->ELTE_OCX_PTTEmergency(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PTTEmergency"));
		}
		break;
	case  ID_MENU_GROUPBREAKOFF:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player-> ELTE_OCX_GroupBreakoff(pInfo->GroupID);
			//remove Item from group list
			if ("-1" == pInfo->GroupCategory)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			CHECK_RESULTE_CODE(strResult, _T(" ELTE_OCX_GroupBreakoff"));
		}
		break;

	case ID_MENU_DCPTTVOLMUTE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			{
				CString strMuteParam;
				strMuteParam.Append(_T("<Content>"));
				strMuteParam.Append(_T("<MuteParam>"));
				strMuteParam.Append(_T("<CallType>"));
				strMuteParam.Append(_T("1"));
				strMuteParam.Append(_T("</CallType>"));
				strMuteParam.Append(_T("</MuteParam>"));
				strMuteParam.Append(_T("</Content>"));
				CString strResult = m_peLTE_Player->ELTE_OCX_VolMute(pInfo->GroupID, strMuteParam);
				CHECK_RESULTE_CODE(strResult, _T("PTT ELTE_OCX_VolMute"));
			}
		}
		break;
	case ID_MENU_DCPTTVOLUNMUTE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			{
				CString strMuteParam;
				strMuteParam.Append(_T("<Content>"));
				strMuteParam.Append(_T("<MuteParam>"));
				strMuteParam.Append(_T("<CallType>"));
				strMuteParam.Append(_T("1"));
				strMuteParam.Append(_T("</CallType>"));
				strMuteParam.Append(_T("</MuteParam>"));
				strMuteParam.Append(_T("</Content>"));
				CString strResult = m_peLTE_Player->ELTE_OCX_VolUnMute(pInfo->GroupID, strMuteParam);
				CHECK_RESULTE_CODE(strResult, _T("PTT ELTE_OCX_VolUnMute"));
			}
		}
		break;
	case ID_MENU_DCPTTRELEASE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player->ELTE_OCX_PTTRelease(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_PTTRelease"));
		}
		break;
	case ID_MENU_SUB_JOIN_DCGROUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);

			CString strResult = m_peLTE_Player->ELTE_OCX_SubJoinGroup(pInfo->GroupID);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SubJoinGroup"));
		}
		break;
	case ID_MENU_JOINCALL:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			CTempUserJoinGrpCall dlg(NULL);
			dlg.SetParamInfo(m_strUserID, pInfo->GroupID);
			dlg.SetELtePlayer(m_peLTE_Player);
			dlg.DoModal();
		}
		break;
	case ID_MENU_MODIFYDGROUP:
		{
			ModifyDGNAParamInfo info;
			CModifyDGNADlg dlg(this);
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			dlg.SetDGNAParamInfo(m_strUserID, pInfo->GroupID, &info);
			int iExit = dlg.DoModal();
			if (IDOK != iExit)
			{
				return;
			}

			m_ModifyDGNAGroupId = pInfo->GroupID;
			// 构造XML
			CString strDGNAParam;
			strDGNAParam.Append(_T("<Content>"));
			strDGNAParam.Append(_T("<DynamicGroupInfo>"));
			strDGNAParam.Append(_T("<GroupID>"));
			strDGNAParam.Append(pInfo->GroupID);
			strDGNAParam.Append(_T("</GroupID>"));

			strDGNAParam.Append(_T("<AddUserList>"));
			for (int i=0; i<199; i++)
			{
				if (info.AddList[i].IsEmpty())
					break;
				strDGNAParam.Append(_T("<AddUserID>"));
				strDGNAParam.Append(info.AddList[i]);
				strDGNAParam.Append(_T("</AddUserID>"));
			}
			strDGNAParam.Append(_T("</AddUserList>"));
			strDGNAParam.Append(_T("<DeleteUserList>"));
			for (int i=0; i<199; i++)
			{
				if (info.DelList[i].IsEmpty())
					break;
				strDGNAParam.Append(_T("<UserID>"));
				strDGNAParam.Append(info.DelList[i]);
				strDGNAParam.Append(_T("</UserID>"));
			}
			strDGNAParam.Append(_T("</DeleteUserList>"));
			strDGNAParam.Append(_T("</DynamicGroupInfo>"));
			strDGNAParam.Append(_T("</Content>"));

			CString strResult = m_peLTE_Player->ELTE_OCX_ModifyDynamicGroup(m_strUserID, strDGNAParam);
			CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ModifyDynamicGroup"));
		}
		break;
	case ID_MENU_ADDPATCHROUPMEMBER:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);
			OperatePatchGroup(m_strUserID, pInfo->GroupID, 1, pInfo->GroupName);
		}
		break;
	case ID_MENU_DELPATCHROUPMEMBER:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);
			OperatePatchGroup(m_strUserID, pInfo->GroupID, 2, pInfo->GroupName);
		}
		break;
	case ID_MENU_RMPATCHROUP:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);

			CString strResult = m_peLTE_Player->ELTE_OCX_CancelPatchGroup(pInfo->GroupID);
			int iRet = GET_XML_RESULTE_CODE(strResult);
			if (iRet == 0)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			else 
			{
				CString str;
				str.Format(_T("ELTE_SDK_CancelPatchGroup Failed. (%d)"), iRet);
				MessageBox(str);
			}
		}
		break;
	case ID_MENU_JOIN_DCGROUP:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			//CString strResult = m_peLTE_Player->ELTE_OCX_JoinGroup(pInfo->GroupID);
			//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_JoinGroup"));
		}
		break;
	case ID_MENU_RM_DGNA:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			CString strResult = m_peLTE_Player->ELTE_OCX_CancelDynamicGroup(pInfo->GroupID);
			int iRet = GET_XML_RESULTE_CODE(strResult);
			if (iRet == 0)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			else if (iRet == -40001)
			{
				MessageBox(_T("没有订阅该动态组."));
			}
			else if (iRet == -40005)
			{
				MessageBox(_T("用户没有操作权限."));
			}
			else 
			{
				CString str;
				str.Format(_T("ELTE_OCX_CancelDynamicGroup Failed. (%d)"), iRet);
				MessageBox(str);
			}
		}
		break;
	case ID_MENU_DCGROUPUSERINFO:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupUserInfo* pInfo = (GroupUserInfo*)m_DcGroups.GetItemData(selectedItem);

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("UserID"), pInfo->UserID);
			dlg.SetInfo(2, _T("GroupID"), pInfo->GroupID);
			dlg.SetInfo(3, _T("UserPriorityInGroup"), pInfo->UserPriorityInGroup);
			dlg.SetInfo(4, _T("MemberType"), pInfo->MemberType);
			dlg.DoModal();
		}
		break;
	}
}

void CDConsoleDlg::GetSelGroupInfo(GroupInfo** pInfo)
{
	if (m_DcGroups.IsWindowVisible())
	{
		HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
		*pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

	}
	else if (m_PatchGroups.IsWindowVisible())
	{
		HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
		*pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);
	}
}

HTREEITEM CDConsoleDlg::FindStrGroupInfo(CString strname, GroupInfo** pInfo)
{
	CTreeCtrl *ptree = NULL;
	if (m_DcGroups.IsWindowVisible())
	{
		ptree = &m_DcGroups;
	}
	else if (m_PatchGroups.IsWindowVisible())
	{
		ptree = &m_PatchGroups;
	}

	HTREEITEM hroot = ptree->GetRootItem();
	// check whether this is a null tree
	if (NULL == hroot)
	{
		//null tree
		*pInfo = NULL; 
		return NULL;
	}
	//cycle check
	while (NULL != hroot)
	{
		//whether this node is the target node
		if (ptree->GetItemText(hroot) == strname)
		{
			//get info of the node
			*pInfo = (GroupInfo*)m_PatchGroups.GetItemData(hroot);
			break;
		}
		else
		{
			//check the brother node
			hroot = ptree->GetNextSiblingItem(hroot);
		}

	}

	return hroot;
}

void CDConsoleDlg::ELTE_PlayerEvent(UINT CtlID, ULONG ulEventId, LPCTSTR pXml)
{
	CElte_playerctrl1* pOcx = (CElte_playerctrl1*)m_pCeLTE_PlayerDemoDlg->GetDlgItem(CtlID);
	if (NULL == pOcx)
	{
		CString strMsg;
		strMsg.Format(_T("GetDlgItem CtlID(%d)"), CtlID);
		MessageBox(strMsg);
		return;
	}

	if (eLTE_PLAYER_HIDE == ulEventId)
	{
		// 释放资源
		std::vector<CElte_playerctrl1*>::iterator itor = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
		if (itor != m_ActiveOcxList.end())
		{
			m_ActiveOcxList.erase(itor);
			m_FreeOcxList.push_back(pOcx);
		}

		// 释放资源
		int iResId = 0;
		std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.begin();
		for (; m_ResourceMap.end() != it; it++)
		{
			ST_REALPLAY_PARAM& param = it->second;
			if (param.pOCX == pOcx)
			{
				iResId = it->first;
				param.Reset();
				m_ResourceMap.erase(it);
				break;
			}
		}

		if (GetDlgItemInt(IDC_EDIT_RESID) == iResId)
		{
			// 按钮状态
			GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
		}
	}
	else if (eLTE_PLAYER_SNAPSHOT == ulEventId)
	{
		CString strMsg = GET_XML_ELEM_VALUE_STR(pXml, _T("SnapshotPath"));
		strMsg.Insert(0, _T("抓图成功:\r\n"));
		MessageBox(strMsg);
	}
	else if (eLTE_PLAYER_MUTE == ulEventId)
	{
		int iResId = 0;
		std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.begin();
		for (; m_ResourceMap.end() != it; it++)
		{
			ST_REALPLAY_PARAM& param = it->second;
			if (param.pOCX == pOcx)
			{
				param.uiState &= ~STATE_ISUNMUTE;
				iResId = it->first;
				break;
			}
		}

		if (GetDlgItemInt(IDC_EDIT_RESID) == iResId)
		{
			GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
		}
	}
	else if (eLTE_PLAYER_UNMUTE == ulEventId)
	{
		int iResId = 0;
		std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.begin();
		for (; m_ResourceMap.end() != it; it++)
		{
			ST_REALPLAY_PARAM& param = it->second;
			if (param.pOCX == pOcx)
			{
				param.uiState |= STATE_ISUNMUTE;
				iResId = it->first;
				break;
			}
		}

		if (GetDlgItemInt(IDC_EDIT_RESID) == iResId)
		{
			GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(TRUE);
		}
	}
	else if (eLTE_PLAYER_REVERSE == ulEventId)
	{
		std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.begin();
		for (; m_ResourceMap.end() != it; it++)
		{
			ST_REALPLAY_PARAM& param = it->second;
			if (param.pOCX == pOcx)
			{
				CString strVideoFormat = GET_XML_ELEM_VALUE_STR(pXml, _T("VideoFormat"));
				CString strCameraType = GET_XML_ELEM_VALUE_STR(pXml, _T("CameraType"));
				CString strUserConfirmType = GET_XML_ELEM_VALUE_STR(pXml, _T("UserConfirmType"));
				CString strMuteType = GET_XML_ELEM_VALUE_STR(pXml, _T("MuteType"));

				param.videoParam.strFormat = strVideoFormat;
				param.videoParam.strCamera = strCameraType;
				param.videoParam.strConfirm = strUserConfirmType;
				param.videoParam.strMute = strMuteType;
				break;
			}
		}
	}
}

void CDConsoleDlg::InitDlg()
{
	// 先清理
	ClearProv();

	GetDlgItem(IDC_BUTTON_PROVINIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(TRUE);

	if(m_pCeLTE_PlayerDemoDlg && !((CeLTE_PlayerDemoDlg*)m_pCeLTE_PlayerDemoDlg)->m_bClose)
	{
		// 获取用户列表
		GetUsers();
		// 获取群组列表
		GetGroups();
		// 获取派接组列表
		GetPatchGroups();

		// 触发状态上报
		m_peLTE_Player->ELTE_OCX_TriggerStatusReport(1);
	}
}

void CDConsoleDlg::ClearProv()
{
	if (0 < m_DcUsers.GetItemCount())
	{
		m_DcUsers.DeleteAllItems();
	}

	if (0 < m_DcGroups.GetCount())
	{
		ClearDcGroupsTree();
		m_DcGroups.DeleteAllItems();
	}

	if (0 < m_PatchGroups.GetCount())
	{
		ClearPatchGroupsTree();
	}
}

void CDConsoleDlg::ClearPatchGroupsTree()
{
	HTREEITEM hItem = m_PatchGroups.GetRootItem();
	while (NULL != hItem)
	{
		GroupInfo* itemData = (GroupInfo*)m_PatchGroups.GetItemData(hItem);
		if (NULL != itemData)
		{
			delete itemData;
		}

		HTREEITEM hChildItem = m_PatchGroups.GetChildItem(hItem);
		while (NULL != hChildItem)
		{
			GroupUserInfo* itemDataChild = (GroupUserInfo*)m_PatchGroups.GetItemData(hChildItem);
			if (NULL != itemDataChild)
			{
				delete itemDataChild;
			}

			hChildItem = m_PatchGroups.GetNextSiblingItem(hChildItem);
		}
		hItem = m_PatchGroups.GetNextSiblingItem(hItem);
	}
	m_PatchGroups.DeleteAllItems();
}


void CDConsoleDlg::UpdateUserStatus(const CString& strUserId, int iStatus)
{
	for (int i=0; NULL != m_DcUsers.m_hWnd, i<m_DcUsers.GetItemCount(); i++)
	{
		CString strResId = m_DcUsers.GetItemText(i, DCUSERS_RESID);
		if (strResId == strUserId)
		{
			int imgId = GetUserImageId(strUserId, iStatus);
			m_DcUsers.SetItem(i, DCUSERS_RESID, LVIF_TEXT|LVIF_IMAGE, strUserId, imgId, 0, 0, 0);
			if (iStatus == 4011)
			{
				//心跳机制不能覆盖原有状态为在线状态。begin
				CString sStatus = m_DcUsers.GetItemText(i, DCUSERS_STATUS);
				if (sStatus != _T("offline"))
				{
					return;
				}
				//心跳机制不能覆盖原有状态为在线状态。end

				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("online"));
			}
			else if (iStatus == 4012)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("offline"));
			}
			else if (iStatus == 4020)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("正在发起呼叫"));
			}
			else if (iStatus == 4021)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("正在振铃"));
			}
			else if (iStatus == 4022)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("使用中"));
			}
			else if (iStatus == 4023)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("空闲"));
			}
		}
	}
}

void CDConsoleDlg::SetRealPlayParam(INT iResID, UINT uiLocalVideoPort, UINT uiLocalAudioPort, UINT uiRemoteVideoPort, UINT uiRemoteAudioPort)
{
	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(iResID);
	if (m_ResourceMap.end() == itor)
	{
		MessageBox(_T("Not found the ResID in SetRealPlayParam."));
		return;
	}

	ST_REALPLAY_PARAM& param = itor->second;
	if (!(param.uiState & STATE_ISPLAY))
	{
		MessageBox(_T("The resource is error state."));
		return;
	}

	param.strLocalIP = m_strLocalIP;
	param.strRemoteIP = m_strServerIP;
	param.uiLocalVideoPort = uiLocalVideoPort;
	param.uiLocalAudioPort = uiLocalAudioPort;
	param.uiRemoteVideoPort = uiRemoteVideoPort;
	param.uiRemoteAudioPort = uiRemoteAudioPort;

	if (GetDlgItemInt(IDC_EDIT_RESID) == iResID && !(param.uiState & STATE_ISSHOW))
	{
		GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(TRUE);
	}
}

void CDConsoleDlg::RealPlayHangup(INT iResID)
{

}

void CDConsoleDlg::RealPlayNotConnected(INT iResID, INT iCallStatus)
{
	if (3008 == iCallStatus)
	{
		CString strResId;
		strResId.Format(L"%d", iResID);
		SetDlgItemText(IDC_EDIT_RESID, strResId);
		OnBnClickedButtonStopplay();
	}
	// 释放资源
	std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.find(iResID);
	if (it == m_ResourceMap.end())
	{
		return;
	}

	ST_REALPLAY_PARAM& param = it->second;
	CElte_playerctrl1* pOcx = param.pOCX;
	param.Reset();
	m_ResourceMap.erase(it);

	// 释放资源
	std::vector<CElte_playerctrl1*>::iterator itor = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
	if (itor != m_ActiveOcxList.end())
	{
		m_ActiveOcxList.erase(itor);
		m_FreeOcxList.push_back(pOcx);
	}

	if (GetDlgItemInt(IDC_EDIT_RESID) == iResID)
	{
		// 按钮状态
		GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
	}
}

void CDConsoleDlg::GetPatchGroups()
{
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetPatchGroups(m_strUserID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetPatchGroups"));

	// 获取群组列表
	GroupInfoList groups;
	CXml::Instance().XmlParsePatchGroups(strResult, groups);

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		CString szItemroot(pInfo->GroupID);
		szItemroot.Append(_T(" "));
		szItemroot.Append(_T("[派接组]"));
		HTREEITEM itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
		m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

		// 组内用户列表
		strResult = m_peLTE_Player->ELTE_OCX_GetGroupMemberByPatchId(pInfo->GroupID);
		/*if (0 != iRet)
		{
		continue;
		}*/

		// 获取用户列表
		GroupUserInfoList groupUsers;
		CXml::Instance().XmlParsePatchGroupUsers(strResult, groupUsers);

		GroupUserInfoList::iterator it = groupUsers.begin();
		for (; groupUsers.end() != it; it++)
		{
			GroupUserInfo* &pUserInfo = *it;
			CString szItem(pUserInfo->MemberGroupId);
			szItem.Append(_T(" "));
			szItem.Append(_T("[普通组]"));
			HTREEITEM item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
			m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
		}

		m_PatchGroups.Expand(itemroot,TVE_EXPAND);
	}
}

void CDConsoleDlg::DelPatchGroupMember(int groupId, int memberId)
{
	CString strGroupId;
	strGroupId.Format(L"%d", groupId);
	CString strMemberId;
	strMemberId.Format(L"%d", memberId);
	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == strGroupId)
		{
			HTREEITEM item = m_PatchGroups.GetChildItem(itemroot);
			while (item)
			{
				GroupUserInfo* pInfo = (GroupUserInfo*)m_PatchGroups.GetItemData(item);
				if (pInfo->MemberGroupId == strMemberId)
				{
					delete pInfo;
					m_PatchGroups.DeleteItem(item);
					break;
				}
				item = m_PatchGroups.GetNextSiblingItem(item);
			}
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}
	m_PatchGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::AddPatchGroupMember(int groupId, int memberId)
{
	CString strGroupId;
	strGroupId.Format(L"%d", groupId);
	CString strMemberId;
	strMemberId.Format(L"%d", memberId);

	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == strGroupId)
		{
			HTREEITEM item = m_PatchGroups.GetChildItem(itemroot);
			while (item)
			{
				GroupUserInfo* pInfo = (GroupUserInfo*)m_PatchGroups.GetItemData(item);
				if (pInfo->MemberGroupId == strMemberId)
				{
					return;
				}
				item = m_PatchGroups.GetNextSiblingItem(item);
			}	

			GroupUserInfo* pUserInfo = new GroupUserInfo;
			pUserInfo->GroupID = strGroupId;
			pUserInfo->MemberGroupId = strMemberId;
			CString szItem(pUserInfo->MemberGroupId);
			szItem.Append(_T(" "));
			szItem.Append(_T("[普通组]"));
			item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
			m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
			break;
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}
	m_PatchGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::AddPatchGroup(int iGroupID)
{
	CString sGroupID;
	sGroupID.Format(_T("%d"), iGroupID);

	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == sGroupID)
		{
			return;
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}

	// 获取群组信息
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetPatchGroupInfo(sGroupID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetPatchGroupInfo"));
	CString strGroupID = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupNumber"));
	CString strGroupCreator	    = GET_XML_ELEM_VALUE_STR(strResult, _T("SetupDcId"));
	CString strGroupPriority	= GET_XML_ELEM_VALUE_STR(strResult, _T("PGPriority"));
	CString strGroupName		= GET_XML_ELEM_VALUE_STR(strResult, _T("PGName"));
	CString strGroupIndex		= GET_XML_ELEM_VALUE_STR(strResult, _T("DcPatchIndex"));
	CString strGroupVPNID		= GET_XML_ELEM_VALUE_STR(strResult, _T("VPNID"));

	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID			= strGroupID;
	pInfo->GroupCreator	    = strGroupCreator;
	pInfo->GroupPriority	= strGroupPriority;
	pInfo->GroupName		= strGroupName;
	pInfo->GroupIndex		= strGroupIndex;
	pInfo->GroupVPNID		= strGroupVPNID;

	// 增加群组
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" "));
	szItemroot.Append(_T("[派接组]"));
	itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

	// 获取群组用户列表
	strResult = m_peLTE_Player->ELTE_OCX_GetGroupMemberByPatchId(sGroupID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetGroupMemberByPatchId"));

	// 获取用户列表
	GroupUserInfoList groupUsers;
	CXml::Instance().XmlParsePatchGroupUsers(strResult, groupUsers);

	GroupUserInfoList::iterator it = groupUsers.begin();
	for (; groupUsers.end() != it; it++)
	{
		GroupUserInfo* &pUserInfo = *it;
		CString szItem(pUserInfo->MemberGroupId);
		szItem.Append(_T(" "));
		szItem.Append(_T("[普通组]"));
		HTREEITEM item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
		m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
	}

	m_PatchGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::RemovePatchGroup(int iGroupID)
{
	CString strGroupID;
	strGroupID.Format(_T("%d"), iGroupID);

	// 在派接组列表中查询该群组
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == strGroupID)
		{
			delete pInfo;

			HTREEITEM item = m_PatchGroups.GetChildItem(itemroot);
			while (item)
			{
				GroupUserInfo* pUserInfo = (GroupUserInfo*)m_PatchGroups.GetItemData(item);
				delete pUserInfo;
				item = m_PatchGroups.GetNextSiblingItem(item);
			}

			m_PatchGroups.DeleteItem(itemroot);
			return;
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}
}

void CDConsoleDlg::GetUsers()
{
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcUsers(m_strUserID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcUsers"));

	UserInfoList users;
	CXml::Instance().XmlParseDcUsers(strResult, users);

	UserInfoList::iterator itor = users.begin();
	for (; users.end() != itor; itor++)
	{
		UserInfo* pUserInfo = *itor;
		int item = m_DcUsers.InsertItem(m_DcUsers.GetItemCount(), _T(""), GetUserImageId(pUserInfo->UserCategory, true));
		m_DcUsers.SetItemText(item, DCUSERS_RESID, pUserInfo->UserID);
		m_DcUsers.SetItemText(item, DCUSERS_STATUS, _T("offline"));
		m_DcUsers.SetItemText(item, DCUSERS_NAME, pUserInfo->UserName);
		m_DcUsers.SetItemText(item, DCUSERS_CATEGORY, pUserInfo->UserCategory);
		m_DcUsers.SetItemText(item, DCUSERS_PRIORITY, pUserInfo->UserPriority);
	}
}

void CDConsoleDlg::GetGroups()
{
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcGroups(m_strUserID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetDcGroups"));

	// 获取群组列表
	GroupInfoList groups;
	CXml::Instance().XmlParseDcGroups(strResult, groups);

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		CString szItemroot(pInfo->GroupID);
		szItemroot.Append(_T(" "));
		szItemroot.Append(GetGroupCategoryString(pInfo->GroupCategory));
		HTREEITEM itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
		m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

		// 组内用户列表
		strResult = m_peLTE_Player->ELTE_OCX_GetGroupUsers(pInfo->GroupID);
		//CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetGroupUsers"));

		// 获取用户列表
		GroupUserInfoList groupUsers;
		CXml::Instance().XmlParseGroupUsers(strResult, groupUsers);
		GroupUserInfoList::iterator it = groupUsers.begin();
		for (; groupUsers.end() != it; it++)
		{
			GroupUserInfo* &pUserInfo = *it;
			CString szItem(pUserInfo->UserID);
			szItem.Append(_T(" "));
			szItem.Append(GetGroupUserMemberTypeString(pUserInfo->MemberType));
			HTREEITEM item = m_DcGroups.InsertItem(szItem, -1, -1, itemroot);
			m_DcGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
		}

		m_DcGroups.Expand(itemroot,TVE_EXPAND);
	}
}

void CDConsoleDlg::AddGroup(int iGroupID)
{
	CString sGroupID;
	sGroupID.Format(_T("%d"), iGroupID);

	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_DcGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(itemroot);
		if (pInfo->GroupID == sGroupID)
		{
			return;
		}
		itemroot = m_DcGroups.GetNextSiblingItem(itemroot);
	}

	// 获取群组信息
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetGroupInfo(sGroupID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetGroupInfo"));
	CString strGroupID = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupID"));
	CString strGroupCategory = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupCategory"));
	CString strGroupPriority = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupPriority"));
	CString strGroupName = GET_XML_ELEM_VALUE_STR(strResult, _T("GroupName"));
	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID = strGroupID;
	pInfo->GroupCategory = strGroupCategory;
	pInfo->GroupPriority = strGroupPriority;
	pInfo->GroupName = strGroupName;

	// 增加群组
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" "));
	szItemroot.Append(GetGroupCategoryString(pInfo->GroupCategory));
	itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

	strResult = m_peLTE_Player->ELTE_OCX_GetGroupUsers(sGroupID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetGroupUsers"));

	// 获取用户列表
	GroupUserInfoList groupUsers;
	CXml::Instance().XmlParseGroupUsers(strResult, groupUsers);
	GroupUserInfoList::iterator it = groupUsers.begin();
	for (; groupUsers.end() != it; it++)
	{
		GroupUserInfo* &pUserInfo = *it;
		CString szItem(pUserInfo->UserID);
		szItem.Append(_T(" "));
		szItem.Append(GetGroupUserMemberTypeString(pUserInfo->MemberType));
		HTREEITEM item = m_DcGroups.InsertItem(szItem, -1, -1, itemroot);
		m_DcGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
	}

	m_DcGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::RemoveGroup(int iGroupID)
{
	CString strGroupID;
	strGroupID.Format(_T("%d"), iGroupID);

	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_DcGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(itemroot);
		if (pInfo->GroupID == strGroupID)
		{
			delete pInfo;

			HTREEITEM item = m_DcGroups.GetChildItem(itemroot);
			if (item)
			{
				GroupUserInfo* pUserInfo = (GroupUserInfo*)m_DcGroups.GetItemData(item);
				delete pUserInfo;
			}

			m_DcGroups.DeleteItem(itemroot);
			return;
		}
		itemroot = m_DcGroups.GetNextSiblingItem(itemroot);
	}
}

void CDConsoleDlg::AddTempGroup(const CString& strGroupID)
{
	// 在群组列表中查询该群组
	HTREEITEM itemroot = m_DcGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(itemroot);
		if (pInfo->GroupID == strGroupID)
		{
			return;
		}
		itemroot = m_DcGroups.GetNextSiblingItem(itemroot);
	}

	// 构造群组信息
	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID = strGroupID;
	pInfo->GroupCategory = "-1";
	pInfo->GroupPriority = "-1";
	pInfo->GroupName = "-1";

	// 增加群组
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" [临时组]"));
	itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

	m_DcGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::AddImage(CImageList& imageList, UINT id)
{
	CBitmap bm;
	bm.LoadBitmap(id);
	imageList.Add(&bm, RGB(0, 0, 0));
}

int CDConsoleDlg::GetUserImageId(const CString& strCategory, bool bGray)
{
	int iUserCategory = StrToInt(strCategory);
	int iImgID = -1;

	switch (iUserCategory) // 用户种类
	{
	case 0:
		{
			// 调度台用户
			iImgID = bGray ? Img_dispatcher_offline : Img_dispatcher_online;
		}
		break;
	case 1:
		{
			// 固定摄像头
			iImgID = bGray ? Img_camera_offline : Img_camera_online;
		}
		break;
	case 2:
		{
			// PSTN用户
			iImgID = bGray ? Img_gwuser_offline : Img_gwuser_online;
		}
		break;
	case 9:
		{
			// PTT用户
			iImgID = bGray ? Img_pttuser_offline : Img_pttuser_online;
		}
		break;
	}

	return iImgID;
}

int CDConsoleDlg::GetUserImageId(const CString& strUserId, int iStatus)
{
	CString strResult = m_peLTE_Player->ELTE_OCX_GetUserInfo(strUserId);
	CString strUserCategory = GET_XML_ELEM_VALUE_STR(strResult, _T("UserCategory"));
	int iImgID = -1;

	if (iStatus == 4012)
	{
		iImgID = GetUserImageId(strUserCategory, true);
	}
	else
	{
		iImgID = GetUserImageId(strUserCategory, false);
	}

	return iImgID;
}

CString CDConsoleDlg::GetGroupCategoryString(const CString& strGroupCategory)
{
	int iGroupCategory = StrToInt(strGroupCategory);
	CString strResult;

	switch (iGroupCategory) // 群组种类
	{
	case 0:
		{
			strResult.Append(_T("[全网广播组]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[普通组]"));
		}
		break;
	case 2:
		{
			strResult.Append(_T("[区域广播组]"));
		}
		break;
	case 8:
		{
			strResult.Append(_T("[外部组]"));
		}
		break;
	case 9:
		{
			strResult.Append(_T("[动态组]"));
		}
		break;
	case 10:
		{
			strResult.Append(_T("[不分类型]"));
		}
		break;
	}

	return strResult;
}

CString CDConsoleDlg::GetGroupUserMemberTypeString(const CString& strMemberType)
{
	int iMeberType = StrToInt(strMemberType);
	CString strResult;

	switch (iMeberType) // 用户类型
	{
	case 0:
		{
			strResult.Append(_T("[动态重组中的组成员]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[普通用户成员]"));
		}
		break;
	}

	return strResult;
}

void CDConsoleDlg::ClearDcGroupsTree()
{
	HTREEITEM hItem = m_DcGroups.GetRootItem();
	while (NULL != hItem)
	{
		GroupInfo* itemData = (GroupInfo*)m_DcGroups.GetItemData(hItem);
		if (NULL != itemData)
		{
			delete itemData;
		}

		HTREEITEM hChildItem = m_DcGroups.GetChildItem(hItem);
		while (NULL != hChildItem)
		{
			GroupUserInfo* itemDataChild = (GroupUserInfo*)m_DcGroups.GetItemData(hChildItem);
			if (NULL != itemDataChild)
			{
				delete itemDataChild;
			}

			hChildItem = m_DcGroups.GetNextSiblingItem(hChildItem);
		}
		hItem = m_DcGroups.GetNextSiblingItem(hItem);
	}
	m_DcGroups.DeleteAllItems();
}


void CDConsoleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	static BOOL s_bMax = FALSE;
	if (s_bMax)
	{
		SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL,0,0,m_RectMax.Width(),m_RectMax.Height(),SWP_NOMOVE);
		GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(_T(""));
	}
	s_bMax = !s_bMax;

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDConsoleDlg::OnBnClickedButtonEvent0()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent0);
}


void CDConsoleDlg::OnBnClickedButtonEvent1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent1);
}


void CDConsoleDlg::OnBnClickedButtonEvent2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent2);
}


void CDConsoleDlg::OnBnClickedButtonEvent3()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent3);
}


void CDConsoleDlg::OnBnClickedButtonEvent4()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent4);
}


void CDConsoleDlg::OnBnClickedButtonEvent5()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent5);
}


void CDConsoleDlg::OnBnClickedButtonEvent6()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent6);
}


void CDConsoleDlg::OnBnClickedButtonEvent7()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent7);
}


void CDConsoleDlg::OnItemclickListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

LRESULT CDConsoleDlg::OnCallMsgReceived(WPARAM wparam, LPARAM lparam)
{
	if (NULL == m_callDlg.m_hWnd)
	{
		m_callDlg.SetELtePlayer(m_peLTE_Player);
		m_callDlg.Create(CCallDlg::IDD, this);
	}
	m_callDlg.SetTile();
	m_callDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CDConsoleDlg::OnBnClickedButtonEvent8()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent8);
}


void CDConsoleDlg::OnBnClickedButtonEvent9()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent9);
}


void CDConsoleDlg::OnBnClickedButtonSds()
{
	// TODO: Add your control notification handler code here
	SDSParamInfo info;
	INT_PTR nRes;
	SDSMessage sdsMessage(this);
	sdsMessage.SetSDSParamInfo(m_strUserID,&info);
	nRes = sdsMessage.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML 格式 ---
	<Content>
		<SDSType></SDSType>			//SDS短数据类型
		<MsgBody></MsgBody>	    //SDS短数据内容
		<Receiver></Receiver>	    //收件人，格式为“MSISDN@broadcast.SERVER_IP”的字符串，比如“1001@broadcast.10.1.1.1”；从2.1版本开始可以只填用户号，例如“1001”
		<AttachFileList>			//附件地址列表
			<AttachFile></AttachFile>
			<AttachFile></AttachFile>
		</AttachFileList>
		<MsgId></MsgId>
	</Content>
	************************************************************************/
	CString strSDSParam;
	strSDSParam.Append(_T("<Content>"));
	strSDSParam.Append(_T("<SDSType>"));
	strSDSParam.Append(info.sdstype);
	strSDSParam.Append(_T("</SDSType>"));
	strSDSParam.Append(_T("<MsgBody>"));
	strSDSParam.Append(info.msgBody);
	strSDSParam.Append(_T("</MsgBody>"));
	strSDSParam.Append(_T("<Receiver>"));
	strSDSParam.Append(info.receiver);
	strSDSParam.Append(_T("</Receiver>"));

	strSDSParam.Append(_T("<AttachFileList>"));
	std::list<CString>::iterator it;/* = info.files.begin();*/
	for (it = info.files.begin(); it != info.files.end(); ++it)
	{
		//if((*it).IsEmpty())
		//	break;
		strSDSParam.Append(_T("<AttachFile>"));
		strSDSParam.Append(*it);
		strSDSParam.Append(_T("</AttachFile>"));
	}

	strSDSParam.Append(_T("</AttachFileList>"));
	
	strSDSParam.Append(_T("</Content>"));

	//DGNAParamInfo info;
	//CDGNAParam dlg(this);
	//dlg.SetDGNAParamInfo(m_strUserID, &info, true);
	//int iExit = dlg.DoModal();
	//if (IDOK != iExit)
	//{
	//	return;
	//}

	CString strResult = m_peLTE_Player->ELTE_OCX_SDSSendMessage(m_strUserID,strSDSParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SDSSendMessage"));
}


void CDConsoleDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent10);
}


void CDConsoleDlg::OnBnClickedButtonEvent11()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent11);
}


void CDConsoleDlg::OnBnClickedButtonDispatchvideo()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();

	DispatchParamInfo info;
	INT_PTR nRes;
	DispatchParamDlg dlg(this);
	info.strFormat = GetVideoFormatString();
	dlg.SetDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<Fmtvalue></Fmtvalue>//D1、CIF、QCIF、720P或1080P
		<DispatchNum></DispatchNum>//视频源用户号，填写视频源终端ID号。
		<Dstviewerlist>//视频分发的目的终端ID列表
			<Dstviewer></Dstviewer>//视频分发的目的终端或调度台，填写目的终端ID号或者目的调度台ID号
		</Dstviewerlist>
		<Channel></Channel>//Reserved，内容可以不填；
	</Content>
	************************************************************************/
	CString strDispatchParam;
	strDispatchParam.Append(_T("<Content>"));
	strDispatchParam.Append(_T("<Fmtvalue>"));
	strDispatchParam.Append(info.strFormat);
	strDispatchParam.Append(_T("</Fmtvalue>"));
	strDispatchParam.Append(_T("<DispatchNum>"));
	strDispatchParam.Append(strResID);
	strDispatchParam.Append(_T("</DispatchNum>"));
	strDispatchParam.Append(_T("<Dstviewerlist>"));
	std::list<CString>::iterator it;/* = info.files.begin();*/
	for (it = info.strDstViewerList.begin(); it != info.strDstViewerList.end(); ++it)
	{
		//if((*it).IsEmpty())
		//	break;
		strDispatchParam.Append(_T("<Dstviewer>"));
		strDispatchParam.Append(*it);
		strDispatchParam.Append(_T("</Dstviewer>"));
	}

	strDispatchParam.Append(_T("</Dstviewerlist>"));

	strDispatchParam.Append(_T("<Channel>"));
	strDispatchParam.Append(_T("</Channel>"));
	strDispatchParam.Append(_T("</Content>"));


	CString strResult = m_peLTE_Player->ELTE_OCX_StartVideoDispatch(strResID, strDispatchParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StartVideoDispatch"));
	GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(TRUE);

}


void CDConsoleDlg::OnBnClickedButtonUndispatch()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();

	UnDispatchParamInfo info;
	INT_PTR nRes;
	UnDispatchParamDlg dlg(this);
	dlg.SetUnDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>发起视频回传的调度员的ID号</ResourceID>
		<UserId>需要挂断的正在分发中的用户的ID号</UserId>
	</Content>
	************************************************************************/
	CString strDispatchParam;
	strDispatchParam.Append(_T("<Content>"));
	strDispatchParam.Append(_T("<UserId>"));
	strDispatchParam.Append(info.strUsrId);
	strDispatchParam.Append(_T("</UserId>"));
	strDispatchParam.Append(_T("</Content>"));


	CString strResult = m_peLTE_Player->ELTE_OCX_StopVideoDispatch(strResID, strDispatchParam);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopVideoDispatch"));
}


afx_msg LRESULT CDConsoleDlg::OnVideoMsgReceived(WPARAM wParam, LPARAM lParam)
{

	CVideoDlg *pVideoDlg = new CVideoDlg;
	if(pVideoDlg == nullptr)
	{
		return NULL;
	}

	pVideoDlg->SetResId(m_videoDlg.m_strResId,m_videoDlg.m_strMuteType,m_videoDlg.m_bUse);

	if (NULL == pVideoDlg->m_hWnd)
	{
		pVideoDlg->SetELtePlayer(m_peLTE_Player);

		pVideoDlg->Create(IDD_DIALOG_VIDEO, NULL);
	}
	CString strTitle("Incoming Video: ");
	strTitle.Append(pVideoDlg->m_strResId);
	pVideoDlg->SetWindowText(strTitle);
	
	pVideoDlg->ShowWindow(SW_SHOW);
	pVideoDlg->setDcDlg(this);

	return 0;

}

afx_msg LRESULT CDConsoleDlg::ShowForRecv(WPARAM wParam, LPARAM lParam)
{
		CElte_playerctrl1* pOcx = NULL;

		CString strResult = NULL;

		if (!m_FreeOcxList.empty())
		{
			std::vector<CElte_playerctrl1*>::iterator itor = m_FreeOcxList.begin();
			pOcx = *itor;
			m_ActiveOcxList.push_back(pOcx);
			m_FreeOcxList.erase(itor);
		}
		else
		{
			pOcx = new CElte_playerctrl1;
			CRect rect;
			m_peLTE_Player->GetClientRect(&rect);
			pOcx->Create(_T(""),WS_CHILD,rect,m_pCeLTE_PlayerDemoDlg,OCX_ID_BEGIN+m_AllOcxList.size());
			m_AllOcxList.push_back(pOcx);
			m_ActiveOcxList.push_back(pOcx);

			strResult = pOcx->ELTE_OCX_Load(eLTE_LOAD_MEDIA_PLAYER);
		}
	

		//参数构造
		CString strLocalParam;
		strLocalParam.Append(_T("<Content>"));
		strLocalParam.Append(_T("<LocalMediaAddr>"));
		strLocalParam.Append(_T("<LocalIP>"));
		strLocalParam.Append(_T("0"));
		strLocalParam.Append(_T("</LocalIP>"));
		strLocalParam.Append(_T("<VideoPort>"));
		strLocalParam.Append(UIntToStr(0));
		strLocalParam.Append(_T("</VideoPort>"));
		strLocalParam.Append(_T("<AudioPort>"));
		strLocalParam.Append(UIntToStr(0));
		strLocalParam.Append(_T("</AudioPort>"));
		strLocalParam.Append(_T("</LocalMediaAddr>"));
		strLocalParam.Append(_T("</Content>"));

		CString strRemoteParam;
		strRemoteParam.Append(_T("<Content>"));
		strRemoteParam.Append(_T("<RemoteMediaAddr>"));
		strRemoteParam.Append(_T("<RemoteIP>"));
		strRemoteParam.Append(_T("0"));
		strRemoteParam.Append(_T("</RemoteIP>"));
		strRemoteParam.Append(_T("<VideoPort>"));
		strRemoteParam.Append(UIntToStr(0));
		strRemoteParam.Append(_T("</VideoPort>"));
		strRemoteParam.Append(_T("<AudioPort>"));
		strRemoteParam.Append(UIntToStr(0));
		strRemoteParam.Append(_T("</AudioPort>"));
		strRemoteParam.Append(_T("</RemoteMediaAddr>"));
		strRemoteParam.Append(_T("</Content>"));

		int iResId = (int)wParam;
		CString strResID = IntToStr(iResId);
		strResult = pOcx->ELTE_OCX_ShowRealPlay(strResID, strLocalParam, strRemoteParam);
		//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_ShowRealPlay"));

		
		std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
		
		if (m_ResourceMap.end() == itor)
		{
			ST_REALPLAY_PARAM param;
			param.uiState |= STATE_ISPLAY;
			param.pOCX = pOcx;
			//param.videoParam = info;
			param.uiVideoType = 1;//<视频分发>或者<视频回传>请求这类视频类型

			param.uiState |= STATE_ISSHOW;
			m_ResourceMap.insert(std::make_pair(StrToInt(strResID), param));
		}
		else
		{
			ST_REALPLAY_PARAM& param = itor->second;
			param.uiState = STATE_ISPLAY;
			param.uiVideoType = 1;//<视频分发>或者<视频回传>请求这类视频类型

			param.uiState |= STATE_ISSHOW;
			param.pOCX = pOcx;
			//param.videoParam = info;
		}

		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISPATCHVIDEO)->EnableWindow(TRUE);		
		GetDlgItem(IDC_BUTTON_REVERSEPLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REALPLAY)->EnableWindow(FALSE);

		return 0;
}


void CDConsoleDlg::OnBnClickedButtonVwall()
{
	// TODO: Add your control notification handler code here
	CHECK_RESOURCE();
	CString strResult = _T("");
	strResult = m_peLTE_Player->ELTE_OCX_GetDcVWallIDList();
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcVWallIDList"));
	VideoChannelList stringList;
	CXml::Instance().XmlParseVWallIDList(strResult, stringList);
	INT_PTR nRes;
	CVWall dlg(this);
	
	dlg.setVideoChannelList(&stringList);

	dlg.setDcDlg(this);
	dlg.SetELtePlayer(m_peLTE_Player);
	dlg.SetResourceId(strResID);

	nRes = dlg.DoModal();
	if (IDOK!=nRes)

	return;
	//CString strVideoChannel = dlg.getVideoChannel();

}


void CDConsoleDlg::OnBnClickedBtnDialphone()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPhoneNum = _T("");
	CString strDialNum = _T("");
	CString strResult = _T("");

	GetDlgItem(IDC_EDIT_PHONENUM)->GetWindowText(strDialNum);
	//参数构造
	strPhoneNum.Append(_T("<Content>"));
	strPhoneNum.Append(_T("<TelNumber>"));
	strPhoneNum.Append(strDialNum);
	strPhoneNum.Append(_T("</TelNumber>"));
	strPhoneNum.Append(_T("</Content>"));
		
	strResult = m_peLTE_Player->ELTE_OCX_TelephoneDial(strPhoneNum);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_TelephoneDial"));	
}


void CDConsoleDlg::OnBnClickedBtnHangup()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDialNum = _T("");
	CString strResult = _T("");
	GetDlgItem(IDC_EDIT_PHONENUM)->GetWindowText(strDialNum);
	strResult = m_peLTE_Player->ELTE_OCX_TelephoneHangup(strDialNum);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_TelephoneHangup"));
}

void CDConsoleDlg::OnBnClickedBtnDiscreetlisten()
{
	// TODO: 在此添加控件通知处理程序代码
	CHECK_DISRESOURCE();
	CString strResult = _T("");
	strResult = m_peLTE_Player->ELTE_OCX_StartDiscreetListen(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StartDiscreetListen"));
}


void CDConsoleDlg::OnBnClickedBtnStopdiscreenlisten()
{
	// TODO: 在此添加控件通知处理程序代码
	CHECK_DISRESOURCE();
	CString strResult = _T("");
	strResult = m_peLTE_Player->ELTE_OCX_StopDiscreetListen(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopDiscreetListen"));
}

void CDConsoleDlg::OnBnClickedBtnEnvlisten()
{
	// TODO: 在此添加控件通知处理程序代码
	CHECK_DISRESOURCE();
	CString strResult = _T("");
	strResult = m_peLTE_Player->ELTE_OCX_StartEnvironmentListen(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StartEnvironmentListen"));
}

//人工转接:reserved
void CDConsoleDlg::OnBnClickedBtnP2ptransfer()
{
	// TODO: 在此添加控件通知处理程序代码
	CHECK_RESOURCE();
	CString strResult = _T("");
	CString strParam = _T("");

	//参数构造
	strParam.Append(_T("<Content>"));
	strParam.Append(_T("</Content>"));

}


void CDConsoleDlg::OnBnClickedButtonCreatepg()
{
	// TODO: Add your control notification handler code here
	OperatePatchGroup(m_strUserID, _T("0"), 0);
}

void CDConsoleDlg::OperatePatchGroup(const CString& strDcID, const CString& strGroupID, const int& iOperateType, const CString& strGroupName)
{
	// TODO: Add your control notification handler code here
	PCHGRPParamInfo info;
	CCreatePatchGroupDlg dlg(this);
	dlg.SetPCHGRPParamInfo(m_strUserID, strGroupID, strGroupName, &info);

	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// 构造XML
	CString strPCHGRPParam;
	strPCHGRPParam.Append(_T("<Content>"));
	strPCHGRPParam.Append(_T("<DcID>"));
	strPCHGRPParam.Append(info.DcID);
	strPCHGRPParam.Append(_T("</DcID>"));
	strPCHGRPParam.Append(_T("<PatchGroupID>"));
	strPCHGRPParam.Append(info.GroupID);
	strPCHGRPParam.Append(_T("</PatchGroupID>"));
	strPCHGRPParam.Append(_T("<PatchGroupName>"));
	strPCHGRPParam.Append(info.Alias);
	strPCHGRPParam.Append(_T("</PatchGroupName>"));
	strPCHGRPParam.Append(_T("<PatchGroupMemberList>"));
	for (int i=0; i<20; i++)
	{
		if (info.GroupList[i].IsEmpty())
			break;
		strPCHGRPParam.Append(_T("<PatchGroupMember>"));
		strPCHGRPParam.Append(info.GroupList[i]);
		strPCHGRPParam.Append(_T("</PatchGroupMember>"));
	}
	strPCHGRPParam.Append(_T("</PatchGroupMemberList>"));
	strPCHGRPParam.Append(_T("</Content>"));

	// 创建派接组
	if (0 == iOperateType)
	{
		CString strResult = m_peLTE_Player->ELTE_OCX_CreatePatchGroup(strPCHGRPParam);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_SDK_CreatePatchGroup"));
	}
	// 增加派接组成员
	if (1 == iOperateType)
	{
		CString strResult = m_peLTE_Player->ELTE_OCX_AddPatchGroupMember(strPCHGRPParam);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_SDK_CreatePatchGroup"));
	}
	// 删除派接组成员
	if (2 == iOperateType)
	{
		CString strResult = m_peLTE_Player->ELTE_OCX_DeletePatchGroupMember(strPCHGRPParam);
		CHECK_RESULTE_CODE(strResult, _T("ELTE_SDK_DeletePatchGroupMember"));
	}

}

void CDConsoleDlg::OnBnClickedButtonPatchgroups()
{
	// TODO: Add your control notification handler code here
	m_DcUsers.ShowWindow(SW_HIDE);
	m_DcGroups.ShowWindow(SW_HIDE);
	m_PatchGroups.ShowWindow(SW_SHOW);
}


void CDConsoleDlg::OnRclickTreePatchgroups(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CPoint pt, pmenu;
	::GetCursorPos(&pt);
	::GetCursorPos(&pmenu);
	m_PatchGroups.ScreenToClient(&pt);

	UINT uFlag = 0;
	HTREEITEM hSelectedItem = m_PatchGroups.HitTest(pt,&uFlag);
	if (NULL == hSelectedItem || !(TVHT_ONITEM & uFlag))
	{
		return;
	}

	m_PatchGroups.SelectItem(hSelectedItem);
	if (m_PatchGroups.GetParentItem(hSelectedItem) == NULL)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(hSelectedItem);

		CMenu pttMenu;
		pttMenu.CreatePopupMenu();
		pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTDIAL, _T("Dial"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTRELEASE, _T("Release"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTHANGUP, _T("Hangup"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTEMERGENCY, _T("Emergency"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_GROUPBREAKOFF, _T("Breakoff"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTVOLMUTE, _T("Mute"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_DCPTTVOLUNMUTE, _T("UnMute"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTDISLISTEN, _T("DisListen_Start"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTOPDISLISTEN, _T("DisListen_Stop"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTENVLISTEN, _T("EnvListen_Start"));

		//组节点
		CMenu menu;
		menu.CreatePopupMenu();
		//menu.AppendMenu(MF_STRING, ID_MENU_SUBGROUP, _T("SubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_GETPATCHGROUPINFO, _T("GetPatchGroupInfo"));
		menu.AppendMenu(MF_STRING, ID_MENU_ADDPATCHROUPMEMBER, _T("AddMember"));
		menu.AppendMenu(MF_STRING, ID_MENU_DELPATCHROUPMEMBER, _T("DelMember"));
		menu.AppendMenu(MF_STRING, ID_MENU_SUB_JOIN_DCGROUP, _T("SubJoinGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_UNSUB_DCGROUP, _T("UnSubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_RMPATCHROUP, _T("RemovePatchGroup"));

		menu.AppendMenu(MF_POPUP, (UINT)pttMenu.m_hMenu, _T("PTT"));
		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
	//else
	//{
	//	//用户节点
	//	CMenu menu;
	//	menu.CreatePopupMenu();
	//	menu.AppendMenu(MF_STRING, ID_MENU_GETGROUPINFO, _T("GetGroupUserInfo"));

	//	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	//}
}


void CDConsoleDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
	return;
}


void CDConsoleDlg::OnBnClickedButtonGetgissub()
{
	// TODO: Add your control notification handler code here
	CString strResult = m_peLTE_Player->ELTE_OCX_GetGisSubscription(m_strUserID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetGisSubscription"));
	::MessageBox(this->GetSafeHwnd(), strResult, _T("GisSubscriptionInfo"), MB_OK);
}
