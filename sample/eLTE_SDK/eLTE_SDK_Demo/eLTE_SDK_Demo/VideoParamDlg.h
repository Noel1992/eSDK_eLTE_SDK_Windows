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
filename    :    VideoParamDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    Video parameter settings dialog
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 Initial version
*********************************************************************/

#pragma once


// CVideoParamDlg dialog

class CVideoParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoParamDlg)

public:
	CVideoParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoParamDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIDEOPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Dialog initialization
	virtual BOOL OnInitDialog();
	// Click the Cancel button to cancel the operation
	afx_msg void OnBnClickedCancel();
	// Click the OK button, set the video back  parameter
	afx_msg void OnBnClickedOk();

public:
	// Set return parameters
	void SetVideoParamInfo(VideoParamInfo* pVideoParamInfo)
	{ m_pVideoParamInfo = pVideoParamInfo; }

private:
	VideoParamInfo* m_pVideoParamInfo;	// Video feedback params
};
