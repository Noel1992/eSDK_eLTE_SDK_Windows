#include "StdAfx.h"
#include "MediaPlayer.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"

// CImage
#include <atlimage.h>

MediaPlayer::MediaPlayer()
	: m_pHwnd(NULL)
	, m_sdlWindow(NULL)
	, m_sdlRenderer(NULL)
	, m_sdlTexture(NULL)
{
	LOG_TRACE();
	m_MutexWindows = CREATE_MUTEX();
	eSDK_MEMSET(&m_srcRect, 0x0, sizeof(SDL_Rect));
	eSDK_MEMSET(&m_dstRect, 0x0, sizeof(SDL_Rect));
	eSDK_MEMSET(&m_outRect, 0x0, sizeof(SDL_Rect));
}//lint !e1401


MediaPlayer::~MediaPlayer()
{
	try
	{
		LOG_TRACE();
		DestoryResource();
		DESTROY_MUTEX(m_MutexWindows);
	}
	catch (...)
	{
	}
}//lint !e1579

ELTE_VOID MediaPlayer::SetPlayHwnd(HWND pHwnd)
{
	LOG_TRACE();
	MutexLocker Locker(m_MutexWindows);
	if(NULL != m_pHwnd)
	{
		DestoryResource();
	}
	if(NULL != pHwnd)
	{
		m_pHwnd = pHwnd;
	}
}

ELTE_VOID MediaPlayer::AdjustPosition(const SDL_Rect& srcRect, const SDL_Rect& dstRect, SDL_Rect& outRect) const
{
	double xr = (double)dstRect.w / (double)srcRect.w;
	double yr = (double)dstRect.h / (double)srcRect.h;
	double r = xr > yr ? yr : xr;

	outRect.x = int(abs(dstRect.w - srcRect.w*r) / 2);
	outRect.y = int(abs(dstRect.h - srcRect.h*r) / 2);
	outRect.w = int(srcRect.w*r);
	outRect.h = int(srcRect.h*r);
}

ELTE_VOID MediaPlayer::PlayVideoStream(const YuvFrame* frame)
{
	MutexLocker Locker(m_MutexWindows);
	if(NULL != m_pHwnd)
	{
		if(NULL == m_sdlWindow)
		{
			// if the play window's (w and h is 0 and 0), create window will failed, so return.
			RECT wnd;
			GetWindowRect(m_pHwnd, &wnd);
			m_dstRect.x = 0;
			m_dstRect.y = 0;    
			m_dstRect.w = wnd.right - wnd.left;    
			m_dstRect.h = wnd.bottom - wnd.top;

			if (0 >= m_dstRect.w || 0 >= m_dstRect.h)
			{
				return;
			}

			m_sdlWindow = SDL_CreateWindowFrom((ELTE_VOID *)m_pHwnd);
			if(NULL == m_sdlWindow)
			{
				LOG_RUN_ERROR("SDL: could not create window - exiting:%s", SDL_GetError());
				m_pHwnd = NULL;
				return;
			}
		}
		if (NULL == m_sdlRenderer)
		{
			m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, 0);
			if(NULL == m_sdlRenderer)
			{
				LOG_RUN_ERROR("SDL: could not create sdlRenderer - exiting:%s", SDL_GetError());
				m_pHwnd = NULL;
				return;
			}
		}
		if(NULL == m_sdlTexture)
		{
			m_sdlTexture = SDL_CreateTexture(m_sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h);
			if(NULL == m_sdlTexture)
			{
				LOG_RUN_ERROR("SDL: could not create sdlTexture - exiting:%s",SDL_GetError()); 
				return;
			}
		}

		//实时更新窗口大小, 这句话加在这里, 会导致锁屏以后, 视频卡住.
		//SDL_SetWindowSize(m_sdlWindow, m_dstRect.w, m_dstRect.h);
		m_srcRect.h = frame->h;
		m_srcRect.w = frame->w;
		m_srcRect.x = 0; 
		m_srcRect.y = 0;
		if(eLTE_SDK_ERR_SUCCESS != SDL_UpdateYUVTexture(m_sdlTexture, &m_srcRect, frame->planes[0], frame->strides[0], frame->planes[1], frame->strides[1], frame->planes[2], frame->strides[2]))
		{
			LOG_RUN_ERROR("SDL_UpdateYUVTexture failed, error is %s.",SDL_GetError());
			return;
		}
		if(eLTE_SDK_ERR_SUCCESS != SDL_RenderClear(m_sdlRenderer))
		{
			LOG_RUN_ERROR("SDL_RenderClear failed, error is %s.",SDL_GetError());
			return;
		} 
		AdjustPosition(m_srcRect, m_dstRect, m_outRect);
		if(eLTE_SDK_ERR_SUCCESS != SDL_RenderCopy(m_sdlRenderer, m_sdlTexture, &m_srcRect, &m_outRect))
		{
			LOG_RUN_ERROR("SDL_RenderCopy failed, error is %s.",SDL_GetError());
			return;
		}
		SDL_RenderPresent(m_sdlRenderer);
	}
}

