// PTZCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
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
