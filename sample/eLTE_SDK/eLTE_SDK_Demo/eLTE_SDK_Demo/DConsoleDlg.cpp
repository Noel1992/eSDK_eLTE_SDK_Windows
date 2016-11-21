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
#include "eLTE_SDK_Demo.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "eLTE_SDK_DemoDlg.h"
#include "Xml.h"
#include "InfoDlg.h"
#include "VideoParamDlg.h"
#include "PTZCtrlDlg.h"
#include "CreateGroupDlg.h"
#include "MediaPlayerDlgMgr.h"
#include "eLTE_Tool.h"
#include "DispatchParamDlg.h"
#include "UnDispatchParamDlg.h"
#include "SDSMessage.h"
#include "CreatePatchGroupDlg.h"
#include "P2PTransferDlg.h"
#include "ModifyDGNADlg.h"
#include "SetGisCfgDlg.h"
#include "TempUserJoinGrpCall.h"

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

typedef enum
{
	ID_MENU_BEGIN = 3000,
	ID_MENU_GETUSERINFO,
	ID_MENU_STARTREALPLAY,
	ID_MENU_STOPREALPLAY,
	ID_MENU_PTZCONTROL,
	ID_MENU_P2PTRANSFER,
	ID_MENU_P2PDIAL,
	ID_MENU_P2PHANGUP,
	ID_MENU_P2PBREAKIN,
	ID_MENU_P2PBREAKOFF,
	ID_MENU_P2PMUTE,
	ID_MENU_P2PUNMUTE,
	ID_MENU_P2PHALFDPXDIAL,
	ID_MENU_P2PHALFDPXRELEASE,
	ID_MENU_GISSUBSCRIBE,
	ID_MENU_GISUNSUBSCRIBE,
	ID_MENU_GETGISCFG,
	ID_MENU_SETGISCFG,
	ID_MENU_GETGROUPINFO,
	ID_MENU_SUBGROUP,
	ID_MENU_UNSUBGROUP,
	ID_MENU_SUBJOINGROUP,
	ID_MENU_JOINCALL,
	ID_MENU_RMDGROUP,
	ID_MENU_MODIFYDGROUP,
	ID_MENU_RMPATCHROUP,
	ID_MENU_ADDPATCHROUPMEMBER,
	ID_MENU_DELPATCHROUPMEMBER,
	ID_MENU_GETPATCHGROUPINFO,
	ID_MENU_GROUPUSERINFO,
	ID_MENU_PTTDIAL,
	ID_MENU_PTTRELEASE,
	ID_MENU_PTTHANGUP,
	ID_MENU_PTTEMERGENCY,
	ID_MENU_GROUPBREAKOFF,
	ID_MENU_PTTMUTE,
	ID_MENU_PTTUNMUTE,
	ID_MENU_P2PSTARTDISLISTEN,
	ID_MENU_P2PSTOPDISLISTEN,
	ID_MENU_P2PSTARTENVLISTEN,
	ID_MENU_PTTSTARTDISLISTEN,
	ID_MENU_PTTSTOPDISLISTEN,
	ID_MENU_PTTSTARTENVLISTEN,
	ID_MENU_END
} NMRCLICK_MENU_LIST;

// CDConsoleDlg dialog

IMPLEMENT_DYNAMIC(CDConsoleDlg, CDialogEx)

CDConsoleDlg::CDConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDConsoleDlg::IDD, pParent)
	, m_pELTE_SDK_DemoDlg(NULL)
	, m_AllResync(_T(""))
	, m_ResStatus(_T(""))
	, m_UserStatus(_T(""))
	, m_ProvChange(_T(""))
	, m_ModuleStatus(_T(""))
	, m_VideoStatus(_T(""))
	, m_CallStatus(_T(""))
	, m_GroupStatus(_T(""))
	, m_SDSReport(_T(""))
	, m_SDSStatus(_T(""))
{

}

CDConsoleDlg::~CDConsoleDlg()
{
}

void CDConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DCUSERS, m_DcUsers);
	DDX_Control(pDX, IDC_TREE_DCGROUPS, m_DcGroups);
	DDX_Control(pDX, IDC_COMBO_RESOURCE, m_ComboVWIDList);
	DDX_Control(pDX, IDC_TREE_PATCHGROUPS, m_PatchGroups);
}


BEGIN_MESSAGE_MAP(CDConsoleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DCUSERS, &CDConsoleDlg::OnBnClickedButtonDcusers)
	ON_BN_CLICKED(IDC_BUTTON_DCGROUPS, &CDConsoleDlg::OnBnClickedButtonDcgroups)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_ALLRESYNC, &CDConsoleDlg::OnBnClickedButtonAllresync)
	ON_BN_CLICKED(IDC_BUTTON_RESSTATUS, &CDConsoleDlg::OnBnClickedButtonResstatus)
	ON_BN_CLICKED(IDC_BUTTON_USERSTATUS, &CDConsoleDlg::OnBnClickedButtonUserstatus)
	ON_BN_CLICKED(IDC_BUTTON_PROVCHANGE, &CDConsoleDlg::OnBnClickedButtonProvchange)
	ON_BN_CLICKED(IDC_BUTTON_MODULESTATUS, &CDConsoleDlg::OnBnClickedButtonModulestatus)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOSTATUS, &CDConsoleDlg::OnBnClickedButtonVideostatus)
	ON_BN_CLICKED(IDC_BUTTON_CALLSTATUS, &CDConsoleDlg::OnBnClickedButtonCallstatus)
	ON_BN_CLICKED(IDC_BUTTON_GROUPSTATUS, &CDConsoleDlg::OnBnClickedButtonGroupstatus)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER, &CDConsoleDlg::OnBnClickedButtonTrigger)
	ON_BN_CLICKED(IDC_BUTTON_PROVINIT, &CDConsoleDlg::OnBnClickedButtonProvinit)
	ON_BN_CLICKED(IDC_BUTTON_PROVEXIT, &CDConsoleDlg::OnBnClickedButtonProvexit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMRClickListDcusers)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DCGROUPS, &CDConsoleDlg::OnNMRClickTreeDcgroups)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickMenuItem)
	ON_BN_CLICKED(IDC_BUTTON_CREATEDG, &CDConsoleDlg::OnBnClickedButtonCreatedg)
	ON_BN_CLICKED(IDC_BUTTON_CREATETG, &CDConsoleDlg::OnBnClickedButtonCreatetg)
	ON_BN_CLICKED(IDC_BUTTON_GETRECFLIE, &CDConsoleDlg::OnBnClickedButtonGetrecflie)
	ON_BN_CLICKED(IDC_BUTTON_DCINFO, &CDConsoleDlg::OnBnClickedButtonDcinfo)
	ON_BN_CLICKED(IDC_BUTTON_GISSTATUS, &CDConsoleDlg::OnBnClickedButtonGisstatus)
	ON_BN_CLICKED(IDC_BUTTON_GISREPORT, &CDConsoleDlg::OnBnClickedButtonGisreport)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_INCOMMING_CALL, &CDConsoleDlg::OnIncommingCall)
	ON_MESSAGE(WM_INCOMMING_VIDEO, &CDConsoleDlg::OnIncommingVideo)
	ON_MESSAGE(WM_START_REAL_PLAY, &CDConsoleDlg::StartRealPlay)
	ON_BN_CLICKED(IDC_BUTTON_DISPATCHVIDEO, &CDConsoleDlg::OnBnClickedButtonDispatchvideo)
	ON_BN_CLICKED(IDC_BUTTON_UNDISPATCH, &CDConsoleDlg::OnBnClickedButtonUndispatch)
	ON_BN_CLICKED(IDC_BUTTON_SDS, &CDConsoleDlg::OnBnClickedButtonSds)
	ON_BN_CLICKED(IDC_BUTTON4, &CDConsoleDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_EVENT11, &CDConsoleDlg::OnBnClickedButtonEvent11)
	ON_MESSAGE(WM_INIT, &CDConsoleDlg::InitDlg)
	ON_BN_CLICKED(IDC_BTN_STARTVWALL, &CDConsoleDlg::OnBnClickedBtnStartvwall)
	ON_BN_CLICKED(IDC_BTN_STOPVWALL, &CDConsoleDlg::OnBnClickedBtnStopvwall)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOURCE, &CDConsoleDlg::OnSelchangeComboResource)
	ON_BN_CLICKED(IDC_BTN_REFRESHLIST, &CDConsoleDlg::OnBnClickedBtnRefreshlist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnDblclkListDcusers)
	ON_BN_CLICKED(IDC_BUTTON_CREATEPG, &CDConsoleDlg::OnBnClickedButtonCreatepg)
	ON_BN_CLICKED(IDC_BUTTON_PATCHGROUPS, &CDConsoleDlg::OnBnClickedButtonPatchgroups)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PATCHGROUPS, &CDConsoleDlg::OnRclickTreePatchgroups)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, &CDConsoleDlg::OnBnClickedButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_GETGISSUB, &CDConsoleDlg::OnBnClickedButtonGetgissub)
