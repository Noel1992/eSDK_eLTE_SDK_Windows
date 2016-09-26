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

#include "stdafx.h"
#include "VideoRender.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "eLTE_Error.h"

//#define  PIXELS 32

VideoRender::VideoRender(void) 
	: m_iResId(0)
	, m_pSharedMemory(NULL)
	, m_bInitSuccess(FALSE)
{
}

VideoRender::VideoRender(const int& resId)
	: m_iResId(resId)
	, m_pSharedMemory(NULL)
	, m_bInitSuccess(FALSE)
{

}


VideoRender::~VideoRender(void)
{
	try
	{
		LOG_TRACE();
		m_iResId = 0;
		if(m_pSharedMemory)
		{
			delete m_pSharedMemory;
			m_pSharedMemory = NULL;
		}
	}
	catch (...)
	{
	}
}

void VideoRender::SetSize(int width, int height, int reserved)
{

}

void VideoRender::SetResourceID(MSISDN logical, MSISDN physical)
{

}

void VideoRender::SetVideoChannel(void* vc)
{

}

bool VideoRender::RenderYUVFrame(YuvFrame* frame, MSISDN physicalRid)
{
	if(NULL == frame)
	{
		return false;
	}
	if(NULL == m_pSharedMemory)
	{
		m_pSharedMemory = new SharedMemory(eLTE_Tool::Int2String(m_iResId));
		if(NULL == m_pSharedMemory)
		{
			LOG_RUN_ERROR("New SharedMemory failed.");
			return true;
		}
	}
	if (!m_bInitSuccess)
	{
		// init again
		if(eLTE_SVC_ERR_SUCCESS != m_pSharedMemory->InitResource())
		{
			m_bInitSuccess = FALSE;
		}
		else
		{
			m_bInitSuccess = TRUE;
		}
	}

	if(m_bInitSuccess)
	{
		VideoStream* pVideoStream = new VideoStream;
		if (NULL == pVideoStream)
		{
			LOG_RUN_ERROR("New VideoStream failed.");
			return true;
		}
		unsigned uiLen = (unsigned)(frame->h * frame->w);

		// size is width + height + strides[0] + strides[1] + strides[2] + Y(height * width) + U(height * width / 4) + V(height * width / 4)
		unsigned uLen = unsigned(5 * sizeof(int) + uiLen * 3 / 2);
		pVideoStream->pStream = new unsigned char[uLen];
		if (NULL == pVideoStream->pStream)
		{
			LOG_RUN_ERROR("New the Stream of VideoStream failed.");
			delete pVideoStream;
			pVideoStream = NULL;
			return true;//lint !e438
		}
		unsigned char* pStart = pVideoStream->pStream;
		
		//width
		eSDK_MEMCPY(pStart, uLen, &(frame->w), sizeof(int));
		pStart += sizeof(int);
		pVideoStream->w = frame->w;
		//height
		eSDK_MEMCPY(pStart, uLen, &(frame->h), sizeof(int));
		pStart += sizeof(int);
		pVideoStream->h = frame->h;
		//strides[0]
		eSDK_MEMCPY(pStart, uLen, &(frame->strides[0]), sizeof(int));
		pStart += sizeof(int);
		//strides[1]
		eSDK_MEMCPY(pStart, uLen, &(frame->strides[1]), sizeof(int));
		pStart += sizeof(int);
		//strides[2]
		eSDK_MEMCPY(pStart, uLen, &(frame->strides[2]), sizeof(int));
		pStart += sizeof(int);

		//y
		eSDK_MEMCPY(pStart, uLen, frame->planes[0], uiLen);
		pStart += uiLen;

		//u
		eSDK_MEMCPY(pStart, uLen, frame->planes[1], uiLen / 4);
		pStart += uiLen / 4;

		//v
		eSDK_MEMCPY(pStart, uLen, frame->planes[2], uiLen / 4);

		pVideoStream->uiSize = unsigned (5 * sizeof(int) + uiLen * 3 / 2);
		m_pSharedMemory->PushStream(pVideoStream);
	}

	return true;
}

RenderControl* VideoRender::getRenderControl()
{
	return NULL;
}

MSISDN VideoRender::getPhysical()
{
	return m_iResId;
}

MSISDN VideoRender::getLogical()
{
	return m_iResId;
}

void VideoRender::stopRendering()
{

}
