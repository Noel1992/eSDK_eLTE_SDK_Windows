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
description :    ҵ�����Ի��򣬵�¼�ɹ�����ʾ���û�ҵ������͹���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 ��ʼ�汾
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
	// �������ڲ����в���
	afx_msg LRESULT StartRealPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT InitDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIncommingVideo(WPARAM wParam, LPARAM lParam);

public:
	// ���ڹر�
	afx_msg void OnClose();
	// ������˫������ʾ/���ػص��¼�
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// ��ʾ�û��б�
	afx_msg void OnBnClickedButtonDcusers();
	// ��ʾȺ���б�
	afx_msg void OnBnClickedButtonDcgroups();
	// ����״̬�ϱ�
	afx_msg void OnBnClickedButtonTrigger();
	// ���ù����ʼ��
	afx_msg void OnBnClickedButtonProvinit();
	// ���ù����˳�
	afx_msg void OnBnClickedButtonProvexit();
	// ����Ҽ������û��б��Ը��û����豸����ҵ��������鿴�û���Ϣ/������Ƶ�ش�/ֹͣ��Ƶ�ش�/��̨���Ƶ�
	afx_msg void OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult);
	// ����Ҽ�����Ⱥ���б��Ը�Ⱥ������û�����ҵ��������鿴����������û���Ϣ/����/ȥ����Ⱥ��/ɾ����̬���
	afx_msg void OnNMRClickTreeDcgroups(NMHDR *pNMHDR, LRESULT *pResult);
	// �Բ˵�ѡ�����ҵ�����
	afx_msg void OnClickMenuItem(UINT uID);
	// ������̬��
	afx_msg void OnBnClickedButtonCreatedg();
	// ������ʱ��
	afx_msg void OnBnClickedButtonCreatetg();
	// ¼���ļ���ѯ
	afx_msg void OnBnClickedButtonGetrecflie();
	// ����Ա��Ϣ
	afx_msg void OnBnClickedButtonDcinfo();

public:
	// �ص���Ϣ��ʾ ELTE_Event_NotifyProvisionAllResync
	afx_msg void OnBnClickedButtonAllresync();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyResourceStatus
	afx_msg void OnBnClickedButtonResstatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyUserStatus
	afx_msg void OnBnClickedButtonUserstatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyProvisionChange
	afx_msg void OnBnClickedButtonProvchange();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyModuleStatus
	afx_msg void OnBnClickedButtonModulestatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyP2pvideocallStatus
	afx_msg void OnBnClickedButtonVideostatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyP2pcallStatus
	afx_msg void OnBnClickedButtonCallstatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyGroupStatus
	afx_msg void OnBnClickedButtonGroupstatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyGISStatus
	afx_msg void OnBnClickedButtonGisstatus();
	// �ص���Ϣ��ʾ ELTE_Event_NotifyGISReport
	afx_msg void OnBnClickedButtonGisreport();

public:
	// ����CELTE_SDK_DemoDlg����ָ��
	void SetELTE_SDK_DemoDlg(CDialogEx* pDlg) { m_pELTE_SDK_DemoDlg = pDlg; }
	// ��¼�ɹ����ʼ������
	void InitDlg();
	// ������̬��/��ʱ��ɹ�����Ⱥ���б�����ӽ���
	void AddGroup(const char* groupId);
	// �����ɽ���ɹ�����Ⱥ���б�����ӽ���
	void AddPatchGroup(const char* groupId);
	// ɾ���ɽ���
	void RemovePatchGroup(int iGroupID);
	// �����ɽ����Ա
	void AddPatchGroupMember(const char* groupId, const char* memberId);
	// ɾ���ɽ����Ա
	void DelPatchGroupMember(const char* groupId, const char* memberId);
	// ˢ���û��б���û�״̬
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	// ˢ����������
	void SetCaller(const CString& strCaller, const bool& bUse) {m_Caller = strCaller; m_CallDlg.SetUse(bUse);}
	//������Ƶ�ַ�����
	void SetVideoParam(const CString& videoFormat, const CString& tarResID) {strVideoFormat = videoFormat; strTarResID = tarResID;}
	//��ȡ��Ƶ��ԴID
	CString GetTarResID() {return strTarResID;}
	//��ȡ��Ƶ��ʽ
	CString GetVideoFormat() {return strVideoFormat;}

	// ���õ�ǰ��Ҫ���ŵ�resid
	void SetPlayResID(std::string strResId);
	// ������Ƶ�ش�������
	void SetCommingVideo(const CString& strResId) {m_strCommingVideo = strResId;}
	//������Ƶ��ǽID�б�
	void setVideoChannelList(VideoChannelList InStringList){this->StringList = InStringList;}
	//��ȡ��Ƶ��ǽID�б�
	VideoChannelList* getVideoChannelListPointer(){return &StringList;}
	// ɾ��Ⱥ��
	void RemoveGroup(int iGroupID);
	// ��ȡ��ǰ�ؼ�ѡ�е�����Ϣ
	void GetSelGroupInfo(GroupInfo** pInfo);
	// ��ȡָ������������Ϣ
	HTREEITEM FindStrGroupInfo(CString strname, GroupInfo** pInfo);

