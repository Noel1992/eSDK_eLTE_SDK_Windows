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
filename		: 	SSL_Socket.h
author			:	zWX229156
created		    :	2015/1/7
description	    :	socket管理类（跟服务端建立连接、断开连接以及进行消息的转发）
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/7 初始版本
*********************************************************************/

#ifndef __SSL_SOCKET_H__
#define __SSL_SOCKET_H__

#include <string>
#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include "mutex_locker.h"
//#include "eLTE_Xml.h"

class CUserMgr;//lint !e763
class SSL_Socket
{
public:
	SSL_Socket();
	~SSL_Socket();

	//设置服务器端口号以及IP地址
	ELTE_VOID SetParam(ELTE_UINT32 uiPort, const std::string& strHost);
	//初始化socket
	ELTE_INT32 Init_SSL_Socket();
	//释放socket
	ELTE_INT32 Uninit_SSL_Socket();
	//建立连接
	ELTE_INT32 BeginConnect(const ELTE_INT32& bConnectAgain = FALSE);	
	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//发送消息
	ELTE_INT32 SendMsg(const std::string& reqXmlStr);
	//闪断重连
	ELTE_INT32 ConnectAgain();

	//清空缓存
	ELTE_INT32 CleanBuffer();

public:
	static UINT __stdcall StartReceiveScriptServer(LPVOID lpParam);

private:
	//释放资源
	ELTE_INT32 CloseResource();
	//连接服务
	ELTE_INT32 ConnectSSL(const ELTE_INT32& bConnectAgain = FALSE);

public:
	//保活
	ELTE_INT32 KeepAlive();

private:
	ELTE_UINT32  m_uiPort;
	std::string  m_strHost;
	ELTE_UINT32  m_socket;
	//ELTE_UINT32  m_socketGTR;
	//ELTE_UINT32  m_socketGTRAccept;
	CUserMgr*    m_pUserMgr;	
};
#endif //__SSL_SOCKET_H__
