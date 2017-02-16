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
filename		: 	ProvisionMgr.h
author			:	zWX229156
created		    :	2015/1/6
description	    :	调度台配置管理类
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/6 初始版本
*********************************************************************/

#ifndef __PROVISION_MGR_H__
#define __PROVISION_MGR_H__

#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CProvisionMgr
{
public:
	CProvisionMgr();
	~CProvisionMgr();

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//获取调度台的群组列表
	ELTE_INT32 GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups) const;

	//获取调度台用户列表
	ELTE_INT32 GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers) const;

	//获取一个群组成员列表
	ELTE_INT32 GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers) const;

	//获取一个群组详细信息
	ELTE_INT32 GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo) const;

	//获取一个用户详细信息
	ELTE_INT32 GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo) const;
	
	//获取一个调度台详细信息
	ELTE_INT32 GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo) const;

private:
	CUserMgr*   m_pUserMgr;
};

#endif // __PROVISION_MGR_H__