END_MESSAGE_MAP()


// CDConsoleDlg message handlers

BOOL CDConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//Set the list box for radio mode
	m_DcUsers.ModifyStyle(0, LVS_SINGLESEL);
	m_DcUsers.InsertColumn(DCUSERS_RESID, _T("ResourceID"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_STATUS, _T("UserStatus"), LVCFMT_LEFT, 100);
	m_DcUsers.InsertColumn(DCUSERS_NAME, _T("UserName"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_CATEGORY, _T("UserCategory"), LVCFMT_LEFT, 85);
	m_DcUsers.InsertColumn(DCUSERS_PRIORITY, _T("UserPriority"), LVCFMT_LEFT, 85);

	//image list
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

	//Set the List style: select all, sub column support picture
	m_DcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP);

	//Set coordinate
	/*GetWindowRect(&m_RectMax);
	m_RectMin = m_RectMax;
	CRect rtSplit;
	GetDlgItem(IDC_STATIC_DCSPLIT)->GetWindowRect(&rtSplit);
	m_RectMin.right = rtSplit.left;
	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);*/

	//Set coordinate
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDConsoleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (NULL != m_pELTE_SDK_DemoDlg && NULL != m_pELTE_SDK_DemoDlg->m_hWnd)
	{
		//ELTE_SDK_SetEventCallBack(NULL, NULL);
		m_pELTE_SDK_DemoDlg->PostMessage(WM_CLOSE);
		m_pELTE_SDK_DemoDlg = NULL;
	}

	CDialogEx::OnClose();
}


void CDConsoleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//static BOOL s_bMax = FALSE;
	//if (s_bMax)
	//{
	//	SetWindowPos(NULL,0,0,m_RectMin.Width(),m_RectMin.Height(),SWP_NOMOVE);
	//}
	//else
	//{
	//	SetWindowPos(NULL,0,0,m_RectMax.Width(),m_RectMax.Height(),SWP_NOMOVE);
	//	GetDlgItem(IDC_EDIT_EVENT)->SetWindowText(_T(""));
	//}
	//s_bMax = !s_bMax;;
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CDConsoleDlg::OnBnClickedButtonDcusers()
{
	// TODO: Add your control notification handler code here
	m_DcUsers.ShowWindow(SW_SHOW);
	m_PatchGroups.ShowWindow(SW_HIDE);
	m_DcGroups.ShowWindow(SW_HIDE);

	//if (0 < m_DcUsers.GetItemCount())
	//{
	//	m_DcUsers.DeleteAllItems();
	//}
	//GetDcUsers();
}


void CDConsoleDlg::OnBnClickedButtonDcgroups()
{
	// TODO: Add your control notification handler code here
	m_DcUsers.ShowWindow(SW_HIDE);
	m_DcGroups.ShowWindow(SW_SHOW);
	m_PatchGroups.ShowWindow(SW_HIDE);

	//if (0 < m_DcGroups.GetCount())
	//{
	//	ClearDcGroupsTree();
	//	m_DcGroups.DeleteAllItems();
	//}
	//GetDcGroups();
}


void CDConsoleDlg::OnBnClickedButtonTrigger()
{
	// TODO: Add your control notification handler code here
	// Trigger status report
	ELTE_INT32 iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_TriggerStatusReport"));
}


void CDConsoleDlg::OnBnClickedButtonProvinit()
{
	// TODO: Add your control notification handler code here
	/*ELTE_INT32 iRet = ELTE_SDK_ProvisionManagerInit(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strServerIP).c_str(), eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strName).c_str());
	if (0 != iRet)
	{
		CString strMsg;
		strMsg.Format(_T("ELTE_SDK_ProvisionManagerInit failed. (%d)"), iRet);
		MessageBox(strMsg);
	}
	else
	{
		InitDlg();
		MessageBox(_T("ELTE_SDK_ProvisionManagerInit success."));
	}*/
}


void CDConsoleDlg::OnBnClickedButtonProvexit()
{
	// TODO: Add your control notification handler code here
	/*ELTE_INT32 iRet = ELTE_SDK_ProvisionManagerExit();
	if (0 != iRet)
	{
		CString strMsg;
		strMsg.Format(_T("ELTE_SDK_ProvisionManagerExit failed. (%d)"), iRet);
		MessageBox(strMsg);
	}
	else
	{
		ClearProv();
		MessageBox(_T("ELTE_SDK_ProvisionManagerExit success."));
	}*/
}


void CDConsoleDlg::AddImage(CImageList& imageList, UINT id)
{
	CBitmap bm;
	bm.LoadBitmap(id);
	imageList.Add(&bm, RGB(0, 0, 0));
}


afx_msg LRESULT CDConsoleDlg::InitDlg(WPARAM wParam, LPARAM lParam)
{
	// clear
	ClearProv();

	//GetDlgItem(IDC_BUTTON_PROVINIT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(TRUE);

	// get users list
	GetDcUsers(_T(""), false);
	// get group list
	GetDcGroups(_T(""), false);
	// get patch group list
	GetPatchGroups(_T(""), false);

	// Trigger status report
	int iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_TriggerStatusReport"));


	//iRet = ELTE_SDK_ProvisionManagerInitMRS(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strMRSIP).c_str());
	//CHECK_API_RETURN(iRet, _T("ELTE_SDK_ProvisionManagerInitMRS"));


	return 0;
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


void CDConsoleDlg::GetDcUsers(const CString strSearch, bool bFlag)
{
	ELTE_CHAR* pDcUsers = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcUsers(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strName).c_str(), &pDcUsers);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetDcUsers"));

	UserInfoList users;
	CXml::Instance().XmlParseDcUsers(pDcUsers, users);
	ELTE_SDK_ReleaseBuffer(pDcUsers);
	pDcUsers = NULL;

	UserInfoList::iterator itor = users.begin();
	for (; users.end() != itor; itor++)
	{
		UserInfo* pUserInfo = *itor;
		if (false == bFlag || (-1) != pUserInfo->UserName.Find(strSearch))
		{
			int item = m_DcUsers.InsertItem(m_DcUsers.GetItemCount(), _T(""), GetUserImageId(pUserInfo->UserCategory, true));
			m_DcUsers.SetItemText(item, DCUSERS_RESID, pUserInfo->UserID);
			m_DcUsers.SetItemText(item, DCUSERS_STATUS, _T("Offline"));
			m_DcUsers.SetItemText(item, DCUSERS_NAME, pUserInfo->UserName);
			m_DcUsers.SetItemText(item, DCUSERS_CATEGORY, pUserInfo->UserCategory);
			m_DcUsers.SetItemText(item, DCUSERS_PRIORITY, pUserInfo->UserPriority);
		}		
	}
}


void CDConsoleDlg::GetDcGroups(const CString strSearch, bool bFlag)
{
	ELTE_CHAR* pDcGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcGroups(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strName).c_str(), &pDcGroups);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetDcGroups"));

	// get group list
	GroupInfoList groups;
	CXml::Instance().XmlParseDcGroups(pDcGroups, groups);
	ELTE_SDK_ReleaseBuffer(pDcGroups);
	pDcGroups = NULL;

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		CString szItemroot(pInfo->GroupID);
		szItemroot.Append(_T(" "));
		szItemroot.Append(GetGroupCategoryString(pInfo->GroupCategory));
		HTREEITEM itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
		m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

		// List of users within the group
		ELTE_CHAR* pGroupUsers = NULL;
		iRet = ELTE_SDK_GetGroupUsers(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupUsers);
		if (0 != iRet)
		{
			continue;
		}

		// get users list
		GroupUserInfoList groupUsers;
		CXml::Instance().XmlParseGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);
		ELTE_SDK_ReleaseBuffer(pGroupUsers);
		pGroupUsers = NULL;

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


