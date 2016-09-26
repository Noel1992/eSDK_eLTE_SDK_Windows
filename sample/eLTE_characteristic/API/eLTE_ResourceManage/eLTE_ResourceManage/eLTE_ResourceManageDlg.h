
// eLTE_ResourceManageDlg.h : header file
//

#pragma once
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_ResourceManageDlg dialog
class CeLTE_ResourceManageDlg : public CDialogEx
{
// Construction
public:
	CeLTE_ResourceManageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ELTE_RESOURCEMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
private:
	CDConsoleDlg m_DcDlg;

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strLocalIP;
	CComboBox m_cmbLocalIP;
	//CComboBox m_cmbLogLevel;
	CComboBox m_cmbMediaPass;
	CString m_strLogSavePath;
	CString m_strMRSIP;
	CString m_strName;
	CString m_strPasswd;
	CString m_strServerIP;
	CString m_strSipPort;
	static int m_iBypass;
	bool m_bClose;

	BOOL m_bIsLogin;
	BOOL m_bMax;
	CString m_strKickOff;   // ”√ªß±ªº∑µÙ

public:
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

private:
	CDConsoleDlg& GetDConsoleDlg();
	CString& GetCurrentUserName();
	BOOL& GetIsLogin();
	void SetIsLogin(BOOL bLogin);
	
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedButtonLogin();
	BOOL WriteIniFile();
	void MoreSetting();
	CString GetTimeString();
	afx_msg void OnClose();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
};
