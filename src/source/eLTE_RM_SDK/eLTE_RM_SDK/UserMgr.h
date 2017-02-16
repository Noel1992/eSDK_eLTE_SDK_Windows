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
/********************************************************************
filename		: 	UserMgr.h
author			:	zWX229156
created		    :	2015/10/12
description	    :	用户管理类
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/12 初始版本
*********************************************************************/

#ifndef __USER_MGR_H__
#define __USER_MGR_H__

#include "eLTE_Types.h"
//User Login class
#include "LoginMgr.h"
#include "ProvisionMgr.h"
//Socket manage class
#include "SSL_Socket.h"
#include "mutex_locker.h"
#include "MediaMgr.h"
#include <string>

#define ELTE_RM_SDK_DLL_NAME   "eLTE_RM_SDK.dll"
#define WAIT_OBJECT_TIME    10000

class CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();

	//设置回调函数和用户数据 
	ELTE_VOID SetEventCallBack(EventCallBack fEventCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//获取事件回调函数
	EventCallBack GetEventCallBack() const { return m_fnEventCallBack;}

	//获取用户数据
	ELTE_VOID* GetEventUserData() {return m_pUserData;}

	//设置调度员账号
	ELTE_VOID SetUserID(const std::string& strUserID);

	//设置服务器IP
	ELTE_VOID SetServerIP(const std::string& strServerIP);

	//设置服务器端口
	ELTE_VOID SetServerSIPPort(const ELTE_UINT32& uiServerSIPPort);

	//获取调度员账号
	std::string GetUserID() const {return m_strUserID;};

	//设置调度员密码
	ELTE_VOID SetUserPwd(const std::string& strUserPwd);

	//获取调度员密码
	std::string GetUserPwd() const {return m_strUserPwd;};

	//设置本地主机IP地址
	ELTE_VOID SetLocalIP(const std::string& strLocalIP);

	//获取本地主机IP地址
	std::string GetLocalIP() const {return m_strLocalIP;};

	//获取本地主机IP地址
	std::string GetServerIP() const {return m_strServerIP;};

	//获取本地主机IP地址
	ELTE_UINT32 GetServerSIPPort() const {return m_uiServerSIPPort;};

	//获取SSL_Socket对象
	const SSL_Socket& GetSSLSocket() const {return m_sslSocket;};

	//获取CLoginMgr对象
	const CLoginMgr& GetLoginMgr() const {return m_loginMgr;};

	//获取CProvisionMgr对象
	const CProvisionMgr& GetProvisionMgr() const {return m_provisionMgr;};

	//获取CMediaMgr对象
	const CMediaMgr& GetMediaMgr() const {return m_mediaMgr;};

	//等待资源
	ELTE_VOID WaitObject(const HANDLE &event = NULL) const;

	ELTE_INT32 WaitObject(const ELTE_UINT32& uiWaitTime, const HANDLE &event = NULL);

	//获取锁
	MUTEX_HANDLE GetMutexHandle() {return m_MutexInterface;};

	//获取事件对象
	HANDLE GetEventHandle() {return m_EventInterface;};


	//设置会话值
	ELTE_VOID SetSessionId(const std::string& sessionId) {m_sessionId = sessionId;};

	//获取会话值
	std::string GetSessionId() const {return m_sessionId;};

	//设置包数据
	ELTE_VOID SetPacketData(const std::string& strPacketData);

	//获取包数据
	const std::string& GetPacketData() const {return m_strPacketData;};

	//Set value of seq and cmd
	ELTE_VOID SetCheckStr(const std::string iseq){m_strseq = iseq;}
	//Get value of seq
	std::string GetCheckSeq(){return m_strseq;}

private:
	EventCallBack m_fnEventCallBack;
	ELTE_VOID*    m_pUserData;

	std::string   m_strUserID;
	std::string   m_strUserPwd;
	std::string   m_strLocalIP;
	std::string   m_strServerIP;
	ELTE_UINT32   m_uiServerSIPPort;
	SSL_Socket    m_sslSocket;
	CLoginMgr     m_loginMgr;
	CProvisionMgr m_provisionMgr;
	CMediaMgr     m_mediaMgr;


	MUTEX_HANDLE  m_MutexInterface;	
	HANDLE        m_EventInterface;
	std::string   m_sessionId;

	std::string   m_strPacketData;
	//string value to record the value of seq
	std::string m_strseq;
};

#endif // __USER_MGR_H__