void CDConsoleDlg::GetPatchGroups(const CString strSearch, bool bFlag)
{
	ELTE_CHAR* pDcGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetPatchGroups(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strName).c_str(), &pDcGroups);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetPatchGroups"));

	// get group list
	GroupInfoList groups;
	CXml::Instance().XmlParsePatchGroups(pDcGroups, groups);
	ELTE_SDK_ReleaseBuffer(pDcGroups);
	pDcGroups = NULL;

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		CString szItemroot(pInfo->GroupID);
		szItemroot.Append(_T(" "));
		szItemroot.Append(_T("[Patch group]"));
		HTREEITEM itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
		m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

		// List of users within the group
		ELTE_CHAR* pGroupUsers = NULL;
		iRet = ELTE_SDK_GetGroupMemberByPatchId(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupUsers);
		if (0 != iRet)
		{
			continue;
		}

		// get users list
		GroupUserInfoList groupUsers;
		CXml::Instance().XmlParsePatchGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);
		ELTE_SDK_ReleaseBuffer(pGroupUsers);
		pGroupUsers = NULL;

		GroupUserInfoList::iterator it = groupUsers.begin();
		for (; groupUsers.end() != it; it++)
		{
			GroupUserInfo* &pUserInfo = *it;
			CString szItem(pUserInfo->MemberGroupId);
			szItem.Append(_T(" "));
			szItem.Append(_T("[Common group]"));
			HTREEITEM item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
			m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
		}

		m_PatchGroups.Expand(itemroot,TVE_EXPAND);
	}
}

void CDConsoleDlg::AddGroup(const char* groupId)
{
	// Query the group in the group list
	HTREEITEM itemroot = m_DcGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(itemroot);
		if (pInfo->GroupID == CString(groupId))
		{
			return;
		}
		itemroot = m_DcGroups.GetNextSiblingItem(itemroot);
	}

	// Get group information
	ELTE_CHAR* pGroupInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGroupInfo(groupId, &pGroupInfo);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGroupInfo"));

	CString strGroupID = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupID"));
	CString strGroupCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupCategory"));
	CString strGroupPriority = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupPriority"));
	CString strGroupName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("GroupName"));
	ELTE_SDK_ReleaseBuffer(pGroupInfo);
	pGroupInfo = NULL;

	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID = strGroupID;
	pInfo->GroupCategory = strGroupCategory;
	pInfo->GroupPriority = strGroupPriority;
	pInfo->GroupName = strGroupName;

	// Add group
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" "));
	szItemroot.Append(GetGroupCategoryString(pInfo->GroupCategory));
	itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

	// Get group users list
	ELTE_CHAR* pGroupUsers;
	iRet = ELTE_SDK_GetGroupUsers(groupId, &pGroupUsers);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGroupUsers"));

	// Get users list
	GroupUserInfoList groupUsers;
	CXml::Instance().XmlParseGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);
	ELTE_SDK_ReleaseBuffer(pGroupUsers);
	pGroupUsers = NULL;

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

void CDConsoleDlg::DelPatchGroupMember(const char* groupId, const char* memberId)
{
	// Query the group in the group list
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == CString(groupId))
		{
			HTREEITEM item = m_PatchGroups.GetChildItem(itemroot);
			while (item)
			{
				GroupUserInfo* pInfo = (GroupUserInfo*)m_PatchGroups.GetItemData(item);
				if (pInfo->MemberGroupId == CString(memberId))
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

void CDConsoleDlg::AddPatchGroupMember(const char* groupId, const char* memberId)
{
	// Query the group in the group list
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == CString(groupId))
		{
			HTREEITEM item = m_PatchGroups.GetChildItem(itemroot);
			while (item)
			{
				GroupUserInfo* pInfo = (GroupUserInfo*)m_PatchGroups.GetItemData(item);
				if (pInfo->MemberGroupId == CString(memberId))
				{
					return;
				}
				item = m_PatchGroups.GetNextSiblingItem(item);
			}	

			GroupUserInfo* pUserInfo = new GroupUserInfo;
			pUserInfo->GroupID = eLTE_Tool::ANSIToUnicode(groupId);
			pUserInfo->MemberGroupId = eLTE_Tool::ANSIToUnicode(memberId);
			CString szItem(pUserInfo->MemberGroupId);
			szItem.Append(_T(" "));
			szItem.Append(_T("[Common group]"));
			item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
			m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
			break;
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}
	m_PatchGroups.Expand(itemroot,TVE_EXPAND);
}

void CDConsoleDlg::AddPatchGroup(const char* groupId)
{
	// Query the group in the group list
	HTREEITEM itemroot = m_PatchGroups.GetRootItem();
	while (itemroot)
	{
		GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(itemroot);
		if (pInfo->GroupID == CString(groupId))
		{
			return;
		}
		itemroot = m_PatchGroups.GetNextSiblingItem(itemroot);
	}

	// get group info
	ELTE_CHAR* pGroupInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetPatchGroupInfo(groupId, &pGroupInfo);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetPatchGroupInfo"));

	CString strGroupID			= GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupNumber"));
	CString strGroupCreator	    = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("SetupDcId"));
	CString strGroupPriority	= GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("PGPriority"));
	CString strGroupName		= GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("PGName"));
	CString strGroupIndex		= GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("DcPatchIndex"));
	CString strGroupVPNID		= GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("VPNID"));
	ELTE_SDK_ReleaseBuffer(pGroupInfo);
	pGroupInfo = NULL;

	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID			= strGroupID;
	pInfo->GroupCreator	    = strGroupCreator;
	pInfo->GroupPriority	= strGroupPriority;
	pInfo->GroupName		= strGroupName;
	pInfo->GroupIndex		= strGroupIndex;
	pInfo->GroupVPNID		= strGroupVPNID;

	// add group
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" "));
	szItemroot.Append(_T("[Patch group]"));
	itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

	// Get group user list
	ELTE_CHAR* pGroupUsers;
	iRet = ELTE_SDK_GetGroupMemberByPatchId(groupId, &pGroupUsers);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGroupMemberByPatchId"));

	// Get user list
	GroupUserInfoList groupUsers;
	CXml::Instance().XmlParsePatchGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);
	ELTE_SDK_ReleaseBuffer(pGroupUsers);
	pGroupUsers = NULL;

	GroupUserInfoList::iterator it = groupUsers.begin();
	for (; groupUsers.end() != it; it++)
	{
		GroupUserInfo* &pUserInfo = *it;
		CString szItem(pUserInfo->MemberGroupId);
		szItem.Append(_T(" "));
		szItem.Append(_T("[Common group]"));
		HTREEITEM item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
		m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
	}

	m_PatchGroups.Expand(itemroot,TVE_EXPAND);
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
				//Heartbeat mechanism can not cover the original state as an online state.begin
				CString sStatus = m_DcUsers.GetItemText(i, DCUSERS_STATUS);
				if (sStatus != _T("Offline"))
				{
					return;
				}
				//Heartbeat mechanism can not cover the original state as an online state.end

				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Online"));
			}
			else if (iStatus == 4012)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Offline"));
			}
			else if (iStatus == 4020)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Calling"));
			}
			else if (iStatus == 4021)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Ringing"));
			}
			else if (iStatus == 4022)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Using"));
			}
			else if (iStatus == 4023)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Online"));
			}
		}
	}
}


CString CDConsoleDlg::GetGroupCategoryString(const CString& strGroupCategory)
{
	int iGroupCategory = StrToInt(strGroupCategory);
	CString strResult;

	switch (iGroupCategory) // Group type
	{
	case 0:
		{
			strResult.Append(_T("[Whole network broadcasting group]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[Common group]"));
		}
		break;
	case 2:
		{
			strResult.Append(_T("[Regional broadcasting group]"));
		}
		break;
	case 8:
		{
			strResult.Append(_T("[External group]"));
		}
		break;
	case 9:
		{
			strResult.Append(_T("[Dynamic group]"));
		}
		break;
	case 10:
		{
			strResult.Append(_T("[Regardless of type]"));
		}
		break;
	}

	return strResult;
}


CString CDConsoleDlg::GetGroupUserMemberTypeString(const CString& strMemberType)
{
	int iMeberType = StrToInt(strMemberType);
	CString strResult;

	switch (iMeberType) // User type
	{
	case 0:
		{
			strResult.Append(_T("[The member of dynamic group]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[Common user Member]"));
		}
		break;
	}

	return strResult;
}


