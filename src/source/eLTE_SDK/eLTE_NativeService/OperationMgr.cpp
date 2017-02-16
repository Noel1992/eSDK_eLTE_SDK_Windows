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
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"
#include "eLTE_Message.h"

#define GET_SDK_POINTER												\
	OperationManager* pOM = OperationManager_T::instance();			\
	if (NULL == pOM)												\
	{																\
		LOG_RUN_ERROR("OperationManager_T get instance failed.");	\
		return eLTE_SVC_ERR_NULL_POINTER;							\
	}																\


OperationMgr::OperationMgr(void)
	: m_userId("")
	, m_passwd("")
	, m_localIP("")
	, m_serverIP("")
	, m_sipPort("")
	, m_bLoginSuccess(FALSE)
{

}


OperationMgr::~OperationMgr(void)
{
	try
	{
		m_userId = "";
		m_passwd = "";
		m_localIP = "";
		m_serverIP = "";
		m_sipPort = "";
		m_bLoginSuccess = FALSE;
	}
	catch (...)
	{

	}
}


OperationMgr& OperationMgr::Instance()
{
	static OperationMgr s_OperationMgr;
	return s_OperationMgr;
}

// 调度台注册
int OperationMgr::Login(const std::string& userId, const std::string& passwd, 
	const std::string& serverIP, const std::string& localIP, const std::string& sipPort)
{
	LOG_TRACE();

	// 状态校验
	if (m_bLoginSuccess)
	{
		LOG_RUN_WARN("Already login.");
		return eLTE_SVC_ERR_SUCCESS;
	}

	// 获取SDK实例指针
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 设置调度台状态
	DcState* pDC = pAC->getConsoleState();
	if (NULL == pDC)
	{
		LOG_RUN_ERROR("getConsoleState failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	int iDcId = eLTE_Tool::String2Int(userId);
	pDC->setDCID(iDcId);
	pDC->setUser(userId.c_str());
	pDC->setPwd(passwd.c_str());
	pDC->setServerIP(serverIP.c_str());
	pDC->setLocalIP(localIP.c_str());
	pDC->setSipPort(sipPort.c_str());

	// 设置登录资源
	ResourceInfo* pResInfo = new ResourceInfo(iDcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	pResInfo->setUserName(userId.c_str());
	pResInfo->setPassWord(passwd.c_str());
	pResInfo->setServerIp(serverIP.c_str());
	pResInfo->setLocalIp(localIP.c_str());
	pResInfo->setSipPort(sipPort.c_str());

	// 调度台注册
	Op_Return_t iRet = pOM->invokeOp(pResInfo, DC_LOGIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		// 释放登陆资源
		delete pResInfo;
		LOG_RUN_ERROR("invokeOp DC_LOGIN failed. (%d)", iRet);
		return iRet;
	}

	// 释放登陆资源
	delete pResInfo;
	pResInfo = NULL;
	// 保存本地数据
	m_userId = userId;
	m_passwd = passwd;
	m_localIP = localIP;
	m_serverIP = serverIP;
	m_sipPort = sipPort;

	return eLTE_SVC_ERR_SUCCESS;//lint !e438
}

// 调度台注销
int OperationMgr::Logout(const std::string& userId)
{
	LOG_TRACE();
	INFO_PARAM1(userId);

	// 状态校验
	if (m_userId.empty())
	{
		return eLTE_SVC_ERR_SUCCESS;
	}

	// 入参校验
	if (m_userId != userId)
	{
		INFO_PARAM2(userId, m_userId);
		LOG_RUN_ERROR("Please login first.");
		return eLTE_SVC_ERR_NOT_LOGIN;
	}

	// 状态校验
	if (!m_bLoginSuccess)
	{
		LOG_RUN_ERROR("Please wait login result.");
		return eLTE_SVC_ERR_WAIT_LOGIN_RESULT;
	}

	// 登陆成功标示
	m_bLoginSuccess = FALSE;

	// 获取SDK实例指针
	ApplicationContext * pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	OperationManager * pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 停止SDK处理业务
	int sRet = pAC->stopRunning();
	if (0 != sRet)
	{
		LOG_RUN_ERROR("stopRunning failed. (%d)", sRet);
		return sRet;
	}

	// 设置登出资源
	ResourceInfo* pResInfo = new ResourceInfo(eLTE_Tool::String2Int(userId));
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	pResInfo->setUserName(userId.c_str());
	pResInfo->setPassWord(m_passwd.c_str());
	pResInfo->setServerIp(m_serverIP.c_str());
	pResInfo->setLocalIp(m_localIP.c_str());
	pResInfo->setSipPort(m_sipPort.c_str());

	// 调度台注销
	Op_Return_t iRet = pOM->invokeOp(pResInfo, DC_LOGOUT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp DC_LOGOUT failed. (%d)", iRet);
	}

	// 释放登出资源
	delete pResInfo;
	pResInfo = NULL;
	// 清理本地变量
	m_userId.clear();
	m_passwd.clear();
	m_localIP.clear();
	m_serverIP.clear();
	m_sipPort.clear();

	return iRet;//lint !e438
}

// 启动SDK处理业务
int OperationMgr::StartRunning()
{
	LOG_TRACE();

	// 校验是否登陆
	if (m_userId.empty())
	{
		// 没有登陆，登出成功则不需要startRunning
		return eLTE_SVC_ERR_SUCCESS;
	}

	if (m_bLoginSuccess)
	{
		return eLTE_SVC_ERR_SUCCESS;
	}

	// 登陆成功标示
	m_bLoginSuccess = TRUE;

	// 获取SDK实例指针
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 启动SDK处理业务
	int iRet = pAC->startRunning(m_serverIP.c_str());
	if (0 != iRet)
	{
		LOG_RUN_ERROR("startRunning failed. (%d)", iRet);
		return iRet;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 触发状态上报
int OperationMgr::TriggerStatusReport(unsigned int uiEnableStatusReport) const
{
	LOG_TRACE();
	INFO_PARAM1(uiEnableStatusReport);

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 触发上报状态
	bool bEnable = ((FALSE == uiEnableStatusReport) ? false : true);
	pAC->triggerStatusReport(bEnable);

	return eLTE_SVC_ERR_SUCCESS;
}

// 调度台发起视频回传
int OperationMgr::VideoMonitor(int iResId, Video_UL_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM5(iResId, param.fmtvalue, param.cameratype, param.mutetype, param.user_confirm_type);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起视频回传
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_MONITOR);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_MONITOR failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 接受视频回传
int OperationMgr::RecvVideoPlay(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起视频回传
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_VIDEO_RECV);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_VIDEO_RECV failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台停止视频分发
int OperationMgr::StopVideoDispatch(int iResId, Video_Dispatch_Delete& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.resId, param.userId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 开始视频分发
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_DISPATCH_DELETE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_DISPATCH_DELETE failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台开始视频分发
int OperationMgr::VideoDispatch(int iResId, Video_Dispatch_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.dispatch_num, param.fmtvalue);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 开始视频分发
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_DISPATCH);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_DISPATCH failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台挂断视频回传
int OperationMgr::VideoHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 挂断视频回传
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_VIDEO_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_VIDEO_HANGUP failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 云台控制
int OperationMgr::PTZControl(int iResId, FVG_control& control) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, control.ptz_control_code, control.ptz_control_value);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起视频回传
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&control, DC_PZT_CONCTROL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DC_PZT_CONCTROL failed. (%d)", iRet);
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 订阅群组
int OperationMgr::SubscribeGroup(int groupId, unsigned int uiEableSubscribeGroup) const
{
	LOG_TRACE();
	INFO_PARAM2(groupId, uiEableSubscribeGroup);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造群组资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("pResInfo is null.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 去订阅群组
	Op_Return_t iRet;
	if (0 == uiEableSubscribeGroup)
	{
		iRet = pOM->invokeOp(pResInfo, GRP_UNSUB);
		if (OP_OK_ACCEPTED != iRet)
		{
			LOG_RUN_ERROR("invokeOp GRP_UNSUB failed. (%d)", iRet);
		}
	}
	// 订阅群组
	else
	{
		iRet = pOM->invokeOp(pResInfo, GRP_SUB);
		if (OP_OK_ACCEPTED != iRet)
		{
			LOG_RUN_ERROR("invokeOp GRP_SUB failed. (%d)", iRet);
		}
	}

	// 释放群组资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 创建动态重组
int OperationMgr::CreateDynamicGroup(DGNA_parameter& param) const
{
	LOG_TRACE();

	// 参数校验
	static const int MAX_ALIAS_LEN = 32;
	static const int MIN_PRIORITY_VAR = 1;
	static const int MAX_PRIORITY_VAR = 15;
	static const int MIN_PERIOD_VAR = 1;
	static const int MAX_PERIOD_VAR = 65535;
	if (param.alias.empty() || param.alias.size() > MAX_ALIAS_LEN)
	{
		LOG_RUN_ERROR("alias is empty or length is greater than 32.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}
	if (param.priority < MIN_PRIORITY_VAR || param.priority > MAX_PRIORITY_VAR)
	{
		LOG_RUN_ERROR("priority is greater than 15 or less than 1.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}
	if (param.maxPeriod < MIN_PERIOD_VAR || param.maxPeriod > MAX_PERIOD_VAR)
	{
		LOG_RUN_ERROR("maxPeriod is greater than 65535 or less than 1.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构建资源
	int iDcID = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcID);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 创建动态重组
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, DGNA_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CREATE failed. (%d)", iRet);
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 删除动态重组
int OperationMgr::CancelDynamicGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构建资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 删除动态重组
	int iRet = pOM->invokeOp_multipara(pResInfo, NULL, DGNA_CANCEL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CANCEL failed. (%d)", iRet);
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 创建派接组、增加派接组成员、删除派接组成员
int OperationMgr::OperatePatchGroup(PCHGRP_Para& param, const unsigned short& uOperateType) const
{
	LOG_TRACE();
	INFO_PARAM3(param.DcId, param.PatchGroupId, param.PatchGroupName);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构建资源
	ResourceInfo* pResInfo = new ResourceInfo(param.DcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	int iRet = 0;
	// 创建动态重组
	switch(uOperateType)
	{
		case ELTE_SERVICE_CREATEPATCHGROUP_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_CREATE);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_CREATE failed. (%d)", iRet);
				}
			}
			break;
		case ELTE_SERVICE_ADDPATCHMEMBER_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_ADD);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_ADD failed. (%d)", iRet);
				}
			}
			break;
		case ELTE_SERVICE_DELETEPATCHMEMBER_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_DEL);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_DEL failed. (%d)", iRet);
				}
			}
			break;
		default:
			{
				iRet = eLTE_SVC_ERR_FAILED;
				LOG_RUN_ERROR("Invalid MsgType: %x", uOperateType);
			}
			break;
	}
	
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 删除派接组
int OperationMgr::CancelPatchGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构建资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 删除动态重组
	int iRet = pOM->invokeOp(pResInfo, PCHGRP_CANCEL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PCHGRP_CANCEL failed. (%d)", iRet);
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 创建临时组
int OperationMgr::CreateTempGroup(DGNA_parameter& param) const
{
	LOG_TRACE();

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构建资源
	int iDcId = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 创建临时组
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, TEMPGRP_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara TEMPGRP_CREATE failed.");
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 申请一个临时组号
int OperationMgr::GetNextTempGroupID() const
{
	LOG_TRACE();
	int iGroupID = 0;

	// 获取临时组号
	DcState* pDcState = DcState::getInstance();
	if (NULL == pDcState)
	{
		LOG_RUN_ERROR("DcState get instance failed.");
		return iGroupID;
	}
	iGroupID = pDcState->getNextTempGroupID();

	return iGroupID;
}

// 发起半双工点呼
int OperationMgr::P2PHalfDpxDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HALFDPX_DIAL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HALFDPX_DIAL failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 释放半双工点呼
int OperationMgr::P2PHalfDpxRelease(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HALFDPX_RELEASE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HALFDPX_RELEASE failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 发起语音点呼
int OperationMgr::P2PDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_DIALOUT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_DIALOUT failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 接收语音点呼
int OperationMgr::P2PRecv(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 接收语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_RECV);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_RECV failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 拒接语音点呼
int OperationMgr::P2PReject(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 拒接语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_REJECT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_REJECT failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 挂断语音点呼
int OperationMgr::P2PHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 挂断语音点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HANGUP failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台强拆点呼
int OperationMgr::P2PBreakoff(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 调度台强拆点呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKOFF failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 抢话（强拆点呼+发起新点呼）
int OperationMgr::P2PBreakin(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 抢话（强拆点呼+发起新点呼）
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_BREAKIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKIN failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 订阅并自动加入群组
int OperationMgr::SubJoinGroup(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 订阅并自动加入群组
	Op_Return_t iRet = pOM->invokeOp(pResInfo, BATCH_CFG);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp BATCH_CFG failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 发起组呼（固定和临时组呼）或抢权
int OperationMgr::PTTDial(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起组呼（固定和临时组呼）或抢权
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_DIAL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_DIAL failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 释放话权
int OperationMgr::PTTRelease(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 释放话权
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_RELEASE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_RELEASE failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 退出组呼
int OperationMgr::PTTHangup(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 退出组呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_HANGUP failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 发起紧急组呼
int OperationMgr::PTTEmergency(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 发起紧急组呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_EMERGENCY);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_EMERGENCY failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台强拆组呼
int OperationMgr::GroupBreakoff(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 调度台强拆组呼
	Op_Return_t iRet = pOM->invokeOp(pResInfo, GRP_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GRP_BREAKOFF failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台执行静音
int OperationMgr::VolMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 调度台执行静音
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_MUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp VOL_MUTE failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::SetUserSpecificGISCfg(int resId, UEGisCfg_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(resId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(resId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 调度台修改终端的GIS配置
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, GIS_CONFIG);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GIS_CONFIG failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::ModifyDynamicGroup(DGNA_Modify_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM2(param.dcId, param.grpId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(param.grpId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 修改动态重组
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, DGNA_EDIT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp DGNA_EDIT failed. (%d)", iRet);	
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 调度台取消静音
int OperationMgr::VolUnMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 调度台取消静音
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_UNMUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp VOL_UNMUTE failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 订阅/去订阅GIS终端
int OperationMgr::GISSubscribe(int iResId, GIS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	param.msg_body = eLTE_Tool::ANSIToUTF8(param.msg_body);

	// 订阅/去订阅GIS终端
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, GIS_SUBSCRIBE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GIS_SUBSCRIBE failed. (%d)", iRet);	
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::SDSSend(const int& iResId, SDS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.sdstype, param.receiver);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造短信资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	param.msg_body = eLTE_Tool::ANSIToUTF8(param.msg_body);

	// 发送短信
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, SDS_SEND);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara SDS_SEND failed. (%d)", iRet);
	}

	// 释放短信资源
	delete pResInfo;
	pResInfo=NULL;
	return iRet;//lint !e438
}

// 调度台发起视频回传
int OperationMgr::VWallStart(int iResId, VWallStart_parameter& param) const
{
	LOG_TRACE();

	// 获取SDK实例指针
	GET_SDK_POINTER;
	
	//获取资源ID
//	iResId = param.DstObjId;

	// 构建资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	// 发起视频上墙
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VWALL_START);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VWALL_START failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 视频下墙
int OperationMgr::VWallStop(int iResId, VWallStop_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId,param.DstObjId, param.SrcObjId);

	// 获取SDK实例指针
	GET_SDK_POINTER;

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 开始视频下墙
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VWALL_STOP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VWALL_STOP failed. (%d)", iRet);
	}
	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

//更新电视墙ID状态
int OperationMgr::UpdateDcVWallIDState(VWallInfoList* const &m_WallInfoList) const
{
	LOG_TRACE();
	if (NULL == m_WallInfoList)
	{
		LOG_RUN_ERROR("m_WallInfoList is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	
	// 获取SDK实例指针
	DcState* pDcState = DcState::getInstance();
	if (NULL == pDcState)
	{
		LOG_RUN_ERROR("DcState get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	VWallInfoList::const_iterator it_b = m_WallInfoList->begin();
	VWallInfoList::const_iterator it_e = m_WallInfoList->end();

	LOG_RUN_INFO("list size:%d",m_WallInfoList->size());
	User_Call_Status* m_WallIdState = NULL;
	for (VWallInfoList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		VWallInfo* const &pVWallIDList = *itor;
		if (NULL == pVWallIDList)
		{
			LOG_RUN_ERROR("pVWallIDList is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		m_WallIdState = pDcState->findUserStatus(pVWallIDList->userid,VIDEOONWALL);		
		
		if (NULL != m_WallIdState)
		{
			LOG_RUN_INFO("UpdateDcVWallIDState ID:%d state:%d",pVWallIDList->userid, m_WallIdState->userStatus);
			pVWallIDList->Status = m_WallIdState->userStatus;
		}				
	}		
	
	m_WallIdState = NULL;	
	return eLTE_SVC_ERR_SUCCESS;//lint !e438
}
// 
// //发起PSTN/PLMN电话呼叫 
// int OperationMgr::TelephoneDial(const std::string& PhoneNum) const
// {
// 	LOG_TRACE();
// 
// 	// 获取SDK实例指针
// 	GET_SDK_POINTER;
// 
// 	//对外部PSTN/PLMN电话发起呼叫,ResourceID固定为99911000
// 	int	iResId = 99911000;
// 
// 	// 构建资源
// 	TIResourceInfo* pTIResInfo = new TIResourceInfo(iResId, PhoneNum);
// 	if (NULL == pTIResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new TIResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 	
// 	// 发起PSTN/PLMN电话呼叫 
// 	Op_Return_t iRet = pOM->invokeOp(pTIResInfo, TIC_DIALOUT);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp TelephoneDial failed. (%d)", iRet);
// 	}
// 	// 释放资源
// 	delete pTIResInfo;
// 	pTIResInfo = NULL;
// 	return iRet;//lint !e438
// }
// 
// //挂断PSTN/PLMN电话呼叫 
// int OperationMgr::TelephoneHangup(const std::string& PhoneNum) const
// {
// 	LOG_TRACE();
// 
// 	// 获取SDK实例指针
// 	GET_SDK_POINTER;
// 
// 	//对外部PSTN/PLMN电话发起呼叫,ResourceID固定为99911000
// 	int	iResId = 99911000;
// 
// 	// 构建资源
// 	TIResourceInfo* pTIResInfo = new TIResourceInfo(iResId, PhoneNum);
// 	if (NULL == pTIResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new TIResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 	
// 	// 挂断PSTN/PLMN电话呼叫
// 	Op_Return_t iRet = pOM->invokeOp(pTIResInfo, TIC_HANGUP);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp TelephoneHangup failed. (%d)", iRet);
// 	}
// 	// 释放资源
// 	delete pTIResInfo;
// 	pTIResInfo = NULL;
// 	return iRet;//lint !e438
// }

//缜密监听
int OperationMgr::DiscreetListenOpera(int ResourceID, Operation_t OperationType) const
{
	LOG_TRACE();

	// 获取SDK实例指针
	GET_SDK_POINTER;
	
	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(ResourceID);
	if (NULL == pResInfo)
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// 开始缜密监听
	Op_Return_t iRet = pOM->invokeOp(pResInfo, OperationType);
	LOG_RUN_INFO("DiscreetListenOpera: %d-%d",pResInfo->resId,OperationType);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp StartDiscreetListen failed. (%d)", iRet);
	}

	// 释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 
// int OperationMgr::TempUserJoinGroup(const int& resid, const PhonePatch_parameter& param) const
// {
// 	LOG_TRACE();
// 	INFO_PARAM3(resid, param.DCID, param.UserID);
// 
// 	//获取业务管理类的全局唯一实例
// 	OperationManager* pOM = OperationManager_T::instance();
// 	if (NULL == pOM)
// 	{
// 		LOG_RUN_ERROR("OperationManager_T get instance failed.");
// 		return eLTE_SVC_ERR_NULL_POINTER;
// 	}
// 
// 	// 构造资源
// 	ResourceInfo* pResInfo = new ResourceInfo(resid);
// 	if (NULL == pResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new ResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 
// 	//调用人工转接业务接口
// 	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo,(void*)&param, TEMPUSER_JOIN_GRPCALL);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp_multipara TEMPUSER_JOIN_GRPCALL failed. (%d)", iRet);
// 	}
// 
// 	//释放资源
// 	delete pResInfo;
// 	pResInfo = NULL;
// 	return iRet;//lint !e438
// }

int OperationMgr::P2PTransfer(const int iResId, transfer_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM4(iResId, param.dcid, param.speakerid, param.objectid);

	//获取业务管理类的全局唯一实例
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// 构造资源
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	//调用人工转接业务接口
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo,(void*)&param, P2P_TRANSFER);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2PTransfer failed. (%d)", iRet);
	}

	//释放资源
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}
