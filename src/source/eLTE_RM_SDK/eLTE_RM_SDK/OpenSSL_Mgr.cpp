#include "stdafx.h"
#include <process.h>

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
#include "XMLProcess.h"
//public function interface class 
#include "eLTE_Tool.h"
//Timer definition class
#include "eLTE_RM_Timer.h"

//#include "EventMgr.h"

#define BUFFER_SIZE		1024
#define WAIT_CALLBACK_TIME 8000

ELTE_INT32 OpenSSL_Mgr::m_iLoseCon=0;
ELTE_INT32 OpenSSL_Mgr::m_iHeartBeat=0;

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
		LOG_RUN_ERROR("CloseResource failed!");//lint !e1551
	}
}//lint !e1579 

OpenSSL_Mgr& OpenSSL_Mgr::Instance()
{
	static OpenSSL_Mgr s_OpenSSL_Mgr;
	return s_OpenSSL_Mgr;
}

//初始化ctx
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
	SSL_load_error_strings();	//lint !e438

	/* 选择会话协议,安全红线要求使用TLSv1 */
	const SSL_METHOD* _method = TLSv1_client_method();
	if (NULL == _method)
	{
		LOG_RUN_ERROR("TLSv1_client_method failed.");
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
		//PACKET_DATA Packet = m_msgQueue.front();
		m_msgQueue.pop();
		//RELEASE_PACKET(Packet);
	}
}

