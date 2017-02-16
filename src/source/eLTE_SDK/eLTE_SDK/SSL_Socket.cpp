#include "stdafx.h"
#include "SSL_Socket.h"
#include "OpenSSL_Mgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "EventMgr.h"
#include "ConfigMgr.h"
#include "UserMgr.h"
#include <process.h>

#pragma comment(lib, "ws2_32.lib")
#define TRY_CONNECT_NUM 6
#define CONNECT_AGAIN_TIME 25000
#define WAIT_STOP_THREAD_TIME 3*1000

SSL_Socket::SSL_Socket(void)
	: m_uiPort(0)
	, m_strHost("")
	, m_socket(INVALID_SOCKET)
	, m_pUserMgr(NULL)
	, m_uiSeqId(0)
	, m_connectMsgThreadId(0)
	, m_connectMsgThread(NULL)
	, m_bStopConMsgThtread(TRUE)
{
	m_MutexSeqId = CREATE_MUTEX();
	m_EventConnectReq = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

SSL_Socket::~SSL_Socket(void)
{
	m_pUserMgr = NULL;
	try
	{
		CloseResource();
		(void)StopSendConnectMsg();
	}
	catch (...)
	{
		
	}
	if(m_EventConnectReq)
	{
		CloseHandle(m_EventConnectReq); 
		m_EventConnectReq = NULL;
	}
	RELEASE_PACKET(m_connectReqPacketData);
}//lint !e1579

ELTE_UINT32 SSL_Socket::GetSeqId()
{
	MutexLocker Locker(m_MutexSeqId);
	return (++m_uiSeqId);	 
}

ELTE_INT32 SSL_Socket::SendMsg(const ELTE_USHORT& usMsgType, CXml& reqXml, const ELTE_INT32& bBodyExist)
{
	LOG_TRACE();
	PACKET_HEAD packetData;
	packetData.MsgType = usMsgType;
	packetData.SeqID = GetSeqId();
	if(NULL != m_pUserMgr && !m_pUserMgr->GetSessionId().empty())
	{
		eSDK_MEMCPY(packetData.SessionID, sizeof(packetData.SessionID), m_pUserMgr->GetSessionId().c_str(), sizeof(packetData.SessionID));
	}
	ELTE_UINT32 uiXmlLen = 0;
	const ELTE_CHAR* pValue = NULL;
	packetData.PacketLength += PACKET_HEAD_SIZE;
	if(bBodyExist)
	{
		pValue = reqXml.GetXMLStream(uiXmlLen);
		packetData.PacketLength += uiXmlLen;
	}
	packetData.PacketLength += 1;
	ELTE_CHAR* pBuf = new ELTE_CHAR[packetData.PacketLength];
	if (NULL == pBuf)
	{
		LOG_RUN_ERROR("New buf failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	eSDK_MEMSET(pBuf, 0x00, packetData.PacketLength);
	eSDK_MEMCPY(pBuf, packetData.PacketLength, &packetData, PACKET_HEAD_SIZE);
	if(bBodyExist && (NULL != pValue))
	{
		eSDK_MEMCPY(pBuf + PACKET_HEAD_SIZE, packetData.PacketLength, pValue, uiXmlLen);
	}
	//����
	ELTE_INT32 iRet = OpenSSL_Mgr::Instance().SendMsg((ELTE_VOID*)pBuf, packetData.PacketLength);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		if(NULL != pBuf)
		{
			delete [] pBuf;
			pBuf = NULL;
		}
		LOG_RUN_ERROR("Send msg failed.");
		if(NULL == m_pUserMgr)
		{
			return eLTE_SDK_ERR_NULL_POINTER;
		}
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
		if(!serverMgr.ServerIsRunning() || eLTE_SDK_ERR_SERVER_NOT_RUNNING == m_pUserMgr->GetServerStatus())
		{
			m_pUserMgr->SetServerStatus(0);
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
		}
		return iRet;//lint !e438
	}
	if(NULL != pBuf)
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	return eLTE_SDK_ERR_SUCCESS;//lint !e438
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
	OpenSSL_Mgr::Instance().SetUserMgr(pUserMgr);
}

ELTE_INT32 SSL_Socket::Init_SSL_Socket()
{
	LOG_TRACE();
	// ״̬У��
	if (INVALID_SOCKET != m_socket)
	{
		LOG_RUN_INFO("Socket is exist.");
		return eLTE_SDK_ERR_SUCCESS;
	}

	// ��ʼ���׽��ֶ�̬��
	WSADATA _wsd;
	if (0 != WSAStartup(MAKEWORD(2,2), &_wsd))
	{
		LOG_RUN_ERROR("WSAStartup failed.");
		return eLTE_SDK_ERR_WSA_START_UP;
	}

	// ��ʼ��openssl��
	ELTE_INT32 iRet = OpenSSL_Mgr::Instance().Init_SSL_CTX();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		WSACleanup();
		LOG_RUN_ERROR("Init_SSL_CTX failed.");
		return iRet;
	}

	// �����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		WSACleanup();
		LOG_RUN_ERROR("Create socket failed.");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SSL_Socket::Uninit_SSL_Socket()
{
	LOG_TRACE();
	m_uiPort = 0;
	m_strHost = "";

	CloseResource();
}

ELTE_INT32 SSL_Socket::ConnectSSL(const ELTE_INT32& bConnectAgain)
{
	LOG_TRACE();
	static ELTE_INT32 bInitAddr = FALSE;
	static SOCKADDR_IN _serverAddr;
	// ״̬У��
	if (INVALID_SOCKET == m_socket)
	{
		LOG_RUN_ERROR("Please init socket first.");
		return eLTE_SDK_ERR_FAILED;
	}

	// ���÷�������ַ
	if(!bInitAddr)
	{
		eSDK_MEMSET(&_serverAddr, 0, sizeof(SOCKADDR_IN));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
		_serverAddr.sin_port = htons((ELTE_USHORT)m_uiPort);
		if(bConnectAgain)
		{
			bInitAddr = TRUE;
		}
	}

	// ���ӷ�����
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
			CloseResource();
			LOG_RUN_ERROR("Try connect failed.");
		}
		return eLTE_SDK_ERR_CONNECT_SERVER;
	}
	// SSL���ӷ�����
	iRet = OpenSSL_Mgr::Instance().Connect_SSL((int)m_socket);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		if(!bConnectAgain)
		{
			CloseResource();
		}
		LOG_RUN_ERROR("Connect_SSL failed. %d", GetLastError());
		return iRet;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SSL_Socket::BeginConnect()
{
	LOG_TRACE();
	ELTE_INT32 iRet = ConnectSSL();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		return iRet;
	}
	// SSL��������ȡ���ݣ�����������߳�
	iRet = OpenSSL_Mgr::Instance().StartRevMsg();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CloseResource();
		LOG_RUN_ERROR("Start rev msg failed.");
		return iRet;
	}

	iRet = OpenSSL_Mgr::Instance().StartProMsg();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CloseResource();
		LOG_RUN_ERROR("Start pro msg failed.");
		return iRet;
	}

	iRet = CEventMgr::Instance().StartEventProcess();
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CloseResource();
		LOG_RUN_ERROR("Start pro event failed.");
		return iRet;
	}

	//���ӷ�����֤
	iRet = Connect();
	if(NULL == m_pUserMgr)
	{
		CloseResource();
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	if (eLTE_SDK_ERR_SERVICE_EXPIRE == iRet)
	{
		//ֹͣ������Ϣ
		(void)OpenSSL_Mgr::Instance().StopRevMsg();
		//���������ٴ�����
		m_pUserMgr->GetServerMgr().StopServer();
		m_pUserMgr->GetServerMgr().StartServer();
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		// �����׽���
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_socket)
		{
			CloseResource();
			LOG_RUN_ERROR("Create socket failed.");
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		iRet = ConnectSSL();
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			CloseResource();
			return iRet;
		}
		iRet = OpenSSL_Mgr::Instance().StartRevMsg();
		if (eLTE_SDK_ERR_SUCCESS != iRet)
		{
			CloseResource();
			LOG_RUN_ERROR("Start pro msg failed.");
			return iRet;
		}
	}
	else if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetbIsDisconnect(TRUE);
		CloseResource();
		LOG_RUN_ERROR("Connect and verify password error,iRet is %d.",iRet);
		return iRet;
	}

	//�������ʱ��,���ͱ�������
	CTimer& timer = const_cast<CTimer&>(m_pUserMgr->GetTimer());
	timer.SetTimer(KEEPALIVE_TIME * 1000);

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 SSL_Socket::KeepAlive()
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml connectReqXml;
	ELTE_INT32 iRet = SendMsg(ELTE_SERVICE_KEEPALIVE_REQ, connectReqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())
		{
			m_pUserMgr->SetServerStatus(0);
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
		}
		return iRet;
	}
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 SSL_Socket::Connect()
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml connectReqXml;
	(void)connectReqXml.AddElem("Content");
	(void)connectReqXml.AddChildElem("ConnectPassword");
	(void)connectReqXml.IntoElem();
	(void)connectReqXml.SetElemValue((CConfigMgr::Instance().GetPassword()).c_str());
	ELTE_INT32 iRet = SendMsg(ELTE_SERVICE_CONNECT_REQ, connectReqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	iRet = (ELTE_INT32)::WaitForSingleObject(m_EventConnectReq, WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())
		{
			m_pUserMgr->SetServerStatus(0);
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
		}
		return iRet;
	}
	iRet = m_connectReqPacketData.RspCode;
	if(eLTE_SDK_ERR_SUCCESS == iRet)
	{
		m_pUserMgr->SetSessionId(m_connectReqPacketData.SessionID);
	}
	::ResetEvent(m_EventConnectReq);
	return iRet;
}

