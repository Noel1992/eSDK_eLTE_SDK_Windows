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
filename    :    CreateGroupDlg.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    ������̬����ʱ��Ի��򣬸��𴴽���ʱ�Ĳ�������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 ��ʼ�汾
*********************************************************************/

#pragma once


// CCreateGroupDlg dialog

class CCreateGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateGroupDlg)

public:
	CCreateGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateGroupDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CREATEGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// �Ի����ʼ��
	virtual BOOL OnInitDialog();
	// ��Ⱥ���б������һ��Ⱥ��
	afx_msg void OnBnClickedButtonAddgroup();
	// ��Ⱥ���б���ɾ��һ��Ⱥ��
	afx_msg void OnBnClickedButtonDelgroup();
	// ���û��б������һ���û�
	afx_msg void OnBnClickedButtonAdduser();
	// ���û��б���ɾ��һ���û�
	afx_msg void OnBnClickedButtonDeluser();
	// ����OK��ť����ɴ�����ʱ��������
	afx_msg void OnBnClickedOk();
	// ����Cancel������ȡ����������
	afx_msg void OnBnClickedCancel();

public:
	// ������������
	void SetDGNAParamInfo(CString strDcId, DGNAParamInfo* pInfo, bool bDynamic);

private:
	DGNAParamInfo* m_pDGNAParamInfo;	// ��������
	CString m_strDcId;					// DcIDֵ
	bool m_bDynamic;					// ������̬�黹����ʱ���־
};
