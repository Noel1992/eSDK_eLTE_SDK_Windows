
// eLTE_ResourceManageDlg.h : header file
//

#pragma once
#include "elte_playerctrl1.h"
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
	CElte_playerctrl1 m_eLTE_Player;
	CString m_strKickOff;   // user kicked off

	
public:
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);	
	BOOL ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml);
	afx_msg void OnBnClickedButtonLogin();
	BOOL WriteIniFile();
	void MoreSetting();
	CString GetTimeString();
	afx_msg void OnClose();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
};
