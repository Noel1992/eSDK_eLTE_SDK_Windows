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
description		:	���嶨ʱ����
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/30 ��ʼ�汾
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

	//�����ṹ��ȷ������ģʽ����ʼ��
	struct object_creater 
	{
		object_creater()
		{
			(void)RMTimer::Instance();
		}
		//����������ȷ�����ṹ�岻���Ż���
		inline void do_nothing() const {}
	};
	static object_creater create_object_;

	/**************************************************************************
	* name			: SetTimer
	* description   : ��ȡTimerʵ��
	* input			: N/A
	* output		: RMTimer& :RMTimerʵ��
	* return		: RMTimer& :RMTimerʵ��
	* remark		: N/A
	**************************************************************************/
	static RMTimer* Instance();

	/**************************************************************************
	* name			: SetTimer
	* description   : ���ö�ʱ��ʱ��
	* input			: tick ��ʱʱ��(millisecond)
	* output		: N/A
	* return		: N/A
	* remark		: N/A
	**************************************************************************/
	void SetTimer(int iID, int tick);

	/**************************************************************************
	* name			: TimeProc
	* description   : ���ö�ʱ��ִ������
	* input			: uID ִ��ID
					  uMsg ��Ϣ
					  dwUser ִ���û�
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
	* description   : ֹͣ��ʱ��
	* input			: N/A
	* output		: N/A
	* return		: 0
	* remark		: N/A
	**************************************************************************/
	int StopTimer(int iID);

	/**************************************************************************
	* name			: getTaskNum
	* description   :��ȡ������
	* input			: N/A
	* output		: N/A
	* return		: iTaskNum ��ǰ������
	* remark		: N/A
	**************************************************************************/
	int getTaskNum(){return iTaskNum;}

	/**************************************************************************
	* name			: setTaskNum
	* description   :����������
	* input			: iVal	��������
	* output		: N/A
	* return		: void
	* remark		: N/A
	**************************************************************************/
	void setTaskNum(int iTaskVal){iTaskNum = iTaskVal;}

protected:
	/**************************************************************************
	* name			: DoProcess
	* description   :��ʱ��ִ�к���
	* input			: N/A
	* output		: N/A
	* return		: 0
	* remark		: N/A
	**************************************************************************/
	int DoProcess();

private:
	HANDLE       m_EventTimer;
	//��ǰִ�е�TimerID����
	static unsigned int  m_uTimerIDArray[TIMERNUM];
	//Timer �������
	static int iTaskNum;	
};

#endif