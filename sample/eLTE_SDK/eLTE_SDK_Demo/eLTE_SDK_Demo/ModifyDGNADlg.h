/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#pragma once


// CModifyDGNADlg dialog

class CModifyDGNADlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyDGNADlg)

public:
	CModifyDGNADlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyDGNADlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFYDGNA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddadduser();
	afx_msg void OnBnClickedButtonAdddeluser();
	afx_msg void OnBnClickedButtonDeladduser();
	afx_msg void OnBnClickedButtonDeldeluser();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	// set dgna param info
	void SetDGNAParamInfo(CString strDcId, CString strGroupId, ModifyDGNAParamInfo* pInfo);

private:
	ModifyDGNAParamInfo* m_pModifyDGNAParamInfo;	// param
	CString m_strDcId;					            // DcID value
	CString m_strGroupId;
public:
	virtual BOOL OnInitDialog();
};
