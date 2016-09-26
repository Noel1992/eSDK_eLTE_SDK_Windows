// DConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_GIS_DemoDlg.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "DataType.h"
#include "eLTE_Tool.h"
#include "Xml.h"
#include "resource.h"
#include "eLTE_Tool.h"

// CDConsoleDlg dialog

IMPLEMENT_DYNAMIC(CDConsoleDlg, CDialogEx)

CDConsoleDlg::CDConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDConsoleDlg::IDD, pParent)
{

}

CDConsoleDlg::~CDConsoleDlg()
{
}

void CDConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DC_USERS, m_DcUsers);
}


BEGIN_MESSAGE_MAP(CDConsoleDlg, CDialogEx)
	ON_MESSAGE(WM_INIT, &CDConsoleDlg::InitDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DC_USERS, &CDConsoleDlg::OnNMRClickListDcUsers)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickMenuItem)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CDConsoleDlg::OnEnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDConsoleDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_GISSTATUS, &CDConsoleDlg::OnBnClickedButtonGisstatus)
	ON_BN_CLICKED(IDC_BUTTON_GISREPORT, &CDConsoleDlg::OnBnClickedButtonGisreport)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

afx_msg LRESULT CDConsoleDlg::InitDlg(WPARAM wParam, LPARAM lParam)
{
	if (0 < m_DcUsers.GetItemCount())
	{
		m_DcUsers.DeleteAllItems();
	}

	GetDcUsers();

	//Trigger status report
	int iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_TriggerStatusReport"));

	return 0;
}

void CDConsoleDlg::OnClickMenuItem(UINT uID)
{
	switch (uID)
	{
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
	}
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

	if (iStatus == 4012)//reg fail
	{
		iImgID = GetUserImageId(strUserCategory, true);
	}
	else
	{
		iImgID = GetUserImageId(strUserCategory, false);
	}

	return iImgID;
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
			//UserInfoList::iterator iter = m_Users.find(StrToInt(strResId));			
			if (iStatus == 4011)// reg ok
			{
				CString sStatus = m_DcUsers.GetItemText(i, DCUSERS_STATUS);
				if (sStatus != _T("Offline"))
				{
					return;
				}

				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Online"));
			}
			else if (iStatus == 4012)// offline
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Offline"));
			}
			else if (iStatus == 4020)//
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Calling"));
			}
			else if (iStatus == 4021)//ringing
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Ring"));
			}
			else if (iStatus == 4022)//using
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Using"));
			}
			else if (iStatus == 4023)//online
			{
				m_DcUsers.SetItemText(i, DCUSERS_STATUS, _T("Online"));
			}
		}
	}
}

void CDConsoleDlg::SetELTE_SDK_DemoDlg(CeLTE_SDK_GIS_DemoDlg* pDlg)
{ 
	m_pELTE_SDK_DemoDlg = pDlg; 
}

