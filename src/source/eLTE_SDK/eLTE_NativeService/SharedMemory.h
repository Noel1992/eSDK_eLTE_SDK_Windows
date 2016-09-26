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
filename    :    SharedMemory.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK内存共享服务类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 初始版本
*********************************************************************/

#ifndef __ELTE_SHARED_MEMORY_H__
#define __ELTE_SHARED_MEMORY_H__

#include "mediamanager.h"
#include "mutex_locker.h"
#include <queue>

#define SHARE_MEMORY_SIZE 5242880 //5*1024*1024

typedef struct _VideoStream{
	int w,h;
	unsigned char *pStream;
	unsigned uiSize;
}VideoStream;

class SharedMemory
{
public:
	SharedMemory(const std::string& strFileName);
	virtual ~SharedMemory(void);
private:
	SharedMemory();
public:
	//初始化资源，创建内存共享、锁以及事件
	int InitResource();

	//视频数据入列
	void PushStream(VideoStream* pStream);

	//视频流处理主循环
	void MainLoop();
	//开始发视频流
	int StartSendStream();

private:
	//停止发视频流
	void StopSendStream();

	//事件出列
	VideoStream* PopStream();

	//清除所有视频流数据
	void ClearStreams();

	//接受数据线程
	static unsigned __stdcall SendMsgThread(void* param);

private:
	HANDLE         m_hFileHandle;
	void*          m_pBuffer;
	HANDLE         m_MutexSend;
	HANDLE         m_EventFull;
	HANDLE         m_EventEmpty;

	std::string    m_strFileName;

	//视频宽高
	int m_iWidth;
	int m_iHeight;

	//发视频流
	HANDLE m_Thread;
	unsigned m_ThreadId;
	int m_bRun;

	MUTEX_HANDLE   m_MutexStreamQueue;
	std::queue<VideoStream*> m_streamQueue;
};
#endif
