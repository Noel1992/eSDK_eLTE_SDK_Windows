
// eLTE_SDK_GIS_DemoDlg.h : header file
//

#pragma once

#include "DConsoleDlg.h"
#include "afxwin.h"

// CeLTE_SDK_GIS_DemoDlg dialog
class CeLTE_SDK_GIS_DemoDlg : public CDialogEx
{
// Construction
public:
	CeLTE_SDK_GIS_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ELTE_SDK_GIS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
public:
	// Event call back function
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

public:
	CString& GetCurrentUserName();
	BOOL& GetIsLogin();
	CDConsoleDlg& GetDConsoleDlg();


	void SetIsLogin(BOOL bLogin);

private:
	//get current time
	CString GetTimeString();
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
	afx_msg void OnBnClickedButtonLogin();

private:
	BOOL    m_bLogin;

	BOOL GetLocalIP();
	BOOL WriteIniFile();
	BOOL ReadIniFile();

	CDConsoleDlg m_DConsoleDlg;	//main wnd
public:
	afx_msg void OnClose();
	CComboBox m_cmbLocalIP;
	CString m_strLocalIP;
	CString m_strUserName;
	CString m_strServerIP;
	CString m_strSipPort;
};
