#pragma once


// CPTZCtrlDlg dialog

class CPTZCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPTZCtrlDlg)

public:
	CPTZCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPTZCtrlDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	// set return parameter
	void SetParam(UINT* pCode, UINT* pValue)
	{
		m_pCode = pCode;
		m_pValue = pValue;
	}

private:
	UINT* m_pCode;		// operation type
	UINT* m_pValue;		// operation value
};