ELTE_VOID OpenSSL_Mgr::CloseResource()
{
	// 关闭回调
	//CEventMgr::Instance().SetEventCallBack(NULL, NULL);

	// 停止线程
	if(eLTE_SDK_ERR_SUCCESS != StopRevMsg())
	{
		LOG_RUN_ERROR("Stop rev msg failed");
		return;
	}

	if(eLTE_SDK_ERR_SUCCESS != StopProMsg())
	{
		LOG_RUN_ERROR("Stop Pro msg failed");
		return;
	}

	//清空队列
	CleanMsgQuene();
	//CEventMgr::Instance().StopEventProcess();
	//CEventMgr::Instance().ClearEvents();
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
	ELTE_CHAR buf[SOCKET_RECV_BUFFRM_LEN_SIZE+1];
	struct timeval timeout;
	fd_set fdset;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	//PACKET_DATA Packet;
	ELTE_UINT32 iTotalReadSize = 0;
	ELTE_INT32 iReadSize = 0;

	if (NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("m_pUserMgr is NULL!");
		return;
	}

	//start heart beat
	if (0 == m_iHeartBeat)
	{
		m_iHeartBeat = 1;
		RMTimer::Instance().SetTimer(HEARTBEAT_TIMER, HEARTBEAT_TIME);
	}

	//receiving message start
	while(!m_bStopRevThtread)
	{
		iTotalReadSize = 0;
		FD_ZERO(&fdset);
		if (1 == m_iLoseCon)
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(m_pUserMgr->GetEventHandle(), INFINITE))
			{
				LOG_RUN_ERROR("reconnect timeout!");
			}
			
		}
		
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
			if (INVALID_SOCKET == tempSocket)
			{
				LOG_RUN_ERROR("select failed.");
			}			
			continue;
		}

		if(FD_ISSET(tempSocket, &fdset))
		{
			//RESET_PACKET(Packet);
			memset(buf, 0, SOCKET_RECV_BUFFRM_LEN_SIZE+1);
			//互斥锁
			MutexLocker Locker(m_MutexSSL);
			// 读取消息头

			iReadSize = SSL_read(m_ssl, buf, SOCKET_RECV_BUFFRM_LEN_SIZE);
			ELTE_INT32 _ssl_err = SSL_get_error(m_ssl, iReadSize);
			//判断链接断开
			if(0 >= iReadSize || SSL_ERROR_SYSCALL == _ssl_err)
			{
				if(NULL != m_pUserMgr)
				{
					if (0 == m_iLoseCon)
 					{
 						m_iLoseCon = 1;
						//stop heart beat timer
						if (RET_SUCCESS != RMTimer::Instance().StopTimer(HEARTBEAT_TIMER))
						{
							LOG_RUN_ERROR("Timer Stop warning: stop timer error or timer stop running");
						}

						//start reconnect timer
 						RMTimer::Instance().SetTimer(RECONNECT_TIMER, RECONNECT_POLLING_TIME);
 					}
					continue;
				}

				if (NULL == m_ssl)
				{
					break;
				}

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
			else if (iReadSize < (ELTE_INT32)SOCKET_RECV_BUFFRM_LEN_SIZE)
			{
				// 读取的包头长度不全，丢弃
				LOG_RUN_DEBUG("Read head is not complete. ReadSize is %d, buf is %s.",iReadSize, buf);
				continue;
			}
			// 只会等于包头长度
			else
			{
				//计算包长
				ELTE_UINT32 iBodyLength = std::stoul(buf, nullptr, 16);
				char* cValue = new char[iBodyLength + 1];
				if(NULL == cValue)
				{
					LOG_RUN_ERROR("new memory failed.");
						// 申请内存失败，线程退出
					break;
				}
				memset(cValue, 0, iBodyLength + 1);
				// 读取消息体
				iReadSize = 0;
				while(iTotalReadSize < iBodyLength)
				{
					// 每次最多只读1024字节
					iReadSize = SSL_read(m_ssl, cValue + iTotalReadSize, BUFFER_SIZE);
					if(0 >= iReadSize) // 读取错误或者链接断开
					{
						//连接断开，进行重连
						/*if(0 == iReadSize)
						{*/
							if(NULL != m_pUserMgr)
							{
								//停止保活
								//CTimer& timer = const_cast<CTimer&>(m_pUserMgr->GetTimer());
								//(void)timer.StopTimer();
								SSL_Socket& ReconSocket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
								ELTE_INT32 iRet = ReconSocket.ConnectAgain();
								if (eLTE_SDK_ERR_SUCCESS == iRet)
								{
									LOG_RUN_INFO("Read body,Connect again successed.");
									//开启保活
									//timer.SetTimer(KEEPALIVE_TIME * 1000);
									break;
								}
							}
						/*}*/
						if (NULL == m_ssl)
						{
							break;
						}
						_ssl_err = SSL_get_error(m_ssl, iReadSize);
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
					LOG_RUN_DEBUG("Read body is not complete. ReadSize is %d, Value is %s.",iTotalReadSize, cValue);
					if (NULL != cValue)
					{
						delete [] cValue;
						cValue = NULL;
					}
					continue;
				}
				// 存入队列
				MutexLocker LockerMsgQueue(m_MutexMsgQueue);
				std::string strValue = cValue;
				m_msgQueue.push(strValue);
				
				if (NULL != cValue)
				{
					delete [] cValue;
					cValue = NULL;
				}	
			}//lint !e438
		}// FD_ISSET
		else
		{
			Sleep(10);
		}
	}
}//lint !e529

