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
	// ������������
	void SetPCHGRPParamInfo(CString strDcId, CString strGroupId, CString strGroupName, PCHGRPParamInfo* pInfo);

private:
	PCHGRPParamInfo* m_pPCHGRPParamInfo;	// ��������
	CString m_strDcId;					    // DcIDֵ
	CString m_strGroupId;                   // Patch Group��
	CString m_strGroupName;                 // Patch Group����
};
