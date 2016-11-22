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

// SDSMessage.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "SDSMessage.h"
#include "afxdialogex.h"
// SDSMessage dialog

IMPLEMENT_DYNAMIC(SDSMessage, CDialogEx)

SDSMessage::SDSMessage(CWnd* pParent /*=NULL*/)
	: CDialogEx(SDSMessage::IDD, pParent)
	, m_filePath(_T(""))
	, m_receiver(_T(""))
	, m_message(_T(""))
	, m_pSDSParamInfo(NULL)
{

}

SDSMessage::~SDSMessage()
{
}

void SDSMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OPEN_EDIT, m_filePath);
	DDX_Text(pDX, IDC_RECEIVER_EDIT, m_receiver);
	DDX_Text(pDX, IDC_MESSAGE_EDIT, m_message);
}


BEGIN_MESSAGE_MAP(SDSMessage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &SDSMessage::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &SDSMessage::OnBnClickedOpenButton)
	ON_BN_CLICKED(IDCANCEL, &SDSMessage::OnBnClickedCancel)
END_MESSAGE_MAP()


// SDSMessage message handlers


void SDSMessage::OnBnClickedButtonSend()
{
	
	 UpdateData(TRUE);

	 //std::list<CString>::iterator it = m_pSDSParamInfo->files.begin();
	 //std::string strFilePath=m_filePath.GetBuffer(0);
	 //std::string strFilePath = (const char*)(m_filePath.GetBuffer());
	
	 //m_filePath.ReleaseBuffer();
	 //m_pSDSParamInfo->files.insert(it,m_filePath);
	 m_pSDSParamInfo->msgBody=m_message;
	 m_pSDSParamInfo->receiver=m_receiver;
	 if (m_filePath=="")
	 {
		 m_pSDSParamInfo->sdstype= "0001";
	 }
	 else
	 {
		 m_pSDSParamInfo->sdstype= "0004";
		 std::list<CString>::iterator it = m_pSDSParamInfo->files.begin();
		 m_pSDSParamInfo->files.insert(it,m_filePath);
	 }
	 CDialog::OnOK();
}


void SDSMessage::OnBnClickedOpenButton()
{
	
	CFileDialog fileDlg(TRUE);
	CString strFilePath;

	 if (IDOK == fileDlg.DoModal())
	 {
		 strFilePath = fileDlg.GetPathName();
		 SetDlgItemText(IDC_OPEN_EDIT, strFilePath);
	 }

}


void SDSMessage::SetSDSParamInfo(CString strDcId, SDSParamInfo* pInfo)
{
	m_pSDSParamInfo=pInfo;
}


void SDSMessage::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}
