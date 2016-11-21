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
description :    Login initialization dialog, is responsible for setting the callback, initialization business and login
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 Initial version
*********************************************************************/

// eLTE_SDK_DemoDlg.h : header file
//

#pragma once

#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_SDK_DemoDlg dialog
class CeLTE_SDK_DemoDlg : public CDialogEx
{
// Construction
public:
	CeLTE_SDK_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ELTE_SDK_DEMO_DIALOG };

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
	// Close window
	afx_msg void OnClose();
	// Left mouse button, display / hide log settings interface
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// Initialization, set callback function, login
	afx_msg void OnBnClickedButtonLogin();

public:
	// Read the login information from the server.ini
	BOOL ReadIniFile();
	// Save the login information, to facilitate the next time do not have to re-enter
	BOOL WriteIniFile();
	// Log parameter setting
	void MoreSetting();
	// Get time string
	CString GetTimeString();
	// Get user name
	CString GetName(){return m_strName;};

public:
	static CeLTE_SDK_DemoDlg& Instance();
	// event callbacks function
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

public:
	CString m_strName;		// User name
	CString m_strPasswd;	// User password
	CString m_strServerIP;	// Server ip
private:	
	CString m_strLocalIP;	// Local ip
	CString m_strSipPort;	// sip port
	CString m_strLogPath;	// Log save path
	int m_iLogLevel;		// Log level

	CRect m_RectMax;		// Large interface coordinates
	CRect m_RectMin;		// Small interface coordinates
	BOOL m_bMax;			// the flag of large interface
	BOOL m_bIsLogin;		// the flag of logining
	BOOL m_bIsInit;			// Initialization flag
	CString m_strKickOff;   // The user is out

public:
	CDConsoleDlg m_DConsoleDlg;	// Business main interface
public:
	CString m_strMRSIP;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetLocalIP();
	CComboBox m_comboLocalIP;
};
