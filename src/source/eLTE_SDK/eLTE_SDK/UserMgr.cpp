#include "stdafx.h"
#include "UserMgr.h"
#include "EventMgr.h"
#include "eLTE_Log.h"
#include "SharedMemoryMgr.h"

ELTE_INT32 CUserMgr::m_iBypass = FALSE;

CUserMgr::CUserMgr()
{
	m_fnEventCallBack = NULL;
	m_pUserData = NULL;
	m_fnYUVFrameCallBack = NULL;
	m_pYUVFrameUserData = NULL;
	m_strUserID = "";
	m_strUserPwd = "";
	m_strLocalIP = "";
	m_sslSocket.SetUserMgr(this);
	m_loginMgr.SetUserMgr(this);
	m_serverMgr.SetUserMgr(this);
	m_provisionMgr.SetUserMgr(this);
	m_operationMgr.SetUserMgr(this);
	m_mediaMgr.SetUserMgr(this);
	m_timer.SetUserMgr(this);
	m_MutexInterface = CREATE_MUTEX();
	m_EventInterface = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_serverStatus = 0;
	m_sessionId = "";
	m_bIsDisconnect = FALSE;
}

CUserMgr::~CUserMgr()
{
	try
	{
		m_fnEventCallBack = NULL;
		m_pUserData = NULL;
		m_fnYUVFrameCallBack = NULL;
		m_pYUVFrameUserData = NULL;
		m_strUserID = "";
		m_strUserPwd = "";
		m_strLocalIP = "";
		DESTROY_MUTEX(m_MutexInterface);
		if(m_EventInterface)
		{
			CloseHandle(m_EventInterface); 
			m_EventInterface = NULL;
		}
		RELEASE_PACKET(m_packetData);
	}
	catch (...)
	{
		
	}
}

ELTE_VOID CUserMgr::SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	m_fnEventCallBack = fEventCallBack;
	m_pUserData = pUserData;
	CEventMgr::Instance().SetEventCallBack(fEventCallBack, pUserData);
}

ELTE_VOID CUserMgr::SetYUVFrameCallBack(YUVFrameCallBack fEventCallBack, ELTE_VOID* pUserData)
{
	m_fnYUVFrameCallBack = fEventCallBack;
	m_pYUVFrameUserData = pUserData;
	SharedMemoryMgr::Instance().SetYUVFrameCallBack(fEventCallBack, pUserData);
}

ELTE_VOID CUserMgr::SetUserID(const std::string& strUserID)
{
	m_strUserID = strUserID;
}

ELTE_VOID CUserMgr::SetUserPwd(const std::string& strUserPwd)
{
	m_strUserPwd = strUserPwd;
}

ELTE_VOID CUserMgr::SetLocalIP(const std::string& strLocalIP)
{
	m_strLocalIP = strLocalIP;
}

ELTE_VOID CUserMgr::WaitObject(const HANDLE &event) const
{
	HANDLE waitObject;
	if(NULL == event)
	{
		waitObject = m_EventInterface;
	}
	else
	{
		waitObject = event;
	}
	for(;;)
	{
		ELTE_ULONG ulResult = ::WaitForSingleObject(waitObject, 0);
		if(0 != ulResult)
		{
			// 时间未到
			continue;
		}
		else
		{
			break;
		}
	}
}

ELTE_INT32 CUserMgr::WaitObject(const ELTE_UINT32& uiWaitTime, const HANDLE &event)
{
	HANDLE waitObject;
	if(NULL == event)
	{
		waitObject = m_EventInterface;
	}
	else
	{
		waitObject = event;
	}
	//等待资源
	ELTE_ULONG ulResult = ::WaitForSingleObject(waitObject, uiWaitTime);
	if(0 == ulResult)
	{
		// 成功
		return eLTE_SDK_ERR_SUCCESS;
	}
	else if(WAIT_TIMEOUT == ulResult)
	{
		// 等待超时
		LOG_RUN_ERROR("Wait timeout.");
		return eLTE_SDK_ERR_WAIT_TIME_OUT;
	}
	else
	{
		LOG_RUN_ERROR("Wait event failed.");
		return eLTE_SDK_ERR_FAILED;
	}
}

ELTE_VOID CUserMgr::SetPacketData(const PACKET_DATA& packetData)
{
	RELEASE_PACKET(m_packetData);
	m_packetData = packetData;
}

ELTE_VOID CUserMgr::SetBypass(const ELTE_INT32& iBypass) const
{
	m_iBypass = iBypass;
}
