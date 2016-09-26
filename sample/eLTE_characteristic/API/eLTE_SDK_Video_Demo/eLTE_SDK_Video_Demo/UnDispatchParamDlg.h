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


// UnDispatchParamDlg dialog

class UnDispatchParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UnDispatchParamDlg)

public:
	UnDispatchParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~UnDispatchParamDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_UNDISPATCHPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetUnDispatchParamInfo(UnDispatchParamInfo* pUnDispatchParamInfo)
	{ m_pUnDispatchParamInfo = pUnDispatchParamInfo; }
private:
	UnDispatchParamInfo* m_pUnDispatchParamInfo;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
