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
description :    使用openssl加密的Socket类，负责客户端监听和客户端线程管理，并接受客户端请求
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
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
	// 获取客户端ssl，业务操作只能有一个客户端连接
	// 尽管服务器支持多客户端同时连接
	SSL* GetOptSSL();
	unsigned int GetSeqNum();
	// 踢出客户端连接
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
