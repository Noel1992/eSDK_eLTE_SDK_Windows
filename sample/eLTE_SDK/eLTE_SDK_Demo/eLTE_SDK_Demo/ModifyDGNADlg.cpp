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

// ModifyDGNADlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "ModifyDGNADlg.h"
#include "afxdialogex.h"


// CModifyDGNADlg dialog

IMPLEMENT_DYNAMIC(CModifyDGNADlg, CDialogEx)

CModifyDGNADlg::CModifyDGNADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyDGNADlg::IDD, pParent)
{

}

CModifyDGNADlg::~CModifyDGNADlg()
{
}

void CModifyDGNADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModifyDGNADlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDADDUSER, &CModifyDGNADlg::OnBnClickedButtonAddadduser)
	ON_BN_CLICKED(IDC_BUTTON_ADDDELUSER, &CModifyDGNADlg::OnBnClickedButtonAdddeluser)
	ON_BN_CLICKED(IDC_BUTTON_DELADDUSER, &CModifyDGNADlg::OnBnClickedButtonDeladduser)
	ON_BN_CLICKED(IDC_BUTTON_DELDELUSER, &CModifyDGNADlg::OnBnClickedButtonDeldeluser)
	ON_BN_CLICKED(IDOK, &CModifyDGNADlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CModifyDGNADlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifyDGNADlg message handlers


void CModifyDGNADlg::OnBnClickedButtonAddadduser()
{
	// TODO: Add your control notification handler code here
	CString strUserID;
	GetDlgItemText(IDC_EDIT_ADD_USER_ID, strUserID);
	if (strUserID.IsEmpty())
		return;

	SetDlgItemText(IDC_EDIT_ADD_USER_ID, _T(""));

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_ADDUSERLIST);

	if (LB_ERR != pList->FindString(-1, strUserID))
	{
		return;
	}

	pList->AddString(strUserID);

}


void CModifyDGNADlg::OnBnClickedButtonAdddeluser()
{
	// TODO: Add your control notification handler code here
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_ADDUSERLIST);

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


void CModifyDGNADlg::OnBnClickedButtonDeladduser()
{
	// TODO: Add your control notification handler code here
	CString strUserID;
	GetDlgItemText(IDC_EDIT_DEL_USER_ID, strUserID);
	if (strUserID.IsEmpty())
		return;

	SetDlgItemText(IDC_EDIT_DEL_USER_ID, _T(""));

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_DELUSERLIST);

	if (LB_ERR != pList->FindString(-1, strUserID))
	{
		return;
	}

	pList->AddString(strUserID);
}


void CModifyDGNADlg::OnBnClickedButtonDeldeluser()
{
	// TODO: Add your control notification handler code here
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_DELUSERLIST);

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


void CModifyDGNADlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strGroupID,strDcID;
	GetDlgItemText(IDC_EDIT_GROUPID, strGroupID);
	GetDlgItemText(IDC_EDIT_DCID, strDcID);


	//if (strGroupID.IsEmpty())
	//{
	//	MessageBox(_T("GroupID is empty."));
	//	return;
	//}
	//if (strDcID.IsEmpty())
	//{
	//	MessageBox(_T("DcID is empty."));
	//	return;
	//}
	m_pModifyDGNAParamInfo->GroupID = strGroupID;
	m_pModifyDGNAParamInfo->DcID = strDcID;

	CListBox* pGroupList = (CListBox*)GetDlgItem(IDC_LIST_ADDUSERLIST);
	int iMaxCount = pGroupList->GetCount() > 199 ? 199 : pGroupList->GetCount();
	for (int i=0; i<iMaxCount; i++)
	{
		CString strGroup_ID;
		pGroupList->GetText(i, strGroup_ID);
		m_pModifyDGNAParamInfo->AddList[i] = strGroup_ID;
	}

	CListBox* pUserList = (CListBox*)GetDlgItem(IDC_LIST_DELUSERLIST);
	iMaxCount = pUserList->GetCount() > 199 ? 199 : pUserList->GetCount();
	for (int i=0; i<iMaxCount; i++)
	{
		CString strUser_ID;
		pUserList->GetText(i, strUser_ID);
		m_pModifyDGNAParamInfo->DelList[i] = strUser_ID;
	}
	CDialogEx::OnOK();
}


void CModifyDGNADlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CModifyDGNADlg::SetDGNAParamInfo(CString strDcId, CString strGroupId, ModifyDGNAParamInfo* pInfo)
{
	m_strDcId = strDcId;
	m_pModifyDGNAParamInfo = pInfo;
	m_strGroupId = strGroupId;
}


BOOL CModifyDGNADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DCID);
	pEdit->SetReadOnly(TRUE);
	SetDlgItemText(IDC_EDIT_DCID, m_strDcId);
	CEdit* pEditGroupID = (CEdit*)GetDlgItem(IDC_EDIT_GROUPID);
	pEditGroupID->SetReadOnly(TRUE);
	SetDlgItemText(IDC_EDIT_GROUPID, m_strGroupId);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
