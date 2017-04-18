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
#include "OpenSSL_Mgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "UserMgr.h"
#include "EventMgr.h"
#include <process.h>
#include <vector>
#include<algorithm>

using namespace std;

#define BUFFER_SIZE		1024
#define WAIT_STOP_THREAD_TIME 3*1000

OpenSSL_Mgr::OpenSSL_Mgr()
	: m_ctx(NULL)
	, m_ssl(NULL)
	, m_revThread(NULL)
	, m_revThreadId(0)
	, m_bStopRevThtread(TRUE)
	, m_proMsgThread(NULL)
	, m_proMsgThreadId(0)
	, m_bStopProMsgThtread(TRUE)
	, m_pUserMgr(NULL)
{
	m_MutexMsgQueue = CREATE_MUTEX();
	m_MutexSSL = CREATE_MUTEX();
}

OpenSSL_Mgr::~OpenSSL_Mgr()
{
	try
	{
		CloseResource();
	}
	catch(...)
	{

	}
}//lint !e1579

OpenSSL_Mgr& OpenSSL_Mgr::Instance()
{
	static OpenSSL_Mgr s_OpenSSL_Mgr;
	return s_OpenSSL_Mgr;
}

ELTE_INT32 OpenSSL_Mgr::Init_SSL_CTX()
{
	LOG_TRACE();
	// 状态校验
	if (NULL != m_ctx)
	{
		LOG_RUN_ERROR("SSL CTX have been init.");
		return eLTE_SDK_ERR_SUCCESS;
	}

	// SSL 库初始化
	SSL_library_init();
	// 载入所有 SSL 算法
	OpenSSL_add_all_algorithms();
	// 载入所有 SSL 错误消息
	SSL_load_error_strings();

	// TLSv1.2
	const SSL_METHOD* _method = TLSv1_2_client_method();
	if (NULL == _method)
	{
		LOG_RUN_ERROR("TLSv1_2_client_method failed.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	m_ctx = SSL_CTX_new(_method);
	if (NULL == m_ctx)
	{
		LOG_RUN_ERROR("SSL_CTX_new failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID OpenSSL_Mgr::Uninit_SSL_CTX()
{
	LOG_TRACE();
	CloseResource();
}

ELTE_VOID OpenSSL_Mgr::CleanMsgQuene()
{
	MutexLocker Locker(m_MutexMsgQueue);
	while (!m_msgQueue.empty())
	{
		PACKET_DATA Packet = m_msgQueue.front();
		m_msgQueue.pop();
		RELEASE_PACKET(Packet);
	}
}

ELTE_VOID OpenSSL_Mgr::CloseResource()
{
	// 关闭回调
	CEventMgr::Instance().SetEventCallBack(NULL, NULL);

	// 停止线程
	if(eLTE_SDK_ERR_SUCCESS != StopRevMsg())
	{
		LOG_RUN_ERROR("Stop rev msg failed");
		return;
	}
	if(eLTE_SDK_ERR_SUCCESS != StopProMsg())
	{
		LOG_RUN_ERROR("Stop rev msg failed");
		return;
	}

	//清空队列
	CleanMsgQuene();
	CEventMgr::Instance().StopEventProcess();
	CEventMgr::Instance().ClearEvents();
	if (NULL != m_ssl)
	{
		// 关闭 SSL 连接
		SSL_shutdown(m_ssl);
		// 释放 SSL
		SSL_free(m_ssl);
		m_ssl = NULL;
	}
	if (NULL != m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
	}
	DESTROY_MUTEX(m_MutexMsgQueue);
	DESTROY_MUTEX(m_MutexSSL);
	m_pUserMgr = NULL;
}

ELTE_INT32 OpenSSL_Mgr::Connect_SSL(int socketfd)
{
	LOG_TRACE();
	// 状态校验
	if (NULL == m_ctx)
	{
		LOG_RUN_ERROR("Please Init SSL CTX first.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	// 基于 ctx 产生一个新的 SSL
	MutexLocker Locker(m_MutexSSL);
	if (m_ssl)
	{
		SSL_free(m_ssl);
		m_ssl = NULL;
	}
	m_ssl = SSL_new(m_ctx);
	if (NULL == m_ssl)
	{
		LOG_RUN_ERROR("SSL_new failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	// 将连接用户的 socket 加入到 SSL
	SSL_set_fd(m_ssl, socketfd);

	// 建立 SSL 连接
	if (-1 == SSL_connect(m_ssl))
	{
		// 关闭 SSL 连接
		SSL_shutdown(m_ssl);
		// 释放 SSL
		SSL_free(m_ssl);
		m_ssl = NULL;
		LOG_RUN_ERROR("SSL_accept failed.");
		return eLTE_SDK_ERR_SSL_CONNECT;
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID OpenSSL_Mgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}

ELTE_VOID OpenSSL_Mgr::DoRevMsg()
{
	LOG_TRACE();
	ELTE_CHAR buf[BUFFER_SIZE+1];
	struct timeval timeout;
	fd_set fdset;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	PACKET_DATA Packet;
	ELTE_UINT32 iTotalReadSize = 0;
	ELTE_INT32 iReadSize = 0;
	while(!m_bStopRevThtread)
	{
		iTotalReadSize = 0;
		FD_ZERO(&fdset);
		// 状态校验
		if(NULL == m_ssl)
		{
			LOG_RUN_ERROR("Please Connect SSL first.");
			return;
		}
		ELTE_UINT32 tempSocket = (ELTE_UINT32)SSL_get_fd(m_ssl);
		if(INVALID_SOCKET == tempSocket)
		{
			LOG_RUN_ERROR("Socket is invalid.");
			return;
		}
		FD_SET(tempSocket, &fdset);
		ELTE_INT32 iSel = select((ELTE_INT32)tempSocket + 1, &fdset, NULL, NULL, (struct timeval *)&timeout);
		if(-1 == iSel)
		{
			LOG_RUN_ERROR("select failed.");
			continue;
		}
		if(FD_ISSET(tempSocket, &fdset))
		{
			RESET_PACKET(Packet);
			eSDK_MEMSET(buf, 0, BUFFER_SIZE+1);
			MutexLocker Locker(m_MutexSSL);
			// 读取消息头
			iReadSize = SSL_read(m_ssl, buf, PACKET_HEAD_SIZE);
			if(0 >= iReadSize) // 读取错误或者链接断开
			{
				//连接断开，进行重连
				/*if(0 == iReadSize)
				{*/
					if(NULL != m_pUserMgr && !m_pUserMgr->GetbIsDisconnect())
					{
						//停止保活
						CTimer& timer = const_cast<CTimer&>(m_pUserMgr->GetTimer());
						(void)timer.StopTimer();
						SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
						ELTE_INT32 iRet = socket.ConnectAgain();
						if (eLTE_SDK_ERR_SUCCESS == iRet)
						{
							LOG_RUN_INFO("Read head,Connect again succeed.");
							//开启保活
							timer.SetTimer(KEEPALIVE_TIME * 1000);
							continue;
						}
					}					
				/*}*/
				if (NULL == m_ssl)
				{
					break;
				}
				ELTE_INT32 _ssl_err = SSL_get_error(m_ssl, iReadSize);
				if (0 == iReadSize && SSL_ERROR_ZERO_RETURN == _ssl_err)
				{
					LOG_RUN_DEBUG("Server disconnect.");
				}
				else
				{
					LOG_RUN_ERROR("SSL_read header failed, thread exit. (iReadSize = %d, SSL_get_error = %d)", iReadSize, _ssl_err);
				}
				// ssl出错，线程退出
				break;
			}
			else if (iReadSize < (ELTE_INT32)PACKET_HEAD_SIZE)
			{
				// 读取的包头长度不全，丢弃
				LOG_RUN_DEBUG("Read head is not complete. ReadSize is %d, buf is %s.",iReadSize, buf);
				continue;
			}
			// 只会等于包头长度
			else
			{
				// 消息头处理
				// 强制解析包头
				PACKET_HEAD* pHead = (PACKET_HEAD*)buf;
				
				// 设置数据包
				Packet.ProtocolVersion = pHead->ProtocolVersion;
				Packet.MsgType = pHead->MsgType;
				Packet.SeqID = pHead->SeqID;
				eSDK_MEMCPY(Packet.SessionID, sizeof(Packet.SessionID), pHead->SessionID, sizeof(Packet.SessionID));
				Packet.RspCode = pHead->RspCode;
				Packet.Type = pHead->Type;
				Packet.PacketLength = pHead->PacketLength;
				
				Packet.ssl = m_ssl;
				if(Packet.PacketLength > PACKET_HEAD_SIZE)
				{
					Packet.Value = new char[(Packet.PacketLength - PACKET_HEAD_SIZE) + 1];
					if(NULL == Packet.Value)
					{
						LOG_RUN_ERROR("new memory failed.");
						// 申请内存失败，线程退出
						break;
					}
					eSDK_MEMSET(Packet.Value, 0, (Packet.PacketLength - PACKET_HEAD_SIZE) + 1);
				}
				// 读取消息体
				iReadSize = 0;
				ELTE_UINT32 iBodyLength = Packet.PacketLength - PACKET_HEAD_SIZE;
				while(iTotalReadSize < iBodyLength)
				{
					// 每次最多只读1024字节
					iReadSize = SSL_read(m_ssl, Packet.Value + iTotalReadSize, BUFFER_SIZE);
					if(0 >= iReadSize) // 读取错误或者链接断开
					{
						//连接断开，进行重连
						/*if(0 == iReadSize)
						{*/
							if(NULL != m_pUserMgr && !m_pUserMgr->GetbIsDisconnect())
							{
								//停止保活
								CTimer& timer = const_cast<CTimer&>(m_pUserMgr->GetTimer());
								(void)timer.StopTimer();
								SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
								ELTE_INT32 iRet = socket.ConnectAgain();
								if (eLTE_SDK_ERR_SUCCESS == iRet)
								{
									LOG_RUN_INFO("Read body,Connect again succeed.");
									//开启保活
									timer.SetTimer(KEEPALIVE_TIME * 1000);
									break;
								}
							}					
						/*}*/
						if (NULL == m_ssl)
						{
							break;
						}
						ELTE_INT32 _ssl_err = SSL_get_error(m_ssl, iReadSize);
						if (0 == iReadSize && SSL_ERROR_ZERO_RETURN == _ssl_err)
						{
							LOG_RUN_DEBUG("Server disconnect.");
						}
						else
						{
							LOG_RUN_ERROR("SSL_read header failed, thread exit. (iReadSize = %d, SSL_get_error = %d)", iReadSize, _ssl_err);
						}
						// ssl出错，线程退出
						break;
					}
					iTotalReadSize += (ELTE_UINT32)iReadSize;
				}
				if(iTotalReadSize < iBodyLength)
				{
					// 接受数据不全，丢弃
					LOG_RUN_DEBUG("Read body is not complete. ReadSize is %d, Value is %s.",iTotalReadSize, Packet.Value);
					continue;
				}
				// 存入队列
				MutexLocker LockerMsgQueue(m_MutexMsgQueue);
				m_msgQueue.push(Packet);
			}// 消息头以及消息体处理
		}// FD_ISSET
		else
		{
			Sleep(10);
		}
	}
}

ELTE_INT32 OpenSSL_Mgr::StartRevMsg()
{
	LOG_TRACE();
	if(NULL != m_revThread)
	{
		CloseHandle(m_revThread);
		m_revThread = NULL;
		LOG_RUN_DEBUG("OpenSSL_Mgr::Stop rev msg thread success, thread id[%d]", m_revThreadId);
	}

	m_revThread = (HANDLE)_beginthreadex(NULL, 0, RevMsgThread, (void*)this, 0, &m_revThreadId);
	if(NULL == m_revThread)
	{
		LOG_RUN_ERROR("OpenSSL_Mgr::Start rev msg thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		m_bStopRevThtread = FALSE;
		LOG_RUN_DEBUG("OpenSSL_Mgr::Start rev msg thread success, thread id[%d]", m_revThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_UINT32 __stdcall OpenSSL_Mgr::RevMsgThread(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	OpenSSL_Mgr* pOpenSSLMgr = (OpenSSL_Mgr*)param;
	//接受消息
	pOpenSSLMgr->DoRevMsg();
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 OpenSSL_Mgr::StopRevMsg()
{
	LOG_TRACE();
	m_bStopRevThtread = TRUE;
	if(NULL == m_revThread)
	{
		LOG_RUN_DEBUG("OpenSSL_Mgr::Stop rev msg thread success, no thread is running");
		return eLTE_SDK_ERR_SUCCESS;
	}

	ELTE_ULONG ulResult = ::WaitForSingleObject(m_revThread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("OpenSSL_Mgr::Wait for single object failed.");
	}
	CloseHandle(m_revThread);
	m_revThread = NULL;
	LOG_RUN_DEBUG("OpenSSL_Mgr::Stop rev msg thread success, thread id[%d]", m_revThreadId);
	m_revThreadId = 0;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 OpenSSL_Mgr::StartProMsg()
{
	LOG_TRACE();
	m_bStopProMsgThtread = FALSE;
	if(NULL != m_proMsgThread)
	{
		CloseHandle(m_proMsgThread);
		m_proMsgThread = NULL;
		LOG_RUN_DEBUG("Stop msg process thread success, thread id[%d]", m_proMsgThreadId);
	}

	m_proMsgThread = (HANDLE)_beginthreadex(NULL, 0, ProMsgThread, (ELTE_VOID*)this, 0, &m_proMsgThreadId);
	if(NULL == m_proMsgThread)
	{
		LOG_RUN_ERROR("Start pro msg thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		m_bStopProMsgThtread = FALSE;
		LOG_RUN_DEBUG("Start pro msg thread success, thread id[%d]", m_proMsgThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_VOID OpenSSL_Mgr::DoProMsg()
{
	PACKET_DATA Packet;
	while(!m_bStopProMsgThtread)
	{
		{
			MutexLocker Locker(m_MutexMsgQueue);
			if(m_msgQueue.empty())
			{
				Sleep(10);
				continue;
			}
			Packet = m_msgQueue.front();
			m_msgQueue.pop();
		}
	
		//消息处理（分发）
		DispatchMsg(Packet);	

		RESET_PACKET(Packet);
	}
}

ELTE_INT32 OpenSSL_Mgr::CheckDispatchMsg(PACKET_DATA& Packet, ELTE_UINT32 Type)
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_FAILED;
	}

	vector<ELTE_UINT32> conditionNo(100);
	if (!conditionNo.empty())
	{
		conditionNo.clear();
	}
	//将条件插入到向量容器中

	conditionNo.push_back(ELTE_SERVICE_LOGIN_RSP);
	conditionNo.push_back(ELTE_SERVICE_LOGOUT_RSP);
	conditionNo.push_back(ELTE_SERVICE_TRIGGERSTATUSREPORT_RSP);
	conditionNo.push_back(ELTE_SERVICE_PROVISIONMANAGERINIT_RSP);
	conditionNo.push_back(ELTE_SERVICE_PROVISIONMANAGERINITMRS_RSP);
	conditionNo.push_back(ELTE_SERVICE_PROVISIONMANAGEREXIT_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETUSERINFO_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETDCGROUPS_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETDCUSERS_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETGROUPUSERS_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETGROUPINFO_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETDCINFO_RSP);
	conditionNo.push_back(ELTE_SERVICE_SUBSCRIBEGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETUSERRECFILEINFOLIST_RSP);
	conditionNo.push_back(ELTE_SERVICE_CREATEDYNAMICGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_CANCELDYNAMICGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETTEMPGROUPID_RSP);
	conditionNo.push_back(ELTE_SERVICE_CREATETEMPGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_STARTREALPLAY_RSP);
	conditionNo.push_back(ELTE_SERVICE_PTZCONTROL_RSP);
	conditionNo.push_back(ELTE_SERVICE_STOPREALPLAY_RSP);
	conditionNo.push_back(ELTE_SERVICE_DISCONNECT_RSP);
	conditionNo.push_back(ELTE_SERVICE_SUBJOINGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_GISSUBSCRIBE_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PDIAL_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PRECV_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PREJECT_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PHANGUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PBREAKOFF_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PBREAKIN_RSP);
	conditionNo.push_back(ELTE_SERVICE_PTTDIAL_RSP);
	conditionNo.push_back(ELTE_SERVICE_PTTRELEASE_RSP);
	conditionNo.push_back(ELTE_SERVICE_PTTHANGUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_PTTEMERGENCY_RSP);
	conditionNo.push_back(ELTE_SERVICE_GROUPBREAKOFF_RSP);
	conditionNo.push_back(ELTE_SERVICE_VOLUMEMUTE_RSP);
	conditionNo.push_back(ELTE_SERVICE_VOLUMEUNMUTE_RSP);
	conditionNo.push_back(ELTE_SERVICE_SDSSEND_RSP);
	conditionNo.push_back(ELTE_SERVICE_STARTDISPATCHVIDEO_RSP);
	conditionNo.push_back(ELTE_SERVICE_STOPDISPATCHVIDEO_RSP);
	conditionNo.push_back(ELTE_SERVICE_RECVVIDEOPLAY_RSP);
	conditionNo.push_back(ELTE_SERVICE_VWALLSTART_RSP);
	conditionNo.push_back(ELTE_SERVICE_VWALLSTOP_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETVWALLLIST_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETVWALLSTATE_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PTRANSFER_RSP);
//	conditionNo.push_back(ELTE_SERVICE_TELEPHONEDIAL_RSP);
//	conditionNo.push_back(ELTE_SERVICE_TELEPHONEHANGUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_STARTDISCREETLISTEN_RSP);
	conditionNo.push_back(ELTE_SERVICE_STOPDISCREETLISTEN_RSP);
	conditionNo.push_back(ELTE_SERVICE_STARTENVIRLISTEN_RSP);
	conditionNo.push_back(ELTE_SERVICE_CREATEPATCHGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_CANCELPATCHGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_ADDPATCHMEMBER_RSP);
	conditionNo.push_back(ELTE_SERVICE_DELETEPATCHMEMBER_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETPATCHGROUPS_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETPATCHGROUPMEMBERS_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETPATCHGROUPINFO_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETUSERSPECIFICGISCFG_RSP);
	conditionNo.push_back(ELTE_SERVICE_SETUSERSPECIFICGISCFG_RSP);
	conditionNo.push_back(ELTE_SERVICE_GETGISSUBSCRIPTION_RSP);
	conditionNo.push_back(ELTE_SERVICE_MODIFYDYNAMICGROUP_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PHALFDPXDIAL_RSP);
	conditionNo.push_back(ELTE_SERVICE_P2PHALFDPXRELEASE_RSP);
//	conditionNo.push_back(ELTE_SERVICE_TEMPUSERJOINGROUP_RSP);	
	conditionNo.push_back(ELTE_SERVICE_STARTRECORD_RSP);
	conditionNo.push_back(ELTE_SERVICE_STOPRECORD_RSP);
	
	vector<ELTE_UINT32>::iterator cond = find(conditionNo.begin(),conditionNo.end(),Type);
	if (cond != conditionNo.end())	
	{
		m_pUserMgr->SetPacketData(Packet);
		::SetEvent(m_pUserMgr->GetEventHandle());
		return eLTE_SDK_ERR_SUCCESS;
	}
	else
	{
		return eLTE_SDK_ERR_FAILED;
	}
	
}

ELTE_VOID OpenSSL_Mgr::DispatchMsg(PACKET_DATA& Packet)
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	switch (Packet.MsgType)
	{
		/*case ELTE_SERVICE_LOGIN_RSP:
		case ELTE_SERVICE_LOGOUT_RSP:
		case ELTE_SERVICE_TRIGGERSTATUSREPORT_RSP:
		case ELTE_SERVICE_PROVISIONMANAGERINIT_RSP:
		case ELTE_SERVICE_PROVISIONMANAGERINITMRS_RSP:
		case ELTE_SERVICE_PROVISIONMANAGEREXIT_RSP:
		case ELTE_SERVICE_GETUSERINFO_RSP:
		case ELTE_SERVICE_GETDCGROUPS_RSP:
		case ELTE_SERVICE_GETDCUSERS_RSP:
		case ELTE_SERVICE_GETGROUPUSERS_RSP:
		case ELTE_SERVICE_GETGROUPINFO_RSP:
		case ELTE_SERVICE_GETDCINFO_RSP:
		case ELTE_SERVICE_SUBSCRIBEGROUP_RSP:
		case ELTE_SERVICE_GETUSERRECFILEINFOLIST_RSP:
		case ELTE_SERVICE_CREATEDYNAMICGROUP_RSP:
		case ELTE_SERVICE_CANCELDYNAMICGROUP_RSP:
		case ELTE_SERVICE_GETTEMPGROUPID_RSP:
		case ELTE_SERVICE_CREATETEMPGROUP_RSP:
		case ELTE_SERVICE_STARTREALPLAY_RSP:
		case ELTE_SERVICE_PTZCONTROL_RSP:
		case ELTE_SERVICE_STOPREALPLAY_RSP:
		case ELTE_SERVICE_DISCONNECT_RSP:
		case ELTE_SERVICE_SUBJOINGROUP_RSP:
		case ELTE_SERVICE_GISSUBSCRIBE_RSP:
		case ELTE_SERVICE_P2PDIAL_RSP:
		case ELTE_SERVICE_P2PRECV_RSP:
		case ELTE_SERVICE_P2PREJECT_RSP:
		case ELTE_SERVICE_P2PHANGUP_RSP:
		case ELTE_SERVICE_P2PBREAKOFF_RSP:
		case ELTE_SERVICE_P2PBREAKIN_RSP:
		case ELTE_SERVICE_PTTDIAL_RSP:
		case ELTE_SERVICE_PTTRELEASE_RSP:
		case ELTE_SERVICE_PTTHANGUP_RSP:
		case ELTE_SERVICE_PTTEMERGENCY_RSP:
		case ELTE_SERVICE_GROUPBREAKOFF_RSP:
		case ELTE_SERVICE_VOLUMEMUTE_RSP:
		case ELTE_SERVICE_VOLUMEUNMUTE_RSP:
		case ELTE_SERVICE_SDSSEND_RSP:
		case ELTE_SERVICE_STARTDISPATCHVIDEO_RSP:
		case ELTE_SERVICE_STOPDISPATCHVIDEO_RSP:
		case ELTE_SERVICE_RECVVIDEOPLAY_RSP:
		case ELTE_SERVICE_VWALLSTART_RSP:
		case ELTE_SERVICE_VWALLSTOP_RSP:
		case ELTE_SERVICE_GETVWALLLIST_RSP:
		case ELTE_SERVICE_GETVWALLSTATE_RSP:
		case ELTE_SERVICE_P2PTRANSFER_RSP:
		case ELTE_SERVICE_TELEPHONEDIAL_RSP:
		case ELTE_SERVICE_TELEPHONEHANGUP_RSP:
		case ELTE_SERVICE_STARTDISCREETLISTEN_RSP:
		case ELTE_SERVICE_STOPDISCREETLISTEN_RSP:
		case ELTE_SERVICE_STARTENVIRLISTEN_RSP:
		case ELTE_SERVICE_CREATEPATCHGROUP_RSP:
		case ELTE_SERVICE_CANCELPATCHGROUP_RSP:
		case ELTE_SERVICE_ADDPATCHMEMBER_RSP:
		case ELTE_SERVICE_DELETEPATCHMEMBER_RSP:
		case ELTE_SERVICE_GETPATCHGROUPS_RSP:
		case ELTE_SERVICE_GETPATCHGROUPMEMBERS_RSP:
		case ELTE_SERVICE_GETPATCHGROUPINFO_RSP:
		case ELTE_SERVICE_GETUSERSPECIFICGISCFG_RSP:
		case ELTE_SERVICE_SETUSERSPECIFICGISCFG_RSP:
		case ELTE_SERVICE_GETGISSUBSCRIPTION_RSP:
		case ELTE_SERVICE_MODIFYDYNAMICGROUP_RSP:
		case ELTE_SERVICE_P2PHALFDPXDIAL_RSP:
		case ELTE_SERVICE_P2PHALFDPXRELEASE_RSP:
		case ELTE_SERVICE_TEMPUSERJOINGROUP_RSP:
		{
			m_pUserMgr->SetPacketData(Packet);
			::SetEvent(m_pUserMgr->GetEventHandle());
		}
		break;*/
		case ELTE_SERVICE_CONNECT_RSP:
		{
			SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
			socket.SetPacketData(Packet);
			::SetEvent(socket.GetEventHandle());
		}
		break;
		case ELTE_SERVICE_KEEPALIVE_RSP:
		{
			CTimer& timer = const_cast<CTimer&>(m_pUserMgr->GetTimer());
			timer.SetPacketData(Packet);
			::SetEvent(timer.GetEventHandle());
		}
		break;
		case ELTE_SERVICE_PROVISIONALLRESYNC_REQ:
		case ELTE_SERVICE_USERSTATUS_REQ:
		case ELTE_SERVICE_MODULESTATUS_REQ:
		case ELTE_SERVICE_RESOURCESTATUS_REQ:
		case ELTE_SERVICE_PROVISIONCHANGE_REQ:
		case ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ:
		case ELTE_SERVICE_P2PCALLSTATUS_REQ:
		case ELTE_SERVICE_GROUPCALLSTATUS_REQ:
		case ELTE_SERVICE_GISREPORT_REQ:
		case ELTE_SERVICE_GISSTATUS_REQ:
		case ELTE_SERVICE_SDSREPORT_REQ:
		case ELTE_SERVICE_SDSSENDSTATUS_REQ:
		{
			ProcessPacketData(Packet);
		}
		break;
		default:
		{
			if (eLTE_SDK_ERR_SUCCESS != CheckDispatchMsg(Packet, Packet.MsgType))
			{
				LOG_RUN_ERROR("Invalid msg_type 0x%x.",Packet.MsgType);
				RELEASE_PACKET(Packet);
			}			
			
		}		
	}
}//lint !e1762

ELTE_VOID OpenSSL_Mgr::ProcessPacketData(const PACKET_DATA& packetData)
{
	LOG_TRACE();
	PACKET_DATA Packet;
	Packet = packetData;
	if(eLTE_SDK_ERR_SUCCESS != Packet.RspCode)
	{
		LOG_RUN_ERROR("RspCode is not success, msg_type is 0x%x.", Packet.MsgType);
	}
	else
	{
		//文本长度
		ELTE_UINT32 uiLen = Packet.PacketLength - PACKET_HEAD_SIZE;
		ELTE_INT32 iEventType = -1;
		switch(Packet.MsgType)
		{
			case ELTE_SERVICE_PROVISIONALLRESYNC_REQ:
			{
				iEventType = ELTE_Event_NotifyProvisionAllResync;
			}
			break;
			case ELTE_SERVICE_USERSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyUserStatus;
			}
			break;
			case ELTE_SERVICE_MODULESTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyModuleStatus;
			}
			break;
			case ELTE_SERVICE_RESOURCESTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyResourceStatus;
			}
			break;
			case ELTE_SERVICE_PROVISIONCHANGE_REQ:
			{
				iEventType = ELTE_Event_NotifyProvisionChange;
			}
			break;
			case ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyP2pvideocallStatus;
			}
			break;
			case ELTE_SERVICE_P2PCALLSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyP2pcallStatus;
			}
			break;
			case ELTE_SERVICE_GROUPCALLSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyGroupStatus;
			}
			break;
			case ELTE_SERVICE_GISREPORT_REQ:
			{
				iEventType = ELTE_Event_NotifyGISReport;
			}
			break;
			case ELTE_SERVICE_GISSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifyGISStatus;
			}
			break;
			case ELTE_SERVICE_SDSREPORT_REQ:
			{
				iEventType = ELTE_Event_NotifySDSReport;
			}
			break;
			case ELTE_SERVICE_SDSSENDSTATUS_REQ:
			{
				iEventType = ELTE_Event_NotifySDSSendStatus;
			}
			break;
			default:
				LOG_RUN_ERROR("Invalid msg_type 0x%x.",Packet.MsgType);
		}
		//存入事件队列
		if(-1 != iEventType && eLTE_SDK_ERR_SUCCESS != CEventMgr::Instance().PushEvent(iEventType, Packet.Value, uiLen))
		{
			LOG_RUN_ERROR("Push event data failed, msg_type is 0x%x.", Packet.MsgType);	
		}	
	}
	PACKET_HEAD head;
	head.MsgType = Packet.MsgType | 0xF000;
	head.RspCode = eLTE_SDK_ERR_SUCCESS;
	head.PacketLength = PACKET_HEAD_SIZE;
	//返回消息给服务端
	if(eLTE_SDK_ERR_SUCCESS != SendMsg((ELTE_VOID*)&head, PACKET_HEAD_SIZE))
	{
		LOG_RUN_ERROR("Send head failed.");
	}
	//释放
	RELEASE_PACKET(Packet);
}

ELTE_UINT32 __stdcall OpenSSL_Mgr::ProMsgThread(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	OpenSSL_Mgr* pOpenSSLMgr = (OpenSSL_Mgr*)param;
	pOpenSSLMgr->DoProMsg();
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 OpenSSL_Mgr::StopProMsg()
{
	m_bStopProMsgThtread = TRUE;
	if(NULL == m_proMsgThread)
	{
		LOG_RUN_DEBUG("Stop pro msg thread success, no thread is running");
		return eLTE_SDK_ERR_SUCCESS;
	}

	ELTE_ULONG ulResult = ::WaitForSingleObject(m_proMsgThread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_proMsgThread);
	m_proMsgThread = NULL;
	LOG_RUN_DEBUG("Stop pro msg thread success, thread id[%d]", m_proMsgThreadId);
	m_proMsgThreadId = 0;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 OpenSSL_Mgr::SendMsg(const ELTE_VOID* pData, const ELTE_UINT32& uiLength)
{
	LOG_TRACE();
	if(NULL == pData)
	{
		LOG_RUN_ERROR("Send data is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	ELTE_INT32 iErrorCode = 0;
	ELTE_CHAR* pBuf = (ELTE_CHAR*)(const_cast<ELTE_VOID*>(pData));

	MutexLocker Locker(m_MutexSSL);
	if (NULL == m_ssl)
	{
		LOG_RUN_ERROR("SSL is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	//发送消息
	ELTE_INT32 iLen = SSL_write(m_ssl, pBuf, (ELTE_INT32)uiLength);
	if (iLen < 0)
	{
		iErrorCode = SSL_get_error(m_ssl, iLen);
		LOG_RUN_ERROR("%s message Send failure ！Error code is %d.", (char*)pData, iErrorCode);
		return eLTE_SDK_ERR_SEND_MSG;
	}
	
	if ((ELTE_UINT32)iLen != uiLength)
	{
		LOG_RUN_ERROR("message Send not completely.");
		return eLTE_SDK_ERR_SEND_MSG;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

