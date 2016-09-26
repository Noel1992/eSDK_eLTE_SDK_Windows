
// eLTE_AudioDlg.h : 头文件
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_AudioDlg 对话框
class CeLTE_AudioDlg : public CDialogEx
{
// 构造
public:
	CeLTE_AudioDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ELTE_AUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// Implementation
private:
	CDConsoleDlg m_DcDlg;

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:	
	CElte_playerctrl1 m_eLTE_Player;
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
	CString m_strKickOff;   // 用户被挤掉
	
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
};
