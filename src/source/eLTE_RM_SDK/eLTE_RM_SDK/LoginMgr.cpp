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
//User Login class
#include "LoginMgr.h"
//Error Code define class
#include "eLTE_Error.h"
//log manage class
#include "eLTE_Log.h"
//#include "eLTE_Xml.h"

//public function interface class
#include "eLTE_Tool.h"
//user manage class
#include "UserMgr.h"
//Json definition class 
#include "json.h"
//Timer definition class
#include "eLTE_RM_Timer.h"

//server port
#define SERVER_PORT 8200

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

	SSL_Socket& sslSocket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	sslSocket.SetParam(SERVER_PORT, pServerIP);
	//建立连接
	ELTE_INT32 iRet = sslSocket.BeginConnect();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Connect ssl socket failed.");
		return iRet;
	}

	Json::Value value;
	Json::FastWriter writer;

	//先发送接入请求
	value["cmd"] = "0FFFF0";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_CONNECT_REQUEST);
	value["isdn"] = pUserId;
	std::string connectStr = writer.write(value);
	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	iRet = socket.SendMsg(connectStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Connect req failed.");
		return iRet;
	}
	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_CONNECT_REQUEST));
	//等待sessionid
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Wait session id failed.");
		return iRet;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	

	//登录请求报文
	value["cmd"] = "0FFFF1";
	value["isdn"] = pUserId;
	value["pwd"] = pPasswd;
	value["serverip"] = pServerIP;
	value["serverport"] = eLTE_Tool::UInt2String(sipPort);	
	value["localip"] = pLocalIP;
	value["session"] = m_pUserMgr->GetSessionId();
	value["seq"] = eLTE_Tool::UInt2String(SEQ_LOGIN);
	std::string loginReqStr = writer.write(value);
	
	//发送消息
	iRet = socket.SendMsg(loginReqStr);

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
	
	//Stop heart beat first
	if (RET_SUCCESS != RMTimer::Instance()->StopTimer(HEARTBEAT_TIMER))
	{
		LOG_RUN_ERROR("Timer Stop warning: stop timer error or timer stop running");
	}
	
	//登出请求报文
	Json::Value value;
	Json::FastWriter writer;
	value["cmd"] = "0FFFF2";
	value["isdn"] = pUserId;
	value["pwd"] = m_pUserMgr->GetUserPwd();
	value["serverip"] = m_pUserMgr->GetServerIP();
	value["serverport"] = eLTE_Tool::UInt2String(m_pUserMgr->GetServerSIPPort());
	value["localip"] = m_pUserMgr->GetLocalIP();
	value["session"] = m_pUserMgr->GetSessionId();
	
	value["seq"] = eLTE_Tool::UInt2String(SEQ_LOGOUT);
	std::string loginReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(loginReqStr);
	return iRet;
}

ELTE_INT32 CLoginMgr::TriggerStatusReport(const ELTE_INT32& iEnableStatusReport) const
{
	//unused
	return eLTE_SDK_ERR_SUCCESS;
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
