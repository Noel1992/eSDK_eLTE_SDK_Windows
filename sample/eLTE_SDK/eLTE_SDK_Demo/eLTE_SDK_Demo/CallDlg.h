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
filename    :    CallDlg.h
author      :    zhangrui z00233055
created     :    2015/07/06
description :    Incoming call processing dlg
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/07/06 Initial version
*********************************************************************/

#pragma once


// CallDlg dialog

class CallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CallDlg)

public:
	CallDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CallDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CString m_Caller;
	bool m_bUse;    //Whether it has been answered

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void SetCaller(const CString& strCaller) {m_Caller = strCaller;}
	void SetUse(const bool& bUse){m_bUse = bUse;}
};
