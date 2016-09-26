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

// ImageButton.cpp : implementation file
//
//lint -e766
#include "stdafx.h"
#include "eLTE_Player.h"
#include "ImageButton.h"
#include "VideoPane.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"


// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CButton)

CImageButton::CImageButton()
	: m_CurImageState(ImgState_Normal)
{
	for (int i = 0; i < IMGAGE_STATE_COUNT; i++)
	{
		m_ImageList[i] = NULL;
	}
}

CImageButton::~CImageButton()
{
	for (int i = 0; i < IMGAGE_STATE_COUNT; i++)
	{
		m_ImageList[i] = NULL;
	}
}


BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(BN_CLICKED, &CImageButton::OnBnClicked)
	ON_MESSAGE(WM_ELTE_INVALID_PARENTWND, OnMsgInvalidParent)
END_MESSAGE_MAP()



// CImageButton message handlers

void CImageButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages

	UpdateImage(dc);
}


void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	TRACKMOUSEEVENT csTME;
	csTME.cbSize = sizeof (csTME);
	csTME.dwFlags = TME_LEAVE | TME_HOVER;
	csTME.hwndTrack = m_hWnd ;// 指定要 追踪 的窗口 
	csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 100ms ，才认为状态为 HOVER
	::_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持 

	CButton::OnMouseMove(nFlags, point);
}


void CImageButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_CurImageState = ImgState_Focus;
	Invalidate(TRUE);

	RelayToolTipEvent(point);

	CButton::OnMouseHover(nFlags, point);
}//lint !e1746


void CImageButton::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	m_CurImageState = ImgState_Normal;
	PostMessage(WM_ELTE_INVALID_PARENTWND);

	CButton::OnMouseLeave();
}


void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_CurImageState = ImgState_Select;

	CButton::OnLButtonDown(nFlags, point);
}


void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_CurImageState = ImgState_Normal;

	CButton::OnLButtonUp(nFlags, point);
}


void CImageButton::OnBnClicked()
{
	// TODO: Add your control notification handler code here

	// 发送点击事件
	CWnd* pParentWnd = GetParent();
	if (NULL != pParentWnd)
	{
		pParentWnd->PostMessage(WM_ELTE_IMG_BTN_CLICKED, (WPARAM)GetDlgCtrlID(), NULL);
	}
}//lint !e1762


LRESULT CImageButton::OnMsgInvalidParent(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pParentWnd = GetParent();
	if (NULL != pParentWnd)
	{
		CRect rect;
		pParentWnd->GetClientRect(rect);
		rect.top = rect.bottom - VIDEO_TOOLBAR_WIDTH;
		pParentWnd->InvalidateRect(rect, TRUE);
	}

	return 0L;
}//lint !e1762


void CImageButton::LoadImages(const CString& strNormal, const CString& strSelect,
	const CString& strFocus, const CString& strDisable, IMAGE_STATE_E eCurImageState)
{
	// 先删除原有的Image资源，实现可以更新Image资源
	for (int i = 0; i < IMGAGE_STATE_COUNT; i++)
	{
		CString strFilePath;
		if (ImgState_Normal == i)
		{
			strFilePath = strNormal;
		}
		else if (ImgState_Select == i)
		{
			strFilePath = strSelect;
		}
		else if (ImgState_Focus == i)
		{
			strFilePath = strFocus;
		}
		else if (ImgState_Disable == i)
		{
			strFilePath = strDisable;
		}

		m_ImageList[i] = Gdiplus::Image::FromFile(strFilePath);
		std::string strPath = eLTE_Tool::UnicodeToANSI(strFilePath.GetString()).c_str();
		if (NULL == m_ImageList[i])
		{
			
			LOG_RUN_ERROR("m_ImageList[%d] is null. (%s)", i, strPath.c_str());
		}
		else
		{
			if (Gdiplus::Ok != m_ImageList[i]->GetLastStatus())
			{
				LOG_RUN_ERROR("Gdiplus::Image::FromFile failed. (%s)", strPath.c_str());
				m_ImageList[i] = NULL;
			}
		}
	}

	m_CurImageState = eCurImageState;
}


void CImageButton::SetCurImgState(IMAGE_STATE_E eCurImageState)
{
	m_CurImageState = eCurImageState;
	Invalidate(TRUE);
}


void CImageButton::AddToolTip(LPCTSTR lpszText)
{
	if (NULL == m_ToolTipCtrl.GetSafeHwnd())
	{
		m_ToolTipCtrl.Create(this);
		m_ToolTipCtrl.SetTipBkColor(RGB(70, 70, 70));
		m_ToolTipCtrl.SetTipTextColor(RGB(255, 255, 255));
		m_ToolTipCtrl.AddTool(this, lpszText);
		m_ToolTipCtrl.Activate(TRUE);
	}
}


void CImageButton::UpdateToolTip(LPCTSTR lpszText)
{
	if (NULL == m_ToolTipCtrl.GetSafeHwnd())
	{
		m_ToolTipCtrl.Create(this);
		m_ToolTipCtrl.SetTipBkColor(RGB(70, 70, 70));
		m_ToolTipCtrl.SetTipTextColor(RGB(255, 255, 255));
		m_ToolTipCtrl.AddTool(this, lpszText);
		m_ToolTipCtrl.Activate(TRUE);
	}
	else
	{
		m_ToolTipCtrl.UpdateTipText(lpszText, this);
	}
}


void CImageButton::UpdateImage(const CPaintDC& dc)
{
	Gdiplus::Image *pImg = m_ImageList[m_CurImageState];
	if (NULL != pImg)
	{
		Gdiplus::Graphics graphics(dc);
		INT iWidth = (INT)pImg->GetWidth();
		INT iHeight = (INT)pImg->GetHeight();
		graphics.DrawImage(pImg, 0, 0, iWidth, iHeight);
	}
}//lint !e1762


void CImageButton::RelayToolTipEvent(const CPoint &point)
{
	if (NULL != m_ToolTipCtrl.GetSafeHwnd())
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = WM_MOUSEMOVE;
		msg.time = 0;
		msg.pt.x = LOWORD(point.y);
		msg.pt.y = HIWORD(point.y);
		msg.wParam = LOWORD(point.x);
		msg.lParam = LOWORD(point.y);

		m_ToolTipCtrl.RelayEvent(&msg);
	}
}
//lint -e766
