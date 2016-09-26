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
#include <atlstr.h>
#include "SessionMgr.h"
#include "eLTE_Message.h"
#include "eLTE_Error.h"
#include "eLTE_Log.h"
#include "SSL_Socket.h"


#pragma comment(lib, "Rpcrt4.lib")


SessionMgr::SessionMgr(void)
	: m_strSessionID("")
	, m_hKeepAliveThread(NULL)
	, m_hMutex(NULL)
	, m_hStopEvent(NULL)
	, m_uiTimeOutCount(0)
{
}


SessionMgr::~SessionMgr(void)
{
	try
	{
		m_strSessionID = "";
		KeepAliveStop();
	}
	catch(...){}
}//lint !e1579

SessionMgr& SessionMgr::Instance()
{
	static SessionMgr s_SessionMgr;
	return s_SessionMgr;
}

int SessionMgr::NewSessionID()
{
	LOG_TRACE();
	GUID guid;
	RPC_STATUS rpc = ::UuidCreate(&guid);
	if (RPC_S_OK != rpc)
	{
		LOG_RUN_ERROR("UuidCreate failed.(%d)", rpc);
		return eLTE_SVC_ERR_NEW_SESSIONID;
	}

	// Guid格式：%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X,
	//				guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]
	// 这里取guid后18位
	char strGuid[32+1] = {0};
	(void)_snprintf_s(strGuid,32+1,_TRUNCATE,"%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);

	char strSessionID[SESSIONID_LENGTH+1] = {0};
	eSDK_MEMCPY(strSessionID, sizeof(strSessionID), strGuid+14, 18);

	// 获取时间字符串
	SYSTEMTIME time;
	GetLocalTime(&time);
	char strTime[14+1] = {0};
	(void)_snprintf_s(strTime,14+1,_TRUNCATE,"%04d%02d%02d%02d%02d%02d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);

	eSDK_MEMCPY(strSessionID+18, sizeof(strSessionID), strTime, 14);

	m_strSessionID = strSessionID;
	LOG_RUN_DEBUG("SessionID:%s", strSessionID);
	return eLTE_SVC_ERR_SUCCESS;
}

const std::string& SessionMgr::GetSessionID() const
{
	return m_strSessionID;
}

void SessionMgr::ClearSessionID()
{
	m_strSessionID = "";
}

// 保活开始
int SessionMgr::KeepAliveStart()
{
	//LOG_TRACE();
	if (NULL != m_hKeepAliveThread)
	{
		SetEvent(m_hStopEvent);
		DWORD dwRet = WaitForSingleObject(m_hKeepAliveThread, 500);
		if (WAIT_TIMEOUT == dwRet)
		{
			LOG_RUN_ERROR("WaitForSingleObject keep alive stop failed.");
			TerminateThread(m_hKeepAliveThread, 0);
		}
		m_hKeepAliveThread = NULL;
		m_uiTimeOutCount = 0;
	}

	// 创建保活线程锁
	if (NULL == m_hMutex)
	{
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
		if (NULL == m_hMutex)
		{
			LOG_RUN_ERROR("CreateMutex failed.");
			return eLTE_SVC_ERR_FAILED;
		}
	}

	// 创建保活关闭信号
	if (NULL == m_hStopEvent)
	{
		m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (NULL == m_hStopEvent)
		{
			LOG_RUN_ERROR("CreateEvent failed.");
			return eLTE_SVC_ERR_FAILED;
		}
	}

	// 创建保活线程
	m_hKeepAliveThread = (HANDLE)_beginthreadex(NULL, 0, KeepAliveThread, NULL, CREATE_SUSPENDED, NULL);
	if (NULL == m_hKeepAliveThread)
	{
		LOG_RUN_ERROR("Create keep alive thread failed.");
		return eLTE_SVC_ERR_FAILED;
	}
	// 启动保活线程
	ResumeThread(m_hKeepAliveThread);

	return eLTE_SVC_ERR_SUCCESS;
}

// 保活刷新
void SessionMgr::KeepAliveFreshen()
{
	LOG_TRACE();
	if (NULL != m_hMutex)
	{
		(void)WaitForSingleObject(Instance().m_hMutex, INFINITE);
		m_uiTimeOutCount = 0;
		ReleaseMutex(Instance().m_hMutex);
	}
}

// 保活结束
void SessionMgr::KeepAliveStop()
{
	LOG_TRACE();
	// 关闭线程
	if (NULL != m_hKeepAliveThread)
	{
		SetEvent(m_hStopEvent);
		DWORD dwRet = WaitForSingleObject(m_hKeepAliveThread, 500);
		if (WAIT_TIMEOUT == dwRet)
		{
			LOG_RUN_ERROR("WaitForSingleObject keep alive stop failed.");
			TerminateThread(m_hKeepAliveThread, 0);
		}
		m_hKeepAliveThread = NULL;
		m_uiTimeOutCount = 0;
	}
	
	// 关闭线程锁
	if (NULL != m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	// 关闭信号
	if (NULL != m_hStopEvent)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
}

unsigned __stdcall SessionMgr::KeepAliveThread(void* param)
{
	LOG_TRACE();
	while (WAIT_TIMEOUT == WaitForSingleObject(Instance().m_hStopEvent, KEEPALIVE_TIME*1000))
	{
		(void)WaitForSingleObject(Instance().m_hMutex, INFINITE);
		if (++Instance().m_uiTimeOutCount > MAX_TIMEOUT_COUNT)
		{
			// 保活超时，关闭服务
			LOG_RUN_ERROR("KeepAlive timeout. begin to stop service.");
			SSL_Socket::Instance().m_bStopSvc = true;
			ReleaseMutex(Instance().m_hMutex);
			break;
		}
		ReleaseMutex(Instance().m_hMutex);
	}

	return 0;
}//lint !e818
