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
filename		:    GISReportMgr.h
author			:    zhangrui z00233055
created		:    2015/06/23
description	:    eLTE SDKҵ����Ϣ�ص��࣬ͨ��ע�����������GIS״̬�¼�
copyright		:    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history			:    2015/06/23 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_SDK_GIS_REPORT_MGR_H__
#define __ELTE_SDK_GIS_REPORT_MGR_H__

#include "gisreportmanager.h"

class GISReportMgr : public GISReportManager
{
public:
	GISReportMgr(void);
	virtual ~GISReportMgr(void);//lint !e1510

	//GIS�ϱ�����
	virtual void notifyGISReport(GISReportIndicator* pGISReportIndicator);

	//GIS����֪ͨ
	virtual void notifyGISSubRsp(GISReportIndicator* pGISReportIndicator);
};

#endif
