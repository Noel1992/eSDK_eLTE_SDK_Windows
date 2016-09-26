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
filename    :    GisReportMgr.h
author      :    zhangziqian zWX229156
created     :    2015/05/14
description :    终端的GIS数据上报以及上报信息的定义
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/05/14 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_GIS_REPORT_MGR_H__
#define __ELTE_OCX_GIS_REPORT_MGR_H__

#include "gisreportmanager.h"

class GisReportMgr : public GISReportManager
{
public:
	GisReportMgr(void);
	virtual ~GisReportMgr(void);//lint !e1510

	//GIS上报数据
	virtual void notifyGISReport(GISReportIndicator *);

	//GIS订阅通知
	virtual void notifyGISSubRsp(GISReportIndicator * );

public:
	// 设置eLTE_PlayerCtrl句柄
	void SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl);

private:
	COleControl* m_pCeLTE_PlayerCtrl;
};

#endif

