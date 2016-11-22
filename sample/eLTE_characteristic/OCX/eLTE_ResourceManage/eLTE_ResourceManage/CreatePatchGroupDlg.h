#pragma once

#include "stdafx.h"

// CCreatePatchGroupDlg dialog

class CCreatePatchGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreatePatchGroupDlg)

public:
	CCreatePatchGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreatePatchGroupDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_CREATEPATCHGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddgroup();
	afx_msg void OnBnClickedButtonDelgroup();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

public:
	// Create parameter transfer
	void SetPCHGRPParamInfo(CString strDcId, CString strGroupId, CString strGroupName, PCHGRPParamInfo* pInfo);

private:
	PCHGRPParamInfo* m_pPCHGRPParamInfo;	// Create parameter
	CString m_strDcId;					    // DcID value
	CString m_strGroupId;                   // Patch Group id
	CString m_strGroupName;                 // Patch Group name
};