ELTE_INT32 MediaPlayer::Snapshot(const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat)
{
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
	if(NULL == m_sdlRenderer)
	{
		LOG_RUN_ERROR("SDL: sdlRenderer is null.");
		return eLTE_SDK_ERR_FAILED;
	}
	if(NULL == m_pHwnd)
	{
		LOG_RUN_ERROR("m_pHwnd is null.");
		return eLTE_SDK_ERR_FAILED;
	}
	HDC hdc = ::GetDC(m_pHwnd);//获取窗口DC
	int nBitPerPixel = GetDeviceCaps(hdc, BITSPIXEL);
	ATL::CImage image;
	if(!image.Create(m_outRect.w, m_outRect.h, nBitPerPixel))
	{
		LOG_RUN_ERROR("Image create failed,error code is %d.",GetLastError());
		return eLTE_SDK_ERR_FAILED;
	}
	if(!BitBlt(image.GetDC(), 0, 0, m_outRect.w, m_outRect.h, hdc, m_outRect.x, m_outRect.y, SRCCOPY))
	{
		LOG_RUN_ERROR("Image BitBlt failed,error code is %d.",GetLastError());
		return eLTE_SDK_ERR_FAILED;
	}
	::ReleaseDC(NULL, hdc);
	image.ReleaseDC();
	if(eLTE_SNAPSHOT_JPG == ulSnapshotFormat)
	{
		if(eLTE_SDK_ERR_SUCCESS != image.Save(strSavePath.c_str(), Gdiplus::ImageFormatJPEG))//ImageFormatJPEG
		{
			LOG_RUN_ERROR("Save jpg failed,error code is %d.",GetLastError());
			return eLTE_SDK_ERR_FAILED;
		}
	}
	else if (eLTE_SNAPSHOT_BMP == ulSnapshotFormat)
	{
		if(eLTE_SDK_ERR_SUCCESS != image.Save(strSavePath.c_str(), Gdiplus::ImageFormatBMP))//ImageFormatBMP
		{
			LOG_RUN_ERROR("Save bmp failed,error code is %d.",GetLastError());
			return eLTE_SDK_ERR_FAILED;
		}
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID MediaPlayer::DestoryResource()
{
	LOG_TRACE();
	if(NULL != m_sdlRenderer)
	{
		SDL_RenderClear(m_sdlRenderer);
	}
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
	m_sdlWindow = NULL;
	m_pHwnd = NULL;
}

ELTE_VOID MediaPlayer::SetDstRect(const ELTE_ULONG& iWidth, const ELTE_ULONG& iHeight)
{
	MutexLocker Locker(m_MutexWindows);
	m_dstRect.x = 0;
	m_dstRect.y = 0;    
	m_dstRect.w = (ELTE_INT32)iWidth;    
	m_dstRect.h = (ELTE_INT32)iHeight;
	
	if(m_sdlWindow)
	{
		SDL_SetWindowSize(m_sdlWindow, m_dstRect.w, m_dstRect.h); 
	}
}
