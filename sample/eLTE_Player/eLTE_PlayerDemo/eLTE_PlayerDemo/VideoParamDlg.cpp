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
#include "eLTE_PlayerDemo.h"
#include "VideoParamDlg.h"
#include "afxdialogex.h"


// CVideoParamDlg dialog

IMPLEMENT_DYNAMIC(CVideoParamDlg, CDialog)

CVideoParamDlg::CVideoParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoParamDlg::IDD, pParent)
	, m_pVideoParamInfo(NULL)
{

}

CVideoParamDlg::~CVideoParamDlg()
{
}

void CVideoParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CVideoParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVideoParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CVideoParamDlg message handlers

BOOL CVideoParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
		pCComboBox->InsertString(0, _T("rear camera"));
		pCComboBox->InsertString(0, _T("front camera"));
		pCComboBox->SetCurSel(0);
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CONFIRM);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("need confirm"));
		pCComboBox->InsertString(0, _T("without confirm"));
		pCComboBox->SetCurSel(0);
	}

	pCComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MUTE);
	if (NULL != pCComboBox)
	{
		pCComboBox->InsertString(0, _T("Mute"));
		pCComboBox->InsertString(0, _T("Unmute"));
		pCComboBox->SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
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

	CDialog::OnOK();
}


void CVideoParamDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
