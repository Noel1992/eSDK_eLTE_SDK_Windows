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
#include "DisplayMgr.h"
#include "AppContextMgr.h"
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "XMLProcess.h"
#include "Logic_Queue.h"
#include "SSL_Socket.h"
#include "SessionMgr.h"
#include "mq/p2pvideocallstatus_indicator.h"
#include "mq/sds_message_indicator.h"
#include "mq/provchange_indicator.h"
#include "mq/grpcallstatus_indicator.h"
#include "mq/p2pcallstatus_indicator.h"
#include "mq/modulestatus_indicator.h"
#include "sdk_helper.h"


DisplayMgr::DisplayMgr(void)
{

}


DisplayMgr::~DisplayMgr(void)
{

}

//通知组呼的状态变更信息
void DisplayMgr::notifyGroupStatus(GrpDspInfo* pGrpInfo)
{
	LOG_TRACE();

	if (NULL == pGrpInfo)
	{
		LOG_RUN_ERROR("pGrpInfo is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyGroupStatus(pGrpInfo, xmlStr);
	FREE_POINTER(pGrpInfo);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyGroupStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_GROUPCALLSTATUS_REQ, xmlStr, iRet);
}

//通知用户和群组资源的状态变更信息
void DisplayMgr::notifyResourceStatus(ResourceStatusIndicator* pResourceStatus)
{
	LOG_TRACE();

	if (NULL == pResourceStatus)
	{
		LOG_RUN_ERROR("pResourceStatus is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyResourceStatus(pResourceStatus, xmlStr);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyResourceStatus failed.");
		FREE_POINTER(pResourceStatus);
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_RESOURCESTATUS_REQ, xmlStr, iRet);

	// 登陆成功后，启动SDK处理业务
	if ((RESREGSTATUS_PROXY == pResourceStatus->getStatusType() || RESREGSTATUS_DIRECT == pResourceStatus->getStatusType())
		&& STATUS_REGOK == pResourceStatus->getStatusValue())
	{
		// 启动SDK处理业务
		(void)OperationMgr::Instance().StartRunning();
		//登录成功后清除登录失败信息
		SDK_Helper::clean_invalid_reg();
	}
	FREE_POINTER(pResourceStatus);
}

//通知点对点呼叫的状态变更信息
void DisplayMgr::notifyP2pcallStatus(P2pcallStatusIndicator* pCallStatus)
{
	LOG_TRACE();

	if (NULL == pCallStatus)
	{
		LOG_RUN_ERROR("pCallStatus is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyP2pcallStatus(pCallStatus, xmlStr);
	FREE_POINTER(pCallStatus);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyP2pcallStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_P2PCALLSTATUS_REQ, xmlStr, iRet);
}

//通知视频呼叫的状态变更信息（包括视频回传），需要YC实现
void DisplayMgr::notifyP2pvideocallStatus(P2pvideocallStatusIndicator* pCallStatus)
{
	LOG_TRACE();

	if (NULL == pCallStatus)
	{
		LOG_RUN_ERROR("pCallStatus is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyP2pvideocallStatus(pCallStatus, xmlStr);
	FREE_POINTER(pCallStatus);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyP2pvideocallStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ, xmlStr, iRet);
}

//通知用户和群组资源的状态变更信息,需要YC实现
void DisplayMgr::notifyUserStatus(ResourceStatusIndicator* pResourceStatus)
{
	LOG_TRACE();

	if (NULL == pResourceStatus)
	{
		LOG_RUN_ERROR("pResourceStatus is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyUserStatus(pResourceStatus, xmlStr);
	FREE_POINTER(pResourceStatus);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyUserStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_USERSTATUS_REQ, xmlStr, iRet);
}

//通知用户状态短消息的服务接收实现
void DisplayMgr::notifyUserSdsStatus(SdsMessageIndicator* pSdsMessage)
{
	if (NULL == pSdsMessage)
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
		return;
	}
	// 获取xml字符串
	std::string xmlStr;
	// 筛选消息
	bool bSdsDirection = pSdsMessage->getSdsDirection();
	if (!bSdsDirection)
	{
		int iRet = XMLProcess::SetXml_NotifyUserSdsStatusReport(pSdsMessage, xmlStr);
		FREE_POINTER(pSdsMessage);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSReport failed.");
			return;
		}

		// 设置逻辑队列数据并放入逻辑队列
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSREPORT_REQ, xmlStr, iRet);
	}
	else
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
	}
}

//模块组件状态事件
void DisplayMgr::notifyModuleStatus(DCModuleStatusIndicator* pModuleStatus)
{
	if (NULL == pModuleStatus)
	{
		LOG_RUN_ERROR("pModuleStatus is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyModuleStatus(pModuleStatus, xmlStr);
	FREE_POINTER(pModuleStatus);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyModuleStatus failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_MODULESTATUS_REQ, xmlStr, iRet);
}

//短数据信息上报给GUI
void DisplayMgr::notifySMS(SdsMessageIndicator* pSdsMessage)
{
	if (NULL == pSdsMessage)
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
		return;
	}
	// 获取xml字符串
	std::string xmlStr;
	// 筛选消息
	bool bSdsDirection = pSdsMessage->getSdsDirection();
	if (!bSdsDirection)
	{
		int iRet = XMLProcess::SetXml_NotifySDSReport(pSdsMessage, xmlStr);
		FREE_POINTER(pSdsMessage);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSReport failed.");
			return;
		}

		// 设置逻辑队列数据并放入逻辑队列
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSREPORT_REQ, xmlStr, iRet);
	}
	else
	{
		int iRet = XMLProcess::SetXml_NotifySDSSendStatus(pSdsMessage, xmlStr);
		FREE_POINTER(pSdsMessage);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSSendStatus failed.");
			return;
		}

		// 设置逻辑队列数据并放入逻辑队列
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSSENDSTATUS_REQ, xmlStr, iRet);
	}
}

//通知配置对象的变更信息， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
void DisplayMgr::notifyProvisionChange(ProvChangeIndicator* pProvChange)
{
	LOG_TRACE();

	if (NULL == pProvChange)
	{
		LOG_RUN_ERROR("pProvChange is null.");
		return;
	}

	// 获取xml字符串
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyProvisionChange(pProvChange, xmlStr);
	FREE_POINTER(pProvChange);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_WARN("SetXml_NotifyProvisionChange failed.");
		return;
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_PROVISIONCHANGE_REQ, xmlStr, iRet);
}

//通知配置对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
void DisplayMgr::notifyProvisionAllResync(ProvChangeIndicator* pProvChangeIndicator)
{
	LOG_TRACE();

	int iRet = eLTE_SVC_ERR_SUCCESS;
	std::string xmlStr("");

	if(NULL != pProvChangeIndicator)
	{
		FREE_POINTER(pProvChangeIndicator);
	}

	// 设置逻辑队列数据并放入逻辑队列
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_PROVISIONALLRESYNC_REQ, xmlStr, iRet);
}

//通知状态对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
void DisplayMgr::notifyStatusAllResync(ResourceStatusIndicator* pResourceStatusIndicator)
{
	if(NULL != pResourceStatusIndicator)
	{
		FREE_POINTER(pResourceStatusIndicator);
	}
}
