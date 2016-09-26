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

#pragma once

#include "resource.h"

// CMediaPlayerDlg dialog

class CMediaPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMediaPlayerDlg)

public:
	CMediaPlayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMediaPlayerDlg();

// Dialog Data
	enum { IDD = IDD_MEDIAPLAYERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetResId(const std::string& strResId){m_strResId = strResId;};
	void CreateVideoStatic(const CRect& rect);
	HWND GetVideoStaticHwnd(){return m_videoStatic.GetSafeHwnd();};

public:
	afx_msg void OnClose();

private:
	std::string m_strResId;
	CStatic m_videoStatic;
	CBrush m_brush;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
