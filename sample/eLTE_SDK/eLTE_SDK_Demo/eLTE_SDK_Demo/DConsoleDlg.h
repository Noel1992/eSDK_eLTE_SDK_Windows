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
description :    业务主对话框，登录成功后显示。用户业务操作和管理
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
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
	// 创建窗口并进行播放
	afx_msg LRESULT StartRealPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT InitDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIncommingVideo(WPARAM wParam, LPARAM lParam);

public:
	// 窗口关闭
	afx_msg void OnClose();
	// 鼠标左键双击，显示/隐藏回调事件
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 显示用户列表
	afx_msg void OnBnClickedButtonDcusers();
	// 显示群组列表
	afx_msg void OnBnClickedButtonDcgroups();
	// 触发状态上报
	afx_msg void OnBnClickedButtonTrigger();
	// 配置管理初始化
	afx_msg void OnBnClickedButtonProvinit();
	// 配置管理退出
	afx_msg void OnBnClickedButtonProvexit();
	// 鼠标右键单击用户列表，对该用户或设备进行业务操作。查看用户信息/发起视频回传/停止视频回传/云台控制等
	afx_msg void OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult);
	// 鼠标右键单击群组列表，对该群组或组用户进行业务操作。查看组或者组内用户信息/订阅/去订阅群组/删除动态组等
	afx_msg void OnNMRClickTreeDcgroups(NMHDR *pNMHDR, LRESULT *pResult);
	// 对菜单选择进行业务操作
	afx_msg void OnClickMenuItem(UINT uID);
	// 创建动态组
	afx_msg void OnBnClickedButtonCreatedg();
	// 创建临时组
	afx_msg void OnBnClickedButtonCreatetg();
	// 录像文件查询
	afx_msg void OnBnClickedButtonGetrecflie();
	// 调度员信息
	afx_msg void OnBnClickedButtonDcinfo();

public:
	// 回调消息显示 ELTE_Event_NotifyProvisionAllResync
	afx_msg void OnBnClickedButtonAllresync();
	// 回调消息显示 ELTE_Event_NotifyResourceStatus
	afx_msg void OnBnClickedButtonResstatus();
	// 回调消息显示 ELTE_Event_NotifyUserStatus
	afx_msg void OnBnClickedButtonUserstatus();
	// 回调消息显示 ELTE_Event_NotifyProvisionChange
	afx_msg void OnBnClickedButtonProvchange();
	// 回调消息显示 ELTE_Event_NotifyModuleStatus
	afx_msg void OnBnClickedButtonModulestatus();
	// 回调消息显示 ELTE_Event_NotifyP2pvideocallStatus
	afx_msg void OnBnClickedButtonVideostatus();
	// 回调消息显示 ELTE_Event_NotifyP2pcallStatus
	afx_msg void OnBnClickedButtonCallstatus();
	// 回调消息显示 ELTE_Event_NotifyGroupStatus
	afx_msg void OnBnClickedButtonGroupstatus();
	// 回调消息显示 ELTE_Event_NotifyGISStatus
	afx_msg void OnBnClickedButtonGisstatus();
	// 回调消息显示 ELTE_Event_NotifyGISReport
	afx_msg void OnBnClickedButtonGisreport();