int CDConsoleDlg::GetUserImageId(const CString& strCategory, bool bGray)
{
	int iUserCategory = StrToInt(strCategory);
	int iImgID = -1;

	switch (iUserCategory) // user type
	{
	case 0:
		{
			// dc user
			iImgID = bGray ? Img_dispatcher_offline : Img_dispatcher_online;
		}
		break;
	case 1:
		{
			// camera
			iImgID = bGray ? Img_camera_offline : Img_camera_online;
		}
		break;
	case 2:
		{
			// PSTN user
			iImgID = bGray ? Img_gwuser_offline : Img_gwuser_online;
		}
		break;
	case 9:
		{
			// PTT user
			iImgID = bGray ? Img_pttuser_offline : Img_pttuser_online;
		}
		break;
	}

	return iImgID;
}

void CDConsoleDlg::RemovePatchGroup(int iGroupID)
{
	CString strGroupID;
	strGroupID.Format(_T("%d"), iGroupID);

	// Query this group in the group list
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

void CDConsoleDlg::RemoveGroup(int iGroupID)
{
	CString strGroupID;
	strGroupID.Format(_T("%d"), iGroupID);

	// Query this group in the group list
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
	// Query this group in the group list
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

	// Construct group information
	GroupInfo* pInfo = new GroupInfo;
	pInfo->GroupID = strGroupID;
	pInfo->GroupCategory = "-1";
	pInfo->GroupPriority = "-1";
	pInfo->GroupName = "-1";

	// add group
	CString szItemroot(pInfo->GroupID);
	szItemroot.Append(_T(" [Temporary group]"));
	itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
	m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);
	m_DcGroups.SelectItem(itemroot);
	m_DcGroups.Expand(itemroot,TVE_EXPAND);
}


