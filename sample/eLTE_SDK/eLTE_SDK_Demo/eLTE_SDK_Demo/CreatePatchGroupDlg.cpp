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

// CreatePatchGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "CreatePatchGroupDlg.h"
#include "afxdialogex.h"


// CCreatePatchGroupDlg dialog

IMPLEMENT_DYNAMIC(CCreatePatchGroupDlg, CDialogEx)

CCreatePatchGroupDlg::CCreatePatchGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreatePatchGroupDlg::IDD, pParent)
	, m_pPCHGRPParamInfo(NULL)
	, m_strDcId(_T(""))
	, m_strGroupId(_T(""))
	, m_strGroupName(_T(""))
{

}

CCreatePatchGroupDlg::~CCreatePatchGroupDlg()
{
}

void CCreatePatchGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreatePatchGroupDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDGROUP, &CCreatePatchGroupDlg::OnBnClickedButtonAddgroup)
	ON_BN_CLICKED(IDC_BUTTON_DELGROUP, &CCreatePatchGroupDlg::OnBnClickedButtonDelgroup)
	ON_BN_CLICKED(IDOK, &CCreatePatchGroupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCreatePatchGroupDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCreatePatchGroupDlg message handlers


void CCreatePatchGroupDlg::OnBnClickedButtonAddgroup()
{
	// TODO: Add your control notification handler code here
	CString strGroupID;
	GetDlgItemText(IDC_EDIT_GROUP_ID, strGroupID);
	if (strGroupID.IsEmpty())
		return;

	SetDlgItemText(IDC_EDIT_GROUP_ID, _T(""));

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_GROUPLIST);

	if (LB_ERR != pList->FindString(-1, strGroupID))
	{
		return;
	}

	pList->AddString(strGroupID);
}


void CCreatePatchGroupDlg::OnBnClickedButtonDelgroup()
{
	// TODO: Add your control notification handler code here
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_GROUPLIST);

	int nMaxItems = pList->GetSelCount();
	LPINT rgIndex = new INT[nMaxItems];
	memset(rgIndex, 0, sizeof(INT)*nMaxItems);
	pList->GetSelItems(nMaxItems, rgIndex);

	for (int i=0; i<nMaxItems; i++)
	{
		pList->DeleteString(rgIndex[i]-i);
	}

	delete []rgIndex;
}


void CCreatePatchGroupDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strDcID,strAlias;
	GetDlgItemText(IDC_EDIT_DCID, strDcID);
	GetDlgItemText(IDC_EDIT_ALIAS, strAlias);

	m_pPCHGRPParamInfo->DcID = strDcID;
	m_pPCHGRPParamInfo->Alias = strAlias;
	m_pPCHGRPParamInfo->GroupID = m_strGroupId;
	CListBox* pGroupList = (CListBox*)GetDlgItem(IDC_LIST_GROUPLIST);
	int iMaxCount = pGroupList->GetCount() > 20 ? 20 : pGroupList->GetCount();
	for (int i=0; i<iMaxCount; i++)
	{
		CString strGroup_ID;
		pGroupList->GetText(i, strGroup_ID);
		m_pPCHGRPParamInfo->GroupList[i] = strGroup_ID;
	}


	CDialogEx::OnOK();
}


void CCreatePatchGroupDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL CCreatePatchGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DCID);
	pEdit->SetReadOnly(TRUE);
	SetDlgItemText(IDC_EDIT_DCID, m_strDcId);

	if(_T("") != m_strGroupName)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALIAS);
		pEdit->SetReadOnly(TRUE);
		SetDlgItemText(IDC_EDIT_ALIAS, m_strGroupName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCreatePatchGroupDlg::SetPCHGRPParamInfo(CString strDcId, CString strGroupId, CString strGroupName, PCHGRPParamInfo* pInfo)
{
	m_strDcId = strDcId;
	m_pPCHGRPParamInfo = pInfo;
	m_strGroupId = strGroupId;
	m_strGroupName = strGroupName;
}
