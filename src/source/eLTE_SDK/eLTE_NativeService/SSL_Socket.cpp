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
#include <winsock2.h>
#include <algorithm>
#include <process.h>
#include "SSL_Socket.h"
#include "OpenSSL_Mgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"


#pragma comment(lib, "ws2_32.lib")

#define CLIENT_NUMBER	5

//lint -e438
// 读取错误
#define CHECK_SSL_READ(_ssl, _size, Packet, _error) {\
	if (0 >= _size)\
	{\
		int _ssl_err = SSL_get_error(_ssl,_size);\
		if (0 == _size && (SSL_ERROR_ZERO_RETURN == _ssl_err || SSL_ERROR_SYSCALL == _ssl_err))\
		{\
			LOG_RUN_INFO("client %d disconnect.", _socket);\
		}\
		else\
		{\
			LOG_RUN_ERROR("SSL_read failed, thread exit. (iReadSize = %d, SSL_get_error = %d)", iReadSize, _ssl_err);\
		}\
		RELEASE_PACKET(Packet);\
		_error = true;\
		break;\
	}\
}


SSL_Socket::SSL_Socket(void)
	: m_uiPort(0)
	, m_strHost("")
	, m_socketSrv(INVALID_SOCKET)
	, m_hMutex(NULL)
	, m_uiSequenceNumber(0)
	, m_bKickOut(false)
	, m_bStopSvc(false)
{
	m_clientMap.clear();
}

SSL_Socket::~SSL_Socket(void)
{
	try
	{
		Clean();
	}
	catch(...)
	{

	}
}//lint !e1579

SSL_Socket& SSL_Socket::Instance()
{
	static SSL_Socket s_SSL_Socket;
	return s_SSL_Socket;
}

void SSL_Socket::SetParam(unsigned int uiPort, const std::string& strHost)
{
	//INFO_PARAM2(uiPort, strHost);
	m_uiPort = uiPort;
	m_strHost = strHost;
}

