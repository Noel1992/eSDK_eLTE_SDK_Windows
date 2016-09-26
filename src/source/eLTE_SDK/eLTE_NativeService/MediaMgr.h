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
filename    :    MediaMgr.h
author      :    zhangziqian wx229156
created     :    2015/06/16
description :    eLTE SDKý���ࡣͨ��ע�����������ý�������Լ�ý�����ָ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/06/16 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_MEDIA_MGR_H__
#define __ELTE_MEDIA_MGR_H__

#include "VideoRender.h"

class MediaMgr : public MediaManager
{
public:
	MediaMgr(void);
	virtual ~MediaMgr(void);//lint !e1510

public:
	virtual bool isMultiResource(MSISDN resID);
	virtual int getLogicalResid(MSISDN physicalResid);
	virtual VideoRenderer* getRender(MSISDN resID);
	virtual VideoRenderer* getRender4MultiResource(MSISDN resID);
	virtual VideoRenderer* new_renderer4MultiResource(MSISDN resID);

	// to construct one VideoRenderer object for a video resource.
	virtual VideoRenderer* new_renderer( MSISDN resID);

	// to Destruct one VideoRenderer object for a video resource.
	virtual void del_renderer( MSISDN resID);

private:
	std::map<int, VideoRender*> m_videoRenders;
};

#endif
