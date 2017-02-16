/********************************************************************
filename    :    SharedMemory.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK�ڴ湲��ͻ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_SHARED_MEMORY_H__
#define __ELTE_SHARED_MEMORY_H__
#include <string>
#include <queue>
#include "eLTE_Types.h"
#include "mutex_locker.h"
#include "MediaPlayer.h"

#define SHARE_MEMORY_SIZE 5242880 //5*1024*1024

#define FREE_ARRAY(pArray) \
	delete [] pArray;		\
	pArray = NULL;		\
	if (NULL == pArray){} \

#define RELEASE_RESOURCE(postStream,hBmpGlobal,pRGBData) \
	postStream->Release();	\
	postStream = NULL;		\
	GlobalFree(hBmpGlobal);	\
	hBmpGlobal = NULL;		\
	delete [] pRGBData;		\
	pRGBData = NULL;		\
	if (NULL == postStream && NULL == hBmpGlobal && NULL == pRGBData){} \

class SharedMemory
{
public:
	SharedMemory(const std::string& strFileName);
	virtual ~SharedMemory();
private:
	SharedMemory();
public:
	//�������������߳�
	ELTE_INT32 StartRevMsg();
	//ֹͣ���������߳�
	ELTE_INT32 StopRevMsg();
	//��ʼ����Դ�����ڴ湲�����Լ��¼�
	ELTE_INT32 InitResource();
	//��Ƶ��������ѭ��
	ELTE_VOID MainLoop();
	//��ʼ��Ƶ�������߳�
	ELTE_INT32 StartYuvFrameProcess();
	//������Ƶ�����Ŵ���
	ELTE_VOID SetPlayWindow(HWND hWnd);
	//���ò��Ŵ��ڴ�С
	ELTE_VOID SetPlayWindowSize(ULONG ulWidth, ULONG ulHeight);
	//ץ��ͼƬ
	ELTE_INT32 LocalSnapshot(const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat);

	ELTE_INT32 Snapshot();

	ELTE_INT32 ConvertYUV420ToRGB(ELTE_UCHAR* pData);

private:

	//ֹͣ��Ƶ�������߳�
	ELTE_VOID StopYuvFrameProcess();

	//���������Ƶ������
	ELTE_VOID ClearYuvFrames();

	ELTE_VOID DoRevMsg();
	//���������߳�
	static ELTE_UINT32 __stdcall RevMsgThread(ELTE_VOID* param);

private:
	//�¼�����
	YuvFrame* PopYuvFrame();
	//�¼�����
	ELTE_VOID PushYuvFrame(YuvFrame* pYuvFrame);
	ELTE_VOID DeleteYuvFrame(YuvFrame* pYuvFrame) const;

private:
	HANDLE         m_hFileHandle;
	ELTE_VOID*     m_pBuffer;
	HANDLE         m_MutexRev;
	HANDLE         m_EventFull;
	HANDLE         m_EventEmpty;
	std::string    m_strFileName;

	HANDLE         m_revThread;
	ELTE_UINT32    m_revThreadId;
	ELTE_INT32     m_bStopRevThtread;
	
	//ȡ��Ƶ��
	HANDLE m_Thread;
	ELTE_UINT32 m_ThreadId;
	ELTE_INT32 m_bRun;

	MUTEX_HANDLE   m_MutexYuvFrameQueue;
	MUTEX_HANDLE   m_MutexCurYuvFrame;
	std::queue<YuvFrame*> m_yuvFrameQueue;

	MediaPlayer    m_mediaPlayer;
	YuvFrame*      m_pCurrentYuvFrame;
};

#endif