int CDConsoleDlg::GetUserImageId(const CString& strUserId, int iStatus)
{
	int iImgID = -1;
	ELTE_CHAR* pUserInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetUserInfo(eLTE_Tool::UnicodeToANSI(strUserId).c_str(), &pUserInfo);
	CHECK_API_RETURN_RET(iRet, _T("ELTE_SDK_GetUserInfo"), iImgID);

	CString strUserCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserCategory"));
	ELTE_SDK_ReleaseBuffer(pUserInfo);
	pUserInfo = NULL;

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

	// Pop-up Menu
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_MENU_GETUSERINFO, _T("GetUserInfo"));
	
	POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
	int item = m_DcUsers.GetNextSelectedItem(pos);
	CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

	// To determine whether the camera
	ELTE_CHAR* pUserInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetUserInfo(eLTE_Tool::UnicodeToANSI(strResID).c_str(), &pUserInfo);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetUserInfo"));

	CString strCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserCategory"));
	ELTE_SDK_ReleaseBuffer(pUserInfo);
	pUserInfo = NULL;

	if (_T("1") == strCategory || _T("9") == strCategory)
	{
		menu.AppendMenu(MF_STRING, ID_MENU_STARTREALPLAY, _T("StartRealPlay"));
		menu.AppendMenu(MF_STRING, ID_MENU_STOPREALPLAY, _T("StopRealPlay"));
	}
	if (_T("1") == strCategory)
	{
		menu.AppendMenu(MF_STRING, ID_MENU_PTZCONTROL, _T("PTZControl"));
	}

	CMenu p2pMenu;
	p2pMenu.CreatePopupMenu();//
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PTRANSFER, _T("Transfer"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PDIAL, _T("Dial"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PHANGUP, _T("Hangup"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PBREAKIN, _T("Breakin"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PBREAKOFF, _T("Breakoff"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PMUTE, _T("Mute"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PUNMUTE, _T("UnMute"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PSTARTDISLISTEN, _T("DisListen_Start"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PSTOPDISLISTEN, _T("DisListen_Stop"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PSTARTENVLISTEN, _T("EnvListen_Start"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PHALFDPXDIAL, _T("HalfDpxDial"));
	p2pMenu.AppendMenu(MF_STRING, ID_MENU_P2PHALFDPXRELEASE, _T("HalfDpxRelease"));
	if(_T("1") == strCategory || _T("2") == strCategory)
	{

	}
	else
	{
		menu.AppendMenu(MF_POPUP, (UINT)p2pMenu.m_hMenu, _T("P2P"));
	}
	CString sStatus = m_DcUsers.GetItemText(nItem, DCUSERS_STATUS);
	if (sStatus == _T("Offline"))
	{
		p2pMenu.EnableMenuItem(ID_MENU_P2PTRANSFER, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PDIAL, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PHANGUP, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PBREAKIN, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PBREAKOFF, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PMUTE, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PUNMUTE, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PHALFDPXDIAL, MF_GRAYED);
		p2pMenu.EnableMenuItem(ID_MENU_P2PHALFDPXRELEASE, MF_GRAYED);
	}

	if (_T("9") == strCategory)
	{
		CMenu gisMenu;
		gisMenu.CreatePopupMenu();//
		gisMenu.AppendMenu(MF_STRING, ID_MENU_GISSUBSCRIBE, _T("Subscribe"));
		gisMenu.AppendMenu(MF_STRING, ID_MENU_GISUNSUBSCRIBE, _T("UnSubscribe"));
		//gisMenu.AppendMenu(MF_STRING, ID_MENU_GETGISCFG, _T("GetCfg"));
		//gisMenu.AppendMenu(MF_STRING, ID_MENU_SETGISCFG, _T("SetCfg"));
		menu.AppendMenu(MF_POPUP, (UINT)gisMenu.m_hMenu, _T("GIS"));
	}

	CPoint pmenu;
	::GetCursorPos(&pmenu);
	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
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

		CMenu pttMenu;
		pttMenu.CreatePopupMenu();
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTDIAL, _T("Dial"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTRELEASE, _T("Release"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTHANGUP, _T("Hangup"));
		
		pttMenu.AppendMenu(MF_STRING, ID_MENU_GROUPBREAKOFF, _T("Breakoff"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTMUTE, _T("Mute"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTUNMUTE, _T("UnMute"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTDISLISTEN, _T("DisListen_Start"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTOPDISLISTEN, _T("DisListen_Stop"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTENVLISTEN, _T("EnvListen_Start"));


		//Temporary group
		if (pInfo->GroupCategory == "-1")
		{
			pttMenu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
			return;
		}
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTEMERGENCY, _T("Emergency"));
		//Group node
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MENU_GETGROUPINFO, _T("GetGroupInfo"));
		//menu.AppendMenu(MF_STRING, ID_MENU_SUBGROUP, _T("SubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_UNSUBGROUP, _T("UnSubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_SUBJOINGROUP, _T("SubJoinGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_JOINCALL, _T("JoinCall"));

		//Dynamic group
		if (pInfo->GroupCategory == "9")
		{
			menu.AppendMenu(MF_STRING, ID_MENU_RMDGROUP, _T("RemoveDynamicGroup"));
			menu.AppendMenu(MF_STRING, ID_MENU_MODIFYDGROUP, _T("Modify"));
		}

		menu.AppendMenu(MF_POPUP, (UINT)pttMenu.m_hMenu, _T("PTT"));
		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
	else
	{
		//User node
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MENU_GROUPUSERINFO, _T("GetGroupUserInfo"));

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
}


void CDConsoleDlg::OnClickMenuItem(UINT uID)
{
	switch (uID)
	{
	case ID_MENU_GETUSERINFO:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_CHAR* pUserInfo = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetUserInfo(eLTE_Tool::UnicodeToANSI(strResID).c_str(), &pUserInfo);
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetUserInfo"));

			CString strUserID = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserID"));
			CString strUserCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserCategory"));
			CString strUserPriority = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserPriority"));
			CString strUserName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pUserInfo), _T("UserName"));
			ELTE_SDK_ReleaseBuffer(pUserInfo);
			pUserInfo = NULL;

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("UserID"), strUserID);
			dlg.SetInfo(2, _T("UserCategory"), strUserCategory);
			dlg.SetInfo(3, _T("UserPriority"), strUserPriority);
			dlg.SetInfo(4, _T("UserName"), strUserName);
			dlg.DoModal();
		}
		break;
	case ID_MENU_STARTREALPLAY:
		{
			VideoParamInfo info;
			CVideoParamDlg dlg(this);
			dlg.SetVideoParamInfo(&info);
			int iExit = dlg.DoModal();
			if (IDOK != iExit)
			{
				return;
			}

			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			// Structural parameters
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

			ELTE_INT32 iRet = ELTE_SDK_StartRealPlay(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strVideoParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StartRealPlay"));
		}
		break;
	case ID_MENU_STOPREALPLAY:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			ELTE_INT32 iRet = ELTE_SDK_StopRealPlay(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StopRealPlay"));

			CMediaPlayerDlgMgr::Instance().DeleteCreateMediaPlayerDlg(eLTE_Tool::UnicodeToANSI(strResID).c_str());
		}
		break;
	case ID_MENU_PTZCONTROL:
		{
			ELTE_UINT32 uiCode, uiValue;
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

			ELTE_INT32 iRet = ELTE_SDK_PTZControl(eLTE_Tool::UnicodeToANSI(strResID).c_str(), uiCode, uiValue);
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTZControl"));
		}
		break;
	case ID_MENU_P2PDIAL:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PDial(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PDial"));
		}
		break;
	case ID_MENU_P2PHALFDPXDIAL:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PHalfDpxDial(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PHalfDpxDail"));
		}
		break;
	case ID_MENU_P2PHALFDPXRELEASE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PHalfDpxRelease(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PHalfDpxRelease"));
		}
		break;
	case ID_MENU_P2PHANGUP:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PHangup(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PHangup"));
		}
		break;
	case ID_MENU_P2PTRANSFER:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			P2PTransferDlg dlg(NULL);
			dlg.SetParamInfo(((CeLTE_SDK_DemoDlg*)m_pELTE_SDK_DemoDlg)->GetName(), strResID);
			dlg.DoModal();
		}
		break;
	case ID_MENU_P2PBREAKIN:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PBreakin(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PBreakin"));
		}
		break;
	case ID_MENU_P2PBREAKOFF:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			ELTE_INT32 iRet = ELTE_SDK_P2PBreakoff(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_P2PBreakoff"));
		}
		break;
	case ID_MENU_P2PMUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strMuteParam("<Content><MuteParam><CallType>0</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolMute(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolMute"));
		}
		break;
	case ID_MENU_P2PUNMUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strMuteParam("<Content><MuteParam><CallType>0</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolUnMute(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolUnMute"));
		}
		break;
	case ID_MENU_GISSUBSCRIBE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strGISParam;
			strGISParam.Format(_T("<Content><GISParam><SubType>7</SubType><ResourceList>%s</ResourceList><Subscriber></Subscriber></GISParam></Content>"), strResID);
			ELTE_INT32 iRet = ELTE_SDK_GISSubscribe(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strGISParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GISSubscribe"));
		}
		break;
	case ID_MENU_GISUNSUBSCRIBE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strGISParam;
			strGISParam.Format(_T("<Content><GISParam><SubType>8</SubType><ResourceList>%s</ResourceList><Subscriber></Subscriber></GISParam></Content>"), strResID);
			ELTE_INT32 iRet = ELTE_SDK_GISSubscribe(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strGISParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GISSubscribe"));
		}
		break;
	case ID_MENU_GETGROUPINFO:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			ELTE_CHAR* pGroupInfo = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetGroupInfo(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupInfo);
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGroupInfo"));

			CString strGroupID = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupID"));
			CString strGroupCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupCategory"));
			CString strGroupPriority = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupPriority"));
			CString strGroupName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("GroupName"));
			CString strGroupCreator = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("GroupCreator"));
			ELTE_SDK_ReleaseBuffer(pGroupInfo);
			pGroupInfo = NULL;

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
	case ID_MENU_JOINCALL:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			CTempUserJoinGrpCall dlg(NULL);
			dlg.SetParamInfo(((CeLTE_SDK_DemoDlg*)m_pELTE_SDK_DemoDlg)->GetName(), pInfo->GroupID);
			dlg.DoModal();
		}
		break;
	case ID_MENU_GETPATCHGROUPINFO:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);

			ELTE_CHAR* pGroupInfo = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetPatchGroupInfo(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupInfo);
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetPatchGroupInfo"));

			CString strGroupID = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupNumber"));
			CString strSetupDcId = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("SetupDcId"));
			CString strGroupPriority = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("PGPriority"));
			CString strGroupName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pGroupInfo), _T("PGName"));
			ELTE_SDK_ReleaseBuffer(pGroupInfo);
			pGroupInfo = NULL;

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("GroupID"), strGroupID);
			dlg.SetInfo(2, _T("GroupCreator"), strSetupDcId);
			dlg.SetInfo(3, _T("GroupPriority"), strGroupPriority);
			dlg.SetInfo(4, _T("GroupName"), strGroupName);
			dlg.DoModal();
		}
		break;
	case ID_MENU_SUBGROUP:
		{
			/*HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			ELTE_INT32 iRet = ELTE_SDK_SubscribeGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), 1);
			CHECK_API_RETURN_VOID(iRet, _T("SubscribeGroup"));*/
		}
		break;
	case ID_MENU_UNSUBGROUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_UnSubscribeGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("UnSubscribeGroup"));
		}
		break;
	case ID_MENU_SUBJOINGROUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_SubJoinGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("SubJoinGroup"));
		}
		break;
	case ID_MENU_RMDGROUP:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			ELTE_INT32 iRet = ELTE_SDK_CancelDynamicGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			if (iRet == 0)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			else if (iRet == -40001)
			{
				MessageBox(_T("No subscription to this dynamic group."));
			}
			else if (iRet == -40005)
			{
				MessageBox(_T("The user does not have access."));
			}
			else 
			{
				CString str;
				str.Format(_T("ELTE_SDK_CancelDynamicGroup Failed. (%d)"), iRet);
				MessageBox(str);
			}
		}
		break;
	case ID_MENU_MODIFYDGROUP:
		{
			ModifyDGNAParamInfo info;
			CModifyDGNADlg dlg(this);
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			dlg.SetDGNAParamInfo(CeLTE_SDK_DemoDlg::Instance().m_strName, pInfo->GroupID, &info);
			int iExit = dlg.DoModal();
			if (IDOK != iExit)
			{
				return;
			}
			
			m_ModifyDGNAGroupId = pInfo->GroupID;
			// XML
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

			ELTE_INT32 iRet = ELTE_SDK_ModifyDynamicGroup(eLTE_Tool::UnicodeToANSI(info.DcID).c_str(), eLTE_Tool::UnicodeToANSI(strDGNAParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_CreateDynamicGroup"));
		}
		break;
	case ID_MENU_ADDPATCHROUPMEMBER:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);
			OperatePatchGroup(CeLTE_SDK_DemoDlg::Instance().m_strName, pInfo->GroupID, 1, pInfo->GroupName);
		}
		break;
	case ID_MENU_DELPATCHROUPMEMBER:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);
			OperatePatchGroup(CeLTE_SDK_DemoDlg::Instance().m_strName, pInfo->GroupID, 2, pInfo->GroupName);
		}
		break;
	case ID_MENU_RMPATCHROUP:
		{
			HTREEITEM selectedItem = m_PatchGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_PatchGroups.GetItemData(selectedItem);

			ELTE_INT32 iRet = ELTE_SDK_CancelPatchGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
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
	case ID_MENU_GROUPUSERINFO:
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
	case  ID_MENU_SETGISCFG:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CSetGisCfgDlg dlg(NULL);
			dlg.SetParamInfo(strResID);
			dlg.DoModal();
		}
		break;
	case ID_MENU_GETGISCFG:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			
			
			ELTE_CHAR* pUeGisCfg = NULL;

			//ELTE_INT32 iRet = ELTE_SDK_GetUserSpecificGISCfg(eLTE_Tool::UnicodeToANSI(strResID).c_str(), &pUeGisCfg);
			//CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetUserSpecificGISCfg"));

			CString xmlStr(pUeGisCfg);

			CString strPeriod = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ReportPeriod"));
			CString strDistance = GET_XML_ELEM_VALUE_STR(xmlStr, _T("ReportDistance"));

			CInfoDlg dlg(this);
			dlg.SetInfo(1, _T("Period"), strPeriod);
			dlg.SetInfo(2, _T("Distance"), strDistance);
			dlg.DoModal();
		}
		break;
	case ID_MENU_PTTDIAL:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_PTTDial(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTTDial"));
			
		}
		break;
	case ID_MENU_PTTRELEASE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_PTTRelease(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTTRelease"));
		}
		break;
	case ID_MENU_PTTHANGUP:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_PTTHangup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			if("-1" == pInfo->GroupCategory)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTTHangup"));
		}
		break;
	case ID_MENU_PTTEMERGENCY:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_PTTEmergency(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTTEmergency"));
		}
		break;
	case ID_MENU_GROUPBREAKOFF:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_GroupBreakoff(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			if("-1" == pInfo->GroupCategory)
			{
				RemoveGroup(StrToInt(pInfo->GroupID));
			}
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GroupBreakoff"));
		}
		break;
	case ID_MENU_PTTMUTE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			CString strMuteParam("<Content><MuteParam><CallType>1</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolMute(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolMute"));
		}
		break;
	case ID_MENU_PTTUNMUTE:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			CString strMuteParam("<Content><MuteParam><CallType>1</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolUnMute(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolUnMute"));
		}
		break;
	case ID_MENU_P2PSTARTDISLISTEN:
	case ID_MENU_PTTSTARTDISLISTEN:
		{
			//Start Discreet Listen
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID;
			if (ID_MENU_P2PSTARTDISLISTEN == uID)
			{
				strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			}
			else
			{
				GroupInfo* pInfo = NULL;
				GetSelGroupInfo(&pInfo);
				strResID = pInfo->GroupID;
			}			

			ELTE_INT32 iRet = ELTE_SDK_StartDiscreetListen(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StartDiscreetListen"));
		}
		break;
	case ID_MENU_P2PSTOPDISLISTEN:
	case ID_MENU_PTTSTOPDISLISTEN:
		{
			//Stop Discreet Listen
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			//get resourceID
			CString strResID;
			if (ID_MENU_P2PSTOPDISLISTEN == uID)
			{
				strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			}
			else
			{
				GroupInfo* pInfo = NULL;
				GetSelGroupInfo(&pInfo);
				strResID = pInfo->GroupID;
			}

			ELTE_INT32 iRet = ELTE_SDK_StopDiscreetListen(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StopDiscreetListen"));
		}
		break;
	case ID_MENU_P2PSTARTENVLISTEN:
	case ID_MENU_PTTSTARTENVLISTEN:
		{
			//Start Environment Listen
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID;
			if (ID_MENU_P2PSTARTENVLISTEN == uID)
			{
				strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			}
			else
			{
				GroupInfo* pInfo = NULL;
				GetSelGroupInfo(&pInfo);
				strResID = pInfo->GroupID;
			}

			ELTE_INT32 iRet = ELTE_SDK_StartEnvironmentListen(eLTE_Tool::UnicodeToANSI(strResID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StartEnvironmentListen"));
		}
		break;
	}
}

afx_msg LRESULT CDConsoleDlg::StartRealPlay(WPARAM wParam, LPARAM lParam)
{
	//create player dlg and show
	CMediaPlayerDlg* pMediaPlayerDlg = CMediaPlayerDlgMgr::Instance().CreateMediaPlayerDlg(m_strResId, this);

	if(pMediaPlayerDlg)
	{
		ELTE_INT32 iRet = ELTE_SDK_SetPlayWindow(m_strResId.c_str(), pMediaPlayerDlg->GetVideoStaticHwnd());
		CHECK_API_RETURN(iRet, _T("ELTE_SDK_SetPlayWindow"));
	}
	return 0;
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

void CDConsoleDlg::SetPlayResID(std::string strResId)
{
	m_strResId = strResId;
}

void CDConsoleDlg::OnBnClickedButtonAllresync()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_AllResync);
}


void CDConsoleDlg::OnBnClickedButtonResstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_ResStatus);
}


