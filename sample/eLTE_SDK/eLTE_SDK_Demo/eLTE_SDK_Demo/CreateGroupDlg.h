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

/********************************************************************
filename    :    CreateGroupDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    Create a dynamic group of temporary groups dialog box, is responsible for the creation of the set of parameters
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 Initial version
*********************************************************************/

#pragma once


// CCreateGroupDlg dialog

class CCreateGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateGroupDlg)

public:
	CCreateGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateGroupDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CREATEGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// Add a group to the group list
	afx_msg void OnBnClickedButtonAddgroup();
	// Delete a group from the group list
	afx_msg void OnBnClickedButtonDelgroup();
	// Add a user to the user list
	afx_msg void OnBnClickedButtonAdduser();
	// Delete a user from the user list
	afx_msg void OnBnClickedButtonDeluser();
	// Click the OK button to complete the parameter settings
	afx_msg void OnBnClickedOk();
	// Click the Cancel button to cancel the creation of the operation
	afx_msg void OnBnClickedCancel();

public:
	// Create parameter transfer
	void SetDGNAParamInfo(CString strDcId, DGNAParamInfo* pInfo, bool bDynamic);

private:
	DGNAParamInfo* m_pDGNAParamInfo;	// Create parameter
	CString m_strDcId;					// DcID
	bool m_bDynamic;					// The flag of Create a dynamic group or temporary group
};
