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
#include "eLTE_Message.h"
#include "mutex_locker.h"
#include "eLTE_Xml.h"

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
	ELTE_VOID Uninit_SSL_Socket();
	//建立连接
	ELTE_INT32 BeginConnect();	
	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//发送消息
	ELTE_INT32 SendMsg(const ELTE_USHORT& usMsgType, CXml& reqXml, const ELTE_INT32& bBodyExist);
	//闪断重连
	ELTE_INT32 ConnectAgain();
	//断开连接
	ELTE_INT32 DisConnect();
	//获取连接请求事件对象
	HANDLE GetEventHandle() {return m_EventConnectReq;};
	//设置连接回应包数据
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);
	//获取连接回应包数据
	const PACKET_DATA& GetPacketData() const {return m_connectReqPacketData;};
	//连接服务验证
	ELTE_INT32 Connect();
	//开启 闪断、重新连接请求线程
	ELTE_INT32 StartSendConnectMsg();
	//停止 闪断、重新连接请求线程
	ELTE_INT32 StopSendConnectMsg();
	//消息处理线程
	static ELTE_UINT32 __stdcall ConnectMsgThread(ELTE_VOID* param);
private:
	//释放资源
	ELTE_VOID CloseResource();
	//获取序列号
	ELTE_UINT32 GetSeqId();
	//连接服务
	ELTE_INT32 ConnectSSL(const ELTE_INT32& bConnectAgain = FALSE);
	//保活
	ELTE_INT32 KeepAlive();
	//传输重新连接请求
	ELTE_VOID DoRevMsg();

private:
	ELTE_UINT32  m_uiPort;
	std::string  m_strHost;
	ELTE_UINT32  m_socket;
	CUserMgr*    m_pUserMgr;
	ELTE_UINT32  m_uiSeqId;
	MUTEX_HANDLE m_MutexSeqId;
	HANDLE       m_EventConnectReq;
	PACKET_DATA  m_connectReqPacketData;
	ELTE_UINT32 m_connectMsgThreadId;
	HANDLE      m_connectMsgThread;
	ELTE_INT32  m_bStopConMsgThtread;
};
#endif //__SSL_SOCKET_H__
