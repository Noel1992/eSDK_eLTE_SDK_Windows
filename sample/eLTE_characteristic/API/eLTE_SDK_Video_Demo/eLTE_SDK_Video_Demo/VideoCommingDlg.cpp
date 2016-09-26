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

// VideoCommingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Video_Demo.h"
#include "VideoCommingDlg.h"
#include "afxdialogex.h"
#include "eLTE_Tool.h"
#include "eLTE_SDK_Video_DemoDlg.h"
#include "DConsoleDlg.h"


// VideoCommingDlg dialog

IMPLEMENT_DYNAMIC(VideoCommingDlg, CDialogEx)

VideoCommingDlg::VideoCommingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(VideoCommingDlg::IDD, pParent)
{

}

VideoCommingDlg::~VideoCommingDlg()
{
}

void VideoCommingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(VideoCommingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &VideoCommingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &VideoCommingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// VideoCommingDlg message handlers


void VideoCommingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ELTE_INT32 iRet = ELTE_SDK_RecvVideoPlay(eLTE_Tool::UnicodeToANSI(m_strResId).c_str());
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_RecvVideoPlay"));
	m_dcDlg->SetPlayResID(eLTE_Tool::UnicodeToANSI(m_strResId).c_str());
	m_dcDlg->PostMessage(WM_START_REAL_PLAY);
	CDialogEx::OnOK();
}

void VideoCommingDlg::SetDConsoleDlg(CDConsoleDlg* pDlg)
{ 
	m_dcDlg = pDlg; 
}

void VideoCommingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	ELTE_INT32 iRet = ELTE_SDK_StopRealPlay(eLTE_Tool::UnicodeToANSI(m_strResId).c_str());
	CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StopRealPlay"));

	CDialogEx::OnCancel();
}
