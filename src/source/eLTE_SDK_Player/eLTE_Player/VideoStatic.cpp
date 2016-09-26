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

// VideoStatic.cpp : implementation file
//

#include "stdafx.h"
#include "VideoStatic.h"
#include "eLTE_PlayerCtrl.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"


// CVideoStatic

IMPLEMENT_DYNAMIC(CVideoStatic, CStatic)

CVideoStatic::CVideoStatic()
	: m_bIsFullScreen(FALSE)
	, m_pParentWnd(NULL)
	, m_skinType(0)
	, m_pImage(NULL)
{
	memset(&m_struOldWndpl, 0, sizeof(m_struOldWndpl));
}

CVideoStatic::~CVideoStatic()
{
	m_pParentWnd = NULL;
	m_pImage = NULL;
}


BEGIN_MESSAGE_MAP(CVideoStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVideoStatic message handlers




void CVideoStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	if (eLTE_SKIN_BLACK == m_skinType)
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, BLACK_COLOR);
		SetBackGroudImage(dc);
	}
	else if (eLTE_SKIN_WHITE == m_skinType)
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, WHITE_COLOR);
		SetBackGroudImage(dc);
	}
}

void CVideoStatic::SetParam(int skinType, const CString& strLogoPath)
{
	m_skinType = skinType;
	m_strLogoPath = strLogoPath;
}

void CVideoStatic::SetParam(const CString& strLogoPath)
{
	m_strLogoPath = strLogoPath;
}

eLTE_RET CVideoStatic::SetFullScreen()
{
	LOG_TRACE();

	if (m_bIsFullScreen)
	{
		LOG_RUN_INFO("Is already a full-screen.");
		return eLTE_ERR_SUCCESS;
	}
	
	if (NULL == m_pParentWnd || NULL == m_pParentWnd->GetSafeHwnd())
	{
		LOG_RUN_ERROR("m_pParentWnd is null or parentwnd had been destory.");
		return eLTE_ERR_FAILED;
	}

	m_pParentWnd->ShowWindow(SW_HIDE);
	SetParent(NULL);
	ModifyStyle(WS_CHILD, WS_POPUP, SWP_NOMOVE | SWP_NOSIZE);
	SetForegroundWindow();
	SetFocus();
	GetCursorPos(&m_OldCursorPoint);
	ShowCursor(FALSE);

	HMONITOR hMonitor = MonitorFromWindow(m_pParentWnd->GetSafeHwnd(),MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor,&info);

	long top = info.rcMonitor.top;
	long left = info.rcMonitor.left;
	long bottom = info.rcMonitor.bottom;
	long right = info.rcMonitor.right;

	GetWindowPlacement(&m_struOldWndpl);

	CRect rectFullScreen;
	rectFullScreen.left = left;
	rectFullScreen.top    = top;
	rectFullScreen.right  = right;
	rectFullScreen.bottom = bottom;

	//enter into full screen;
	WINDOWPLACEMENT struWndpl;
	struWndpl.length = sizeof(WINDOWPLACEMENT);
	struWndpl.flags   = 0;
	struWndpl.showCmd = SW_SHOWNORMAL;
	struWndpl.rcNormalPosition = rectFullScreen;
	SetWindowPlacement(&struWndpl);
	// 置顶窗口
	SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	m_bIsFullScreen = TRUE;

	// 更新播放窗口大小
	if(!m_ulByMediaPass)
	{
		if(m_pParentWnd)
		{
			CeLTE_PlayerCtrl* pCtrl = (CeLTE_PlayerCtrl*)(((CVideoPane*)m_pParentWnd)->GetCeLTE_PlayerCtrl());//lint !e1774
			if(pCtrl)
			{
				ELTE_INT32 iResId = pCtrl->GetMediaPlayer().GetMediaPlayerResID();
				if(eLTE_ERR_SUCCESS != ELTE_SDK_SetPlayWindowSize(eLTE_Tool::Int2String(iResId).c_str(), (ELTE_ULONG)(right - left), (ELTE_ULONG)(bottom - top)))
				{
					LOG_RUN_ERROR("When set full screen, call ELTE_SDK_SetPlayWindowSize failed, the res id is %d.", iResId);
				}
			}
		}
	}

	// 键盘钩子
	if (NULL == CeLTE_PlayerCtrl::s_KeyboardHook)
	{
		CeLTE_PlayerCtrl::s_KeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, CeLTE_PlayerCtrl::KeyboardProc, AfxGetInstanceHandle(), 0);
	}
	// 鼠标钩子
	if (NULL == CeLTE_PlayerCtrl::s_MouseHook)
	{
		CeLTE_PlayerCtrl::s_MouseHook = ::SetWindowsHookEx(WH_MOUSE_LL, CeLTE_PlayerCtrl::MouseProc, AfxGetInstanceHandle(), 0);
	}

	return eLTE_ERR_SUCCESS;
}