private:
	// ����ͼƬ��Դ
	void AddImage(CImageList& imageList, UINT id);
	// ���ù�����������
	void ClearProv();
	// Ⱥ���б���������
	void ClearDcGroupsTree();
	// �ɽ����б���������
	void ClearPatchGroupsTree();
	// ��ȡ����̨�û��б�
	void GetDcUsers(const CString strSearch, bool bFlag);
	// ��ȡ����̨Ⱥ���б�
	void GetDcGroups(const CString strSearch, bool bFlag);
	// ��ȡ�ɽ����б�
	void GetPatchGroups(const CString strSearch, bool bFlag);
	// ��ȡȺ������
	CString GetGroupCategoryString(const CString& strGroupCategory);
	// ��ȡ�����û�����
	CString GetGroupUserMemberTypeString(const CString& strMemberType);
	// �����û����ͻ�ȡ��ԴͼƬ
	int GetUserImageId(const CString& strCategory, bool bGray);
	// �����û�״̬��ȡ��ԴͼƬ
	int GetUserImageId(const CString& strUserId, int iStatus);	
	// �����ʱ��
	void AddTempGroup(const CString& strGroupID);
	// �����ɽ���
	void OperatePatchGroup(const CString& strDcID, const CString& strGroupID, const int& iOperateType, const CString& strGroupName = _T(""));
	// ��ȡץͼ��
	std::string GetSnapshotPath(const std::string& strResId) const;
	CString GetModuleDir();
	

private:
	CListCtrl m_DcUsers;	    // ����̨�û��б�
	CTreeCtrl m_DcGroups;	    // ����̨Ⱥ���б�
	CImageList m_ImageList;	    // ͼƬ��Դ�б�
	CString m_Caller;		    // ��������
	CString m_strCommingVideo;  // ��Ƶ�ش�������
	VideoChannelList StringList;//��Ƶ��ǽ�б�
	CString m_VideoChannel;
	CString m_ResourceID;
	CString strVideoFormat;
	CString strTarResID;

private:
	CRect m_RectMax;		// ���������
	CRect m_RectMin;		// ����С����

public:
	CString m_AllResync;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyProvisionAllResync
	CString m_ResStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyResourceStatus
	CString m_UserStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyUserStatus
	CString m_ProvChange;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyProvisionChange
	CString m_ModuleStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyModuleStatus
	CString m_VideoStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifportyP2pvideocallStatus
	CString m_CallStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyP2pcallStatus
	CString m_GroupStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyGroupStatus
	CString m_GISStatus;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyGISStatus
	CString m_GISReport;	// �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifyGISReport
	CString m_SDSReport;    // �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifySDSReport
	CString m_SDSStatus;    // �ص���Ϣƴ�ӳɵ��ַ��� ELTE_Event_NotifySDSStatus

private:
	CDialogEx* m_pELTE_SDK_DemoDlg;	// ��¼����ָ��
	CRECFileDlg m_RECFileDlg;		// ¼���ļ���ѯ����
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
