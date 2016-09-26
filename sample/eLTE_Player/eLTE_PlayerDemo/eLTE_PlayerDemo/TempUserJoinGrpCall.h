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
#include "elte_playerctrl1.h"
#include "resource.h"


// CTempUserJoinGrpCall dialog

class CTempUserJoinGrpCall : public CDialogEx
{
	DECLARE_DYNAMIC(CTempUserJoinGrpCall)

public:
	CTempUserJoinGrpCall(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTempUserJoinGrpCall();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPUSERJOINGRPCALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	void SetParamInfo(const CString& strDcID, const CString& strSrcNum);
	void SetELtePlayer(CElte_playerctrl1* peLTE_Player)
	{ m_peLTE_Player = peLTE_Player;}
private:
	CString m_strDcID;
	CString m_strSrcNum;
	CElte_playerctrl1* m_peLTE_Player;
};
