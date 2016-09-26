/********************************************************************
filename    :    MediaPlayer.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK媒体播放类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 初始版本
*********************************************************************/

#ifndef __ELTE_MEDIA_PLAYER_H__
#define __ELTE_MEDIA_PLAYER_H__

#include "SDL.h"
#include "eLTE_Types.h"
#include "mutex_locker.h"
#include <string>

// 抓图类型
typedef enum
{
	eLTE_SNAPSHOT_INVALID = 0,
	eLTE_SNAPSHOT_JPG,				// JPG 格式
	eLTE_SNAPSHOT_BMP				// BMP 格式
} eLTE_SNAPSHOT_FORMAT_E;

class MediaPlayer
{
public:
	MediaPlayer();
	virtual ~MediaPlayer();

	//设置播放窗口
	ELTE_VOID SetPlayHwnd(HWND pHwnd);
	//播放视频流
	ELTE_VOID PlayVideoStream(const YuvFrame* pYuvFrame);
	ELTE_VOID SetDstRect(const ELTE_ULONG& iWidth, const ELTE_ULONG& iHeight);
	ELTE_INT32 Snapshot(const std::string& strSnapshotPath, const ELTE_ULONG& ulSnapshotFormat);
private:
	ELTE_VOID AdjustPosition(const SDL_Rect& srcRect, const SDL_Rect& dstRect, SDL_Rect& outRect) const;
	ELTE_VOID DestoryResource();
private:
	HWND           m_pHwnd;
	SDL_Window*    m_sdlWindow;
	SDL_Renderer*  m_sdlRenderer;
	SDL_Texture*   m_sdlTexture;
	/*SDL_Surface*   m_sdlSurface;*/
	SDL_Rect       m_srcRect;
	SDL_Rect       m_dstRect;
	SDL_Rect       m_outRect;

	MUTEX_HANDLE  m_MutexWindows;
};

#endif
