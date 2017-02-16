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

#include "stdafx.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "XMLProcess.h"
#include "Logic_Queue.h"
#include "SSL_Socket.h"
#include "SessionMgr.h"
#include "GISReportMgr.h"



GISReportMgr::GISReportMgr(void)
{
}


GISReportMgr::~GISReportMgr(void)
{
}

void GISReportMgr::notifyGISReport(GISReportIndicator* pGISReportIndicator)
{
	LOG_TRACE();

	if (NULL == pGISReportIndicator)
	{
		LOG_RUN_ERROR("pGISReportIndicator is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyGISReport(pGISReportIndicator, xmlStr);
	if(pGISReportIndicator)
	{
		delete pGISReportIndicator;
		pGISReportIndicator = NULL;
		if(pGISReportIndicator==NULL){}
	}
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyGISReport failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_GISREPORT_REQ, xmlStr, iRet)
}

void GISReportMgr::notifyGISSubRsp(GISReportIndicator* pGISReportIndicator)
{
	LOG_TRACE();

	if (NULL == pGISReportIndicator)
	{
		LOG_RUN_ERROR("pGISReportIndicator is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyGISStatus(pGISReportIndicator, xmlStr);
	if(pGISReportIndicator)
	{
		delete pGISReportIndicator;
		pGISReportIndicator = NULL;
		if(pGISReportIndicator==NULL){}
	}
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyGISStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_GISSTATUS_REQ, xmlStr, iRet);
}
