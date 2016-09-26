/********************************************************************
filename		: 	ServerMgr.h
author			:	zWX229156
created		    :	2015/1/8
description	    :	服务管理类（服务启动、读取配置文件、连接服务以及断开服务）
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/8 初始版本
*********************************************************************/

#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "eLTE_Types.h"
#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CServerMgr
{
public:
	CServerMgr();
	~CServerMgr();

	//加载资源
	ELTE_INT32 Init(ELTE_INT32 iMediaBypass);

	//释放资源
	ELTE_INT32 CleanUp() const;

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//获取初始化标志
	ELTE_INT32 GetInitValue() const {return m_bInit;};

	//启动服务
	ELTE_VOID StartServer() const;

	//停止服务
	ELTE_VOID StopServer() const;

	//判断服务状态
	ELTE_INT32 ServerIsRunning() const;

private:
	CUserMgr*   m_pUserMgr;
	ELTE_INT32  m_bInit;
	ELTE_INT32  m_bMediaBypass;
};

#endif // __SERVER_MGR_H__
