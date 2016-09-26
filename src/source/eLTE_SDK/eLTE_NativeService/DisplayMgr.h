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
filename    :    DisplayMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    eLTE SDK业务消息回调类。通过注册该类来接收调度台状态指示事件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_DISPLAY_MGR_H__
#define __ELTE_OCX_DISPLAY_MGR_H__


class DisplayMgr : public DisplayManager
{
public:
	DisplayMgr(void);
	virtual ~DisplayMgr(void); //lint !e1510

	//通知组呼的状态变更信息
	virtual void notifyGroupStatus(GrpDspInfo * grpInfo);

	//通知用户和群组资源的状态变更信息
	virtual void notifyResourceStatus(ResourceStatusIndicator * resourceStatus);

	//通知点对点呼叫的状态变更信息
	virtual void notifyP2pcallStatus(P2pcallStatusIndicator * callStatus);

	//通知视频呼叫的状态变更信息（包括视频回传），需要YC实现
	virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator * callStatus);

	//通知用户和群组资源的状态变更信息,需要YC实现
	virtual void notifyUserStatus(ResourceStatusIndicator * resourceStatus);

	//通知用户状态短消息的服务接收实现
	virtual void notifyUserSdsStatus(SdsMessageIndicator * sdsmsg);

	//模块组件状态事件
	virtual void notifyModuleStatus(DCModuleStatusIndicator * moduleStatus);

	//短数据信息上报给GUI
	virtual void notifySMS(SdsMessageIndicator * sdsmsg);

	//通知配置对象的变更信息， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	virtual void notifyProvisionChange(ProvChangeIndicator* pProvChange);

	//通知配置对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	virtual void notifyProvisionAllResync(ProvChangeIndicator* pProvChangeIndicator);

	//通知状态对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	virtual void notifyStatusAllResync(ResourceStatusIndicator* pResourceStatusIndicator);
};

#endif
