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
#include "afxwin.h"
#include "elte_playerctrl1.h"
//typedef std::vector<CString> VideoChannelList;
// CVWall dialog
class CDConsoleDlg;

class CVWall : public CDialogEx
{
	DECLARE_DYNAMIC(CVWall)

public:
	CVWall(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVWall();

// Dialog Data
	enum { IDD = IDD_DIALOG_VWALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboBox;
	virtual BOOL OnInitDialog();
	void setVideoChannelList(VideoChannelList* StringList){this->StringList = StringList;}
	VideoChannelList* getVideoChannelListPointer(){return StringList;}
	CString getVideoChannel(){return videoChannel;}
	void setDcDlg(CDConsoleDlg* pDcDlg);
	void SetELtePlayer(CElte_playerctrl1* peLTE_Player);
	void SetResourceId(CString resourceId){strResourceId = resourceId;}
private:
	VideoChannelList *StringList ;
	CString videoChannel;
	CDConsoleDlg* pDcDlg;
	CElte_playerctrl1* m_peLTE_Player;
	CString strResourceId;

public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};
