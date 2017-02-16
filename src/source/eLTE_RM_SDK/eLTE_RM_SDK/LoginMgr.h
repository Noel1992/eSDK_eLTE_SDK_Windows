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
filename		: 	LoginMgr.h
author			:	zWX229156
created		    :	2015/1/5
description	    :	用户登录类
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 初始版本
*********************************************************************/

#ifndef __LOGIN_MGR_H__
#define __LOGIN_MGR_H__

#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CLoginMgr
{
public:
	CLoginMgr();
	~CLoginMgr();

	//登录
	ELTE_INT32 Login(const ELTE_CHAR* pUserId, const ELTE_CHAR* pPasswd, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, const ELTE_UINT32& sipPort) const;

	//注销
	ELTE_INT32 Logout(const ELTE_CHAR* pUserId) const;

	//触发状态上报
	ELTE_INT32 TriggerStatusReport(const ELTE_INT32& iEnableStatusReport) const;

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

private:
	CUserMgr*    m_pUserMgr;
};

#endif // __LOGIN_MGR_H__
