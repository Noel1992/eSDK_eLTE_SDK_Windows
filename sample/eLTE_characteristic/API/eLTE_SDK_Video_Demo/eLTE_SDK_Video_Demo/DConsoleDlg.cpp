// DConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Video_DemoDlg.h"
#include "DConsoleDlg.h"
#include "afxdialogex.h"
#include "DataType.h"
#include "eLTE_Tool.h"
#include "Xml.h"
#include "resource.h"
#include "eLTE_Tool.h"
#include "VideoParamDlg.h"
#include "MediaPlayerDlgMgr.h"
#include "VideoCommingDlg.h"
#include "DispatchParamDlg.h"
#include "UnDispatchParamDlg.h"

// CDConsoleDlg dialog

IMPLEMENT_DYNAMIC(CDConsoleDlg, CDialogEx)

CDConsoleDlg::CDConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDConsoleDlg::IDD, pParent)
	, m_ResStatus(_T(""))
	, m_ModuleStatus(_T(""))
	, m_GroupStatus(_T(""))
	, m_VideoStatus(_T(""))
{

}

CDConsoleDlg::~CDConsoleDlg()
{
}

void CDConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DC_USERS, m_DcUsers);
	DDX_Control(pDX, IDC_COMBO_RESOURCE, m_ComboVWIDList);
}


BEGIN_MESSAGE_MAP(CDConsoleDlg, CDialogEx)
	ON_MESSAGE(WM_INIT, &CDConsoleDlg::InitDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DC_USERS, &CDConsoleDlg::OnNMRClickListDcUsers)
	ON_COMMAND_RANGE(ID_MENU_BEGIN, ID_MENU_END, &CDConsoleDlg::OnClickMenuItem)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CDConsoleDlg::OnEnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDConsoleDlg::OnBnClickedButtonSearch)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_START_REAL_PLAY, &CDConsoleDlg::StartRealPlay)
	ON_MESSAGE(WM_INCOMMING_VIDEO, &CDConsoleDlg::OnIncommingVideo)
	ON_BN_CLICKED(IDC_BTN_REFRESHLIST, &CDConsoleDlg::OnBnClickedBtnRefreshlist)
	ON_BN_CLICKED(IDC_BTN_STARTVWALL, &CDConsoleDlg::OnBnClickedBtnStartvwall)
	ON_BN_CLICKED(IDC_BTN_STOPVWALL, &CDConsoleDlg::OnBnClickedBtnStopvwall)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOURCE, &CDConsoleDlg::OnCbnSelchangeComboResource)
	ON_BN_CLICKED(IDC_BUTTON_DISPATCHVIDEO, &CDConsoleDlg::OnBnClickedButtonDispatchvideo)
	ON_BN_CLICKED(IDC_BUTTON_UNDISPATCH, &CDConsoleDlg::OnBnClickedButtonUndispatch)
	ON_COMMAND(IDOK, &CDConsoleDlg::OnIdok)
	ON_BN_CLICKED(IDC_BUTTON_RESSTATUS, &CDConsoleDlg::OnBnClickedButtonResstatus)
	ON_BN_CLICKED(IDC_BUTTON_MODULESTATUS, &CDConsoleDlg::OnBnClickedButtonModulestatus)
	ON_BN_CLICKED(IDC_BUTTON_GROUPSTATUS, &CDConsoleDlg::OnBnClickedButtonGroupstatus)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOSTATUS, &CDConsoleDlg::OnBnClickedButtonVideostatus)
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

afx_msg LRESULT CDConsoleDlg::OnIncommingVideo(WPARAM wParam, LPARAM lParam)
{
	VideoCommingDlg* pVideoCommingDlg = new VideoCommingDlg;
	if (NULL == pVideoCommingDlg)
	{
		return NULL;
	}
	pVideoCommingDlg->SetDConsoleDlg(this);
	pVideoCommingDlg->SetResId(m_strCommingVideo);

	pVideoCommingDlg->Create(IDD_DIALOG_VIDEO, NULL);

	CString strTitle("Incoming Video: ");
	strTitle.Append(m_strCommingVideo);
	pVideoCommingDlg->SetWindowText(strTitle);
	pVideoCommingDlg->ShowWindow(SW_SHOW);


	return 0;
}

