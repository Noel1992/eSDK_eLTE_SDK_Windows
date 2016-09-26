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

// P2PTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "P2PTransferDlg.h"
#include "afxdialogex.h"
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
