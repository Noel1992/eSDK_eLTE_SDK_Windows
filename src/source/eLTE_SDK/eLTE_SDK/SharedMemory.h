/********************************************************************
filename    :    SharedMemory.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK内存共享客户类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 初始版本
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
	//创建接受数据线程
	ELTE_INT32 StartRevMsg();
	//停止接受数据线程
	ELTE_INT32 StopRevMsg();
	//初始化资源，打开内存共享、锁以及事件
	ELTE_INT32 InitResource();
	//视频流处理主循环
	ELTE_VOID MainLoop();
	//开始视频流处理线程
	ELTE_INT32 StartYuvFrameProcess();
	//设置视频流播放窗口
	ELTE_VOID SetPlayWindow(HWND hWnd);
	//设置播放窗口大小
	ELTE_VOID SetPlayWindowSize(ULONG ulWidth, ULONG ulHeight);
	//抓拍图片
	ELTE_INT32 LocalSnapshot(const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat);

	ELTE_INT32 Snapshot();

	ELTE_INT32 ConvertYUV420ToRGB(ELTE_UCHAR* pData);

private:

	//停止视频流处理线程
	ELTE_VOID StopYuvFrameProcess();

	//清除所有视频流数据
	ELTE_VOID ClearYuvFrames();

	ELTE_VOID DoRevMsg();
	//接受数据线程
	static ELTE_UINT32 __stdcall RevMsgThread(ELTE_VOID* param);

private:
	//事件出列
	YuvFrame* PopYuvFrame();
	//事件入列
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
	
	//取视频流
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
