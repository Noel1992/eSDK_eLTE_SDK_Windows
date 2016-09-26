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
filename    :    PTZCtrlDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    云台控制参数设置对话框
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
*********************************************************************/

#pragma once


// CPTZCtrlDlg dialog

class CPTZCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPTZCtrlDlg)

public:
	CPTZCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPTZCtrlDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PTZCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// 单击OK按键，返回设置参数进行云台控制
	afx_msg void OnBnClickedOk();
	// 单击Cancel按键，取消云台控制操作
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
