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
filename		: 	eLTE_SDK.cpp
author			:	zWX229156
created			:	2015/1/5	
description		:	定义 DLL 应用程序的导出函数
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 初始版本
*********************************************************************/

#include "stdafx.h"
#include "eLTE_SDK.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "UserMgr.h"
#include "OpenSSL_Mgr.h"
#include "eLTE_Tool.h"
#include "SharedMemoryMgr.h"
#include <string>



// SDK版本号
#define ELTE_SDK_VERSION		"2.1.10.0"
#define LOG_CATEGORY            3
CUserMgr* g_pUserMgr = NULL;
static std::string g_strLogPath = "";
static ELTE_UINT32 g_uiLogLevel = INVALID_LOG_LEVEL;

#define CHECK_INTERFACE_POINTER(p, fmt, ...) {\
	if (NULL == (p))\
	{\
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_NULL_POINTER, (fmt), ##__VA_ARGS__);\
		LOG_RUN_ERROR("%s is null.", #p);\
		return eLTE_SDK_ERR_NULL_POINTER;\
	}\
} 

#pragma comment(linker, "/EXPORT:ELTE_SDK_SetLogPath=_ELTE_SDK_SetLogPath@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SetLogLevel=_ELTE_SDK_SetLogLevel@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_ReleaseBuffer=_ELTE_SDK_ReleaseBuffer@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetVersion=_ELTE_SDK_GetVersion@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_Init=_ELTE_SDK_Init@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_Cleanup=_ELTE_SDK_Cleanup@0")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SetEventCallBack=_ELTE_SDK_SetEventCallBack@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SetYUVFrameCallBack=_ELTE_SDK_SetYUVFrameCallBack@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_Login=_ELTE_SDK_Login@20")
#pragma comment(linker, "/EXPORT:ELTE_SDK_Logout=_ELTE_SDK_Logout@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_TriggerStatusReport=_ELTE_SDK_TriggerStatusReport@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_ProvisionManagerInitMRS=_ELTE_SDK_ProvisionManagerInitMRS@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetDcGroups=_ELTE_SDK_GetDcGroups@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetDcUsers=_ELTE_SDK_GetDcUsers@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetGroupUsers=_ELTE_SDK_GetGroupUsers@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetGroupInfo=_ELTE_SDK_GetGroupInfo@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetUserInfo=_ELTE_SDK_GetUserInfo@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetDcInfo=_ELTE_SDK_GetDcInfo@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_UnSubscribeGroup=_ELTE_SDK_UnSubscribeGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SubJoinGroup=_ELTE_SDK_SubJoinGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_CreateDynamicGroup=_ELTE_SDK_CreateDynamicGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_CancelDynamicGroup=_ELTE_SDK_CancelDynamicGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetTempGroupID=_ELTE_SDK_GetTempGroupID@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_CreateTempGroup=_ELTE_SDK_CreateTempGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetUserRECFileInfoList=_ELTE_SDK_GetUserRECFileInfoList@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StartRealPlay=_ELTE_SDK_StartRealPlay@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SetPlayWindow=_ELTE_SDK_SetPlayWindow@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_LocalSnapshot=_ELTE_SDK_LocalSnapshot@12")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SetPlayWindowSize=_ELTE_SDK_SetPlayWindowSize@12")
#pragma comment(linker, "/EXPORT:ELTE_SDK_PTZControl=_ELTE_SDK_PTZControl@12")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StopRealPlay=_ELTE_SDK_StopRealPlay@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PDial=_ELTE_SDK_P2PDial@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PHalfDpxRelease=_ELTE_SDK_P2PHalfDpxRelease@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PHalfDpxDial=_ELTE_SDK_P2PHalfDpxDial@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PRecv=_ELTE_SDK_P2PRecv@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PReject=_ELTE_SDK_P2PReject@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PHangup=_ELTE_SDK_P2PHangup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PBreakoff=_ELTE_SDK_P2PBreakoff@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PBreakin=_ELTE_SDK_P2PBreakin@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_PTTDial=_ELTE_SDK_PTTDial@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_PTTRelease=_ELTE_SDK_PTTRelease@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_PTTHangup=_ELTE_SDK_PTTHangup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_PTTEmergency=_ELTE_SDK_PTTEmergency@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GroupBreakoff=_ELTE_SDK_GroupBreakoff@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_VolMute=_ELTE_SDK_VolMute@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_VolUnMute=_ELTE_SDK_VolUnMute@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GISSubscribe=_ELTE_SDK_GISSubscribe@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_SDSSendMessage=_ELTE_SDK_SDSSendMessage@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StartVideoDispatch=_ELTE_SDK_StartVideoDispatch@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StopVideoDispatch=_ELTE_SDK_StopVideoDispatch@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_RecvVideoPlay=_ELTE_SDK_RecvVideoPlay@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_VWallStart=_ELTE_SDK_VWallStart@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetDcVWallIDList=_ELTE_SDK_GetDcVWallIDList@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_VWallStop=_ELTE_SDK_VWallStop@8")
//#pragma comment(linker, "/EXPORT:ELTE_SDK_TelephoneDial=_ELTE_SDK_TelephoneDial@4")
//#pragma comment(linker, "/EXPORT:ELTE_SDK_TelephoneHangup=_ELTE_SDK_TelephoneHangup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StartDiscreetListen=_ELTE_SDK_StartDiscreetListen@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StopDiscreetListen=_ELTE_SDK_StopDiscreetListen@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_StartEnvironmentListen=_ELTE_SDK_StartEnvironmentListen@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_P2PTransfer=_ELTE_SDK_P2PTransfer@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_CreatePatchGroup=_ELTE_SDK_CreatePatchGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_CancelPatchGroup=_ELTE_SDK_CancelPatchGroup@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_AddPatchGroupMember=_ELTE_SDK_AddPatchGroupMember@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_DeletePatchGroupMember=_ELTE_SDK_DeletePatchGroupMember@4")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetPatchGroups=_ELTE_SDK_GetPatchGroups@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetGroupMemberByPatchId=_ELTE_SDK_GetGroupMemberByPatchId@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetPatchGroupInfo=_ELTE_SDK_GetPatchGroupInfo@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_GetGisSubscription=_ELTE_SDK_GetGisSubscription@8")
//#pragma comment(linker, "/EXPORT:ELTE_SDK_TempUserJoinGroup=_ELTE_SDK_TempUserJoinGroup@8")
#pragma comment(linker, "/EXPORT:ELTE_SDK_ModifyDynamicGroup=_ELTE_SDK_ModifyDynamicGroup@8")

