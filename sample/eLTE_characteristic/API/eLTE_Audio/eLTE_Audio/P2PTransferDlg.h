#pragma once

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
private:
	CString m_strDcID;
	CString m_strSrcNum;
};
