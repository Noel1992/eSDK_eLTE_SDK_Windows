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
#include "EventMgr.h"
#include "eLTE_Log.h"
#include <process.h>

#define MAX_EVENT_QUEUE_LEN    4096

#define WAIT_EVENT_PROCESS_TIME 3*1000

CEventMgr::CEventMgr()
{
	m_pUserData = NULL;
	m_fnEventCallBack = NULL;
	m_MutexEventQueue = CREATE_MUTEX();
	m_bRun = FALSE;
	m_Thread = NULL;
	m_ThreadId = 0;
}

CEventMgr::~CEventMgr()
{
	try
	{
		m_pUserData = NULL;
		m_fnEventCallBack = NULL;
		StopEventProcess();
		ClearEvents();
		DESTROY_MUTEX(m_MutexEventQueue);
	}
	catch (...)
	{	

	}
}//lint !e1579

CEventMgr& CEventMgr::Instance()
{ 
	static CEventMgr _instance; 
	return _instance; 
}

ELTE_VOID CEventMgr::SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	m_fnEventCallBack = fEventCallBack;
	m_pUserData = pUserData;
}

ELTE_INT32 CEventMgr::PushEvent(ELTE_INT32 iEventType, const ELTE_VOID* pEventData, ELTE_UINT32 uiBufSize)
{
	MutexLocker Locker(m_MutexEventQueue);

	EVENT_DATA* pstEventData = new EVENT_DATA;
	if(NULL == pstEventData)
	{
		LOG_RUN_ERROR("New eventdata failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	eSDK_MEMSET(pstEventData, 0x0, sizeof(EVENT_DATA));
	pstEventData->iEventType = iEventType;

	if(NULL != pEventData)
	{
		ELTE_CHAR* pEventBuf = new ELTE_CHAR[uiBufSize + 1];
		if(NULL == pEventBuf)
		{
			delete pstEventData;
			LOG_RUN_ERROR("New event data buffer failed.");
			return eLTE_SDK_ERR_FAILED;
		}
		eSDK_MEMSET(pEventBuf, 0x00, uiBufSize + 1);
		eSDK_MEMCPY(pEventBuf, uiBufSize + 1, pEventData, uiBufSize);
		pstEventData->pEventData = (ELTE_VOID*)pEventBuf;
		pstEventData->uiBufSize = uiBufSize + 1;
	}
	if(MAX_EVENT_QUEUE_LEN <= m_eventQueue.size())
	{
		LOG_RUN_DEBUG("Event queue is over max size[%d], front event will be discarded", MAX_EVENT_QUEUE_LEN);
		EVENT_DATA* pstEventFront = m_eventQueue.front();
		if(NULL != pstEventFront && NULL != pstEventFront->pEventData)
		{
			delete [] ((ELTE_CHAR*)(pstEventFront->pEventData));
			pstEventFront->pEventData = NULL;

			delete pstEventFront;
			pstEventFront = NULL;
		}//lint !e438
		m_eventQueue.pop();
	}//lint !e438

	m_eventQueue.push(pstEventData);
	return eLTE_SDK_ERR_SUCCESS;//lint !e429
}

EVENT_DATA* CEventMgr::PopEvent()
{
	MutexLocker Locker(m_MutexEventQueue);
	if(0 == m_eventQueue.size())
	{
		return NULL;
	}

	EVENT_DATA* pstEventData = m_eventQueue.front();
	m_eventQueue.pop();
	return pstEventData;
}

ELTE_VOID CEventMgr::ClearEvents()
{
	MutexLocker Locker(m_MutexEventQueue);
	
	while(!m_eventQueue.empty())
	{
		EVENT_DATA* pstEventFront = m_eventQueue.front();
		
		if(NULL != pstEventFront)
		{	
			ELTE_CHAR* pTmp = (ELTE_CHAR*)(pstEventFront->pEventData);
			if(NULL != pTmp)
			{
				delete [] pTmp;
				pTmp = NULL;
			}
			delete pstEventFront;
			pstEventFront = NULL;
		}//lint !e438
		m_eventQueue.pop();
	}//lint !e438
}

ELTE_UINT32 __stdcall ThreadFun(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CEventMgr* pEventMgr = (CEventMgr*) param;
	pEventMgr->MainLoop();
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID CEventMgr::MainLoop()
{
	while(m_bRun)
	{
		EVENT_DATA* pstEvent = PopEvent();
		if(NULL == pstEvent)
		{
			Sleep(100);
			continue;
		}
		else if(NULL != m_fnEventCallBack)
		{
			m_fnEventCallBack(pstEvent->iEventType, pstEvent->pEventData, pstEvent->uiBufSize, m_pUserData);
		}
		if(NULL != pstEvent && NULL != pstEvent->pEventData)
		{
			delete [] ((ELTE_CHAR*)(pstEvent->pEventData));
			pstEvent->pEventData = NULL;

			delete pstEvent;
			pstEvent = NULL;
		}
	}//lint !e438
	m_Thread = NULL;
}

ELTE_INT32 CEventMgr::StartEventProcess()
{
	LOG_TRACE();
	if(NULL != m_Thread)
	{
		CloseHandle(m_Thread);
		m_Thread = NULL;
		LOG_RUN_DEBUG("Stop msg process thread success, thread id[%d]", m_ThreadId);
	}

	m_bRun = TRUE;
	m_Thread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFun, (ELTE_VOID*)this, 0, &m_ThreadId);
	if(NULL == m_Thread)
	{
		LOG_RUN_ERROR("Start event process thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		LOG_RUN_DEBUG("Start event process thread success, thread id[%d]", m_ThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_VOID CEventMgr::StopEventProcess()
{
	LOG_TRACE();
	m_bRun = FALSE;
	if(NULL == m_Thread)
	{
		LOG_RUN_INFO("Stop event process thread success, no thread is running");
		return;
	}
	
	ELTE_ULONG ulResult = ::WaitForSingleObject(m_Thread, WAIT_EVENT_PROCESS_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_Thread);
	m_Thread = NULL;
	LOG_RUN_DEBUG("Stop event process thread success, thread id[%d]", m_ThreadId);
	m_ThreadId = 0;
}