// 设置日志路径
ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogPath(const ELTE_CHAR* pLogFilePath)
{
	if(NULL == pLogFilePath)
	{
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	std::string strPath(pLogFilePath);
	eLTE_Tool::RelativePathToAbsolute(strPath);
	g_strLogPath = strPath;
	return eLTE_SDK_ERR_SUCCESS;
}

// 设置日志级别
ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogLevel(ELTE_UINT32 iLogLevel)
{
	g_uiLogLevel = iLogLevel;
	return eLTE_SDK_ERR_SUCCESS;
}

// 释放SDK内部申请的内存
ELTE_INT32 __SDK_CALL ELTE_SDK_ReleaseBuffer(ELTE_CHAR* pBuffer)
{
	LOG_INTERFACE_TRACE();
	if(NULL != pBuffer)
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}	
	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "");
	return eLTE_SDK_ERR_SUCCESS;//lint !e438
}

// 获取SDK版本号
ELTE_INT32 __SDK_CALL ELTE_SDK_GetVersion(ELTE_CHAR** pVersion)
{
	LOG_INTERFACE_TRACE();

	ELTE_UINT32 iLength = strlen(ELTE_SDK_VERSION);
	*pVersion = new ELTE_CHAR[iLength + 1];
	CHECK_INTERFACE_POINTER(*pVersion, "");
	eSDK_MEMSET(*pVersion , 0x0, (iLength + 1));
	eSDK_MEMCPY(*pVersion, iLength + 1, ELTE_SDK_VERSION, iLength);

	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "Version:%s", *pVersion);
	return eLTE_SDK_ERR_SUCCESS;
}

