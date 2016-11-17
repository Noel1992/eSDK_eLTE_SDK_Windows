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

// VWall.cpp : implementation file
//

#include "stdafx.h"
#include "eLTE_PlayerDemo.h"
#include "VWall.h"
#include "afxdialogex.h"
#include "DConsoleDlg.h"
#include "Xml.h"


// CVWall dialog

IMPLEMENT_DYNAMIC(CVWall, CDialogEx)

CVWall::CVWall(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVWall::IDD, pParent),StringList(NULL)
{
	
}

CVWall::~CVWall()
{
	//delete StringList;
	//StringList = nullptr;
}

void CVWall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comboBox);
}


BEGIN_MESSAGE_MAP(CVWall, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CVWall::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_START, &CVWall::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CVWall::OnBnClickedButtonStop)
//	ON_BN_CLICKED(IDOK, &CVWall::OnBnClickedOk)
END_MESSAGE_MAP()


// CVWall message handlers


BOOL CVWall::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	if (StringList != NULL)
	{
		VideoChannelList::iterator it;
		for(it= (*StringList).begin();it!=(*StringList).end();it++)
		{
			comboBox.AddString(it->first);
		}
		//comboBox.SetCurSel(0);
	}

//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CVWall::OnCbnSelchangeCombo1()
{
	
	int iSel;
	iSel = comboBox.GetCurSel();
	comboBox.GetLBText(iSel,videoChannel);

	CString strResult;

	//Refresh list and status
	strResult = m_peLTE_Player->ELTE_OCX_GetDcVWallIDList();
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_GetDcVWallIDList"));
	VideoChannelList* pStringList = getVideoChannelListPointer();
	CXml::Instance().XmlParseVWallIDList(strResult, (*pStringList));

	CString StrIDState = (*pStringList)[videoChannel];	

	if ("1" == StrIDState || "4023" == StrIDState)
	{
		GetDlgItem(IDC_STATIC_VIDSTATE)->SetWindowText(_T("Available"));
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}
	else if ("4022" == StrIDState)
	{
		GetDlgItem(IDC_STATIC_VIDSTATE)->SetWindowText(_T("Unavailable"));
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else
	{
		MessageBox(_T("VWall channel state error!"));
		return;
	}	

	//Recycling
	pStringList = NULL;	
	
}


void CVWall::OnBnClickedButtonStart()
{
	/************************************************************************/
	/* 
	--- XML format ---
	<Content>
	<VideoParam>
	<DstObjId ></DstObjId >//Video on the wall, for a video channel
	<StrFmt ></StrFmt >//reserved
	</VideoParam>
	</Content>
	*/
	/************************************************************************/

	//Judge the wall information
	VideoChannelList* pVCL = getVideoChannelListPointer();

	if ("1" != (*pVCL)[videoChannel] && "4023" != (*pVCL)[videoChannel])
	{
		MessageBox(_T("当前状态不可用"));
		return;
	}

	
	CString strVideoChannelStart;
	//construct Video wall XML
	// construct reference
	strVideoChannelStart.Append(_T("<Content>"));
	strVideoChannelStart.Append(_T("<VideoParam>"));
	strVideoChannelStart.Append(_T("<DstObjId>"));
	strVideoChannelStart.Append(videoChannel);
	strVideoChannelStart.Append(_T("</DstObjId>"));
	strVideoChannelStart.Append(_T("<StrFmt>"));
	strVideoChannelStart.Append(_T("</StrFmt>"));
	strVideoChannelStart.Append(_T("</VideoParam>"));
	strVideoChannelStart.Append(_T("</Content>"));
	CString strResult = m_peLTE_Player->ELTE_OCX_VWallStart(strResourceId,strVideoChannelStart);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopVideoDispatch"));
	GetDlgItem(IDC_STATIC_VIDSTATE)->SetWindowText(_T("Sending Message..."));
	Sleep(2000);
	OnCbnSelchangeCombo1();
}


void CVWall::OnBnClickedButtonStop()
{
	
	/************************************************************************
	--- XML format ---
	<Content>
	<DstObjId>Video Wall decoder ID</DstObjId>
	</Content>
	************************************************************************/
	VideoChannelList* pVCL = getVideoChannelListPointer();

	if ("4022" != (*pVCL)[videoChannel])
	{
		MessageBox(_T("Please wait a moment. The current decorder can not close."));
		return;
	}

	CString strVideoChannelStop;
	// construct reference
	strVideoChannelStop.Append(_T("<Content>"));
	strVideoChannelStop.Append(_T("<DstObjId>"));
	strVideoChannelStop.Append(videoChannel);
	strVideoChannelStop.Append(_T("</DstObjId>"));
	strVideoChannelStop.Append(_T("</Content>"));

	CString strResult = m_peLTE_Player->ELTE_OCX_VWallStop(strResourceId,strVideoChannelStop);
	CHECK_RESULTE_CODE(strResult, _T("ELTE_OCX_StopVideoDispatch"));
	GetDlgItem(IDC_STATIC_VIDSTATE)->SetWindowText(_T("Sending Message..."));
	Sleep(3000);
	OnCbnSelchangeCombo1();
}

void CVWall::setDcDlg(CDConsoleDlg* pdcDlg)
{ 
	pDcDlg = pdcDlg;
}

void CVWall::SetELtePlayer(CElte_playerctrl1* peLTE_Player)
{ 
	m_peLTE_Player = peLTE_Player;
}
