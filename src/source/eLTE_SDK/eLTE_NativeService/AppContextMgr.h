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
filename    :    AppContextMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    ����̨��ʼ�����˳�����½���ǳ��߼�ʵ���ࡣ��Ӧ��eLTE SDK�ڵ�ApplicationContext��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_OCX_APPCONTEXT_MGR_H__
#define __ELTE_OCX_APPCONTEXT_MGR_H__

#include "DisplayMgr.h"
#include "MediaMgr.h"
#include "GISReportMgr.h"


class AppContextMgr
{
public:
	static AppContextMgr& Instance();

private:
	AppContextMgr(void);
	~AppContextMgr(void);

public:
	// ��ʼ��SDK
	int Init();
	// �˳�SDK
	int Exit() const;

	void SetBypass(unsigned int uiBypass) {m_uiBypass = uiBypass;}

private:
	// ������������
	bool ChangeEnvironmentVariable() const;

private:
	// ע���DisplayManager�ص���ʵ��
	DisplayMgr*     m_pDisplayMgr;

	MediaMgr*       m_pMediaMgr;
	unsigned int    m_uiBypass;

	GISReportMgr* m_pGISReportMgr;
};

#endif