// 初始化SDK
ELTE_INT32 __SDK_CALL ELTE_SDK_Init(ELTE_INT32 iMediaBypass)
{
	//打开日志
	std::string strIniPath = eLTE_Tool::GetDllPath(ELTE_SDK_DLL_NAME);
	strIniPath.append(LOG_INI_FILE_NAME);
	ELTE_UINT32 logLevel[LOG_CATEGORY];
	for (ELTE_UINT32 i = 0; i < LOG_CATEGORY; ++ i)
	{
		logLevel[i] = g_uiLogLevel;
	}
 	ELTE_INT32 iRet = LOG_INIT(strIniPath.c_str(), logLevel, g_strLogPath.c_str());
	if(RET_SUCCESS != iRet)
	{
		return eLTE_SDK_ERR_LOG_INIT;
	}
	LOG_INTERFACE_TRACE();
	LOG_RUN_INFO("eSDK eLTE compile time is %s : %s", __DATE__, __TIME__);
	if(NULL == g_pUserMgr)
	{
		g_pUserMgr = new CUserMgr();
		if(NULL == g_pUserMgr)
		{
			LOG_RUN_ERROR("New CUserMgr failed.");
			LOG_INTERFACE_INFO(eLTE_SDK_ERR_CREATE_OBJECT, "");
			//关闭日志
			LOG_EXIT();
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
	}
	g_pUserMgr->SetBypass(iMediaBypass);
	iRet = (const_cast<CServerMgr&>(g_pUserMgr->GetServerMgr())).Init(iMediaBypass);
	LOG_INTERFACE_INFO(iRet, "MediaBypass:%d", iMediaBypass);
	return iRet;
}

// 释放SDK
ELTE_INT32 __SDK_CALL ELTE_SDK_Cleanup()
{
	LOG_INTERFACE_TRACE();
	ELTE_INT32 iRet = eLTE_SDK_ERR_SUCCESS;
	if (NULL != g_pUserMgr)
	{
		//停止保活
		CTimer& timer = const_cast<CTimer&>(g_pUserMgr->GetTimer());
		(void)timer.StopTimer();
		SSL_Socket& socket = const_cast<SSL_Socket&>(g_pUserMgr->GetSSLSocket());
		g_pUserMgr->SetbIsDisconnect(TRUE);
		iRet = socket.DisConnect();
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Disconnect req failed.");
		}
		iRet = g_pUserMgr->GetServerMgr().CleanUp();
		delete g_pUserMgr;
		g_pUserMgr = NULL;
	}

	LOG_INTERFACE_INFO(iRet, "");
	//关闭日志
	LOG_EXIT();
	return iRet;
}

// 设置消息回调函数
ELTE_INT32 __SDK_CALL ELTE_SDK_SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	LOG_INTERFACE_TRACE();
	CHECK_INTERFACE_POINTER(g_pUserMgr, "");
	g_pUserMgr->SetEventCallBack(fEventCallBack, pUserData);
	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "");
	return eLTE_SDK_ERR_SUCCESS;
}

// 设置视频数据回调函数
ELTE_INT32 __SDK_CALL ELTE_SDK_SetYUVFrameCallBack(YUVFrameCallBack fYUVFrameCallBack, ELTE_VOID* pUserData)
{
	LOG_INTERFACE_TRACE();
	CHECK_INTERFACE_POINTER(g_pUserMgr, "");
	g_pUserMgr->SetYUVFrameCallBack(fYUVFrameCallBack, pUserData);
	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "");
	return eLTE_SDK_ERR_SUCCESS;
}

// 登录
ELTE_INT32 __SDK_CALL ELTE_SDK_Login(const ELTE_CHAR* pUserID, const ELTE_CHAR* pPWD, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, ELTE_UINT32 uiServerSIPPort)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID || NULL == pPWD || NULL == pServerIP || NULL == pLocalIP)
	{
		LOG_RUN_ERROR("UserID,PWD,ServerIP or LocalIP is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s, ServerIP:**.**.**.**, LocalIP:**.**.**.**, ServerSIPPort:***", pUserID);
	g_pUserMgr->SetUserID(pUserID);
	g_pUserMgr->SetUserPwd(pPWD);
	g_pUserMgr->SetLocalIP(pLocalIP);
	ELTE_INT32 iRet = g_pUserMgr->GetLoginMgr().Login(pUserID, pPWD, pServerIP, pLocalIP, uiServerSIPPort);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, ServerIP:**.**.**.**, LocalIP:**.**.**.**, ServerSIPPort:***", pUserID);
	return iRet;
}
 
// 注销
ELTE_INT32 __SDK_CALL ELTE_SDK_Logout(const ELTE_CHAR* pUserID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID)
	{
		LOG_RUN_ERROR("UserID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s.", pUserID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s", pUserID);
	ELTE_INT32 iRet = g_pUserMgr->GetLoginMgr().Logout(pUserID);
	LOG_INTERFACE_INFO(iRet, "UserID:%s", pUserID);
	return iRet;
}

// 触发状态上报
ELTE_INT32 __SDK_CALL ELTE_SDK_TriggerStatusReport(ELTE_INT32 iEnableStatusReport)
{
	LOG_INTERFACE_TRACE();
	LOG_RUN_INFO("EnableStatusReport:%d.", iEnableStatusReport);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "EnableStatusReport:%d", iEnableStatusReport);
	ELTE_INT32 iRet = g_pUserMgr->GetLoginMgr().TriggerStatusReport(iEnableStatusReport);
	LOG_INTERFACE_INFO(iRet, "EnableStatusReport:%d", iEnableStatusReport);
	return iRet;
}

