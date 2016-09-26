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
description :    创建动态组临时组对话框，负责创建组时的参数设置
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
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
	// 对话框初始化
	virtual BOOL OnInitDialog();
	// 往群组列表中添加一个群组
	afx_msg void OnBnClickedButtonAddgroup();
	// 从群组列表中删除一个群组
	afx_msg void OnBnClickedButtonDelgroup();
	// 往用户列表中添加一个用户
	afx_msg void OnBnClickedButtonAdduser();
	// 从用户列表中删除一个用户
	afx_msg void OnBnClickedButtonDeluser();
	// 单击OK按钮，完成创建组时参数设置
	afx_msg void OnBnClickedOk();
	// 单击Cancel按键，取消创建操作
	afx_msg void OnBnClickedCancel();

public:
	// 创建参数传递
	void SetDGNAParamInfo(CString strDcId, DGNAParamInfo* pInfo, bool bDynamic);

private:
	DGNAParamInfo* m_pDGNAParamInfo;	// 创建参数
	CString m_strDcId;					// DcID值
	bool m_bDynamic;					// 创建动态组还是临时组标志
};
