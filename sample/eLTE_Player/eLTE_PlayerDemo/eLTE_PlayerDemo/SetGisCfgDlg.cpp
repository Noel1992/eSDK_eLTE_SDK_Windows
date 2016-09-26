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

// SetGisCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetGisCfgDlg.h"
#include "afxdialogex.h"
#include "Xml.h"


// CSetGisCfgDlg dialog

IMPLEMENT_DYNAMIC(CSetGisCfgDlg, CDialogEx)

CSetGisCfgDlg::CSetGisCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetGisCfgDlg::IDD, pParent)
	, m_strResId(_T(""))
{

}

CSetGisCfgDlg::~CSetGisCfgDlg()
{
}

void CSetGisCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetGisCfgDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetGisCfgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetGisCfgDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetGisCfgDlg message handlers


void CSetGisCfgDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strPeriod = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_PERIOD))->GetWindowText(strPeriod);
	CString strDistance = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_DISTANCE))->GetWindowText(strDistance);

	CString strGisParam = _T("");
	strGisParam.Append(_T("<Content>"));
	strGisParam.Append(_T("<UeGisCfgInfo>"));
	strGisParam.Append(_T("<ReportPeriod>"));
	strGisParam.Append(strPeriod);
	strGisParam.Append(_T("</ReportPeriod>"));
	strGisParam.Append(_T("<ReportDistance>"));
	strGisParam.Append(strDistance);
	strGisParam.Append(_T("</ReportDistance>"));
	strGisParam.Append(_T("</UeGisCfgInfo>"));
	strGisParam.Append(_T("</Content>"));

	//CString strResult = m_peLTE_Player->ELTE_OCX_SetGisParam(m_strResId, strGisParam);
	//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_SetGisParam"));

	CDialogEx::OnOK();
}


void CSetGisCfgDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CSetGisCfgDlg::SetParamInfo(const CString& strResId)
{
	m_strResId = strResId;
}