ELTE_INT32 SSL_Socket::ConnectAgain()
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
	ELTE_INT32 iRet = 0;
	if(serverMgr.ServerIsRunning())
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		// �����׽���
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_socket)
		{
			CloseResource();
			LOG_RUN_ERROR("Create socket failed.");
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		LOG_RUN_INFO("Connect again start...");
		double start = GetTickCount(); 
		double end = 0;
		do 
		{
			iRet = ConnectSSL(TRUE);
			if(eLTE_SDK_ERR_SUCCESS == iRet)
			{
				break;
			}
		
			end = GetTickCount(); 
		} while (end - start < CONNECT_AGAIN_TIME);
	}
	else
	{
		return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
	}
	LOG_RUN_INFO("Connect again end...");

	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Connect again ssl failed.");
		m_pUserMgr->SetServerStatus(eLTE_SDK_ERR_SERVER_NOT_RUNNING);
		return iRet;
	}
	iRet = StartSendConnectMsg();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Start send connect msg thread failed.");
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SSL_Socket::CloseResource()
{
	// SSLȥ��ʼ��
	OpenSSL_Mgr::Instance().Uninit_SSL_CTX();

	if (INVALID_SOCKET != m_socket)
	{
		//�ر��׽���
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		//�ͷ��׽�����Դ
		WSACleanup();
	}
	DESTROY_MUTEX(m_MutexSeqId);
}

