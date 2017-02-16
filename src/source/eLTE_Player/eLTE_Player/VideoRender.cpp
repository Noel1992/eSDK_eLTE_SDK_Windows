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
#include "eLTE_PlayerCtrl.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include <atlimage.h>

//#define  PIXELS 32

VideoRender::VideoRender(void) 
	: m_iResId(0)
	, m_pHwnd(NULL)
	, m_sdlWindow(NULL)
	, m_sdlRenderer(NULL)
	, m_sdlTexture(NULL)
	/*, m_sdlSurface(NULL)*/
{
	LOG_TRACE();
	eSDK_MEMSET(&m_srcRect, 0x0, sizeof(m_srcRect));
	eSDK_MEMSET(&m_dstRect, 0x0, sizeof(m_dstRect));
	eSDK_MEMSET(&m_outRect, 0x0, sizeof(m_outRect));
}//lint !e1401

VideoRender::VideoRender(const int& resId)
	: m_iResId(resId)
	, m_pHwnd(NULL)
	, m_sdlWindow(NULL)
	, m_sdlRenderer(NULL)
	, m_sdlTexture(NULL)
	/*, m_sdlSurface(NULL)*/
{
	LOG_TRACE();
	eSDK_MEMSET(&m_srcRect, 0x0, sizeof(m_srcRect));
	eSDK_MEMSET(&m_dstRect, 0x0, sizeof(m_dstRect));
	eSDK_MEMSET(&m_outRect, 0x0, sizeof(m_outRect));
}//lint !e1401


VideoRender::~VideoRender(void)
{
	try
	{
		LOG_TRACE();
		SDL_RenderClear(m_sdlRenderer);
		m_iResId = 0;
		if(NULL != m_sdlTexture)
		{
			SDL_DestroyTexture(m_sdlTexture);
			m_sdlTexture = NULL;
		}
		if(NULL != m_sdlRenderer)
		{
			SDL_DestroyRenderer(m_sdlRenderer);
			m_sdlRenderer = NULL;
		}
		//if(NULL != m_sdlWindow)
		//{
		//	SDL_DestroyWindow(m_sdlWindow);窗口需要重复使用，SDL_DestroyWindow会销毁窗口句柄
		//}
		m_sdlWindow = NULL;
		/*if(NULL != m_sdlSurface)
		{
			SDL_FreeSurface(m_sdlSurface);
			m_sdlSurface = NULL;
		}*/

		m_pHwnd = NULL;
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
	if(NULL == m_pHwnd)
	{
		std::set<CeLTE_PlayerCtrl*>::iterator itor_b = CeLTE_PlayerCtrl::m_eLTE_PlayerList.begin();
		std::set<CeLTE_PlayerCtrl*>::iterator itor_e = CeLTE_PlayerCtrl::m_eLTE_PlayerList.end();
		for (; itor_e != itor_b; itor_b++)
		{
			CeLTE_PlayerCtrl* pOCX = *itor_b;
			if (NULL != pOCX)
			{
				int iResId = pOCX->GetMediaPlayer().GetMediaPlayerResID();
				if (m_iResId == iResId)
				{
					m_pHwnd = pOCX->GetVideoPane().GetVideoStaticSafeHwnd();
					(const_cast<CVideoPane&>(pOCX->GetVideoPane())).EnableImageButton(TRUE);
					RECT wnd;
					pOCX->GetVideoPane().GetVideoStatic().GetWindowRect(&wnd);
					m_dstRect.x = 0;
					m_dstRect.y = 0;    
					m_dstRect.w = wnd.right - wnd.left;    
					m_dstRect.h = wnd.bottom - wnd.top;
					break;
				}
			}
		}
	}

	if (0 >= m_dstRect.w || 0 >= m_dstRect.h)
	{
		return true;//DTS2015060209675
	}
	

	if(NULL != m_pHwnd)
	{
		if(NULL == m_sdlWindow)
		{
			m_sdlWindow = SDL_CreateWindowFrom((void *)m_pHwnd);
			if(NULL == m_sdlWindow)
			{
				LOG_RUN_ERROR("SDL: could not create window - exiting:%s", SDL_GetError());
				return false;
			}
		}
		if (NULL == m_sdlRenderer)
		{
			m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, 0);
			if(NULL == m_sdlRenderer)
			{
				LOG_RUN_ERROR("SDL: could not create sdlRenderer - exiting:%s", SDL_GetError());
				return false;
			}
		}
		if(NULL == m_sdlTexture)
		{
			m_sdlTexture = SDL_CreateTexture(m_sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h);
			if(NULL == m_sdlTexture)
			{
				LOG_RUN_ERROR("SDL: could not create sdlTexture - exiting:%s",SDL_GetError()); 
				return false;
			}
		}
		/*if(NULL == m_sdlSurface)
		{
		m_sdlSurface = SDL_CreateRGBSurface(0, m_dstRect.w, m_dstRect.h, PIXELS, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		if(NULL == m_sdlSurface)
		{
		LOG_RUN_ERROR("SDL: could not create sdlSurface - exiting:%s",SDL_GetError()); 
		return false;
		}
		}*/
		m_srcRect.h = frame->h;
		m_srcRect.w = frame->w;
		m_srcRect.x = 0;
		m_srcRect.y = 0;
		if(eLTE_ERR_SUCCESS != SDL_UpdateYUVTexture(m_sdlTexture, &m_srcRect, frame->planes[0], frame->strides[0], frame->planes[1], frame->strides[1], frame->planes[2], frame->strides[2]))
		{
			LOG_RUN_ERROR("SDL_UpdateYUVTexture failed, error is %s.",SDL_GetError());
			return false;
		}
		if(eLTE_ERR_SUCCESS != SDL_RenderClear(m_sdlRenderer))
		{
			LOG_RUN_ERROR("SDL_RenderClear failed, error is %s.",SDL_GetError());
			return false;
		} 
		AdjustPosition(m_srcRect, m_dstRect, m_outRect);
		if(eLTE_ERR_SUCCESS != SDL_RenderCopy(m_sdlRenderer, m_sdlTexture, &m_srcRect, &m_outRect))
		{
			LOG_RUN_ERROR("SDL_RenderCopy failed, error is %s.",SDL_GetError());
			return false;
		}
		SDL_RenderPresent(m_sdlRenderer);
	}
	return true;
}

