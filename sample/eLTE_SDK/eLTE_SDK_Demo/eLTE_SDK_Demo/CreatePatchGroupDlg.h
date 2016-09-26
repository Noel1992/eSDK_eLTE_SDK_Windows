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
description :    创建派接组对话框，负责创建组时的参数设置
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2016/02/02 初始版本
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
	// 创建参数传递
	void SetPCHGRPParamInfo(CString strDcId, CString strGroupId, CString strGroupName, PCHGRPParamInfo* pInfo);

private:
	PCHGRPParamInfo* m_pPCHGRPParamInfo;	// 创建参数
	CString m_strDcId;					    // DcID值
	CString m_strGroupId;                   // 派接组号
	CString m_strGroupName;                 // 派接组名称
};
