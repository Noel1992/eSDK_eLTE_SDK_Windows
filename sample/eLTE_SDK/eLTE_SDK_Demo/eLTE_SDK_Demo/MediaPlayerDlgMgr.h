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
filename    :    MediaPlayerDlgMgr.h
author      :    zhangziqian zWX229156
created     :    2015/07/01
description :    Media player window management class
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/07/01 Initial version
*********************************************************************/

#include "MediaPlayerDlg.h"
#include <string.h>
#include <map>

#pragma once

// CMediaPlayerDlgMgr

class CMediaPlayerDlgMgr
{
public:
	static CMediaPlayerDlgMgr& Instance();
	virtual ~CMediaPlayerDlgMgr(void);

	CMediaPlayerDlg* CreateMediaPlayerDlg(const std::string& strResId, CWnd* pParentWnd);
	void DeleteCreateMediaPlayerDlg(const std::string& strResId);
	CMediaPlayerDlg* GetMediaPlayerDlg(const std::string& strResId);

private:
	CMediaPlayerDlgMgr(void);

private:
	std::map<std::string, CMediaPlayerDlg*> m_mediaPlayerDlgs;
};

