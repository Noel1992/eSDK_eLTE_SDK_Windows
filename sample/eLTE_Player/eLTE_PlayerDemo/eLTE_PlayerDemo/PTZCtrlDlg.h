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
	// 设置返回参数
	void SetParam(UINT* pCode, UINT* pValue)
	{
		m_pCode = pCode;
		m_pValue = pValue;
	}

private:
	UINT* m_pCode;		// 操作类型
	UINT* m_pValue;		// 操作数值
};
