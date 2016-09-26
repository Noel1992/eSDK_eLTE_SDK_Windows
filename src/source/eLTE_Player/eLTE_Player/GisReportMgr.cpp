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
#include "GisReportMgr.h"
#include "eLTE_Log.h"
#include "mq\gisreportindicator.h"

GisReportMgr::GisReportMgr(void)
	: m_pCeLTE_PlayerCtrl(NULL)
{
}


GisReportMgr::~GisReportMgr(void)
{
	m_pCeLTE_PlayerCtrl = NULL;
}

void GisReportMgr::notifyGISReport(GISReportIndicator* pGISReportIndicator)
{
	LOG_TRACE();

	if (NULL == pGISReportIndicator)
	{
		LOG_RUN_ERROR("pGISReportIndicator is null.");
		return;
	}

	// 发送事件消息
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		GisReportInfo* pGisReportInfo = new GisReportInfo;
		if (NULL == pGisReportInfo)//lint !e774
		{
			LOG_RUN_ERROR("pGisReportInfo is null.");
			return;
		}
		pGisReportInfo->strSender = pGISReportIndicator->getSender();
		pGisReportInfo->strTime = pGISReportIndicator->getTime();
		pGisReportInfo->strAltitude = pGISReportIndicator->getAltitude();
		pGisReportInfo->strLatitude = pGISReportIndicator->getLatitude();
		pGisReportInfo->strLongtitude = pGISReportIndicator->getLongitude();
		pGisReportInfo->iTriggerCode = pGISReportIndicator->getReasonCode();
		pGisReportInfo->iReportState = pGISReportIndicator->getRptState();

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_GIS_REPORT, (LPARAM)pGisReportInfo);
	}
}

void GisReportMgr::notifyGISSubRsp(GISReportIndicator* pGISReportIndicator)
{
	LOG_TRACE();

	if (NULL == pGISReportIndicator)
	{
		LOG_RUN_ERROR("pGISReportIndicator is null.");
		return;
	}

	// 发送事件消息
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		GisReportInfo* pGisReportInfo = new GisReportInfo;
		if (NULL == pGisReportInfo)//lint !e774
		{
			LOG_RUN_ERROR("pGisReportInfo is null.");
			return;
		}
		pGisReportInfo->strSender = pGISReportIndicator->getSender();
		pGisReportInfo->ackStateMap.insert(pGISReportIndicator->getAckRsp().begin(), pGISReportIndicator->getAckRsp().end());

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_GIS_STATUS, (LPARAM)pGisReportInfo);
	}
}

// 设置eLTE_PlayerCtrl句柄
void GisReportMgr::SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl)
{
	m_pCeLTE_PlayerCtrl = pCeLTE_PlayerCtrl;
}