eLTE_RET CVideoStatic::ResetFullScreen()
{
	LOG_TRACE();

	if (!m_bIsFullScreen)
	{
		LOG_RUN_INFO("Is already not full-screen.");
		return eLTE_ERR_SUCCESS;
	}

	if (NULL == m_pParentWnd || NULL == m_pParentWnd->GetSafeHwnd())
	{
		LOG_RUN_ERROR("m_pParentWnd is null or parentwnd had been destory.");
		return eLTE_ERR_FAILED;
	}
	m_pParentWnd->ShowWindow(SW_HIDE);
	// 恢复父窗口指针，并显示
	ModifyStyle(WS_POPUP, WS_CHILD, 0);
	SetParent(m_pParentWnd);
	m_pParentWnd->Invalidate(TRUE);

	m_pParentWnd->ShowWindow(SW_SHOW);

	m_pParentWnd->SetForegroundWindow();
	SetForegroundWindow();

	m_pParentWnd->SetFocus();
	SetWindowPlacement(&m_struOldWndpl);

	m_bIsFullScreen = FALSE;
	// 更新播放窗口大小
	if(!m_ulByMediaPass)
	{
		if(m_pParentWnd)
		{
			RECT wnd;
			memset(&wnd, 0x0, sizeof(wnd));
			if(m_pParentWnd)
			{
				((CVideoPane*)m_pParentWnd)->GetVideoStatic().GetWindowRect(&wnd);//lint !e1774
				CeLTE_PlayerCtrl* pCtrl = (CeLTE_PlayerCtrl*)(((CVideoPane*)m_pParentWnd)->GetCeLTE_PlayerCtrl());//lint !e1774
				if(pCtrl)
				{
					ELTE_INT32 iResId = pCtrl->GetMediaPlayer().GetMediaPlayerResID();
					if(eLTE_ERR_SUCCESS != ELTE_SDK_SetPlayWindowSize(eLTE_Tool::Int2String(iResId).c_str(), (ELTE_ULONG)(wnd.right - wnd.left), (ELTE_ULONG)(wnd.bottom - wnd.top)))
					{
						LOG_RUN_ERROR("When reset full screen, call ELTE_SDK_SetPlayWindowSize failed, the res id is %d.", iResId);
					}
				}
			}
		}
	}
	// 键盘钩子
	if (NULL != CeLTE_PlayerCtrl::s_KeyboardHook)
	{
		::UnhookWindowsHookEx(CeLTE_PlayerCtrl::s_KeyboardHook);
		CeLTE_PlayerCtrl::s_KeyboardHook = NULL;
	}
	// 鼠标钩子
	if (NULL != CeLTE_PlayerCtrl::s_MouseHook)
	{
		::UnhookWindowsHookEx(CeLTE_PlayerCtrl::s_MouseHook);
		CeLTE_PlayerCtrl::s_MouseHook = NULL;
	}

	ShowCursor(TRUE);
	SetCursorPos(m_OldCursorPoint.x, m_OldCursorPoint.y);

	return eLTE_ERR_SUCCESS;
}


BOOL CVideoStatic::IsFullScreen() const
{
	return m_bIsFullScreen;
}


void CVideoStatic::SetBackGroudImage(const CPaintDC& dc)
{
	// 加载背景logo图片
	if (NULL == m_pImage)
	{
		m_pImage = Gdiplus::Image::FromFile(m_strLogoPath);
		if (NULL == m_pImage)
		{
			LOG_RUN_ERROR("m_pImage is null.");
			return;
		}
		else
		{
			if (Gdiplus::Ok != m_pImage->GetLastStatus())
			{
				std::string strPath = eLTE_Tool::UnicodeToANSI(m_strLogoPath.GetString());
				LOG_RUN_ERROR("Gdiplus::Image::FromFile failed. (%s)", strPath.c_str());
				m_pImage = NULL;
				return;
			}
		}
	}

	// 居中位置计算
	CRect rect;
	GetClientRect(&rect);
	int iWidth = (int)m_pImage->GetWidth();
	int iHeight = (int)m_pImage->GetHeight();
	if ((double)rect.right/rect.bottom > (double)iWidth/iHeight)
	{
		iHeight = rect.bottom / 3;
		iWidth = iHeight * (int)m_pImage->GetWidth() / (int)m_pImage->GetHeight();
	}
	else
	{
		iWidth = rect.right / 3;
		iHeight = iWidth * (int)m_pImage->GetHeight() / (int)m_pImage->GetWidth();
	}

	int iLeft = (rect.right - iWidth) / 2;
	int iTop = (rect.bottom - iHeight) / 2;

	// 绘制图片
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Rect gRect(iLeft, iTop, iWidth, iHeight);
	Gdiplus::ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(Gdiplus::WrapModeTileFlipXY);
	graphics.DrawImage(m_pImage, gRect, 0, 0, (INT)m_pImage->GetWidth(), (INT)m_pImage->GetHeight(), Gdiplus::UnitPixel, &ImgAtt);
}


// 设置父窗体
void CVideoStatic::SetParentWnd(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}