void CDConsoleDlg::OnBnClickedButtonUserstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_UserStatus);
}


void CDConsoleDlg::OnBnClickedButtonProvchange()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_ProvChange);
}


void CDConsoleDlg::OnBnClickedButtonModulestatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_ModuleStatus);
}


void CDConsoleDlg::OnBnClickedButtonVideostatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_VideoStatus);
}


void CDConsoleDlg::OnBnClickedButtonCallstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_CallStatus);
}


void CDConsoleDlg::OnBnClickedButtonGroupstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_GroupStatus);
}


void CDConsoleDlg::OnBnClickedButtonGisstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_GISStatus);
}


void CDConsoleDlg::OnBnClickedButtonGisreport()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_GISReport);
}


void CDConsoleDlg::OnBnClickedButtonCreatedg()
{
	// TODO: Add your control notification handler code here
	DGNAParamInfo info;
	CCreateGroupDlg dlg(this);
	dlg.SetDGNAParamInfo(CeLTE_SDK_DemoDlg::Instance().m_strName, &info, true);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// construct XML
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

	ELTE_INT32 iRet = ELTE_SDK_CreateDynamicGroup(eLTE_Tool::UnicodeToANSI(strDGNAParam).c_str());
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_CreateDynamicGroup"));
}


void CDConsoleDlg::OnBnClickedButtonCreatetg()
{
	// TODO: Add your control notification handler code here
	DGNAParamInfo info;
	CCreateGroupDlg dlg(this);
	dlg.SetDGNAParamInfo(CeLTE_SDK_DemoDlg::Instance().m_strName, &info, false);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	ELTE_INT32 groupId;
	ELTE_INT32 iRet = ELTE_SDK_GetTempGroupID(&groupId);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetTempGroupID"));

	// construct XML
	CString strDGNAParam;
	strDGNAParam.Append(_T("<Content>"));
	strDGNAParam.Append(_T("<GroupID>"));
	strDGNAParam.Append(IntToStr(groupId));
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

	iRet = ELTE_SDK_CreateTempGroup(eLTE_Tool::UnicodeToANSI(strDGNAParam).c_str());
	if (0 == iRet)
	{
		AddTempGroup(IntToStr(groupId));
	}
	else
	{
		CString str;
		str.Format(_T("ELTE_SDK_CreateTempGroup Failed. (%d)"), iRet);
		MessageBox(str);
	}
}


void CDConsoleDlg::OnBnClickedButtonGetrecflie()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_RECFileDlg.m_hWnd)
	{
		m_RECFileDlg.Create(CRECFileDlg::IDD, this);
	}
	m_RECFileDlg.ShowWindow(SW_SHOW);
}


void CDConsoleDlg::OnBnClickedButtonDcinfo()
{
	// TODO: Add your control notification handler code here
	ELTE_CHAR* pDcInfo = NULL;
	if(NULL == m_pELTE_SDK_DemoDlg)
	{
		return;
	}
	ELTE_INT32 iRet = ELTE_SDK_GetDcInfo(eLTE_Tool::UnicodeToANSI(((CeLTE_SDK_DemoDlg*)m_pELTE_SDK_DemoDlg)->GetName()).c_str(), &pDcInfo);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_GetDcInfo"));

	DcInfo dcInfo;
	dcInfo.DcID			= GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pDcInfo), _T("DcID"));
	dcInfo.Priviledge	= GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pDcInfo), _T("Priviledge"));
	dcInfo.Role			= GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pDcInfo), _T("Role"));
	dcInfo.Alias		= GET_XML_ELEM_VALUE_STR(eLTE_Tool::UTF8ToUnicode(pDcInfo), _T("Alias"));

	CXml::Instance().XmlParseDcInfo(eLTE_Tool::ANSIToUnicode(pDcInfo), dcInfo);

	CInfoDlg dlg(this);
	dlg.SetInfo(1, _T("DcID"), dcInfo.DcID);
	dlg.SetInfo(2, _T("Priviledge"), dcInfo.Priviledge);
	dlg.SetInfo(3, _T("Role"), dcInfo.Role);
	dlg.SetInfo(4, _T("Alias"), dcInfo.Alias);
	dlg.DoModal();

	ELTE_SDK_ReleaseBuffer(pDcInfo);
	pDcInfo = NULL;
}

afx_msg LRESULT CDConsoleDlg::OnIncommingVideo(WPARAM wParam, LPARAM lParam)
{
	/*if (NULL == m_VideoCommingDlg.m_hWnd)
	{
	m_VideoCommingDlg.Create(CallDlg::IDD, this);
	}

	CString strTitle("Incoming Video: ");
	strTitle.Append(m_strCommingVideo);
	m_VideoCommingDlg.SetWindowText(strTitle);

	m_VideoCommingDlg.SetResId(m_strCommingVideo);
	m_VideoCommingDlg.ShowWindow(SW_SHOW);*/

	VideoCommingDlg* pVideoCommingDlg = new VideoCommingDlg;
	if (NULL == pVideoCommingDlg)
	{
		return NULL;
	}
	pVideoCommingDlg->SetResId(m_strCommingVideo);

	pVideoCommingDlg->Create(IDD_DIALOG_VIDEO, NULL);

	CString strTitle("Incoming Video: ");
	strTitle.Append(m_strCommingVideo);
	pVideoCommingDlg->SetWindowText(strTitle);
	pVideoCommingDlg->ShowWindow(SW_SHOW);


	return 0;
}

