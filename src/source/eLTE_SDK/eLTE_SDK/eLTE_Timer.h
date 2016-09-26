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
filename    :    eLTE_Timer.h
author      :    zWX229156
created     :    2015/03/05
description :    定时器工作类
copyright   :    Copyright (c) 2015-2017 Huawei Tech.Co.,Ltd
history     :    2015/03/05 初始版本
*********************************************************************/

#ifndef __ELTE_TIMER_H__
#define __ELTE_TIMER_H__

#include "eLTE_Types.h"
#include <Windows.h>
#include "eLTE_Message.h"

class CUserMgr;//lint !e763
class CTimer
{
public:
	CTimer(void);
	virtual ~CTimer(void);

	//设置定时器
	ELTE_VOID SetTimer(ELTE_UINT32 tick);
	//获取定时器触发时间
	ELTE_UINT32 GetDelayTime() const;
	//设定定时器触发时间
	ELTE_VOID SetDelayTime(const ELTE_UINT32& delayTime);
	//设定定时器类型
	ELTE_VOID SetTimerType(const ELTE_UINT32& timerType);
	//定时器回调函数
	static ELTE_VOID __stdcall TimeProc(
		UINT uID,      
		UINT uMsg,     
		DWORD_PTR dwUser,  
		DWORD dw1,     
		DWORD dw2      
		);
	//停止定时器
	ELTE_INT32 StopTimer();
	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//获取事件对象
	HANDLE GetEventHandle() {return m_EventTimer;};
	//设置包数据
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);
	//获取包数据
	const PACKET_DATA& GetPacketData() const {return m_packetData;};
private:
	//处理定时器工作
	ELTE_INT32 DoProcess();
private:
	HANDLE       m_EventTimer;
	CUserMgr*    m_pUserMgr;
	ELTE_UINT32  m_uTimerID;
	ELTE_UINT32  m_uDelayTime;
	ELTE_UINT32  m_uTimerType;
	PACKET_DATA  m_packetData;
};
#endif
