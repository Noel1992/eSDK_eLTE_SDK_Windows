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
filename		: 	eLTE_RM_Timer.h
author			:	sWX299049
created		    :	2015/10/30
description		:	定义定时器类
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/30 初始版本
*********************************************************************/
#ifndef _ELTE_RM_TIMER_H_
#define _ELTE_RM_TIMER_H_

#include <Windows.h>
#include "Mmsystem.h"

#pragma comment(lib, "Winmm.lib")

#define TIMERNUM 3
//max number of Timer task
#define  MAX_TASK_NUM 2
#define IDLE_TIMER 0
#define RECONNECT_TIMER 1
#define HEARTBEAT_TIMER 2
#define MAX_LOSECONCOUNT 5

#define RECONNECT_POLLING_TIME 5000

#define HEARTBEAT_TIME 10000

class RMTimer
{
public:
	//	RMTimer(void);
	//	virtual ~RMTimer(void);

	//辅助结构，确保单例模式被初始化
	struct object_creater 
	{
		object_creater()
		{
			(void)RMTimer::Instance();
		}
		//辅助函数，确保被结构体不被优化掉
		inline void do_nothing() const {}
	};
	static object_creater create_object_;

	/**************************************************************************
	* name			: SetTimer
	* description   : 获取Timer实例
	* input			: N/A
	* output		: RMTimer& :RMTimer实例
	* return		: RMTimer& :RMTimer实例
	* remark		: N/A
	**************************************************************************/
	static RMTimer* Instance();

	/**************************************************************************
	* name			: SetTimer
	* description   : 设置定时器时间
	* input			: tick 定时时间(millisecond)
	* output		: N/A
	* return		: N/A
	* remark		: N/A
	**************************************************************************/
	void SetTimer(int iID, int tick);

	/**************************************************************************
	* name			: TimeProc
	* description   : 设置定时器执行内容
	* input			: uID 执行ID
					  uMsg 信息
					  dwUser 执行用户
					  dw1
					  dw2
	* output		: N/A
	* return		: N/A
	* remark		: N/A
	**************************************************************************/
	static void __stdcall TimeProc(
		UINT uID,
		UINT uMsg,
		DWORD_PTR dwUser,
		DWORD dw1,
		DWORD dw2
		);

	/**************************************************************************
	* name			: StopTimer
	* description   : 停止计时器
	* input			: N/A
	* output		: N/A
	* return		: 0
	* remark		: N/A
	**************************************************************************/
	int StopTimer(int iID);

	/**************************************************************************
	* name			: getTaskNum
	* description   :获取任务编号
	* input			: N/A
	* output		: N/A
	* return		: iTaskNum 当前任务编号
	* remark		: N/A
	**************************************************************************/
	int getTaskNum(){return iTaskNum;}

	/**************************************************************************
	* name			: setTaskNum
	* description   :设置任务编号
	* input			: iVal	新任务编号
	* output		: N/A
	* return		: void
	* remark		: N/A
	**************************************************************************/
	void setTaskNum(int iTaskVal){iTaskNum = iTaskVal;}

protected:
	/**************************************************************************
	* name			: DoProcess
	* description   :定时器执行函数
	* input			: N/A
	* output		: N/A
	* return		: 0
	* remark		: N/A
	**************************************************************************/
	int DoProcess();

private:
	HANDLE       m_EventTimer;
	//当前执行的TimerID数组
	static unsigned int  m_uTimerIDArray[TIMERNUM];
	//Timer 任务计数
	static int iTaskNum;	
};

#endif