public:
	// 设置CELTE_SDK_DemoDlg窗口指针
	void SetELTE_SDK_DemoDlg(CDialogEx* pDlg) { m_pELTE_SDK_DemoDlg = pDlg; }
	// 登录成功后初始化界面
	void InitDlg();
	// 创建动态组/临时组成功后在群组列表中添加界面
	void AddGroup(const char* groupId);
	// 创建派接组成功后在群组列表中添加界面
	void AddPatchGroup(const char* groupId);
	// 删除派接组
	void RemovePatchGroup(int iGroupID);
	// 增加派接组成员
	void AddPatchGroupMember(const char* groupId, const char* memberId);
	// 删除派接组成员
	void DelPatchGroupMember(const char* groupId, const char* memberId);
	// 刷新用户列表的用户状态
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	// 刷新来电主叫
	void SetCaller(const CString& strCaller, const bool& bUse) {m_Caller = strCaller; m_CallDlg.SetUse(bUse);}
	//设置视频分发参数
	void SetVideoParam(const CString& videoFormat, const CString& tarResID) {strVideoFormat = videoFormat; strTarResID = tarResID;}
	//获取视频资源ID
	CString GetTarResID() {return strTarResID;}
	//获取视频格式
	CString GetVideoFormat() {return strVideoFormat;}

	// 设置当前需要播放的resid
	void SetPlayResID(std::string strResId);
	// 设置视频回传请求到来
	void SetCommingVideo(const CString& strResId) {m_strCommingVideo = strResId;}
	//设置视频上墙ID列表
	void setVideoChannelList(VideoChannelList InStringList){this->StringList = InStringList;}
	//获取视频上墙ID列表
	VideoChannelList* getVideoChannelListPointer(){return &StringList;}
	// 删除群组
	void RemoveGroup(int iGroupID);
	// 获取当前控件选中的组信息
	void GetSelGroupInfo(GroupInfo** pInfo);
	// 获取指定组名的组信息
	HTREEITEM FindStrGroupInfo(CString strname, GroupInfo** pInfo);

private:
	// 增加图片资源
	void AddImage(CImageList& imageList, UINT id);
	// 配置管理数据清理
	void ClearProv();
	// 群组列表数据清理
	void ClearDcGroupsTree();
	// 派接组列表数据清理
	void ClearPatchGroupsTree();
	// 获取调度台用户列表
	void GetDcUsers(const CString strSearch, bool bFlag);
	// 获取调度台群组列表
	void GetDcGroups(const CString strSearch, bool bFlag);
	// 获取派接组列表
	void GetPatchGroups(const CString strSearch, bool bFlag);
	// 获取群组种类
	CString GetGroupCategoryString(const CString& strGroupCategory);
	// 获取组内用户类型
	CString GetGroupUserMemberTypeString(const CString& strMemberType);
	// 根据用户类型获取资源图片
	int GetUserImageId(const CString& strCategory, bool bGray);
	// 根据用户状态获取资源图片
	int GetUserImageId(const CString& strUserId, int iStatus);	
	// 添加临时组
	void AddTempGroup(const CString& strGroupID);
	// 操作派接组
	void OperatePatchGroup(const CString& strDcID, const CString& strGroupID, const int& iOperateType, const CString& strGroupName = _T(""));
	// 获取抓图名
	std::string GetSnapshotPath(const std::string& strResId) const;
	CString GetModuleDir();
	

private:
	CListCtrl m_DcUsers;	    // 调度台用户列表
	CTreeCtrl m_DcGroups;	    // 调度台群组列表
	CImageList m_ImageList;	    // 图片资源列表
	CString m_Caller;		    // 来电主叫
	CString m_strCommingVideo;  // 视频回传请求到来
	VideoChannelList StringList;//视频上墙列表
	CString m_VideoChannel;
	CString m_ResourceID;
	CString strVideoFormat;
	CString strTarResID;

private:
	CRect m_RectMax;		// 界面大坐标
	CRect m_RectMin;		// 界面小坐标

public:
	CString m_AllResync;	// 回调消息拼接成的字符串 ELTE_Event_NotifyProvisionAllResync
	CString m_ResStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyResourceStatus
	CString m_UserStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyUserStatus
	CString m_ProvChange;	// 回调消息拼接成的字符串 ELTE_Event_NotifyProvisionChange
	CString m_ModuleStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyModuleStatus
	CString m_VideoStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifportyP2pvideocallStatus
	CString m_CallStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyP2pcallStatus
	CString m_GroupStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyGroupStatus
	CString m_GISStatus;	// 回调消息拼接成的字符串 ELTE_Event_NotifyGISStatus
	CString m_GISReport;	// 回调消息拼接成的字符串 ELTE_Event_NotifyGISReport
	CString m_SDSReport;    // 回调消息拼接成的字符串 ELTE_Event_NotifySDSReport
	CString m_SDSStatus;    // 回调消息拼接成的字符串 ELTE_Event_NotifySDSStatus

private:
	CDialogEx* m_pELTE_SDK_DemoDlg;	// 登录窗口指针
	CRECFileDlg m_RECFileDlg;		// 录像文件查询窗口
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
