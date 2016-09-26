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

// VideoParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "VideoParamDlg.h"
#include "afxdialogex.h"


// CVideoParamDlg dialog

IMPLEMENT_DYNAMIC(CVideoParamDlg, CDialogEx)

CVideoParamDlg::CVideoParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoParamDlg::IDD, pParent)
	, m_pVideoParamInfo(NULL)
{

}

CVideoParamDlg::~CVideoParamDlg()
{
}

void CVideoParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoParamDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CVideoParamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CVideoParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVideoParamDlg message handlers


BOOL CVideoParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("1080P"));
		pCComboBox->InsertString(0, _T("720P"));
		pCComboBox->InsertString(0, _T("QCIF"));
		pCComboBox->InsertString(0, _T("CIF"));
		pCComboBox->InsertString(0, _T("D1"));
		pCComboBox->SetCurSel(0);
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("后置摄像头"));
		pCComboBox->InsertString(0, _T("前置摄像头"));
		pCComboBox->SetCurSel(0);
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CONFIRM);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("需要用户确认"));
		pCComboBox->InsertString(0, _T("不需要用户确认"));
		pCComboBox->SetCurSel(0);
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MUTE);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("无伴音"));
		pCComboBox->InsertString(0, _T("需要伴音"));
		pCComboBox->SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CVideoParamDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CVideoParamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	int iSel = pCComboBox->GetCurSel();
	if (0 == iSel)
	{
		m_pVideoParamInfo->strFormat = _T("D1");
	}
	else if (1 == iSel)
	{
		m_pVideoParamInfo->strFormat = _T("CIF");
	}
	else if (2 == iSel)
	{
		m_pVideoParamInfo->strFormat = _T("QCIF");
	}
	else if (3 == iSel)
	{
		m_pVideoParamInfo->strFormat = _T("720P");
	}
	else if (4 == iSel)
	{
		m_pVideoParamInfo->strFormat = _T("1080P");
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAMERA);
	iSel = pCComboBox->GetCurSel();
	if (0 == iSel)
	{
		m_pVideoParamInfo->strCamera = _T("0");
	}
	else if (1 == iSel)
	{
		m_pVideoParamInfo->strCamera = _T("1");
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CONFIRM);
	iSel = pCComboBox->GetCurSel();
	if (0 == iSel)
	{
		m_pVideoParamInfo->strConfirm = _T("0");
	}
	else if (1 == iSel)
	{
		m_pVideoParamInfo->strConfirm = _T("1");
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MUTE);
	pCComboBox->GetLBText(pCComboBox->GetCurSel(), m_pVideoParamInfo->strMute);
	iSel = pCComboBox->GetCurSel();
	if (0 == iSel)
	{
		m_pVideoParamInfo->strMute = _T("0");
	}
	else if (1 == iSel)
	{
		m_pVideoParamInfo->strMute = _T("1");
	}

	CDialogEx::OnOK();
}
