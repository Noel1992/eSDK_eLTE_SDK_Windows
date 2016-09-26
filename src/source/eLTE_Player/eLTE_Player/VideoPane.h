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
filename    :    VideoPane.h
author      :    yubinbin wx200189
created     :    2014/07/30
description :    控件视频播放窗体。是视频回传的播放窗体
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/30 初始版本
*********************************************************************/

#pragma once

#include "eLTE_Player.h"
#include "VideoStatic.h"
#include "ImageButton.h"
#include "Language.h"
// CVideoPane dialog

#define VIDEO_TOOLBAR_WIDTH		30	// 工具条宽度
#define VIDEO_IMG_BTN_WIDTH		25	// 图片按钮宽度
#define VIDEO_IMG_BTN_SPACE		10	// 图片按钮间隔
#define VIDEO_RESIZING_WIDTH	5	// 调整边框宽度

#define BTN_CLOSE_WIDTH			25	// 关闭按钮宽度
#define BTN_CLOSE_HEIGHT		25	// 关闭按钮高度
#define BTN_CLOSE_SPACE			10	// 关闭按钮间距

#define CAPTION_HEIGHT			30	// 标题栏高度

//标题栏文本定义
#define CAPTION_FONT_SIZE		20					// 字体大小
#define CAPTION_FONT_COLOR		RGB(84, 84, 84)		// 字体颜色
#define CAPTION_FONT_BKCLR		TRANSPARENT			// 字体背景颜色
#define CAPTION_FONT_NAME		_T("微软雅黑")		// 字体字样

// 图片名字定义
#define IMG_RESOURCE_PATH		_T("Skin\\ToolBar\\")		// 图片资源目录
#define IMG_CAPTION_PATH		_T("Skin\\Caption\\")		// 标题栏资源目录

#define IMG_CAPTION				_T("caption.png")			// 标题栏背景图片，获取焦点
#define IMG_CAPTION_FOCUS		_T("caption_focus.png")		// 标题栏背景图片，获取焦点
#define IMG_CAPTION_DEFOCUS		_T("caption_defocus.png")	// 标题栏背景图片，无焦点

#define IMG_CLOSE_NORMAL		_T("close_normal.png")		// 关闭按钮，正常
#define IMG_CLOSE_SELECT		_T("close_select.png")		// 关闭按钮，选中
#define IMG_CLOSE_FOCUS			_T("close_focus.png")		// 关闭按钮，获取焦点
#define IMG_CLOSE_DISABLE		_T("close_disable.png")		// 关闭按钮，禁止使用

#define IMG_SNAPSHOT_NORMAL		_T("snapshot_normal.png")	// 抓图，正常
#define IMG_SNAPSHOT_SELECT		_T("snapshot_select.png")	// 抓图，选中
#define IMG_SNAPSHOT_FOCUS		_T("snapshot_focus.png")	// 抓图，获取焦点
#define IMG_SNAPSHOT_DISABLE	_T("snapshot_disable.png")	// 抓图，禁止使用

#define IMG_REVERSEPLAY_NORMAL	_T("reverseplay_normal.png")	// 切换，正常
#define IMG_REVERSEPLAY_SELECT	_T("reverseplay_select.png")	// 切换，选中
#define IMG_REVERSEPLAY_FOCUS	_T("reverseplay_focus.png")	// 切换，获取焦点
#define IMG_REVERSEPLAY_DISABLE	_T("reverseplay_disable.png")// 切换，禁止使用

#define IMG_UNMUTE_NORMAL		_T("unmute_normal.png")		// 伴音，正常
#define IMG_UNMUTE_SELECT		_T("unmute_select.png")		// 伴音，选中
#define IMG_UNMUTE_FOCUS		_T("unmute_focus.png")		// 伴音，获取焦点
#define IMG_UNMUTE_DISABLE		_T("unmute_disable.png")	// 伴音，禁止使用

#define IMG_MUTE_NORMAL			_T("mute_normal.png")		// 静音，正常
#define IMG_MUTE_SELECT			_T("mute_select.png")		// 静音，选中
#define IMG_MUTE_FOCUS			_T("mute_focus.png")		// 静音，获取焦点
#define IMG_MUTE_DISABLE		_T("mute_disable.png")		// 静音，禁止使用

#define IMG_FULLSCREEN_NORMAL	_T("fullscreen_normal.png")	// 全屏，正常
#define IMG_FULLSCREEN_SELECT	_T("fullscreen_select.png")	// 全屏，选中
#define IMG_FULLSCREEN_FOCUS	_T("fullscreen_focus.png")	// 全屏，获取焦点
#define IMG_FULLSCREEN_DISABLE	_T("fullscreen_disable.png")// 全屏，禁止使用


class CVideoPane : public CDialog
{
	DECLARE_DYNAMIC(CVideoPane)

public:
	CVideoPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoPane();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIDEOPANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnMsgImgBtnClicked(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);

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
	// 获取播放窗体句柄
	HWND GetVideoStaticSafeHwnd() const;
	// 激活关闭按钮
	void EnableImageButton(BOOL bEnable);
	// 激活关闭伴音
	void EnableAudio(BOOL bEnable);
	// 设置伴音
	void PlayAudio(BOOL bPlay);
	// 显示工具栏
	void ShowToolBar(BOOL bShow);
	// 设置语言
	void SetLanguage(int ilang);
	// 设置标题栏文本
	void SetCaptionText(const CString& strText);
	// 设置OCX控件指针
	void SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl);
	// 获取OCX控件指针
	COleControl* GetCeLTE_PlayerCtrl(){return m_pCeLTE_PlayerCtrl;};
	// 获取播放窗口实例
	const CVideoStatic& GetVideoStatic() const {return m_VideoStatic;};
	// 获取语音按钮状态
	BOOL GetAudioBtnState() const;

private:
	void CreateCaption(const CRect& rect, const CString& strOcxPath);
	void CreateToolBar(const CRect& rect, const CString& strOcxPath);
	void CreateVideoStatic(const CRect& rect, const CString& strOcxPath);

private:
	CVideoStatic m_VideoStatic;
	Language m_Language;

	CImageButton m_SnapshotBtn;
	CImageButton m_ReversePlayBtn;
	CImageButton m_AudioBtn;
	CImageButton m_FullScreenBtn;
	CImageButton m_CloseBtn;

	Gdiplus::Image* m_pImgCaption;
	CString m_strCaptionText;

	BOOL m_bEnableAudio;
	BOOL m_bHaveSound;
	BOOL m_bShowToolBar;

private:
	COleControl* m_pCeLTE_PlayerCtrl;
};
