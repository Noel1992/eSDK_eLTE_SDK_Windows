// P2PTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "P2PTransferDlg.h"
#include "afxdialogex.h"
#include "Xml.h"


// P2PTransferDlg dialog

IMPLEMENT_DYNAMIC(P2PTransferDlg, CDialogEx)

P2PTransferDlg::P2PTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(P2PTransferDlg::IDD, pParent)
	, m_peLTE_Player(NULL)
{

}

P2PTransferDlg::~P2PTransferDlg()
{
	m_peLTE_Player = NULL;
}

void P2PTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(P2PTransferDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &P2PTransferDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &P2PTransferDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// P2PTransferDlg message handlers


void P2PTransferDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strDstNum = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_DSTNUMBER))->GetWindowText(strDstNum);


	CString p2pTransferParam = _T("");
	p2pTransferParam.Append(_T("<Content>"));
	p2pTransferParam.Append(_T("<DcID>"));
	p2pTransferParam.Append(m_strDcID);
	p2pTransferParam.Append(_T("</DcID>"));
	p2pTransferParam.Append(_T("<SpeakerID>"));
	p2pTransferParam.Append(m_strSrcNum);
	p2pTransferParam.Append(_T("</SpeakerID>"));
	p2pTransferParam.Append(_T("<ObjectID>"));
	p2pTransferParam.Append(strDstNum);
	p2pTransferParam.Append(_T("</ObjectID>"));
	p2pTransferParam.Append(_T("</Content>"));

	CString strResult = m_peLTE_Player->ELTE_OCX_P2PTransfer(m_strDcID, p2pTransferParam);
	//CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_P2PTransfer"));

	CDialogEx::OnOK();
}


void P2PTransferDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void P2PTransferDlg::SetParamInfo(const CString& strDcID, const CString& strSrcNum)
{
	m_strDcID = strDcID;
	m_strSrcNum = strSrcNum;
}