ELTE_INT32 SSL_Socket::DisConnect()
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml disconnectReqXml;
	//������Ϣ
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = SendMsg(ELTE_SERVICE_DISCONNECT_REQ, disconnectReqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Do not get disconnect rsp, iRet:%d.", iRet);
	}
	iRet = m_pUserMgr->GetPacketData().RspCode;
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Disconnect rsp is not success, iRet:%d.", iRet);
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SSL_Socket::SetPacketData(const PACKET_DATA& packetData)
{
	RELEASE_PACKET(m_connectReqPacketData);
	m_connectReqPacketData = packetData;
}

ELTE_INT32 SSL_Socket::StartSendConnectMsg()
{
	LOG_TRACE();
	m_bStopConMsgThtread = TRUE;
	if(NULL != m_connectMsgThread)
	{
		CloseHandle(m_connectMsgThread);
		m_connectMsgThread = NULL;
		LOG_RUN_DEBUG("Stop connect msg thread success, thread id[%d]", m_connectMsgThreadId);
	}

	m_connectMsgThread = (HANDLE)_beginthreadex(NULL, 0, ConnectMsgThread, (ELTE_VOID*)this, 0, &m_connectMsgThreadId);
	if(NULL == m_connectMsgThread)
	{
		LOG_RUN_ERROR("Start connect msg thread failed");
		return eLTE_SDK_ERR_CREATE_OBJECT;
	}
	else
	{
		m_bStopConMsgThtread = FALSE;
		LOG_RUN_DEBUG("Start connect msg thread success, thread id[%d]", m_connectMsgThreadId);
		return eLTE_SDK_ERR_SUCCESS;
	}
}

ELTE_INT32 SSL_Socket::StopSendConnectMsg()
{
	LOG_TRACE();
	m_bStopConMsgThtread = TRUE;
	if(NULL == m_connectMsgThread)
	{
		LOG_RUN_DEBUG("Stop connect msg thread success, no thread is running");
		return eLTE_SDK_ERR_SUCCESS;
	}

	ELTE_ULONG ulResult = ::WaitForSingleObject(m_connectMsgThread, WAIT_STOP_THREAD_TIME);//lint -e206
	if(0 != ulResult)
	{
		LOG_RUN_ERROR("Wait for single object failed.");
	}
	CloseHandle(m_connectMsgThread);
	m_connectMsgThread = NULL;
	LOG_RUN_DEBUG("Stop connect msg thread success, thread id[%d]", m_connectMsgThreadId);
	m_connectMsgThreadId = 0;
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_UINT32 __stdcall SSL_Socket::ConnectMsgThread(ELTE_VOID* param)
{
	if(NULL == param)
	{
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	SSL_Socket* pSocket = (SSL_Socket*)param;
	pSocket->DoRevMsg();
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID SSL_Socket::DoRevMsg()
{
	while(!m_bStopConMsgThtread)
	{
		//���ӷ�����֤,������Ҫ׷�ӷ���sessionid
		ELTE_INT32 iRet = Connect();
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Connect and verify again failed.");
			m_pUserMgr->SetServerStatus(eLTE_SDK_ERR_SERVER_NOT_RUNNING);
		}
		break;
	}
}
