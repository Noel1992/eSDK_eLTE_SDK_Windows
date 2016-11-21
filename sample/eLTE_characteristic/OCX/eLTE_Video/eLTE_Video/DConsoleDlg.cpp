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
#include "eLTE_Video.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "eLTE_VideoDlg.h"
#include "VideoParamDlg.h"
#include "DispatchParamDlg.h"
#include "UnDispatchParamDlg.h"
#include "VWall.h"

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

#define ID_MENU_DCP2PTRANSFER    (ID_MENU_BEGIN+24)
#define ID_MENU_RMPATCHROUP      (ID_MENU_BEGIN+25)
#define	ID_MENU_ADDPATCHROUPMEMBER (ID_MENU_BEGIN+26)
#define	ID_MENU_DELPATCHROUPMEMBER (ID_MENU_BEGIN+27)
#define	ID_MENU_GETPATCHGROUPINFO (ID_MENU_BEGIN+28)
#define ID_MENU_PTZCONTROL        (ID_MENU_BEGIN+29)

#define ID_MENU_END				 (ID_MENU_BEGIN+30)


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
	, m_pCeLTE_VideoDlg(NULL)
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

	m_strSearch = _T("");
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
	DDX_Control(pDX, IDC_COMBO_OBJSEL, m_cmbObjSel);
	DDX_Text(pDX, IDC_EDIT_RESSEARCH, m_strSearch);
}


BEGIN_MESSAGE_MAP(CDConsoleDlg, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DCGROUPS, &CDConsoleDlg::OnNMRClickTreeDcgroups)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickTreeMenuItem)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_EVENT2, &CDConsoleDlg::OnBnClickedButtonEvent2)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDConsoleDlg::OnItemclickListDcusers)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PATCHGROUPS, &CDConsoleDlg::OnRclickTreePatchgroups)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJSEL, &CDConsoleDlg::OnSelchangeComboObjsel)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDConsoleDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, &CDConsoleDlg::OnBnClickedButtonStopplay)
	ON_BN_CLICKED(IDC_BUTTON_REALPLAY, &CDConsoleDlg::OnBnClickedButtonRealplay)
	ON_BN_CLICKED(IDC_BUTTON_DISPATCHVIDEO, &CDConsoleDlg::OnBnClickedButtonDispatchvideo)
	ON_BN_CLICKED(IDC_BUTTON_REVERSEPLAY, &CDConsoleDlg::OnBnClickedButtonReverseplay)
	ON_BN_CLICKED(IDC_BUTTON_UNDISPATCH, &CDConsoleDlg::OnBnClickedButtonUndispatch)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, &CDConsoleDlg::OnBnClickedButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CDConsoleDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_VWALL, &CDConsoleDlg::OnBnClickedButtonVwall)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CDConsoleDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_MUTE, &CDConsoleDlg::OnBnClickedButtonMute)
	ON_BN_CLICKED(IDC_BUTTON_UNMUTE, &CDConsoleDlg::OnBnClickedButtonUnmute)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CDConsoleDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_NORMALSCREEN, &CDConsoleDlg::OnBnClickedButtonNormalscreen)
	ON_CBN_SELCHANGE(IDC_COMBO_TOOLBAR, &CDConsoleDlg::OnCbnSelchangeComboToolbar)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG, &CDConsoleDlg::OnCbnSelchangeComboLang)
	ON_BN_CLICKED(IDC_BUTTON_TITLETEXT, &CDConsoleDlg::OnBnClickedButtonTitletext)
	ON_BN_CLICKED(IDC_BUTTON_WINPOS, &CDConsoleDlg::OnBnClickedButtonWinpos)
	ON_BN_CLICKED(IDC_BUTTON_EVENT1, &CDConsoleDlg::OnBnClickedButtonEvent1)
	ON_BN_CLICKED(IDC_BUTTON_EVENT7, &CDConsoleDlg::OnBnClickedButtonEvent7)
	ON_BN_CLICKED(IDC_BUTTON_EVENT4, &CDConsoleDlg::OnBnClickedButtonEvent4)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMDblclkListDcusers)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMRClickListDcusers)
	ON_MESSAGE(WM_VIDEORECEIVED, &CDConsoleDlg::OnVideoMsgReceived)
	ON_MESSAGE(WM_SHOW_COMMING_VIDEO, &CDConsoleDlg::ShowForRecv)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDConsoleDlg message handlers