ELTE_INT32 OpenSSL_Mgr::StartRevMsg()
{
	LOG_TRACE();
	if(NULL != m_revThread)
	{
		CloseHandle(m_revThread);
		m_revThread = NULL;
		LOG_RUN_DEBUG("Stop rev msg thread success, thread id[%d]", m_revThreadId);
	}

	m_revThread = (HANDLE)_beginthreadex(NULL, 0, RevMsgThread, (void*)this, 0, &m_revThreadId);
	if(NULL == m_revThread)
	{
		LOG_RUN_ERROR("Start rev msg thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		m_bStopRevThtread = FALSE;
		LOG_RUN_DEBUG("Start rev msg thread success, thread id[%d]", m_revThreadId);
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
	m_bStopRevThtread = TRUE;
	if(NULL == m_revThread)
	{
		LOG_RUN_DEBUG("Stop rev msg thread success, no thread is running");
		return eLTE_SDK_ERR_SUCCESS;
	}

	if(INFINITE == WaitForSingleObject(m_revThread, INFINITE))
	{
		LOG_RUN_ERROR("m_revThread wait Time out!");
		return eLTE_SDK_ERR_WAIT_TIME_OUT;
	}
	
	CloseHandle(m_revThread);
	m_revThread = NULL;
	LOG_RUN_DEBUG("Stop rev msg thread success, thread id[%d]", m_revThreadId);
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
	std::string strPacket;
	while(!m_bStopProMsgThtread)
	{
		{
			MutexLocker Locker(m_MutexMsgQueue);
			if(m_msgQueue.empty())
			{
				Sleep(5);
				continue;
			}
			strPacket = m_msgQueue.front();
			m_msgQueue.pop();
		}

		//消息处理（分发）
		DispatchMsg(strPacket);	
	}
}

ELTE_VOID OpenSSL_Mgr::DispatchMsg(const std::string& strPacket)
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}

	LOG_RUN_DEBUG("Server Rsp is %s", strPacket.c_str());

	ELTE_INT32 iEventType = -1;
	std::string xmlStr;
	Json::Reader reader;
	Json::Value root;
	Json::StyledWriter writer;
	if(!reader.parse(strPacket, root) || !root.isObject())
	{
		LOG_RUN_ERROR("parse json command format error!!!!");
		return;
	}
	
	std::string strRsp;
	if ("1FFFF0" == root["cmd"].asString())//接入请求
	{
		strRsp = root["rsp"].asString();
		if("0" == strRsp)//success
		{
			m_pUserMgr->SetSessionId(root["session"].asString());
			LOG_RUN_INFO("Connect request : SetEvent!");
			::SetEvent(m_pUserMgr->GetEventHandle());
		}
		else//fail
		{
			LOG_RUN_ERROR("Get session id failed.");
		}
	}
	else if("0FFFFF" == root["cmd"].asString())//保持心跳
	{
		SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
		if(eLTE_SDK_ERR_SUCCESS != socket.KeepAlive())
		{
			LOG_RUN_ERROR("Send keep alive req failed.");
		}
	}
	else if("1FFFF1" == root["cmd"].asString())//login
	{
		// 获取xml字符串
		ELTE_INT32 iRet = XMLProcess::SetXml_NotifyResourceStatus(root["isdn"].asString(), "", RESREGSTATUS_PROXY, eLTE_Tool::String2Int(root["rsp"].asString()), "", "", xmlStr);
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifyResourceStatus of login failed.");
		}
		iEventType = ELTE_Event_NotifyResourceStatus;
	}
	else if ("1FFFF2" == root["cmd"].asString())//logout
	{
		// 获取xml字符串
		ELTE_INT32 iRet = XMLProcess::SetXml_NotifyResourceStatus(root["isdn"].asString(), "", RESREGSTATUS_PROXY, eLTE_Tool::String2Int(root["rsp"].asString()), "", "", xmlStr);
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifyResourceStatus of logout failed.");
		}

		iEventType = ELTE_Event_NotifyResourceStatus;
	}
	else if ("110006" == root["cmd"].asString())//query
	{
		std::string strSeq = root["seq"].asString();
		ELTE_UINT32 uiSeq = eLTE_Tool::String2UInt(strSeq);
		if(SEQ_GETGROUPUSERS == uiSeq)//group users
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetGroupUsers_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetDcUsers_Rsp of logout failed.");
			}
		}
		else if(SEQ_GETDCGROUPS == uiSeq)//dc groups
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetDcGroups_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetDcGroups_Rsp of logout failed.");
			}
		}
		else if(SEQ_GETDCUSERS == uiSeq) //dc users
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetDcUsers_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetDcUsers_Rsp of logout failed.");
			}
		}
		if (root["seq"].asString() != m_pUserMgr->GetCheckSeq())
		{
			LOG_RUN_ERROR("seq or cmd value has been changed!");
			return;
		}
		m_pUserMgr->SetPacketData(xmlStr);
		LOG_RUN_INFO("query 110006 :SetEvent!");
		::SetEvent(m_pUserMgr->GetEventHandle());
	}
	else if("110016" == root["cmd"].asString())//query
	{
		Json::Value param = root["param"];
		std::string strSeq = root["seq"].asString();
		ELTE_UINT32 uiSeq = eLTE_Tool::String2UInt(strSeq);
		if(SEQ_GETUSERINFO == uiSeq)//user info
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetUserInfo_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetUserInfo_Rsp of logout failed.");
			}
		}
		if(SEQ_GETGROUPINFO == uiSeq)//group info
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetGroupInfo_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetGroupInfo_Rsp of logout failed.");
			}
		}
		if(SEQ_GETDCINFO == uiSeq)//dc info
		{
			ELTE_INT32 iRet = XMLProcess::SetXml_GetDcInfo_Rsp(root, xmlStr);
			if (eLTE_SDK_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("SetXml_GetDcInfo_Rsp of logout failed.");
			}
		}
		m_pUserMgr->SetPacketData(xmlStr);
		LOG_RUN_INFO("query 110016 : SetEvent!");
		::SetEvent(m_pUserMgr->GetEventHandle());
	}
	else if ("280000" == root["cmd"].asString())//user status
	{
		// 获取xml字符串
		ELTE_INT32 iRet = XMLProcess::SetXml_NotifyUserStatus(root, xmlStr);
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifyUserStatus of logout failed.");
		}

		iEventType = ELTE_Event_NotifyUserStatus;
	}
	else if ("290000" == root["cmd"].asString())//服务模块状态
	{

		// 获取xml字符串
		ELTE_INT32 iRet = XMLProcess::SetXml_NotifyModuleStatus(root, xmlStr);
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifyUserStatus of logout failed.");
		}

		iEventType = ELTE_Event_NotifyModuleStatus;
	}
	else if("100007"==root["cmd"].asString())//播放和挂断状态回传
	{
		std::string opt=root["opt"].asString();
//		std::string strRsp = root["rsp"].asString();
		m_pUserMgr->SetPacketData(root["rsp"].asString());
		LOG_RUN_INFO("Query 100007 : SetEvent!");
		::SetEvent(m_pUserMgr->GetEventHandle());
	}
	else if("200007"==root["cmd"].asString())//视频播放成功，状态回传
	{
		ELTE_INT32 iRet=XMLProcess::SetXml_NotifyP2pvideocallStatus(root,xmlStr);
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifyP2pvideocallStatus failed");
		}
		iEventType = ELTE_Event_NotifyP2pvideocallStatus;
	}
	else if("100009"==root["cmd"].asString())//云台控制
	{
		std::string opt=root["opt"].asString();
//		std::string strRsp = root["rsp"].asString();
		m_pUserMgr->SetPacketData(root["rsp"].asString());
		LOG_RUN_INFO("Query 100009 : SetEvent!");
		::SetEvent(m_pUserMgr->GetEventHandle());
	}


	//回调方式需要修改
	EventCallBack m_fnEventCallBack = m_pUserMgr->GetEventCallBack();
	if(m_fnEventCallBack && -1 != iEventType)
	{
		ELTE_UINT32 uiLength = xmlStr.length();
		if (uiLength > 0)
		{
			m_fnEventCallBack(iEventType, (ELTE_VOID*)xmlStr.c_str(), uiLength, m_pUserMgr->GetEventUserData());
		}		
	}
	
}

