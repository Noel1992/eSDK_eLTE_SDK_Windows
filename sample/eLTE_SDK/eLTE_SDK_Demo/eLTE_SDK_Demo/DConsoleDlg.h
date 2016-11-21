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

/********************************************************************
filename    :    eLTE_SDK_DemoDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    Business main dialog, after the success of the login display. User business operation and management
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 Initial version
*********************************************************************/

#pragma once
#include "afxcmn.h"
#include "RECFileDlg.h"
#include "CallDlg.h"
#include "VideoCommingDlg.h"
#include "afxwin.h"


// CDConsoleDlg dialog

class CDConsoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDConsoleDlg)

public:
	CDConsoleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDConsoleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DCONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnIncommingCall(WPARAM wParam, LPARAM lParam);
	// Create a window and play
	afx_msg LRESULT StartRealPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT InitDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIncommingVideo(WPARAM wParam, LPARAM lParam);

public:
	// Close window
	afx_msg void OnClose();
	// Left mouse button, show / hide callback events
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// Display user list
	afx_msg void OnBnClickedButtonDcusers();
	// Display group list
	afx_msg void OnBnClickedButtonDcgroups();
	// Trigger status report
	afx_msg void OnBnClickedButtonTrigger();
	// Configuration management initialization
	afx_msg void OnBnClickedButtonProvinit();
	// Configuration management exit
	afx_msg void OnBnClickedButtonProvexit();
	// Right click the user list, the user or device for business operations. View user information / call video / stop video / tilt control etc.
	afx_msg void OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult);
	// Right click the group list, the group or group of users to conduct business operations. View group or group user information / subscribe / subscribe to group / delete dynamic group, etc.
	afx_msg void OnNMRClickTreeDcgroups(NMHDR *pNMHDR, LRESULT *pResult);
	// Business operation on menu selection
	afx_msg void OnClickMenuItem(UINT uID);
	// Create dynamic group
	afx_msg void OnBnClickedButtonCreatedg();
	// Create temporary groups
	afx_msg void OnBnClickedButtonCreatetg();
	// Video file query
	afx_msg void OnBnClickedButtonGetrecflie();
	// Dispatcher information
	afx_msg void OnBnClickedButtonDcinfo();

public:
	// Callback message display ELTE_Event_NotifyProvisionAllResync
	afx_msg void OnBnClickedButtonAllresync();
	// Callback message display ELTE_Event_NotifyResourceStatus
	afx_msg void OnBnClickedButtonResstatus();
	// Callback message display ELTE_Event_NotifyUserStatus
	afx_msg void OnBnClickedButtonUserstatus();
	// Callback message display ELTE_Event_NotifyProvisionChange
	afx_msg void OnBnClickedButtonProvchange();
	// Callback message display ELTE_Event_NotifyModuleStatus
	afx_msg void OnBnClickedButtonModulestatus();
	// Callback message display ELTE_Event_NotifyP2pvideocallStatus
	afx_msg void OnBnClickedButtonVideostatus();
	// Callback message display ELTE_Event_NotifyP2pcallStatus
	afx_msg void OnBnClickedButtonCallstatus();
	// Callback message display ELTE_Event_NotifyGroupStatus
	afx_msg void OnBnClickedButtonGroupstatus();
	// Callback message display ELTE_Event_NotifyGISStatus
	afx_msg void OnBnClickedButtonGisstatus();
	// Callback message display ELTE_Event_NotifyGISReport
	afx_msg void OnBnClickedButtonGisreport();

public:
	// Set CELTE_SDK_DemoDlg window pointer
	void SetELTE_SDK_DemoDlg(CDialogEx* pDlg) { m_pELTE_SDK_DemoDlg = pDlg; }
	// After login successful initialization interface
	void InitDlg();
	// After creating a dynamic group / temporary group, and add in the group list interface
	void AddGroup(const char* groupId);
	// After creating a patch group, and add in the group list interface
	void AddPatchGroup(const char* groupId);
	// Delete patch group
	void RemovePatchGroup(int iGroupID);
	// Add group membership
	void AddPatchGroupMember(const char* groupId, const char* memberId);
	// 
	void DelPatchGroupMember(const char* groupId, const char* memberId);
	// Refresh the user status of the user list
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	// Refresh the incoming caller
	void SetCaller(const CString& strCaller, const bool& bUse) {m_Caller = strCaller; m_CallDlg.SetUse(bUse);}
	// Set video distribution parameters
	void SetVideoParam(const CString& videoFormat, const CString& tarResID) {strVideoFormat = videoFormat; strTarResID = tarResID;}
	// Get video resources ID
	CString GetTarResID() {return strTarResID;}
	// Get video format
	CString GetVideoFormat() {return strVideoFormat;}

	// Set the resource id of the current need to play 
	void SetPlayResID(std::string strResId);
	// Set the video request coming back
	void SetCommingVideo(const CString& strResId) {m_strCommingVideo = strResId;}
	// Set the wall id list of the video
	void setVideoChannelList(VideoChannelList InStringList){this->StringList = InStringList;}
	// Get the wall id list of the video on 
	VideoChannelList* getVideoChannelListPointer(){return &StringList;}
	// Delete Group
	void RemoveGroup(int iGroupID);
	// Get the selected group information by the current control
	void GetSelGroupInfo(GroupInfo** pInfo);
	// Get the group information for the specified group name
	HTREEITEM FindStrGroupInfo(CString strname, GroupInfo** pInfo);