BOOL CDConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_DcUsers.ModifyStyle(0, LVS_SINGLESEL);
	m_DcUsers.InsertColumn(DCUSERS_RESID, _T("ResourceID"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_STATUS, _T("UserStatus"), LVCFMT_LEFT, 100);
	m_DcUsers.InsertColumn(DCUSERS_NAME, _T("UserName"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_CATEGORY, _T("UserCategory"), LVCFMT_LEFT, 85);
	m_DcUsers.InsertColumn(DCUSERS_PRIORITY, _T("UserPriority"), LVCFMT_LEFT, 85);

	//picture list
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

	//set List style:select all、child list support picture
	m_DcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP);

	//set coordinate
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

	// initial data
	CComboBox* pToorBar = (CComboBox*)GetDlgItem(IDC_COMBO_TOOLBAR);
	pToorBar->InsertString(0, _T("hide"));
	pToorBar->InsertString(0, _T("show"));
	pToorBar->SetCurSel(0);
	CComboBox* pLang = (CComboBox*)GetDlgItem(IDC_COMBO_LANG);
	pLang->InsertString(0, _T("Spanish"));
	pLang->InsertString(0, _T("English"));
	pLang->InsertString(0, _T("Chinese"));
	pLang->SetCurSel(0);
	SetDlgItemText(IDC_EDIT_TITLE, _T("Live Video"));
	SetDlgItemInt(IDC_EDIT_LEFT, 0);
	SetDlgItemInt(IDC_EDIT_TOP, 0);
	SetDlgItemInt(IDC_EDIT_WIDTH, 450);
	SetDlgItemInt(IDC_EDIT_HEIGHT, 400);

	//initial User Type comboBox
	m_cmbObjSel.InsertString(0, _T("DcUsers"));
	m_cmbObjSel.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDConsoleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
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

	if (m_pCeLTE_VideoDlg && m_pCeLTE_VideoDlg->GetSafeHwnd())
	{
		m_pCeLTE_VideoDlg->PostMessage(WM_CLOSE);
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


void CDConsoleDlg::OnBnClickedButtonDcinfo()
{
	// TODO: Add your control notification handler code here
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcInfo(m_strUserID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcInfo"));

	DcInfo dcInfo;
	CXml::Instance().XmlParseDcInfo(strResult, dcInfo);

	CString strDcInfo(_T(""));
	strDcInfo.Append(_T("DcID :"));
	strDcInfo.Append(dcInfo.DcID);
	strDcInfo.Append(_T(" Priviledge :"));
	strDcInfo.Append(dcInfo.Priviledge);
	strDcInfo.Append(_T(" Role :"));
	strDcInfo.Append(dcInfo.Role);
	strDcInfo.Append(_T(" Alias :"));
	strDcInfo.Append(dcInfo.Alias);
	
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(strDcInfo);
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
		//user node
		CMenu menu;
		menu.CreatePopupMenu();
		if (pInfo->GroupCategory != "-1")
		{
			menu.AppendMenu(MF_STRING, ID_MENU_UNSUB_DCGROUP, _T("UnSub"));
			menu.AppendMenu(MF_STRING, ID_MENU_SUB_JOIN_DCGROUP, _T("SubJoin"));
		}

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
	else
	{
		//user node
		CMenu menu;
		menu.CreatePopupMenu();
		//menu.AppendMenu(MF_STRING, ID_MENU_DCGROUPUSERINFO, _T("Info"));

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
}


void CDConsoleDlg::OnClickTreeMenuItem(UINT uID)
{
	switch (uID)
	{
	case ID_MENU_SUB_DCGROUP:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

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

			CString strDcInfo(_T(""));
			strDcInfo.Append(_T("GroupID"));
			strDcInfo.Append(strGroupID);
			strDcInfo.Append(_T("GroupCreator"));
			strDcInfo.Append(strSetupDcId);
			strDcInfo.Append(_T("GroupPriority"));
			strDcInfo.Append(strGroupPriority);
			strDcInfo.Append(_T("GroupName"));
			strDcInfo.Append(strGroupName);

			GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(strDcInfo);
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
				MessageBox(_T("No subscription to this dynamic group."));
			}
			else if (iRet == -40005)
			{
				MessageBox(_T("user permission limit."));
			}
			else 
			{
				CString str;
				str.Format(_T("ELTE_OCX_CancelDynamicGroup Failed. (%d)"), iRet);
				MessageBox(str);
			}
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


void CDConsoleDlg::InitDlg()
{
	// clean first
	ClearProv();

	if(m_pCeLTE_VideoDlg && !((CeLTE_VideoDlg*)m_pCeLTE_VideoDlg)->m_bClose)
	{
		// get user list
		GetUsers(_T(""),false);
		// get group list
		GetGroups(_T(""),false);
		// get Patch Group list
		GetPatchGroups(_T(""),false);

		// Trigger status report
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
				//Heartbeat mechanism can not cover the original state as an online state。start
				CString sStatus = m_DcUsers.GetItemText(i, DCUSERS_STATUS);
				if (sStatus != _T("offline"))
				{
					return;
				}
				//Heartbeat mechanism can not cover the original state as an online state。end

				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("online"));
			}
			else if (iStatus == 4012)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("offline"));
			}
			else if (iStatus == 4020)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("start calling"));
			}
			else if (iStatus == 4021)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("ringing"));
			}
			else if (iStatus == 4022)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("using"));
			}
			else if (iStatus == 4023)
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("free"));
			}
		}
	}
}


