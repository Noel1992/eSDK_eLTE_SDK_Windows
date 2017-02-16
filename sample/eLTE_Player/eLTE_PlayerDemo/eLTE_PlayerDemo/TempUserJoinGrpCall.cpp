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

// TempUserJoinGrpCall.cpp : implementation file
//

#include "stdafx.h"
#include "TempUserJoinGrpCall.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Xml.h"


// CTempUserJoinGrpCall dialog

IMPLEMENT_DYNAMIC(CTempUserJoinGrpCall, CDialogEx)

CTempUserJoinGrpCall::CTempUserJoinGrpCall(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTempUserJoinGrpCall::IDD, pParent)
{

}

CTempUserJoinGrpCall::~CTempUserJoinGrpCall()
{
}

void CTempUserJoinGrpCall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempUserJoinGrpCall, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTempUserJoinGrpCall::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTempUserJoinGrpCall::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTempUserJoinGrpCall message handlers


void CTempUserJoinGrpCall::OnBnClickedOk()
{
	
	CString strDstNum = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_USERID))->GetWindowText(strDstNum);


	CString strPhonePatchParam = _T("");
	strPhonePatchParam.Append(_T("<Content>"));
	strPhonePatchParam.Append(_T("<PhonePatchParam>"));
	strPhonePatchParam.Append(_T("<DcID>"));
	strPhonePatchParam.Append(m_strDcID);
	strPhonePatchParam.Append(_T("</DcID>"));
	strPhonePatchParam.Append(_T("<UserID>"));
	strPhonePatchParam.Append(strDstNum);
	strPhonePatchParam.Append(_T("</UserID>"));
	strPhonePatchParam.Append(_T("</PhonePatchParam>"));
	strPhonePatchParam.Append(_T("</Content>"));

// 	CString strResult = m_peLTE_Player->ELTE_OCX_TempUserJoinGroup(m_strSrcNum, strPhonePatchParam);
// 	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_TempUserJoinGroup"));

	CDialogEx::OnOK();
}


void CTempUserJoinGrpCall::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}

void CTempUserJoinGrpCall::SetParamInfo(const CString& strDcID, const CString& strSrcNum)
{
	m_strDcID = strDcID;
	m_strSrcNum = strSrcNum;
}
