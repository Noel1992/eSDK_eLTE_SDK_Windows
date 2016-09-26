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
#include "MediaMgr.h"
#include "eLTE_Log.h"

MediaMgr::MediaMgr(void)
{
}


MediaMgr::~MediaMgr(void)
{
}


bool MediaMgr::isMultiResource(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);

	return false;
}

int MediaMgr::getLogicalResid(MSISDN physicalResid)
{
	LOG_TRACE();
	INFO_PARAM1(physicalResid);

	return physicalResid;
}

VideoRenderer* MediaMgr::getRender(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);

	std::map <int, VideoRender*>::iterator iter = m_videoRenders.find(resID);
	if(iter == m_videoRenders.end())
	{
		return NULL;
	}

	return iter->second;
}

VideoRenderer* MediaMgr::getRender4MultiResource(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);

	return NULL;
}

VideoRenderer* MediaMgr::new_renderer4MultiResource(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);

	return NULL;
}

// to construct one VideoRenderer object for a video resource.
VideoRenderer* MediaMgr::new_renderer(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);
	std::map <int, VideoRender*>::iterator iter = m_videoRenders.find(resID);
	VideoRender* pVideoRender = NULL;
	if(iter == m_videoRenders.end())
	{
		pVideoRender = new VideoRender(resID);
		if (NULL == pVideoRender)
		{
			return NULL;
		}
		(void)m_videoRenders.insert(std::pair <int, VideoRender*> (resID, pVideoRender));
		return pVideoRender;
	}
	return iter->second;
}

// to Destruct one VideoRenderer object for a video resource.
void MediaMgr::del_renderer(MSISDN resID)
{
	LOG_TRACE();
	INFO_PARAM1(resID);

	std::map <int, VideoRender*>::iterator iter = m_videoRenders.find(resID);
	if(iter == m_videoRenders.end())
	{
		return;
	}
	delete iter->second;

	m_videoRenders.erase(iter);
}
