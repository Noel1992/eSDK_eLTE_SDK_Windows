
// eLTE_VideoDlg.h : 头文件
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"

// CeLTE_VideoDlg 对话框
class CeLTE_VideoDlg : public CDialogEx
{
// 构造
public:
	CeLTE_VideoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ELTE_VIDEO_DIALOG };

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
	CElte_playerctrl1 m_eLTE_Player;
	CString m_strLocalIP;
	CComboBox m_cmbLocalIP;
	CComboBox m_cmbMediaPass;
	CString m_strMRSIP;
	CString m_strName;
	CString m_strPasswd;
	CString m_strServerIP;
	CString m_strSipPort;
	static int m_iBypass;
	CString m_strLogSavePath;
	bool m_bClose;
	BOOL m_bIsLogin;
	BOOL m_bMax;
	CString m_strKickOff;   // 用户被挤掉

public:
	void MoreSetting();
	BOOL WriteIniFile();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
	afx_msg void OnBnClickedButtonLogin();
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);
	afx_msg void OnClose();
	BOOL ELTE_OCX_PlayerEventEltePlayerctrl1(UINT CtlID, unsigned long ulEventType, LPCTSTR pEventDataXml);
	CString GetTimeString();
};
