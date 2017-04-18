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
#include "AppContextMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"


AppContextMgr::AppContextMgr(void)
	: m_pDisplayMgr(NULL)
	, m_pMediaMgr(NULL)
	, m_uiBypass (0)
	, m_pGISReportMgr(NULL)
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
	if (NULL != m_pGISReportMgr)
	{
		try
		{
			delete m_pGISReportMgr;
			m_pGISReportMgr = NULL;
		}
		catch ( ... )
		{
			m_pGISReportMgr = NULL;
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

	// ���ù���·��
	char srcWorkpath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, srcWorkpath);
	std::string workpath = eLTE_Tool::GetServicePath();
	SetCurrentDirectory(workpath.c_str());
	LOG_RUN_DEBUG("Work path is %s", workpath.c_str());
	// �޸Ļ�������
	if (!ChangeEnvironmentVariable())
	{
		LOG_RUN_ERROR("ChangeEnvironmentVariable failed.");
	}

	// ��ʼ��ACE
	ACE::init();

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	pAC->setWorkMode(APPMODE_DISPATCH_CONSOLE);
	if (1 == m_uiBypass)
	{
		//the third part handle both video and audio
		pAC->setBypassBuildMedia(true);
	}
	else if (0 == m_uiBypass || 3 == m_uiBypass)
	{
		if (0 == m_uiBypass)
		{
			//SDK handle both video and audio
			pAC->setBypassBuildMedia(false);
		}
		else if (3 == m_uiBypass)
		{
			//video sdk ����audio ����������
			pAC->setBypassBuildMedia(true, false);
		}
		if (NULL == m_pMediaMgr)
		{
			m_pMediaMgr = new MediaMgr();
			if (NULL == m_pMediaMgr)//lint !e774
			{
				LOG_RUN_ERROR("m_pMediaMgr is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}
		}
		MediaManager* pMediaManager = dynamic_cast<MediaManager*>(m_pMediaMgr);
		if (NULL == pMediaManager)
		{
			LOG_RUN_ERROR("pMediaManager is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		pAC->setMediaManager(pMediaManager);
	}
	else if (2 == m_uiBypass)
	{
		//audio sdk ����video ����������
		pAC->setBypassBuildMedia(false, true);
	}
	else
	{
		LOG_RUN_ERROR("Bypass Value out of range.");
	}

	if (NULL == m_pDisplayMgr)
	{
		m_pDisplayMgr = new DisplayMgr();
		if (NULL == m_pDisplayMgr)//lint !e774
		{
			LOG_RUN_ERROR("m_pDisplayMgr is null.");
			return eLTE_SVC_ERR_MEMORY_APPLY;
		}
	}
	DisplayManager* pDisplayManager = dynamic_cast<DisplayManager*>(m_pDisplayMgr);
	if (NULL == pDisplayManager)
	{
		LOG_RUN_ERROR("pDisplayManager is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	pAC->setDisplayManager(pDisplayManager);

	if (NULL == m_pGISReportMgr)
	{
		m_pGISReportMgr = new GISReportMgr();
		if (NULL == m_pGISReportMgr)//lint !e774
		{
			LOG_RUN_ERROR("m_pGISReportMgr is null.");
			return eLTE_SVC_ERR_MEMORY_APPLY;
		}
	}
	GISReportManager* pGISReportMgr = dynamic_cast<GISReportManager*>(m_pGISReportMgr);
	if (NULL == pGISReportMgr)
	{
		LOG_RUN_ERROR("pGISReportMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	pAC->setReportManager(pGISReportMgr);

	// ��ʼ��SDK�������־������SDK����ʱ�������־
	ubp::platform::trace::UBPTrace* pUBP = ubp::platform::trace::UBPTrace_T::instance();
	if (NULL != pUBP)
	{
		pUBP->Init("DC");
	}	

	// ��ʼ��SDK
	int iRet = pAC->initApp();
	if (0 != iRet)
	{
		LOG_RUN_ERROR("initApp failed. (%d)", iRet);
	}

	// ��ԭ����·��
	//SetCurrentDirectory(srcWorkpath);

	return iRet;
}

int AppContextMgr::Exit() const
{
	LOG_TRACE();

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// �˳�SDK
	int iRet = pAC->exitApp();
	if (0 != iRet)
	{
		LOG_RUN_ERROR("exitApp failed. (%d)", iRet);
	}

	// ȥ��ʼ��ACE
	ACE::fini();

	return iRet;
}

bool AppContextMgr::ChangeEnvironmentVariable() const
{
	// Get the eLTE SDK directory
	const int BUFSIZE = 4096;
	const char* VARNAME = "EASY_INSTALL";
	std::string strPath = eLTE_Tool::GetServicePath();

	// ���뻺�汣�滷������
	char* pOldVal = (char*)malloc(BUFSIZE*sizeof(char));
	if (NULL == pOldVal)
	{
		return false;
	}
	eSDK_MEMSET(pOldVal, 0, BUFSIZE*sizeof(char));

	// ��ȡ��������
	DWORD dwRet = GetEnvironmentVariable(VARNAME, pOldVal, BUFSIZE);
	if (0 == dwRet)
	{
		DWORD dwErr = GetLastError();
		if(ERROR_ENVVAR_NOT_FOUND == dwErr) // ��������EASY_INSTALL������
		{
			eSDK_MEMSET(pOldVal, 0, BUFSIZE*sizeof(char));
		}
		else
		{
			free(pOldVal);
			return false;
		}
	}
	else if (BUFSIZE < dwRet) // ���治��
	{
		// ���������ڴ�
		pOldVal = (char*)realloc(pOldVal, dwRet*sizeof(char));
		if (NULL == pOldVal)
		{
			return false;
		}
		eSDK_MEMSET(pOldVal, 0, dwRet*sizeof(char));

		// ��ȡ��������
		dwRet = GetEnvironmentVariable(VARNAME, pOldVal, dwRet);
		if (0 == dwRet)
		{
			free(pOldVal);
			return false;
		}
	}

	// ��������������("" == pOldVal)������ֵ����ȷ
	if (0 != std::strcmp(pOldVal, strPath.c_str()))
	{
		if (0 == SetEnvironmentVariable(VARNAME, strPath.c_str()))
		{
			// ����/�޸Ļ�������ʧ��
			free(pOldVal);
			return false;
		}
	}

	free(pOldVal);
	return true;
}
