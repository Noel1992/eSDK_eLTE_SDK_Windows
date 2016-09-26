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
description :    ��ʱ��������
copyright   :    Copyright (c) 2015-2017 Huawei Tech.Co.,Ltd
history     :    2015/03/05 ��ʼ�汾
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

	//���ö�ʱ��
	ELTE_VOID SetTimer(ELTE_UINT32 tick);
	//��ȡ��ʱ������ʱ��
	ELTE_UINT32 GetDelayTime() const;
	//�趨��ʱ������ʱ��
	ELTE_VOID SetDelayTime(const ELTE_UINT32& delayTime);
	//�趨��ʱ������
	ELTE_VOID SetTimerType(const ELTE_UINT32& timerType);
	//��ʱ���ص�����
	static ELTE_VOID __stdcall TimeProc(
		UINT uID,      
		UINT uMsg,     
		DWORD_PTR dwUser,  
		DWORD dw1,     
		DWORD dw2      
		);
	//ֹͣ��ʱ��
	ELTE_INT32 StopTimer();
	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//��ȡ�¼�����
	HANDLE GetEventHandle() {return m_EventTimer;};
	//���ð�����
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);
	//��ȡ������
	const PACKET_DATA& GetPacketData() const {return m_packetData;};
private:
	//����ʱ������
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
