/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
/********************************************************************
filename		: 	eLTE_RM_SDK.cpp
author			:	zWX229156
created			:	2015/10/12	
description		:	定义 DLL 应用程序的导出函数
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/12 初始版本
*********************************************************************/

#include "stdafx.h"
#include "eLTE_RM_SDK.h"
//Error Code define class
#include "eLTE_Error.h"
//public function interface class 
#include "eLTE_Tool.h"
//log manage class 
#include "eLTE_Log.h"
//type define
#include "eLTE_Types.h"
//user manage class 
#include "UserMgr.h"


// SDK版本号
#define eLTE_SDK_VERSION		"1.5.50.0"
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
#pragma comment(linker, "/EXPORT:ELTE_SDK_Init=_ELTE_SDK_Init@0")
#pragma comment(linker, "/EXPORT:ELTE_SDK_Login=_ELTE_SDK_Login@20")

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
ELTE_INT32 __SDK_CALL ELTE_SDK_ReleaseBuffer(ELTE_CHAR* pBuffer)	//lint !e438
{
	LOG_INTERFACE_TRACE();
	if(NULL != pBuffer)
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}	
	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "");
	return eLTE_SDK_ERR_SUCCESS;	//lint !e438
}

// 获取SDK版本号
ELTE_INT32 __SDK_CALL ELTE_SDK_GetVersion(ELTE_CHAR** pVersion)
{
	LOG_INTERFACE_TRACE();

	ELTE_UINT32 iLength = strlen(eLTE_SDK_VERSION);
	*pVersion = new ELTE_CHAR[iLength + 1];
	CHECK_INTERFACE_POINTER(*pVersion, "");
	eSDK_MEMSET(*pVersion , 0x0, (iLength + 1));
	eSDK_MEMCPY(*pVersion, iLength+1, eLTE_SDK_VERSION, iLength);

	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "Version:%s", *pVersion);
	return eLTE_SDK_ERR_SUCCESS;
}

// 初始化SDK
ELTE_INT32 __SDK_CALL ELTE_SDK_Init()
{
	//打开日志
	MutexLocker Locker(CREATE_MUTEX());
	std::string strIniPath = eLTE_Tool::GetDllPath(ELTE_RM_SDK_DLL_NAME);
	strIniPath.append(LOG_INI_FILE_NAME);
	ELTE_UINT32 logLevel[LOG_CATEGORY];
	for (ELTE_UINT32 i = 0; i < LOG_CATEGORY; ++i)
	{
		logLevel[i] = g_uiLogLevel;
	}

	ELTE_INT32 iRet = LOG_INIT(strIniPath.c_str(), logLevel, g_strLogPath.c_str());
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		return eLTE_SDK_ERR_LOG_INIT;
	}

	LOG_INTERFACE_TRACE();
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
	
	//初始化socket
	SSL_Socket& sslSocket = const_cast<SSL_Socket&>(g_pUserMgr->GetSSLSocket());
	iRet = sslSocket.Init_SSL_Socket();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init ssl socket failed.");
		return iRet;
	}

	LOG_INTERFACE_INFO(iRet, "");
	return iRet;
}

// 登录
ELTE_INT32 __SDK_CALL ELTE_SDK_Login(const ELTE_CHAR* pUserID, const ELTE_CHAR* pPWD, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, ELTE_UINT32 uiServerSIPPort)
{
	LOG_INTERFACE_TRACE();
	if(NULL == pUserID || NULL == pPWD || NULL == pServerIP || NULL == pLocalIP)
	{
		LOG_RUN_ERROR("UserID or PWD or ServerIP or LocalIP is null.");
		LOG_INTERFACE_INFO(eLTE_SDK_ERR_INVALID_PARAM, "");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}
	LOG_RUN_INFO("UserID:%s, ServerIP:%s, LocalIP:%s, ServerSIPPort:%d.", pUserID, pServerIP, pLocalIP, uiServerSIPPort);

	CHECK_INTERFACE_POINTER(g_pUserMgr, "UserID:%s, ServerIP:%s, LocalIP:%s, ServerSIPPort:%d", pUserID, pServerIP, pLocalIP, uiServerSIPPort);
	g_pUserMgr->SetUserID(pUserID);
	g_pUserMgr->SetUserPwd(pPWD);
	g_pUserMgr->SetLocalIP(pLocalIP);
	g_pUserMgr->SetServerIP(pServerIP);
	g_pUserMgr->SetServerSIPPort(uiServerSIPPort);
	ELTE_INT32 iRet = g_pUserMgr->GetLoginMgr().Login(pUserID, pPWD, pServerIP, pLocalIP, uiServerSIPPort);
	LOG_INTERFACE_INFO(iRet, "UserID:%s, ServerIP:%s, LocalIP:%s, ServerSIPPort:%d", pUserID, pServerIP, pLocalIP, uiServerSIPPort);
	return iRet;
}

// 登出
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

// 设置消息回调函数
ELTE_INT32 __SDK_CALL ELTE_SDK_SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	LOG_INTERFACE_TRACE();
	CHECK_INTERFACE_POINTER(g_pUserMgr, "");
	g_pUserMgr->SetEventCallBack(fEventCallBack, pUserData);
	LOG_INTERFACE_INFO(eLTE_SDK_ERR_SUCCESS, "");
	return eLTE_SDK_ERR_SUCCESS;
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

// 释放SDK
ELTE_INT32 __SDK_CALL ELTE_SDK_Cleanup()
{
	LOG_INTERFACE_TRACE();
	ELTE_INT32 iRet = eLTE_SDK_ERR_SUCCESS;
	if (NULL != g_pUserMgr)
	{
		//断开连接
		SSL_Socket& socket = const_cast<SSL_Socket&>(g_pUserMgr->GetSSLSocket());
		iRet = socket.Uninit_SSL_Socket();
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Uninit SSL Socket failed");
			return iRet;
		}
		delete g_pUserMgr;
		g_pUserMgr = NULL;
	}

	LOG_INTERFACE_INFO(iRet, "");
	//关闭日志
	LOG_EXIT();
	return iRet;
}