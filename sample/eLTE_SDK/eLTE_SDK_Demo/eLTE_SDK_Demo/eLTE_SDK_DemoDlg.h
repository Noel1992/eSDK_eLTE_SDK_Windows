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
description :    ��¼��ʼ���Ի��򣬸������ûص�����ʼ��ҵ��͵�¼
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 ��ʼ�汾
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
	// ���ڹر�
	afx_msg void OnClose();
	// ������˫������ʾ/������־���ý���
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// ��ʼ�������ûص�����¼
	afx_msg void OnBnClickedButtonLogin();

public:
	// ��ȡserver.ini����ĵ�¼��Ϣ
	BOOL ReadIniFile();
	// �����¼����Ϣ�������´β����ٴ�����
	BOOL WriteIniFile();
	// ��־��������
	void MoreSetting();
	// ��ȡʱ���ַ���
	CString GetTimeString();
	// ��ȡ�û���
	CString GetName(){return m_strName;};

public:
	static CeLTE_SDK_DemoDlg& Instance();
	// �¼��ص�����
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

public:
	CString m_strName;		// �û���
	CString m_strPasswd;	// ����
	CString m_strServerIP;	// ������IP
private:	
	CString m_strLocalIP;	// ����IP
	CString m_strSipPort;	// sip�˿�
	CString m_strLogPath;	// ��־����·��
	int m_iLogLevel;		// ��־����

	CRect m_RectMax;		// ���������
	CRect m_RectMin;		// С��������
	BOOL m_bMax;			// ������־
	BOOL m_bIsLogin;		// ��¼��־
	BOOL m_bIsInit;			// ��ʼ����־
	CString m_strKickOff;   // �û�������

public:
	CDConsoleDlg m_DConsoleDlg;	// ҵ��������
public:
	CString m_strMRSIP;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetLocalIP();
	CComboBox m_comboLocalIP;
};
