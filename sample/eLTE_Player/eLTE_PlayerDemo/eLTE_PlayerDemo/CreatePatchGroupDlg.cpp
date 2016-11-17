// CreatePatchGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
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
	
	CString strDcID,strAlias;
	GetDlgItemText(IDC_EDIT_DCID, strDcID);
	GetDlgItemText(IDC_EDIT_ALIAS, strAlias);

	m_pPCHGRPParamInfo->DcID = strDcID;
	m_pPCHGRPParamInfo->Alias = strAlias;
	m_pPCHGRPParamInfo->GroupID = m_strGroupId;
	CListBox* pGroupList = (CListBox*)GetDlgItem(IDC_LIST_GROUPLIST);
	int iMaxCount = pGroupList->GetCount() > 8 ? 8 : pGroupList->GetCount();
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
	
	CDialogEx::OnCancel();
}

void CCreatePatchGroupDlg::SetPCHGRPParamInfo(CString strDcId, CString strGroupId, CString strGroupName, PCHGRPParamInfo* pInfo)
{
	m_strDcId = strDcId;
	m_pPCHGRPParamInfo = pInfo;
	m_strGroupId = strGroupId;
	m_strGroupName = strGroupName;
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
