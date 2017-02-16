/********************************************************************
filename		: 	OpenSSL_Mgr.h
author			:	zWX229156
created		    :	2015/1/7
description	    :	ssl管理类（进行ctx和ssl实例定义以及释放，服务端数据的接受、存储以及处理）
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/7 初始版本
*********************************************************************/
#ifndef __OPENSSL_MGR_H__
#define __OPENSSL_MGR_H__

#include <openssl/ssl.h>
#include <queue>
#include "eLTE_Types.h"
#include "eLTE_Message.h"
#include "mutex_locker.h"

class CUserMgr;//lint !e763
class OpenSSL_Mgr
{
private:
	OpenSSL_Mgr();
	~OpenSSL_Mgr();

public:
	//获取实例
	static OpenSSL_Mgr& Instance();
	//初始化ctx
	ELTE_INT32 Init_SSL_CTX();
	//释放ctx以及ssl
	ELTE_VOID Uninit_SSL_CTX();
	//连接ssl
	ELTE_INT32 Connect_SSL(ELTE_INT32 socketfd);
	//创建接受数据线程
	ELTE_INT32 StartRevMsg();
	//接受数据线程
	static ELTE_UINT32 __stdcall RevMsgThread(ELTE_VOID* param);
	//停止接受数据线程
	ELTE_INT32 StopRevMsg();
	//创建消息处理线程
	ELTE_INT32 StartProMsg();
	//消息处理线程
	static ELTE_UINT32 __stdcall ProMsgThread(ELTE_VOID* param);
	//停止消息处理线程
	ELTE_INT32 StopProMsg();
	//发送数据
	ELTE_INT32 SendMsg(const ELTE_VOID* pData, const ELTE_UINT32& uiLength);
	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
private:
	//接受服务器传来的数据
	ELTE_VOID DoRevMsg();
	//消息处理
	ELTE_VOID DoProMsg();
	//释放资源
	ELTE_VOID CloseResource();
	//分发消息
	ELTE_VOID DispatchMsg(PACKET_DATA& Packet);
	//释放数据
	ELTE_VOID CleanMsgQuene();
	//处理事件的packetdata
	ELTE_VOID ProcessPacketData(const PACKET_DATA& packetData);

	//信息分发返回类型低圈复杂度判断
	ELTE_INT32 CheckDispatchMsg(PACKET_DATA& Packet, ELTE_UINT32 Type);


private:
	SSL_CTX* m_ctx;
	SSL* m_ssl;
	HANDLE m_revThread;
	ELTE_UINT32 m_revThreadId;
	ELTE_INT32 m_bStopRevThtread;
	std::queue<PACKET_DATA> m_msgQueue;
	MUTEX_HANDLE m_MutexMsgQueue;
	HANDLE m_proMsgThread;
	ELTE_UINT32 m_proMsgThreadId;
	ELTE_INT32  m_bStopProMsgThtread;
	MUTEX_HANDLE m_MutexSSL;
	CUserMgr* m_pUserMgr;
};

#endif
