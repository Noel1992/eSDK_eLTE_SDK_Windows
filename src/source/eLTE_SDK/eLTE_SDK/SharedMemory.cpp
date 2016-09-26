#include "StdAfx.h"
#include "SharedMemory.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "UserMgr.h"
#include "SharedMemoryMgr.h"
#include "eLTE_Tool.h"
#include <process.h>
#include <atlimage.h> 

#define WAIT_STOP_THREAD_TIME 3*1000

SharedMemory::SharedMemory(const std::string& strFileName)
	: m_hFileHandle(NULL)
	, m_pBuffer(NULL)
	, m_MutexRev(NULL)
	, m_EventFull(NULL)
	, m_EventEmpty(NULL)
	, m_strFileName(strFileName)
	, m_revThread(NULL)
	, m_revThreadId(0)
	, m_bStopRevThtread(FALSE)
	, m_Thread(NULL)
	, m_ThreadId(0)
	, m_bRun(FALSE)
{
	m_MutexYuvFrameQueue = CREATE_MUTEX();
	m_MutexCurYuvFrame = CREATE_MUTEX();
	m_pCurrentYuvFrame = NULL;
}


SharedMemory::~SharedMemory()
{
	try
	{
		// 停止线程
		if(0 != StopRevMsg())
		{
			return;
		}
		UnmapViewOfFile(m_pBuffer);
		m_pBuffer = NULL;
		CloseHandle(m_hFileHandle);
		m_hFileHandle = NULL;

		DESTROY_MUTEX(m_MutexRev);
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

		StopYuvFrameProcess();
		ClearYuvFrames();
		DeleteYuvFrame(m_pCurrentYuvFrame);
		DESTROY_MUTEX(m_MutexYuvFrameQueue);
		DESTROY_MUTEX(m_MutexCurYuvFrame);
	}
	catch (...)
	{	

	}
}//lint !e1579