ELTE_UINT32 __stdcall OpenSSL_Mgr::ProMsgThread(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	OpenSSL_Mgr* pOpenSSLMgr = (OpenSSL_Mgr*)param;
	if (NULL != pOpenSSLMgr)
	{
		pOpenSSLMgr->DoProMsg();
	}
	
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

	if(INFINITE == WaitForSingleObject(m_proMsgThread, INFINITE))
	{
		LOG_RUN_ERROR("m_proMsgThread wait Time out!");
		return eLTE_SDK_ERR_WAIT_TIME_OUT;
	}
	CloseHandle(m_proMsgThread);
	m_proMsgThread = NULL;
	LOG_RUN_DEBUG("Stop pro msg thread success, thread id[%d]", m_proMsgThreadId);
	m_proMsgThreadId = 0;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 OpenSSL_Mgr::SendMsg(const std::string& strData, const ELTE_UINT32& uiLength)
{
	LOG_TRACE();

	ELTE_INT32 iErrorCode = 0;

	MutexLocker Locker(m_MutexSSL);
	if (NULL == m_ssl)
	{
		LOG_RUN_ERROR("SSL is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	//发送消息
	ELTE_INT32 iLen = SSL_write(m_ssl, strData.c_str(), (ELTE_INT32)uiLength);
	if (iLen < 0)
	{
		iErrorCode = SSL_get_error(m_ssl, iLen);
		LOG_RUN_ERROR("%s message Send failure ！Error code is %d.", strData.c_str(), iErrorCode);
		return eLTE_SDK_ERR_SEND_MSG;
	}
	
	if ((ELTE_UINT32)iLen != uiLength)
	{
		LOG_RUN_ERROR("message Send not completely.");
		return eLTE_SDK_ERR_SEND_MSG;
	}
	return eLTE_SDK_ERR_SUCCESS;
}


ELTE_INT32 OpenSSL_Mgr::SSL_ReConnect()
{
	LOG_TRACE();
	ELTE_INT32 iRet=0;
	Json::Value ErrValue;
	Json::FastWriter writer;
	std::string strErrValue;
	std::string xmlStr;

	if (NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("m_pUserMgr is NULL!");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	iRet = socket.ConnectAgain();

	ErrValue["cmd"] = "290000";
	ErrValue["isdn"] = m_pUserMgr->GetUserID();
	ErrValue["statustype"] = "6";
	ErrValue["para"] = "value";
	ErrValue["session"] = m_pUserMgr->GetSessionId();

	if (eLTE_SDK_ERR_SUCCESS == iRet)
	{
		LOG_RUN_INFO("SSL_ReConnect::SIP_RECONNECT SUCCESS!");
		//msg:link accessible		
		ErrValue["statusvalue"] = "10";		
		//reset BLoseCon
		m_iLoseCon = 0;
		//reset Heart Beat sign
		m_iHeartBeat = 0;
		//send single to stop waitting
		::SetEvent(m_pUserMgr->GetEventHandle());
	}
	else
	{
		LOG_RUN_INFO("SSL_ReConnect::SIP_DISCONNECT");
		//msg：link unaccessible
		ErrValue["statusvalue"] = "9";		
	}

	//send message back 
	strErrValue = writer.write(ErrValue);
	iRet = XMLProcess::SetXml_NotifyModuleStatus(ErrValue, xmlStr);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyUserStatus of logout failed.");
	}

	EventCallBack m_fnEventCallBack = m_pUserMgr->GetEventCallBack();
	//if (WAIT_TIMEOUT == WaitForSingleObject(m_fnEventCallBack, WAIT_CALLBACK_TIME))
	//{
	//	return eLTE_SDK_ERR_CONNECT_SERVER;
	//}

	if(m_fnEventCallBack)
	{
		ELTE_UINT32 uiLength = xmlStr.length();
		if (uiLength > 0)
		{
			m_fnEventCallBack(ELTE_Event_NotifyModuleStatus, (ELTE_VOID*)xmlStr.c_str(), uiLength, m_pUserMgr->GetEventUserData());
		}		
	}

	return iRet;
}

ELTE_INT32 OpenSSL_Mgr::SSL_HeartBeat()
{
	LOG_TRACE();
	LOG_RUN_INFO("FUNCTION : SSL_HeartBeat");
	ELTE_INT32 iRet=0;
	if (NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	//send message to confirm status
	iRet = socket.KeepAlive();
	if(RET_SUCCESS != iRet)
	{	
		LOG_RUN_ERROR("Send keep alive req failed.");
		//start Reconnect Timer & Stop Heart beat
		//or just send a single
	}
	return iRet;
}