// 媒体服务器初始化
ELTE_INT32 __SDK_CALL ELTE_SDK_ProvisionManagerInitMRS(const ELTE_CHAR* pServerIP)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pServerIP)
	{
		LOG_RUN_ERROR("ServerIP is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ServerIP:%s.", "***.***.***.***");

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ServerIP:%s", pServerIP);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().ProvisionManagerInitMRS(pServerIP);
	LOG_INTERFACE_INFO(iRet, "ServerIP:%s", pServerIP);
	return iRet;
}

// 获取调度台群组列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID)
	{
		LOG_RUN_ERROR("UserID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s.", pUserID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s", pUserID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetDcGroups(pUserID, pDcGroups);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, DcGroups:%s", pUserID, NULL == *pDcGroups ? "" : *pDcGroups);
	return iRet;
}

// 获取调度台用户列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID)
	{
		LOG_RUN_ERROR("UserID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s.", pUserID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s", pUserID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetDcUsers(pUserID, pDcUsers);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, DcUsers:%s", pUserID, NULL == *pDcUsers ? "" : *pDcUsers);
	return iRet;
}
// 获取一个群组成员列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetGroupUsers(pGroupID, pGroupUsers);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s, GroupUsers:%s", pGroupID, NULL == *pGroupUsers ? "" : *pGroupUsers);
	return iRet;
}

// 获取一个群组详细信息
ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetGroupInfo(pGroupID, pGroupInfo);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s, GroupInfo:%s", pGroupID, NULL == *pGroupInfo ? "" : *pGroupInfo);
	return iRet;
}

// 获取一个用户详细信息
ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID)
	{
		LOG_RUN_ERROR("UserID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s.", pUserID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s", pUserID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetUserInfo(pUserID, pUserInfo);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, UserInfo:%s", pUserID, NULL == *pUserInfo ? "" : *pUserInfo);
	return iRet;
}