void CDConsoleDlg::GetUsers(const CString strSearch, bool bFlag)
{
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcUsers(m_strUserID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcUsers"));

	UserInfoList users;
	CXml::Instance().XmlParseDcUsers(strResult, users);

	UserInfoList::iterator itor = users.begin();
	for (; users.end() != itor; itor++)
	{
		UserInfo* pUserInfo = *itor;
		if (false == bFlag || (-1) != pUserInfo->UserID.Find(strSearch))
		{
			int item = m_DcUsers.InsertItem(m_DcUsers.GetItemCount(), _T(""), GetUserImageId(pUserInfo->UserCategory, true));
			m_DcUsers.SetItemText(item, DCUSERS_RESID, pUserInfo->UserID);
			m_DcUsers.SetItemText(item, DCUSERS_STATUS, _T("offline"));
			m_DcUsers.SetItemText(item, DCUSERS_NAME, pUserInfo->UserName);
			m_DcUsers.SetItemText(item, DCUSERS_CATEGORY, pUserInfo->UserCategory);
			m_DcUsers.SetItemText(item, DCUSERS_PRIORITY, pUserInfo->UserPriority);
		}
	}
}

void CDConsoleDlg::GetGroups(const CString strSearch, bool bFlag)
{
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetDcGroups(m_strUserID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetDcGroups"));

	// get group list
	GroupInfoList groups;
	CXml::Instance().XmlParseDcGroups(strResult, groups);

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		if (false == bFlag || (-1) != pInfo->GroupID.Find(strSearch))
		{
			CString szItemroot(pInfo->GroupID);
			szItemroot.Append(_T(" "));
			szItemroot.Append(GetGroupCategoryString(pInfo->GroupCategory));
			HTREEITEM itemroot = m_DcGroups.InsertItem(szItemroot, -1, -1, NULL);
			m_DcGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

			// group member list
			strResult = m_peLTE_Player->ELTE_OCX_GetGroupUsers(pInfo->GroupID);
			//CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetGroupUsers"));

			// get user list
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
}


void CDConsoleDlg::GetPatchGroups(const CString strSearch, bool bFlag)
{
	int iRet = 0;
	CString strResult = m_peLTE_Player->ELTE_OCX_GetPatchGroups(m_strUserID);
	CHECK_RESULTE_CODE_M(strResult, iRet, _T("ELTE_OCX_GetPatchGroups"));

	// get group list
	GroupInfoList groups;
	CXml::Instance().XmlParsePatchGroups(strResult, groups);

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		if (false == bFlag || (-1) != pInfo->GroupID.Find(strSearch))
		{
			CString szItemroot(pInfo->GroupID);
			szItemroot.Append(_T(" "));
			szItemroot.Append(_T("[Patch Group]"));
			HTREEITEM itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
			m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

			// group member list
			strResult = m_peLTE_Player->ELTE_OCX_GetGroupMemberByPatchId(pInfo->GroupID);

			// get user list
			GroupUserInfoList groupUsers;
			CXml::Instance().XmlParsePatchGroupUsers(strResult, groupUsers);

			GroupUserInfoList::iterator it = groupUsers.begin();
			for (; groupUsers.end() != it; it++)
			{
				GroupUserInfo* &pUserInfo = *it;
				CString szItem(pUserInfo->MemberGroupId);
				szItem.Append(_T(" "));
				szItem.Append(_T("[Normal Group]"));
				HTREEITEM item = m_PatchGroups.InsertItem(szItem, -1, -1, itemroot);
				m_PatchGroups.SetItemData(item, (DWORD_PTR)pUserInfo);
			}

			m_PatchGroups.Expand(itemroot,TVE_EXPAND);
		}
	}
}


void CDConsoleDlg::RemoveGroup(int iGroupID)
{
	CString strGroupID;
	strGroupID.Format(_T("%d"), iGroupID);

	// Query the group in the group list
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

	switch (iUserCategory) // user type
	{
	case 0:
		{
			// dispatcher
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

	switch (iGroupCategory) // group type
	{
	case 0:
		{
			strResult.Append(_T("[Whole network broadcasting group]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[Normal Group]"));
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

	switch (iMeberType) // user type
	{
	case 0:
		{
			strResult.Append(_T("[Dynamic reorganization of the group members]"));
		}
		break;
	case 1:
		{
			strResult.Append(_T("[Normal Group User]"));
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


void CDConsoleDlg::OnBnClickedButtonEvent0()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent0);
}


void CDConsoleDlg::OnBnClickedButtonEvent2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent2);
}


void CDConsoleDlg::OnBnClickedButtonEvent6()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent6);
}


void CDConsoleDlg::OnItemclickListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}


void CDConsoleDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent10);
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

		//group node
		CMenu menu;
		menu.CreatePopupMenu();
		//menu.AppendMenu(MF_STRING, ID_MENU_SUBGROUP, _T("SubscribeGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_GETPATCHGROUPINFO, _T("GetPatchGroupInfo"));
		menu.AppendMenu(MF_STRING, ID_MENU_SUB_JOIN_DCGROUP, _T("SubJoinGroup"));
		menu.AppendMenu(MF_STRING, ID_MENU_UNSUB_DCGROUP, _T("UnSubscribeGroup"));

		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
}


void CDConsoleDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
	return;
}


void CDConsoleDlg::OnSelchangeComboObjsel()
{
	
	switch (m_cmbObjSel.GetCurSel())
	{
	case 0:
		{
			m_DcUsers.ShowWindow(SW_SHOW);
			m_DcGroups.ShowWindow(SW_HIDE);
			m_PatchGroups.ShowWindow(SW_HIDE);
		}
		break;
	case 1:
		{
			m_DcUsers.ShowWindow(SW_HIDE);
			m_DcGroups.ShowWindow(SW_SHOW);
			m_PatchGroups.ShowWindow(SW_HIDE);
		}
		break;
	case 2:
		{
			m_DcUsers.ShowWindow(SW_HIDE);
			m_DcGroups.ShowWindow(SW_HIDE);
			m_PatchGroups.ShowWindow(SW_SHOW);
		}
		break;
	default:
		;
	}
	
}

void CDConsoleDlg::OnBnClickedButtonSearch()
{
	
	bool bFlag = true;
	UpdateData(TRUE);
	if ("" == m_strSearch)
	{
		bFlag = false;
	}
	else
	{
		bFlag = true;
	}
	switch (m_cmbObjSel.GetCurSel())
	{
	case 0:
		{
			//clear DcUsers List
			m_DcUsers.DeleteAllItems();
			// 获取用户列表
			GetUsers(m_strSearch, bFlag);
		}
		break;
	case 1:
		{
			//clear DcGroups List
			m_DcGroups.DeleteAllItems();
			// get group list
			GetGroups(m_strSearch, bFlag);
		}
		break;
	case 2:
		{
			//clear PatchGroup List
			m_PatchGroups.DeleteAllItems();
			// get Patch Group list
			GetPatchGroups(m_strSearch, bFlag);
		}
		break;
	default:
		;
	}
	// trigger status report
	m_peLTE_Player->ELTE_OCX_TriggerStatusReport(1);
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


void CDConsoleDlg::ELTE_PlayerEvent(UINT CtlID, ULONG ulEventId, LPCTSTR pXml)
{
	CElte_playerctrl1* pOcx = (CElte_playerctrl1*)m_pCeLTE_VideoDlg->GetDlgItem(CtlID);
	if (NULL == pOcx)
	{
		CString strMsg;
		strMsg.Format(_T("GetDlgItem CtlID(%d)"), CtlID);
		MessageBox(strMsg);
		return;
	}

	if (eLTE_PLAYER_HIDE == ulEventId)
	{
		std::vector<CElte_playerctrl1*>::iterator itor = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
		if (itor != m_ActiveOcxList.end())
		{
			m_ActiveOcxList.erase(itor);
			m_FreeOcxList.push_back(pOcx);
		}

		// release resource
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
			// button status
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


void CDConsoleDlg::RealPlayNotConnected(INT iResID, INT iCallStatus)
{
	if (3008 == iCallStatus)
	{
		CString strResId;
		strResId.Format(L"%d", iResID);
		SetDlgItemText(IDC_EDIT_RESID, strResId);
		OnBnClickedButtonStopplay();
	}
	// release resource
	std::map<int, ST_REALPLAY_PARAM>::iterator it = m_ResourceMap.find(iResID);
	if (it == m_ResourceMap.end())
	{
		return;
	}

	ST_REALPLAY_PARAM& param = it->second;
	CElte_playerctrl1* pOcx = param.pOCX;
	param.Reset();
	m_ResourceMap.erase(it);

	// release resource
	std::vector<CElte_playerctrl1*>::iterator itor = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
	if (itor != m_ActiveOcxList.end())
	{
		m_ActiveOcxList.erase(itor);
		m_FreeOcxList.push_back(pOcx);
	}

	if (GetDlgItemInt(IDC_EDIT_RESID) == iResID)
	{
		// button status
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

void CDConsoleDlg::OnBnClickedButtonStopplay()
{
	
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

	// release resource
	CElte_playerctrl1* pOcx = param.pOCX;
	param.Reset();
	m_ResourceMap.erase(itor);

	std::vector<CElte_playerctrl1*>::iterator it = std::find(m_ActiveOcxList.begin(), m_ActiveOcxList.end(), pOcx);
	if (it != m_ActiveOcxList.end())
	{
		m_ActiveOcxList.erase(it);
		m_FreeOcxList.push_back(pOcx);
	}

	// button status
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

void CDConsoleDlg::RealPlayHangup(INT iResID)
{

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

	// construct reference
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
		pOcx->Create(_T(""),WS_CHILD,rect,m_pCeLTE_VideoDlg,OCX_ID_BEGIN+m_AllOcxList.size());
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

	// button status
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


void CDConsoleDlg::OnBnClickedButtonDispatchvideo()
{
	
	CHECK_RESOURCE();

	DispatchParamInfo info;
	INT_PTR nRes;
	DispatchParamDlg dlg(this);
	dlg.SetDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML format ---
	<Content>
		<ResourceID>resource ID</ResourceID>
		<Fmtvalue></Fmtvalue>//D1、CIF、QCIF、720P or 1080P
		<DispatchNum></DispatchNum>//Video source user number, fill in the video source terminal ID.
		<Dstviewerlist>//Video distribution destination terminal ID list
			<Dstviewer></Dstviewer>//the target of video distribute, terminal ID or distributer ID
		</Dstviewerlist>
		<Channel></Channel>//Reserved；
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
	std::list<CString>::iterator it;
	for (it = info.strDstViewerList.begin(); it != info.strDstViewerList.end(); ++it)
	{
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


void CDConsoleDlg::OnBnClickedButtonReverseplay()
{
	
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

	// construct reference
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

	if(CeLTE_VideoDlg::m_iBypass)
	{
		param.uiState &= ~STATE_ISSHOW;
		GetDlgItem(IDC_BUTTON_HIDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BUTTON_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(FALSE);
}


void CDConsoleDlg::OnBnClickedButtonUndispatch()
{
	
	CHECK_RESOURCE();

	UnDispatchParamInfo info;
	INT_PTR nRes;
	UnDispatchParamDlg dlg(this);
	dlg.SetUnDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;

	/************************************************************************
	--- XML format ---
	<Content>
		<ResourceID>Launch video backhaul dispatcher ID</ResourceID>
		<UserId>the number of users in the distribution of the ID which need to hang up</UserId>
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


void CDConsoleDlg::OnBnClickedButtonSnapshot()
{
	
	CHECK_RESOURCE_EX();

	CString strResult = param.pOCX->ELTE_OCX_Snapshot(strResID);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_Snapshot"));
}


void CDConsoleDlg::OnBnClickedButtonShow()
{
	
	CHECK_RESOURCE_EX();

	//reference construct
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


void CDConsoleDlg::OnBnClickedButtonVwall()
{
	
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
}


void CDConsoleDlg::OnBnClickedButtonHide()
{
	
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
	
	CHECK_RESOURCE_EX();

	CString strResult; 
	if(CeLTE_VideoDlg::m_iBypass)
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


void CDConsoleDlg::OnBnClickedButtonUnmute()
{
	
	CHECK_RESOURCE_EX();
	CString strResult;
	if(CeLTE_VideoDlg::m_iBypass)
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


void CDConsoleDlg::OnBnClickedButtonFullscreen()
{
	
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


void CDConsoleDlg::OnBnClickedButtonTitletext()
{
	
	CHECK_RESOURCE_EX();

	CString strText;
	GetDlgItemText(IDC_EDIT_TITLE, strText);
	CString strResult = param.pOCX->ELTE_OCX_SetTitleText(strText);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetTitleText"));
}


void CDConsoleDlg::OnBnClickedButtonWinpos()
{
	
	CHECK_RESOURCE_EX();

	int iLeft = GetDlgItemInt(IDC_EDIT_LEFT);
	int iTop = GetDlgItemInt(IDC_EDIT_TOP);
	int iWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
	int iHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);

	CString strResult = param.pOCX->ELTE_OCX_SetVideoWindowPos(iLeft, iTop, iWidth, iHeight);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetVideoWindowPos"));
}


void CDConsoleDlg::OnBnClickedButtonEvent1()
{
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent1);
}


void CDConsoleDlg::OnBnClickedButtonEvent7()
{
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent7);
}


void CDConsoleDlg::OnBnClickedButtonEvent4()
{
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent4);
}


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

	// whether is a camera
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
	SetDlgItemText(IDC_EDIT_TITLE, _T("实时视频 ")+strResID);

	// Set resource status
	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));
	if (m_ResourceMap.end() == itor)
	{
		// button status
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
		// button status
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

	//pop-up menu
	CMenu menu;
	menu.CreatePopupMenu();
	CString sUserType = m_DcUsers.GetItemText(nItem, DCUSERS_CATEGORY);
	
	CPoint pmenu;
	::GetCursorPos(&pmenu);
	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
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
		pOcx->Create(_T(""),WS_CHILD,rect,m_pCeLTE_VideoDlg,OCX_ID_BEGIN+m_AllOcxList.size());
		m_AllOcxList.push_back(pOcx);
		m_ActiveOcxList.push_back(pOcx);

		strResult = pOcx->ELTE_OCX_Load(eLTE_LOAD_MEDIA_PLAYER);
	}

	//reference construct
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

	std::map<int, ST_REALPLAY_PARAM>::iterator itor = m_ResourceMap.find(StrToInt(strResID));

	if (m_ResourceMap.end() == itor)
	{
		ST_REALPLAY_PARAM param;
		param.uiState |= STATE_ISPLAY;
		param.pOCX = pOcx;
		param.uiVideoType = 1;//<Video distribution> or <video backhaul>

		param.uiState |= STATE_ISSHOW;
		m_ResourceMap.insert(std::make_pair(StrToInt(strResID), param));
	}
	else
	{
		ST_REALPLAY_PARAM& param = itor->second;
		param.uiState = STATE_ISPLAY;
		param.uiVideoType = 1;//<Video distribution> or <video backhaul>

		param.uiState |= STATE_ISSHOW;
		param.pOCX = pOcx;
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


BOOL CDConsoleDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam && GetFocus() == GetDlgItem(IDC_EDIT_RESSEARCH))   
	{   
		OnBnClickedButtonSearch();
	} 
	return CDialog::PreTranslateMessage(pMsg);
}
