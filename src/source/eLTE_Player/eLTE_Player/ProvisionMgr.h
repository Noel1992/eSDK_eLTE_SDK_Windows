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
filename    :    ProvisionMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    调度台配置管理类。负责群组和用户逻辑处理。对应eLTE SDK内的ProvisionManager类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_PROVISION_MGR_H__
#define __ELTE_OCX_PROVISION_MGR_H__

#include "DataType.h"


class ProvisionMgr
{
public:
	static ProvisionMgr& Instance();

private:
	ProvisionMgr(void);
	~ProvisionMgr(void);

public:
	// 配置管理接口初始化
	int Init(const std::string& strSrvIp, int iUserId);
	// 退出配置管理接口
	int Exit();
	// 获取调度台的群组列表
	int GetDcGroups(int iUserId, DcGroups* &pDcGroups) const;
	// 获取调度台的用户列表
	int GetDcUsers(int iUserId, DcUsers* &pDcUsers) const;
	// 获取一个群组的成员列表
	int GetGroupUsers(int iGroupId, GrpUserList* &pGrpUserList) const;
	// 获取一个群组的详细配置信息
	int GetGroupInfo(int iGroupId, GroupInfo* &pGroupInfo) const;
	// 获取一个用户的详细信息
	int GetUserInfo(int iUserId, UserInfo* &pUserInfo) const;
	// 获取DC详细信息
	int GetDcInfo(int iUserId, DcProperty* &pDcProperty) const;
	// 查询录音文件信息
	int GetUserRECFileInfoList(RECQueryInfo* pCondition, UserRecFileInfoList* &pInfoList);
	// 媒体服务器初始化
	int InitMRS(const std::string& strSrvIp);

private:
	ProvisionManager* m_pProvMgr;
};

#endif
