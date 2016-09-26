/********************************************************************
filename    :    MediaPlayer.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDKý�岥����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_MEDIA_PLAYER_H__
#define __ELTE_MEDIA_PLAYER_H__

#include "SDL.h"
#include "eLTE_Types.h"
#include "mutex_locker.h"
#include <string>

// ץͼ����
typedef enum
{
	eLTE_SNAPSHOT_INVALID = 0,
	eLTE_SNAPSHOT_JPG,				// JPG ��ʽ
	eLTE_SNAPSHOT_BMP				// BMP ��ʽ
} eLTE_SNAPSHOT_FORMAT_E;

class MediaPlayer
{
public:
	MediaPlayer();
	virtual ~MediaPlayer();

	//���ò��Ŵ���
	ELTE_VOID SetPlayHwnd(HWND pHwnd);
	//������Ƶ��
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
