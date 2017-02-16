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
#include "stdafx.h"
#include <winsock2.h>
#include <process.h>

//Socket manage class
#include "SSL_Socket.h"
//SSL manage class
#include "OpenSSL_Mgr.h"
//log manage class 
#include "eLTE_Log.h"
//Error Code define class
#include "eLTE_Error.h"
//user manage class 
#include "UserMgr.h"
//Json definition class 
#include "json.h"


#pragma comment(lib, "ws2_32.lib")
#define TRY_CONNECT_NUM 6

SSL_Socket::SSL_Socket(void)
	: m_uiPort(0)
	, m_strHost("")
	, m_socket(INVALID_SOCKET)
	, m_pUserMgr(NULL)	
{
	
}

SSL_Socket::~SSL_Socket(void)
{
	m_pUserMgr = NULL;
	try
	{
		(void)CloseResource();
	}
	catch (...)
	{
		
	}
}//lint !e1579

ELTE_INT32 SSL_Socket::SendMsg(const std::string& reqXmlStr)
{
	LOG_TRACE();
	ELTE_UINT32 uiLength=0;

	if(reqXmlStr.empty())
	{
		return eLTE_SDK_ERR_SUCCESS;
	}

	ELTE_CHAR cLenStr[SOCKET_RECV_BUFFRM_LEN_SIZE + 1];
	eSDK_MEMSET (cLenStr, 0, sizeof(cLenStr));
	std::string strSrc = reqXmlStr;
	int iStrLen = reqXmlStr.length();
	if (-1 == sprintf_s(cLenStr, "%06x", iStrLen))
	{
		LOG_RUN_ERROR("sprintf_s run error!");
	}
	
	std::string strSend(cLenStr);//加上包长
	strSend.append(strSrc);
	
	uiLength = strSend.length();
	if (uiLength > 0)
	{
		ELTE_INT32 iRet = OpenSSL_Mgr::Instance()->SendMsg(strSend, uiLength);

		if(iRet != eLTE_SDK_ERR_SUCCESS)
		{
			LOG_RUN_ERROR("Send msg failed.");
			return iRet;
		}
	}
	else
	{
		LOG_RUN_ERROR("Send message length error!");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}

	
	
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SSL_Socket::SetParam(ELTE_UINT32 uiPort, const std::string& strHost)
{
	m_uiPort = uiPort;
	m_strHost = strHost;
}

ELTE_VOID SSL_Socket::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
	OpenSSL_Mgr::Instance()->SetUserMgr(pUserMgr);
}

ELTE_INT32 SSL_Socket::Init_SSL_Socket()
{
	LOG_TRACE();
	// 状态校验
	if (INVALID_SOCKET != m_socket)
	{
		LOG_RUN_INFO("Socket is exist.");
		return eLTE_SDK_ERR_SUCCESS;
	}

	// 初始化套结字动态库
	WSADATA _wsd;
	if (0 != WSAStartup(MAKEWORD(2,2), &_wsd))
	{
		LOG_RUN_ERROR("WSAStartup failed.");
		return eLTE_SDK_ERR_WSA_START_UP;
	}

	// 初始化openssl库
	ELTE_INT32 iRet = OpenSSL_Mgr::Instance()->Init_SSL_CTX();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		WSACleanup();
		LOG_RUN_ERROR("Init_SSL_CTX failed.");
		return iRet;
	}

	// 创建套接字
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		WSACleanup();
		LOG_RUN_ERROR("Create socket failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	return eLTE_SDK_ERR_SUCCESS;
}


ELTE_INT32 SSL_Socket::Uninit_SSL_Socket()
{
	LOG_TRACE();
	m_uiPort = 0;
	m_strHost = "";

	return CloseResource();
}

