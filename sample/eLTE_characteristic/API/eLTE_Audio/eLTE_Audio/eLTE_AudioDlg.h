
// eLTE_AudioDlg.h : header file
//

#pragma once
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_AudioDlg dlg
class CeLTE_AudioDlg : public CDialogEx
{
// construct
public:
	CeLTE_AudioDlg(CWnd* pParent = NULL);	// Standard Constructors

// Dialog data
	enum { IDD = IDD_ELTE_AUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
private:
	CDConsoleDlg m_DcDlg;

protected:
	HICON m_hIcon;

	// Generated message mapping function
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:	
	CString m_strSipPort;
	CString m_strServerIP;
	CString m_strPasswd;
//	CEdit m_strName;
//	CEdit m_MRSIP;
	CString m_strName;
	CString m_strMRSIP;
	CComboBox m_cmbMediaPass;
	CComboBox m_cmbLocalIP;
	CString m_strLocalIP;
	CString m_strLogSavePath;
	static int m_iBypass;
	BOOL m_bIsLogin;
	BOOL m_bMax;
	bool m_bClose;
	CString m_strKickOff;   // kick off
	
public:
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

public:
	afx_msg void OnBnClickedButtonLogin();
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);
	afx_msg void OnClose();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
	BOOL WriteIniFile();
	void MoreSetting();
	CString GetTimeString();

private:
	CDConsoleDlg& GetDConsoleDlg();
	CString& GetCurrentUserName();
	BOOL& GetIsLogin();
	void SetIsLogin(BOOL bLogin);
};
