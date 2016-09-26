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
filename    :    InfoDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    用户信息群组信息显示对话框
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
*********************************************************************/

#pragma once


// CInfoDlg dialog

class CInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 对话框初始化
	virtual BOOL OnInitDialog();

public:
	// 设置对话框显示参数值
	void SetInfo(int iIndex, CString strTitle, CString strContent);

private:
	CString m_strTitle1;		// 第一行显示标题
	CString m_strTitle2;		// 第二行显示标题
	CString m_strTitle3;		// 第三行显示标题
	CString m_strTitle4;		// 第四行显示标题
	CString m_strContent1;		// 第一行显示文本
	CString m_strContent2;		// 第二行显示文本
	CString m_strContent3;		// 第三行显示文本
	CString m_strContent4;		// 第四行显示文本
};
