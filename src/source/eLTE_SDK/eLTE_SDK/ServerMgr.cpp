#include "stdafx.h"
#include "ServerMgr.h"
#include "eLTE_Error.h"
#include "eLTE_Log.h"
#include "eLTE_Xml.h"
#include "UserMgr.h"
#include "ConfigMgr.h"
#include "SDL.h"

#define SERVICE_NAME		"HUAWEI eSDK eLTE Service"

CServerMgr::CServerMgr()
{
	m_pUserMgr = NULL;
	m_bInit    = FALSE;
	m_bMediaBypass = FALSE;
}

CServerMgr::~CServerMgr()
{
	m_pUserMgr = NULL;
	m_bInit    = FALSE;
	m_bMediaBypass = FALSE;
}

ELTE_VOID CServerMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}

ELTE_INT32 CServerMgr::Init(ELTE_INT32 iMediaBypass)
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("User Mgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	if(TRUE == m_bInit)
	{
		return eLTE_SDK_ERR_SUCCESS;
	}
	m_bMediaBypass = iMediaBypass;
	//启动服务
	StartServer();
	//加载配置文件
	ELTE_INT32 iRet = CConfigMgr::Instance().Init();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Read ini file failed.");
		return iRet;
	}
	//设置服务器端口号
	SSL_Socket& sslSocket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	sslSocket.SetParam(CConfigMgr::Instance().GetServerPort(), CConfigMgr::Instance().GetServerIP());
	iRet = sslSocket.Init_SSL_Socket();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init ssl socket failed.");
		return iRet;
	}
	//建立连接
	iRet = sslSocket.BeginConnect();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Connect ssl socket failed.");
		return iRet;
	}
	// 初始化sdl
	if(!m_bMediaBypass)
	{
		if(SDL_Init(SDL_INIT_VIDEO)) 
		{   
			LOG_RUN_ERROR("Could not initialize SDL - %s\n", SDL_GetError());
			return eLTE_SDK_ERR_SDL_INIT;
		}
	}
	m_bInit = TRUE;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 CServerMgr::CleanUp() const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("User Mgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	//断开连接
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	socket.Uninit_SSL_Socket();
	//释放配置文件资源
	CConfigMgr::Instance().Exit();
	if(!m_bMediaBypass)
	{
		SDL_Quit();
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID CServerMgr::StartServer() const
{
	ELTE_ULONG ulErrorCode = 0;
	SERVICE_STATUS ServicesStatus;
	eSDK_MEMSET(&ServicesStatus, 0x0, sizeof(SERVICE_STATUS));
	LOG_RUN_INFO("Starting service...");
	//打开服务管理器
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS);
	SC_HANDLE scv = NULL;
	if(NULL != scm)
	{
		//打开服务
		scv = OpenService(scm, SERVICE_NAME, SERVICE_START | SERVICE_INTERROGATE | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if(NULL != scv)
		{
			//启动服务
			BOOL bRet = FALSE;
			if (m_bMediaBypass)
			{
				LPCSTR args[] = {"MediaBypass"};
				bRet = StartService(scv, 1, args);
			} 
			else
			{
				bRet = StartService(scv, 0, NULL);
			}
			
			if(FALSE == bRet)
			{
				ulErrorCode = GetLastError();
				if(ERROR_SERVICE_ALREADY_RUNNING == ulErrorCode)
				{
					LOG_RUN_INFO("Service already running.");
					CloseServiceHandle(scv);
					CloseServiceHandle(scm);
					scm = scv = NULL;
					return;//lint !e438
				}
				else
				{
					LOG_RUN_ERROR("Start service failed, errorcode is %lu.", ulErrorCode);
				}
			}
			else
			{
				LOG_RUN_INFO("Service start success.");
			}
			while(0 != QueryServiceStatus(scv,&ServicesStatus))
			{
				if(SERVICE_START_PENDING == ServicesStatus.dwCurrentState)
				{
					Sleep(100);
				}
				else
				{
					break;
				}
			}
			CloseServiceHandle(scv);
		}
		else
		{
			LOG_RUN_ERROR("Open service failed, please install service. Error=%d\n",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else{
		LOG_RUN_ERROR("Open scmanger failed. Error=%d",GetLastError());
	}
	scm = scv = NULL;
}//lint !e438

ELTE_VOID CServerMgr::StopServer() const
{
	SERVICE_STATUS ServicesStatus;
	eSDK_MEMSET(&ServicesStatus, 0x0, sizeof(SERVICE_STATUS));
	LOG_RUN_INFO("Stoping service...");
	//打开服务管理器
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS);
	SC_HANDLE scv = NULL;
	if(NULL != scm)
	{
		//打开服务
		scv = OpenService(scm, SERVICE_NAME, SERVICE_START | SERVICE_INTERROGATE | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if(NULL != scv)
		{
			QueryServiceStatus(scv,&ServicesStatus);  
			if(SERVICE_RUNNING == ServicesStatus.dwCurrentState)
			{  
				ControlService(scv,SERVICE_CONTROL_STOP,&ServicesStatus);  
			} 
			while(0 != QueryServiceStatus(scv,&ServicesStatus))
			{
				if(SERVICE_STOPPED != ServicesStatus.dwCurrentState)
				{
					Sleep(100);
				}
				else
				{
					break;
				}
			}
			CloseServiceHandle(scv);
			LOG_RUN_INFO("Success close service.");
		}
		else
		{
			LOG_RUN_ERROR("Open service failed, please install service. Error=%d\n",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else{
		LOG_RUN_ERROR("Open scmanger failed. Error=%d",GetLastError());
	}
	scm = scv = NULL;
}//lint !e438

ELTE_INT32 CServerMgr::ServerIsRunning() const
{
	SERVICE_STATUS ServicesStatus;
	eSDK_MEMSET(&ServicesStatus, 0x0, sizeof(SERVICE_STATUS));
	//打开服务管理器
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS);
	SC_HANDLE scv = NULL;
	if(NULL != scm)
	{
		//打开服务
		scv = OpenService(scm, SERVICE_NAME, SERVICE_START | SERVICE_INTERROGATE | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if(NULL != scv)
		{
			QueryServiceStatus(scv,&ServicesStatus);  
			if(SERVICE_RUNNING == ServicesStatus.dwCurrentState)
			{  
				CloseServiceHandle(scv);
				CloseServiceHandle(scm);
				scm = scv = NULL;
				return TRUE;//lint !e438
			} 

			CloseServiceHandle(scv);
		}
		else
		{
			LOG_RUN_ERROR("Open service failed, please install service. Error=%d\n",GetLastError());
		}
		CloseServiceHandle(scm);
	}
	else{
		LOG_RUN_ERROR("Open scmanger failed. Error=%d",GetLastError());
	}
	scm = scv = NULL;
	return FALSE;//lint !e438
}
