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

#include "DataType.h"
#include "DisplayMgr.h"
#include "MediaMgr.h"
#include "GisReportMgr.h"

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
	// ����eLTE_PlayerCtrl���
	void SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl);
	// ��ȡע���ý�������ʵ��
	MediaMgr* GetMediaMgr() {return m_pMediaMgr;};
private:
	// ע���DisplayManager�ص���ʵ��
	DisplayMgr* m_pDisplayMgr;

	// ע���ý�������ʵ��
	MediaMgr* m_pMediaMgr;

	// ע���GisReportManager�ص���ʵ��
	GisReportMgr* m_pGisReportMgr;
};

#endif
