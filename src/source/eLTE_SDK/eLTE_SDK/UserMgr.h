/********************************************************************
filename		: 	UserMgr.h
author			:	zWX229156
created		    :	2015/1/5
description	    :	�û�������
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 ��ʼ�汾
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

	//���ûص��������û����� 
	ELTE_VOID SetEventCallBack(EventCallBack fEventCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//������Ƶ���ݻص��������û����� 
	ELTE_VOID SetYUVFrameCallBack(YUVFrameCallBack fYUVFrameCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//��ȡ�¼��ص�����
	EventCallBack GetEventCallBack() const { return m_fnEventCallBack;}

	//��ȡ�û�����
	ELTE_VOID* GetEventUserData() {return m_pUserData;}

	//���õ���Ա�˺�
	ELTE_VOID SetUserID(const std::string& strUserID);

	//��ȡ����Ա�˺�
	std::string GetUserID() const {return m_strUserID;};

	//���õ���Ա����
	ELTE_VOID SetUserPwd(const std::string& strUserPwd);

	//��ȡ����Ա����
	std::string GetUserPwd() const {return m_strUserPwd;};

	//���ñ�������IP��ַ
	ELTE_VOID SetLocalIP(const std::string& strLocalIP);

	//��ȡ��������IP��ַ
	std::string GetLocalIP() const {return m_strLocalIP;};

	//��ȡSSL_Socket����
	const SSL_Socket& GetSSLSocket() const {return m_sslSocket;};

	//��ȡCLoginMgr����
	const CLoginMgr& GetLoginMgr() const {return m_loginMgr;};

	//��ȡCProvisionMgr����
	const CProvisionMgr& GetProvisionMgr() const {return m_provisionMgr;};

	//��ȡCServerMgr����
	const CServerMgr& GetServerMgr() const {return m_serverMgr;};

	//��ȡCOperationMgr����
	const COperationMgr& GetOperationMgr() const {return m_operationMgr;};

	//��ȡCMediaMgr����
	const CMediaMgr& GetMediaMgr() const {return m_mediaMgr;};

	//��ȡCTimer����
	const CTimer& GetTimer() const {return m_timer;};

	//�ȴ���Դ
	ELTE_VOID WaitObject(const HANDLE &event = NULL) const;

	ELTE_INT32 WaitObject(const ELTE_UINT32& uiWaitTime, const HANDLE &event = NULL);

	//��ȡ��
	MUTEX_HANDLE GetMutexHandle() {return m_MutexInterface;};

	//���ð�����
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);

	//��ȡ������
	const PACKET_DATA& GetPacketData() const {return m_packetData;};

	//��ȡ�¼�����
	HANDLE GetEventHandle() {return m_EventInterface;};

	//���÷���״ֵ̬
	ELTE_VOID SetServerStatus(const ELTE_INT32& serverStatus) {m_serverStatus = serverStatus;};

	//��ȡ����״ֵ̬
	ELTE_INT32 GetServerStatus() const {return m_serverStatus;};

	//���ûỰֵ
	ELTE_VOID SetSessionId(const std::string& sessionId) {m_sessionId = sessionId;};

	//��ȡ�Ựֵ
	std::string GetSessionId() const {return m_sessionId;};

	//�����Ƿ����ֵ
	ELTE_VOID SetbIsDisconnect(const ELTE_INT32& bIsDisconnect) {m_bIsDisconnect = bIsDisconnect;};

	//��ȡ�Ƿ����ֵ
	ELTE_INT32 GetbIsDisconnect() const {return m_bIsDisconnect;};

	//�����Ƿ���ý����
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
