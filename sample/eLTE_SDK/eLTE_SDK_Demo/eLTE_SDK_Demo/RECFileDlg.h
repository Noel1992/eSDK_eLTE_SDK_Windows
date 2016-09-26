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
filename    :    RECFileDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    录像文件查询对话框，负责查询录像文件列表
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
*********************************************************************/

#pragma once


// CRECFileDlg dialog

class CRECFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRECFileDlg)

public:
	CRECFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRECFileDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 单击OK按键，使用查询条件中设置的参数，查询录像文件列表
	afx_msg void OnBnClickedOk();
	// 单击Cancel按键，取消录像文件查询操作
	afx_msg void OnBnClickedCancel();
};
