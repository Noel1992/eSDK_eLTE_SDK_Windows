
// eLTE_SDSDlg.h : header file
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_SDSDlg dialog
class CeLTE_SDSDlg : public CDialogEx
{
// construct
public:
	CeLTE_SDSDlg(CWnd* pParent = NULL);	// standard construct function

// dialog data
	enum { IDD = IDD_ELTE_SDS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV supported


// Realization
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
	CString m_strLocalIP;
	CComboBox m_cmbLocalIP;
	CString m_strMRSIP;
	CString m_strName;
	CString m_strPasswd;
	CString m_strServerIP;
	CString m_strSipPort;
	CString m_strLogSavePath;
	CComboBox m_cmbMediaPass;

	static int m_iBypass;

	BOOL m_bIsLogin;
	bool m_bClose;
	BOOL m_bMax;
	CString m_strKickOff;   // user kick off

public:
	afx_msg void OnBnClickedButtonLogin();
	CElte_playerctrl1 m_eLTE_Player;
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);
	
	void MoreSetting();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
	BOOL WriteIniFile();
	CString GetTimeString();
};
