#include "StdAfx.h"
#include "SharedMemoryMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "SharedMemory.h"

SharedMemoryMgr::~SharedMemoryMgr()
{
	m_fnYUVFrameCallBack = NULL;
	m_pYUVFrameUserData = NULL;
}

SharedMemoryMgr& SharedMemoryMgr::Instance()
{
	static SharedMemoryMgr _instance; 
	return _instance; 
}

SharedMemoryMgr::SharedMemoryMgr()
{
	m_fnYUVFrameCallBack = NULL;
	m_pYUVFrameUserData = NULL;
}

ELTE_VOID SharedMemoryMgr::SetYUVFrameCallBack(YUVFrameCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	m_fnYUVFrameCallBack = fEventCallBack;
	m_pYUVFrameUserData = pUserData;
}

ELTE_INT32 SharedMemoryMgr::CreateSharedMemory(const std::string& strResId)
{
	LOG_TRACE();
	INFO_PARAM1(strResId);

	std::map <std::string, SharedMemory*>::iterator iter = m_sharedMemorys.find(strResId);
	if(m_sharedMemorys.end() != iter)
	{
		return eLTE_SDK_ERR_SUCCESS;
	}
	SharedMemory* pSharedMemory = new SharedMemory(strResId);
	if (NULL == pSharedMemory)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	//初始化资源
	ELTE_INT32 iRet = pSharedMemory->InitResource();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		delete pSharedMemory;
		pSharedMemory = NULL;
		return iRet;//lint !e438
	}

	//开启接受视频流线程
	iRet = pSharedMemory->StartYuvFrameProcess();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		delete pSharedMemory;
		pSharedMemory = NULL;
		return iRet;//lint !e438
	}

	//开启发送视频流线程
	iRet = pSharedMemory->StartRevMsg();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		delete pSharedMemory;
		pSharedMemory = NULL;
		return iRet;//lint !e438
	}

	(ELTE_VOID)m_sharedMemorys.insert(std::pair <std::string, SharedMemory*> (strResId, pSharedMemory));

	return eLTE_SDK_ERR_SUCCESS;//lint !e429
}

ELTE_VOID SharedMemoryMgr::DeleteSharedMemory(const std::string& strResId)
{
	LOG_TRACE();
	INFO_PARAM1(strResId);

	std::map <std::string, SharedMemory*>::iterator iter = m_sharedMemorys.find(strResId);
	if(iter == m_sharedMemorys.end())
	{
		return;
	}
	delete iter->second;

	m_sharedMemorys.erase(iter);
}

ELTE_INT32 SharedMemoryMgr::SetPlayWindow(const std::string& strResId, HWND hWnd)
{
	LOG_TRACE();
	INFO_PARAM1(strResId);

	std::map <std::string, SharedMemory*>::iterator iter = m_sharedMemorys.find(strResId);
	if(iter == m_sharedMemorys.end())
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	iter->second->SetPlayWindow(hWnd);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SharedMemoryMgr::LocalSnapshot(const std::string& strResourceID, const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat)
{
	LOG_TRACE();
	INFO_PARAM1(strResourceID);

	std::map <std::string, SharedMemory*>::iterator iter = m_sharedMemorys.find(strResourceID);
	if(iter == m_sharedMemorys.end())
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	ELTE_INT32 iRet = iter->second->LocalSnapshot(strSnapshotPath, ulSnapshotFormat);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		return iRet;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SharedMemoryMgr::SetPlayWindowSize(const std::string& strResId, ULONG ulWidth, ULONG ulHeight)
{
	LOG_TRACE();
	INFO_PARAM1(strResId);

	std::map <std::string, SharedMemory*>::iterator iter = m_sharedMemorys.find(strResId);
	if(iter == m_sharedMemorys.end())
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	iter->second->SetPlayWindowSize(ulWidth, ulHeight);
	return eLTE_SDK_ERR_SUCCESS;
}

