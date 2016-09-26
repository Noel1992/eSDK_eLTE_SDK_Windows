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

// PTZCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "PTZCtrlDlg.h"
#include "afxdialogex.h"


// CPTZCtrlDlg dialog

IMPLEMENT_DYNAMIC(CPTZCtrlDlg, CDialogEx)

CPTZCtrlDlg::CPTZCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPTZCtrlDlg::IDD, pParent)
	, m_pCode(NULL)
	, m_pValue(NULL)
{

}

CPTZCtrlDlg::~CPTZCtrlDlg()
{
}

void CPTZCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPTZCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPTZCtrlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPTZCtrlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPTZCtrlDlg message handlers


void CPTZCtrlDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	*m_pCode = GetDlgItemInt(IDC_EDIT_PTZCODE);
	*m_pValue = GetDlgItemInt(IDC_EDIT_PTZVALUE);

	CDialogEx::OnOK();
}


void CPTZCtrlDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_pCode = NULL;
	m_pValue = NULL;
	CDialogEx::OnCancel();
}