void CDConsoleDlg::OnClickMenuItem(UINT uID)
{
	switch (uID)
	{
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

			// init param
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

			//CMediaPlayerDlgMgr::Instance().DeleteCreateMediaPlayerDlg(eLTE_Tool::UnicodeToANSI(strResID).c_str());
		}
		break;
	case ID_MENU_SNAPSHOT:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);
			//default jpg
			std::string strSavepath = GetSnapshotPath(m_strResId);
			ELTE_INT32 iRet = ELTE_SDK_LocalSnapshot(eLTE_Tool::UnicodeToANSI(strResID).c_str(), strSavepath.c_str(), (ELTE_ULONG)1);
			if(0 == iRet)
			{
				MessageBox(_T("Screenshots have been stored under the snapshot directory"));
			}
		}
		break;
	case ID_MENU_MUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strMuteParam("<Content><MuteParam><CallType>2</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolMute(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolMute"));
		}
		break;
	case ID_MENU_UNMUTE:
		{
			POSITION pos = m_DcUsers.GetFirstSelectedItemPosition();
			int item = m_DcUsers.GetNextSelectedItem(pos);
			CString strResID = m_DcUsers.GetItemText(item, DCUSERS_RESID);

			CString strMuteParam("<Content><MuteParam><CallType>2</CallType></MuteParam></Content>");
			ELTE_INT32 iRet = ELTE_SDK_VolUnMute(eLTE_Tool::UnicodeToANSI(strResID).c_str(), eLTE_Tool::UnicodeToANSI(strMuteParam).c_str());
			CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_VolMute"));
		}
		break;
	}
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

void CDConsoleDlg::SetELTE_SDK_DemoDlg(CeLTE_SDK_Video_DemoDlg* pDlg)
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
		if(_T("9") == pUserInfo->UserCategory || _T("1") == pUserInfo->UserCategory)
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