private:
	// Add image resources
	void AddImage(CImageList& imageList, UINT id);
	// Cleanup the configuration management data 
	void ClearProv();
	// Clean up group list data
	void ClearDcGroupsTree();
	// Clean up patch group list data
	void ClearPatchGroupsTree();
	// Get user list of current dispatch console
	void GetDcUsers(const CString strSearch, bool bFlag);
	// Get group list of current dispatch console
	void GetDcGroups(const CString strSearch, bool bFlag);
	// Get patch group list
	void GetPatchGroups(const CString strSearch, bool bFlag);
	// Get group type
	CString GetGroupCategoryString(const CString& strGroupCategory);
	// Gets the type of user within the group
	CString GetGroupUserMemberTypeString(const CString& strMemberType);
	// Obtain image according to user type.
	int GetUserImageId(const CString& strCategory, bool bGray);
	// Obtain image according to user status.
	int GetUserImageId(const CString& strUserId, int iStatus);	
	// Add temporary groups
	void AddTempGroup(const CString& strGroupID);
	// Operate patch group
	void OperatePatchGroup(const CString& strDcID, const CString& strGroupID, const int& iOperateType, const CString& strGroupName = _T(""));
	// Get Snapshot path
	std::string GetSnapshotPath(const std::string& strResId) const;
	CString GetModuleDir();
	

private:
	CListCtrl m_DcUsers;	    // dc users list
	CTreeCtrl m_DcGroups;	    // dc group list 
	CImageList m_ImageList;	    // image list
	CString m_Caller;		    // caller
	CString m_strCommingVideo;  // video request
	VideoChannelList StringList;// video wall list
	CString m_VideoChannel;
	CString m_ResourceID;
	CString strVideoFormat;
	CString strTarResID;

private:
	CRect m_RectMax;		// The big coordinates of interface
	CRect m_RectMin;		// The small coordinates of interface

public:
	CString m_AllResync;	// String callback message ELTE_Event_NotifyProvisionAllResync
	CString m_ResStatus;	// String callback message ELTE_Event_NotifyResourceStatus
	CString m_UserStatus;	// String callback message ELTE_Event_NotifyUserStatus
	CString m_ProvChange;	// String callback message ELTE_Event_NotifyProvisionChange
	CString m_ModuleStatus;	// String callback message ELTE_Event_NotifyModuleStatus
	CString m_VideoStatus;	// String callback message ELTE_Event_NotifportyP2pvideocallStatus
	CString m_CallStatus;	// String callback message ELTE_Event_NotifyP2pcallStatus
	CString m_GroupStatus;	// String callback message ELTE_Event_NotifyGroupStatus
	CString m_GISStatus;	// String callback message ELTE_Event_NotifyGISStatus
	CString m_GISReport;	// String callback message ELTE_Event_NotifyGISReport
	CString m_SDSReport;    // String callback message ELTE_Event_NotifySDSReport
	CString m_SDSStatus;    // String callback message ELTE_Event_NotifySDSStatus

private:
	CDialogEx* m_pELTE_SDK_DemoDlg;	// Login window pointer
	CRECFileDlg m_RECFileDlg;		// Video files query window
	CallDlg m_CallDlg;
	std::string m_strResId;
	VideoCommingDlg m_VideoCommingDlg;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDispatchvideo();
	afx_msg void OnBnClickedButtonUndispatch();
	afx_msg void OnBnClickedButtonSds();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonEvent11();
	afx_msg void OnBnClickedBtnStartvwall();
	afx_msg void OnBnClickedBtnStopvwall();
	CComboBox m_ComboVWIDList;
	afx_msg void OnSelchangeComboResource();
	afx_msg void OnBnClickedBtnRefreshlist();
	afx_msg void OnDblclkListDcusers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCreatepg();
	afx_msg void OnBnClickedButtonPatchgroups();
	CTreeCtrl m_PatchGroups;
	afx_msg void OnRclickTreePatchgroups(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnBnClickedButtonSnapshot();
	public:
		CString m_ModifyDGNAGroupId;
		afx_msg void OnBnClickedButtonGetgissub();
};
