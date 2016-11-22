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

// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_Video.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "DConsoleDlg.h"


IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
	, m_peLTE_Player(NULL)
	, m_strResId(_T(""))
	, m_bUse(0)
	, m_strMuteType(_T(""))
{
	//pDcDlg = NULL;
}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVideoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVideoDlg::OnBnClickedCancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CVideoDlg message handlers


void CVideoDlg::SetELtePlayer(CElte_playerctrl1* peLTE_Player)
{
	m_peLTE_Player = peLTE_Player;
}


void CVideoDlg::SetResId(CString strResId,CString strMuteType, int bUse)
{
	m_strResId = strResId;
	m_strMuteType = strMuteType;
	m_bUse = bUse;
}


void CVideoDlg::SetTile(void)
{
	CString strTitle("Incoming video: ");
	strTitle.Append(m_strResId);
	if(this->GetSafeHwnd())
	{
		SetWindowText(strTitle);
	}
}


void CVideoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!m_bUse)
	{
		CString strRst = m_peLTE_Player->ELTE_OCX_RecvVideoPlay(m_strResId,m_strMuteType);
		CHECK_RESULTE_CODE(strRst, _T("ELTE_OCX_RecvVideoPlay"));
		
		{
			Sleep(200);
			//mute
			CString strMuteParam;
			strMuteParam.Append(_T("<Content>"));
			strMuteParam.Append(_T("<MuteParam>"));
			strMuteParam.Append(_T("<CallType>"));
			strMuteParam.Append(_T("2"));
			strMuteParam.Append(_T("</CallType>"));
			strMuteParam.Append(_T("</MuteParam>"));
			strMuteParam.Append(_T("</Content>"));

			strRst = m_peLTE_Player->ELTE_OCX_VolMute(m_strResId, strMuteParam);
			CHECK_RESULTE_CODE(strRst, _T("ELTE_OCX_VolMute"));

			pDcDlg->GetDlgItem(IDC_BUTTON_UNMUTE)->EnableWindow(TRUE);
			pDcDlg->GetDlgItem(IDC_BUTTON_MUTE)->EnableWindow(FALSE);
			pDcDlg->SetDlgItemText(IDC_EDIT_RESID, m_strResId);
		}

		pDcDlg->PostMessage(WM_SHOW_COMMING_VIDEO, (WPARAM)StrToInt(m_strResId));
	}
	CDialogEx::OnOK();
}


void CVideoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CString strRst = m_peLTE_Player->ELTE_OCX_StopRealPlay(m_strResId);
	CHECK_RESULTE_CODE(strRst, _T("ELTE_OCX_StopRealPlay"));
	CDialogEx::OnCancel();
}

void CVideoDlg::setDcDlg(CDConsoleDlg* pdcDlg)
{
	pDcDlg = pdcDlg;
}

int CVideoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == CDialogEx::OnCreate(lpCreateStruct))
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
