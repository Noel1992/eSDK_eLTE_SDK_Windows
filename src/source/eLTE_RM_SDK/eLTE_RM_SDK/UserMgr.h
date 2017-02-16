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
description	    :	�û�������
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/12 ��ʼ�汾
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

	//���ûص��������û����� 
	ELTE_VOID SetEventCallBack(EventCallBack fEventCallBack = NULL, ELTE_VOID* pUserData = NULL);

	//��ȡ�¼��ص�����
	EventCallBack GetEventCallBack() const { return m_fnEventCallBack;}

	//��ȡ�û�����
	ELTE_VOID* GetEventUserData() {return m_pUserData;}

	//���õ���Ա�˺�
	ELTE_VOID SetUserID(const std::string& strUserID);

	//���÷�����IP
	ELTE_VOID SetServerIP(const std::string& strServerIP);

	//���÷������˿�
	ELTE_VOID SetServerSIPPort(const ELTE_UINT32& uiServerSIPPort);

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

	//��ȡ��������IP��ַ
	std::string GetServerIP() const {return m_strServerIP;};

	//��ȡ��������IP��ַ
	ELTE_UINT32 GetServerSIPPort() const {return m_uiServerSIPPort;};

	//��ȡSSL_Socket����
	const SSL_Socket& GetSSLSocket() const {return m_sslSocket;};

	//��ȡCLoginMgr����
	const CLoginMgr& GetLoginMgr() const {return m_loginMgr;};

	//��ȡCProvisionMgr����
	const CProvisionMgr& GetProvisionMgr() const {return m_provisionMgr;};

	//��ȡCMediaMgr����
	const CMediaMgr& GetMediaMgr() const {return m_mediaMgr;};

	//�ȴ���Դ
	ELTE_VOID WaitObject(const HANDLE &event = NULL) const;

	ELTE_INT32 WaitObject(const ELTE_UINT32& uiWaitTime, const HANDLE &event = NULL);

	//��ȡ��
	MUTEX_HANDLE GetMutexHandle() {return m_MutexInterface;};

	//��ȡ�¼�����
	HANDLE GetEventHandle() {return m_EventInterface;};


	//���ûỰֵ
	ELTE_VOID SetSessionId(const std::string& sessionId) {m_sessionId = sessionId;};

	//��ȡ�Ựֵ
	std::string GetSessionId() const {return m_sessionId;};

	//���ð�����
	ELTE_VOID SetPacketData(const std::string& strPacketData);

	//��ȡ������
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
