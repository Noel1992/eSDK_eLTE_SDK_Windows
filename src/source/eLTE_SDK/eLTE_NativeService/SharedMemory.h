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
description :    eLTE SDK�ڴ湲�������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 ��ʼ�汾
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
	//��ʼ����Դ�������ڴ湲�����Լ��¼�
	int InitResource();

	//��Ƶ��������
	void PushStream(VideoStream* pStream);

	//��Ƶ��������ѭ��
	void MainLoop();
	//��ʼ����Ƶ��
	int StartSendStream();

private:
	//ֹͣ����Ƶ��
	void StopSendStream();

	//�¼�����
	VideoStream* PopStream();

	//���������Ƶ������
	void ClearStreams();

	//���������߳�
	static unsigned __stdcall SendMsgThread(void* param);

private:
	HANDLE         m_hFileHandle;
	void*          m_pBuffer;
	HANDLE         m_MutexSend;
	HANDLE         m_EventFull;
	HANDLE         m_EventEmpty;

	std::string    m_strFileName;

	//��Ƶ���
	int m_iWidth;
	int m_iHeight;

	//����Ƶ��
	HANDLE m_Thread;
	unsigned m_ThreadId;
	int m_bRun;

	MUTEX_HANDLE   m_MutexStreamQueue;
	std::queue<VideoStream*> m_streamQueue;
};
#endif
