#pragma once
#include "elte_playerctrl1.h"

// P2PTransferDlg dialog

class P2PTransferDlg : public CDialogEx
{
	DECLARE_DYNAMIC(P2PTransferDlg)

public:
	P2PTransferDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~P2PTransferDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_P2PTRANSFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	void SetParamInfo(const CString& strDcID, const CString& strSrcNum);
	void SetELtePlayer(CElte_playerctrl1* peLTE_Player)
	{ m_peLTE_Player = peLTE_Player;}
private:
	CString m_strDcID;
	CString m_strSrcNum;
	CElte_playerctrl1* m_peLTE_Player;
};
