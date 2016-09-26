#include "stdafx.h"
#include "LoginMgr.h"
#include "eLTE_Error.h"
#include "eLTE_Log.h"
#include "eLTE_Xml.h"
#include "eLTE_Tool.h"
#include "UserMgr.h"

CLoginMgr::CLoginMgr()
{
	m_pUserMgr = NULL;
}

CLoginMgr::~CLoginMgr()
{
	m_pUserMgr = NULL;
}

ELTE_INT32 CLoginMgr::Login(const ELTE_CHAR* pUserId, const ELTE_CHAR* pPasswd, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, const ELTE_UINT32& sipPort) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml loginReqXml;
	(void)loginReqXml.AddElem("Content");
	(void)loginReqXml.AddChildElem("UserID");
	(void)loginReqXml.IntoElem();
	(void)loginReqXml.SetElemValue(pUserId);
	(void)loginReqXml.AddElem("PWD");
	(void)loginReqXml.SetElemValue(pPasswd);
	(void)loginReqXml.AddElem("ServerIP");
	(void)loginReqXml.SetElemValue(pServerIP);
	(void)loginReqXml.AddElem("LocalIP");
	(void)loginReqXml.SetElemValue(pLocalIP);
	(void)loginReqXml.AddElem("ServerSIPPort");
	(void)loginReqXml.SetElemValue(eLTE_Tool::UInt2String(sipPort).c_str());

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_LOGIN_REQ, loginReqXml, TRUE);
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

ELTE_INT32 CLoginMgr::Logout(const ELTE_CHAR* pUserId) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml logoutReqXml;
	(void)logoutReqXml.AddElem("Content");
	(void)logoutReqXml.AddChildElem("UserID");
	(void)logoutReqXml.IntoElem();
	(void)logoutReqXml.SetElemValue(pUserId);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_LOGOUT_REQ, logoutReqXml, TRUE);
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

ELTE_INT32 CLoginMgr::TriggerStatusReport(const ELTE_INT32& iEnableStatusReport) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	//请求xml
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("EnableStatusReport");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(eLTE_Tool::Int2String(iEnableStatusReport).c_str());

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_TRIGGERSTATUSREPORT_REQ, reqXml, TRUE);
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

ELTE_VOID CLoginMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}