afx_msg LRESULT CDConsoleDlg::OnIncommingCall(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_CallDlg.m_hWnd)
	{
		m_CallDlg.Create(CallDlg::IDD, this);
	}

	CString strTitle("Incoming Call: ");
	strTitle.Append(m_Caller);
	m_CallDlg.SetWindowText(strTitle);

	//INT_PTR nResponse = m_CallDlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	ELTE_INT32 iRet = ELTE_SDK_P2PRecv(m_Caller);
	//	CHECK_API_RETURN_RET(iRet, _T("ELTE_SDK_P2PRecv"), 0);
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	ELTE_INT32 iRet = ELTE_SDK_P2PReject(m_Caller);
	//	CHECK_API_RETURN_RET(iRet, _T("ELTE_SDK_P2PReject"), 0);
	//}

	m_CallDlg.SetCaller(m_Caller);
	m_CallDlg.ShowWindow(SW_SHOW);

	return 0;
}


BOOL CDConsoleDlg::PreTranslateMessage(MSG* pMsg)
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


void CDConsoleDlg::OnBnClickedButtonDispatchvideo()
{
	// TODO: Add your control notification handler code here

	DispatchParamInfo info;
	INT_PTR nRes;
	DispatchParamDlg dlg(this);
	info.strFormat = GetVideoFormat();
	info.strResourceId = GetTarResID();
	dlg.SetDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML format ---
	<Content>
		<ResourceID>res ID</ResourceID>
		<Fmtvalue></Fmtvalue>//D1¡¢CIF¡¢QCIF¡¢720P or 1080P
		<DispatchNum></DispatchNum>//Video source user number, fill in the video source terminal ID¡£
		<Dstviewerlist>//Video distribution destination terminal ID list
			<Dstviewer></Dstviewer>//Video distribution of the purpose of the terminal or terminal, fill out the purpose of terminal ID or the purpose of scheduling the station ID
		</Dstviewerlist>
		<Channel></Channel>//Reserved£»
	</Content>
	************************************************************************/
	CString strDispatchParam;
	strDispatchParam.Append(_T("<Content>"));
	strDispatchParam.Append(_T("<Fmtvalue>"));
	strDispatchParam.Append(info.strFormat);
	strDispatchParam.Append(_T("</Fmtvalue>"));
	strDispatchParam.Append(_T("<DispatchNum>"));
	strDispatchParam.Append(info.strResourceId);
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


	//CString strResult = m_peLTE_Player->ELTE_OCX_StartVideoDispatch(strResID, strDispatchParam);
	//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StartVideoDispatch"));
	//GetDlgItem(IDC_BUTTON_UNDISPATCH)->EnableWindow(TRUE);
	ELTE_INT32 iRet = ELTE_SDK_StartVideoDispatch(eLTE_Tool::UnicodeToANSI(info.strResourceId).c_str(),
		eLTE_Tool::UnicodeToANSI(strDispatchParam).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_StartVideoDispatch"));
}


void CDConsoleDlg::OnBnClickedButtonUndispatch()
{
	// TODO: Add your control notification handler code here
	UnDispatchParamInfo info;
	INT_PTR nRes;
	UnDispatchParamDlg dlg(this);
	info.strResId = GetTarResID();
	dlg.SetUnDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML format ---
	<Content>
		<ResourceID>The id of current distribution</ResourceID>
		<UserId>Need to stop a id of previous distribution</UserId>
	</Content>
	************************************************************************/
	CString strDispatchParam;
	strDispatchParam.Append(_T("<Content>"));
	strDispatchParam.Append(_T("<UserId>"));
	strDispatchParam.Append(info.strUsrId);
	strDispatchParam.Append(_T("</UserId>"));
	strDispatchParam.Append(_T("</Content>"));


	ELTE_INT32 iRet = ELTE_SDK_StopVideoDispatch(eLTE_Tool::UnicodeToANSI(info.strResId).c_str(),
		eLTE_Tool::UnicodeToANSI(strDispatchParam).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_StopVideoDispatch"));
}


void CDConsoleDlg::OnBnClickedButtonSds()
{
	// TODO: Add your control notification handler code here
	SDSParamInfo info;
	INT_PTR nRes;
	SDSMessage sdsMessage(this);
	sdsMessage.SetSDSParamInfo(((CeLTE_SDK_DemoDlg*)m_pELTE_SDK_DemoDlg)->GetName(),&info);
	nRes = sdsMessage.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML format ---
	<Content>
		<SDSType></SDSType>			//SDS type
		<MsgBody></MsgBody>	    //SDS body
		<Receiver></Receiver>	    //for example¡°1001¡±
		<AttachFileList>			//Annex address list
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

	ELTE_INT32 iRet = ELTE_SDK_SDSSendMessage(eLTE_Tool::UnicodeToANSI(((CeLTE_SDK_DemoDlg*)m_pELTE_SDK_DemoDlg)->GetName()).c_str(),
		eLTE_Tool::UnicodeToANSI(strSDSParam).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_SDSSendMessage"));
}


void CDConsoleDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENT)->SetWindowText(m_SDSReport);
}


void CDConsoleDlg::OnBnClickedButtonEvent11()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENT)->SetWindowText(m_SDSStatus);
}


//start vwall
void CDConsoleDlg::OnBnClickedBtnStartvwall()
{
	/************************************************************************/
	/* 
	--- XML format ---
	<Content>
	<VideoParam>
	<DstObjId ></DstObjId >// a video channel
	<StrFmt ></StrFmt >//Reserve
	</VideoParam>
	</Content>
	*/
	/************************************************************************/

	//Judge the wall information
	VideoChannelList *pVCL = getVideoChannelListPointer();

	if ("1" != (*pVCL)[m_VideoChannel] && "4023" != (*pVCL)[m_VideoChannel])
	{
		MessageBox(_T("The current state is not available"));
		return;
	}

	GetDlgItem(IDC_EDIT_RESOURCEID)->GetWindowText(m_ResourceID);
	// TODO: Add your control notification handler code here
	CString strVideoChannelStart;
	//Video wall XML
	//Structure parameter
	strVideoChannelStart.Append(_T("<Content>"));
	strVideoChannelStart.Append(_T("<VideoParam>"));
	strVideoChannelStart.Append(_T("<DstObjId>"));
	strVideoChannelStart.Append(m_VideoChannel);
	strVideoChannelStart.Append(_T("</DstObjId>"));
	strVideoChannelStart.Append(_T("<StrFmt>"));
	strVideoChannelStart.Append(_T("</StrFmt>"));
	strVideoChannelStart.Append(_T("</VideoParam>"));
	strVideoChannelStart.Append(_T("</Content>"));
	int iRet = ELTE_SDK_VWallStart(eLTE_Tool::UnicodeToANSI(m_ResourceID).c_str(),eLTE_Tool::UnicodeToANSI(strVideoChannelStart).c_str());
	if (0 != iRet)
	{
		MessageBox(_T("VWallStart failed."));
		return;
	}
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Sending Message..."));
	Sleep(2000);
	OnSelchangeComboResource();
}

//stop vwall
void CDConsoleDlg::OnBnClickedBtnStopvwall()
{
	/************************************************************************
	--- XML format ---
	<Content>
	<DstObjId>a video channel</DstObjId>
	</Content>
	************************************************************************/
	VideoChannelList* pVCL = getVideoChannelListPointer();

	if ("4022" != (*pVCL)[m_VideoChannel])
	{
		MessageBox(_T("Please wait a moment. The current state is not closed."));
		return;
	}

	GetDlgItem(IDC_EDIT_RESOURCEID)->GetWindowText(m_ResourceID);
	CString strVideoChannelStop;
	//xml
	strVideoChannelStop.Append(_T("<Content>"));
	strVideoChannelStop.Append(_T("<DstObjId>"));
	strVideoChannelStop.Append(m_VideoChannel);
	strVideoChannelStop.Append(_T("</DstObjId>"));
	strVideoChannelStop.Append(_T("</Content>"));

	int iRet = ELTE_SDK_VWallStop(eLTE_Tool::UnicodeToANSI(m_ResourceID).c_str(),eLTE_Tool::UnicodeToANSI(strVideoChannelStop).c_str());
	if (0 != iRet)
	{
		MessageBox(_T("VWallStop failed."));
		return;
	}
	//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopVideoDispatch"));
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Sending Message..."));
	Sleep(2000);
	OnSelchangeComboResource();
}


