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

// eLTE_NativeService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsvc.h>
#include <iostream>
#include "eLTE_NativeService.h"
#include "SSL_Socket.h"
#include "Logic_Queue.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "eLTE_Error.h"
#include "ConfigMgr.h"
#include "AppContextMgr.h"
#include "SessionMgr.h"


SERVICE_STATUS_HANDLE g_hSvcCtrl = NULL;
SERVICE_STATUS g_svc_status;

int _tmain(int argc, char* argv[])//lint !e752 !e1768 !e1727
{
	//打开日志
	std::string strIniPath = eLTE_Tool::GetServicePath();
	strIniPath.append(LOG_INI_FILE_NAME);
	unsigned int logLevel[LOG_CATEGORY] = {INVALID_LOG_LEVEL,INVALID_LOG_LEVEL,INVALID_LOG_LEVEL};
	std::string strLogPath = eLTE_Tool::GetServicePath();
	strLogPath.append("log");
	int iRet = LOG_INIT(strIniPath.c_str(), logLevel, strLogPath.c_str());
	if(RET_SUCCESS != iRet)
	{
		return eLTE_SDK_ERR_LOG_INIT;
	}
	
	LOG_TRACE();

	if (INSTALL_AGRC == argc && NULL != argv)
	{
		if (0 == _stricmp(argv[1], "/install") || 0 == _stricmp(argv[1], "-install"))
		{
			InstallService();
		}
		else if (0 == _stricmp(argv[1], "/uninstall") || 0 == _stricmp(argv[1], "-uninstall"))
		{
			UninstallService();
		}
	}
	else
	{
		SERVICE_TABLE_ENTRY svcTable[2];
		svcTable[0].lpServiceName = MY_SERVICE_NAME;
		svcTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;
		svcTable[1].lpServiceName = NULL;
		svcTable[1].lpServiceProc = NULL;

		if (!StartServiceCtrlDispatcher(svcTable))
		{
			LOG_RUN_ERROR("StartServiceCtrlDispatcher failed.");
		}
	}

	// 用于直接以普通应用程序的形式启动服务器，方便debug调试服务程序业务

	// 初始化SDK
	//int iRet = AppContextMgr::Instance().Init();
	//if (eLTE_SVC_ERR_SUCCESS != iRet)
	//{
	//	LOG_RUN_ERROR("Init elte sdk failed.(%d)", iRet);
	//}
	//else
	//{
	//	// 开启业务监听
	//	StartRun();

	//	// 退出SDK
	//	iRet = AppContextMgr::Instance().Exit();
	//	if (eLTE_SVC_ERR_SUCCESS != iRet)
	//	{
	//		LOG_RUN_ERROR("Exit elte sdk failed.(%d)", iRet);
	//	}
	//}

	LOG_EXIT();
	return 0;
}//lint !e818

VOID InstallService()
{
	LOG_TRACE();
	SC_HANDLE hSCMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCMgr)
	{
		std::cout << "Install service failed! (OpenSCManager return null. GetLastError == " << GetLastError() << ")" << std::endl;
		LOG_RUN_ERROR("OpenSCManager failed. (%d)", GetLastError());
		return;
	}

	SC_HANDLE hSvc = CreateService(hSCMgr,
		MY_SERVICE_NAME,
		MY_SERVICE_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		eLTE_Tool::GetServicePathEx().c_str(),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
		);
	if (NULL == hSvc)
	{
		std::cout << "Install service failed. (CreateService return null. GetLastError == " << GetLastError() << ")" << std::endl;
		LOG_RUN_ERROR("CreateService failed. (%d)", GetLastError());
		CloseServiceHandle(hSCMgr);
		return;
	}

	LOG_RUN_DEBUG("Install service success.");
	std::cout << "Install service success." << std::endl;
	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCMgr);
}

