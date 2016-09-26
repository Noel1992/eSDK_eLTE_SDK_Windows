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

/********************************************************************
filename    :    VideoRender.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDK视频绘制类。通过注册该类将视频数据传送到上层应用
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 初始版本
*********************************************************************/

#ifndef __ELTE_VIDEO_RENDER_H__
#define __ELTE_VIDEO_RENDER_H__

#include "mediamanager.h"
#include "SharedMemory.h"

class VideoRender : public VideoRenderer
{
public:
	VideoRender(void);
	VideoRender(const int& resId);
	virtual ~VideoRender(void);//lint !e1510

	// 设置视频帧尺寸.
	virtual void SetSize(int width, int height, int reserved);

	//设置视频帧对应的REsourceID
	virtual void SetResourceID(MSISDN logical, MSISDN physical);

	// 设置render所服务的videochannel
	virtual void SetVideoChannel(void* vc);

	// 绘制当前YUV视频帧.非const表示可能会修改YuvFrame的方向，对称性等
	virtual bool RenderYUVFrame(YuvFrame* frame, MSISDN physicalRid);

	virtual RenderControl* getRenderControl();
	virtual MSISDN getPhysical();
	virtual MSISDN getLogical();
	virtual void stopRendering();

private:
	int            m_iResId;
	SharedMemory*  m_pSharedMemory;
	int            m_bInitSuccess;
};

#endif
