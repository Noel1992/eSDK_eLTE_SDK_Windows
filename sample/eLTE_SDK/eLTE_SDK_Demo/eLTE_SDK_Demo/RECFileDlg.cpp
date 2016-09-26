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

// RECFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Demo.h"
#include "RECFileDlg.h"
#include "afxdialogex.h"
#include "eLTE_Tool.h"
#include "eLTE_SDK_DemoDlg.h"


// CRECFileDlg dialog

IMPLEMENT_DYNAMIC(CRECFileDlg, CDialogEx)

CRECFileDlg::CRECFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRECFileDlg::IDD, pParent)
{

}

CRECFileDlg::~CRECFileDlg()
{
}

void CRECFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRECFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRECFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRECFileDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRECFileDlg message handlers


void CRECFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	static int iFlag = 0;
	ELTE_INT32 iRet;
	if(0 == iFlag)
	{
		//连接录音录像服务器
		iRet = ELTE_SDK_ProvisionManagerInitMRS(eLTE_Tool::UnicodeToANSI(CeLTE_SDK_DemoDlg::Instance().m_strMRSIP).c_str());

		CHECK_API_RETURN(iRet, _T("ELTE_SDK_ProvisionManagerInitMRS"));
		if (0 == iRet)
		{
			iFlag = 1;
		}
	}
	CString CallType, Caller, Callee, ResourceID, StartSec, EndSec;
	GetDlgItemText(IDC_EDIT_CALLTYPE, CallType);
	GetDlgItemText(IDC_EDIT_CALLER, Caller);
	GetDlgItemText(IDC_EDIT_CALLEE, Callee);
	GetDlgItemText(IDC_EDIT_RESID, ResourceID);
	GetDlgItemText(IDC_EDIT_STARTSEC, StartSec);
	GetDlgItemText(IDC_EDIT_ENDSEC, EndSec);

	/************************************************************************
	--- XML 格式 ---
		<Content>
			<RECQueryInfo>
				<CallType></CallType>
				<Caller></Caller>
				<Callee></Callee>
				<ResourceID></ResourceID>
				<StartSec></StartSec>
				<EndSec></EndSec>
			</RECQueryInfo>
		</Content>
	************************************************************************/
	CString strQuery;
	strQuery.Append(_T("<Content>"));
	strQuery.Append(_T("<RECQueryInfo>"));
	strQuery.Append(_T("<CallType>"));
	strQuery.Append(CallType);
	strQuery.Append(_T("</CallType>"));
	strQuery.Append(_T("<Caller>"));
	strQuery.Append(Caller);
	strQuery.Append(_T("</Caller>"));
	strQuery.Append(_T("<Callee>"));
	strQuery.Append(Callee);
	strQuery.Append(_T("</Callee>"));
	strQuery.Append(_T("<ResourceID>"));
	strQuery.Append(ResourceID);
	strQuery.Append(_T("</ResourceID>"));
	strQuery.Append(_T("<StartSec>"));
	strQuery.Append(StartSec);
	strQuery.Append(_T("</StartSec>"));
	strQuery.Append(_T("<EndSec>"));
	strQuery.Append(EndSec);
	strQuery.Append(_T("</EndSec>"));
	strQuery.Append(_T("</RECQueryInfo>"));
	strQuery.Append(_T("</Content>"));

	ELTE_CHAR* pQueryResult = NULL;
	iRet = ELTE_SDK_GetUserRECFileInfoList(eLTE_Tool::UnicodeToANSI(strQuery).c_str(), &pQueryResult);
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_GetUserRECFileInfoList"));

	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<RECFileInfoList>
			<RECFileInfo>
				<CallType></CallType>
				<Caller></Caller>
				<Callee></Callee>
				<ResourceID></ResourceID>
				<StartSec></StartSec>
				<EndSec></EndSec>
				<UrlFTP></UrlFTP>
				<UrlRTSP></UrlRTSP>
			</RECFileInfo>
		<RECFileInfoList>
	</Content>
	************************************************************************/
	SetDlgItemText(IDC_EDIT_QUERYLIST, eLTE_Tool::ANSIToUnicode(pQueryResult));

	ELTE_SDK_ReleaseBuffer(pQueryResult);
	pQueryResult = NULL;
}


void CRECFileDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_CALLTYPE, _T(""));
	SetDlgItemText(IDC_EDIT_CALLER, _T(""));
	SetDlgItemText(IDC_EDIT_CALLEE, _T(""));
	SetDlgItemText(IDC_EDIT_RESID, _T(""));
	SetDlgItemText(IDC_EDIT_STARTSEC, _T(""));
	SetDlgItemText(IDC_EDIT_ENDSEC, _T(""));
	SetDlgItemText(IDC_EDIT_QUERYLIST, _T(""));

	CDialogEx::OnCancel();
}
