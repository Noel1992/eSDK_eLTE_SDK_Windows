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

#pragma once
#include "afxcmn.h"
#include "CallDlg.h"
#include "RECFileDlg.h"

// DConsoleDlg dialog
#define STATE_INIT		0x00
#define STATE_ISPLAY	0x01
#define STATE_ISSHOW	0x02
#define STATE_ISUNMUTE	0x04
struct ST_REALPLAY_PARAM
{
	UINT uiState;
	UINT uiRemoteVideoPort;
	UINT uiRemoteAudioPort;
	UINT uiLocalVideoPort;
	UINT uiLocalAudioPort;
	UINT uiVideoType;
	CString strRemoteIP;
	CString strLocalIP;
	VideoParamInfo videoParam;

	ST_REALPLAY_PARAM()
	{
		uiState = STATE_INIT;
		uiRemoteVideoPort = 0;
		uiRemoteAudioPort = 0;
		uiLocalVideoPort = 0;
		uiLocalAudioPort = 0;
		uiVideoType = 0;
		strRemoteIP = _T("");
		strLocalIP = _T("");
	}

	ST_REALPLAY_PARAM(const ST_REALPLAY_PARAM& v)
	{
		uiState = v.uiState;
		uiRemoteVideoPort = v.uiRemoteVideoPort;
		uiRemoteAudioPort = v.uiRemoteAudioPort;
		uiLocalVideoPort = v.uiLocalVideoPort;
		uiLocalAudioPort = v.uiLocalAudioPort;
		strRemoteIP = v.strRemoteIP;
		strLocalIP = v.strLocalIP;
		videoParam = v.videoParam;
	}

	void Reset()
	{
		uiState = STATE_INIT;
		uiRemoteVideoPort = 0;
		uiRemoteAudioPort = 0;
		uiLocalVideoPort = 0;
		uiLocalAudioPort = 0;
		strRemoteIP = _T("");
		strLocalIP = _T("");
		videoParam.Reset();
	}
};

#define OCX_ID_BEGIN IDC_ELTE_PLAYERCTRL1
#define OCX_ID_END		(OCX_ID_BEGIN + 100)


class CDConsoleDlg : public CDialog
{
	DECLARE_DYNAMIC(CDConsoleDlg)

public:
	CDConsoleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDConsoleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DCONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnCallMsgReceived(WPARAM wparam, LPARAM lparam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDcusers();
	afx_msg void OnBnClickedButtonDcgroups();
	afx_msg void OnBnClickedButtonDcinfo();
	afx_msg void OnCbnSelchangeComboToolbar();
	afx_msg void OnCbnSelchangeComboLang();
	afx_msg void OnNMRClickTreeDcgroups(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickTreeMenuItem(UINT uID);

	afx_msg void OnBnClickedButtonEvent0();
	afx_msg void OnBnClickedButtonEvent2();
	afx_msg void OnBnClickedButtonEvent6();

//public:
	//void ELTE_PlayerEvent(UINT CtlID, ULONG ulEventId, LPCTSTR pXml);

public:
	void InitDlg();
	void SetCeLTE_AudioDlg(CDialog* pDlg)
		{ m_pCeLTE_AudioDlg = pDlg; }
	void SetParam(const CString& strUserID, const CString& strServerIP, const CString& strLocalIP)
		{ m_strUserID = strUserID; m_strServerIP = strServerIP; m_strLocalIP = strLocalIP; }
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	void SetRealPlayParam(INT iResID, UINT uiLocalVideoPort, UINT uiLocalAudioPort, UINT uiRemoteVideoPort, UINT uiRemoteAudioPort);
	void RealPlayHangup(INT iResID);
	void RealPlayNotConnected(INT iResID, INT iCallStatus);
	HTREEITEM FindStrGroupInfo(CString strname, GroupInfo** pInfo);
	void SetCaller(const CString& strCaller, const int& bUse){m_callDlg.SetResId(strCaller, bUse);}
	void RemoveGroup(int iGroupID);

private:
	void ClearProv();
	//get user list
	void GetUsers(const CString strSearch, bool bFlag);
	//Get Group List
	void GetGroups(const CString strSearch, bool bFlag);
	// get patch group list
	void GetPatchGroups(const CString strSearch, bool bFlag);
	void AddImage(CImageList& imageList, UINT id);
	int GetUserImageId(const CString& strCategory, bool bGray);
	int GetUserImageId(const CString& strUserId, int iStatus);
	CString GetGroupCategoryString(const CString& strGroupCategory);
	CString GetGroupUserMemberTypeString(const CString& strMemberType);
	void ClearDcGroupsTree();
	// Clean up patch group list data
	void ClearPatchGroupsTree();
	// Get the selected group information by the current control
	void GetSelGroupInfo(GroupInfo** pInfo);
	virtual void OnOK();
	afx_msg void OnRclickTreePatchgroups(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemclickListDcusers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboObjsel();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonEvent5();
	afx_msg void OnBnClickedBtnDiscreetlisten();
	afx_msg void OnBnClickedBtnStopdiscreenlisten();
	afx_msg void OnBnClickedButtonRecflist();
	afx_msg void OnNMRClickListDcusers(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CListCtrl m_DcUsers;
	CTreeCtrl m_DcGroups;
	CCallDlg m_callDlg;
	CRECFileDlg m_RECFileDlg;
	CImageList m_ImageList;
	bool       m_bBreakIn;

private:
	CDialog* m_pCeLTE_AudioDlg;
	CString m_strUserID;
	CString m_strServerIP;
	CString m_strLocalIP;
	CString m_strMRSIP;

private:
	std::map<int, ST_REALPLAY_PARAM> m_ResourceMap;

public:
	CRect m_RectMax;
	CRect m_RectMin;
	CString m_strEvent0;
	CString m_strEvent1;
	CString m_strEvent2;
	CString m_strEvent3;
	CString m_strEvent4;
	CString m_strEvent5;
	CString m_strEvent6;
	CString m_strEvent7;
	CString m_strEvent8;
	CString m_strEvent9;
	CString m_strEvent10;
	CString m_strEvent11;	

public:
	CTreeCtrl m_PatchGroups;
	CComboBox m_cmbObjSel;
	CString m_strSearch;

	afx_msg void OnBnClickedBtnCallphone();
	afx_msg void OnBnClickedBtnHangup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
