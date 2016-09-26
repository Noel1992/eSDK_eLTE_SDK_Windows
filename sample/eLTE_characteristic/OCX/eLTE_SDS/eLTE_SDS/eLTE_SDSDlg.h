
// eLTE_SDSDlg.h : 头文件
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_SDSDlg 对话框
class CeLTE_SDSDlg : public CDialogEx
{
// 构造
public:
	CeLTE_SDSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ELTE_SDS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
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
	CString m_strKickOff;   // user 被挤掉

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
