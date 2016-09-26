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
description :    调度台初始化、退出、登陆、登出逻辑实现类。对应于eLTE SDK内的ApplicationContext类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
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
	// 初始化SDK
	int Init();
	// 退出SDK
	int Exit() const;

	void SetBypass(unsigned int uiBypass) {m_uiBypass = uiBypass;}

private:
	// 环境变量设置
	bool ChangeEnvironmentVariable() const;

private:
	// 注册的DisplayManager回调类实例
	DisplayMgr*     m_pDisplayMgr;

	MediaMgr*       m_pMediaMgr;
	unsigned int    m_uiBypass;

	GISReportMgr* m_pGISReportMgr;
};

#endif
