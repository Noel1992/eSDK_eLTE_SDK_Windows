
// eLTE_GISDlg.h : ͷ�ļ�
//

#pragma once
#include "elte_playerctrl1.h"
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_GISDlg �Ի���
class CeLTE_GISDlg : public CDialogEx
{
// ����
public:
	CeLTE_GISDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ELTE_GIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
	CDConsoleDlg m_DcDlg;

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	CString m_strName;
	CString m_strSipPort;
	CString m_strServerIP;
	CString m_strPasswd;
	CComboBox m_cmbLocalIP;
	CString m_strLocalIP;
	static int m_iBypass;
	bool m_bClose;
	BOOL m_bIsLogin;
	BOOL m_bMax;
	CString m_strLogSavePath;
	CElte_playerctrl1 m_eLTE_Player;
	CString m_strKickOff;   // user ������

public:
	BOOL GetLocalIP();
	BOOL ReadIniFile();
	void MoreSetting();
	BOOL WriteIniFile();
	DECLARE_EVENTSINK_MAP()
	void ELTE_OCX_EventEltePlayerctrl1(unsigned long ulEventType, LPCTSTR pEventDataXml);
	CString GetTimeString();
};
