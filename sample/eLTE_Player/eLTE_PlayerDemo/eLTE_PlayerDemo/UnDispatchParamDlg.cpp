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

// UnDispatchParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "UnDispatchParamDlg.h"
#include "afxdialogex.h"


// UnDispatchParamDlg dialog

IMPLEMENT_DYNAMIC(UnDispatchParamDlg, CDialogEx)

UnDispatchParamDlg::UnDispatchParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(UnDispatchParamDlg::IDD, pParent)
	, m_pUnDispatchParamInfo(NULL)
{

}

UnDispatchParamDlg::~UnDispatchParamDlg()
{
}

void UnDispatchParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UnDispatchParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &UnDispatchParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &UnDispatchParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// UnDispatchParamDlg message handlers


void UnDispatchParamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_USERORRESID))->GetWindowText(m_pUnDispatchParamInfo->strUsrId);

	CDialogEx::OnOK();
}


BOOL UnDispatchParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void UnDispatchParamDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
