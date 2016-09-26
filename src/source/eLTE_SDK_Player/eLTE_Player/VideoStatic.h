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

/********************************************************************
filename    :    VideoStatic.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    控件视频播放窗体。是视频回传的播放窗体
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#pragma once

#include "DataType.h"

typedef enum
{
	eLTE_SKIN_INVALID = 0,
	eLTE_SKIN_BLACK,
	eLTE_SKIN_WHITE
} eLTE_SKIN_TYPE_E;

#define BLACK_COLOR RGB(88, 88, 88)
#define WHITE_COLOR RGB(255,255,255)

// CVideoStatic

class CVideoStatic : public CStatic
{
	DECLARE_DYNAMIC(CVideoStatic)

public:
	CVideoStatic();
	virtual ~CVideoStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	// 设置皮肤参数
	void SetParam(int skinType, const CString& strLogoPath);
	// 设置图片路径
	void SetParam(const CString& strLogoPath);
	// 全屏
	eLTE_RET SetFullScreen();
	// 退出全屏
	eLTE_RET ResetFullScreen();
	// 是否全屏
	BOOL IsFullScreen() const;
	// 设置父窗体
	void SetParentWnd(CWnd* pParentWnd);

private:
	// 设置背景图片
	void SetBackGroudImage(const CPaintDC& dc);

private:
	// 全屏前的屏幕大小
	WINDOWPLACEMENT m_struOldWndpl;
	// 全屏标志
	BOOL m_bIsFullScreen;
	// 父窗口
	CWnd* m_pParentWnd;
	// 鼠标位置
	CPoint m_OldCursorPoint;

private:
	// 皮肤类型
	int m_skinType;
	// 背景图片路径
	CString m_strLogoPath;
	// 背景logo图片
	Gdiplus::Image* m_pImage;
};