int SharedMemory::InitResource()
{
	LOG_TRACE();

	std::string strTemp;
	strTemp.append("Global\\");
	strTemp.append(m_strFileName);
	m_hFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHARE_MEMORY_SIZE, strTemp.c_str());
	if(NULL == m_hFileHandle)
	{
		LOG_RUN_ERROR("CreateFileMapping failed, error:%d.", GetLastError());
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	m_pBuffer = MapViewOfFile(m_hFileHandle, FILE_MAP_READ, 0, 0, 0);
	if(NULL == m_pBuffer)
	{
		LOG_RUN_ERROR("MapViewOfFile failed, error:%d.", GetLastError());
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	m_MutexRev = ::CreateMutex(NULL, FALSE, strTemp.append("_mutex").c_str());
	if(NULL == m_MutexRev)
	{
		LOG_RUN_ERROR("CreateMutex failed, error:%d.", GetLastError());
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	m_EventFull = ::CreateEvent(NULL, FALSE, FALSE, strTemp.append("_full_event").c_str());
	if(NULL == m_EventFull)
	{
		LOG_RUN_ERROR("CreateEventFull failed, error:%d.", GetLastError());
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	m_EventEmpty = ::CreateEvent(NULL , FALSE, TRUE, strTemp.append("_empty_event").c_str());
	if(NULL == m_EventEmpty)
	{
		LOG_RUN_ERROR("CreateEventEmpty failed, error:%d.", GetLastError());
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SharedMemory::StartRevMsg()
{
	LOG_TRACE();
	if(NULL != m_revThread)
	{
		CloseHandle(m_revThread);
		m_revThread = NULL;
		LOG_RUN_DEBUG("Stop rev msg thread success, thread id[%d]", m_revThreadId);
	}

	m_revThread = (HANDLE)_beginthreadex(NULL, 0, RevMsgThread, (void*)this, 0, &m_revThreadId);
	if(NULL == m_revThread)
	{
		LOG_RUN_ERROR("Start rev msg thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		m_bStopRevThtread = FALSE;
		LOG_RUN_DEBUG("Start rev msg thread success, thread id[%d]", m_revThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_UINT32 __stdcall SharedMemory::RevMsgThread(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	SharedMemory* pShareMemory = (SharedMemory*)param;
	//接受消息
	pShareMemory->DoRevMsg();
	return eLTE_SDK_ERR_SUCCESS;
}


ELTE_VOID SharedMemory::DoRevMsg()
{
	while(!m_bStopRevThtread)
	{
		if(0 != WaitForSingleObject(m_EventFull, 40))
		{
			// 时间未到
			continue;
		}
		MutexLocker Locker(m_MutexRev);
		if(NULL != m_pBuffer)
		{
			YuvFrame* pYuvFrame = new YuvFrame;
			if(NULL == pYuvFrame)
			{
				LOG_RUN_ERROR("New yuvframe failed.");
				SetEvent(m_EventFull);
				continue;
			}
			pYuvFrame->planes[0]=NULL;
			pYuvFrame->planes[1]=NULL;
			pYuvFrame->planes[2]=NULL;
			pYuvFrame->planes[3]=NULL;
			ELTE_UCHAR* pStart = (ELTE_UCHAR*)m_pBuffer;
			//获取width
			eSDK_MEMCPY(&pYuvFrame->w, sizeof(pYuvFrame->w), pStart, sizeof(ELTE_INT32));
			pStart = pStart + sizeof(ELTE_INT32);
			//获取height
			eSDK_MEMCPY(&pYuvFrame->h, sizeof(pYuvFrame->h), pStart, sizeof(int));
			pStart = pStart + sizeof(int);
			//获取strides[0]
			eSDK_MEMCPY(&(pYuvFrame->strides[0]), sizeof(pYuvFrame->strides[0]), pStart, sizeof(ELTE_INT32));
			pStart = pStart + sizeof(ELTE_INT32);
			//获取strides[1]
			eSDK_MEMCPY(&(pYuvFrame->strides[1]), sizeof(pYuvFrame->strides[1]), pStart, sizeof(int));
			pStart = pStart + sizeof(int);
			//获取strides[2]
			eSDK_MEMCPY(&(pYuvFrame->strides[2]), sizeof(pYuvFrame->strides[2]), pStart, sizeof(ELTE_INT32));
			pStart = pStart + sizeof(ELTE_INT32);

			pYuvFrame->strides[3] = 0;

			//y
			ELTE_UINT32 uiLen = (ELTE_UINT32)(pYuvFrame->w * pYuvFrame->h);
			pYuvFrame->planes[0] = new ELTE_UCHAR[uiLen + 1];
			if(NULL == pYuvFrame->planes[0])
			{
				LOG_RUN_ERROR("New yuvframe planes0 failed.");
				delete pYuvFrame;
				pYuvFrame = NULL;
				SetEvent(m_EventFull);
				continue;
			}
			eSDK_MEMCPY(pYuvFrame->planes[0], uiLen + 1, pStart, uiLen);
			pStart = pStart + uiLen;
			pYuvFrame->planes[0][uiLen] = '\0';

			//u
			pYuvFrame->planes[1] = new ELTE_UCHAR[uiLen / 4 + 1];
			if(NULL == pYuvFrame->planes[1])
			{
				LOG_RUN_ERROR("New yuvframe planes1 failed.");
				delete [] pYuvFrame->planes[0];
				pYuvFrame->planes[0] = NULL;
				delete pYuvFrame;
				pYuvFrame = NULL;
				SetEvent(m_EventFull);
				continue;
			}
			eSDK_MEMCPY(pYuvFrame->planes[1], uiLen / 4 + 1, pStart, uiLen / 4);
			pStart = pStart + uiLen / 4;
			pYuvFrame->planes[1][uiLen / 4] = '\0';

			//v
			pYuvFrame->planes[2] = new ELTE_UCHAR[uiLen / 4 + 1];
			if(NULL == pYuvFrame->planes[2])
			{
				LOG_RUN_ERROR("New yuvframe planes2 failed.");
				delete [] pYuvFrame->planes[0];
				pYuvFrame->planes[0] = NULL;
				delete [] pYuvFrame->planes[1];
				pYuvFrame->planes[1] = NULL;
				delete pYuvFrame;
				pYuvFrame = NULL;
				SetEvent(m_EventFull);
				continue;
			}
			eSDK_MEMCPY(pYuvFrame->planes[2], uiLen / 4 + 1, pStart, uiLen / 4);
			pStart = pStart + uiLen / 4;
			pYuvFrame->planes[2][uiLen / 4] = '\0';
			pStart = NULL;//lint !e838
			SetEvent(m_EventEmpty);

			{
				MutexLocker curLocker(m_MutexCurYuvFrame);
				if(NULL == m_pCurrentYuvFrame)
				{
					m_pCurrentYuvFrame = new YuvFrame;
					if (NULL == m_pCurrentYuvFrame)
					{
						LOG_RUN_ERROR("New curyuvframe failed.");
						continue;
					}
					m_pCurrentYuvFrame->planes[0] = NULL;
					m_pCurrentYuvFrame->planes[1] = NULL;
					m_pCurrentYuvFrame->planes[2] = NULL;
				}
				m_pCurrentYuvFrame->h = pYuvFrame->h;
				m_pCurrentYuvFrame->w = pYuvFrame->w;
				//y
				if(NULL == m_pCurrentYuvFrame->planes[0])
				{
					m_pCurrentYuvFrame->planes[0] = new ELTE_UCHAR[uiLen + 1];
					if(NULL == m_pCurrentYuvFrame->planes[0])
					{
						LOG_RUN_ERROR("New currentyuvframe planes0 failed.");
						DeleteYuvFrame(pYuvFrame);
						continue;
					}
				}
				eSDK_MEMCPY(m_pCurrentYuvFrame->planes[0], uiLen + 1, pYuvFrame->planes[0], uiLen);
				m_pCurrentYuvFrame->planes[0][uiLen] = '\0';

				//u
				if(NULL == m_pCurrentYuvFrame->planes[1])
				{
					m_pCurrentYuvFrame->planes[1] = new ELTE_UCHAR[uiLen / 4 + 1];
					if(NULL == m_pCurrentYuvFrame->planes[1])
					{
						LOG_RUN_ERROR("New currentyuvframe planes1 failed.");
						DeleteYuvFrame(pYuvFrame);
						continue;
					}
				}
				eSDK_MEMCPY(m_pCurrentYuvFrame->planes[1], uiLen / 4 + 1, pYuvFrame->planes[1], uiLen / 4);
				m_pCurrentYuvFrame->planes[1][uiLen / 4] = '\0';

				//v
				if(NULL == m_pCurrentYuvFrame->planes[2])
				{
					m_pCurrentYuvFrame->planes[2] = new ELTE_UCHAR[uiLen / 4 + 1];
					if(NULL == m_pCurrentYuvFrame->planes[2])
					{
						LOG_RUN_ERROR("New currentyuvframe planes2 failed.");
						DeleteYuvFrame(pYuvFrame);
						continue;
					}
				}
				eSDK_MEMCPY(m_pCurrentYuvFrame->planes[2], uiLen / 4 + 1, pYuvFrame->planes[2], uiLen / 4);
				m_pCurrentYuvFrame->planes[2][uiLen / 4] = '\0';
			}

			//视频播放
			m_mediaPlayer.PlayVideoStream(pYuvFrame);
			//放入队列
			PushYuvFrame(pYuvFrame);
		}//lint !e438

	}
}

ELTE_INT32 SharedMemory::LocalSnapshot(const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat)
{
	ELTE_UCHAR* pRGBData = NULL;
	ELTE_INT32 iRGBSize = 0;
	ELTE_INT32 iHeight = 0;
	ELTE_INT32 iWidth = 0;
	{
		MutexLocker curLocker(m_MutexCurYuvFrame);
		if(NULL == m_pCurrentYuvFrame || NULL == m_pCurrentYuvFrame->planes[0] || NULL == m_pCurrentYuvFrame->planes[1] || NULL == m_pCurrentYuvFrame->planes[2])
		{
			LOG_RUN_ERROR("m_pCurrentYuvFrame or its planes is null.");
			return eLTE_SDK_ERR_NULL_POINTER;
		}
		iHeight = m_pCurrentYuvFrame->h;
		iWidth = m_pCurrentYuvFrame->w;
		LOG_RUN_DEBUG("Current yuv frame's height is %d, width is %d", iHeight, iWidth);
		ELTE_INT32 iBaseSize = iHeight * iWidth;
		iRGBSize = iBaseSize * 3;

		pRGBData = new ELTE_UCHAR[iRGBSize];
		if (NULL == pRGBData)
		{
			LOG_RUN_ERROR("new pRGBData failed.");
			return eLTE_SDK_ERR_NULL_POINTER;
		}
		eSDK_MEMSET(pRGBData, 0, iRGBSize);

		ELTE_INT32 iRet = ConvertYUV420ToRGB(pRGBData);
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			delete [] pRGBData;
			pRGBData = NULL;
			return iRet;
		}
	}
	
	BITMAPINFOHEADER bmInfoHeader = {0};
	bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfoHeader.biWidth = iWidth;
	bmInfoHeader.biHeight = -iHeight;
	bmInfoHeader.biPlanes = 1;
	bmInfoHeader.biBitCount = 24;
	bmInfoHeader.biSizeImage = ((((bmInfoHeader.biWidth * bmInfoHeader.biBitCount) + 31) & ~31) / 8) * bmInfoHeader.biHeight;

	BITMAPFILEHEADER bmFileHeader = {0};
	bmFileHeader.bfType = 0x4d42; //bmp 
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bmInfoHeader.biSizeImage;

	IStream *postStream = NULL;
	HRESULT hr = S_FALSE;
	HGLOBAL hBmpGlobal = GlobalAlloc(GMEM_MOVEABLE, bmFileHeader.bfSize);
	hr = CreateStreamOnHGlobal(hBmpGlobal, true, &postStream);
	if(S_OK != hr)
	{
		LOG_RUN_ERROR("CreateStreamOnHGlobal failed.");
		GlobalFree(hBmpGlobal);
		hBmpGlobal = NULL;
		delete [] pRGBData;
		pRGBData = NULL;
		return eLTE_SDK_ERR_FAILED;
	}
	DWORD dwWrite = 0;
	if(S_OK != postStream->Write(&bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWrite))
	{
		LOG_RUN_ERROR("write bmFileHeader failed.");
		postStream->Release();
		postStream = NULL;
		GlobalFree(hBmpGlobal);
		hBmpGlobal = NULL;
		delete [] pRGBData;
		pRGBData = NULL;
		return eLTE_SDK_ERR_FAILED;
	}
	if(S_OK != postStream->Write(&bmInfoHeader, sizeof(BITMAPINFOHEADER), &dwWrite))
	{
		LOG_RUN_ERROR("write bmInfoHeader failed.");
		postStream->Release();
		postStream = NULL;
		GlobalFree(hBmpGlobal);
		hBmpGlobal = NULL;
		delete [] pRGBData;
		pRGBData = NULL;
		return eLTE_SDK_ERR_FAILED;
	}
	if(S_OK != postStream->Write(pRGBData, iRGBSize, &dwWrite))
	{
		LOG_RUN_ERROR("write bmData failed.");
		postStream->Release();
		postStream = NULL;
		GlobalFree(hBmpGlobal);
		hBmpGlobal = NULL;
		delete [] pRGBData;
		pRGBData = NULL;
		return eLTE_SDK_ERR_FAILED;
	}
	ATL::CImage ima;
	if(S_OK != ima.Load(postStream))
	{
		LOG_RUN_ERROR("ima load failed.");
		postStream->Release();
		postStream = NULL;
		GlobalFree(hBmpGlobal);
		hBmpGlobal = NULL;
		delete [] pRGBData;
		pRGBData = NULL;
		return eLTE_SDK_ERR_FAILED;
	}

	size_t index = strSnapshotPath.find_last_of("\\");
	std::string strSavePath;
	if(std::string::npos != index)
	{
		strSavePath = strSnapshotPath.substr(0, index);
		// 相对路径转绝对路径
		eLTE_Tool::RelativePathToAbsolute(strSavePath);
		if(!eLTE_Tool::CreateMultipleDirectory(strSavePath))
		{
			LOG_RUN_ERROR("CreateMultipleDirectory %s failed, GetLastError %d.", strSnapshotPath.c_str(), GetLastError());
		}
		strSavePath.append(strSnapshotPath.substr(index));
	}
	if(eLTE_SNAPSHOT_JPG == ulSnapshotFormat)
	{
		if(eLTE_SDK_ERR_SUCCESS != ima.Save(strSavePath.c_str(), Gdiplus::ImageFormatJPEG))//ImageFormatJPEG
		{
			LOG_RUN_ERROR("Save jpg failed,error code is %d.",GetLastError());
			postStream->Release();
			postStream = NULL;
			GlobalFree(hBmpGlobal);
			hBmpGlobal = NULL;
			delete [] pRGBData;
			pRGBData = NULL;
			return eLTE_SDK_ERR_FAILED;
		}
	}
	else if (eLTE_SNAPSHOT_BMP == ulSnapshotFormat)
	{
		if(eLTE_SDK_ERR_SUCCESS != ima.Save(strSavePath.c_str(), Gdiplus::ImageFormatBMP))//ImageFormatBMP
		{
			LOG_RUN_ERROR("Save bmp failed,error code is %d.",GetLastError());
			postStream->Release();
			postStream = NULL;
			GlobalFree(hBmpGlobal);
			hBmpGlobal = NULL;
			delete [] pRGBData;
			pRGBData = NULL;
			return eLTE_SDK_ERR_FAILED;
		}
	}

	postStream->Release();
	postStream = NULL;
	GlobalFree(hBmpGlobal);
	hBmpGlobal = NULL;
	delete [] pRGBData;
	pRGBData = NULL;

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SharedMemory::ConvertYUV420ToRGB(ELTE_UCHAR* pData)
{
	ELTE_UCHAR* pY = m_pCurrentYuvFrame->planes[0];
	ELTE_UCHAR* pU = m_pCurrentYuvFrame->planes[1]; 
	ELTE_UCHAR* pV = m_pCurrentYuvFrame->planes[2];
	ELTE_INT32 iHeight = m_pCurrentYuvFrame->h;
	ELTE_INT32 iWidth = m_pCurrentYuvFrame->w;
	if (!pData)  
	{  
		LOG_RUN_ERROR("pData is null.");
		return eLTE_SDK_ERR_FAILED;  
	} 
	ELTE_INT32 iBaseSize = iHeight * iWidth;
	ELTE_INT32 iRGBSize = iBaseSize * 3;

	ELTE_UCHAR* pRGBData = new ELTE_UCHAR[iRGBSize];
	if (NULL == pRGBData)
	{
		LOG_RUN_ERROR("new pRGBData failed.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	eSDK_MEMSET(pRGBData, 0, iRGBSize);
	
	ELTE_INT32 iTemp = 0;
	ELTE_UCHAR* pRData = pRGBData;
	ELTE_UCHAR* pGData = pRGBData + iBaseSize;
	ELTE_UCHAR* pBData = pGData + iBaseSize;
	ELTE_INT32 iIndex = 0;
	ELTE_INT32 iYUVIndex = 0;

	for(ELTE_INT32 i = 0; i < iHeight; ++i)
	{
		for (ELTE_INT32 j = 0; j < iWidth; ++j)
		{
			iIndex = i * iWidth + j;
			//iYUVIndex = iIndex;
			iYUVIndex = (i >> 1) * (iWidth >> 1) + (j >> 1); 
			/* r分量 */  
			iTemp = (ELTE_INT32)(pY[iIndex] + (pV[iYUVIndex] - 128) * 1.4022);
			pRData[iIndex] = iTemp < 0 ? 0 : (iTemp > 255 ? 255 : iTemp);

			/* g分量 */  
			iTemp = (ELTE_INT32)(pY[iIndex] + (pU[iYUVIndex] - 128) * (-0.3456) +  (pV[iYUVIndex] - 128) * (-0.7145));  
			pGData[iIndex] = iTemp < 0 ? 0 : (iTemp > 255 ? 255 : iTemp);

			/* b分量 */  
			iTemp = (ELTE_INT32)(pY[iIndex] + (pU[iYUVIndex] - 128) * 1.771);  
			pBData[iIndex] = iTemp < 0 ? 0 : (iTemp > 255 ? 255 : iTemp);
		}
	}

	ELTE_INT32 iWidthStep = iWidth * 3;

	for(ELTE_INT32 i = 0; i < iHeight; ++i)
	{
		for (ELTE_INT32 j = 0; j < iWidth; ++j)
		{
			pData[i * iWidthStep + j * 3 + 2] = pRData[i * iWidth + j];   //R  
			pData[i * iWidthStep + j * 3 + 1] = pGData[i * iWidth + j];   //G  
			pData[i * iWidthStep + j * 3 + 0] = pBData[i * iWidth + j];   //B  
		}
	}

	delete [] pRGBData;
	pRGBData = NULL;

	return eLTE_SDK_ERR_SUCCESS;
}

int SharedMemory::StopRevMsg()
{
	LOG_TRACE();
	m_bStopRevThtread = TRUE;
	if(NULL == m_revThread)
	{
		LOG_RUN_DEBUG("Stop rev msg thread success, no thread is running");
		return eLTE_SDK_ERR_SUCCESS;
	}
	ELTE_ULONG ulResult = ::WaitForSingleObject(m_revThread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_revThread);
	m_revThread = NULL;
	LOG_RUN_DEBUG("Stop rev msg thread success, thread id[%d]", m_revThreadId);
	m_revThreadId = 0;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SharedMemory::PushYuvFrame(YuvFrame* pYuvFrame)
{
	if(NULL ==  pYuvFrame)
	{
		LOG_RUN_ERROR("Yuvframe is null.");
	}
	MutexLocker Locker(m_MutexYuvFrameQueue);
	m_yuvFrameQueue.push(pYuvFrame);
}

YuvFrame* SharedMemory::PopYuvFrame()
{
	MutexLocker Locker(m_MutexYuvFrameQueue);
	if(0 == m_yuvFrameQueue.size())
	{
		return NULL;
	}

	YuvFrame* pstYuvFrame = m_yuvFrameQueue.front();
	m_yuvFrameQueue.pop();
	return pstYuvFrame;
}

ELTE_VOID SharedMemory::ClearYuvFrames()
{
	MutexLocker Locker(m_MutexYuvFrameQueue);

	while(!m_yuvFrameQueue.empty())
	{
		YuvFrame* pstYuvFrameFront = m_yuvFrameQueue.front();
		if(pstYuvFrameFront)
		{
			DeleteYuvFrame(pstYuvFrameFront);
			m_yuvFrameQueue.pop();
		}
	}//lint !e438
}


ELTE_UINT32 __stdcall ThreadYuvFrameFun(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	SharedMemory* pSharedMemory = (SharedMemory*) param;
	pSharedMemory->MainLoop();
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SharedMemory::StartYuvFrameProcess()
{
	LOG_TRACE();
	if(NULL != m_Thread)
	{
		CloseHandle(m_Thread);
		m_Thread = NULL;
		LOG_RUN_DEBUG("Stop YuvFrame process thread success, thread id[%d]", m_ThreadId);
	}

	m_bRun = TRUE;
	m_Thread = (HANDLE)_beginthreadex(NULL, 0, &ThreadYuvFrameFun, (ELTE_VOID*)this, 0, &m_ThreadId);
	if(NULL == m_Thread)
	{
		LOG_RUN_ERROR("Start YuvFrame process thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		LOG_RUN_DEBUG("Start YuvFrame process thread success, thread id[%d]", m_ThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_VOID SharedMemory::StopYuvFrameProcess()
{
	LOG_TRACE();
	m_bRun = FALSE;
	if(NULL == m_Thread)
	{
		LOG_RUN_INFO("Stop YuvFrame process thread success, no thread is running");
		return;
	}
	ELTE_ULONG ulResult = ::WaitForSingleObject(m_Thread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_Thread);
	m_Thread = NULL;
	LOG_RUN_DEBUG("Stop YuvFrame process thread success, thread id[%d]", m_ThreadId);
	m_ThreadId = 0;
}

ELTE_VOID SharedMemory::MainLoop()
{
	YUVFrameCallBack yuvFrameCallBack = NULL;
	ELTE_VOID* pUserData = NULL;
	ELTE_INT32 iResId = eLTE_Tool::String2Int(m_strFileName);
	while(m_bRun)
	{
		YuvFrame* pYuvFrame = PopYuvFrame();
		if(NULL == pYuvFrame)
		{
			Sleep(40);
			continue;
		}
		
		//回调发送
		yuvFrameCallBack = SharedMemoryMgr::Instance().GetYUVFrameCallBack();
		pUserData = SharedMemoryMgr::Instance().GetUserData();
		if (yuvFrameCallBack)
		{
			yuvFrameCallBack(iResId, pYuvFrame, pUserData);
		}
		DeleteYuvFrame(pYuvFrame);
	}
}

ELTE_VOID SharedMemory::SetPlayWindow(HWND hWnd)
{
	m_mediaPlayer.SetPlayHwnd(hWnd);
}

ELTE_VOID SharedMemory::DeleteYuvFrame(YuvFrame* pYuvFrame) const
{
	if (NULL == pYuvFrame)
	{
		return;
	}
	if(NULL != pYuvFrame->planes[0])
	{
		delete [] pYuvFrame->planes[0];
		pYuvFrame->planes[0] = NULL;
	}
	if(NULL != pYuvFrame->planes[1])
	{
		delete [] pYuvFrame->planes[1];
		pYuvFrame->planes[1] = NULL;
	}
	if(NULL != pYuvFrame->planes[2])
	{
		delete [] pYuvFrame->planes[2];
		pYuvFrame->planes[2] = NULL;
	}
	delete pYuvFrame;
	pYuvFrame = NULL;
}//lint !e438


ELTE_VOID SharedMemory::SetPlayWindowSize(ULONG ulWidth, ULONG ulHeight)
{
	m_mediaPlayer.SetDstRect(ulWidth, ulHeight);
}
