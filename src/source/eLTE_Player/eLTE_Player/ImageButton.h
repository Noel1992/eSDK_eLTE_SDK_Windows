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
filename    :    ImageButton.h
author      :    yubinbin wx200189
created     :    2014/08/05
description :    图片按钮控件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/08/05 初始版本
*********************************************************************/

#pragma once


// CImageButton

//////////////////////////////////////////////////////////////////////////
typedef enum
{
	ImgState_Normal = 0,
	ImgState_Select,
	ImgState_Focus,
	ImgState_Disable
} IMAGE_STATE_E;

#define IMGAGE_STATE_COUNT 4

//////////////////////////////////////////////////////////////////////////
class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)

public:
	CImageButton();
	virtual ~CImageButton();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClicked();
	afx_msg LRESULT OnMsgInvalidParent(WPARAM wParam, LPARAM lParam);

public:
	void LoadImages(const CString& strNormal, const CString& strSelect,
		const CString& strFocus, const CString& strDisable, IMAGE_STATE_E eCurImageState = ImgState_Normal);
	void SetCurImgState(IMAGE_STATE_E eCurImageState);
	void AddToolTip(LPCTSTR lpszText);
	void UpdateToolTip(LPCTSTR lpszText);

private:
	void UpdateImage(const CPaintDC& dc);
	void RelayToolTipEvent(const CPoint &point);

private:
	Gdiplus::Image *m_ImageList[IMGAGE_STATE_COUNT];
	IMAGE_STATE_E m_CurImageState;
	CToolTipCtrl m_ToolTipCtrl;
};


