
// eLTE_SDSDlg.h : ͷ�ļ�
//

#pragma once
#include "DConsoleDlg.h"
#include "afxwin.h"


// CeLTE_SDSDlg �Ի���
class CeLTE_SDSDlg : public CDialogEx
{
// ����
public:
	CeLTE_SDSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ELTE_SDS_DIALOG };

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
	CString m_strKickOff;   // �û�������

public:
	static ELTE_VOID __SDK_CALL ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

private:
	CDConsoleDlg& GetDConsoleDlg();
	CString& GetCurrentUserName();
	BOOL& GetIsLogin();
	void SetIsLogin(BOOL bLogin);

public:
	afx_msg void OnBnClickedButtonLogin();
	DECLARE_EVENTSINK_MAP()
	
	void MoreSetting();
	BOOL GetLocalIP();
	BOOL ReadIniFile();
	BOOL WriteIniFile();
	CString GetTimeString();
};
