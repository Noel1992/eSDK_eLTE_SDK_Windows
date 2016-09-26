// P2PTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_Audio.h"
#include "P2PTransferDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "eLTE_Tool.h"


// P2PTransferDlg dialog

IMPLEMENT_DYNAMIC(P2PTransferDlg, CDialogEx)

	P2PTransferDlg::P2PTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(P2PTransferDlg::IDD, pParent)
{

}

P2PTransferDlg::~P2PTransferDlg()
{
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

	ELTE_INT32 iRet = ELTE_SDK_P2PTransfer(eLTE_Tool::UnicodeToANSI(m_strDcID).c_str(), eLTE_Tool::UnicodeToANSI(p2pTransferParam).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_P2PTransfer"));

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
