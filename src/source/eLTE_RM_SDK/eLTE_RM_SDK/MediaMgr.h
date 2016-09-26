/********************************************************************
filename		: 	MediaMgr.h
author			:	zWX229156
created		    :	2015/10/14
description	    :	媒体功能管理类（开始播放实时视频、开始云台控制以及停止播放实时视频）
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/14 初始版本
*********************************************************************/

#ifndef __MEDIA_MGR_H__
#define __MEDIA_MGR_H__

#include "eLTE_Types.h"

class CUserMgr;//lint !e763
class CMediaMgr
{
public:
	CMediaMgr();
	~CMediaMgr();

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//开始播放实时视频
	ELTE_INT32 StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	// 开始云台控制
	ELTE_INT32 PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue) const;

	// 停止播放实时视频
	ELTE_INT32 StopRealPlay(const ELTE_CHAR* pResourceID) const;

private:
	CUserMgr*    m_pUserMgr;
};

#endif // __MEDIA_MGR_H__
