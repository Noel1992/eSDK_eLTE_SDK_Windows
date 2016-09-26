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
#include "eLTE_SDK_Demo.h"
#include "CallDlg.h"
#include "afxdialogex.h"
#include "eLTE_SDK.h"
#include "DataType.h"
#include "eLTE_Tool.h"


// CallDlg dialog

IMPLEMENT_DYNAMIC(CallDlg, CDialogEx)

CallDlg::CallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CallDlg::IDD, pParent)
	, m_bUse(false)
{

}

CallDlg::~CallDlg()
{
}

void CallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CallDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CallDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CallDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CallDlg message handlers


void CallDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!m_bUse)
	{
		ELTE_INT32 iRet = ELTE_SDK_P2PRecv(eLTE_Tool::UnicodeToANSI(m_Caller).c_str());
		CHECK_API_RETURN(iRet, _T("ELTE_SDK_P2PRecv"));
	}

	CDialogEx::OnOK();
}


void CallDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	ELTE_INT32 iRet = ELTE_SDK_P2PReject(eLTE_Tool::UnicodeToANSI(m_Caller).c_str());
	CHECK_API_RETURN(iRet, _T("ELTE_SDK_P2PReject"));

	CDialogEx::OnCancel();
}
