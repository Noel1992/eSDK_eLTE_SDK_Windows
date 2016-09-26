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

// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
	, m_strTitle1(_T(""))
	, m_strTitle2(_T(""))
	, m_strTitle3(_T(""))
	, m_strTitle4(_T(""))
	, m_strContent1(_T(""))
	, m_strContent2(_T(""))
	, m_strContent3(_T(""))
	, m_strContent4(_T(""))
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
END_MESSAGE_MAP()


// CInfoDlg message handlers
BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_STATIC_INFO1)->SetWindowText(m_strTitle1);
	GetDlgItem(IDC_STATIC_INFO2)->SetWindowText(m_strTitle2);
	GetDlgItem(IDC_STATIC_INFO3)->SetWindowText(m_strTitle3);
	GetDlgItem(IDC_STATIC_INFO4)->SetWindowText(m_strTitle4);
	GetDlgItem(IDC_EDIT_INFO1)->SetWindowText(m_strContent1);
	GetDlgItem(IDC_EDIT_INFO2)->SetWindowText(m_strContent2);
	if(_T("") == m_strTitle3)
	{
		GetDlgItem(IDC_EDIT_INFO3)->ShowWindow(FALSE);
	}
	if(_T("") == m_strTitle4)
	{
		GetDlgItem(IDC_EDIT_INFO4)->ShowWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInfoDlg::SetInfo(int iIndex, CString strTitle, CString strContent)
{
	switch (iIndex)
	{
	case 1:
		{
			m_strTitle1 = strTitle;
			m_strContent1 = strContent;
		}
		break;
	case 2:
		{
			m_strTitle2 = strTitle;
			m_strContent2 = strContent;
		}
		break;
	case 3:
		{
			m_strTitle3 = strTitle;
			m_strContent3 = strContent;
		}
		break;
	case 4:
		{
			m_strTitle4 = strTitle;
			m_strContent4 = strContent;
		}
		break;
	}
}