VOID UninstallService()
{
	LOG_TRACE();
	SC_HANDLE hSCMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCMgr)
	{
		std::cout << "Uninstall service failed. (OpenSCManager return null. GetLastError == " << GetLastError() << ")" << std::endl;
		LOG_RUN_ERROR("OpenSCManager failed. (%d)", GetLastError());
		return;
	}

	SC_HANDLE hSvc = OpenService(hSCMgr,MY_SERVICE_NAME, SERVICE_STOP | DELETE);
	if (NULL == hSvc)
	{
		std::cout << "Uninstall service failed. (OpenService return null. GetLastError == " << GetLastError() << ")" << std::endl;
		LOG_RUN_ERROR("CreateService failed. (%d)", GetLastError());
		CloseServiceHandle(hSCMgr);
		return;
	}

	//停止服务
	ControlService(hSvc, SERVICE_CONTROL_STOP, &g_svc_status);
	//删除服务
	if (DeleteService(hSvc))
	{
		LOG_RUN_DEBUG("Uninstall service success.");
		std::cout << "Uninstall service success." << std::endl;
	}
	else
	{
		LOG_RUN_DEBUG("DeleteService failed. (%d)", GetLastError());
		std::cout << "Uninstall service failed. (DeleteService return false. GetLastError == " << GetLastError() << ")" << std::endl;
	}

	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCMgr);
}

VOID WINAPI ServiceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors)
{
	LOG_TRACE();
	if (1 < dwNumServicesArgs && 0 == strcmp("MediaBypass", lpServiceArgVectors[1]))
	{
		AppContextMgr::Instance().SetBypass(TRUE);
	}

	g_hSvcCtrl = RegisterServiceCtrlHandler(MY_SERVICE_NAME, ControlHandler);
	if (NULL == g_hSvcCtrl)
	{
		LOG_RUN_ERROR("RegisterServiceCtrlHandler failed.");
		return;
	}

	// SERVICE_START_PENDING
	g_svc_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_svc_status.dwCurrentState = SERVICE_START_PENDING;
	g_svc_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_svc_status.dwWin32ExitCode = NO_ERROR;
	g_svc_status.dwServiceSpecificExitCode = 0;
	g_svc_status.dwCheckPoint = 0;
	g_svc_status.dwWaitHint = 0;
	SetServiceStatus(g_hSvcCtrl, &g_svc_status);
	LOG_RUN_DEBUG("SetServiceStatus SERVICE_START_PENDING");

	// 初始化SDK
	int iRet = AppContextMgr::Instance().Init();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init elte sdk failed.(%d)", iRet);
	}
	else
	{
		// SERVICE_RUNNING
		g_svc_status.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(g_hSvcCtrl, &g_svc_status);
		LOG_RUN_DEBUG("SetServiceStatus SERVICE_RUNNING");

		// 开启业务监听
		StartRun();

		// 退出SDK
		iRet = AppContextMgr::Instance().Exit();
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Exit elte sdk failed.(%d)", iRet);
		}
	}

	// SERVICE_STOPPED
	g_svc_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hSvcCtrl, &g_svc_status);
	LOG_RUN_DEBUG("SetServiceStatus SERVICE_STOPPED");
}//lint !e818

VOID WINAPI ControlHandler(DWORD dwControl)
{
	LOG_TRACE();
	INFO_PARAM1(dwControl);
	switch (dwControl)
	{
	case SERVICE_CONTROL_STOP:
		{
			g_svc_status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(g_hSvcCtrl, &g_svc_status);
			SSL_Socket::Instance().m_bStopSvc = true;
		}
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		{
			g_svc_status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(g_hSvcCtrl, &g_svc_status);
			SSL_Socket::Instance().m_bStopSvc = true;
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid service control.");
		}
		break;
	}
}

void StartRun()
{
	LOG_TRACE();

	// 初始化配置文件和默认参数配置
	int iRet = ConfigMgr::Instance().Init();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init ConfigMgr failed.");
		return;
	}

	// 初始化逻辑队列
	iRet = Logic_Queue::Instance().Init();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init logic queue failed.");
		return;
	}

	// 初始化网络套接字
	iRet = SSL_Socket::Instance().Init_SSL_Socket();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init ssl socket failed.");
		// 停止服务
		SSL_Socket::Instance().m_bStopSvc = true;
		// 清理逻辑队列
		Logic_Queue::Instance().Uninit();
		// 清理网络套接字
		SSL_Socket::Instance().Uninit_SSL_Socket();
		return;
	}

	// 开启网络监听
	iRet = SSL_Socket::Instance().BeginAccept();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Accept ssl socket failed.");	
	}

	// 清理逻辑队列
	Logic_Queue::Instance().Uninit();
	// 清理网络套接字
	SSL_Socket::Instance().Uninit_SSL_Socket();
	// 停止保活
	SessionMgr::Instance().KeepAliveStop();
}