void CDConsoleDlg::GetDcUsers()
{
	ELTE_CHAR* pDcUsers = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcUsers(eLTE_Tool::UnicodeToANSI(m_pELTE_SDK_DemoDlg->GetCurrentUserName()).c_str(), &pDcUsers);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetDcUsers"));

	CXml::Instance().XmlParseDcUsers(pDcUsers, m_Users);
	ELTE_SDK_ReleaseBuffer(pDcUsers);
	pDcUsers = NULL;

	UserInfoList::iterator itor = m_Users.begin();
	for (; m_Users.end() != itor; itor++)
	{
		UserInfo* pUserInfo = itor->second;
		if(_T("9") == pUserInfo->UserCategory)
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


// CDConsoleDlg message handlers


BOOL CDConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	//change list style
	m_DcUsers.ModifyStyle(0, LVS_SINGLESEL);

	//add title
	m_DcUsers.InsertColumn(DCUSERS_RESID, _T("ResourceID"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_STATUS, _T("UserStatus"), LVCFMT_LEFT, 100);
	m_DcUsers.InsertColumn(DCUSERS_NAME, _T("UserName"), LVCFMT_LEFT, 70);
	m_DcUsers.InsertColumn(DCUSERS_CATEGORY, _T("UserCategory"), LVCFMT_LEFT, 85);
	m_DcUsers.InsertColumn(DCUSERS_PRIORITY, _T("UserPriority"), LVCFMT_LEFT, 85);

	//set image
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
	m_ImageList.Detach();

	//change list style
	m_DcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP);

	CRect rect;
	GetDlgItem(IDC_STATIC_DC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRect rt(rect);
	rt.left = 10;
	rt.top = 50;
	rt.right -= 10;

	//set list pos
	m_DcUsers.MoveWindow(rt);
	m_DcUsers.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDConsoleDlg::AddImage(CImageList& imageList, UINT id)
{
	CBitmap bm;
	bm.LoadBitmap(id);
	imageList.Add(&bm, RGB(0, 0, 0));
}


void CDConsoleDlg::OnNMRClickListDcUsers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if (-1 == nItem)
	{
		return;
	}

	//get current pos
	POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
	int item = m_DcUsers.GetNextSelectedItem(pos);
	CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

	//get res type
	CString strCategory = m_DcUsers.GetItemText(item, DCUSERS_CATEGORY);

	//get res status
	CString sStatus = m_DcUsers.GetItemText(nItem, DCUSERS_STATUS);

	//9 is ptt res
	if (_T("9") == strCategory && _T("Online") == sStatus)
	{
		//create menu
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MENU_GISSUBSCRIBE, _T("Subscribe"));
		menu.AppendMenu(MF_STRING, ID_MENU_GISUNSUBSCRIBE, _T("UnSubscribe"));
		CPoint pmenu;
		::GetCursorPos(&pmenu);
		menu.TrackPopupMenu(0, pmenu.x, pmenu.y, this);
	}
}


void CDConsoleDlg::OnEnChangeEditSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	CString strKey;
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strKey);
	if(_T("") == strKey)
	{
		if (0 < m_DcUsers.GetItemCount())
		{
			m_DcUsers.DeleteAllItems();
		}
		// show all ptt user
		UserInfoList::iterator itor = m_Users.begin();
		for (; m_Users.end() != itor; itor++)
		{
			UserInfo* pUserInfo = itor->second;
			if(_T("9") == pUserInfo->UserCategory)
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
	//Refresh user status
	int iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_TriggerStatusReport"));
}

//key search
void CDConsoleDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strKey;
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strKey);
	if(_T("") == strKey)
	{
		return;
	}
	UserInfoList::iterator itor = m_Users.begin();
	if (0 < m_DcUsers.GetItemCount())
	{
		m_DcUsers.DeleteAllItems();
	}
	for (; m_Users.end() != itor; itor++)
	{
		UserInfo* pUserInfo = itor->second;
		if( -1 != pUserInfo->UserName.Find(strKey) && (_T("9")) == pUserInfo->UserCategory)
		{
			int item = m_DcUsers.InsertItem(m_DcUsers.GetItemCount(), _T(""), GetUserImageId(pUserInfo->UserCategory, true));
			m_DcUsers.SetItemText(item, DCUSERS_RESID, pUserInfo->UserID);
			m_DcUsers.SetItemText(item, DCUSERS_STATUS, _T("Offline"));
			m_DcUsers.SetItemText(item, DCUSERS_NAME, pUserInfo->UserName);
			m_DcUsers.SetItemText(item, DCUSERS_CATEGORY, pUserInfo->UserCategory);
			m_DcUsers.SetItemText(item, DCUSERS_PRIORITY, pUserInfo->UserPriority);
		}
	}
	//Refresh user status
	int iRet = ELTE_SDK_TriggerStatusReport(1);
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_TriggerStatusReport"));
}

void CDConsoleDlg::SetGISStatus(CString& strGISStatus)
{
	m_strGISStatus.Append(strGISStatus);
}

void CDConsoleDlg::SetGISReport(CString& strGISReport)
{
	m_strGISReport.Append(strGISReport);
}


void CDConsoleDlg::OnBnClickedButtonGisstatus()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_strGISStatus);
}


void CDConsoleDlg::OnBnClickedButtonGisreport()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_EVENT, m_strGISReport);
}


void CDConsoleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	for (UserInfoList::iterator iter = m_Users.begin(); iter != m_Users.end(); ++iter)
	{
		if (iter->second)
		{
			delete (iter->second);
			iter->second = NULL;
		}
	}

	// logout
	ELTE_INT32 iRet =  ELTE_SDK_Logout(eLTE_Tool::UnicodeToANSI(m_pELTE_SDK_DemoDlg->GetCurrentUserName()).c_str());
	if(0 != iRet)
	{
		MessageBox(_T("ELTE_SDK_Logout failed."));
	}

	CDialogEx::OnClose();
}
