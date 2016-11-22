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
#include "elte_playerctrl1.h"
//#include "DConsoleDlg.h"

// CVideoDlg dialog

class CDConsoleDlg;

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// OCX object pointer
	CElte_playerctrl1* m_peLTE_Player;
	CString m_strResId;
	CString m_strMuteType;
	int m_bUse;
	void SetELtePlayer(CElte_playerctrl1* peLTE_Player);
	void SetResId(CString strResId,CString strMuteType, int bUse);
	void SetTile(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void setDcDlg(CDConsoleDlg* pDcDlg);

private:
	CDConsoleDlg* pDcDlg; 
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