ELTE_INT32 SSL_Socket::ConnectSSL(const ELTE_INT32& bConnectAgain)
{
	LOG_TRACE();
	static ELTE_INT32 bInitAddr = FALSE;
	static SOCKADDR_IN _serverAddr;
	// 状态校验
	if (INVALID_SOCKET == m_socket)
	{
		LOG_RUN_ERROR("Please init socket first.");
		return eLTE_SDK_ERR_FAILED;
	}

	// 设置服务器地址
	if(!bInitAddr)
	{
		eSDK_MEMSET(&_serverAddr, 0, sizeof(SOCKADDR_IN));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_addr.s_addr = inet_addr(m_strHost.c_str());
		_serverAddr.sin_port = htons((ELTE_USHORT)m_uiPort);
		if(bConnectAgain)
		{
			bInitAddr = TRUE;
		}
	}

	// 连接服务器
	ELTE_INT32 iTryNum = 0;
	ELTE_INT32 iRet = 0;
	while (TRY_CONNECT_NUM > iTryNum)
	{
		iRet = connect(m_socket, (LPSOCKADDR)&_serverAddr, sizeof(SOCKADDR_IN));
		if(SOCKET_ERROR == iRet)
		{
			++iTryNum;
		}
		else
		{
			break;
		}
	}
	if(TRY_CONNECT_NUM <= iTryNum)
	{
		if(!bConnectAgain)
		{
			(void)CloseResource();
			LOG_RUN_ERROR("Try connect failed.");
		}
		return eLTE_SDK_ERR_CONNECT_SERVER;
	}
	// SSL连接服务器
	iRet = OpenSSL_Mgr::Instance()->Connect_SSL((int)m_socket);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		if(!bConnectAgain)
		{
			(void)CloseResource();
		}
		LOG_RUN_ERROR("Connect_SSL failed. %d", GetLastError());
		return iRet;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SSL_Socket::BeginConnect(const ELTE_INT32& bConnectAgain)
{
	LOG_TRACE();
	ELTE_INT32 iRet = ConnectSSL(bConnectAgain);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		return iRet;
	}
	// SSL服务器读取数据，这里会阻塞线程
	iRet = OpenSSL_Mgr::Instance()->StartRevMsg();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		(void)CloseResource();
		LOG_RUN_ERROR("Start rev msg failed.");
		return iRet;
	}

	iRet = OpenSSL_Mgr::Instance()->StartProMsg();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		(void)CloseResource();
		LOG_RUN_ERROR("Start pro msg failed.");
		return iRet;
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SSL_Socket::KeepAlive()
{
	LOG_TRACE();
	if (NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("m_pUserMgr is NULL!");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	Json::Value value;
	Json::FastWriter writer;
	value["cmd"] = "1FFFFF";	
	value["session"] = m_pUserMgr->GetSessionId();
	std::string strKeepAlive = writer.write( value );
	ELTE_INT32 iRet = SendMsg(strKeepAlive);
	
	return iRet;
}

//闪断重连
ELTE_INT32 SSL_Socket::ConnectAgain()
{
	ELTE_INT32 iRet = 0;
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	//socket去除初始化
	if (INVALID_SOCKET != m_socket)
	{
		//关闭套接字
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		//释放套接字资源
		WSACleanup();
	}

	//重新初始化socket
	iRet = Init_SSL_Socket();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Init ssl socket failed.");
		return iRet;
	}

	//设置链接参数
	static SOCKADDR_IN _serverAddr;
	eSDK_MEMSET(&_serverAddr, 0, sizeof(SOCKADDR_IN));
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = inet_addr(m_strHost.c_str());
	_serverAddr.sin_port = htons((ELTE_USHORT)m_uiPort);
	//尝试链接
	iRet = connect(m_socket, (LPSOCKADDR)&_serverAddr, sizeof(SOCKADDR_IN));
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("ConnectAgain :: Connect ssl socket failed.");

		return iRet;
	}
	else
	{
		if (INVALID_SOCKET != m_socket)
		{
			//关闭套接字
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			//释放套接字资源
			WSACleanup();
		}
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SSL_Socket::CleanBuffer()
{
	ELTE_INT32 iRet=0;
	iRet = setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,(char FAR *)iRet,sizeof(ELTE_INT32));
	if (0 != iRet)
	{
		LOG_RUN_ERROR("socket: setsockopt error accured!");
	}
	return iRet;
}

ELTE_INT32 SSL_Socket::CloseResource()
{
	
	// SSL去初始化
	ELTE_INT32 iRet = OpenSSL_Mgr::Instance()->Uninit_SSL_CTX();

	if (INVALID_SOCKET != m_socket)
	{
		//关闭套接字
		closesocket(m_socket);		
		m_socket = INVALID_SOCKET;
		//释放套接字资源
		WSACleanup();		
	}

	return iRet;
}

