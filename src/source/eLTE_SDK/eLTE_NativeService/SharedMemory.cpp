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
#include "SharedMemory.h"
#include "eLTE_Log.h"
#include "mutex_locker.h"
#include "eLTE_Error.h"

#define WAIT_STOP_THREAD_TIME 3*1000

SharedMemory::SharedMemory(const std::string& strFileName)
	: m_hFileHandle(NULL)
	, m_pBuffer(NULL)
	, m_MutexSend(NULL)
	, m_EventFull(NULL)
	, m_EventEmpty(NULL)
	, m_strFileName(strFileName)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_Thread(NULL)
	, m_ThreadId(0)
	, m_bRun(FALSE)
{
	m_MutexStreamQueue = CREATE_MUTEX();
}

int SharedMemory::InitResource()
{
	LOG_TRACE();
	std::string strTemp;
	strTemp.append("Global\\");
	strTemp.append(m_strFileName);
	m_hFileHandle = OpenFileMapping(FILE_MAP_WRITE, FALSE, strTemp.c_str());
	if(NULL == m_hFileHandle)
	{
		LOG_RUN_ERROR("OpenFileMapping failed, error:%d.", GetLastError());
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	m_pBuffer = MapViewOfFile(m_hFileHandle, FILE_MAP_WRITE, 0, 0, 0);
	if(NULL == m_pBuffer)
	{
		LOG_RUN_ERROR("MapViewOfFile failed, error:%d.", GetLastError());
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	m_MutexSend = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, strTemp.append("_mutex").c_str());
	if(NULL == m_MutexSend)
	{
		LOG_RUN_ERROR("OpenMutex failed, error:%d.", GetLastError());
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	m_EventFull = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, strTemp.append("_full_event").c_str());
	if(NULL == m_EventFull)
	{
		LOG_RUN_ERROR("OpenEventFull failed, error:%d.", GetLastError());
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	m_EventEmpty = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, strTemp.append("_empty_event").c_str());
	if(NULL == m_EventEmpty)
	{
		LOG_RUN_ERROR("OpenEventEmpty failed, error:%d.", GetLastError());
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	//make empty event has signal
	SetEvent(m_EventEmpty);

	int iRet = StartSendStream();
	if(eLTE_SVC_ERR_SUCCESS != iRet)
	{
		return iRet;
	}
	return eLTE_SVC_ERR_SUCCESS;
}


SharedMemory::~SharedMemory(void)
{
	try
	{
		if (m_pBuffer)
		{
			(void)UnmapViewOfFile(m_pBuffer);
			m_pBuffer = NULL;
		}
		if(m_hFileHandle)
		{
			CloseHandle(m_hFileHandle);
			m_hFileHandle = NULL;
		}
		if(m_MutexSend)
		{
			CloseHandle(m_MutexSend);
			m_MutexSend = NULL;
		}
		if(m_EventFull)
		{
			CloseHandle(m_EventFull);
			m_EventFull = NULL;
		}
		if(m_EventEmpty)
		{
			CloseHandle(m_EventEmpty);
			m_EventEmpty = NULL;
		}

		StopSendStream();
		ClearStreams();
		DESTROY_MUTEX(m_MutexStreamQueue);
	}
	catch (...)
	{
		
	}
}//lint !e1579

void SharedMemory::PushStream(VideoStream* pStream)
{
	if(NULL ==  pStream)
	{
		LOG_RUN_ERROR("Stream is null.");
	}
	MutexLocker Locker(m_MutexStreamQueue);
	m_streamQueue.push(pStream);
}

VideoStream* SharedMemory::PopStream()
{
	MutexLocker Locker(m_MutexStreamQueue);
	if(0 == m_streamQueue.size())
	{
		return NULL;
	}

	VideoStream* pStream = m_streamQueue.front();
	m_streamQueue.pop();
	return pStream;
}

void SharedMemory::ClearStreams()
{
	MutexLocker Locker(m_MutexStreamQueue);

	while(!m_streamQueue.empty())
	{
		VideoStream* pVideoStream = m_streamQueue.front();
		if(pVideoStream)
		{
			if (pVideoStream->pStream)
			{
				delete [] pVideoStream->pStream;
				pVideoStream->pStream = NULL;
			}
			delete pVideoStream;
			pVideoStream = NULL;
			m_streamQueue.pop();
		}
	}//lint !e438
}


unsigned __stdcall ThreadStreamFun(void* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	SharedMemory* pSharedMemory = (SharedMemory*) param;
	pSharedMemory->MainLoop();
	return eLTE_SDK_ERR_SUCCESS;
}

int SharedMemory::StartSendStream()
{
	LOG_TRACE();
	if(NULL != m_Thread)
	{
		CloseHandle(m_Thread);
		m_Thread = NULL;
		LOG_RUN_DEBUG("Stop send stream thread success, thread id[%d]", m_ThreadId);
	}

	m_bRun = TRUE;
	m_Thread = (HANDLE)_beginthreadex(NULL, 0, &ThreadStreamFun, (void*)this, 0, &m_ThreadId);
	if(NULL == m_Thread)
	{
		LOG_RUN_ERROR("Start send stream thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		LOG_RUN_DEBUG("Start send stream thread success, thread id[%d]", m_ThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

void SharedMemory::StopSendStream()
{
	LOG_TRACE();
	m_bRun = FALSE;
	if(NULL == m_Thread)
	{
		LOG_RUN_INFO("Stop send stream thread success, no thread is running");
		return;
	}

	unsigned long ulResult = ::WaitForSingleObject(m_Thread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_Thread);
	m_Thread = NULL;
	LOG_RUN_DEBUG("Stop send stream thread success, thread id[%d]", m_ThreadId);
	m_ThreadId = 0;
}

void SharedMemory::MainLoop()
{
	while(m_bRun)
	{
		if(0 != WaitForSingleObject(m_EventEmpty, 40))
		{
			// Ê±¼äÎ´µ½
			continue;
		}
		VideoStream* pVideoStream = PopStream();
		if(NULL == pVideoStream)
		{
			Sleep(40);
			SetEvent(m_EventEmpty);
			continue;
		}

		MutexLocker Locker(m_MutexSend);

		if(m_pBuffer)
		{
			// size is width + height + strides[0] + strides[1] + strides[2] + the frame's size
			eSDK_MEMCPY(m_pBuffer, SHARE_MEMORY_SIZE, pVideoStream->pStream, pVideoStream->uiSize);
		}

		SetEvent(m_EventFull);
		if (pVideoStream->pStream)
		{
			delete [] pVideoStream->pStream;
			pVideoStream->pStream = NULL;
		}
		delete pVideoStream;
		pVideoStream = NULL;
	}//lint !e438
}
