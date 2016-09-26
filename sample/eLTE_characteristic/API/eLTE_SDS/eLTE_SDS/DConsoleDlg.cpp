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
#include "eLTE_SDS.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "eLTE_SDSDlg.h"
#include "SDSMessage.h"
#include "eLTE_Tool.h"

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
	, m_pCeLTE_SDSDlg(NULL)
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
//	ON_BN_CLICKED(IDC_BUTTON_DCUSERS, &CDConsoleDlg::OnBnClickedButtonDcusers)
//	ON_BN_CLICKED(IDC_BUTTON_DCGROUPS, &CDConsoleDlg::OnBnClickedButtonDcgroups)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DCGROUPS, &CDConsoleDlg::OnNMRClickTreeDcgroups)
//	ON_NOTIFY(NM_RCLICK, IDC_LIST_DCUSERS, &CDConsoleDlg::OnNMRClickListDcusers)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickTreeMenuItem)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDConsoleDlg::OnItemclickListDcusers)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PATCHGROUPS, &CDConsoleDlg::OnRclickTreePatchgroups)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJSEL, &CDConsoleDlg::OnSelchangeComboObjsel)
//	ON_EN_CHANGE(IDC_EDIT_RESSEARCH, &CDConsoleDlg::OnChangeEditRessearch)
	ON_BN_CLICKED(IDC_BUTTON_SDS, &CDConsoleDlg::OnBnClickedButtonSds)
	ON_BN_CLICKED(IDC_BUTTON2, &CDConsoleDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_EVENT11, &CDConsoleDlg::OnBnClickedButtonEvent11)
	ON_BN_CLICKED(IDC_BUTTON_EVENT2, &CDConsoleDlg::OnBnClickedButtonEvent2)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDConsoleDlg::OnBnClickedButtonSearch)
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

	//initial User Type comboBox
	m_cmbObjSel.InsertString(0, _T("DcUsers"));
	m_cmbObjSel.InsertString(1, _T("DcGroups"));
	m_cmbObjSel.InsertString(2, _T("PatchGroups"));
	m_cmbObjSel.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDConsoleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ClearDcGroupsTree();

	if (m_pCeLTE_SDSDlg && m_pCeLTE_SDSDlg->GetSafeHwnd())
	{
		m_pCeLTE_SDSDlg->PostMessage(WM_CLOSE);
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
	ELTE_CHAR* pDcInfo = NULL;
	if(NULL == m_pCeLTE_SDSDlg)
	{
		return;
	}
	ELTE_INT32 iRet = ELTE_SDK_GetDcInfo(eLTE_Tool::UnicodeToANSI(m_strUserID).c_str(), &pDcInfo);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_GetDcInfo"));

	DcInfo dcInfo;
	CXml::Instance().XmlParseDcInfo(eLTE_Tool::ANSIToUnicode(pDcInfo), dcInfo);

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
	ELTE_SDK_ReleaseBuffer(pDcInfo);
	pDcInfo = NULL;
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
		}
	
		/*menu.AppendMenu(MF_STRING, ID_MENU_JOIN_DCGROUP, _T("JoinGroup"));*/
		
		//临时组
		if (pInfo->GroupCategory == "-1")
		{	
			menu.EnableMenuItem(ID_MENU_DCGROUPINFO, MF_GRAYED);
		}

		//动态组
		if (pInfo->GroupCategory == "9")//怎么判断动态群组是谁创建的？
		{
			menu.AppendMenu(MF_STRING, ID_MENU_RM_DGNA, _T("Remove"));
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

//void CDConsoleDlg::OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	*pResult = 0;
//	int nItem = pNMListView->iItem;
//	if (-1 == nItem)
//	{
//		return;
//	}
//
//	//弹出菜单
//	CMenu menu;
//	menu.CreatePopupMenu();
//	menu.AppendMenu(MF_STRING, ID_MENU_DCUSERINFO, _T("Info"));
//	CString sUserType = m_DcUsers.GetItemText(nItem, DCUSERS_CATEGORY);
//
//	CPoint pmenu;
//	::GetCursorPos(&pmenu);
//	menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
//}


void CDConsoleDlg::OnClickTreeMenuItem(UINT uID)
{
	switch (uID)
	{
	case ID_MENU_DCUSERINFO:
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
			CString strUserName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pUserInfo), _T("UserName"));
			ELTE_SDK_ReleaseBuffer(pUserInfo);
			pUserInfo = NULL;

			CString strDcInfo(_T(""));
			strDcInfo.Append(_T("DcID :"));
			strDcInfo.Append(strUserID);
			strDcInfo.Append(_T(" Priviledge :"));
			strDcInfo.Append(strUserCategory);
			strDcInfo.Append(_T(" Role :"));
			strDcInfo.Append(strUserPriority);
			strDcInfo.Append(_T(" Alias :"));
			strDcInfo.Append(strUserName);

			GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(strDcInfo);
		}
		break;
	case ID_MENU_DCGROUPINFO:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupInfo* pInfo = (GroupInfo*)m_DcGroups.GetItemData(selectedItem);

			ELTE_CHAR* pGroupInfo = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetGroupInfo(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupInfo);
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetGroupInfo"));

			CString strGroupID = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupID"));
			CString strGroupCategory = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupCategory"));
			CString strGroupPriority = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupPriority"));
			CString strGroupName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupName"));
			CString strGroupCreator = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("GroupCreator"));

			ELTE_SDK_ReleaseBuffer(pGroupInfo);
			pGroupInfo = NULL;

			CString strDcInfo(_T(""));
			strDcInfo.Append(_T("GroupID"));
			strDcInfo.Append(strGroupID);
			if(_T("9") == strGroupCategory)
			{
				strDcInfo.Append(_T("GroupCreator"));
				strDcInfo.Append(strGroupCreator);
			}
			else
			{
				strDcInfo.Append(_T("GroupCategory"));
				strDcInfo.Append(strGroupCategory);
			}
			strDcInfo.Append(_T("GroupPriority"));
			strDcInfo.Append(strGroupPriority);
			strDcInfo.Append(_T("GroupName"));
			strDcInfo.Append(strGroupName);

			GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(strDcInfo);
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
			ELTE_INT32 iRet = ELTE_SDK_UnSubscribeGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("UnSubscribeGroup"));
		}
		break;
	case ID_MENU_DCPTTDIAL:
		{
			GroupInfo* pInfo = NULL;
			GetSelGroupInfo(&pInfo);
			ELTE_INT32 iRet = ELTE_SDK_PTTDial(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_PTTDial"));
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
			CString strGroupName = GET_XML_ELEM_VALUE_STR(eLTE_Tool::ANSIToUnicode(pGroupInfo), _T("PGName"));

			ELTE_SDK_ReleaseBuffer(pGroupInfo);
			pGroupInfo = NULL;

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
			ELTE_INT32 iRet = ELTE_SDK_SubJoinGroup(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("SubJoinGroup"));
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
	case ID_MENU_DCGROUPUSERINFO:
		{
			HTREEITEM selectedItem = m_DcGroups.GetSelectedItem();
			GroupUserInfo* pInfo = (GroupUserInfo*)m_DcGroups.GetItemData(selectedItem);

			CString strDcInfo(_T(""));
			strDcInfo.Append(_T("UserID"));
			strDcInfo.Append(pInfo->UserID);
			strDcInfo.Append(_T("GroupID"));
			strDcInfo.Append(pInfo->GroupID);
			strDcInfo.Append(_T("UserPriorityInGroup"));
			strDcInfo.Append(pInfo->UserPriorityInGroup);
			strDcInfo.Append(_T("MemberType"));
			strDcInfo.Append(pInfo->MemberType);

			GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(strDcInfo);
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


void CDConsoleDlg::InitDlg()
{
	// 先清理
	ClearProv();

// 	GetDlgItem(IDC_BUTTON_PROVINIT)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BUTTON_PROVEXIT)->EnableWindow(TRUE);

	if(m_pCeLTE_SDSDlg && !((CeLTE_SDSDlg*)m_pCeLTE_SDSDlg)->m_bClose)
	{
		// 获取用户列表
		GetUsers(_T(""),false);
		// 获取群组列表
		GetGroups(_T(""),false);
		// 获取派接组列表
		GetPatchGroups(_T(""),false);

		// 触发状态上报
		ELTE_INT32 iRet = ELTE_SDK_TriggerStatusReport(1);
		CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_TriggerStatusReport"));
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

void CDConsoleDlg::GetUsers(const CString strSearch, bool bFlag)
{
	ELTE_CHAR* pDcUsers = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcUsers(eLTE_Tool::UnicodeToANSI(m_strUserID).c_str(), &pDcUsers);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetDcUsers"));

	UserInfoList users;
	CXml::Instance().XmlParseDcUsers(eLTE_Tool::ANSIToUnicode(pDcUsers), users);
	ELTE_SDK_ReleaseBuffer(pDcUsers);
	pDcUsers = NULL;

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
	ELTE_CHAR* pDcGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcGroups(eLTE_Tool::UnicodeToANSI(m_strUserID).c_str(), &pDcGroups);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetDcGroups"));

	// 获取群组列表
	GroupInfoList groups;
	CXml::Instance().XmlParseDcGroups(eLTE_Tool::ANSIToUnicode(pDcGroups), groups);
	ELTE_SDK_ReleaseBuffer(pDcGroups);
	pDcGroups = NULL;

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

			// 组内用户列表
			ELTE_CHAR* pGroupUsers = NULL;
			iRet = ELTE_SDK_GetGroupUsers(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupUsers);
			if (0 != iRet)
			{
				continue;
			}

			// 获取用户列表
			GroupUserInfoList groupUsers;
			CXml::Instance().XmlParseGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);
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
	ELTE_CHAR* pDcGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetPatchGroups(eLTE_Tool::UnicodeToANSI(m_strUserID).c_str(), &pDcGroups);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetPatchGroups"));

	// 获取群组列表
	GroupInfoList groups;
	CXml::Instance().XmlParsePatchGroups(eLTE_Tool::ANSIToUnicode(pDcGroups), groups);

	GroupInfoList::iterator itor = groups.begin();
	for (; groups.end() != itor; itor++)
	{
		GroupInfo* &pInfo = *itor;
		if (false == bFlag || (-1) != pInfo->GroupID.Find(strSearch))
		{
			CString szItemroot(pInfo->GroupID);
			szItemroot.Append(_T(" "));
			szItemroot.Append(_T("[派接组]"));
			HTREEITEM itemroot = m_PatchGroups.InsertItem(szItemroot, -1, -1, NULL);
			m_PatchGroups.SetItemData(itemroot, (DWORD_PTR)pInfo);

			// 组内用户列表
			ELTE_CHAR* pGroupUsers = NULL;
			iRet = ELTE_SDK_GetGroupMemberByPatchId(eLTE_Tool::UnicodeToANSI(pInfo->GroupID).c_str(), &pGroupUsers);
			if (0 != iRet)
			{
				continue;
			}
			/*if (0 != iRet)
			{
			continue;
			}*/

			// 获取用户列表
			GroupUserInfoList groupUsers;
			CXml::Instance().XmlParsePatchGroupUsers(eLTE_Tool::ANSIToUnicode(pGroupUsers), groupUsers);

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

CString CDConsoleDlg::GetGroupCategoryString(const CString& strGroupCategory)
{
	int iGroupCategory = StrToInt(strGroupCategory);
	CString strResult;

	switch (iGroupCategory) // 群组种类
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


void CDConsoleDlg::OnBnClickedButtonEvent0()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent0);
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
	// TODO: 在此添加控件通知处理程序代码
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


//void CDConsoleDlg::OnChangeEditRessearch()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// 响应搜索框输入变更事件
//	
//	
//}

void CDConsoleDlg::OnBnClickedButtonSds()
{
	// TODO: 在此添加控件通知处理程序代码
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

	ELTE_INT32 iRet = ELTE_SDK_SDSSendMessage(eLTE_Tool::UnicodeToANSI(m_strUserID).c_str(),
		eLTE_Tool::UnicodeToANSI(strSDSParam).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_SDSSendMessage"));
}


void CDConsoleDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent10);
}


void CDConsoleDlg::OnBnClickedButtonEvent11()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent11);
}


void CDConsoleDlg::OnBnClickedButtonEvent2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_EVENTMSG)->SetWindowText(m_strEvent2);
}


void CDConsoleDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
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
			// 获取群组列表
			GetGroups(m_strSearch, bFlag);
		}
		break;
	case 2:
		{
			//clear PatchGroup List
			m_PatchGroups.DeleteAllItems();
			// 获取派接组列表
			GetPatchGroups(m_strSearch, bFlag);
		}
		break;
	default:
		;
	}
	// 触发状态上报
	ELTE_INT32 iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_TriggerStatusReport"));
}