void CDConsoleDlg::SetPlayResID(std::string strResId)
{
	m_strResId = strResId;
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
			// PSTN
			iImgID = bGray ? Img_gwuser_offline : Img_gwuser_online;
		}
		break;
	case 9:
		{
			// PTT
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

	//9 is ptt res, 1 is camera
	if ((_T("1") == strCategory || _T("9") == strCategory) && _T("Offline") != sStatus)
	{
		//create menu
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_MENU_STARTREALPLAY, _T("StartRealPlay"));
		menu.AppendMenu(MF_STRING, ID_MENU_STOPREALPLAY, _T("StopRealPlay"));
		menu.AppendMenu(MF_STRING, ID_MENU_SNAPSHOT, _T("SnapShot"));
		menu.AppendMenu(MF_STRING, ID_MENU_MUTE, _T("Mute"));
		menu.AppendMenu(MF_STRING, ID_MENU_UNMUTE, _T("UnMute"));
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
			if(_T("9") == pUserInfo->UserCategory || _T("1") == pUserInfo->UserCategory)
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
		if( -1 != pUserInfo->UserName.Find(strKey) && (_T("9") == pUserInfo->UserCategory || _T("1") == pUserInfo->UserCategory))
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


void CDConsoleDlg::OnBnClickedBtnRefreshlist()
{
	CString strVWallStopParam = _T("");
	ELTE_CHAR *pStr = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcVWallIDList(&pStr);
	if (0 != iRet)
	{
		MessageBox(_T("GetDcVWallIDList error."));
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
	// TODO: Add your control notification handler code here
}


void CDConsoleDlg::OnBnClickedBtnStartvwall()
{
	// TODO: Add your control notification handler code here
	VideoChannelList *pVCL = getVideoChannelListPointer();

	if ("1" != (*pVCL)[m_VideoChannel] && "4023" != (*pVCL)[m_VideoChannel])
	{
		MessageBox(_T("current status unavailable."));
		return;
	}

	GetDlgItem(IDC_EDIT_RESOURCEID)->GetWindowText(m_ResourceID);
	// TODO: Add your control notification handler code here
	CString strVideoChannelStart;
	//init param
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
	OnCbnSelchangeComboResource();
}


void CDConsoleDlg::OnBnClickedBtnStopvwall()
{
	// TODO: Add your control notification handler code here
	/************************************************************************
	--- XML format ---
	<Content>
	<DstObjId>dst vall id </DstObjId>
	</Content>
	************************************************************************/
	VideoChannelList* pVCL = getVideoChannelListPointer();

	if ("4022" != (*pVCL)[m_VideoChannel])
	{
		MessageBox(_T("Please wait, the current state can not be closed."));
		return;
	}

	GetDlgItem(IDC_EDIT_RESOURCEID)->GetWindowText(m_ResourceID);
	CString strVideoChannelStop;
	//init param
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
	OnCbnSelchangeComboResource();
}


void CDConsoleDlg::OnCbnSelchangeComboResource()
{
	// TODO: Add your control notification handler code here
	int iSel;
	iSel = m_ComboVWIDList.GetCurSel();
	m_ComboVWIDList.GetLBText(iSel,m_VideoChannel);
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


void CDConsoleDlg::OnBnClickedButtonDispatchvideo()
{
	// TODO: Add your control notification handler code here
	DispatchParamInfo info;
	INT_PTR nRes;
	DispatchParamDlg dlg(this);
	dlg.SetDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	std::map<CString, int>::iterator&  iter = m_pELTE_SDK_DemoDlg->GetVideoInfo().find(info.strResourceId);
	if(m_pELTE_SDK_DemoDlg->GetVideoInfo().end() != iter && 5 != iter->second)//5 is invalid
	{
		if (0 == iter->second)
		{
			info.strFormat = _T("NO");
		}
		else if (1 == iter->second)
		{
			info.strFormat = _T("CIF");
		}
	}
	else
	{
		//default
		info.strFormat = _T("NO");
	}

	//UpdateData(TRUE);

	/************************************************************************
	--- XML style ---
	<Content>
		<ResourceID>Res ID</ResourceID>
		<Fmtvalue></Fmtvalue>//D1、CIF、QCIF、720P或1080P
		<DispatchNum></DispatchNum>//source res id
		<Dstviewerlist>//dst id list
			<Dstviewer></Dstviewer>//dst 
		</Dstviewerlist>
		<Channel></Channel>//Reserved
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
	dlg.SetUnDispatchParamInfo(&info);
	nRes = dlg.DoModal();
	if (IDOK!=nRes)
		return;
	//UpdateData(TRUE);

	/************************************************************************
	--- XML style ---
	<Content>
		<ResourceID>current dispatch id</ResourceID>
		<UserId>dispatching id</UserId>
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


void CDConsoleDlg::OnIdok()
{
	// TODO: Add your command handler code here
	return;
}


BOOL CDConsoleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam && GetFocus() == GetDlgItem(IDC_EDIT_SEARCH)) 
	{ 
		OnBnClickedButtonSearch();
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDConsoleDlg::OnBnClickedButtonResstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_EVENT, CeLTE_SDK_Video_DemoDlg::Instance().m_DConsoleDlg.m_ResStatus);
}


void CDConsoleDlg::OnBnClickedButtonModulestatus()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_EVENT, CeLTE_SDK_Video_DemoDlg::Instance().m_DConsoleDlg.m_ModuleStatus);
}


void CDConsoleDlg::OnBnClickedButtonGroupstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_EVENT, CeLTE_SDK_Video_DemoDlg::Instance().m_DConsoleDlg.m_GroupStatus);
}


void CDConsoleDlg::OnBnClickedButtonVideostatus()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_EVENT, CeLTE_SDK_Video_DemoDlg::Instance().m_DConsoleDlg.m_VideoStatus);
}