void CDConsoleDlg::OnSelchangeComboResource()
{
	int iSel;
	iSel = m_ComboVWIDList.GetCurSel();
	m_ComboVWIDList.GetLBText(iSel,m_VideoChannel);
//	m_ComboVWIDList.ResetContent();
	//Refresh list and status
	CString strVWallStopParam = _T("");
	ELTE_CHAR *pStr;
	ELTE_INT32 iRet = ELTE_SDK_GetDcVWallIDList(&pStr);
	if (0 != iRet)
	{
		MessageBox(_T("GetDcVWallIDList error"));
	}
	strVWallStopParam = pStr;
	VideoChannelList *stringList = getVideoChannelListPointer();
	CXml::Instance().XmlParseVWallIDList(strVWallStopParam, (*stringList));

	CString StrIDState = (*stringList)[m_VideoChannel];
	if ("1" == StrIDState || "4023" == StrIDState)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Available"));
		GetDlgItem(IDC_BTN_STOPVWALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STARTVWALL)->EnableWindow(TRUE);
	}
	else if ("4022" == StrIDState)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Unavailable"));
		GetDlgItem(IDC_BTN_STOPVWALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STARTVWALL)->EnableWindow(FALSE);
	}
	else
	{
		MessageBox(_T("VWall channel state error!"));
		return;
	}	
	
}


void CDConsoleDlg::OnBnClickedBtnRefreshlist()
{
	CString strVWallStopParam = _T("");
	ELTE_CHAR *pStr = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcVWallIDList(&pStr);
	if (0 != iRet)
	{
		MessageBox(_T("GetDcVWallIDList error"));
	}
	strVWallStopParam = pStr;

	VideoChannelList stringList;
	CXml::Instance().XmlParseVWallIDList(strVWallStopParam, stringList);

	setVideoChannelList(stringList);

	VideoChannelList::iterator it;
	m_ComboVWIDList.ResetContent();
	for (it= stringList.begin(); it!=stringList.end();it++)
	{
		m_ComboVWIDList.AddString(it->first);
	}
}


void CDConsoleDlg::OnDblclkListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
	int item = m_DcUsers.GetNextSelectedItem(pos);
	m_ResourceID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
	
	*pResult = 0;

	VideoParamInfo info;
	CVideoParamDlg dlg(this);
	dlg.SetVideoParamInfo(&info);
	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// xml
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

	ELTE_INT32 iRet = ELTE_SDK_StartRealPlay(eLTE_Tool::UnicodeToANSI(m_ResourceID).c_str(), eLTE_Tool::UnicodeToANSI(strVideoParam).c_str());
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StartRealPlay"));
	SetDlgItemText(IDC_EDIT_RESOURCEID, m_ResourceID);
}

void CDConsoleDlg::OnBnClickedButtonCreatepg()
{
	// TODO: Add your control notification handler code here
	OperatePatchGroup(CeLTE_SDK_DemoDlg::Instance().m_strName, _T("0"), 0);
}

void CDConsoleDlg::OperatePatchGroup(const CString& strDcID, const CString& strGroupID, const int& iOperateType, const CString& strGroupName)
{
	// TODO: Add your control notification handler code here
	PCHGRPParamInfo info;
	CCreatePatchGroupDlg dlg(this);
	dlg.SetPCHGRPParamInfo(CeLTE_SDK_DemoDlg::Instance().m_strName, strGroupID, strGroupName, &info);

	int iExit = dlg.DoModal();
	if (IDOK != iExit)
	{
		return;
	}

	// XML
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

	// create patch group
	if (0 == iOperateType)
	{
		ELTE_INT32 iRet = ELTE_SDK_CreatePatchGroup(eLTE_Tool::UnicodeToANSI(strPCHGRPParam).c_str());
		CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_CreatePatchGroup"));
	}
	// add patch group member
	if (1 == iOperateType)
	{
		ELTE_INT32 iRet = ELTE_SDK_AddPatchGroupMember(eLTE_Tool::UnicodeToANSI(strPCHGRPParam).c_str());
		CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_AddPatchGroupMember"));
	}
	// del patch group member
	if (2 == iOperateType)
	{
		ELTE_INT32 iRet = ELTE_SDK_DeletePatchGroupMember(eLTE_Tool::UnicodeToANSI(strPCHGRPParam).c_str());
		CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_DeletePatchGroupMember"));
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
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTDIAL, _T("Dial"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTRELEASE, _T("Release"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTHANGUP, _T("Hangup"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTEMERGENCY, _T("Emergency"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_GROUPBREAKOFF, _T("Breakoff"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTMUTE, _T("Mute"));
		pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTUNMUTE, _T("UnMute"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTDISLISTEN, _T("DisListen_Start"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTOPDISLISTEN, _T("DisListen_Stop"));
		//pttMenu.AppendMenu(MF_STRING, ID_MENU_PTTSTARTENVLISTEN, _T("EnvListen_Start"));

		//group node
		CMenu menu;
		menu.CreatePopupMenu();
		//menu.AppendMenu(MF_STRING, ID_MENU_SUBGROUP, _T("SubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_GETPATCHGROUPINFO, _T("GetPatchGroupInfo"));
		menu.AppendMenu(MF_STRING, ID_MENU_ADDPATCHROUPMEMBER, _T("AddMember"));
		menu.AppendMenu(MF_STRING, ID_MENU_DELPATCHROUPMEMBER, _T("DelMember"));
		menu.AppendMenu(MF_STRING, ID_MENU_SUBJOINGROUP, _T("SubJoinGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_UNSUBGROUP, _T("UnSubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_RMPATCHROUP, _T("RemovePatchGroup"));
		
		menu.AppendMenu(MF_POPUP, (UINT)pttMenu.m_hMenu, _T("PTT"));
		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
	//else
	//{
	//	//user node
	//	CMenu menu;
	//	menu.CreatePopupMenu();
	//	menu.AppendMenu(MF_STRING, ID_MENU_GETGROUPINFO, _T("GetGroupUserInfo"));

	//	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	//}
}


void CDConsoleDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	return;
	//CDialogEx::OnOK();
}


void CDConsoleDlg::OnBnClickedButtonSnapshot()
{
	std::string m_strSavepath = GetSnapshotPath(m_strResId);
	ELTE_INT32 iRet = ELTE_SDK_LocalSnapshot(m_strResId.c_str(), m_strSavepath.c_str(), (ELTE_ULONG)1);
	MessageBox(_T("Screenshots have been stored under the snapshot directory"));
}

CString CDConsoleDlg::GetModuleDir()
{ 
	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileNameA(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind( _T('\\') );
	if( nPos < 0 )
		return CString("");
	else
		return csFullPath.Left( nPos ); 
}

std::string CDConsoleDlg::GetSnapshotPath(const std::string& strResId) const
{
	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(_T("%Y%m%d%H%M%S"));
	std::string strSnapshotName = strResId;
	strSnapshotName.append("_");
	strSnapshotName.append(eLTE_Tool::UnicodeToANSI(szTime.GetBuffer()));

	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileNameA(module, pFileName, MAX_PATH);

	std::string strSnapshotPath = pFileName;
	//Remove the EXE string
	size_t index = strSnapshotPath.find_last_of("\\");
	std::string strSavePath;
	if(std::string::npos != index)
	{
		strSavePath = strSnapshotPath.substr(0, index);
	}

	strSavePath.append("\\snapshot\\"+strSnapshotName);
	strSavePath.append(".jpg");	

	return strSavePath;
}

void CDConsoleDlg::OnBnClickedButtonGetgissub()
{
	// TODO: Add your control notification handler code here
	ELTE_CHAR* pChar = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGisSubscription(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strName).c_str(), &pChar);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGisSubscription"));
	if(NULL != pChar)
	{
		CString str(pChar);
		::MessageBox(this->GetSafeHwnd(), str, _T("GisSubscriptionInfo"), MB_OK);
	}
}
