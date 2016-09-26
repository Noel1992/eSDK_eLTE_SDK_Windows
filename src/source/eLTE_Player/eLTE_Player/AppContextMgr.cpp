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

#include "StdAfx.h"
#include "AppContextMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "eLTE_PlayerCtrl.h"


AppContextMgr::AppContextMgr(void)
	: m_pDisplayMgr(NULL)
	, m_pMediaMgr(NULL)
	, m_pGisReportMgr(NULL)
{
}


AppContextMgr::~AppContextMgr(void)
{
	if (NULL != m_pDisplayMgr)
	{
		try
		{
			delete m_pDisplayMgr;
			m_pDisplayMgr = NULL;
		}
		catch ( ... )
		{
			m_pDisplayMgr = NULL;
		}
	}

	if (NULL != m_pMediaMgr)
	{
		try
		{
			delete m_pMediaMgr;
			m_pMediaMgr = NULL;
		}
		catch ( ... )
		{
			m_pMediaMgr = NULL;
		}
	}

	if (NULL != m_pGisReportMgr)
	{
		try
		{
			delete m_pGisReportMgr;
			m_pGisReportMgr = NULL;
		}
		catch ( ... )
		{
			m_pGisReportMgr = NULL;
		}
	}
}


AppContextMgr& AppContextMgr::Instance()
{
	static AppContextMgr s_AppContextMgr;
	return s_AppContextMgr;
}

int AppContextMgr::Init()
{
	LOG_TRACE();

	// 设置工作路径
	TCHAR srcWorkpath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, srcWorkpath);
	CString workpath = eLTE_Tool::GetOcxPath();
	SetCurrentDirectory(workpath);

	// 初始化ACE
	ACE::init();

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}
	pAC->setWorkMode(APPMODE_DISPATCH_CONSOLE);
	if (CeLTE_PlayerCtrl::m_ulBypass)
	{
		pAC->setBypassBuildMedia(true);
	}
	else
	{
		pAC->setBypassBuildMedia(false);
		if (NULL == m_pMediaMgr)
		{
			m_pMediaMgr = new MediaMgr();
			if (NULL == m_pMediaMgr)//lint !e774
			{
				LOG_RUN_ERROR("m_pMediaMgr is null.");
				return eLTE_ERR_FAILED;
			}
		}
		MediaManager* pMediaManager = dynamic_cast<MediaManager*>(m_pMediaMgr);
		if (NULL == pMediaManager)
		{
			LOG_RUN_ERROR("pMediaManager is null.");
			return eLTE_ERR_FAILED;
		}
		pAC->setMediaManager(pMediaManager);
	}
	

	if (NULL == m_pDisplayMgr)
	{
		m_pDisplayMgr = new DisplayMgr();
		if (NULL == m_pDisplayMgr)//lint !e774
		{
			LOG_RUN_ERROR("m_pDisplayMgr is null.");
			return eLTE_ERR_FAILED;
		}
	}
	DisplayManager* pDisplayManager = dynamic_cast<DisplayManager*>(m_pDisplayMgr);
	if (NULL == pDisplayManager)
	{
		LOG_RUN_ERROR("pDisplayManager is null.");
		return eLTE_ERR_FAILED;
	}
	pAC->setDisplayManager(pDisplayManager);

	if (NULL == m_pGisReportMgr)
	{
		m_pGisReportMgr = new GisReportMgr();
		if (NULL == m_pGisReportMgr)//lint !e774
		{
			LOG_RUN_ERROR("m_pGisReportMgr is null.");
			return eLTE_ERR_FAILED;
		}
	}
	GISReportManager* pGISReportManager = dynamic_cast<GISReportManager*>(m_pGisReportMgr);
	if (NULL == pGISReportManager)
	{
		LOG_RUN_ERROR("pGISReportManager is null.");
		return eLTE_ERR_FAILED;
	}
	pAC->setReportManager(pGISReportManager);

	// 初始化SDK的输出日志，否则SDK运行时不输出日志
	ubp::platform::trace::UBPTrace* pUBP = ubp::platform::trace::UBPTrace_T::instance();
	if (NULL != pUBP)
	{
		pUBP->Init("DC");
	}	

	// 初始化SDK
	int iRet = pAC->initApp();
	if (0 != iRet)
	{
		LOG_RUN_ERROR("initApp failed. (%d)", iRet);
	}

	// 还原工作路径
	SetCurrentDirectory(srcWorkpath);

	return iRet;
}

int AppContextMgr::Exit() const
{
	LOG_TRACE();

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// 退出SDK
	int iRet = pAC->exitApp();
	if (0 != iRet)
	{
		LOG_RUN_ERROR("exitApp failed. (%d)", iRet);
	}

	// 去初始化ACE
	ACE::fini();

	return iRet;
}

void AppContextMgr::SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl)
{
	if (NULL != m_pDisplayMgr)
	{
		m_pDisplayMgr->SetCeLTE_PlayerCtrl(pCeLTE_PlayerCtrl);
	}

	if (NULL != m_pGisReportMgr)
	{
		m_pGisReportMgr->SetCeLTE_PlayerCtrl(pCeLTE_PlayerCtrl);
	}
}