void VideoRender::AdjustPosition(const SDL_Rect& srcRect, const SDL_Rect& dstRect, SDL_Rect& outRect) const
{
	double xr = (double)dstRect.w / (double)srcRect.w;
	double yr = (double)dstRect.h / (double)srcRect.h;
	double r = xr > yr ? yr : xr;

	outRect.x = int(abs(dstRect.w - srcRect.w*r) / 2);
	outRect.y = int(abs(dstRect.h - srcRect.h*r) / 2);
	outRect.w = int(srcRect.w*r);
	outRect.h = int(srcRect.h*r);
}

void VideoRender::SetDstRect(const int& iLeft, const int& iTop, const int& iWidth, const int& iHeight)
{
	m_dstRect.x = iLeft;
	m_dstRect.y = iTop;    
	m_dstRect.w = iWidth;    
	m_dstRect.h = iHeight;
	SDL_SetWindowSize(m_sdlWindow, m_dstRect.w, m_dstRect.h); 
}

int VideoRender::LocalSnapshot(const std::string& strSnapshotPath, const int& iSnapshotFormat)
{
	if(NULL == m_sdlRenderer)
	{
		LOG_RUN_ERROR("SDL: sdlRenderer is null.");
		return eLTE_ERR_FAILED;
	}
	if(NULL == m_pHwnd)
	{
		LOG_RUN_ERROR("m_pHwnd is null.");
		return eLTE_ERR_FAILED;
	}
	HDC hdc = ::GetDC(m_pHwnd);//获取窗口DC
	int nBitPerPixel = GetDeviceCaps(hdc, BITSPIXEL);
	ATL::CImage image;
	if(!image.Create(m_outRect.w, m_outRect.h, nBitPerPixel))
	{
		LOG_RUN_ERROR("Image create failed,error code is %d.",GetLastError());
		return eLTE_ERR_FAILED;
	}
	if(!BitBlt(image.GetDC(), 0, 0, m_outRect.w, m_outRect.h, hdc, m_outRect.x, m_outRect.y, SRCCOPY))
	{
		LOG_RUN_ERROR("Image BitBlt failed,error code is %d.",GetLastError());
		return eLTE_ERR_FAILED;
	}
	::ReleaseDC(NULL, hdc);
	image.ReleaseDC();
	if(eLTE_SNAPSHOT_JPG == iSnapshotFormat)
	{
		if(eLTE_ERR_SUCCESS != image.Save(eLTE_Tool::ANSIToUnicode(strSnapshotPath).c_str(), Gdiplus::ImageFormatJPEG))//ImageFormatJPEG
		{
			LOG_RUN_ERROR("Save jpg failed,error code is %d.",GetLastError());
			return eLTE_ERR_FAILED;
		}
	}
	else if (eLTE_SNAPSHOT_BMP == iSnapshotFormat)
	{
		if(eLTE_ERR_SUCCESS != image.Save(eLTE_Tool::ANSIToUnicode(strSnapshotPath).c_str(), Gdiplus::ImageFormatBMP))//ImageFormatBMP
		{
			LOG_RUN_ERROR("Save bmp failed,error code is %d.",GetLastError());
			return eLTE_ERR_FAILED;
		}
	}
	return eLTE_ERR_SUCCESS;
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
