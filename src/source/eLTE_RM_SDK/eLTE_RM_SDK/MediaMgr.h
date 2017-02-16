/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
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
