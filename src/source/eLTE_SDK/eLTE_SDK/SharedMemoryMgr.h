/********************************************************************
filename    :    SharedMemoryMgr.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK内存共享客户管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 初始版本
*********************************************************************/

#ifndef __ELTE_SHARED_MEMORY_MGR_H__
#define __ELTE_SHARED_MEMORY_MGR_H__

#include "eLTE_Types.h"
#include <map>

class SharedMemory;

class SharedMemoryMgr
{
public:
	virtual ~SharedMemoryMgr();
	static SharedMemoryMgr& Instance();
private:
	SharedMemoryMgr();
public:
	ELTE_INT32 CreateSharedMemory(const std::string& strResId);
	ELTE_VOID DeleteSharedMemory(const std::string& strResId);
	ELTE_INT32 SetPlayWindow(const std::string& strResId, HWND hWnd);
	//设置视频数据回调函数和用户数据 
	ELTE_VOID SetYUVFrameCallBack(YUVFrameCallBack fYUVFrameCallBack = NULL, ELTE_VOID* pUserData = NULL);
	YUVFrameCallBack GetYUVFrameCallBack()const {return m_fnYUVFrameCallBack;};
	ELTE_VOID* GetUserData(){return m_pYUVFrameUserData;};
	ELTE_INT32 SetPlayWindowSize(const std::string& strResId, ULONG ulWidth, ULONG ulHeight);
	ELTE_INT32 LocalSnapshot(const std::string& strResourceID, const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat);
private:
	std::map<std::string, SharedMemory*> m_sharedMemorys;
	YUVFrameCallBack m_fnYUVFrameCallBack;
	ELTE_VOID*    m_pYUVFrameUserData;
};
#endif
