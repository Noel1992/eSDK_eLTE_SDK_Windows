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
description :    登录初始化对话框，负责设置回调，初始化业务和登录
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
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
	// 窗口关闭
	afx_msg void OnClose();
	// 鼠标左键双击，显示/隐藏日志设置界面
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 初始化、设置回调、登录
	afx_msg void OnBnClickedButtonLogin();

public:
	// 读取server.ini里面的登录信息
	BOOL ReadIniFile();
	// 保存登录的信息，方便下次不用再次输入
	BOOL WriteIniFile();
	// 日志参数设置
	void MoreSetting();
	// 获取时间字符串
	CString GetTimeString();
	// 获取用户名
	CString GetName(){return m_strName;};

public:
	static CeLTE_SDK_DemoDlg& Instance();
	// 事件回调函数
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

public:
	CString m_strName;		// 用户名
	CString m_strPasswd;	// 密码
	CString m_strServerIP;	// 服务器IP
private:	
	CString m_strLocalIP;	// 本地IP
	CString m_strSipPort;	// sip端口
	CString m_strLogPath;	// 日志保存路径
	int m_iLogLevel;		// 日志级别

	CRect m_RectMax;		// 大界面坐标
	CRect m_RectMin;		// 小界面坐标
	BOOL m_bMax;			// 界面大标志
	BOOL m_bIsLogin;		// 登录标志
	BOOL m_bIsInit;			// 初始化标志
	CString m_strKickOff;   // 用户被挤掉

public:
	CDConsoleDlg m_DConsoleDlg;	// 业务主界面
public:
	CString m_strMRSIP;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetLocalIP();
	CComboBox m_comboLocalIP;
};