// 获取一个调度台详细信息
ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID)
	{
		LOG_RUN_ERROR("UserID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s.", pUserID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s", pUserID);
	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetDcInfo(pUserID, pDcInfo);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, DcInfo:%s", pUserID, NULL == *pDcInfo ? "" : *pDcInfo);
	return iRet;
}

// 取消订阅群组
ELTE_INT32 __SDK_CALL ELTE_SDK_UnSubscribeGroup(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().UnSubscribeGroup(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 订阅并自动加入群组
ELTE_INT32 __SDK_CALL ELTE_SDK_SubJoinGroup(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().SubJoinGroup(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 创建动态群组
ELTE_INT32 __SDK_CALL ELTE_SDK_CreateDynamicGroup(const ELTE_CHAR* pDGNAParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pDGNAParam)
	{
		LOG_RUN_ERROR("DGNAParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("DGNAParam:%s.", pDGNAParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "DGNAParam:%s", pDGNAParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().CreateDynamicGroup(pDGNAParam);
	LOG_INTERFACE_INFO(iRet, "DGNAParam:%s", pDGNAParam);
	return iRet;
}

// 删除动态群组
ELTE_INT32 __SDK_CALL ELTE_SDK_CancelDynamicGroup(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().CancelDynamicGroup(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 创建派接组
ELTE_INT32 __SDK_CALL ELTE_SDK_CreatePatchGroup(const ELTE_CHAR* pPatchGroupParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pPatchGroupParam)
	{
		LOG_RUN_ERROR("PatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupParam:%s.", pPatchGroupParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupParam:%s", pPatchGroupParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().OperatePatchGroup(pPatchGroupParam, ELTE_SERVICE_CREATEPATCHGROUP_REQ);
	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", pPatchGroupParam);
	return iRet;
}

// 删除派接组
ELTE_INT32 __SDK_CALL ELTE_SDK_CancelPatchGroup(const ELTE_CHAR* pPatchGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pPatchGroupID)
	{
		LOG_RUN_ERROR("PatchGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupID:%s.", pPatchGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupID:%s", pPatchGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().CancelPatchGroup(pPatchGroupID);
	LOG_INTERFACE_INFO(iRet, "PatchGroupID:%s", pPatchGroupID);
	return iRet;
}

// 增加派接组成员
ELTE_INT32 __SDK_CALL ELTE_SDK_AddPatchGroupMember(const ELTE_CHAR* pPatchGroupParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pPatchGroupParam)
	{
		LOG_RUN_ERROR("PatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupParam:%s.", pPatchGroupParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupParam:%s", pPatchGroupParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().OperatePatchGroup(pPatchGroupParam, ELTE_SERVICE_ADDPATCHMEMBER_REQ);
	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", pPatchGroupParam);
	return iRet;
}

// 删除派接组成员
ELTE_INT32 __SDK_CALL ELTE_SDK_DeletePatchGroupMember(const ELTE_CHAR* pPatchGroupParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pPatchGroupParam)
	{
		LOG_RUN_ERROR("PatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupParam:%s.", pPatchGroupParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupParam:%s", pPatchGroupParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().OperatePatchGroup(pPatchGroupParam, ELTE_SERVICE_DELETEPATCHMEMBER_REQ);
	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", pPatchGroupParam);
	return iRet;
}

//获取指定调度台的所有派接组列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetPatchGroups(const ELTE_CHAR* pDcUser, ELTE_CHAR** pPatchGroups)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pDcUser)
	{
		LOG_RUN_ERROR("DcUser is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("DcUser:%s.", pDcUser);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "DcUser:%s", pDcUser);

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetPatchGroups(pDcUser, pPatchGroups);
	LOG_INTERFACE_INFO(iRet, "DcUser:%s", pDcUser);
	return iRet;
}

// 获取指定派接组管理的成员列表
ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupMemberByPatchId(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupMembers)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pPatchGroupId)
	{
		LOG_RUN_ERROR("PatchGroupId is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupId:%s.", pPatchGroupId);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupId:%s", pPatchGroupId);

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetGroupMemberByPatchId(pPatchGroupId, pPatchGroupMembers);
	LOG_INTERFACE_INFO(iRet, "PatchGroupId:%s", pPatchGroupId);
	return iRet;
}

// 获取指定派接组属性信息
ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetPatchGroupInfo(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupInfo)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pPatchGroupId)
	{
		LOG_RUN_ERROR("PatchGroupId is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("PatchGroupId:%s.", pPatchGroupId);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "PatchGroupId:%s", pPatchGroupId);

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetPatchGroupInfo(pPatchGroupId, pPatchGroupInfo);
	LOG_INTERFACE_INFO(iRet, "PatchGroupId:%s", pPatchGroupId);
	return iRet;
}

// 获取临时组号
ELTE_INT32 __SDK_CALL ELTE_SDK_GetTempGroupID(ELTE_INT32* iGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == iGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%d", *iGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().GetTempGroupID(iGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%d", *iGroupID);
	return iRet;
}

// 创建临时组
ELTE_INT32 __SDK_CALL ELTE_SDK_CreateTempGroup(const ELTE_CHAR* pTEMPGParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pTEMPGParam)
	{
		LOG_RUN_ERROR("TEMPGParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("TEMPGParam:%s.", pTEMPGParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "TEMPGParam:%s", pTEMPGParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().CreateTempGroup(pTEMPGParam);
	LOG_INTERFACE_INFO(iRet, "TEMPGParam:%s", pTEMPGParam);
	return iRet;
}

// 获取录像文件列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pQueryXml)
	{
		LOG_RUN_ERROR("QueryXml is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("QueryXml:%s.", pQueryXml);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "QueryXml:%s", pQueryXml);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().GetUserRECFileInfoList(pQueryXml, pRspXml);
	LOG_INTERFACE_INFO(iRet, "QueryXml:%s", pQueryXml);
	return iRet;
}

// 开始播放实时视频
ELTE_INT32 __SDK_CALL ELTE_SDK_StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pVideoParam)
	{
		LOG_RUN_ERROR("ResourceID or VideoParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, VideoParam:%s.", pResourceID, pVideoParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, VideoParam:%s", pResourceID, pVideoParam);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StartRealPlay(pResourceID, pVideoParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", pResourceID, pVideoParam);
	return iRet;
}

// 接受视频回传
ELTE_INT32 __SDK_CALL ELTE_SDK_RecvVideoPlay(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().RecvVideoPlay(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_SetPlayWindow(const ELTE_CHAR* pResourceID, ELTE_VOID* hWnd)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	if (NULL == hWnd)
	{
		LOG_RUN_DEBUG("Wnd is null.");
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);

	ELTE_INT32 iRet = SharedMemoryMgr::Instance().SetPlayWindow(pResourceID, (HWND)hWnd);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s.", pResourceID);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_LocalSnapshot(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSnapshotPath, ELTE_ULONG ulSnapshotFormat)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pSnapshotPath)
	{
		LOG_RUN_ERROR("ResourceID or SnapshotPath is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, SnapshotPath:%s, SnapshotFormat:%u.", pResourceID, pSnapshotPath, ulSnapshotFormat);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, SnapshotPath:%s, SnapshotFormat:%u.", pResourceID, pSnapshotPath, ulSnapshotFormat);

	ELTE_INT32 iRet = SharedMemoryMgr::Instance().LocalSnapshot(pResourceID, pSnapshotPath, ulSnapshotFormat);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s.", pResourceID);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_SetPlayWindowSize(const ELTE_CHAR* pResourceID, ULONG ulWidth, ULONG ulHeight)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	
	ELTE_INT32 iRet = 0;
	if(!CUserMgr::m_iBypass)
	{
		iRet = SharedMemoryMgr::Instance().SetPlayWindowSize(pResourceID, ulWidth, ulHeight);
	}
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s.", pResourceID);
	return iRet;
}

// 开始云台控制
ELTE_INT32 __SDK_CALL ELTE_SDK_PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, PTZControlCode:%d, PTZControlValue:%d.", pResourceID, iPTZControlCode, iPTZControlValue);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, PTZControlCode:%d, PTZControlValue:%d", pResourceID, iPTZControlCode, iPTZControlValue);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().PTZControl(pResourceID, iPTZControlCode, iPTZControlValue);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, PTZControlCode:%d, PTZControlValue:%d", pResourceID, iPTZControlCode, iPTZControlValue);
	return iRet;
}

// 停止播放实时视频
ELTE_INT32 __SDK_CALL ELTE_SDK_StopRealPlay(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StopRealPlay(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 发起语音点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PDial(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PDial(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHalfDpxRelease(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PHalfDpxRelease(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 发起半双工点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHalfDpxDial(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PHalfDpxDial(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 接收语音点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PRecv(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PRecv(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 拒接语音点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PReject(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PReject(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 挂断语音点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHangup(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PHangup(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 调度台强拆点呼
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PBreakoff(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PBreakoff(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 抢话（强拆点呼+发起新点呼）
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PBreakin(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PBreakin(pResourceID);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

// 发起组呼（固定和临时组呼）或抢权
ELTE_INT32 __SDK_CALL ELTE_SDK_PTTDial(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().PTTDial(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 释放话权
ELTE_INT32 __SDK_CALL ELTE_SDK_PTTRelease(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().PTTRelease(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 退出组呼
ELTE_INT32 __SDK_CALL ELTE_SDK_PTTHangup(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().PTTHangup(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 发起紧急组呼
ELTE_INT32 __SDK_CALL ELTE_SDK_PTTEmergency(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().PTTEmergency(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 调度台强拆组呼
ELTE_INT32 __SDK_CALL ELTE_SDK_GroupBreakoff(const ELTE_CHAR* pGroupID)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pGroupID)
	{
		LOG_RUN_ERROR("GroupID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("GroupID:%s.", pGroupID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "GroupID:%s", pGroupID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().GroupBreakoff(pGroupID);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", pGroupID);
	return iRet;
}

// 调度台执行静音
ELTE_INT32 __SDK_CALL ELTE_SDK_VolMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pMuteParam)
	{
		LOG_RUN_ERROR("ResourceID or MuteParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, MuteParam:%s.", pResourceID, pMuteParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, MuteParam:%s", pResourceID, pMuteParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().VolMute(pResourceID, pMuteParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, MuteParam:%s", pResourceID, pMuteParam);
	return iRet;
}

// 调度台取消静音
ELTE_INT32 __SDK_CALL ELTE_SDK_VolUnMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pMuteParam)
	{
		LOG_RUN_ERROR("ResourceID or MuteParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, MuteParam:%s.", pResourceID, pMuteParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, MuteParam:%s", pResourceID, pMuteParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().VolUnMute(pResourceID, pMuteParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, MuteParam:%s", pResourceID, pMuteParam);
	return iRet;
}

// 订阅/去订阅GIS终端
ELTE_INT32 __SDK_CALL ELTE_SDK_GISSubscribe(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pGISParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pGISParam)
	{
		LOG_RUN_ERROR("ResourceID or GISParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, GISParam:%s.", pResourceID, pGISParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, GISParam:%s", pResourceID, pGISParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().GISSubscribe(pResourceID, pGISParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, GISParam:%s", pResourceID, pGISParam);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_SDSSendMessage(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSDSParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pSDSParam)
	{
		LOG_RUN_ERROR("ResourceID or SDSParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().SDSSendMessage(pResourceID, pSDSParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_StartVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pVideoDispatchParam)
	{
		LOG_RUN_ERROR("ResourceID or VideoDispatchParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, VideoDispatchParam:%s.", pResourceID, pVideoDispatchParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, VideoDispatchParam:%s", pResourceID, pVideoDispatchParam);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StartVideoDispatch(pResourceID, pVideoDispatchParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoDispatchParam:%s", pResourceID, pVideoDispatchParam);
	return iRet;
}

ELTE_INT32 __SDK_CALL ELTE_SDK_StopVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pVideoDispatchParam)
	{
		LOG_RUN_ERROR("ResourceID or VideoDispatchParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, VideoDispatchParam:%s.", pResourceID, pVideoDispatchParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, VideoDispatchParam:%s", pResourceID, pVideoDispatchParam);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StopVideoDispatch(pResourceID, pVideoDispatchParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoDispatchParam:%s", pResourceID, pVideoDispatchParam);
	return iRet;
}

// 开始视频上墙
ELTE_INT32 __SDK_CALL ELTE_SDK_VWallStart(const ELTE_CHAR* pVWallResID, const ELTE_CHAR* pVWallStartParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pVWallResID || NULL == pVWallStartParam)
	{
		LOG_RUN_ERROR("ResourceID or VideoParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, VideoParam:%s.", pVWallResID, pVWallStartParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, VideoParam:%s", pVWallResID, pVWallStartParam);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().VWallStart(pVWallResID, pVWallStartParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", pVWallResID, pVWallStartParam);
	return iRet;
}

// 获取视频上墙ID列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcVWallIDList(ELTE_CHAR** pVWallIDList)
{
	LOG_INTERFACE_TRACE();

	if (NULL == g_pUserMgr)
	{
		LOG_RUN_ERROR("g_pUserMgr is NULL");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetDcVWallIDList(pVWallIDList);
	LOG_INTERFACE_INFO(iRet, "VWallIDList:%s", NULL == *pVWallIDList ? "" : *pVWallIDList);
	return iRet;
}

//视频下墙
ELTE_INT32 __SDK_CALL ELTE_SDK_VWallStop(const ELTE_CHAR* pVWallResID, const ELTE_CHAR* pVWallStopParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pVWallResID || NULL == pVWallStopParam)
	{
		LOG_RUN_ERROR("ResourceID or VWallStopParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_VWallStop :ResourceID:%s, VWallStopParam:%s.", pVWallResID, pVWallStopParam);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, VWallStopParam:%s", pVWallResID, pVWallStopParam);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().VWallStop(pVWallResID, pVWallStopParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VWallStopParam:%s", pVWallResID, pVWallStopParam);
	return iRet;
}
/*
// 发起PSTN/PLMN电话呼叫
ELTE_INT32 __SDK_CALL ELTE_SDK_TelephoneDial(const ELTE_CHAR* pTelNumber)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pTelNumber)
	{
		LOG_RUN_ERROR("pTelNumber is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_TelephoneDial :TelNumber:%s.", pTelNumber);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "TelNumber:%s", pTelNumber);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().TelephoneDial(pTelNumber);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s", pTelNumber);
	return iRet;
}

//停止PSTN/PLMN电话呼叫
ELTE_INT32 __SDK_CALL ELTE_SDK_TelephoneHangup(const ELTE_CHAR* pTelNumber)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pTelNumber)
	{
		LOG_RUN_ERROR("pTelNumber is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_TelephoneHangup :TelNumber:%s.", pTelNumber);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "TelNumber:%s", pTelNumber);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().TelephoneHangup(pTelNumber);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s", pTelNumber);
	return iRet;
}
*/
//发起缜密侦听
ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartDiscreetListen(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID)
	{
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_StartDiscreetListen :pResourceID:%s.", pResourceID);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "pResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StartDiscreetListen(pResourceID);
	LOG_INTERFACE_INFO(iRet, "pResourceID:%s", pResourceID);
	return iRet;
}

//停止缜密侦听
ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StopDiscreetListen(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID)
	{
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_StopDiscreetListen :pResourceID:%s.", pResourceID);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "pResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StopDiscreetListen(pResourceID);
	LOG_INTERFACE_INFO(iRet, "pResourceID:%s", pResourceID);
	return iRet;
}

//发起环境侦听
ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartEnvironmentListen(const ELTE_CHAR* pResourceID)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID)
	{
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_StartEnvironmentListen :pResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "pResourceID:%s", pResourceID);
	ELTE_INT32 iRet = g_pUserMgr->GetMediaMgr().StartEnvironmentListen(pResourceID);
	LOG_INTERFACE_INFO(iRet, "pResourceID:%s", pResourceID);
	return iRet;
}

//发起人工转接
ELTE_INT32 __SDK_CALL ELTE_SDK_P2PTransfer(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pP2PTransferParam)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pResourceID || NULL == pP2PTransferParam)
	{
		LOG_RUN_ERROR("ResourceID or P2PTransferParam is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ELTE_SDK_P2PTransfer :ResourceID:%s, P2PTransferParam:%s.", pResourceID, pP2PTransferParam);
	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, P2PTransferParam:%s", pResourceID, pP2PTransferParam);
	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().P2PTransfer(pResourceID, pP2PTransferParam);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VWallStopParam:%s", pResourceID, pP2PTransferParam);
	return iRet;
}

//查询终端的GIS配置信息
ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserSpecificGISCfg(const ELTE_CHAR* pResourceID, ELTE_CHAR** pUeGisCfg)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetUserSpecificGISCfg(pResourceID, pUeGisCfg);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

//设置终端的GIS配置信息
ELTE_INT32 __SDK_CALL ELTE_SDK_SetGisParam(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUeGisCfg)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID || NULL == pUeGisCfg)
	{
		LOG_RUN_ERROR("ResourceID or UeGisCfg is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, UeGisCfg:%s.", pResourceID, pUeGisCfg);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, UeGisCfg:%s.", pResourceID, pUeGisCfg);

	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().SetGisParam(pResourceID, pUeGisCfg);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, UeGisCfg:%s.", pResourceID, pUeGisCfg);
	return iRet;
}

//获取调度台订阅GIS上报的终端列表
ELTE_INT32 __SDK_CALL ELTE_SDK_GetGisSubscription(const ELTE_CHAR* pResourceID, ELTE_CHAR** pGisQuerySubList)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID)
	{
		LOG_RUN_ERROR("ResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s.", pResourceID);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s", pResourceID);

	ELTE_INT32 iRet = g_pUserMgr->GetProvisionMgr().GetGisSubscription(pResourceID, pGisQuerySubList);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", pResourceID);
	return iRet;
}

//修改动态群组
ELTE_INT32 __SDK_CALL ELTE_SDK_ModifyDynamicGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pDynamicGroupInfo)
{
	LOG_INTERFACE_TRACE();
	if (NULL == pResourceID || NULL == pDynamicGroupInfo)
	{
		LOG_RUN_ERROR("ResourceID or DynamicGroupInfo is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("ResourceID:%s, DynamicGroupInfo:%s.", pResourceID, pDynamicGroupInfo);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, DynamicGroupInfo:%s", pResourceID, pDynamicGroupInfo);

	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().ModifyDynamicGroup(pResourceID, pDynamicGroupInfo);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, DynamicGroupInfo:%s", pResourceID, pDynamicGroupInfo);
	return iRet;
}

// ELTE_INT32 __SDK_CALL ELTE_SDK_TempUserJoinGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pPhonePatchParam)
// {
// 	LOG_INTERFACE_TRACE();
// 	if (NULL == pResourceID || NULL == pPhonePatchParam)
// 	{
// 		LOG_RUN_ERROR("ResourceID or PhonePatchParam is null.");
// 		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, ""); 
// 		return eLTE_SDK_ERR_INVALID_PARAM;
// 	}
// 	LOG_RUN_INFO("ResourceID:%s, PhonePatchParam:%s.", pResourceID, pPhonePatchParam);
// 
// 	CHECK_INTERFACE_POINTER(g_pUserMgr, "ResourceID:%s, PhonePatchParam:%s", pResourceID, pPhonePatchParam);
// 
// 	ELTE_INT32 iRet = g_pUserMgr->GetOperationMgr().TempUserJoinGroup(pResourceID, pPhonePatchParam);
// 	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, PhonePatchParam:%s", pResourceID, pPhonePatchParam);
// 	return iRet;
// }