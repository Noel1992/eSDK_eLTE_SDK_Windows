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

// CallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "CallDlg.h"
#include "afxdialogex.h"
#include "Xml.h"


// CCallDlg dialog

IMPLEMENT_DYNAMIC(CCallDlg, CDialogEx)

CCallDlg::CCallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCallDlg::IDD, pParent)
	, m_peLTE_Player(NULL)
	, m_bUse(0)
{

}

CCallDlg::~CCallDlg()
{
	m_peLTE_Player = NULL;
	m_bUse = 0;
}

void CCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCallDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCallDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCallDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCallDlg message handlers


void CCallDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!m_bUse)
	{
		CString strRst = m_peLTE_Player->ELTE_OCX_P2PRecv(m_strResId);
		CHECK_RESULTE_CODE(strRst, _T("ELTE_OCX_P2PRecv"));
	}
	CDialogEx::OnOK();
}


void CCallDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CString strRst = m_peLTE_Player->ELTE_OCX_P2PReject(m_strResId);
	CHECK_RESULTE_CODE(strRst, _T("ELTE_OCX_P2PReject"));
	CDialogEx::OnCancel();
}

void CCallDlg::SetResId(CString strResId, int bUse)
{
	m_strResId = strResId;
	m_bUse = bUse;
}

void CCallDlg::SetTile()
{
	CString strTitle("Incoming call: ");
	strTitle.Append(m_strResId);
	if(this->GetSafeHwnd())
	{
		SetWindowText(strTitle);
	}
}
