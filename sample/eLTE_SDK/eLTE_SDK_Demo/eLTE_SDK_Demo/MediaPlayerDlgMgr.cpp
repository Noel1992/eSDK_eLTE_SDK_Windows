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

#include "StdAfx.h"
#include "MediaPlayerDlgMgr.h"
#include "eLTE_Tool.h"


CMediaPlayerDlgMgr::CMediaPlayerDlgMgr(void)
{
}


CMediaPlayerDlgMgr::~CMediaPlayerDlgMgr(void)
{
}


CMediaPlayerDlgMgr& CMediaPlayerDlgMgr::Instance()
{
	static CMediaPlayerDlgMgr _instance; 
	return _instance; 
}

CMediaPlayerDlg* CMediaPlayerDlgMgr::CreateMediaPlayerDlg(const std::string& strResId, CWnd* pParentWnd)
{
	std::map <std::string, CMediaPlayerDlg*>::iterator iter = m_mediaPlayerDlgs.find(strResId);
	if(m_mediaPlayerDlgs.end() != iter)
	{
		return iter->second;
	}
	CMediaPlayerDlg* pMediaPlayDlg = new CMediaPlayerDlg;

	if (NULL == pMediaPlayDlg)
	{
		return NULL;
	}
	pMediaPlayDlg->SetResId(strResId);

	pMediaPlayDlg->Create(IDD_MEDIAPLAYERDLG, NULL);

	CRect rect;
	pMediaPlayDlg->GetClientRect(rect);
	pMediaPlayDlg->CreateVideoStatic(rect);

	pMediaPlayDlg->SetWindowText(eLTE_Tool::ANSIToUnicode(strResId));
	pMediaPlayDlg->ShowWindow(SW_SHOWNA);
	(void)m_mediaPlayerDlgs.insert(std::pair <std::string, CMediaPlayerDlg*> (strResId, pMediaPlayDlg));

	return pMediaPlayDlg;
}

CMediaPlayerDlg* CMediaPlayerDlgMgr::GetMediaPlayerDlg(const std::string& strResId)
{
	std::map <std::string, CMediaPlayerDlg*>::iterator iter = m_mediaPlayerDlgs.find(strResId);
	if(m_mediaPlayerDlgs.end() != iter)
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

void CMediaPlayerDlgMgr::DeleteCreateMediaPlayerDlg(const std::string& strResId)
{
	
	std::map <std::string, CMediaPlayerDlg*>::iterator iter = m_mediaPlayerDlgs.find(strResId);
	if(iter == m_mediaPlayerDlgs.end())
	{
		return;
	}

	if(iter->second->GetSafeHwnd())
	{
		iter->second->DestroyWindow();
	}

	m_mediaPlayerDlgs.erase(iter);
}