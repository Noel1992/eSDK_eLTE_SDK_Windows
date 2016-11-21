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
filename    :    CreatePatchGroupDlg.h
author      :    zhangziqian wx229156
created     :    2016/02/02
description :    Create a patch group dialog, is responsible for the creation of the set of parameters
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2016/02/02 Initial version
*********************************************************************/

#pragma once


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
	CString m_strDcId;					    // DcID
	CString m_strGroupId;                   // Patch group id
	CString m_strGroupName;                 // Patch group name
};
