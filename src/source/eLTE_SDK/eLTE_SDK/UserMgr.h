/********************************************************************
filename		: 	UserMgr.h
author			:	zWX229156
created		    :	2015/1/5
description	    :	用户管理类
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 初始版本
*********************************************************************/

#ifndef __USER_MGR_H__
#define __USER_MGR_H__

#include "eLTE_Types.h"
#include "LoginMgr.h"
#include "SSL_Socket.h"
#include "ProvisionMgr.h"
#include "ServerMgr.h"
#include "OperationMgr.h"
#include "MediaMgr.h"
#include "eLTE_Timer.h"
#include "eLTE_Message.h"
#include "mutex_locker.h"
#include <string>

#define ELTE_SDK_DLL_NAME   "eLTE_SDK.dll"
#define WAIT_OBJECT_TIME    30000

class CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();

	//设置回调函数和用户数据 
	ELTE_VOID SetEventCallBack(EventCallBack fEventCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//设置视频数据回调函数和用户数据 
	ELTE_VOID SetYUVFrameCallBack(YUVFrameCallBack fYUVFrameCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//获取事件回调函数
	EventCallBack GetEventCallBack() const { return m_fnEventCallBack;}

	//获取用户数据
	ELTE_VOID* GetEventUserData() {return m_pUserData;}

	//设置调度员账号
	ELTE_VOID SetUserID(const std::string& strUserID);

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

	//获取SSL_Socket对象
	const SSL_Socket& GetSSLSocket() const {return m_sslSocket;};

	//获取CLoginMgr对象
	const CLoginMgr& GetLoginMgr() const {return m_loginMgr;};

	//获取CProvisionMgr对象
	const CProvisionMgr& GetProvisionMgr() const {return m_provisionMgr;};

	//获取CServerMgr对象
	const CServerMgr& GetServerMgr() const {return m_serverMgr;};

	//获取COperationMgr对象
	const COperationMgr& GetOperationMgr() const {return m_operationMgr;};

	//获取CMediaMgr对象
	const CMediaMgr& GetMediaMgr() const {return m_mediaMgr;};

	//获取CTimer对象
	const CTimer& GetTimer() const {return m_timer;};

	//等待资源
	ELTE_VOID WaitObject(const HANDLE &event = NULL) const;

	ELTE_INT32 WaitObject(const ELTE_UINT32& uiWaitTime, const HANDLE &event = NULL);

	//获取锁
	MUTEX_HANDLE GetMutexHandle() {return m_MutexInterface;};

	//设置包数据
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);

	//获取包数据
	const PACKET_DATA& GetPacketData() const {return m_packetData;};

	//获取事件对象
	HANDLE GetEventHandle() {return m_EventInterface;};

	//设置服务状态值
	ELTE_VOID SetServerStatus(const ELTE_INT32& serverStatus) {m_serverStatus = serverStatus;};

	//获取服务状态值
	ELTE_INT32 GetServerStatus() const {return m_serverStatus;};

	//设置会话值
	ELTE_VOID SetSessionId(const std::string& sessionId) {m_sessionId = sessionId;};

	//获取会话值
	std::string GetSessionId() const {return m_sessionId;};

	//设置是否断连值
	ELTE_VOID SetbIsDisconnect(const ELTE_INT32& bIsDisconnect) {m_bIsDisconnect = bIsDisconnect;};

	//获取是否断连值
	ELTE_INT32 GetbIsDisconnect() const {return m_bIsDisconnect;};

	//设置是否处理媒体流
	ELTE_VOID SetBypass(const ELTE_INT32& iBypass) const;

private:
	EventCallBack m_fnEventCallBack;
	ELTE_VOID*    m_pUserData;

	YUVFrameCallBack m_fnYUVFrameCallBack;
	ELTE_VOID*    m_pYUVFrameUserData;

	std::string   m_strUserID;
	std::string   m_strUserPwd;
	std::string   m_strLocalIP;
	SSL_Socket    m_sslSocket;
	CLoginMgr     m_loginMgr;
	CProvisionMgr m_provisionMgr;
	CServerMgr    m_serverMgr;
	MUTEX_HANDLE  m_MutexInterface;
	PACKET_DATA   m_packetData;
	HANDLE        m_EventInterface;
	COperationMgr m_operationMgr;
	CMediaMgr     m_mediaMgr;
	CTimer        m_timer;
	ELTE_INT32    m_serverStatus;
	std::string   m_sessionId;
	ELTE_INT32    m_bIsDisconnect;
public:
	static ELTE_INT32 m_iBypass;
};

#endif // __USER_MGR_H__
