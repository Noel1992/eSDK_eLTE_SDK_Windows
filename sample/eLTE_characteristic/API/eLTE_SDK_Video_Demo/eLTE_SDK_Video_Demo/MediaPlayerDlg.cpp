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

// MediaPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_SDK_Video_Demo.h"
#include "MediaPlayerDlg.h"
#include "afxdialogex.h"
#include "MediaPlayerDlgMgr.h"


// CMediaPlayerDlg dialog

IMPLEMENT_DYNAMIC(CMediaPlayerDlg, CDialogEx)

CMediaPlayerDlg::CMediaPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediaPlayerDlg::IDD, pParent)
{
	m_brush.CreateSolidBrush(RGB(0, 0, 0));
}

CMediaPlayerDlg::~CMediaPlayerDlg()
{
	if (NULL != m_videoStatic.GetSafeHwnd())
	{
		m_videoStatic.DestroyWindow();
	}
}

void CMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMediaPlayerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMediaPlayerDlg message handlers


void CMediaPlayerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	ELTE_INT32 iRet = ELTE_SDK_StopRealPlay(m_strResId.c_str());
	//CHECK_API_RETURN_VOID(iRet, _T("ELTE_SDK_StopRealPlay"));

	CMediaPlayerDlgMgr::Instance().DeleteCreateMediaPlayerDlg(m_strResId);

	//DestroyWindow();

	//CDialogEx::OnClose();
}

void CMediaPlayerDlg::CreateVideoStatic(const CRect& rect)
{
	CRect rtVideo(rect);

	BOOL bRet = m_videoStatic.Create(NULL, WS_CHILD | WS_VISIBLE, rtVideo, this);
	m_videoStatic.SetParent(this);

	m_videoStatic.Invalidate(TRUE);
}

HBRUSH CMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	
	if(m_videoStatic.GetSafeHwnd() && pWnd == CWnd::FromHandle(m_videoStatic.GetSafeHwnd()))
	{
		return m_brush;
	}

	return hbr;
}


void CMediaPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
}


void CMediaPlayerDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::PostNcDestroy();

	delete this;
}


void CMediaPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (NULL != m_videoStatic.GetSafeHwnd())
	{
		m_videoStatic.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);
		ELTE_SDK_SetPlayWindowSize(m_strResId.c_str(), cx, cy);
	}
}


BOOL CMediaPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return FALSE;
	}
	else
	{
		return CDialogEx::PreTranslateMessage(pMsg);
	}
}
