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


// SDSMessage dialog

class SDSMessage : public CDialogEx
{
	DECLARE_DYNAMIC(SDSMessage)

public:
	SDSMessage(CWnd* pParent = NULL);   // standard constructor
	virtual ~SDSMessage();

// Dialog Data
	enum { IDD = IDD_DIALOG_SDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedCancel();
	CString m_message;
	CString m_filePath;
	CString m_receiver;
private:
	SDSParamInfo* m_pSDSParamInfo;
public:
	void SetSDSParamInfo(CString strDcId, SDSParamInfo* pInfo);
};
