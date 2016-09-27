
// eLTE_SDK_Video_DemoDlg.h : header file
//

#pragma once

#include "DConsoleDlg.h"

// CeLTE_SDK_Video_DemoDlg dialog
class CeLTE_SDK_Video_DemoDlg : public CDialogEx
{
// Construction
public:
	CeLTE_SDK_Video_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ELTE_SDK_VIDEO_DEMO_DIALOG };

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
	//get video info
	std::map<CString, int>& GetVideoInfo(){return m_VideoInfo;}

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
	CString m_strUserName;
	CString m_strServerIP;
	CString m_strLocalIP;
	CString m_strSipPort;
	BOOL    m_bLogin;

	CDConsoleDlg m_DConsoleDlg;	//main wnd
	std::map<CString, int> m_VideoInfo; //resid and videoformat
public:
	afx_msg void OnClose();
};