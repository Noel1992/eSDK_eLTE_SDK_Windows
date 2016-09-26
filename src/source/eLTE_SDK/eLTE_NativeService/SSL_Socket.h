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

/********************************************************************
filename    :    SSL_Socket.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    ʹ��openssl���ܵ�Socket�࣬����ͻ��˼����Ϳͻ����̹߳��������ܿͻ�������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 ��ʼ�汾
*********************************************************************/

#ifndef __SSL_SOCKET_H__
#define __SSL_SOCKET_H__

#include <string>
#include <vector>
#include <map>
#include "Logic_Queue.h"


class SSL_Socket
{
private:
	SSL_Socket(void);
	~SSL_Socket(void);

public:
	static SSL_Socket& Instance();
	void SetParam(unsigned int uiPort, const std::string& strHost);
	int Init_SSL_Socket();
	void Uninit_SSL_Socket();
	int BeginAccept();

public:
	// ��ȡ�ͻ���ssl��ҵ�����ֻ����һ���ͻ�������
	// ���ܷ�����֧�ֶ�ͻ���ͬʱ����
	SSL* GetOptSSL();
	unsigned int GetSeqNum();
	// �߳��ͻ�������
	void KickOutClient(SSL* ssl);

private:
	void Clean();
	static unsigned __stdcall ClientThread(void* param);

private:
	unsigned int m_uiPort;
	std::string m_strHost;
	SOCKET m_socketSrv;
	std::map<SSL*, HANDLE> m_clientMap;
	HANDLE m_hMutex;
	unsigned int m_uiSequenceNumber;
	bool m_bKickOut;

public:
	bool m_bStopSvc;	
};

#endif
