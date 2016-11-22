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


// eLTE_PlayerDemoDlg.h : header file
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_PlayerDemoDlg dialog
class CeLTE_PlayerDemoDlg : public CDialogEx
{
// Construction
public:
	CeLTE_PlayerDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ELTE_PLAYERDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);
	BOOL ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml);

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	BOOL ReadIniFile();
	BOOL WriteIniFile();
	void MoreSetting();
	CString GetTimeString();
	BOOL GetLocalIP();
public:
	CString GetMRSIP();

private:
	CElte_playerctrl1 m_eLTE_Player;
	CDConsoleDlg m_DcDlg;

public:
	CString m_strName;
	CString m_strPasswd;
	CString m_strServerIP;
	CString m_strLocalIP;
	CString m_strSipPort;
	
	CString m_strLogSavePath;
	CString m_strBGLogoPath;
	CComboBox m_cmbLogLevel;

	CRect m_RectMax;
	CRect m_RectMin;

	BOOL m_bIsLogin;
	BOOL m_bMax;
	CComboBox m_cmbMediaPass;
	static int m_iBypass;
	CString m_strMRSIP;
	bool m_bClose;
	CString m_strKickOff;   // user was kicked off
	CComboBox m_comboLocalIP;
};