int SSL_Socket::Init_SSL_Socket()
{
	LOG_TRACE();

	// 创建互斥锁
	if (NULL == m_hMutex)
	{
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
		if (NULL == m_hMutex)
		{
			LOG_RUN_ERROR("CreateMutex failed.");
			return eLTE_SVC_ERR_FAILED;
		}
	}	

	// 状态校验
	if (INVALID_SOCKET != m_socketSrv)
	{
		LOG_RUN_ERROR("Socket is listening.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 初始化套结字动态库
	WSADATA _wsd;
	if (0 != WSAStartup(MAKEWORD(2,2), &_wsd))
	{
		LOG_RUN_ERROR("WSAStartup failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 初始化openssl库
	int iRet = OpenSSL_Mgr::Instance().Init_SSL_CTX();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		WSACleanup();
		LOG_RUN_ERROR("Init_SSL_CTX failed.");
		return iRet;
	}

	// 创建套接字
	m_socketSrv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socketSrv)
	{
		LOG_RUN_ERROR("Create socket failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 设置服务器地址
	SOCKADDR_IN _serverAddr;
	eSDK_MEMSET(&_serverAddr, 0, sizeof(SOCKADDR_IN));
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	_serverAddr.sin_port = htons((unsigned short)m_uiPort);

	// 绑定套接字
	iRet = bind(m_socketSrv, (LPSOCKADDR)&_serverAddr, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == iRet)
	{
		//关闭套接字
		closesocket(m_socketSrv);
		m_socketSrv = INVALID_SOCKET;
		//释放套接字资源
		WSACleanup();
		LOG_RUN_ERROR("bind failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 开始监听 
	iRet = listen(m_socketSrv, CLIENT_NUMBER);
	if(SOCKET_ERROR == iRet)
	{
		//关闭套接字
		closesocket(m_socketSrv);
		m_socketSrv = INVALID_SOCKET;
		//释放套接字资源
		WSACleanup();
		LOG_RUN_ERROR("listen failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

void SSL_Socket::Uninit_SSL_Socket()
{
	LOG_TRACE();
	Clean();
}

void SSL_Socket::Clean()
{
	m_uiPort = 0;
	m_strHost = "";
	// 是否客户端ssl和socket
	std::map<SSL*, HANDLE>::iterator itor = m_clientMap.begin();
	for ( ; itor != m_clientMap.end(); itor++)
	{
		SSL* _ssl = itor->first;
		HANDLE hThread = itor->second;
		SOCKET _socket = (SOCKET)SSL_get_fd(_ssl);
		if (INVALID_SOCKET != _socket)
		{
			// 关闭套接字
			closesocket(_socket);
		}
		if (NULL != _ssl)
		{
			// 关闭 SSL 连接
			SSL_shutdown(_ssl);
			// 释放 SSL
			SSL_free(_ssl);
		}
		if (NULL != hThread)
		{
			CloseHandle(hThread);
		}
	}
	m_clientMap.clear();

	OpenSSL_Mgr::Instance().Uninit_SSL_CTX();

	if (INVALID_SOCKET != m_socketSrv)
	{
		//关闭套接字
		closesocket(m_socketSrv);
		m_socketSrv = INVALID_SOCKET;
		//释放套接字资源
		WSACleanup();
	}

	if (NULL != m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	m_uiSequenceNumber = 0;
	m_bStopSvc = false;
	m_bKickOut = false;
}

int SSL_Socket::BeginAccept()
{
	LOG_TRACE();
	// 状态校验
	if (INVALID_SOCKET == m_socketSrv)
	{
		LOG_RUN_ERROR("Please openlisten first.");
		return eLTE_SVC_ERR_FAILED;
	}

	struct timeval _tval;
	_tval.tv_sec = 0;
	_tval.tv_usec = 500000;
	struct fd_set _fds;
	int maxfdp = (int)m_socketSrv+1;
	while (TRUE)//lint !e716
	{		
		FD_ZERO(&_fds);
		FD_SET(m_socketSrv, &_fds);
		int iSel = select(maxfdp, &_fds, NULL, NULL, &_tval);
		if (-1 == iSel)
		{
			LOG_RUN_ERROR("select failed.");
			continue;
		}
		if (FD_ISSET(m_socketSrv, &_fds))
		{
			// 等待客户端连上来
			SOCKADDR_IN _clientAddr;
			eSDK_MEMSET(&_clientAddr, 0, sizeof(SOCKADDR_IN));
			int iLenAddr = sizeof(SOCKADDR_IN);
			SOCKET _socketClient = accept(m_socketSrv, (LPSOCKADDR)&_clientAddr, &iLenAddr);
			if (INVALID_SOCKET == _socketClient)
			{
				LOG_RUN_ERROR("accept failed.");
				continue;
			}
			LOG_RUN_DEBUG("IP: %s, Port: %d, Socket: %d", inet_ntoa(_clientAddr.sin_addr), _clientAddr.sin_port, _socketClient);
			
			// 限制只能连接一个客户端，业务只能满足一个客户端。
			if (!m_clientMap.empty())
			{
				closesocket(_socketClient);
				_socketClient = INVALID_SOCKET;
				LOG_RUN_ERROR("Only have one client connect.");
				continue;
			}

			// SSL客户端连接
			SSL* _ssl = OpenSSL_Mgr::Instance().Accept_SSL((int)_socketClient);
			if (NULL == _ssl)
			{
				closesocket(_socketClient);
				_socketClient = INVALID_SOCKET;
				LOG_RUN_ERROR("Accept_SSL failed.");
				continue;
			}
			LOG_RUN_DEBUG("Accept_SSL a new client.");

			// 创建客户端处理线程
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ClientThread, (void*)_ssl, CREATE_SUSPENDED, NULL);
			if (NULL == hThread)
			{
				closesocket(_socketClient);
				_socketClient = INVALID_SOCKET;
				LOG_RUN_ERROR("_beginthreadex failed.");
				continue;
			}

			// 添加到客户端socket列表中
			if (NULL != m_hMutex)
			{
				(void)WaitForSingleObject(m_hMutex, INFINITE);
			}
			std::map<SSL*, HANDLE>::iterator itor = m_clientMap.find(_ssl);
			if (m_clientMap.end() != itor)
			{
				LOG_RUN_ERROR("The new client socket have been in the client socket list.");
			}
			else
			{
				(void)m_clientMap.insert(std::make_pair(_ssl, hThread));
			}
			if (NULL != m_hMutex)
			{
				ReleaseMutex(m_hMutex);
			}

			// 启动客户端线程
			ResumeThread(hThread);
		}
		else
		{
			Sleep(10);
		}
		// 服务停止判断
		if (m_bStopSvc)
		{
			LOG_RUN_INFO("Stop service.");
			break;
		}
	}
	
	// 获取所有客户端线程Handle
	HANDLE* pThreads = NULL;
	if (NULL != m_hMutex)
	{
		(void)WaitForSingleObject(m_hMutex, INFINITE);
	}
	size_t clientSize = m_clientMap.size();
	if (clientSize > 0)
	{
		pThreads = new HANDLE[clientSize];
		if (NULL != pThreads)
		{
			eSDK_MEMSET(pThreads, 0, sizeof(HANDLE)*clientSize);
			std::map<SSL*, HANDLE>::iterator itor = m_clientMap.begin();
			for (int i=0; m_clientMap.end() != itor; itor++)
			{
				pThreads[i++] = itor->second;
			}
		}
	}
	if (NULL != m_hMutex)
	{
		ReleaseMutex(m_hMutex);
	}
	// 等待所有客户端线程结束
	if (NULL != pThreads)
	{
		const int iWaitTimeOut = 10000;
		DWORD dwRet = WaitForMultipleObjects(clientSize, pThreads, TRUE, iWaitTimeOut);
		if (WAIT_TIMEOUT == dwRet)
		{
			LOG_RUN_ERROR("WaitForMultipleObjects all client thead end failed.");
		}
		delete[] pThreads;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

SSL* SSL_Socket::GetOptSSL()
{
	if (NULL == m_hMutex)
	{
		LOG_RUN_ERROR("m_hMutex is null. please init ssl socket first.");
		return NULL;
	}
	(void)WaitForSingleObject(m_hMutex, INFINITE);
	SSL* _ssl = NULL;
	if (m_clientMap.empty())
	{
		LOG_RUN_ERROR("Not have the connect client.");
	}
	else
	{
		_ssl = m_clientMap.begin()->first;
	}
	ReleaseMutex(m_hMutex);
	return _ssl;
}

unsigned int SSL_Socket::GetSeqNum()
{
	return ++m_uiSequenceNumber;
}

void SSL_Socket::KickOutClient(SSL* ssl)
{
	LOG_TRACE();

	// 获取客户端线程句柄
	HANDLE hThread = NULL;
	if (NULL != Instance().m_hMutex)
	{
		(void)WaitForSingleObject(Instance().m_hMutex, INFINITE);
	}
	std::map<SSL*, HANDLE>::iterator itor = m_clientMap.find(ssl);
	if (m_clientMap.end() != itor)
	{
		hThread = itor->second;
	}
	if (NULL != m_hMutex)
	{
		ReleaseMutex(m_hMutex);
	}

	// 关闭客户端线程
	if (NULL != hThread)
	{
		m_bKickOut = true;
		DWORD dwRet = WaitForSingleObject(hThread, 1000);
		if (dwRet == WAIT_TIMEOUT)
		{
			LOG_RUN_ERROR("Kickout client failed.");
		}
		m_bKickOut = false;
	}
}

unsigned __stdcall SSL_Socket::ClientThread(void* param)
{
	LOG_TRACE();

	// 获取SSL和SOCKET参数
	SSL* _ssl = (SSL*)param;
	if (NULL == _ssl)
	{
		LOG_RUN_ERROR("_ssl is null, thread exit.");
		return 0;
	}

	SOCKET _socket = (SOCKET)SSL_get_fd(_ssl);
	if (INVALID_SOCKET == _socket)
	{
		// 关闭 SSL 连接
		SSL_shutdown(_ssl);
		// 释放 SSL
		SSL_free(_ssl);
		_ssl = NULL;
		LOG_RUN_ERROR("_socket is INVALID_SOCKET, thread exit.");
		return 0;
	}

	struct timeval _tval;
	_tval.tv_sec = 0;
	_tval.tv_usec = 500000;
	struct fd_set _fds;
	int maxfdp = (int)_socket+1;
	PACKET_DATA Packet;
	bool bReadErr = false;

	// 读取客户端消息
	char buf[PACKET_HEAD_SIZE];
	while (!Instance().m_bKickOut)//lint !e716
	{
		FD_ZERO(&_fds);
		FD_SET(_socket, &_fds);
		int iSel = select(maxfdp, &_fds, NULL, NULL, &_tval);
		if (-1 == iSel)
		{
			LOG_RUN_ERROR("select failed.");
			continue;
		}
		if (FD_ISSET(_socket, &_fds))
		{
			RESET_PACKET(Packet);
			size_t _left = 0;

			// 读取消息
			do 
			{
				// 读取包头
				if (INVALID_MSG_TYPE == Packet.MsgType)
				{
					eSDK_MEMSET(buf, 0, PACKET_HEAD_SIZE);
					int iReadSize = SSL_read(_ssl, buf, PACKET_HEAD_SIZE);
					CHECK_SSL_READ(_ssl, iReadSize, Packet, bReadErr);//读取错误判断
					if (PACKET_HEAD_SIZE == (unsigned int)iReadSize)
					{
						// 强制解析包头
						PACKET_HEAD* pHead = (PACKET_HEAD*)buf;
						if (NULL == pHead)
						{
							LOG_RUN_ERROR("pHead is null.");
							bReadErr = true;
							break;
						}

						// 设置数据包
						Packet.ProtocolVersion = pHead->ProtocolVersion;
						Packet.MsgType = pHead->MsgType;
						Packet.SeqID = pHead->SeqID;
						eSDK_MEMCPY(Packet.SessionID, sizeof(Packet.SessionID), pHead->SessionID, SESSIONID_LENGTH);
						Packet.RspCode = pHead->RspCode;
						Packet.Type = pHead->Type;
						Packet.PacketLength = pHead->PacketLength;
						Packet.ssl = _ssl;
						if (Packet.PacketLength > PACKET_HEAD_SIZE)
						{
							size_t _size = (Packet.PacketLength-PACKET_HEAD_SIZE)+1;
							Packet.Value = new char[_size];
							if (NULL == Packet.Value)
							{
								LOG_RUN_ERROR("new memory failed.");
								bReadErr = true;
								break;
							}
							eSDK_MEMSET(Packet.Value, 0, _size);
							_left = Packet.PacketLength-PACKET_HEAD_SIZE;
						}
					}
					else
					{
						// 数据污染
						LOG_RUN_ERROR("SSL_read failed, pollution data.");
						bReadErr = true;
						break;
					}
				}
				else
				{
					// 设置最大读取字节
					size_t _maxSize = _left > BUFFER_SIZE ? BUFFER_SIZE : _left;
					size_t _index = (Packet.PacketLength-PACKET_HEAD_SIZE)-_left;
					int iReadSize = SSL_read(_ssl, &Packet.Value[_index], (int)_maxSize);
					CHECK_SSL_READ(_ssl, iReadSize, Packet, bReadErr);//读取错误判断
					_left -= (size_t)iReadSize;
				}
			} while (0 < _left);

			// SSL_read出错，线程退出
			if (bReadErr)
			{
				break;
			}

			// 放入消息队列
			if (INVALID_MSG_TYPE != Packet.MsgType)
			{
				if (NULL == Packet.Value)
				{
					INFO_PARAM2(Packet.MsgType, Packet.PacketLength);
				}
				else
				{
					if(ELTE_SERVICE_SDSSEND_REQ != Packet.MsgType && ELTE_SERVICE_LOGIN_REQ != Packet.MsgType && ELTE_SERVICE_CONNECT_REQ != Packet.MsgType)
					{
						INFO_PARAM3(Packet.MsgType, Packet.PacketLength, Packet.Value);
					}
				}
				int iRet = Logic_Queue::Instance().Push(Packet);
				if (eLTE_SVC_ERR_SUCCESS != iRet)
				{
					LOG_RUN_ERROR("Push to the queue failed.");
				}
			}
		}
		else
		{
			Sleep(10);
		}
		// 服务停止判断
		if (Instance().m_bStopSvc)
		{
			LOG_RUN_INFO("Stop service.");
			break;
		}
	}

	// 从客户端socket列表中删除
	if (NULL != Instance().m_hMutex)
	{
		(void)WaitForSingleObject(Instance().m_hMutex, INFINITE);
	}
	std::map<SSL*, HANDLE>::iterator itor = Instance().m_clientMap.find(_ssl);
	if (Instance().m_clientMap.end() != itor)
	{
		CloseHandle(itor->second);
		Instance().m_clientMap.erase(itor);
	}
	if (NULL != Instance().m_hMutex)
	{
		ReleaseMutex(Instance().m_hMutex);
	}

	// 关闭 SSL 连接
	SSL_shutdown(_ssl);
	// 释放 SSL
	SSL_free(_ssl);
	_ssl = NULL;
	// 关闭socket
	closesocket(_socket);
	_socket = INVALID_SOCKET;

	/************************************************************************/
	/* 直接停服                                                             */
	//Instance().m_bStopSvc = true;
	/************************************************************************/
	return 0;
}
