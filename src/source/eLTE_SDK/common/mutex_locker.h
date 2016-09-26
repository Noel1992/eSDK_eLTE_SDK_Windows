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
filename		: 	mutex_locker.h
author			:	zWX229156
created		    :	2015/1/8
description		:	互斥类定义
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/8 初始版本
*********************************************************************/

#ifndef _MUTEX_LOCKER_H_INCLUDED_
#define _MUTEX_LOCKER_H_INCLUDED_

#ifdef _WIN32
typedef HANDLE MUTEX_HANDLE;
#define CREATE_MUTEX() ::CreateMutex(NULL, FALSE, NULL)
#define DESTROY_MUTEX(mute) do { ::CloseHandle(mute); mute=NULL; } while(0)
#define DECLARE_MUTEX(mute) (MUTEX_HANDLE mute = NULL)
#define DECLARE_MUTEX_REF(mute) MUTEX_HANDLE mute

class MutexLocker
{
private:
	MutexLocker()
	{
		m_bLocked = false;
		m_mutex = NULL;
	}
public:
	MutexLocker(HANDLE mutex, DWORD dwWaitTime = INFINITE) : m_mutex(mutex)
	{
		if(NULL != m_mutex && 0 == ::WaitForSingleObject(m_mutex, dwWaitTime))
		{
			m_bLocked = true;
		}
		else
		{
			m_bLocked = false;
		}
	}
	~MutexLocker()
	{
		if(m_bLocked)
		{
			::ReleaseMutex(m_mutex);
		}
		m_bLocked = false;
		m_mutex = NULL;
	}

	bool isLocked() const
	{
		return m_bLocked; 
	}
private:
	HANDLE m_mutex;
	bool   m_bLocked;
};
#else
#ifndef uint32_t
#include <stdint.h>
#endif
#ifndef u_int32_t
typedef uint32_t u_int32_t
#endif

#include <pthread.h>
;
typedef pthread_mutex_t MUTEX_HANDLE;
inline MUTEX_HANDLE pthread_mutex_null()
{
    MUTEX_HANDLE mute;
    eSDK_eSDK_MEMSET(&mute, 0, sizeof(mute));
    return mute;
}
#define DECLARE_MUTEX(mute) MUTEX_HANDLE mute = pthread_mutex_null()
#define DECLARE_MUTEX_REF(mute) MUTEX_HANDLE & mute

inline MUTEX_HANDLE CREATE_MUTEX() 
{
    MUTEX_HANDLE mutex;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    return mutex;
}
#define DESTROY_MUTEX(mute) do { ::pthread_mutex_destroy(&mute); } while(0)
#define INFINITE 0xFFFFFFFFU
class MutexLocker
{
public:
	MutexLocker(MUTEX_HANDLE &mutex, u_int32_t dwWaitTime = INFINITE) : m_mutex(mutex)
	{
		if(INFINITE == dwWaitTime)
		{
			m_bLocked = pthread_mutex_lock(&m_mutex) ? false : true;
		}
		else
		{
			struct timespec abstime;
			abstime.tv_sec = dwWaitTime / 1000;
			abstime.tv_nsec = ( dwWaitTime % 1000 ) * 1000 * 1000;
			m_bLocked = pthread_mutex_timedlock(&m_mutex, &abstime) ? false : true;
		}
	}
	~MutexLocker()
	{
		if(m_bLocked)
		{
			pthread_mutex_unlock(&m_mutex);
			m_bLocked = false;
		}
	}

	bool isLocked()
	{
		return m_bLocked; 
	}
private:
	MUTEX_HANDLE & m_mutex;
	bool   m_bLocked;
};

#endif

#endif
