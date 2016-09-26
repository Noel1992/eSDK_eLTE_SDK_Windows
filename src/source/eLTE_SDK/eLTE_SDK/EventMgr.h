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
filename		: 	EventMgr.h
author			:	zWX229156
created		    :	2015/1/12
description	    :	事件管理类
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/12 初始版本
*********************************************************************/

#ifndef __EVENT_MGR_H__
#define __EVENT_MGR_H__

#include "eLTE_Types.h"
#include "eLTE_Error.h"
#include "mutex_locker.h"
#include <queue>

typedef struct
{
	ELTE_INT32 iEventType;
	ELTE_UINT32 uiBufSize;
	ELTE_VOID* pEventData;
}EVENT_DATA;

class CUserMgr;//lint !e763
class CEventMgr
{
public:
	CEventMgr();
	~CEventMgr();

	//获取CEventMgr静态实例
	static CEventMgr& Instance();

	//设置回调函数
	ELTE_VOID SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData);

	//事件入列
	ELTE_INT32 PushEvent(ELTE_INT32 iEventType, const ELTE_VOID* pEventData, ELTE_UINT32 uiBufSize);

	//开始事件处理线程
	ELTE_INT32 StartEventProcess();

	//停止事件处理线程
	ELTE_VOID StopEventProcess();

	//回调处理主循环
	ELTE_VOID MainLoop();

	//清除所有事件
	ELTE_VOID ClearEvents();

private:
	//事件出列
	EVENT_DATA* PopEvent();

private:
	ELTE_VOID* m_pUserData;
	EventCallBack m_fnEventCallBack;
	MUTEX_HANDLE m_MutexEventQueue;
	std::queue<EVENT_DATA*> m_eventQueue;

	HANDLE m_Thread;
	ELTE_UINT32 m_ThreadId;
	ELTE_INT32 m_bRun;
};

#endif // __EVENT_MGR_H__
